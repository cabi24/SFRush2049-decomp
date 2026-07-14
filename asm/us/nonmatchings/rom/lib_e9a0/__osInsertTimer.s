nonmatching __osInsertTimer, 0x28

glabel __osInsertTimer
    /* EF90 8000E390 3C038003 */  lui        $v1, %hi(__osPiMgrState)
    /* EF94 8000E394 2463C380 */  addiu      $v1, $v1, %lo(__osPiMgrState)
    /* EF98 8000E398 8C6E0000 */  lw         $t6, 0x0($v1)
    /* EF9C 8000E39C 55C00004 */  bnel       $t6, $zero, .L8000E3B0
    /* EFA0 8000E3A0 8C620008 */   lw        $v0, 0x8($v1)
    /* EFA4 8000E3A4 03E00008 */  jr         $ra
    /* EFA8 8000E3A8 00001025 */   or        $v0, $zero, $zero
    /* EFAC 8000E3AC 8C620008 */  lw         $v0, 0x8($v1)
  .L8000E3B0:
    /* EFB0 8000E3B0 03E00008 */  jr         $ra
    /* EFB4 8000E3B4 00000000 */   nop
endlabel __osInsertTimer
    /* EFB8 8000E3B8 00000000 */  nop
    /* EFBC 8000E3BC 00000000 */  nop
