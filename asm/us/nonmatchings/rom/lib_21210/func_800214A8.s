nonmatching func_800214A8, 0x20

glabel func_800214A8
    /* 220A8 800214A8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 220AC 800214AC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 220B0 800214B0 0C008454 */  jal        func_80021150
    /* 220B4 800214B4 2485010C */   addiu     $a1, $a0, 0x10C
    /* 220B8 800214B8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 220BC 800214BC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 220C0 800214C0 03E00008 */  jr         $ra
    /* 220C4 800214C4 00000000 */   nop
endlabel func_800214A8
