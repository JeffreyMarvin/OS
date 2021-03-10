#pragma once

#include <stdint.h>
#include "multiboot.h"

struct RSDP_Descriptor {
 char Signature[8];
 uint8_t Checksum;
 char OEMID[6];
 uint8_t Revision;
 uint32_t RsdtAddress;
} __attribute__ ((packed));

struct RSDP_Descriptor20 {
 RSDP_Descriptor firstPart;
 
 uint32_t Length;
 uint64_t XsdtAddress;
 uint8_t ExtendedChecksum;
 uint8_t reserved[3];
} __attribute__ ((packed));

RSDP_Descriptor* find_rspd(multiboot_info_t* mbd);
