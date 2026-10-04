nonmatching func_8001E0C0, 0x14

glabel func_8001E0C0
    /* 1ECC0 8001E0C0 3C018005 */  lui        $at, %hi(D_8004FD50)
    /* 1ECC4 8001E0C4 AC20FD50 */  sw         $zero, %lo(D_8004FD50)($at)
    /* 1ECC8 8001E0C8 3C018005 */  lui        $at, %hi(D_8004FD54)
    /* 1ECCC 8001E0CC 03E00008 */  jr         $ra
    /* 1ECD0 8001E0D0 AC20FD54 */   sw        $zero, %lo(D_8004FD54)($at)
endlabel func_8001E0C0
