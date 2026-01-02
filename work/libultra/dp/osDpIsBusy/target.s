# Source: E340.s
# Address: 0x8000D740

glabel func_8000D740
    /* E340 8000D740 3C0EA410 */  lui        $t6, %hi(DPC_STATUS_REG)
    /* E344 8000D744 8DC2000C */  lw         $v0, %lo(DPC_STATUS_REG)($t6)
    /* E348 8000D748 304F0100 */  andi       $t7, $v0, 0x100
    /* E34C 8000D74C 11E00003 */  beqz       $t7, .L8000D75C
    /* E350 8000D750 00001025 */   or        $v0, $zero, $zero
    /* E354 8000D754 03E00008 */  jr         $ra
    /* E358 8000D758 24020001 */   addiu     $v0, $zero, 0x1
  .L8000D75C:
    /* E35C 8000D75C 03E00008 */  jr         $ra
    /* E360 8000D760 00000000 */   nop