# Source: game_code.bin (decompressed)
# Address: 0x8008D8D8

glabel matrix_scale_apply
    /* 8008D8D8 27BDFFE8 */  addiu $sp, $sp, -24
    /* 8008D8DC 04E10005 */  bgez $a3, 0x8008D8F4
    /* 8008D8E0 AFBF0014 */  sw $ra, 20($sp)
    /* 8008D8E4 0C022C9B */  jal 0x8008B26C
    /* 8008D8E8 01003825 */  or $a3, $t0, $zero
    /* 8008D8EC 10000010 */  beq $zero, $zero, 0x8008D930
    /* 8008D8F0 8FBF0014 */  lw $ra, 20($sp)
    /* 8008D8F4 12000008 */  beq $s0, $zero, 0x8008D918
    /* 8008D8F8 24050001 */  addiu $a1, $zero, 1
    /* 8008D8FC 240E0001 */  addiu $t6, $zero, 1
    /* 8008D900 00EE3004 */  .word 0x00EE3004
    /* 8008D904 85040006 */  lh $a0, 6($t0)
    /* 8008D908 0C022C36 */  jal 0x8008B0D8
    /* 8008D90C 00002825 */  or $a1, $zero, $zero
    /* 8008D910 10000006 */  beq $zero, $zero, 0x8008D92C
    /* 8008D914 02001025 */  or $v0, $s0, $zero
    /* 8008D918 240F0001 */  addiu $t7, $zero, 1
    /* 8008D91C 00EF3004 */  .word 0x00EF3004
    /* 8008D920 0C022BA3 */  jal 0x8008AE8C
    /* 8008D924 85040006 */  lh $a0, 6($t0)
    /* 8008D928 02001025 */  or $v0, $s0, $zero
    /* 8008D92C 8FBF0014 */  lw $ra, 20($sp)
    /* 8008D930 27BD0018 */  addiu $sp, $sp, 24
    /* 8008D934 03E00008 */  jr $ra
    /* 8008D938 00000000 */  nop
