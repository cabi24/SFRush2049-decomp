# Source: game_code.bin (decompressed)
# Address: 0x800D2E94

glabel minimap_render
    /* 800D2E94 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800D2E98 AFB30020 */  sw $s3, 32($sp)
    /* 800D2E9C 3C138014 */  lui $s3, 0x8014
    /* 800D2EA0 AFB70030 */  sw $s7, 48($sp)
    /* 800D2EA4 AFB6002C */  sw $s6, 44($sp)
    /* 800D2EA8 AFB50028 */  sw $s5, 40($sp)
    /* 800D2EAC AFB40024 */  sw $s4, 36($sp)
    /* 800D2EB0 AFB2001C */  sw $s2, 28($sp)
    /* 800D2EB4 AFB10018 */  sw $s1, 24($sp)
    /* 800D2EB8 AFB00014 */  sw $s0, 20($sp)
    /* 800D2EBC 8FB00050 */  lw $s0, 80($sp)
    /* 800D2EC0 00808825 */  or $s1, $a0, $zero
    /* 800D2EC4 8FB20058 */  lw $s2, 88($sp)
    /* 800D2EC8 8FB40054 */  lw $s4, 84($sp)
    /* 800D2ECC 00C0A825 */  or $s5, $a2, $zero
    /* 800D2ED0 00E0B025 */  or $s6, $a3, $zero
    /* 800D2ED4 00A0B825 */  or $s7, $a1, $zero
    /* 800D2ED8 267307F0 */  addiu $s3, $s3, 2032
    /* 800D2EDC AFBF0034 */  sw $ra, 52($sp)
    /* 800D2EE0 02202025 */  or $a0, $s1, $zero
    /* 800D2EE4 0C02E636 */  jal 0x800B98D8
    /* 800D2EE8 02402825 */  or $a1, $s2, $zero
    /* 800D2EEC 14400003 */  bne $v0, $zero, 0x800D2EFC
    /* 800D2EF0 00000000 */  nop
    /* 800D2EF4 10000021 */  beq $zero, $zero, 0x800D2F7C
    /* 800D2EF8 00001025 */  or $v0, $zero, $zero
    /* 800D2EFC 16400002 */  bne $s2, $zero, 0x800D2F08
    /* 800D2F00 00000000 */  nop
    /* 800D2F04 AE000000 */  sw $zero, 0($s0)
    /* 800D2F08 0622000A */  .word 0x0622000A
    /* 800D2F0C AEB10000 */  sw $s1, 0($s5)
    /* 800D2F10 5280000C */  .word 0x5280000C
    /* 800D2F14 8E69000C */  lw $t1, 12($s3)
    /* 800D2F18 8E6E000C */  lw $t6, 12($s3)
    /* 800D2F1C 00117900 */  sll $t7, $s1, 4
    /* 800D2F20 01CFC021 */  addu $t8, $t6, $t7
    /* 800D2F24 93190000 */  lbu $t9, 0($t8)
    /* 800D2F28 53200006 */  .word 0x53200006
    /* 800D2F2C 8E69000C */  lw $t1, 12($s3)
    /* 800D2F30 AEB10000 */  sw $s1, 0($s5)
    /* 800D2F34 AED70000 */  sw $s7, 0($s6)
    /* 800D2F38 10000010 */  beq $zero, $zero, 0x800D2F7C
    /* 800D2F3C 24020001 */  addiu $v0, $zero, 1
    /* 800D2F40 8E69000C */  lw $t1, 12($s3)
    /* 800D2F44 00111100 */  sll $v0, $s1, 4
    /* 800D2F48 8E080000 */  lw $t0, 0($s0)
    /* 800D2F4C 01225021 */  addu $t2, $t1, $v0
    /* 800D2F50 954B000A */  lhu $t3, 10($t2)
    /* 800D2F54 26520001 */  addiu $s2, $s2, 1
    /* 800D2F58 010B6021 */  addu $t4, $t0, $t3
    /* 800D2F5C 01976823 */  subu $t5, $t4, $s7
    /* 800D2F60 AE0D0000 */  sw $t5, 0($s0)
    /* 800D2F64 8E6E000C */  lw $t6, 12($s3)
    /* 800D2F68 01C21821 */  addu $v1, $t6, $v0
    /* 800D2F6C 80710004 */  lb $s1, 4($v1)
    /* 800D2F70 AFB1003C */  sw $s1, 60($sp)
    /* 800D2F74 1000FFDA */  beq $zero, $zero, 0x800D2EE0
    /* 800D2F78 94770006 */  lhu $s7, 6($v1)
    /* 800D2F7C 8FBF0034 */  lw $ra, 52($sp)
    /* 800D2F80 8FB00014 */  lw $s0, 20($sp)
    /* 800D2F84 8FB10018 */  lw $s1, 24($sp)
    /* 800D2F88 8FB2001C */  lw $s2, 28($sp)
    /* 800D2F8C 8FB30020 */  lw $s3, 32($sp)
    /* 800D2F90 8FB40024 */  lw $s4, 36($sp)
    /* 800D2F94 8FB50028 */  lw $s5, 40($sp)
    /* 800D2F98 8FB6002C */  lw $s6, 44($sp)
    /* 800D2F9C 8FB70030 */  lw $s7, 48($sp)
    /* 800D2FA0 03E00008 */  jr $ra
    /* 800D2FA4 27BD0040 */  addiu $sp, $sp, 64
