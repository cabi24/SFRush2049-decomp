nonmatching func_80014EBC, 0x2C

glabel func_80014EBC
    /* 15ABC 80014EBC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 15AC0 80014EC0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 15AC4 80014EC4 AFA40018 */  sw         $a0, 0x18($sp)
    /* 15AC8 80014EC8 8CAE0008 */  lw         $t6, 0x8($a1)
    /* 15ACC 80014ECC 3084FFFF */  andi       $a0, $a0, 0xFFFF
    /* 15AD0 80014ED0 0C005387 */  jal        func_80014E1C
    /* 15AD4 80014ED4 00AE2821 */   addu      $a1, $a1, $t6
    /* 15AD8 80014ED8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 15ADC 80014EDC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 15AE0 80014EE0 03E00008 */  jr         $ra
    /* 15AE4 80014EE4 00000000 */   nop
endlabel func_80014EBC
