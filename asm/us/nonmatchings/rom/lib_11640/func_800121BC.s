nonmatching func_800121BC, 0x44

glabel func_800121BC
    /* 12DBC 800121BC 3C198004 */  lui        $t9, %hi(D_8003801C)
    /* 12DC0 800121C0 8F39801C */  lw         $t9, %lo(D_8003801C)($t9)
    /* 12DC4 800121C4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 12DC8 800121C8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 12DCC 800121CC 3C048004 */  lui        $a0, %hi(D_8003833C)
    /* 12DD0 800121D0 0320F809 */  jalr       $t9
    /* 12DD4 800121D4 8C84833C */   lw        $a0, %lo(D_8003833C)($a0)
    /* 12DD8 800121D8 3C198004 */  lui        $t9, %hi(D_8003801C)
    /* 12DDC 800121DC 8F39801C */  lw         $t9, %lo(D_8003801C)($t9)
    /* 12DE0 800121E0 3C048004 */  lui        $a0, %hi(D_80038338)
    /* 12DE4 800121E4 8C848338 */  lw         $a0, %lo(D_80038338)($a0)
    /* 12DE8 800121E8 0320F809 */  jalr       $t9
    /* 12DEC 800121EC 00000000 */   nop
    /* 12DF0 800121F0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 12DF4 800121F4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 12DF8 800121F8 03E00008 */  jr         $ra
    /* 12DFC 800121FC 00000000 */   nop
endlabel func_800121BC
