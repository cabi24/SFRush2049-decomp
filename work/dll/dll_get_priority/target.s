# Source: CC50.s
# Address: 0x8000C490

glabel func_8000C490
    /* D090 8000C490 14800003 */  bnez       $a0, .L8000C4A0
    /* D094 8000C494 00000000 */   nop
    /* D098 8000C498 3C048003 */  lui        $a0, %hi(D_8002C3E0)
    /* D09C 8000C49C 8C84C3E0 */  lw         $a0, %lo(D_8002C3E0)($a0)
  .L8000C4A0:
    /* D0A0 8000C4A0 03E00008 */  jr         $ra
    /* D0A4 8000C4A4 8C820004 */   lw        $v0, 0x4($a0)