# Source: A800.s
# Address: 0x80009C00

glabel func_80009C00
    /* A800 80009C00 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* A804 80009C04 03E00008 */  jr         $ra
    /* A808 80009C08 00811025 */   or        $v0, $a0, $at