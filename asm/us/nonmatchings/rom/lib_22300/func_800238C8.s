nonmatching func_800238C8, 0x2C

glabel func_800238C8
    /* 244C8 800238C8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 244CC 800238CC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 244D0 800238D0 00A03025 */  or         $a2, $a1, $zero
    /* 244D4 800238D4 24850130 */  addiu      $a1, $a0, 0x130
    /* 244D8 800238D8 0C008DD5 */  jal        func_80023754
    /* 244DC 800238DC 3C070800 */   lui       $a3, (0x8000000 >> 16)
    /* 244E0 800238E0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 244E4 800238E4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 244E8 800238E8 00001025 */  or         $v0, $zero, $zero
    /* 244EC 800238EC 03E00008 */  jr         $ra
    /* 244F0 800238F0 00000000 */   nop
endlabel func_800238C8
