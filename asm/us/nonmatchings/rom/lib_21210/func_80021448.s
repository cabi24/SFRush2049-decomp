nonmatching func_80021448, 0x20

glabel func_80021448
    /* 22048 80021448 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 2204C 8002144C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 22050 80021450 0C008454 */  jal        func_80021150
    /* 22054 80021454 248500D6 */   addiu     $a1, $a0, 0xD6
    /* 22058 80021458 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 2205C 8002145C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 22060 80021460 03E00008 */  jr         $ra
    /* 22064 80021464 00000000 */   nop
endlabel func_80021448
