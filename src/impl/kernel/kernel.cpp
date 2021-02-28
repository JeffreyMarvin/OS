#include "print.h"
#include "multiboot.h"

void print_multiboot_overview(multiboot_info_t* mbd){
    print_line("Multiboot Information Structure");
    print_str("Address: "); print_hex((uint64_t)mbd); print_line();
    print_str("Flags: "); print_hex(mbd->flags); print_line();

    if(mbd->flags & MULTIBOOT_INFO_MEMORY){
        print_str("Low Mem: "); print_hex(mbd->mem_lower); print_str(" High Mem: "); print_hex(mbd->mem_upper); print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_BOOTDEV){
        print_str("Boot Device: "); print_hex(mbd->boot_device); print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_CMDLINE){
        print_str("Cmd Line: "); print_hex(mbd->cmdline); print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_MODS){
        print_str("Mods Count: "); print_hex(mbd->mods_count); print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_AOUT_SYMS){
        print_str("Symbol Table: "); print_hex(mbd->u.aout_sym.addr); print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_ELF_SHDR){
        print_str("Section Header: "); print_hex(mbd->u.elf_sec.addr); print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_MEM_MAP){
        print_str("Mem Map: "); print_hex(mbd->mmap_addr); print_str(" Lenght: "); print_hex(mbd->mmap_length); print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_DRIVE_INFO){
        print_str("Drives: "); print_hex(mbd->drives_addr); print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_CONFIG_TABLE){
        print_str("Config Table: "); print_hex(mbd->config_table); print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME){
        print_str("Boot Loader: "); print_num(mbd->boot_loader_name); print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_APM_TABLE){
        print_str("APM Table: "); print_hex(mbd->apm_table); print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_VBE_INFO){
        print_str("VBE Mode: "); print_num(mbd->vbe_mode); print_line();
    }
    if(mbd->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO){
        print_str("Framebuffer: "); print_num(mbd->framebuffer_type); print_line();
    }
}

void print_mmap(multiboot_info_t* mbd){
    if(!mbd->flags & 0x0040){
        return;
    }
    print_line();
    print_line("Memory Map:");
    print_line(" Address            | Length             | Type");
    print_line("--------------------+--------------------+-------");
    uint64_t mmap_addr = mbd->mmap_addr, mmap_length = mbd->mmap_length;
    uint64_t i = 0;
    while(i < mmap_length) {
        multiboot_memory_map_t* entry = (multiboot_memory_map_t*) (mmap_addr + i);
        print_str(" ");
        print_hex((((uint64_t)entry->base_addr_high) << 32) + entry->base_addr_low); print_str(" | ");
        print_hex((((uint64_t)entry->length_high) << 32) + entry->length_low); print_str(" | ");
        switch (entry->type)
        {
        case MULTIBOOT_MEMORY_AVAILABLE:
            print_line("Available");
            break;
        case MULTIBOOT_MEMORY_RESERVED:
            print_line("Reserved");
            break;
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
            print_line("ACPI Reclaimable");
            break;
        case MULTIBOOT_MEMORY_NVS:
            print_line("NVS");
            break;
        case MULTIBOOT_MEMORY_BADRAM:
            print_line("Bad RAM");
            break;
        default:
            print_line("Unknown");
            break;
        }
        i += entry->size + 4;
    }
}

extern "C" void kernel_main(uint64_t grub_arguments) {

    uint32_t magic = grub_arguments >> 32;
    multiboot_info_t* mbd = (struct multiboot_info*) ((uint64_t)((uint32_t) grub_arguments));

    print_clear();
    print_set_color(PRINT_COLOR_CYAN, PRINT_COLOR_BLACK);
    print_line("Welcome to our 64-bit kernel!");
    print_line();

    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        print_line("ERROR: Multiboot booting error (mismatch magic number)");
        print_str("       Expected: "); print_hex((uint32_t)MULTIBOOT_BOOTLOADER_MAGIC);
        print_str(" Found: "); print_hex(magic);
        print_line();
       return;
    }

    print_multiboot_overview(mbd);
    if(mbd->flags & 0x0040) {
        print_mmap(mbd);
    }
}