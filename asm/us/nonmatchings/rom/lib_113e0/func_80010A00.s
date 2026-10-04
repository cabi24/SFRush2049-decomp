nonmatching func_80010A00, 0xC

glabel func_80010A00
    /* 11600 80010A00 3C028003 */  lui        $v0, %hi(D_8002C630)
    /* 11604 80010A04 03E00008 */  jr         $ra
    /* 11608 80010A08 9042C630 */   lbu       $v0, %lo(D_8002C630)($v0)
endlabel func_80010A00
