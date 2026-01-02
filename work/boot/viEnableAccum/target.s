# Source: 1050.s
# Address: 0x800013C0

glabel func_800013C0
    /* 1FC0 800013C0 240E0001 */  addiu      $t6, $zero, 0x1
    /* 1FC4 800013C4 3C018003 */  lui        $at, %hi(D_8002AFB0)
    /* 1FC8 800013C8 AC2EAFB0 */  sw         $t6, %lo(D_8002AFB0)($at)
    /* 1FCC 800013CC 03E00008 */  jr         $ra
    /* 1FD0 800013D0 00000000 */   nop
    /* 1FD4 800013D4 03E00008 */  jr         $ra
    /* 1FD8 800013D8 00000000 */   nop