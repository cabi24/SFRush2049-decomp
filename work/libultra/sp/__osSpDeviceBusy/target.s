# Source: E310.s
# Address: 0x8000D710

glabel func_8000D710
    /* E310 8000D710 3C0EA404 */  lui        $t6, %hi(SP_STATUS_REG)
    /* E314 8000D714 8DC20010 */  lw         $v0, %lo(SP_STATUS_REG)($t6)
    /* E318 8000D718 304F001C */  andi       $t7, $v0, 0x1C
    /* E31C 8000D71C 11E00003 */  beqz       $t7, .L8000D72C
    /* E320 8000D720 00001025 */   or        $v0, $zero, $zero
    /* E324 8000D724 03E00008 */  jr         $ra
    /* E328 8000D728 24020001 */   addiu     $v0, $zero, 0x1
  .L8000D72C:
    /* E32C 8000D72C 03E00008 */  jr         $ra
    /* E330 8000D730 00000000 */   nop