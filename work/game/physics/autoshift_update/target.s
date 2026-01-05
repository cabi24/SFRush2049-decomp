# Source: game_code.bin (decompressed)
# Address: 0x800ABB04

glabel autoshift_update
    /* 800ABB04 0320F809 */  jalr $ra, $t9
    /* 800ABB08 00000000 */  nop
    /* 800ABB0C 1000000F */  beq $zero, $zero, 0x800ABB4C
    /* 800ABB10 8FBF0014 */  lw $ra, 20($sp)
    /* 800ABB14 3C0F8012 */  lui $t7, 0x8012
    /* 800ABB18 25EF8DFC */  addiu $t7, $t7, -29188
    /* 800ABB1C 8DE10000 */  lw $at, 0($t7)
    /* 800ABB20 27A50040 */  addiu $a1, $sp, 64
    /* 800ABB24 26260044 */  addiu $a2, $s1, 68
    /* 800ABB28 ACA10000 */  sw $at, 0($a1)
    /* 800ABB2C 8DF80004 */  lw $t8, 4($t7)
    /* 800ABB30 26270014 */  addiu $a3, $s1, 20
    /* 800ABB34 ACB80004 */  sw $t8, 4($a1)
    /* 800ABB38 8DE10008 */  lw $at, 8($t7)
    /* 800ABB3C ACA10008 */  sw $at, 8($a1)
    /* 800ABB40 0C02ADD4 */  jal 0x800AB750
    /* 800ABB44 82240065 */  lb $a0, 101($s1)
    /* 800ABB48 8FBF0014 */  lw $ra, 20($sp)
    /* 800ABB4C 27BD0060 */  addiu $sp, $sp, 96
    /* 800ABB50 03E00008 */  jr $ra
    /* 800ABB54 00000000 */  nop
