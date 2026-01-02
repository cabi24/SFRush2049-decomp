# Source: E370.s
# Address: 0x8000D770

glabel func_8000D770
    /* E370 8000D770 40846000 */  mtc0       $a0, $12 /* handwritten instruction */
    /* E374 8000D774 00000000 */  nop
    /* E378 8000D778 03E00008 */  jr         $ra
    /* E37C 8000D77C 00000000 */   nop