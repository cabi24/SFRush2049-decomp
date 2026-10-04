nonmatching func_800225DC, 0x20

glabel func_800225DC
    /* 231DC 800225DC 8CAE0000 */  lw         $t6, 0x0($a1)
    /* 231E0 800225E0 00001025 */  or         $v0, $zero, $zero
    /* 231E4 800225E4 000E7C02 */  srl        $t7, $t6, 16
    /* 231E8 800225E8 A08F006A */  sb         $t7, 0x6A($a0)
    /* 231EC 800225EC 8CB80000 */  lw         $t8, 0x0($a1)
    /* 231F0 800225F0 0018CA02 */  srl        $t9, $t8, 8
    /* 231F4 800225F4 03E00008 */  jr         $ra
    /* 231F8 800225F8 A099006B */   sb        $t9, 0x6B($a0)
endlabel func_800225DC
