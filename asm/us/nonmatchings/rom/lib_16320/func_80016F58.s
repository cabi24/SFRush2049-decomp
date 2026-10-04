nonmatching func_80016F58, 0x28

glabel func_80016F58
    /* 17B58 80016F58 90810004 */  lbu        $at, 0x4($a0)
    /* 17B5C 80016F5C 908E0005 */  lbu        $t6, 0x5($a0)
    /* 17B60 80016F60 90AF0005 */  lbu        $t7, 0x5($a1)
    /* 17B64 80016F64 00010A00 */  sll        $at, $at, 8
    /* 17B68 80016F68 01C17025 */  or         $t6, $t6, $at
    /* 17B6C 80016F6C 90A10004 */  lbu        $at, 0x4($a1)
    /* 17B70 80016F70 00010A00 */  sll        $at, $at, 8
    /* 17B74 80016F74 01E17825 */  or         $t7, $t7, $at
    /* 17B78 80016F78 03E00008 */  jr         $ra
    /* 17B7C 80016F7C 01CF1023 */   subu      $v0, $t6, $t7
endlabel func_80016F58
