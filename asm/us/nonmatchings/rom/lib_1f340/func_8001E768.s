nonmatching func_8001E768, 0x28

glabel func_8001E768
    /* 1F368 8001E768 3C198004 */  lui        $t9, %hi(D_8003801C)
    /* 1F36C 8001E76C 8F39801C */  lw         $t9, %lo(D_8003801C)($t9)
    /* 1F370 8001E770 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1F374 8001E774 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1F378 8001E778 0320F809 */  jalr       $t9
    /* 1F37C 8001E77C 00000000 */   nop
    /* 1F380 8001E780 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1F384 8001E784 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1F388 8001E788 03E00008 */  jr         $ra
    /* 1F38C 8001E78C 00000000 */   nop
endlabel func_8001E768
