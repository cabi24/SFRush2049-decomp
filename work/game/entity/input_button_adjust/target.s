# Source: game_code.bin (decompressed)
# Address: 0x80094D68

glabel input_button_adjust
    /* 80094D68 10A00009 */  beq $a1, $zero, 0x80094D90
    /* 80094D6C 00044140 */  sll $t0, $a0, 5
    /* 80094D70 3C0F8014 */  lui $t7, 0x8014
    /* 80094D74 25EF0BF0 */  addiu $t7, $t7, 3056
    /* 80094D78 00047140 */  sll $t6, $a0, 5
    /* 80094D7C 01CF1021 */  addu $v0, $t6, $t7
    /* 80094D80 90580015 */  lbu $t8, 21($v0)
    /* 80094D84 37190004 */  ori $t9, $t8, 0x0004
    /* 80094D88 03E00008 */  jr $ra
    /* 80094D8C A0590015 */  sb $t9, 21($v0)
