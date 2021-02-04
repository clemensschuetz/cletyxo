#pragma once
#include "../BasicRenderer.h"
#include "../userinput/Keyboard.h"
#include "../userinput/Mouse.h"

namespace Visionizer
{

    #define PIC1_COMMAND 0x20 // PIC1 is the parent
    #define PIC1_DATA 0x21

    #define PIC2_COMMAND 0xA0 // PIC2 is the child
    #define PIC2_DATA 0xA1

    #define PIC_EOI 0x20      // PIC_EndOfInterrupt

    #define ICW1_INIT 0x10
    #define ICW1_ICW4 0x01
    #define ICW4_8086 0x01

    // READ: https://wiki.osdev.org/Exceptions

    struct interrupt_frame;
    __attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame);
    // Occurs when there are 2 unhandled faults in a row
    __attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame);
    // There are many things that could happen, the following are common: A Segment Error, 
    __attribute__((interrupt)) void GPFault_Handler(interrupt_frame* frame);

    // Adds the keyboard functionality
    __attribute__((interrupt)) void KeyboardInt_Handler(interrupt_frame* frame);
    // Adds the Mouse functionality
    __attribute__((interrupt)) void MouseInt_Handler(interrupt_frame* frame);

    // PIC-THINGS

    // Remaps the PIC-Chips (PIC1 - Parent, PIC2 - Child)
    void RemapPIC();

    // Clears the buffer of the parent pic | This enables making more than one keyboard press during one session possible
    void PICEndParent();

    // Clears the buffer of the child pic | This enables making more than one keyboard press during one session possible
    void PICEndChild();



}
