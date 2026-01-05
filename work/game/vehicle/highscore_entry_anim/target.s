# Source: game_code.bin (decompressed)
# Address: 0x80104A58

glabel highscore_entry_anim
    /* 80104A58 0C02DD28 */  jal 0x800B74A0
    /* 80104A5C 00000000 */  nop
    /* 80104A60 3C108011 */  lui $s0, 0x8011
    /* 80104A64 261049A8 */  addiu $s0, $s0, 18856
    /* 80104A68 02002025 */  or $a0, $s0, $zero
    /* 80104A6C 0C02CFE9 */  jal 0x800B3FA4
    /* 80104A70 2405FFFF */  addiu $a1, $zero, -1
    /* 80104A74 0002C842 */  srl $t9, $v0, 1
    /* 80104A78 02797823 */  subu $t7, $s3, $t9
    /* 80104A7C 000F2400 */  sll $a0, $t7, 16
    /* 80104A80 001E2C00 */  sll $a1, $fp, 16
    /* 80104A84 0005CC03 */  sra $t9, $a1, 16
    /* 80104A88 0004C403 */  sra $t8, $a0, 16
    /* 80104A8C 03002025 */  or $a0, $t8, $zero
    /* 80104A90 03202825 */  or $a1, $t9, $zero
    /* 80104A94 0C02DC75 */  jal 0x800B71D4
    /* 80104A98 02003025 */  or $a2, $s0, $zero
    /* 80104A9C 8FAE0018 */  lw $t6, 24($sp)
    /* 80104AA0 2401FFFF */  addiu $at, $zero, -1
    /* 80104AA4 26730024 */  addiu $s3, $s3, 36
    /* 80104AA8 85CF0000 */  lh $t7, 0($t6)
    /* 80104AAC 24040010 */  addiu $a0, $zero, 16
    /* 80104AB0 15E10003 */  bne $t7, $at, 0x80104AC0
    /* 80104AB4 00000000 */  nop
    /* 80104AB8 10000001 */  beq $zero, $zero, 0x80104AC0
    /* 80104ABC 24040016 */  addiu $a0, $zero, 22
    /* 80104AC0 0C02DD28 */  jal 0x800B74A0
    /* 80104AC4 00000000 */  nop
    /* 80104AC8 3C108011 */  lui $s0, 0x8011
    /* 80104ACC 261049AC */  addiu $s0, $s0, 18860
    /* 80104AD0 02002025 */  or $a0, $s0, $zero
    /* 80104AD4 0C02CFE9 */  jal 0x800B3FA4
    /* 80104AD8 2405FFFF */  addiu $a1, $zero, -1
    /* 80104ADC 00027042 */  srl $t6, $v0, 1
    /* 80104AE0 026EC023 */  subu $t8, $s3, $t6
    /* 80104AE4 00182400 */  sll $a0, $t8, 16
    /* 80104AE8 001E2C00 */  sll $a1, $fp, 16
    /* 80104AEC 00057403 */  sra $t6, $a1, 16
    /* 80104AF0 0004CC03 */  sra $t9, $a0, 16
    /* 80104AF4 03202025 */  or $a0, $t9, $zero
    /* 80104AF8 01C02825 */  or $a1, $t6, $zero
    /* 80104AFC 0C02DC75 */  jal 0x800B71D4
    /* 80104B00 02003025 */  or $a2, $s0, $zero
    /* 80104B04 8FBF0014 */  lw $ra, 20($sp)
    /* 80104B08 27BD0088 */  addiu $sp, $sp, 136
    /* 80104B0C 03E00008 */  jr $ra
    /* 80104B10 00000000 */  nop
