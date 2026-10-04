nonmatching func_80016CE0, 0x10

glabel func_80016CE0
    /* 178E0 80016CE0 948E0000 */  lhu        $t6, 0x0($a0)
    /* 178E4 80016CE4 94AF0000 */  lhu        $t7, 0x0($a1)
    /* 178E8 80016CE8 03E00008 */  jr         $ra
    /* 178EC 80016CEC 01CF1023 */   subu      $v0, $t6, $t7
endlabel func_80016CE0
