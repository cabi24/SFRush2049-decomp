nonmatching func_8002389C, 0x2C

glabel func_8002389C
    /* 2449C 8002389C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 244A0 800238A0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 244A4 800238A4 00A03025 */  or         $a2, $a1, $zero
    /* 244A8 800238A8 2485011E */  addiu      $a1, $a0, 0x11E
    /* 244AC 800238AC 0C008DD5 */  jal        func_80023754
    /* 244B0 800238B0 3C070100 */   lui       $a3, (0x1000000 >> 16)
    /* 244B4 800238B4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 244B8 800238B8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 244BC 800238BC 00001025 */  or         $v0, $zero, $zero
    /* 244C0 800238C0 03E00008 */  jr         $ra
    /* 244C4 800238C4 00000000 */   nop
endlabel func_8002389C
