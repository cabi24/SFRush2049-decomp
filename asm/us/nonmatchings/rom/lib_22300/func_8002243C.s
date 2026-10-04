nonmatching func_8002243C, 0x20

glabel func_8002243C
    /* 2303C 8002243C 8CAE0000 */  lw         $t6, 0x0($a1)
    /* 23040 80022440 00001025 */  or         $v0, $zero, $zero
    /* 23044 80022444 000E7C02 */  srl        $t7, $t6, 16
    /* 23048 80022448 31F8FFFF */  andi       $t8, $t7, 0xFFFF
    /* 2304C 8002244C 0018CBC0 */  sll        $t9, $t8, 15
    /* 23050 80022450 A8990028 */  swl        $t9, 0x28($a0)
    /* 23054 80022454 03E00008 */  jr         $ra
    /* 23058 80022458 B899002B */   swr       $t9, 0x2B($a0)
endlabel func_8002243C
