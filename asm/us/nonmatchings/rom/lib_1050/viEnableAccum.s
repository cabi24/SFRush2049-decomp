nonmatching viEnableAccum, 0x1C

glabel viEnableAccum
    /* 1FC0 800013C0 240E0001 */  addiu      $t6, $zero, 0x1
    /* 1FC4 800013C4 3C018003 */  lui        $at, %hi(gScTimeEnabled)
    /* 1FC8 800013C8 AC2EAFB0 */  sw         $t6, %lo(gScTimeEnabled)($at)
    /* 1FCC 800013CC 03E00008 */  jr         $ra
    /* 1FD0 800013D0 00000000 */   nop
    /* 1FD4 800013D4 03E00008 */  jr         $ra
    /* 1FD8 800013D8 00000000 */   nop
endlabel viEnableAccum
