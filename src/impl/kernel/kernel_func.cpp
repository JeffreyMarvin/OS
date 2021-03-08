#include "kernel_func.h"
#include "page_frame_allocator.h"
#include "text_console.h"
#include "gdt.h"
#include "interrupt.h"

extern uint64_t _kernel_start;
extern uint64_t _kernel_end;
extern Page_Frame_Allocator GlobalAllocator;
extern Text_Console GlobalConsole;

void kernel_init(multiboot_info_t* mbd) {

    init_gdt();

    init_page_frame_allocator(mbd);

    init_global_console(mbd);

    init_idt();

    return;
}

void init_gdt(){
    GDT_descriptor gdt_descriptor;
    gdt_descriptor.Size = sizeof(GDT) - 1;
    gdt_descriptor.Offset = (uint64_t)&default_GDT;
    load_GDT(&gdt_descriptor);
}

void init_page_frame_allocator(multiboot_info_t* mbd){
    GlobalAllocator = Page_Frame_Allocator(mbd);
}

void init_global_console(multiboot_info_t* mbd){
    if(mbd->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO) {  //Did the bootloader tell us about the framebuffer?
        if(mbd->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT){  //Is it the easy text type?
            GlobalConsole = Text_Console(mbd->framebuffer_addr, mbd->framebuffer_width, mbd->framebuffer_height, Text_Console::PRINT_COLOR_CYAN, Text_Console::PRINT_COLOR_BLACK);  //Set it up with params from the bootloader
        } else {
            return;  //Don't support other graphical modes.  Yet.
        }
    } else {
        GlobalConsole = Text_Console(0xB8000, 80, 25, Text_Console::PRINT_COLOR_CYAN, Text_Console::PRINT_COLOR_BLACK); //If no data, hope Text_Console works with default values
    }
}



void kernel_panic(const char* message){
    GlobalConsole.set_color(Text_Console::PRINT_COLOR_BLACK, Text_Console::PRINT_COLOR_CYAN);
    GlobalConsole.clear_screen();
    GlobalConsole.set_position(20, 10);
    GlobalConsole.print_line("KERNEL PANIC");
    GlobalConsole.print_line(message);
    while(true);//hang
}

void print_mmap(multiboot_info_t* mbd){
    if(!mbd->flags & 0x0040){
        return;
    }
    GlobalConsole.print_line();
    GlobalConsole.print_line("Memory Map:");
    GlobalConsole.print_line(" Address            | Length             | Type");
    GlobalConsole.print_line("--------------------+--------------------+-------");
    uint64_t mmap_addr = mbd->mmap_addr, mmap_length = mbd->mmap_length;
    uint64_t i = 0;
    while(i < mmap_length) {
        multiboot_memory_map_t* entry = (multiboot_memory_map_t*) (mmap_addr + i);
        GlobalConsole.print_str(" ");
        GlobalConsole.print_hex((((uint64_t)entry->base_addr_high) << 32) + entry->base_addr_low); GlobalConsole.print_str(" | ");
        GlobalConsole.print_num(((((uint64_t)entry->length_high) << 32) + entry->length_low) / 1024); GlobalConsole.print_str(" kB "); GlobalConsole.set_x_position(41); GlobalConsole.print_str("| ");
        switch (entry->type)
        {
        case MULTIBOOT_MEMORY_AVAILABLE:
            GlobalConsole.print_line("Available");
            break;
        case MULTIBOOT_MEMORY_RESERVED:
            GlobalConsole.print_line("Reserved");
            break;
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
            GlobalConsole.print_line("ACPI Reclaimable");
            break;
        case MULTIBOOT_MEMORY_NVS:
            GlobalConsole.print_line("NVS");
            break;
        case MULTIBOOT_MEMORY_BADRAM:
            GlobalConsole.print_line("Bad RAM");
            break;
        default:
            GlobalConsole.print_line("Unknown");
            break;
        }
        i += entry->size + 4;
    }
}