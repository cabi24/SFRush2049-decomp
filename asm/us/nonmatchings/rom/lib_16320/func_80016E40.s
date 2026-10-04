nonmatching func_80016E40, 0x28

glabel func_80016E40
    /* 17A40 80016E40 90810004 */  lbu        $at, 0x4($a0)
    /* 17A44 80016E44 908E0005 */  lbu        $t6, 0x5($a0)
    /* 17A48 80016E48 90AF0005 */  lbu        $t7, 0x5($a1)
    /* 17A4C 80016E4C 00010A00 */  sll        $at, $at, 8
    /* 17A50 80016E50 01C17025 */  or         $t6, $t6, $at
    /* 17A54 80016E54 90A10004 */  lbu        $at, 0x4($a1)
    /* 17A58 80016E58 00010A00 */  sll        $at, $at, 8
    /* 17A5C 80016E5C 01E17825 */  or         $t7, $t7, $at
    /* 17A60 80016E60 03E00008 */  jr         $ra
    /* 17A64 80016E64 01CF1023 */   subu      $v0, $t6, $t7
endlabel func_80016E40
