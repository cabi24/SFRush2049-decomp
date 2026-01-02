# Source: 1050.s
# Address: 0x800013DC

glabel func_800013DC
    /* 1FDC 800013DC 3C018003 */  lui        $at, %hi(D_8002AFB0)
    /* 1FE0 800013E0 AC20AFB0 */  sw         $zero, %lo(D_8002AFB0)($at)
    /* 1FE4 800013E4 03E00008 */  jr         $ra
    /* 1FE8 800013E8 00000000 */   nop
    /* 1FEC 800013EC 03E00008 */  jr         $ra
    /* 1FF0 800013F0 00000000 */   nop