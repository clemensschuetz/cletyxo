#pragma once
#include <stdint.h>

namespace Visionizer
{
    // ALL IO FUNCTIONS SHOULD BE FOLLOWED BY AN IOWAIT() IN ORDER FOR OLDER DEVICES TO CATCH UP


    // Putting a byte onto the bus | Use IOWait() afterwards to let older devices catch up.
    void outb(uint16_t port, uint8_t value);
    // Reads and returns a value from a port | Use IOWait() afterwards to let older devices catch up.
    uint8_t inb(uint16_t port);

    // This will send a little bit to an unused port so we waste one IO Cycle | Sends 0 to 0x80 | Used so older devices can catch up with the iobus
    void IOWait();

}