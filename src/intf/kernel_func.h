#pragma once

#include "multiboot.h"

void print_mmap(multiboot_info_t* mbd);
void kernel_init(multiboot_info_t* mbd);
void kernel_panic(const char* message);
void init_gdt();
void init_page_frame_allocator(multiboot_info_t* mbd);
void init_page_table_manager();
void init_global_console(multiboot_info_t* mbd);
void init_idt();
