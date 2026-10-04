nonmatching func_80021F08, 0x60

glabel func_80021F08
    /* 22B08 80021F08 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 22B0C 80021F0C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 22B10 80021F10 AFA40018 */  sw         $a0, 0x18($sp)
    /* 22B14 80021F14 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 22B18 80021F18 8CA40000 */  lw         $a0, 0x0($a1)
    /* 22B1C 80021F1C 00042402 */  srl        $a0, $a0, 16
    /* 22B20 80021F20 0C005B08 */  jal        func_80016C20
    /* 22B24 80021F24 3084FFFF */   andi      $a0, $a0, 0xFFFF
    /* 22B28 80021F28 1040000A */  beqz       $v0, .L80021F54
    /* 22B2C 80021F2C 8FA30018 */   lw        $v1, 0x18($sp)
    /* 22B30 80021F30 A862001C */  swl        $v0, 0x1C($v1)
    /* 22B34 80021F34 B862001F */  swr        $v0, 0x1F($v1)
    /* 22B38 80021F38 8FAF001C */  lw         $t7, 0x1C($sp)
    /* 22B3C 80021F3C 8DF80004 */  lw         $t8, 0x4($t7)
    /* 22B40 80021F40 3319FFFF */  andi       $t9, $t8, 0xFFFF
    /* 22B44 80021F44 001940C0 */  sll        $t0, $t9, 3
    /* 22B48 80021F48 01024821 */  addu       $t1, $t0, $v0
    /* 22B4C 80021F4C A8690020 */  swl        $t1, 0x20($v1)
    /* 22B50 80021F50 B8690023 */  swr        $t1, 0x23($v1)
  .L80021F54:
    /* 22B54 80021F54 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 22B58 80021F58 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 22B5C 80021F5C 00001025 */  or         $v0, $zero, $zero
    /* 22B60 80021F60 03E00008 */  jr         $ra
    /* 22B64 80021F64 00000000 */   nop
endlabel func_80021F08
