# Source: game_code.bin (decompressed)
# Address: 0x80095DE0

glabel particles_animate
    /* 80095DE0 01C27825 */  or $t7, $t6, $v0
    /* 80095DE4 0324C021 */  addu $t8, $t9, $a0
    /* 80095DE8 AF0F0008 */  sw $t7, 8($t8)
    /* 80095DEC 8CE60000 */  lw $a2, 0($a3)
    /* 80095DF0 240E0001 */  addiu $t6, $zero, 1
    /* 80095DF4 00C1082B */  sltu $at, $a2, $at
    /* 80095DF8 14200003 */  bne $at, $zero, 0x80095E08
    /* 80095DFC 24D90001 */  addiu $t9, $a2, 1
    /* 80095E00 10000002 */  beq $zero, $zero, 0x80095E0C
    /* 80095E04 ACEE0000 */  sw $t6, 0($a3)
    /* 80095E08 ACF90000 */  sw $t9, 0($a3)
    /* 80095E0C 8CAF0000 */  lw $t7, 0($a1)
    /* 80095E10 24020001 */  addiu $v0, $zero, 1
    /* 80095E14 008FC021 */  addu $t8, $a0, $t7
    /* 80095E18 10000007 */  beq $zero, $zero, 0x80095E38
    /* 80095E1C AFB8004C */  sw $t8, 76($sp)
    /* 80095E20 24420001 */  addiu $v0, $v0, 1
    /* 80095E24 0045082B */  sltu $at, $v0, $a1
    /* 80095E28 24840018 */  addiu $a0, $a0, 24
    /* 80095E2C 1420FFCB */  bne $at, $zero, 0x80095D5C
    /* 80095E30 24630018 */  addiu $v1, $v1, 24
    /* 80095E34 00001025 */  or $v0, $zero, $zero
    /* 80095E38 14400003 */  bne $v0, $zero, 0x80095E48
    /* 80095E3C 3C048014 */  lui $a0, 0x8014
    /* 80095E40 10000015 */  beq $zero, $zero, 0x80095E98
    /* 80095E44 2402FFFF */  addiu $v0, $zero, -1
    /* 80095E48 24846138 */  addiu $a0, $a0, 24888
    /* 80095E4C 8C85000C */  lw $a1, 12($a0)
    /* 80095E50 A0A00009 */  sb $zero, 9($a1)
    /* 80095E54 AFA80050 */  sw $t0, 80($sp)
    /* 80095E58 0C024847 */  jal 0x8009211C
    /* 80095E5C AFA50034 */  sw $a1, 52($sp)
    /* 80095E60 8FB40034 */  lw $s4, 52($sp)
    /* 80095E64 8FA80050 */  lw $t0, 80($sp)
    /* 80095E68 A2800008 */  sb $zero, 8($s4)
    /* 80095E6C 8FAE004C */  lw $t6, 76($sp)
    /* 80095E70 AE8E000C */  sw $t6, 12($s4)
    /* 80095E74 8FB9004C */  lw $t9, 76($sp)
    /* 80095E78 AF28000C */  sw $t0, 12($t9)
    /* 80095E7C C7A40054 */  lwc1 $f4, 84($sp)
    /* 80095E80 E6840010 */  swc1 $f4, 16($s4)
    /* 80095E84 C7A60058 */  lwc1 $f6, 88($sp)
    /* 80095E88 0C0254F3 */  jal 0x800953CC
    /* 80095E8C E6860014 */  swc1 $f6, 20($s4)
    /* 80095E90 8FAF004C */  lw $t7, 76($sp)
    /* 80095E94 8DE20008 */  lw $v0, 8($t7)
    /* 80095E98 8FBF002C */  lw $ra, 44($sp)
    /* 80095E9C 8FB00018 */  lw $s0, 24($sp)
    /* 80095EA0 8FB1001C */  lw $s1, 28($sp)
    /* 80095EA4 8FB20020 */  lw $s2, 32($sp)
    /* 80095EA8 8FB30024 */  lw $s3, 36($sp)
    /* 80095EAC 8FB40028 */  lw $s4, 40($sp)
    /* 80095EB0 03E00008 */  jr $ra
    /* 80095EB4 27BD0050 */  addiu $sp, $sp, 80
