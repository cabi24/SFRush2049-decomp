# Source: game_code.bin (decompressed)
# Address: 0x800BE7BC

glabel race_position_update
    /* 800BE7BC 27BDFF88 */  addiu $sp, $sp, -120
    /* 800BE7C0 AFB1001C */  sw $s1, 28($sp)
    /* 800BE7C4 00808825 */  or $s1, $a0, $zero
    /* 800BE7C8 AFB00018 */  sw $s0, 24($sp)
    /* 800BE7CC 00A08025 */  or $s0, $a1, $zero
    /* 800BE7D0 AFBF0034 */  sw $ra, 52($sp)
    /* 800BE7D4 00A02025 */  or $a0, $a1, $zero
    /* 800BE7D8 AFB60030 */  sw $s6, 48($sp)
    /* 800BE7DC AFB5002C */  sw $s5, 44($sp)
    /* 800BE7E0 AFB40028 */  sw $s4, 40($sp)
    /* 800BE7E4 AFB30024 */  sw $s3, 36($sp)
    /* 800BE7E8 AFB20020 */  sw $s2, 32($sp)
    /* 800BE7EC AFA60080 */  sw $a2, 128($sp)
    /* 800BE7F0 0C02CFE9 */  jal 0x800B3FA4
    /* 800BE7F4 2405FFFF */  addiu $a1, $zero, -1
    /* 800BE7F8 87AE0082 */  lh $t6, 130($sp)
    /* 800BE7FC 00403025 */  or $a2, $v0, $zero
    /* 800BE800 02002825 */  or $a1, $s0, $zero
    /* 800BE804 01C2082A */  slt $at, $t6, $v0
    /* 800BE808 14200005 */  bne $at, $zero, 0x800BE820
    /* 800BE80C 02202025 */  or $a0, $s1, $zero
    /* 800BE810 0C02F9A9 */  jal 0x800BE6A4
    /* 800BE814 02202025 */  or $a0, $s1, $zero
    /* 800BE818 10000058 */  beq $zero, $zero, 0x800BE97C
    /* 800BE81C 8FBF0034 */  lw $ra, 52($sp)
    /* 800BE820 92360000 */  lbu $s6, 0($s1)
    /* 800BE824 AFA6005C */  sw $a2, 92($sp)
    /* 800BE828 02002825 */  or $a1, $s0, $zero
    /* 800BE82C 3ACF00FF */  xori $t7, $s6, 0x00FF
    /* 800BE830 0C02F9A9 */  jal 0x800BE6A4
    /* 800BE834 2DF60001 */  sltiu $s6, $t7, 1
    /* 800BE838 0C02F9D1 */  jal 0x800BE744
    /* 800BE83C 02202025 */  or $a0, $s1, $zero
    /* 800BE840 28410002 */  slti $at, $v0, 2
    /* 800BE844 1420004C */  bne $at, $zero, 0x800BE978
    /* 800BE848 8FA6005C */  lw $a2, 92($sp)
    /* 800BE84C 12C00018 */  beq $s6, $zero, 0x800BE8B0
    /* 800BE850 2450FFFE */  addiu $s0, $v0, -2
    /* 800BE854 241800FF */  addiu $t8, $zero, 255
    /* 800BE858 3C058012 */  lui $a1, 0x8012
    /* 800BE85C A3B80060 */  sb $t8, 96($sp)
    /* 800BE860 24A51DE8 */  addiu $a1, $a1, 7656
    /* 800BE864 27A40060 */  addiu $a0, $sp, 96
    /* 800BE868 0C02F93C */  jal 0x800BE4F0
    /* 800BE86C AFA6005C */  sw $a2, 92($sp)
    /* 800BE870 0010C840 */  sll $t9, $s0, 1
    /* 800BE874 03311821 */  addu $v1, $t9, $s1
    /* 800BE878 90680001 */  lbu $t0, 1($v1)
    /* 800BE87C 8FA6005C */  lw $a2, 92($sp)
    /* 800BE880 A3A80067 */  sb $t0, 103($sp)
    /* 800BE884 90690002 */  lbu $t1, 2($v1)
    /* 800BE888 A3A90068 */  sb $t1, 104($sp)
    /* 800BE88C 906A0003 */  lbu $t2, 3($v1)
    /* 800BE890 A3AA0069 */  sb $t2, 105($sp)
    /* 800BE894 906B0004 */  lbu $t3, 4($v1)
    /* 800BE898 A3A0006B */  sb $zero, 107($sp)
    /* 800BE89C A3A0006C */  sb $zero, 108($sp)
    /* 800BE8A0 A3AB006A */  sb $t3, 106($sp)
    /* 800BE8A4 A0600001 */  sb $zero, 1($v1)
    /* 800BE8A8 1000000C */  beq $zero, $zero, 0x800BE8DC
    /* 800BE8AC A0600002 */  sb $zero, 2($v1)
    /* 800BE8B0 3C058012 */  lui $a1, 0x8012
    /* 800BE8B4 24A51DE8 */  addiu $a1, $a1, 7656
    /* 800BE8B8 27A40060 */  addiu $a0, $sp, 96
    /* 800BE8BC 0C02F9A9 */  jal 0x800BE6A4
    /* 800BE8C0 AFA6005C */  sw $a2, 92($sp)
    /* 800BE8C4 02309021 */  addu $s2, $s1, $s0
    /* 800BE8C8 02402825 */  or $a1, $s2, $zero
    /* 800BE8CC 0C02F93C */  jal 0x800BE4F0
    /* 800BE8D0 27A40060 */  addiu $a0, $sp, 96
    /* 800BE8D4 8FA6005C */  lw $a2, 92($sp)
    /* 800BE8D8 A2400000 */  sb $zero, 0($s2)
    /* 800BE8DC 27A40060 */  addiu $a0, $sp, 96
    /* 800BE8E0 2405FFFF */  addiu $a1, $zero, -1
    /* 800BE8E4 0C02CFE9 */  jal 0x800B3FA4
    /* 800BE8E8 AFA6005C */  sw $a2, 92($sp)
    /* 800BE8EC 3C138015 */  lui $s3, 0x8015
    /* 800BE8F0 87AC0082 */  lh $t4, 130($sp)
    /* 800BE8F4 00401825 */  or $v1, $v0, $zero
    /* 800BE8F8 26739B70 */  addiu $s3, $s3, -25744
    /* 800BE8FC 82620000 */  lb $v0, 0($s3)
    /* 800BE900 8FA6005C */  lw $a2, 92($sp)
    /* 800BE904 01839023 */  subu $s2, $t4, $v1
    /* 800BE908 02426823 */  subu $t5, $s2, $v0
    /* 800BE90C 01A6082A */  slt $at, $t5, $a2
    /* 800BE910 10200016 */  beq $at, $zero, 0x800BE96C
    /* 800BE914 2414FFFF */  addiu $s4, $zero, -1
    /* 800BE918 12C00006 */  beq $s6, $zero, 0x800BE934
    /* 800BE91C 2610FFFF */  addiu $s0, $s0, -1
    /* 800BE920 00107040 */  sll $t6, $s0, 1
    /* 800BE924 022E1821 */  addu $v1, $s1, $t6
    /* 800BE928 A0600001 */  sb $zero, 1($v1)
    /* 800BE92C 10000003 */  beq $zero, $zero, 0x800BE93C
    /* 800BE930 A0600002 */  sb $zero, 2($v1)
    /* 800BE934 02307821 */  addu $t7, $s1, $s0
    /* 800BE938 A1E00000 */  sb $zero, 0($t7)
    /* 800BE93C 1200000B */  beq $s0, $zero, 0x800BE96C
    /* 800BE940 02202025 */  or $a0, $s1, $zero
    /* 800BE944 00142C00 */  sll $a1, $s4, 16
    /* 800BE948 0005C403 */  sra $t8, $a1, 16
    /* 800BE94C 0C02CFE9 */  jal 0x800B3FA4
    /* 800BE950 03002825 */  or $a1, $t8, $zero
    /* 800BE954 00403025 */  or $a2, $v0, $zero
    /* 800BE958 82620000 */  lb $v0, 0($s3)
    /* 800BE95C 0242C823 */  subu $t9, $s2, $v0
    /* 800BE960 0326082A */  slt $at, $t9, $a2
    /* 800BE964 1420FFEC */  bne $at, $zero, 0x800BE918
    /* 800BE968 00000000 */  nop
    /* 800BE96C 02202025 */  or $a0, $s1, $zero
    /* 800BE970 0C02F93C */  jal 0x800BE4F0
    /* 800BE974 27A50060 */  addiu $a1, $sp, 96
    /* 800BE978 8FBF0034 */  lw $ra, 52($sp)
    /* 800BE97C 8FB00018 */  lw $s0, 24($sp)
    /* 800BE980 8FB1001C */  lw $s1, 28($sp)
    /* 800BE984 8FB20020 */  lw $s2, 32($sp)
    /* 800BE988 8FB30024 */  lw $s3, 36($sp)
    /* 800BE98C 8FB40028 */  lw $s4, 40($sp)
    /* 800BE990 8FB5002C */  lw $s5, 44($sp)
    /* 800BE994 8FB60030 */  lw $s6, 48($sp)
    /* 800BE998 03E00008 */  jr $ra
    /* 800BE99C 27BD0078 */  addiu $sp, $sp, 120
