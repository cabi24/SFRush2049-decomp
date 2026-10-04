nonmatching func_800238F4, 0x2C

glabel func_800238F4
    /* 244F4 800238F4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 244F8 800238F8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 244FC 800238FC 00A03025 */  or         $a2, $a1, $zero
    /* 24500 80023900 24850142 */  addiu      $a1, $a0, 0x142
    /* 24504 80023904 0C008DD5 */  jal        func_80023754
    /* 24508 80023908 3C070400 */   lui       $a3, (0x4000000 >> 16)
    /* 2450C 8002390C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 24510 80023910 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 24514 80023914 00001025 */  or         $v0, $zero, $zero
    /* 24518 80023918 03E00008 */  jr         $ra
    /* 2451C 8002391C 00000000 */   nop
endlabel func_800238F4
