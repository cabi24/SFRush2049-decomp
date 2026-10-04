nonmatching func_800214E8, 0x20

glabel func_800214E8
    /* 220E8 800214E8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 220EC 800214EC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 220F0 800214F0 0C008454 */  jal        func_80021150
    /* 220F4 800214F4 24850130 */   addiu     $a1, $a0, 0x130
    /* 220F8 800214F8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 220FC 800214FC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 22100 80021500 03E00008 */  jr         $ra
    /* 22104 80021504 00000000 */   nop
endlabel func_800214E8
