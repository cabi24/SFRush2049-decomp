nonmatching func_80014E64, 0x2C

glabel func_80014E64
    /* 15A64 80014E64 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 15A68 80014E68 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 15A6C 80014E6C AFA40018 */  sw         $a0, 0x18($sp)
    /* 15A70 80014E70 8CAE0000 */  lw         $t6, 0x0($a1)
    /* 15A74 80014E74 3084FFFF */  andi       $a0, $a0, 0xFFFF
    /* 15A78 80014E78 0C005387 */  jal        func_80014E1C
    /* 15A7C 80014E7C 00AE2821 */   addu      $a1, $a1, $t6
    /* 15A80 80014E80 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 15A84 80014E84 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 15A88 80014E88 03E00008 */  jr         $ra
    /* 15A8C 80014E8C 00000000 */   nop
endlabel func_80014E64
