nonmatching func_80014C40, 0x20

glabel func_80014C40
    /* 15840 80014C40 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 15844 80014C44 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 15848 80014C48 0C001F28 */  jal        osWritebackDCache
    /* 1584C 80014C4C 00052840 */   sll       $a1, $a1, 1
    /* 15850 80014C50 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 15854 80014C54 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 15858 80014C58 03E00008 */  jr         $ra
    /* 1585C 80014C5C 00000000 */   nop
endlabel func_80014C40
