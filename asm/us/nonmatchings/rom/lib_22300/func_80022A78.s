nonmatching func_80022A78, 0x20

glabel func_80022A78
    /* 23678 80022A78 AFA50004 */  sw         $a1, 0x4($sp)
    /* 2367C 80022A7C 888E0024 */  lwl        $t6, 0x24($a0)
    /* 23680 80022A80 988E0027 */  lwr        $t6, 0x27($a0)
    /* 23684 80022A84 00001025 */  or         $v0, $zero, $zero
    /* 23688 80022A88 35CF0080 */  ori        $t7, $t6, 0x80
    /* 2368C 80022A8C A88F0024 */  swl        $t7, 0x24($a0)
    /* 23690 80022A90 03E00008 */  jr         $ra
    /* 23694 80022A94 B88F0027 */   swr       $t7, 0x27($a0)
endlabel func_80022A78
