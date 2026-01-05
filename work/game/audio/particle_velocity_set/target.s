# Source: game_code.bin (decompressed)
# Address: 0x800B82C8

glabel particle_velocity_set
    /* 800B82C8 27BDFFD8 */  addiu $sp, $sp, -40
    /* 800B82CC AFB30020 */  sw $s3, 32($sp)
    /* 800B82D0 AFB2001C */  sw $s2, 28($sp)
    /* 800B82D4 AFB10018 */  sw $s1, 24($sp)
    /* 800B82D8 AFB00014 */  sw $s0, 20($sp)
    /* 800B82DC AFBF0024 */  sw $ra, 36($sp)
    /* 800B82E0 00008025 */  or $s0, $zero, $zero
    /* 800B82E4 24110004 */  addiu $s1, $zero, 4
    /* 800B82E8 24120001 */  addiu $s2, $zero, 1
    /* 800B82EC 24130002 */  addiu $s3, $zero, 2
    /* 800B82F0 16000004 */  bne $s0, $zero, 0x800B8304
    /* 800B82F4 00122400 */  sll $a0, $s2, 16
    /* 800B82F8 00047403 */  sra $t6, $a0, 16
    /* 800B82FC 1000000D */  beq $zero, $zero, 0x800B8334
    /* 800B8300 01C02025 */  or $a0, $t6, $zero
    /* 800B8304 16120003 */  bne $s0, $s2, 0x800B8314
    /* 800B8308 00000000 */  nop
    /* 800B830C 10000006 */  beq $zero, $zero, 0x800B8328
    /* 800B8310 02601025 */  or $v0, $s3, $zero
    /* 800B8314 16130003 */  bne $s0, $s3, 0x800B8324
    /* 800B8318 24030008 */  addiu $v1, $zero, 8
    /* 800B831C 10000001 */  beq $zero, $zero, 0x800B8324
    /* 800B8320 02201825 */  or $v1, $s1, $zero
    /* 800B8324 00601025 */  or $v0, $v1, $zero
    /* 800B8328 00022400 */  sll $a0, $v0, 16
    /* 800B832C 00047C03 */  sra $t7, $a0, 16
    /* 800B8330 01E02025 */  or $a0, $t7, $zero
    /* 800B8334 0C02E087 */  jal 0x800B821C
    /* 800B8338 00000000 */  nop
    /* 800B833C 26100001 */  addiu $s0, $s0, 1
    /* 800B8340 1611FFEB */  bne $s0, $s1, 0x800B82F0
    /* 800B8344 00000000 */  nop
    /* 800B8348 0C02E047 */  jal 0x800B811C
    /* 800B834C 00000000 */  nop
    /* 800B8350 8FBF0024 */  lw $ra, 36($sp)
    /* 800B8354 8FB00014 */  lw $s0, 20($sp)
    /* 800B8358 8FB10018 */  lw $s1, 24($sp)
    /* 800B835C 8FB2001C */  lw $s2, 28($sp)
    /* 800B8360 8FB30020 */  lw $s3, 32($sp)
    /* 800B8364 03E00008 */  jr $ra
    /* 800B8368 27BD0028 */  addiu $sp, $sp, 40
