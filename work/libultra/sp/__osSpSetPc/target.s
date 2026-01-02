# Source: E250.s
# Address: 0x8000D650

glabel func_8000D650
    /* E250 8000D650 3C0EA404 */  lui        $t6, %hi(SP_STATUS_REG)
    /* E254 8000D654 8DC20010 */  lw         $v0, %lo(SP_STATUS_REG)($t6)
    /* E258 8000D658 3C18A408 */  lui        $t8, %hi(SP_PC)
    /* E25C 8000D65C 304F0001 */  andi       $t7, $v0, 0x1
    /* E260 8000D660 15E00003 */  bnez       $t7, .L8000D670
    /* E264 8000D664 00001025 */   or        $v0, $zero, $zero
    /* E268 8000D668 03E00008 */  jr         $ra
    /* E26C 8000D66C 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8000D670:
    /* E270 8000D670 AF040000 */  sw         $a0, %lo(SP_PC)($t8)
    /* E274 8000D674 03E00008 */  jr         $ra
    /* E278 8000D678 00000000 */   nop