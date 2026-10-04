nonmatching func_80023544, 0x20

glabel func_80023544
    /* 24144 80023544 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 24148 80023548 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 2414C 8002354C 0C008CEC */  jal        func_800233B0
    /* 24150 80023550 00003025 */   or        $a2, $zero, $zero
    /* 24154 80023554 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 24158 80023558 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 2415C 8002355C 03E00008 */  jr         $ra
    /* 24160 80023560 00000000 */   nop
endlabel func_80023544
