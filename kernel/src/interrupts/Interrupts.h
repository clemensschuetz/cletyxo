#pragma once
#include "../BasicRenderer.h"

namespace Visionizer
{

    // READ: https://wiki.osdev.org/Exceptions
    
    struct interrupt_frame;
    __attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame* frame);
    // Occurs when there are 2 unhandled faults in a row
    __attribute__((interrupt)) void DoubleFault_Handler(struct interrupt_frame* frame);
    // There are many things that could happen, the following are common: A Segment Error, 
    __attribute__((interrupt)) void GPFault_Handler(struct interrupt_frame* frame);
}
