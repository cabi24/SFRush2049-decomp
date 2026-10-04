nonmatching func_80021468, 0x20

glabel func_80021468
    /* 22068 80021468 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 2206C 8002146C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 22070 80021470 0C008454 */  jal        func_80021150
    /* 22074 80021474 248500E8 */   addiu     $a1, $a0, 0xE8
    /* 22078 80021478 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 2207C 8002147C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 22080 80021480 03E00008 */  jr         $ra
    /* 22084 80021484 00000000 */   nop
endlabel func_80021468
