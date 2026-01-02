# Source: D0B0.s
# Address: 0x8000C520

glabel func_8000C520
    /* D120 8000C520 40086000 */  mfc0       $t0, $12 /* handwritten instruction */
    /* D124 8000C524 01044025 */  or         $t0, $t0, $a0
    /* D128 8000C528 40886000 */  mtc0       $t0, $12 /* handwritten instruction */
    /* D12C 8000C52C 00000000 */  nop
    /* D130 8000C530 00000000 */  nop
    /* D134 8000C534 03E00008 */  jr         $ra
    /* D138 8000C538 00000000 */   nop