#pragma once

#include <stdint.h>

struct GDT_descriptor {
    uint16_t Size;
    uint64_t Offset;
} __attribute__((packed));

struct GDT_entry {
    uint16_t Limit0;
    uint16_t Base0;
    uint8_t Base1;
    uint8_t AccessByte;
    uint8_t Limit1_Flags;
    uint8_t Base2;
}__attribute__((packed));

struct GDT {
    GDT_entry Null; //0x00
    GDT_entry KernelCode; //0x08
    GDT_entry KernelData; //0x10
    GDT_entry UserNull;
    GDT_entry UserCode;
    GDT_entry UserData;
} __attribute__((packed)) 
__attribute((aligned(0x1000)));

extern GDT default_GDT;

extern "C" void load_GDT(GDT_descriptor* gdt_descriptor);