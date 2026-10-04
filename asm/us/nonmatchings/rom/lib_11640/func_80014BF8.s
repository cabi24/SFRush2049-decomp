nonmatching func_80014BF8, 0x20

glabel func_80014BF8
    /* 157F8 80014BF8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 157FC 80014BFC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 15800 80014C00 0C00441D */  jal        func_80011074
    /* 15804 80014C04 00000000 */   nop
    /* 15808 80014C08 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1580C 80014C0C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 15810 80014C10 03E00008 */  jr         $ra
    /* 15814 80014C14 00000000 */   nop
endlabel func_80014BF8
