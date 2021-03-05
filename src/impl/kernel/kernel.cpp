#include "text_console.h"
#include "multiboot.h"

Text_Console console;

void print_multiboot_overview(multiboot_info_t* mbd){
    console.print_line("Multiboot Information Structure");
    console.print_str("Address: "); console.print_hex((uint64_t)mbd); console.print_line();
    console.print_str("Flags: "); console.print_hex(mbd->flags); console.print_line();

    if(mbd->flags & MULTIBOOT_INFO_MEMORY){
        console.print_str("Low Mem: "); console.print_hex(mbd->mem_lower); console.print_str(" High Mem: "); console.print_hex(mbd->mem_upper); console.print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_BOOTDEV){
        console.print_str("Boot Device: "); console.print_hex(mbd->boot_device); console.print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_CMDLINE){
        console.print_str("Cmd Line: "); console.print_hex(mbd->cmdline); console.print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_MODS){
        console.print_str("Mods Count: "); console.print_hex(mbd->mods_count); console.print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_AOUT_SYMS){
        console.print_str("Symbol Table: "); console.print_hex(mbd->u.aout_sym.addr); console.print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_ELF_SHDR){
        console.print_str("Section Header: "); console.print_hex(mbd->u.elf_sec.addr); console.print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_MEM_MAP){
        console.print_str("Mem Map: "); console.print_hex(mbd->mmap_addr); console.print_str(" Lenght: "); console.print_hex(mbd->mmap_length); console.print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_DRIVE_INFO){
        console.print_str("Drives: "); console.print_hex(mbd->drives_addr); console.print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_CONFIG_TABLE){
        console.print_str("Config Table: "); console.print_hex(mbd->config_table); console.print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME){
        console.print_str("Boot Loader: "); console.print_num(mbd->boot_loader_name); console.print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_APM_TABLE){
        console.print_str("APM Table: "); console.print_hex(mbd->apm_table); console.print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_VBE_INFO){
        console.print_str("VBE Mode: "); console.print_num(mbd->vbe_mode); console.print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO){
        console.print_str("Framebuffer: "); console.print_num(mbd->framebuffer_type); console.print_line();
    }
}

void print_mmap(multiboot_info_t* mbd){
    if(!mbd->flags & 0x0040){
        return;
    }
    console.print_line();
    console.print_line("Memory Map:");
    console.print_line(" Address            | Length             | Type");
    console.print_line("--------------------+--------------------+-------");
    uint64_t mmap_addr = mbd->mmap_addr, mmap_length = mbd->mmap_length;
    uint64_t i = 0;
    while(i < mmap_length) {
        multiboot_memory_map_t* entry = (multiboot_memory_map_t*) (mmap_addr + i);
        console.print_str(" ");
        console.print_hex((((uint64_t)entry->base_addr_high) << 32) + entry->base_addr_low); console.print_str(" | ");
        console.print_hex((((uint64_t)entry->length_high) << 32) + entry->length_low); console.print_str(" | ");
        switch (entry->type)
        {
        case MULTIBOOT_MEMORY_AVAILABLE:
            console.print_line("Available");
            break;
        case MULTIBOOT_MEMORY_RESERVED:
            console.print_line("Reserved");
            break;
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
            console.print_line("ACPI Reclaimable");
            break;
        case MULTIBOOT_MEMORY_NVS:
            console.print_line("NVS");
            break;
        case MULTIBOOT_MEMORY_BADRAM:
            console.print_line("Bad RAM");
            break;
        default:
            console.print_line("Unknown");
            break;
        }
        i += entry->size + 4;
    }
}

extern "C" void kernel_main(uint64_t grub_arguments) {

    uint32_t magic = grub_arguments >> 32;
    multiboot_info_t* mbd = (struct multiboot_info*) ((uint64_t)((uint32_t) grub_arguments));

    if(mbd->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO) {  //Did the bootloader tell us about the framebuffer?
        if(mbd->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT){  //Is it the easy text type?
            console = Text_Console(mbd->framebuffer_addr, mbd->framebuffer_width, mbd->framebuffer_height, console.PRINT_COLOR_CYAN, console.PRINT_COLOR_BLACK);  //Set it up with params from the bootloader
        } else {
            return;  //Don't support other graphical modes.  Yet.
        }
    } else {
        console = Text_Console(0xB8000, 80, 25, console.PRINT_COLOR_CYAN, console.PRINT_COLOR_BLACK); //If no data, hope Text_Console works with default values
    }

    console.clear_screen();
    console.print_line("Welcome to our 64-bit kernel!");
    console.print_line();

    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        console.print_line("ERROR: Multiboot booting error (mismatch magic number)");
        console.print_str("       Expected: "); console.print_hex((uint32_t)MULTIBOOT_BOOTLOADER_MAGIC);
        console.print_str(" Found: "); console.print_hex(magic);
        console.print_line();
       return;
    }

    print_multiboot_overview(mbd);
    if(mbd->flags & MULTIBOOT_INFO_MEM_MAP) {
        print_mmap(mbd);
    }
}