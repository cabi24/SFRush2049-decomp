nonmatching func_80020518, 0x10

glabel func_80020518
    /* 21118 80020518 3C018005 */  lui        $at, %hi(D_8004F2F8)
    /* 2111C 8002051C AFA40000 */  sw         $a0, 0x0($sp)
    /* 21120 80020520 03E00008 */  jr         $ra
    /* 21124 80020524 A024F2F8 */   sb        $a0, %lo(D_8004F2F8)($at)
endlabel func_80020518
