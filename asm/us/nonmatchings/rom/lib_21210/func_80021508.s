nonmatching func_80021508, 0x20

glabel func_80021508
    /* 22108 80021508 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 2210C 8002150C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 22110 80021510 0C008454 */  jal        func_80021150
    /* 22114 80021514 24850142 */   addiu     $a1, $a0, 0x142
    /* 22118 80021518 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 2211C 8002151C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 22120 80021520 03E00008 */  jr         $ra
    /* 22124 80021524 00000000 */   nop
endlabel func_80021508
