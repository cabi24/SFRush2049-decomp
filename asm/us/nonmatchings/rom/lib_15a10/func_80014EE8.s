nonmatching func_80014EE8, 0x2C

glabel func_80014EE8
    /* 15AE8 80014EE8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 15AEC 80014EEC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 15AF0 80014EF0 AFA40018 */  sw         $a0, 0x18($sp)
    /* 15AF4 80014EF4 8CAE000C */  lw         $t6, 0xC($a1)
    /* 15AF8 80014EF8 3084FFFF */  andi       $a0, $a0, 0xFFFF
    /* 15AFC 80014EFC 0C005387 */  jal        func_80014E1C
    /* 15B00 80014F00 00AE2821 */   addu      $a1, $a1, $t6
    /* 15B04 80014F04 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 15B08 80014F08 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 15B0C 80014F0C 03E00008 */  jr         $ra
    /* 15B10 80014F10 00000000 */   nop
endlabel func_80014EE8
