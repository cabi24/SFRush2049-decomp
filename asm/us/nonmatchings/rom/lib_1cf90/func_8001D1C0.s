nonmatching func_8001D1C0, 0x34

glabel func_8001D1C0
    /* 1DDC0 8001D1C0 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 1DDC4 8001D1C4 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 1DDC8 8001D1C8 00001025 */  or         $v0, $zero, $zero
    /* 1DDCC 8001D1CC 11C00007 */  beqz       $t6, .L8001D1EC
    /* 1DDD0 8001D1D0 00000000 */   nop
    /* 1DDD4 8001D1D4 8C820008 */  lw         $v0, 0x8($a0)
    /* 1DDD8 8001D1D8 3C010001 */  lui        $at, (0x10000 >> 16)
    /* 1DDDC 8001D1DC 00411024 */  and        $v0, $v0, $at
    /* 1DDE0 8001D1E0 0002102B */  sltu       $v0, $zero, $v0
    /* 1DDE4 8001D1E4 03E00008 */  jr         $ra
    /* 1DDE8 8001D1E8 304200FF */   andi      $v0, $v0, 0xFF
  .L8001D1EC:
    /* 1DDEC 8001D1EC 03E00008 */  jr         $ra
    /* 1DDF0 8001D1F0 00000000 */   nop
endlabel func_8001D1C0
