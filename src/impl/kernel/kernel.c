#include "print.h"
#include "multiboot.h"

void kernel_main(uint64_t grub_arguments) {

    uint32_t magic = grub_arguments >> 32;
    multiboot_info_t* mbd = (struct multiboot_info*) ((uint32_t) grub_arguments);

    print_clear();
    print_set_color(PRINT_COLOR_CYAN, PRINT_COLOR_BLACK);
    print_line("Welcome to our 64-bit kernel!");
    print_line("");
    // print_line("Testing print_byte(): ");
    // for(int i = 0; i < 16; i++) {
    //     for(int j = 0; j < 16; j++){
    //         print_byte(i * 16 + j);
    //         print_char(' ');
    //     }
    //     print_line("");
    // }
    // print_line("");

    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        print_line("ERROR: Multiboot booting error (mismatch magic number)");
        print_str("       Expected: "); print_double_word(MULTIBOOT_BOOTLOADER_MAGIC);
        print_str(" Found: "); print_double_word(magic);
        print_line("");
       return;
   }

    print_line("Multiboot Information Structure");
    print_str("Flags: "); print_double_word(mbd->flags); print_line("");
    if(mbd->flags & 0x0001){
        print_str("Low Mem: "); print_double_word(mbd->mem_lower); print_str(" High Mem: "); print_double_word(mbd->mem_upper); print_line("");
    }
    if(mbd->flags & 0x0002){
        print_str("Boot Device: "); print_double_word(mbd->boot_device); print_line("");
    }
    if(mbd->flags & 0x0004){
        print_str("Cmd Line: "); print_double_word(mbd->cmdline); print_line("");
    }
    if(mbd->flags & 0x0008){
        print_str("Mods Count: "); print_double_word(mbd->mods_count); print_line("");
    }
    if(mbd->flags & 0x0010){
        print_str("Symbol Table: "); print_double_word(mbd->u.aout_sym.addr); print_line("");
    }
    if(mbd->flags & 0x0020){
        print_str("Section Header: "); print_double_word(mbd->u.elf_sec.addr); print_line("");
    }
    if(mbd->flags & 0x0040){
        print_str("Mem Map: "); print_double_word(mbd->mmap_addr); print_line("");
    }
    if(mbd->flags & 0x0080){
        print_str("Drives: "); print_double_word(mbd->drives_addr); print_line("");
    }
    if(mbd->flags & 0x0100){
        print_str("Config Table: "); print_double_word(mbd->config_table); print_line("");
    }
    if(mbd->flags & 0x0200){
        print_str("Boot Loader: "); print_double_word(mbd->boot_loader_name); print_line("");
    }
    if(mbd->flags & 0x0400){
        print_str("APM Table: "); print_double_word(mbd->apm_table); print_line("");
    }
    if(mbd->flags & 0x0800){
        print_str("VBE Mode: "); print_double_word(mbd->vbe_mode); print_line("");
    }
    if(mbd->flags & 0x1000){
        print_str("Framebuffer: "); print_double_word(mbd->framebuffer_addr); print_line("");
    }
}
