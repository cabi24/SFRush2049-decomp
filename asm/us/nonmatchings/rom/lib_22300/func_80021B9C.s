nonmatching func_80021B9C, 0x24

glabel func_80021B9C
    /* 2279C 80021B9C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 227A0 80021BA0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 227A4 80021BA4 240E0001 */  addiu      $t6, $zero, 0x1
    /* 227A8 80021BA8 0C00864F */  jal        func_8002193C
    /* 227AC 80021BAC A0AE0006 */   sb        $t6, 0x6($a1)
    /* 227B0 80021BB0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 227B4 80021BB4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 227B8 80021BB8 03E00008 */  jr         $ra
    /* 227BC 80021BBC 00000000 */   nop
endlabel func_80021B9C
