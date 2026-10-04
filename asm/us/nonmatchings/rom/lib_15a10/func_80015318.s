nonmatching func_80015318, 0x30

glabel func_80015318
    /* 15F18 80015318 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 15F1C 8001531C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 15F20 80015320 AFA40018 */  sw         $a0, 0x18($sp)
    /* 15F24 80015324 00A03825 */  or         $a3, $a1, $zero
    /* 15F28 80015328 3084FFFF */  andi       $a0, $a0, 0xFFFF
    /* 15F2C 8001532C 94E60000 */  lhu        $a2, 0x0($a3)
    /* 15F30 80015330 0C005934 */  jal        func_800164D0
    /* 15F34 80015334 24A50004 */   addiu     $a1, $a1, 0x4
    /* 15F38 80015338 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 15F3C 8001533C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 15F40 80015340 03E00008 */  jr         $ra
    /* 15F44 80015344 00000000 */   nop
endlabel func_80015318
