# Source: game_code.bin (decompressed)
# Address: 0x80092C58

glabel entity_name_copy
    /* 80092C58 27BDFFB8 */  addiu $sp, $sp, -72
    /* 80092C5C AFB70030 */  sw $s7, 48($sp)
    /* 80092C60 AFB50028 */  sw $s5, 40($sp)
    /* 80092C64 AFB40024 */  sw $s4, 36($sp)
    /* 80092C68 AFB2001C */  sw $s2, 28($sp)
    /* 80092C6C 00C09025 */  or $s2, $a2, $zero
    /* 80092C70 00E0A025 */  or $s4, $a3, $zero
    /* 80092C74 00A0A825 */  or $s5, $a1, $zero
    /* 80092C78 0080B825 */  or $s7, $a0, $zero
    /* 80092C7C AFBF0034 */  sw $ra, 52($sp)
    /* 80092C80 AFB6002C */  sw $s6, 44($sp)
    /* 80092C84 AFB30020 */  sw $s3, 32($sp)
    /* 80092C88 AFB10018 */  sw $s1, 24($sp)
    /* 80092C8C 10C00003 */  beq $a2, $zero, 0x80092C9C
    /* 80092C90 AFB00014 */  sw $s0, 20($sp)
    /* 80092C94 14E00003 */  bne $a3, $zero, 0x80092CA4
    /* 80092C98 00000000 */  nop
    /* 80092C9C 1000002D */  beq $zero, $zero, 0x80092D54
    /* 80092CA0 00001025 */  or $v0, $zero, $zero
    /* 80092CA4 02540019 */  multu $s2, $s4
    /* 80092CA8 8FB60058 */  lw $s6, 88($sp)
    /* 80092CAC 00007012 */  mflo $t6
    /* 80092CB0 02AE7821 */  addu $t7, $s5, $t6
    /* 80092CB4 1240001A */  beq $s2, $zero, 0x80092D20
    /* 80092CB8 AFAF003C */  sw $t7, 60($sp)
    /* 80092CBC 00128842 */  srl $s1, $s2, 1
    /* 80092CC0 02340019 */  multu $s1, $s4
    /* 80092CC4 02E02025 */  or $a0, $s7, $zero
    /* 80092CC8 0000C012 */  mflo $t8
    /* 80092CCC 02B88021 */  addu $s0, $s5, $t8
    /* 80092CD0 02009825 */  or $s3, $s0, $zero
    /* 80092CD4 02C0F809 */  jalr $ra, $s6
    /* 80092CD8 02002825 */  or $a1, $s0, $zero
    /* 80092CDC 04410003 */  bgez $v0, 0x80092CEC
    /* 80092CE0 32590001 */  andi $t9, $s2, 0x0001
    /* 80092CE4 1000000C */  beq $zero, $zero, 0x80092D18
    /* 80092CE8 02209025 */  or $s2, $s1, $zero
    /* 80092CEC 18400008 */  blez $v0, 0x80092D10
    /* 80092CF0 00000000 */  nop
    /* 80092CF4 13200003 */  beq $t9, $zero, 0x80092D04
    /* 80092CF8 0274A821 */  addu $s5, $s3, $s4
    /* 80092CFC 10000006 */  beq $zero, $zero, 0x80092D18
    /* 80092D00 02209023 */  subu $s2, $s1, $zero
    /* 80092D04 24020001 */  addiu $v0, $zero, 1
    /* 80092D08 10000003 */  beq $zero, $zero, 0x80092D18
    /* 80092D0C 02229023 */  subu $s2, $s1, $v0
    /* 80092D10 10000010 */  beq $zero, $zero, 0x80092D54
    /* 80092D14 02001025 */  or $v0, $s0, $zero
    /* 80092D18 5640FFE9 */  .word 0x5640FFE9
    /* 80092D1C 00128842 */  srl $s1, $s2, 1
    /* 80092D20 8FA8003C */  lw $t0, 60($sp)
    /* 80092D24 8FB60058 */  lw $s6, 88($sp)
    /* 80092D28 02E02025 */  or $a0, $s7, $zero
    /* 80092D2C 02A8082B */  sltu $at, $s5, $t0
    /* 80092D30 50200008 */  .word 0x50200008
    /* 80092D34 00001025 */  or $v0, $zero, $zero
    /* 80092D38 02C0F809 */  jalr $ra, $s6
    /* 80092D3C 02A02825 */  or $a1, $s5, $zero
    /* 80092D40 54400004 */  .word 0x54400004
    /* 80092D44 00001025 */  or $v0, $zero, $zero
    /* 80092D48 10000002 */  beq $zero, $zero, 0x80092D54
    /* 80092D4C 02A01025 */  or $v0, $s5, $zero
    /* 80092D50 00001025 */  or $v0, $zero, $zero
    /* 80092D54 8FBF0034 */  lw $ra, 52($sp)
    /* 80092D58 8FB00014 */  lw $s0, 20($sp)
    /* 80092D5C 8FB10018 */  lw $s1, 24($sp)
    /* 80092D60 8FB2001C */  lw $s2, 28($sp)
    /* 80092D64 8FB30020 */  lw $s3, 32($sp)
    /* 80092D68 8FB40024 */  lw $s4, 36($sp)
    /* 80092D6C 8FB50028 */  lw $s5, 40($sp)
    /* 80092D70 8FB6002C */  lw $s6, 44($sp)
    /* 80092D74 8FB70030 */  lw $s7, 48($sp)
    /* 80092D78 03E00008 */  jr $ra
    /* 80092D7C 27BD0048 */  addiu $sp, $sp, 72
