#include "Interrupts.h" 
#include "../Panic.h"
#include "../IO.h"



namespace Visionizer
{
    // READ: https://wiki.osdev.org/Exceptions
    
    __attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame)
    {
        Panic("Page Fault Detected", "This error occurs because of the following reasons: A page directory / table entry is not present in physical memory, something isnt loading right, a protection check failed or if a reserved bit is set to 1."); // TODO Add Explanation
        while(true);
    }

    
    __attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame)
    {
        Panic("Double Fault Detected", "This error occurs whenever there are two unhandled fatal faults in a row"); // TODO Add Explanation
        while(true);
    }

    
    __attribute__((interrupt)) void GPFault_Handler(interrupt_frame* frame)
    {
        Panic("General Protection Fault Detected", "This error can occur because of several reasons including a segment error, an unprivileged-execution error, if you write a 1 in a reserved register field or writing invalid value combinations or if you are referencing / accessing a null-descriptor."); // TODO Add Explanation
        while(true);
    }

    // PIC


    // Interrupts for Pic-related devices

    // Adds interrupt for pic keyboard
    __attribute__((interrupt)) void KeyboardInt_Handler(interrupt_frame* frame)
    {
        uint8_t scancode = inb(0x60); // Keycode | 0x60 is the port of the PIC keyboard
        
        HandleKeyboard(scancode);


        PICEndParent();
    }

    __attribute__((interrupt)) void MouseInt_Handler(interrupt_frame* frame){

        uint8_t mouseData = inb(0x60);

        HandlePS2Mouse(mouseData);

        PICEndChild();
    }





    // PIC-Related utilities


    void PICEndParent()
    {
        outb(PIC1_COMMAND, PIC_EOI);
    }

    void PICEndChild()
    {
        outb(PIC2_COMMAND, PIC_EOI);
        outb(PIC1_COMMAND, PIC_EOI);
    }
    
    // Remaps the PIC Chips
    void RemapPIC()
    {
        uint8_t a1, a2;

        // Getting the bitmasks for later restorage
        a1 = inb(PIC1_DATA);
        IOWait();
        a2 = inb(PIC2_DATA);
        IOWait();

        // Initializing the PIC-Chips
        outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
        IOWait();
        outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
        IOWait();

        // Other things | The ports are just preference
        outb(PIC1_DATA, 0x20);
        IOWait();
        outb(PIC2_DATA, 0x28);
        IOWait();

        // Tell the 2 chips that they belong to each other
        outb(PIC1_DATA, 4);
        IOWait();
        outb(PIC1_DATA, 2);
        IOWait();

        // Tell the chips that they have to work in 8086
        outb(PIC1_DATA, ICW4_8086);
        IOWait();
        outb(PIC2_DATA, ICW4_8086);
        IOWait();


        // Restoring the old bitmasks (a1, a2)
        outb(PIC1_DATA, a1);
        IOWait();
        outb(PIC2_DATA, a2);
        
    }



}