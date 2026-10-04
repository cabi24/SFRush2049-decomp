nonmatching func_80014E90, 0x2C

glabel func_80014E90
    /* 15A90 80014E90 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 15A94 80014E94 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 15A98 80014E98 AFA40018 */  sw         $a0, 0x18($sp)
    /* 15A9C 80014E9C 8CAE0004 */  lw         $t6, 0x4($a1)
    /* 15AA0 80014EA0 3084FFFF */  andi       $a0, $a0, 0xFFFF
    /* 15AA4 80014EA4 0C005387 */  jal        func_80014E1C
    /* 15AA8 80014EA8 00AE2821 */   addu      $a1, $a1, $t6
    /* 15AAC 80014EAC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 15AB0 80014EB0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 15AB4 80014EB4 03E00008 */  jr         $ra
    /* 15AB8 80014EB8 00000000 */   nop
endlabel func_80014E90
