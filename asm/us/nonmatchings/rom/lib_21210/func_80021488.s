nonmatching func_80021488, 0x20

glabel func_80021488
    /* 22088 80021488 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 2208C 8002148C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 22090 80021490 0C008454 */  jal        func_80021150
    /* 22094 80021494 248500FA */   addiu     $a1, $a0, 0xFA
    /* 22098 80021498 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 2209C 8002149C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 220A0 800214A0 03E00008 */  jr         $ra
    /* 220A4 800214A4 00000000 */   nop
endlabel func_80021488
