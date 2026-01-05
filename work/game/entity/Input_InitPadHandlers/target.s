# Source: game_code.bin (decompressed)
# Address: 0x80094E8C

glabel Input_InitPadHandlers
    /* 80094E8C 25EF0BF0 */  addiu $t7, $t7, 3056
    /* 80094E90 00047140 */  sll $t6, $a0, 5
    /* 80094E94 01CF1021 */  addu $v0, $t6, $t7
    /* 80094E98 8FB80010 */  lw $t8, 16($sp)
    /* 80094E9C 8FB90014 */  lw $t9, 20($sp)
    /* 80094EA0 8FA80018 */  lw $t0, 24($sp)
    /* 80094EA4 8FA9001C */  lw $t1, 28($sp)
    /* 80094EA8 AC460004 */  sw $a2, 4($v0)
    /* 80094EAC AC470000 */  sw $a3, 0($v0)
    /* 80094EB0 A4450008 */  sh $a1, 8($v0)
    /* 80094EB4 A458000A */  sh $t8, 10($v0)
    /* 80094EB8 A459000C */  sh $t9, 12($v0)
    /* 80094EBC A4480010 */  sh $t0, 16($v0)
    /* 80094EC0 03E00008 */  jr $ra
    /* 80094EC4 A4490012 */  sh $t1, 18($v0)
