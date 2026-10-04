nonmatching func_800149BC, 0x20

glabel func_800149BC
    /* 155BC 800149BC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 155C0 800149C0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 155C4 800149C4 0C004721 */  jal        func_80011C84
    /* 155C8 800149C8 3084FFFF */   andi      $a0, $a0, 0xFFFF
    /* 155CC 800149CC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 155D0 800149D0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 155D4 800149D4 03E00008 */  jr         $ra
    /* 155D8 800149D8 00000000 */   nop
endlabel func_800149BC
