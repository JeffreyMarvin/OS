#include "text_console.h"
#include "multiboot.h"
#include "gdt.h"
#include "page_frame_allocator.h"

Text_Console console;

extern uint64_t _kernel_start;
extern uint64_t _kernel_end;

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

struct grub_args {
    uint32_t multiboot_address;
    uint32_t magic;
} __attribute__((packed));

void kernel_init(uint32_t magic, multiboot_info_t* mbd) {

    GDT_descriptor gdt_descriptor;
    gdt_descriptor.Size = sizeof(GDT) - 1;
    gdt_descriptor.Offset = (uint64_t)&default_GDT;
    load_GDT(&gdt_descriptor);

    GlobalAllocator = Page_Frame_Allocator(mbd);

    if(mbd->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO) {  //Did the bootloader tell us about the framebuffer?
        if(mbd->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT){  //Is it the easy text type?
            console = Text_Console(mbd->framebuffer_addr, mbd->framebuffer_width, mbd->framebuffer_height, console.PRINT_COLOR_CYAN, console.PRINT_COLOR_BLACK);  //Set it up with params from the bootloader
        } else {
            return;  //Don't support other graphical modes.  Yet.
        }
    } else {
        console = Text_Console(0xB8000, 80, 25, console.PRINT_COLOR_CYAN, console.PRINT_COLOR_BLACK); //If no data, hope Text_Console works with default values
    }
}

extern "C" void kernel_main(grub_args grub_arguments) {

    uint32_t magic = grub_arguments.magic;
    multiboot_info_t* mbd = (multiboot_info_t*) ((uint64_t)(grub_arguments.multiboot_address));

    kernel_init(grub_arguments.magic, mbd);

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

    if(mbd->flags & MULTIBOOT_INFO_MEM_MAP) {
        print_mmap(mbd);
    }

    console.print_line();
    console.print_str("_kernel_start: "); console.print_hex((uint64_t)&_kernel_start); console.print_line();
    console.print_str("_kernel_end:   "); console.print_hex((uint64_t)&_kernel_end); console.print_line();

    console.print_line();
    console.print_str("Total memory:    "); console.print_num(GlobalAllocator.get_total_RAM() / 1024); console.print_line(" kB");
    console.print_str("Free memory:     "); console.print_num(GlobalAllocator.get_free_RAM() / 1024); console.print_line(" kB");
    console.print_str("Used memory:     "); console.print_num(GlobalAllocator.get_used_RAM() / 1024); console.print_line(" kB");
    console.print_str("Reserved memory: "); console.print_num(GlobalAllocator.get_reserved_RAM() / 1024); console.print_line(" kB");
    console.print_line();

    while(true) {
        //Kernel Main Loop
    }
}