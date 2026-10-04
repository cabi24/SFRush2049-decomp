nonmatching func_80025594, 0x5C

glabel func_80025594
    /* 26194 80025594 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 26198 80025598 2401FFFF */  addiu      $at, $zero, -0x1
    /* 2619C 8002559C 1081000F */  beq        $a0, $at, .L800255DC
    /* 261A0 800255A0 AFBF0014 */   sw        $ra, 0x14($sp)
    /* 261A4 800255A4 3C0E8003 */  lui        $t6, %hi(D_8002D480)
    /* 261A8 800255A8 25CED480 */  addiu      $t6, $t6, %lo(D_8002D480)
    /* 261AC 800255AC 91CF0000 */  lbu        $t7, 0x0($t6)
    /* 261B0 800255B0 51E0000B */  beql       $t7, $zero, .L800255E0
    /* 261B4 800255B4 00001025 */   or        $v0, $zero, $zero
    /* 261B8 800255B8 0C009499 */  jal        func_80025264
    /* 261BC 800255BC 00000000 */   nop
    /* 261C0 800255C0 2401FFFF */  addiu      $at, $zero, -0x1
    /* 261C4 800255C4 10410005 */  beq        $v0, $at, .L800255DC
    /* 261C8 800255C8 00402025 */   or        $a0, $v0, $zero
    /* 261CC 800255CC 0C009535 */  jal        func_800254D4
    /* 261D0 800255D0 00000000 */   nop
    /* 261D4 800255D4 10000002 */  b          .L800255E0
    /* 261D8 800255D8 24020001 */   addiu     $v0, $zero, 0x1
  .L800255DC:
    /* 261DC 800255DC 00001025 */  or         $v0, $zero, $zero
  .L800255E0:
    /* 261E0 800255E0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 261E4 800255E4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 261E8 800255E8 03E00008 */  jr         $ra
    /* 261EC 800255EC 00000000 */   nop
endlabel func_80025594
