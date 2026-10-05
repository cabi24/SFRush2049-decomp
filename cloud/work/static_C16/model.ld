SECTIONS { .text 0x8000C050 : { *(.text) } .data 0x8002C3F0 : { *(.data) } .timer_bss 0x80037C30 (NOLOAD) : { *(.bss) } /DISCARD/ : { *(.reginfo) *(.options) *(.mdebug) *(.gptab.*) } }
