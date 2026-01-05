# Source: game_code.bin (decompressed)
# Address: 0x80094D20

glabel input_connected_check
    /* 80094D20 10A00009 */  beq $a1, $zero, 0x80094D48
    /* 80094D24 00044140 */  sll $t0, $a0, 5
    /* 80094D28 3C0F8014 */  lui $t7, 0x8014
    /* 80094D2C 25EF0BF0 */  addiu $t7, $t7, 3056
    /* 80094D30 00047140 */  sll $t6, $a0, 5
    /* 80094D34 01CF1021 */  addu $v0, $t6, $t7
    /* 80094D38 90580015 */  lbu $t8, 21($v0)
    /* 80094D3C 37190080 */  ori $t9, $t8, 0x0080
    /* 80094D40 03E00008 */  jr $ra
    /* 80094D44 A0590015 */  sb $t9, 21($v0)
