#pragma once
#include <stdint.h>
#include "../IO.h"
#include "../Math.h"
#include "../BasicRenderer.h"

namespace Visionizer
{
    // Mouse Buttons
    #define PS2LeftButton 0b000000001
    #define PS2MiddleButton 0b000000010
    #define PS2RightButton 0b00000100

    // PS2-Bits
    #define PS2XSign 0b00010000
    #define PS2YSign 0b00110000
    #define PS2XOverflow 0b0010000
    #define PS2YOverflow 0b10000000


    // Initializes a PS2 MOuse
    void InitPS2Mouse();
    // Fills the packets with data
    void HandlePS2Mouse(uint8_t data);
    // Processes The packets from HandlePS2Mouse()
    void ProcessMousePacket();


    // The Mouse Position
    extern Point MousePosition;

}
