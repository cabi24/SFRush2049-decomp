nonmatching func_80014E10, 0xC

glabel func_80014E10
    /* 15A10 80014E10 3C018004 */  lui        $at, %hi(D_80038390)
    /* 15A14 80014E14 03E00008 */  jr         $ra
    /* 15A18 80014E18 A4208390 */   sh        $zero, %lo(D_80038390)($at)
endlabel func_80014E10
