nonmatching func_80021528, 0x20

glabel func_80021528
    /* 22128 80021528 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 2212C 8002152C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 22130 80021530 0C008454 */  jal        func_80021150
    /* 22134 80021534 24850154 */   addiu     $a1, $a0, 0x154
    /* 22138 80021538 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 2213C 8002153C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 22140 80021540 03E00008 */  jr         $ra
    /* 22144 80021544 00000000 */   nop
endlabel func_80021528
