#include "print.h"
#include "multiboot.h"

void kernel_main(uint64_t grub_arguments) {

    uint32_t magic = grub_arguments >> 32;
    multiboot_info_t* mbd = (struct multiboot_info*) ((uint32_t) grub_arguments);

    print_clear();
    print_set_color(PRINT_COLOR_CYAN, PRINT_COLOR_BLACK);
    print_line("Welcome to our 64-bit kernel!");
    print_line("");
    print_line("Testing print_byte(): ");
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j++){
            print_byte(i * 16 + j);
            print_char(' ');
        }
        print_line("");
    }
    print_line("");

    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        print_line("ERROR: Multiboot booting error (mismatch magic number)");
        print_str("       Expected: ");
        print_double_word(MULTIBOOT_BOOTLOADER_MAGIC);
        print_str(" Found: ");
        print_double_word(magic);
        print_line("");
       return;
   }

    if(mbd->flags >> 6 & 0x1) { 
        print_line("Multiboot MMAP Valid");
        print_str("Flags: "); print_word(mbd->flags); print_line("");
        print_str("Low Mem: "); print_double_word(mbd->mem_lower); print_line("");
        print_str("High Mem: "); print_double_word(mbd->mem_upper); print_line("");
        multiboot_memory_map_t* mmap = (multiboot_memory_map_t*) mbd->mmap_addr;
        // for(int i = 0; i < mbd->mmap_length; i += mmap->size + sizeof(mmap->size)) {
        //     print_str("Base: "); print_quad_word((mmap+i)->addr); print_str("Length: "); print_quad_word((mmap+i)->len); print_str("Type: "); print_double_word((mmap+i)->type); print_line("");
        // }
    } else {
        print_line("Multiboot MMAP Not Valid");
        print_str("Flags: "); print_word(mbd->flags); print_line("");
        print_str("Low Mem: "); print_double_word(mbd->mem_lower); print_line("");
        print_str("High Mem: "); print_double_word(mbd->mem_upper); print_line("");
    }
}
