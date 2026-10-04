nonmatching func_800214C8, 0x20

glabel func_800214C8
    /* 220C8 800214C8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 220CC 800214CC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 220D0 800214D0 0C008454 */  jal        func_80021150
    /* 220D4 800214D4 2485011E */   addiu     $a1, $a0, 0x11E
    /* 220D8 800214D8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 220DC 800214DC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 220E0 800214E0 03E00008 */  jr         $ra
    /* 220E4 800214E4 00000000 */   nop
endlabel func_800214C8
