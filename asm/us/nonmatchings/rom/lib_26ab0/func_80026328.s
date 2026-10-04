nonmatching func_80026328, 0x20

glabel func_80026328
    /* 26F28 80026328 808E11DD */  lb         $t6, 0x11DD($a0)
    /* 26F2C 8002632C 24010002 */  addiu      $at, $zero, 0x2
    /* 26F30 80026330 240F0003 */  addiu      $t7, $zero, 0x3
    /* 26F34 80026334 15C10002 */  bne        $t6, $at, .L80026340
    /* 26F38 80026338 00000000 */   nop
    /* 26F3C 8002633C A08F11DD */  sb         $t7, 0x11DD($a0)
  .L80026340:
    /* 26F40 80026340 03E00008 */  jr         $ra
    /* 26F44 80026344 00000000 */   nop
endlabel func_80026328
