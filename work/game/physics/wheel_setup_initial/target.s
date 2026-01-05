# Source: game_code.bin (decompressed)
# Address: 0x800A4AC4

glabel wheel_setup_initial
    /* 800A4AC4 27BDFFD0 */  addiu $sp, $sp, -48
    /* 800A4AC8 AFB00018 */  sw $s0, 24($sp)
    /* 800A4ACC AFB30024 */  sw $s3, 36($sp)
    /* 800A4AD0 AFB20020 */  sw $s2, 32($sp)
    /* 800A4AD4 AFB1001C */  sw $s1, 28($sp)
    /* 800A4AD8 3C108015 */  lui $s0, 0x8015
    /* 800A4ADC 00809025 */  or $s2, $a0, $zero
    /* 800A4AE0 AFBF002C */  sw $ra, 44($sp)
    /* 800A4AE4 AFB40028 */  sw $s4, 40($sp)
    /* 800A4AE8 26106D38 */  addiu $s0, $s0, 27960
    /* 800A4AEC 00008825 */  or $s1, $zero, $zero
    /* 800A4AF0 24130040 */  addiu $s3, $zero, 64
    /* 800A4AF4 8E0E000C */  lw $t6, 12($s0)
    /* 800A4AF8 51C00007 */  .word 0x51C00007
    /* 800A4AFC 26310001 */  addiu $s1, $s1, 1
    /* 800A4B00 920F0006 */  lbu $t7, 6($s0)
    /* 800A4B04 564F0004 */  .word 0x564F0004
    /* 800A4B08 26310001 */  addiu $s1, $s1, 1
    /* 800A4B0C 0C02584C */  jal 0x80096130
    /* 800A4B10 02202025 */  or $a0, $s1, $zero
    /* 800A4B14 26310001 */  addiu $s1, $s1, 1
    /* 800A4B18 1633FFF6 */  bne $s1, $s3, 0x800A4AF4
    /* 800A4B1C 26100014 */  addiu $s0, $s0, 20
    /* 800A4B20 8FBF002C */  lw $ra, 44($sp)
    /* 800A4B24 8FB00018 */  lw $s0, 24($sp)
    /* 800A4B28 8FB1001C */  lw $s1, 28($sp)
    /* 800A4B2C 8FB20020 */  lw $s2, 32($sp)
    /* 800A4B30 8FB30024 */  lw $s3, 36($sp)
    /* 800A4B34 8FB40028 */  lw $s4, 40($sp)
    /* 800A4B38 03E00008 */  jr $ra
    /* 800A4B3C 27BD0030 */  addiu $sp, $sp, 48
