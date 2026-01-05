# Source: game_code.bin (decompressed)
# Address: 0x80094F88

glabel input_new_data_wrapper
    /* 80094F88 27BDFFE8 */  addiu $sp, $sp, -24
    /* 80094F8C AFBF0014 */  sw $ra, 20($sp)
    /* 80094F90 8083001A */  lb $v1, 26($a0)
    /* 80094F94 50A30007 */  .word 0x50A30007
    /* 80094F98 8FBF0014 */  lw $ra, 20($sp)
    /* 80094F9C A085001A */  sb $a1, 26($a0)
    /* 80094FA0 0C0253B2 */  jal 0x80094EC8
    /* 80094FA4 AFA40018 */  sw $a0, 24($sp)
    /* 80094FA8 8FA40018 */  lw $a0, 24($sp)
    /* 80094FAC 8083001A */  lb $v1, 26($a0)
    /* 80094FB0 8FBF0014 */  lw $ra, 20($sp)
    /* 80094FB4 27BD0018 */  addiu $sp, $sp, 24
    /* 80094FB8 00601025 */  or $v0, $v1, $zero
    /* 80094FBC 03E00008 */  jr $ra
    /* 80094FC0 00000000 */  nop
