# Source: E4F0.s
# Address: 0x8000DBB0

glabel func_8000DBB0
    /* E7B0 8000DBB0 40026800 */  mfc0       $v0, $13 /* handwritten instruction */
    /* E7B4 8000DBB4 03E00008 */  jr         $ra
    /* E7B8 8000DBB8 00000000 */   nop