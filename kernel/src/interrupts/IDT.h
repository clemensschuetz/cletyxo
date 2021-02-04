#pragma once
#include <stdint.h>

namespace Visionizer
{
    // MACROS

    // IDT Type & Attributes Interrupt Gate (Byte)
    #define IDT_TA_InterruptGate 0b10001110
    // IDT Type & Attributes Call Gate (Byte)
    #define IDT_TA_CallGate      0b10001100
    // IDT Type & Attributes Trap Gate (Byte)
    #define IDT_TA_TrapGate      0b10001111



    // STRUCTS

    struct IDTDescEntry {
        uint16_t offset0; 
        uint16_t selector;  
        uint8_t ist; // Interrupt Stack Table
        uint8_t type_attr; // Types and Attributes
        uint16_t offset1;
        uint32_t offset2;
        uint32_t ignore; // Ignore me, just for completion
        void SetOffset(uint64_t offset); // Sets the offset
        uint64_t GetOffset(); // Gets the offset
    };

    struct IDTR {
        uint16_t Limit; // The limit | Full size of an IDTR: 0x0FFF
        uint64_t Offset;
    } __attribute__((packed));


}