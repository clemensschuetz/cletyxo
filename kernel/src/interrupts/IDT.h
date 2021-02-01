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

    // An entry in the IDT Descriptor Table
    struct IDTDescEntry
    {
        uint16_t offset0;
        uint16_t selector;
        uint8_t ist; // Interrupt Stack Table
        uint8_t type_attr; // Type and Attributes
        uint16_t offset1;
        uint32_t offset2;
        uint32_t ignore; // IGnore me, I just exist for completion
        void SetOffset(uint64_t offset);
        uint64_t GetOffset();
    };

    // The IDT Descriptor Table
    struct IDTR
    {
        uint16_t limit; // The limit | Size of a full IDTR: 0x0FFF
        uint16_t offset;
    }__attribute__((packed));


}