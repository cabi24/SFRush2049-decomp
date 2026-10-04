nonmatching func_8001CCC0, 0x1C

glabel func_8001CCC0
    /* 1D8C0 8001CCC0 2C814000 */  sltiu      $at, $a0, 0x4000
    /* 1D8C4 8001CCC4 14200003 */  bnez       $at, .L8001CCD4
    /* 1D8C8 8001CCC8 3082FFFF */   andi      $v0, $a0, 0xFFFF
    /* 1D8CC 8001CCCC 03E00008 */  jr         $ra
    /* 1D8D0 8001CCD0 24023FFF */   addiu     $v0, $zero, 0x3FFF
  .L8001CCD4:
    /* 1D8D4 8001CCD4 03E00008 */  jr         $ra
    /* 1D8D8 8001CCD8 00000000 */   nop
endlabel func_8001CCC0
