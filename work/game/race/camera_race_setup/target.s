# Source: game_code.bin (decompressed)
# Address: 0x800F8D9C

glabel camera_race_setup
    /* 800F8D9C 27BDFFC8 */  addiu $sp, $sp, -56
    /* 800F8DA0 3C038015 */  lui $v1, 0x8015
    /* 800F8DA4 80632744 */  lb $v1, 10052($v1)
    /* 800F8DA8 AFB20020 */  sw $s2, 32($sp)
    /* 800F8DAC AFBF0034 */  sw $ra, 52($sp)
    /* 800F8DB0 AFB60030 */  sw $s6, 48($sp)
    /* 800F8DB4 AFB5002C */  sw $s5, 44($sp)
    /* 800F8DB8 AFB40028 */  sw $s4, 40($sp)
    /* 800F8DBC AFB30024 */  sw $s3, 36($sp)
    /* 800F8DC0 AFB1001C */  sw $s1, 28($sp)
    /* 800F8DC4 AFB00018 */  sw $s0, 24($sp)
    /* 800F8DC8 18600027 */  blez $v1, 0x800F8E68
    /* 800F8DCC 00009025 */  or $s2, $zero, $zero
    /* 800F8DD0 3C158014 */  lui $s5, 0x8014
    /* 800F8DD4 3C138015 */  lui $s3, 0x8015
    /* 800F8DD8 2673A250 */  addiu $s3, $s3, -23984
    /* 800F8DDC 26B539F8 */  addiu $s5, $s5, 14840
    /* 800F8DE0 24160001 */  addiu $s6, $zero, 1
    /* 800F8DE4 24140808 */  addiu $s4, $zero, 2056
    /* 800F8DE8 02540019 */  multu $s2, $s4
    /* 800F8DEC 00007012 */  mflo $t6
    /* 800F8DF0 026E7821 */  addu $t7, $s3, $t6
    /* 800F8DF4 85F007C6 */  lh $s0, 1990($t7)
    /* 800F8DF8 02140019 */  multu $s0, $s4
    /* 800F8DFC 02B08821 */  addu $s1, $s5, $s0
    /* 800F8E00 0000C012 */  mflo $t8
    /* 800F8E04 02781021 */  addu $v0, $s3, $t8
    /* 800F8E08 805907DD */  lb $t9, 2013($v0)
    /* 800F8E0C 53200011 */  .word 0x53200011
    /* 800F8E10 26520001 */  addiu $s2, $s2, 1
    /* 800F8E14 82280000 */  lb $t0, 0($s1)
    /* 800F8E18 00102400 */  sll $a0, $s0, 16
    /* 800F8E1C 00044C03 */  sra $t1, $a0, 16
    /* 800F8E20 15000004 */  bne $t0, $zero, 0x800F8E34
    /* 800F8E24 01202025 */  or $a0, $t1, $zero
    /* 800F8E28 90450008 */  lbu $a1, 8($v0)
    /* 800F8E2C 0C02C803 */  jal 0x800B200C
    /* 800F8E30 02C03025 */  or $a2, $s6, $zero
    /* 800F8E34 00102400 */  sll $a0, $s0, 16
    /* 800F8E38 00045403 */  sra $t2, $a0, 16
    /* 800F8E3C A2360000 */  sb $s6, 0($s1)
    /* 800F8E40 0C02C060 */  jal 0x800B0180
    /* 800F8E44 01402025 */  or $a0, $t2, $zero
    /* 800F8E48 3C038015 */  lui $v1, 0x8015
    /* 800F8E4C 80632744 */  lb $v1, 10052($v1)
    /* 800F8E50 26520001 */  addiu $s2, $s2, 1
    /* 800F8E54 00125C00 */  sll $t3, $s2, 16
    /* 800F8E58 000B9403 */  sra $s2, $t3, 16
    /* 800F8E5C 0243082A */  slt $at, $s2, $v1
    /* 800F8E60 1420FFE1 */  bne $at, $zero, 0x800F8DE8
    /* 800F8E64 00000000 */  nop
    /* 800F8E68 8FBF0034 */  lw $ra, 52($sp)
    /* 800F8E6C 8FB00018 */  lw $s0, 24($sp)
    /* 800F8E70 8FB1001C */  lw $s1, 28($sp)
    /* 800F8E74 8FB20020 */  lw $s2, 32($sp)
    /* 800F8E78 8FB30024 */  lw $s3, 36($sp)
    /* 800F8E7C 8FB40028 */  lw $s4, 40($sp)
    /* 800F8E80 8FB5002C */  lw $s5, 44($sp)
    /* 800F8E84 8FB60030 */  lw $s6, 48($sp)
    /* 800F8E88 03E00008 */  jr $ra
    /* 800F8E8C 27BD0038 */  addiu $sp, $sp, 56
