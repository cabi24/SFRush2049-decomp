SECTIONS { .text 0x80007E80 : { *(.text) } .data 0x8002C360 : { *(.data) } .sdk_initialize_bss 0x800367D0 (NOLOAD) : { *(.bss) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.gptab.*) } }
