# Source: game_code.bin (decompressed)
# Address: 0x8008D6B0

glabel math_utility
    /* 8008D6B0 C4840000 */  lwc1 $f4, 0($a0)
    /* 8008D6B4 E4A40000 */  swc1 $f4, 0($a1)
    /* 8008D6B8 C4860004 */  lwc1 $f6, 4($a0)
    /* 8008D6BC E4A60004 */  swc1 $f6, 4($a1)
    /* 8008D6C0 C4880008 */  lwc1 $f8, 8($a0)
    /* 8008D6C4 E4A80008 */  swc1 $f8, 8($a1)
    /* 8008D6C8 C48A000C */  lwc1 $f10, 12($a0)
    /* 8008D6CC E4AA000C */  swc1 $f10, 12($a1)
    /* 8008D6D0 C4900010 */  lwc1 $f16, 16($a0)
    /* 8008D6D4 E4B00010 */  swc1 $f16, 16($a1)
    /* 8008D6D8 C4920014 */  lwc1 $f18, 20($a0)
    /* 8008D6DC E4B20014 */  swc1 $f18, 20($a1)
    /* 8008D6E0 C4840018 */  lwc1 $f4, 24($a0)
    /* 8008D6E4 E4A40018 */  swc1 $f4, 24($a1)
    /* 8008D6E8 C486001C */  lwc1 $f6, 28($a0)
    /* 8008D6EC E4A6001C */  swc1 $f6, 28($a1)
    /* 8008D6F0 C4880020 */  lwc1 $f8, 32($a0)
    /* 8008D6F4 03E00008 */  jr $ra
    /* 8008D6F8 E4A80020 */  swc1 $f8, 32($a1)
