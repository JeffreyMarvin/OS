#pragma once
#include <stdint.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct interrupt_frame;
__attribute__((interrupt)) void page_fault_handler(interrupt_frame* frame);
__attribute__((interrupt)) void double_fault_handler(interrupt_frame* frame);
__attribute__((interrupt)) void gp_fault_handler(interrupt_frame* frame);
__attribute__((interrupt)) void invalid_op_handler(interrupt_frame* frame);
__attribute__((interrupt)) void keyboard_int_handler(interrupt_frame* frame);
__attribute__((interrupt)) void pit_int_handler(interrupt_frame* frame);

void SetIDTGate(void* handler, uint8_t entry_offset, uint8_t type_attr, uint8_t selector);
void init_idt();

void remap_pic();
void pic_end_master();
void pic_end_slave();