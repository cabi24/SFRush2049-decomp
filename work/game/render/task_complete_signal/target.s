# Source: game_code.bin (decompressed)
# Address: 0x8008AC74

glabel task_complete_signal
    /* 8008AC74 27BDFFE0 */  addiu $sp, $sp, -32
    /* 8008AC78 AFB10018 */  sw $s1, 24($sp)
    /* 8008AC7C 3C118015 */  lui $s1, 0x8015
    /* 8008AC80 26313E68 */  addiu $s1, $s1, 15976
    /* 8008AC84 AFBF001C */  sw $ra, 28($sp)
    /* 8008AC88 AFA40020 */  sw $a0, 32($sp)
    /* 8008AC8C 3C058015 */  lui $a1, 0x8015
    /* 8008AC90 AFB00014 */  sw $s0, 20($sp)
    /* 8008AC94 24A53EF0 */  addiu $a1, $a1, 16112
    /* 8008AC98 02202025 */  or $a0, $s1, $zero
    /* 8008AC9C 0C001A80 */  jal 0x80006A00
    /* 8008ACA0 24060001 */  addiu $a2, $zero, 1
    /* 8008ACA4 3C108015 */  lui $s0, 0x8015
    /* 8008ACA8 26103F10 */  addiu $s0, $s0, 16144
    /* 8008ACAC A2000000 */  sb $zero, 0($s0)
    /* 8008ACB0 02202025 */  or $a0, $s1, $zero
    /* 8008ACB4 00002825 */  or $a1, $zero, $zero
    /* 8008ACB8 0C001C9C */  jal 0x80007270
    /* 8008ACBC 24060001 */  addiu $a2, $zero, 1
    /* 8008ACC0 0C022AF9 */  jal 0x8008ABE4
    /* 8008ACC4 00000000 */  nop
    /* 8008ACC8 5440FFFA */  .word 0x5440FFFA
    /* 8008ACCC 02202025 */  or $a0, $s1, $zero
    /* 8008ACD0 A2000000 */  sb $zero, 0($s0)
    /* 8008ACD4 8E04000C */  lw $a0, 12($s0)
    /* 8008ACD8 00002825 */  or $a1, $zero, $zero
    /* 8008ACDC 0C001D78 */  jal 0x800075E0
    /* 8008ACE0 24060001 */  addiu $a2, $zero, 1
    /* 8008ACE4 1000FFF3 */  beq $zero, $zero, 0x8008ACB4
    /* 8008ACE8 02202025 */  or $a0, $s1, $zero
    /* 8008ACEC 00000000 */  nop
    /* 8008ACF0 8FBF001C */  lw $ra, 28($sp)
    /* 8008ACF4 8FB00014 */  lw $s0, 20($sp)
    /* 8008ACF8 8FB10018 */  lw $s1, 24($sp)
    /* 8008ACFC 03E00008 */  jr $ra
    /* 8008AD00 27BD0020 */  addiu $sp, $sp, 32
