nonmatching bzero_alt, 0xC

glabel bzero_alt
    /* DEA0 8000D2A0 3C0EA404 */  lui        $t6, %hi(SP_STATUS_REG)
    /* DEA4 8000D2A4 03E00008 */  jr         $ra
    /* DEA8 8000D2A8 8DC20010 */   lw        $v0, %lo(SP_STATUS_REG)($t6)
endlabel bzero_alt
    /* DEAC 8000D2AC 00000000 */  nop
