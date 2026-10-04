nonmatching func_8001F864, 0x34

glabel func_8001F864
    /* 20464 8001F864 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 20468 8001F868 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 2046C 8001F86C 0C007DFB */  jal        func_8001F7EC
    /* 20470 8001F870 00000000 */   nop
    /* 20474 8001F874 0C007B8D */  jal        func_8001EE34
    /* 20478 8001F878 00000000 */   nop
    /* 2047C 8001F87C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 20480 8001F880 3C018005 */  lui        $at, %hi(D_800504C2)
    /* 20484 8001F884 A02004C2 */  sb         $zero, %lo(D_800504C2)($at)
    /* 20488 8001F888 3C018005 */  lui        $at, %hi(D_800504C3)
    /* 2048C 8001F88C A02004C3 */  sb         $zero, %lo(D_800504C3)($at)
    /* 20490 8001F890 03E00008 */  jr         $ra
    /* 20494 8001F894 27BD0018 */   addiu     $sp, $sp, 0x18
endlabel func_8001F864
