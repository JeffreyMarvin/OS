#pragma once
#include <stdint.h>
#include "multiboot.h"

struct grub_args {
    uint32_t multiboot_address;
    uint32_t magic;
} __attribute__((packed));

extern "C" void kernel_start(grub_args grub_arguments);

void kernel_main(multiboot_info_t* mbd);