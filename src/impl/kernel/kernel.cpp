#include "kernel.h"
#include "kernel_func.h"
#include "text_console.h"
#include "page_frame_allocator.h"
#include "rsdp.h"
#include "pit.h"

extern Page_Frame_Allocator GlobalAllocator;
extern Text_Console GlobalConsole;

extern "C" void kernel_start(grub_args grub_arguments) {

    uint32_t magic = grub_arguments.magic;
    multiboot_info_t* mbd = (multiboot_info_t*) ((uint64_t)(grub_arguments.multiboot_address));
    
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        kernel_panic("Multiboot booting error (magic number mismatch)\n");
    }

    kernel_init(mbd);

    kernel_main(mbd);
}

void kernel_main(multiboot_info_t* mbd) {

    GlobalConsole.clear_screen();
    GlobalConsole.print_line("Welcome to our 64-bit kernel!");
    GlobalConsole.print_line();

    print_mmap(mbd);

    GlobalConsole.print_line();
    GlobalConsole.print_str("Total memory:    "); GlobalConsole.print_num(GlobalAllocator.get_total_RAM() / 1024); GlobalConsole.print_line(" kB");
    GlobalConsole.print_str("Free memory:     "); GlobalConsole.print_num(GlobalAllocator.get_free_RAM() / 1024); GlobalConsole.print_line(" kB");
    GlobalConsole.print_str("Used memory:     "); GlobalConsole.print_num(GlobalAllocator.get_used_RAM() / 1024); GlobalConsole.print_line(" kB");
    GlobalConsole.print_str("Reserved memory: "); GlobalConsole.print_num(GlobalAllocator.get_reserved_RAM() / 1024); GlobalConsole.print_line(" kB");
    GlobalConsole.print_line();
    
    RSDP_Descriptor* rsdp = find_rspd(mbd);

    GlobalConsole.print_str("RSPD: "); GlobalConsole.print_hex((uint64_t)&rsdp); GlobalConsole.print_line();
    GlobalConsole.print_str("Checksum:"); GlobalConsole.print_hex(rsdp->Checksum); GlobalConsole.print_line();
    GlobalConsole.print_str("Rev:"); GlobalConsole.print_hex(rsdp->Revision); GlobalConsole.print_line();
    GlobalConsole.print_str("RSDT Addr:"); GlobalConsole.print_hex(rsdp->RsdtAddress); GlobalConsole.print_line();

    GlobalConsole.print_line("Starting Main Loop");
    while(true) {
        draw_time(80,25);
        PIT::sleep(100);
    }
}