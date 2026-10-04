nonmatching func_80014D1C, 0x10

glabel func_80014D1C
    /* 1591C 80014D1C 240E0001 */  addiu      $t6, $zero, 0x1
    /* 15920 80014D20 3C018004 */  lui        $at, %hi(D_80038291)
    /* 15924 80014D24 03E00008 */  jr         $ra
    /* 15928 80014D28 A02E8291 */   sb        $t6, %lo(D_80038291)($at)
endlabel func_80014D1C
    /* 1592C 80014D2C 00000000 */  nop
