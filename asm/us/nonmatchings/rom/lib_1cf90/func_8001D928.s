nonmatching func_8001D928, 0x1C

glabel func_8001D928
    /* 1E528 8001D928 3C018005 */  lui        $at, %hi(D_8004FF20)
    /* 1E52C 8001D92C A020FF20 */  sb         $zero, %lo(D_8004FF20)($at)
    /* 1E530 8001D930 3C018005 */  lui        $at, %hi(D_800502A8)
    /* 1E534 8001D934 A02002A8 */  sb         $zero, %lo(D_800502A8)($at)
    /* 1E538 8001D938 3C018005 */  lui        $at, %hi(D_80050430)
    /* 1E53C 8001D93C 03E00008 */  jr         $ra
    /* 1E540 8001D940 A0200430 */   sb        $zero, %lo(D_80050430)($at)
endlabel func_8001D928
