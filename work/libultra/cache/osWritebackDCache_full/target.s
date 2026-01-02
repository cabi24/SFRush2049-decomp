# Source: 92F0.s
# Address: 0x800086F0

glabel func_800086F0
    /* 92F0 800086F0 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* 92F4 800086F4 AFA40008 */  sw         $a0, 0x8($sp)
    /* 92F8 800086F8 AFA5000C */  sw         $a1, 0xC($sp)
    /* 92FC 800086FC AFA60010 */  sw         $a2, 0x10($sp)
    /* 9300 80008700 AFA70014 */  sw         $a3, 0x14($sp)
    /* 9304 80008704 03E00008 */  jr         $ra
    /* 9308 80008708 27BD0008 */   addiu     $sp, $sp, 0x8