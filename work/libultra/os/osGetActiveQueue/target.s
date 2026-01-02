# Source: D260.s
# Address: 0x8000C660

glabel func_8000C660
    /* D260 8000C660 3C028003 */  lui        $v0, %hi(D_8002C460)
    /* D264 8000C664 03E00008 */  jr         $ra
    /* D268 8000C668 8C42C460 */   lw        $v0, %lo(D_8002C460)($v0)