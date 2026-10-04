nonmatching func_8001E740, 0x28

glabel func_8001E740
    /* 1F340 8001E740 3C198004 */  lui        $t9, %hi(D_80038018)
    /* 1F344 8001E744 8F398018 */  lw         $t9, %lo(D_80038018)($t9)
    /* 1F348 8001E748 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1F34C 8001E74C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1F350 8001E750 0320F809 */  jalr       $t9
    /* 1F354 8001E754 00002825 */   or        $a1, $zero, $zero
    /* 1F358 8001E758 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1F35C 8001E75C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1F360 8001E760 03E00008 */  jr         $ra
    /* 1F364 8001E764 00000000 */   nop
endlabel func_8001E740
