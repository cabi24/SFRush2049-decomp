nonmatching func_80021428, 0x20

glabel func_80021428
    /* 22028 80021428 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 2202C 8002142C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 22030 80021430 0C008454 */  jal        func_80021150
    /* 22034 80021434 248500C4 */   addiu     $a1, $a0, 0xC4
    /* 22038 80021438 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 2203C 8002143C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 22040 80021440 03E00008 */  jr         $ra
    /* 22044 80021444 00000000 */   nop
endlabel func_80021428
