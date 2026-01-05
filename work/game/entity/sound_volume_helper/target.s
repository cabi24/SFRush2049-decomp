# Source: game_code.bin (decompressed)
# Address: 0x8009515C

glabel sound_volume_helper
    /* 8009515C 27BDFFE8 */  addiu $sp, $sp, -24
    /* 80095160 AFBF0014 */  sw $ra, 20($sp)
    /* 80095164 000E7980 */  sll $t7, $t6, 6
    /* 80095168 05E30007 */  .word 0x05E30007
    /* 8009516C 8FBF0014 */  lw $ra, 20($sp)
    /* 80095170 8098001A */  lb $t8, 26($a0)
    /* 80095174 53000004 */  .word 0x53000004
    /* 80095178 8FBF0014 */  lw $ra, 20($sp)
    /* 8009517C 0C0253B2 */  jal 0x80094EC8
    /* 80095180 A080001A */  sb $zero, 26($a0)
    /* 80095184 8FBF0014 */  lw $ra, 20($sp)
    /* 80095188 27BD0018 */  addiu $sp, $sp, 24
    /* 8009518C 24020001 */  addiu $v0, $zero, 1
    /* 80095190 03E00008 */  jr $ra
    /* 80095194 00000000 */  nop
