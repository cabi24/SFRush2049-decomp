# Source: 92F0.s
# Address: 0x8000870C

glabel func_8000870C
    /* 930C 8000870C 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* 9310 80008710 AFA40008 */  sw         $a0, 0x8($sp)
    /* 9314 80008714 AFA5000C */  sw         $a1, 0xC($sp)
    /* 9318 80008718 AFA60010 */  sw         $a2, 0x10($sp)
    /* 931C 8000871C AFA70014 */  sw         $a3, 0x14($sp)
    /* 9320 80008720 03E00008 */  jr         $ra
    /* 9324 80008724 27BD0008 */   addiu     $sp, $sp, 0x8