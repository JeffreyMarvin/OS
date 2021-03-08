#include "interrupt.h"
#include "idt.h"
#include "io.h"
#include "kernel_func.h"
#include "page_frame_allocator.h"

extern Page_Frame_Allocator GlobalAllocator;

IDTR idtr;
void SetIDTGate(void* handler, uint8_t entry_offset, uint8_t type_attr, uint8_t selector){
    IDTDescEntry* interrupt = (IDTDescEntry*)(idtr.offset + entry_offset * sizeof(IDTDescEntry));
    interrupt->SetOffset((uint64_t)handler);
    interrupt->type_attr = type_attr;
    interrupt->selector = selector;
}

void init_idt(){

    idtr.limit = 0x0FFF;
    idtr.offset = (uint64_t)GlobalAllocator.request_page();

    SetIDTGate((void*)page_fault_handler, 0xE, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)double_fault_handler, 0x8, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)gp_fault_handler, 0xD, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)keyboard_int_handler, 0x21, IDT_TA_InterruptGate, 0x08);
     
    asm ("lidt %0" : : "m" (idtr));

    remap_pic();

    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);

    asm ("sti");

    return;
}

__attribute__((interrupt)) void page_fault_handler(interrupt_frame* frame){
    kernel_panic("Page Fault Detected");
}

__attribute__((interrupt)) void double_fault_handler(interrupt_frame* frame){
    kernel_panic("Double Fault Detected");
}

__attribute__((interrupt)) void gp_fault_handler(interrupt_frame* frame){
    kernel_panic("General Protection Fault Detected");
}


__attribute__((interrupt)) void keyboard_int_handler(interrupt_frame* frame){
    //TODO: Handle Keyboard Input
}

void pic_end_master(){
    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_end_slave(){
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}
   

void remap_pic(){
    uint8_t a1, a2; 

    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);

}