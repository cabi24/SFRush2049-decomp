# Source: game_code.bin (decompressed)
# Address: 0x800B0870

glabel physics_update_all
    /* 800B0870 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800B0874 AFBF001C */  sw $ra, 28($sp)
    /* 800B0878 AFB00018 */  sw $s0, 24($sp)
    /* 800B087C 10400003 */  beq $v0, $zero, 0x800B088C
    /* 800B0880 00402025 */  or $a0, $v0, $zero
    /* 800B0884 10000002 */  beq $zero, $zero, 0x800B0890
    /* 800B0888 8C500000 */  lw $s0, 0($v0)
    /* 800B088C 00008025 */  or $s0, $zero, $zero
    /* 800B0890 1080000B */  beq $a0, $zero, 0x800B08C0
    /* 800B0894 00000000 */  nop
    /* 800B0898 8C820014 */  lw $v0, 20($a0)
    /* 800B089C 10400003 */  beq $v0, $zero, 0x800B08AC
    /* 800B08A0 00000000 */  nop
    /* 800B08A4 0040F809 */  jalr $ra, $v0
    /* 800B08A8 24050001 */  addiu $a1, $zero, 1
    /* 800B08AC 12000002 */  beq $s0, $zero, 0x800B08B8
    /* 800B08B0 02002025 */  or $a0, $s0, $zero
    /* 800B08B4 8C900000 */  lw $s0, 0($a0)
    /* 800B08B8 5480FFF8 */  .word 0x5480FFF8
    /* 800B08BC 8C820014 */  lw $v0, 20($a0)
    /* 800B08C0 0C02C19B */  jal 0x800B066C
    /* 800B08C4 00000000 */  nop
    /* 800B08C8 8FBF001C */  lw $ra, 28($sp)
    /* 800B08CC 8FB00018 */  lw $s0, 24($sp)
    /* 800B08D0 27BD0020 */  addiu $sp, $sp, 32
    /* 800B08D4 03E00008 */  jr $ra
    /* 800B08D8 00000000 */  nop
