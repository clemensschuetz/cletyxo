OUTPUT_FORMAT(elf64-x86-64)
ENTRY(kmain)

SECTIONS
{
    /* Then let's place all the other traditional executable sections afterwards. */
    .text : ALIGN(0x1000) {
        *(.text*)
    }

    .rodata : ALIGN(0x1000) {
        *(.rodata*)
    }

    .data : ALIGN(0x1000) {
        *(.data*)
    }

    .bss : ALIGN(0x1000) {
        *(COMMON)
        *(.bss*)
    }
}