nonmatching func_800109C0, 0x40

glabel func_800109C0
    /* 115C0 800109C0 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 115C4 800109C4 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 115C8 800109C8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 115CC 800109CC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 115D0 800109D0 51C00008 */  beql       $t6, $zero, .L800109F4
    /* 115D4 800109D4 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 115D8 800109D8 0C00513C */  jal        func_800144F0
    /* 115DC 800109DC 00000000 */   nop
    /* 115E0 800109E0 0C007835 */  jal        func_8001E0D4
    /* 115E4 800109E4 00000000 */   nop
    /* 115E8 800109E8 3C018003 */  lui        $at, %hi(D_8002C630)
    /* 115EC 800109EC A020C630 */  sb         $zero, %lo(D_8002C630)($at)
    /* 115F0 800109F0 8FBF0014 */  lw         $ra, 0x14($sp)
  .L800109F4:
    /* 115F4 800109F4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 115F8 800109F8 03E00008 */  jr         $ra
    /* 115FC 800109FC 00000000 */   nop
endlabel func_800109C0
