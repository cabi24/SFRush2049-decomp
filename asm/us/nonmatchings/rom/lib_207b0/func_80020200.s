nonmatching func_80020200, 0x1C

glabel func_80020200
    /* 20E00 80020200 308E000F */  andi       $t6, $a0, 0xF
    /* 20E04 80020204 000E7840 */  sll        $t7, $t6, 1
    /* 20E08 80020208 3C028005 */  lui        $v0, %hi(D_8004BE98)
    /* 20E0C 8002020C 004F1021 */  addu       $v0, $v0, $t7
    /* 20E10 80020210 AFA40000 */  sw         $a0, 0x0($sp)
    /* 20E14 80020214 03E00008 */  jr         $ra
    /* 20E18 80020218 8442BE98 */   lh        $v0, %lo(D_8004BE98)($v0)
endlabel func_80020200
