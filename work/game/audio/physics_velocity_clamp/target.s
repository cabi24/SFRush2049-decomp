# Source: game_code.bin (decompressed)
# Address: 0x800B9A0C

glabel physics_velocity_clamp
    /* 800B9A0C 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800B9A10 AFB50028 */  sw $s5, 40($sp)
    /* 800B9A14 AFB40024 */  sw $s4, 36($sp)
    /* 800B9A18 3C148015 */  lui $s4, 0x8015
    /* 800B9A1C 3C158014 */  lui $s5, 0x8014
    /* 800B9A20 AFB30020 */  sw $s3, 32($sp)
    /* 800B9A24 AFB2001C */  sw $s2, 28($sp)
    /* 800B9A28 AFB10018 */  sw $s1, 24($sp)
    /* 800B9A2C AFB00014 */  sw $s0, 20($sp)
    /* 800B9A30 00A08025 */  or $s0, $a1, $zero
    /* 800B9A34 00C08825 */  or $s1, $a2, $zero
    /* 800B9A38 00809025 */  or $s2, $a0, $zero
    /* 800B9A3C 00E09825 */  or $s3, $a3, $zero
    /* 800B9A40 26B507F0 */  addiu $s5, $s5, 2032
    /* 800B9A44 26941CE8 */  addiu $s4, $s4, 7400
    /* 800B9A48 AFBF002C */  sw $ra, 44($sp)
    /* 800B9A4C 02402025 */  or $a0, $s2, $zero
    /* 800B9A50 0C02E636 */  jal 0x800B98D8
    /* 800B9A54 02602825 */  or $a1, $s3, $zero
    /* 800B9A58 14400003 */  bne $v0, $zero, 0x800B9A68
    /* 800B9A5C 0012C040 */  sll $t8, $s2, 1
    /* 800B9A60 10000037 */  beq $zero, $zero, 0x800B9B40
    /* 800B9A64 00001025 */  or $v0, $zero, $zero
    /* 800B9A68 06410013 */  bgez $s2, 0x800B9AB8
    /* 800B9A6C 3C198015 */  lui $t9, 0x8015
    /* 800B9A70 86860008 */  lh $a2, 8($s4)
    /* 800B9A74 3C028015 */  lui $v0, 0x8015
    /* 800B9A78 24421D38 */  addiu $v0, $v0, 7480
    /* 800B9A7C 28C10002 */  slti $at, $a2, 2
    /* 800B9A80 14200009 */  bne $at, $zero, 0x800B9AA8
    /* 800B9A84 24040001 */  addiu $a0, $zero, 1
    /* 800B9A88 844E002E */  lh $t6, 46($v0)
    /* 800B9A8C 020E082A */  slt $at, $s0, $t6
    /* 800B9A90 54200006 */  .word 0x54200006
    /* 800B9A94 248FFFFF */  addiu $t7, $a0, -1
    /* 800B9A98 24840001 */  addiu $a0, $a0, 1
    /* 800B9A9C 0086082A */  slt $at, $a0, $a2
    /* 800B9AA0 1420FFF9 */  bne $at, $zero, 0x800B9A88
    /* 800B9AA4 24420050 */  addiu $v0, $v0, 80
    /* 800B9AA8 248FFFFF */  addiu $t7, $a0, -1
    /* 800B9AAC AE2F0000 */  sw $t7, 0($s1)
    /* 800B9AB0 10000023 */  beq $zero, $zero, 0x800B9B40
    /* 800B9AB4 24020001 */  addiu $v0, $zero, 1
    /* 800B9AB8 86860008 */  lh $a2, 8($s4)
    /* 800B9ABC 2405FFFF */  addiu $a1, $zero, -1
    /* 800B9AC0 00002025 */  or $a0, $zero, $zero
    /* 800B9AC4 18C00012 */  blez $a2, 0x800B9B10
    /* 800B9AC8 27391CE8 */  addiu $t9, $t9, 7400
    /* 800B9ACC 03191821 */  addu $v1, $t8, $t9
    /* 800B9AD0 84620038 */  lh $v0, 56($v1)
    /* 800B9AD4 0442000B */  .word 0x0442000B
    /* 800B9AD8 24840001 */  addiu $a0, $a0, 1
    /* 800B9ADC 04A00002 */  bltz $a1, 0x800B9AE8
    /* 800B9AE0 00A2082A */  slt $at, $a1, $v0
    /* 800B9AE4 10200006 */  beq $at, $zero, 0x800B9B00
    /* 800B9AE8 0202082A */  slt $at, $s0, $v0
    /* 800B9AEC 54200005 */  .word 0x54200005
    /* 800B9AF0 24840001 */  addiu $a0, $a0, 1
    /* 800B9AF4 AE240000 */  sw $a0, 0($s1)
    /* 800B9AF8 86860008 */  lh $a2, 8($s4)
    /* 800B9AFC 84650038 */  lh $a1, 56($v1)
    /* 800B9B00 24840001 */  addiu $a0, $a0, 1
    /* 800B9B04 0086082A */  slt $at, $a0, $a2
    /* 800B9B08 1420FFF1 */  bne $at, $zero, 0x800B9AD0
    /* 800B9B0C 24630050 */  addiu $v1, $v1, 80
    /* 800B9B10 04A20004 */  .word 0x04A20004
    /* 800B9B14 8EA8000C */  lw $t0, 12($s5)
    /* 800B9B18 10000009 */  beq $zero, $zero, 0x800B9B40
    /* 800B9B1C 24020001 */  addiu $v0, $zero, 1
    /* 800B9B20 8EA8000C */  lw $t0, 12($s5)
    /* 800B9B24 00124900 */  sll $t1, $s2, 4
    /* 800B9B28 26730001 */  addiu $s3, $s3, 1
    /* 800B9B2C 01091021 */  addu $v0, $t0, $t1
    /* 800B9B30 80520001 */  lb $s2, 1($v0)
    /* 800B9B34 AFB20034 */  sw $s2, 52($sp)
    /* 800B9B38 1000FFC4 */  beq $zero, $zero, 0x800B9A4C
    /* 800B9B3C 94500002 */  lhu $s0, 2($v0)
    /* 800B9B40 8FBF002C */  lw $ra, 44($sp)
    /* 800B9B44 8FB00014 */  lw $s0, 20($sp)
    /* 800B9B48 8FB10018 */  lw $s1, 24($sp)
    /* 800B9B4C 8FB2001C */  lw $s2, 28($sp)
    /* 800B9B50 8FB30020 */  lw $s3, 32($sp)
    /* 800B9B54 8FB40024 */  lw $s4, 36($sp)
    /* 800B9B58 8FB50028 */  lw $s5, 40($sp)
    /* 800B9B5C 03E00008 */  jr $ra
    /* 800B9B60 27BD0040 */  addiu $sp, $sp, 64
