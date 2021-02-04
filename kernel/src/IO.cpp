#include "IO.h"






namespace Visionizer
{
    // ALL IO FUNCTIONS SHOULD BE FOLLOWED BY AN IOWAIT() IN ORDER FOR OLDER DEVICES TO CATCH UP

    // Putting a byte onto the bus
    void outb(uint16_t port, uint8_t value)
    {
        asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
    }
    // Reads a value
    uint8_t inb(uint16_t port)
    {
        uint8_t returnVal;
        asm volatile ("inb %1, %0"
        : "=a"(returnVal)
        : "Nd"(port));
        return returnVal;
    }
    

    // Waits for a little time
    void IOWait()
    {
        // This will send a little bit to an unused port so we waste one IO Cycle | Sends 0 to 0x80
        asm volatile("outb %%al, $0x80" : : "a"(0));
    }
}
