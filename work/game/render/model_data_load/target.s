# Source: game_code.bin (decompressed)
# Address: 0x8008AE8C

glabel model_data_load
    /* 8008AE8C 27BDFFD0 */  addiu $sp, $sp, -48
    /* 8008AE90 AFB10018 */  sw $s1, 24($sp)
    /* 8008AE94 3C118013 */  lui $s1, 0x8013
    /* 8008AE98 AFB40024 */  sw $s4, 36($sp)
    /* 8008AE9C AFB30020 */  sw $s3, 32($sp)
    /* 8008AEA0 AFB2001C */  sw $s2, 28($sp)
    /* 8008AEA4 24120044 */  addiu $s2, $zero, 68
    /* 8008AEA8 00C09825 */  or $s3, $a2, $zero
    /* 8008AEAC 2414FFFF */  addiu $s4, $zero, -1
    /* 8008AEB0 2631E700 */  addiu $s1, $s1, -6400
    /* 8008AEB4 AFBF002C */  sw $ra, 44($sp)
    /* 8008AEB8 AFB50028 */  sw $s5, 40($sp)
    /* 8008AEBC AFB00014 */  sw $s0, 20($sp)
    /* 8008AEC0 24030002 */  addiu $v1, $zero, 2
    /* 8008AEC4 14A30011 */  bne $a1, $v1, 0x8008AF0C
    /* 8008AEC8 00047400 */  sll $t6, $a0, 16
    /* 8008AECC 000E7C03 */  sra $t7, $t6, 16
    /* 8008AED0 01F20019 */  multu $t7, $s2
    /* 8008AED4 00134A00 */  sll $t1, $s3, 8
    /* 8008AED8 0000C012 */  mflo $t8
    /* 8008AEDC 0238C821 */  addu $t9, $s1, $t8
    /* 8008AEE0 8F220000 */  lw $v0, 0($t9)
    /* 8008AEE4 00920019 */  multu $a0, $s2
    /* 8008AEE8 00495025 */  or $t2, $v0, $t1
    /* 8008AEEC 00004012 */  mflo $t0
    /* 8008AEF0 02288021 */  addu $s0, $s1, $t0
    /* 8008AEF4 86020016 */  lh $v0, 22($s0)
    /* 8008AEF8 AE0A0000 */  sw $t2, 0($s0)
    /* 8008AEFC 10540037 */  beq $v0, $s4, 0x8008AFDC
    /* 8008AF00 00402025 */  or $a0, $v0, $zero
    /* 8008AF04 1000FFEF */  beq $zero, $zero, 0x8008AEC4
    /* 8008AF08 24050003 */  addiu $a1, $zero, 3
    /* 8008AF0C 14A0000E */  bne $a1, $zero, 0x8008AF48
    /* 8008AF10 24010001 */  addiu $at, $zero, 1
    /* 8008AF14 00045C00 */  sll $t3, $a0, 16
    /* 8008AF18 000B6403 */  sra $t4, $t3, 16
    /* 8008AF1C 01920019 */  multu $t4, $s2
    /* 8008AF20 3C018000 */  lui $at, 0x8000
    /* 8008AF24 00006812 */  mflo $t5
    /* 8008AF28 022D7021 */  addu $t6, $s1, $t5
    /* 8008AF2C 8DC20000 */  lw $v0, 0($t6)
    /* 8008AF30 00920019 */  multu $a0, $s2
    /* 8008AF34 00417825 */  or $t7, $v0, $at
    /* 8008AF38 0000C012 */  mflo $t8
    /* 8008AF3C 0238C821 */  addu $t9, $s1, $t8
    /* 8008AF40 10000026 */  beq $zero, $zero, 0x8008AFDC
    /* 8008AF44 AF2F0000 */  sw $t7, 0($t9)
    /* 8008AF48 14A1000D */  bne $a1, $at, 0x8008AF80
    /* 8008AF4C 00044400 */  sll $t0, $a0, 16
    /* 8008AF50 00084C03 */  sra $t1, $t0, 16
    /* 8008AF54 01320019 */  multu $t1, $s2
    /* 8008AF58 00136200 */  sll $t4, $s3, 8
    /* 8008AF5C 00005012 */  mflo $t2
    /* 8008AF60 022A5821 */  addu $t3, $s1, $t2
    /* 8008AF64 8D620000 */  lw $v0, 0($t3)
    /* 8008AF68 00920019 */  multu $a0, $s2
    /* 8008AF6C 004C6825 */  or $t5, $v0, $t4
    /* 8008AF70 00007012 */  mflo $t6
    /* 8008AF74 022EC021 */  addu $t8, $s1, $t6
    /* 8008AF78 10000018 */  beq $zero, $zero, 0x8008AFDC
    /* 8008AF7C AF0D0000 */  sw $t5, 0($t8)
    /* 8008AF80 24010003 */  addiu $at, $zero, 3
    /* 8008AF84 14A10015 */  bne $a1, $at, 0x8008AFDC
    /* 8008AF88 0013AA00 */  sll $s5, $s3, 8
    /* 8008AF8C 00047C00 */  sll $t7, $a0, 16
    /* 8008AF90 000FCC03 */  sra $t9, $t7, 16
    /* 8008AF94 03320019 */  multu $t9, $s2
    /* 8008AF98 24050003 */  addiu $a1, $zero, 3
    /* 8008AF9C 00004012 */  mflo $t0
    /* 8008AFA0 02284821 */  addu $t1, $s1, $t0
    /* 8008AFA4 8D220000 */  lw $v0, 0($t1)
    /* 8008AFA8 00920019 */  multu $a0, $s2
    /* 8008AFAC 00555825 */  or $t3, $v0, $s5
    /* 8008AFB0 00005012 */  mflo $t2
    /* 8008AFB4 022A8021 */  addu $s0, $s1, $t2
    /* 8008AFB8 86040016 */  lh $a0, 22($s0)
    /* 8008AFBC AE0B0000 */  sw $t3, 0($s0)
    /* 8008AFC0 50940004 */  .word 0x50940004
    /* 8008AFC4 86040018 */  lh $a0, 24($s0)
    /* 8008AFC8 0C022BA3 */  jal 0x8008AE8C
    /* 8008AFCC 02603025 */  or $a2, $s3, $zero
    /* 8008AFD0 86040018 */  lh $a0, 24($s0)
    /* 8008AFD4 5494FFEE */  .word 0x5494FFEE
    /* 8008AFD8 00047C00 */  sll $t7, $a0, 16
    /* 8008AFDC 8FBF002C */  lw $ra, 44($sp)
    /* 8008AFE0 8FB00014 */  lw $s0, 20($sp)
    /* 8008AFE4 8FB10018 */  lw $s1, 24($sp)
    /* 8008AFE8 8FB2001C */  lw $s2, 28($sp)
    /* 8008AFEC 8FB30020 */  lw $s3, 32($sp)
    /* 8008AFF0 8FB40024 */  lw $s4, 36($sp)
    /* 8008AFF4 8FB50028 */  lw $s5, 40($sp)
    /* 8008AFF8 03E00008 */  jr $ra
    /* 8008AFFC 27BD0030 */  addiu $sp, $sp, 48
