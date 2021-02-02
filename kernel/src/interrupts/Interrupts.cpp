#include "Interrupts.h" 
#include "../Panic.h"

namespace Visionizer
{
    // READ: https://wiki.osdev.org/Exceptions
    
    __attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame* frame){
        Panic("Page Fault Detected", "This error occurs because of the following reasons: A page directory / table entry is not present in physical memory, something isnt loading right, a protection check failed or if a reserved bit is set to 1."); // TODO Add Explanation
        int* test = (int*)0x80000000000;
		*test = 2;
        while(true);
    }

    
    __attribute__((interrupt)) void DoubleFault_Handler(struct interrupt_frame* frame){
        Panic("Double Fault Detected", "This error occurs whenever there are two unhandled fatal faults in a row"); // TODO Add Explanation
        while(true);
    }

    
    __attribute__((interrupt)) void GPFault_Handler(struct interrupt_frame* frame){
        Panic("General Fault Detected", "This error can occur because of several reasons including a segment error, an unprivileged-execution error, if you write a 1 in a reserved register field or writing invalid value combinations or if you are referencing / accessing a null-descriptor."); // TODO Add Explanation
        while(true);
    }

}