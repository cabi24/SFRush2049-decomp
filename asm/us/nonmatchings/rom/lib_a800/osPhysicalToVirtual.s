nonmatching osPhysicalToVirtual, 0xC

glabel osPhysicalToVirtual
    /* A800 80009C00 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* A804 80009C04 03E00008 */  jr         $ra
    /* A808 80009C08 00811025 */   or        $v0, $a0, $at
endlabel osPhysicalToVirtual
    /* A80C 80009C0C 00000000 */  nop
