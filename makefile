CC = x86_64-elf-gcc
ASM = nasm
LD = x86_64-elf-ld

CFLAGS = -ffreestanding -fshort-wchar -mno-red-zone -mfpmath=387 -mno-sse2
INT_CFLAGS = -mgeneral-regs-only
ASMFLAGS = -f elf64
LDFLAGS = -n -static -Bsymbolic -nostdlib

kernel_c_source_files := $(shell find src/impl/kernel -name *.c)
kernel_c_object_files := $(patsubst src/impl/kernel/%.c, build/kernel/%.o, $(kernel_c_source_files))
kernel_cpp_source_files := $(shell find src/impl/kernel -name *.cpp)
kernel_cpp_object_files := $(patsubst src/impl/kernel/%.cpp, build/kernel/%.o, $(kernel_cpp_source_files))

x86_64_c_source_files := $(shell find src/impl/x86_64 -name *.c)
x86_64_c_object_files := $(patsubst src/impl/x86_64/%.c, build/x86_64/%.o, $(x86_64_c_source_files))
x86_64_cpp_source_files := $(shell find src/impl/x86_64 -name *.cpp)
x86_64_cpp_object_files := $(patsubst src/impl/x86_64/%.cpp, build/x86_64/%.o, $(x86_64_cpp_source_files))

x86_64_asm_source_files := $(shell find src/impl/x86_64 -name *.asm)
x86_64_asm_object_files := $(patsubst src/impl/x86_64/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

kernel_object_files := $(kernel_c_object_files) $(kernel_cpp_object_files)
x86_64_object_files := $(x86_64_c_object_files) $(x86_64_cpp_object_files) $(x86_64_asm_object_files)

$(kernel_c_object_files): build/kernel/%.o : src/impl/kernel/%.c
	mkdir -p $(dir $@) && \
	$(CC) -c -I src/intf $(CFLAGS) $(patsubst build/kernel/%.o, src/impl/kernel/%.c, $@) -o $@
$(kernel_cpp_object_files): build/kernel/%.o : src/impl/kernel/%.cpp
	mkdir -p $(dir $@) && \
	$(CC) -c -I src/intf $(CFLAGS) $(patsubst build/kernel/%.o, src/impl/kernel/%.cpp, $@) -o $@

$(x86_64_c_object_files): build/x86_64/%.o : src/impl/x86_64/%.c
	mkdir -p $(dir $@) && \
	$(CC) -c -I src/intf $(CFLAGS) $(patsubst build/x86_64/%.o, src/impl/x86_64/%.c, $@) -o $@
$(x86_64_cpp_object_files): build/x86_64/%.o : src/impl/x86_64/%.cpp
	@if [ "$(findstring interrupt,$@)" = "interrupt" ]; then \
		mkdir -p $(dir $@) && \
		$(CC) -c -I src/intf $(CFLAGS) $(INT_CFLAGS) $(patsubst build/x86_64/%.o, src/impl/x86_64/%.cpp, $@) -o $@ ; \
	else \
		mkdir -p $(dir $@) && \
		$(CC) -c -I src/intf $(CFLAGS) $(patsubst build/x86_64/%.o, src/impl/x86_64/%.cpp, $@) -o $@ ; \
	fi;

$(x86_64_asm_object_files): build/x86_64/%.o : src/impl/x86_64/%.asm
	mkdir -p $(dir $@) && \
	$(ASM) $(ASMFLAGS) $(patsubst build/x86_64/%.o, src/impl/x86_64/%.asm, $@) -o $@

.PHONY: build-x86_64
build-x86_64: $(kernel_object_files) $(x86_64_object_files)
	mkdir -p dist/x86_64 && \
	$(LD) $(LDFLAGS) -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld $(kernel_object_files) $(x86_64_object_files) && \
	cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso