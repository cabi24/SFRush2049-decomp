nonmatching func_80014BD8, 0x20

glabel func_80014BD8
    /* 157D8 80014BD8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 157DC 80014BDC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 157E0 80014BE0 0C0043A0 */  jal        func_80010E80
    /* 157E4 80014BE4 00000000 */   nop
    /* 157E8 80014BE8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 157EC 80014BEC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 157F0 80014BF0 03E00008 */  jr         $ra
    /* 157F4 80014BF4 00000000 */   nop
endlabel func_80014BD8
