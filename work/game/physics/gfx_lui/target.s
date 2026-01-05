# Source: game_code.bin (decompressed)
# Address: 0x800A1D38

glabel gfx_lui
    /* 800A1D38 01D87821 */  addu $t7, $t6, $t8
    /* 800A1D3C AD4F0000 */  sw $t7, 0($t2)
    /* 800A1D40 10000007 */  beq $zero, $zero, 0x800A1D60
    /* 800A1D44 ACAD0000 */  sw $t5, 0($a1)
    /* 800A1D48 10000005 */  beq $zero, $zero, 0x800A1D60
    /* 800A1D4C ACAE0000 */  sw $t6, 0($a1)
    /* 800A1D50 51000004 */  .word 0x51000004
    /* 800A1D54 24840001 */  addiu $a0, $a0, 1
    /* 800A1D58 10000004 */  beq $zero, $zero, 0x800A1D6C
    /* 800A1D5C 24020001 */  addiu $v0, $zero, 1
    /* 800A1D60 24840001 */  addiu $a0, $a0, 1
    /* 800A1D64 5490FFE7 */  .word 0x5490FFE7
    /* 800A1D68 90EE0000 */  lbu $t6, 0($a3)
    /* 800A1D6C 54400013 */  .word 0x54400013
    /* 800A1D70 8CAF0000 */  lw $t7, 0($a1)
    /* 800A1D74 906E0011 */  lbu $t6, 17($v1)
    /* 800A1D78 90790010 */  lbu $t9, 16($v1)
    /* 800A1D7C 24C60001 */  addiu $a2, $a2, 1
    /* 800A1D80 01CC0019 */  multu $t6, $t4
    /* 800A1D84 0019C080 */  sll $t8, $t9, 2
    /* 800A1D88 0319C023 */  subu $t8, $t8, $t9
    /* 800A1D8C 0018C180 */  sll $t8, $t8, 6
    /* 800A1D90 0319C021 */  addu $t8, $t8, $t9
    /* 800A1D94 0018C080 */  sll $t8, $t8, 2
    /* 800A1D98 01787821 */  addu $t7, $t3, $t8
    /* 800A1D9C 24E70001 */  addiu $a3, $a3, 1
    /* 800A1DA0 0000C812 */  mflo $t9
    /* 800A1DA4 01F9C021 */  addu $t8, $t7, $t9
    /* 800A1DA8 970E0088 */  lhu $t6, 136($t8)
    /* 800A1DAC 00CE082B */  sltu $at, $a2, $t6
    /* 800A1DB0 5420FFD0 */  .word 0x5420FFD0
    /* 800A1DB4 90F80000 */  lbu $t8, 0($a3)
    /* 800A1DB8 8CAF0000 */  lw $t7, 0($a1)
    /* 800A1DBC 000FC940 */  sll $t9, $t7, 5
    /* 800A1DC0 2F380001 */  sltiu $t8, $t9, 1
    /* 800A1DC4 ACB90000 */  sw $t9, 0($a1)
    /* 800A1DC8 3B020001 */  xori $v0, $t8, 0x0001
    /* 800A1DCC 03E00008 */  jr $ra
    /* 800A1DD0 00000000 */  nop
