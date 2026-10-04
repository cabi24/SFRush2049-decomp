nonmatching func_80015348, 0x24

glabel func_80015348
    /* 15F48 80015348 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 15F4C 8001534C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 15F50 80015350 AFA40018 */  sw         $a0, 0x18($sp)
    /* 15F54 80015354 0C005987 */  jal        func_8001661C
    /* 15F58 80015358 3084FFFF */   andi      $a0, $a0, 0xFFFF
    /* 15F5C 8001535C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 15F60 80015360 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 15F64 80015364 03E00008 */  jr         $ra
    /* 15F68 80015368 00000000 */   nop
endlabel func_80015348
