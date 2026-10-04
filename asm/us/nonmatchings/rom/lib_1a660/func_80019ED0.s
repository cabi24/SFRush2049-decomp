nonmatching func_80019ED0, 0x78

glabel func_80019ED0
    /* 1AAD0 80019ED0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1AAD4 80019ED4 30A700FF */  andi       $a3, $a1, 0xFF
    /* 1AAD8 80019ED8 240100FF */  addiu      $at, $zero, 0xFF
    /* 1AADC 80019EDC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1AAE0 80019EE0 AFA40018 */  sw         $a0, 0x18($sp)
    /* 1AAE4 80019EE4 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 1AAE8 80019EE8 10E10012 */  beq        $a3, $at, .L80019F34
    /* 1AAEC 80019EEC AFA60020 */   sw        $a2, 0x20($sp)
    /* 1AAF0 80019EF0 30E400FF */  andi       $a0, $a3, 0xFF
    /* 1AAF4 80019EF4 93A50023 */  lbu        $a1, 0x23($sp)
    /* 1AAF8 80019EF8 0C00840A */  jal        func_80021028
    /* 1AAFC 80019EFC A3A7001F */   sb        $a3, 0x1F($sp)
    /* 1AB00 80019F00 1040000C */  beqz       $v0, .L80019F34
    /* 1AB04 80019F04 93A7001F */   lbu       $a3, 0x1F($sp)
    /* 1AB08 80019F08 93A4001B */  lbu        $a0, 0x1B($sp)
    /* 1AB0C 80019F0C 30E500FF */  andi       $a1, $a3, 0xFF
    /* 1AB10 80019F10 93A60023 */  lbu        $a2, 0x23($sp)
    /* 1AB14 80019F14 3084007F */  andi       $a0, $a0, 0x7F
    /* 1AB18 80019F18 0C006723 */  jal        func_80019C8C
    /* 1AB1C 80019F1C 308400FF */   andi      $a0, $a0, 0xFF
    /* 1AB20 80019F20 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1AB24 80019F24 50410004 */  beql       $v0, $at, .L80019F38
    /* 1AB28 80019F28 2402FFFF */   addiu     $v0, $zero, -0x1
    /* 1AB2C 80019F2C 10000003 */  b          .L80019F3C
    /* 1AB30 80019F30 8FBF0014 */   lw        $ra, 0x14($sp)
  .L80019F34:
    /* 1AB34 80019F34 2402FFFF */  addiu      $v0, $zero, -0x1
  .L80019F38:
    /* 1AB38 80019F38 8FBF0014 */  lw         $ra, 0x14($sp)
  .L80019F3C:
    /* 1AB3C 80019F3C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1AB40 80019F40 03E00008 */  jr         $ra
    /* 1AB44 80019F44 00000000 */   nop
endlabel func_80019ED0
