# Source: game_code.bin (decompressed)
# Address: 0x80095380

glabel poll_input
    /* 80095380 932E001A */  lbu $t6, 26($t9)
    /* 80095384 25CF0001 */  addiu $t7, $t6, 1
    /* 80095388 A32F001A */  sb $t7, 26($t9)
    /* 8009538C 8FBF0014 */  lw $ra, 20($sp)
    /* 80095390 27BD0018 */  addiu $sp, $sp, 24
    /* 80095394 03E00008 */  jr $ra
    /* 80095398 00000000 */  nop
