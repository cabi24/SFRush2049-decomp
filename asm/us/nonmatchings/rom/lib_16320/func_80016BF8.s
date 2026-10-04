nonmatching func_80016BF8, 0x28

glabel func_80016BF8
    /* 177F8 80016BF8 90810004 */  lbu        $at, 0x4($a0)
    /* 177FC 80016BFC 908E0005 */  lbu        $t6, 0x5($a0)
    /* 17800 80016C00 90AF0005 */  lbu        $t7, 0x5($a1)
    /* 17804 80016C04 00010A00 */  sll        $at, $at, 8
    /* 17808 80016C08 01C17025 */  or         $t6, $t6, $at
    /* 1780C 80016C0C 90A10004 */  lbu        $at, 0x4($a1)
    /* 17810 80016C10 00010A00 */  sll        $at, $at, 8
    /* 17814 80016C14 01E17825 */  or         $t7, $t7, $at
    /* 17818 80016C18 03E00008 */  jr         $ra
    /* 1781C 80016C1C 01CF1023 */   subu      $v0, $t6, $t7
endlabel func_80016BF8
