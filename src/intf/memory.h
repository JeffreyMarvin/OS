#pragma once

#include "multiboot.h"
#include <stdint.h>
#include <stddef.h>

uint64_t getMemorySize(multiboot_info_t* mbd);