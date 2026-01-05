# Source: game_code.bin (decompressed)
# Address: 0x800B9D68

glabel physics_friction_apply
    /* 800B9D68 27BDFFA0 */  addiu $sp, $sp, -96
    /* 800B9D6C AFB20028 */  sw $s2, 40($sp)
    /* 800B9D70 3C128014 */  lui $s2, 0x8014
    /* 800B9D74 265207F0 */  addiu $s2, $s2, 2032
    /* 800B9D78 92420008 */  lbu $v0, 8($s2)
    /* 800B9D7C AFB3002C */  sw $s3, 44($sp)
    /* 800B9D80 AFBF0044 */  sw $ra, 68($sp)
    /* 800B9D84 AFBE0040 */  sw $fp, 64($sp)
    /* 800B9D88 AFB7003C */  sw $s7, 60($sp)
    /* 800B9D8C AFB60038 */  sw $s6, 56($sp)
    /* 800B9D90 AFB50034 */  sw $s5, 52($sp)
    /* 800B9D94 AFB40030 */  sw $s4, 48($sp)
    /* 800B9D98 AFB10024 */  sw $s1, 36($sp)
    /* 800B9D9C AFB00020 */  sw $s0, 32($sp)
    /* 800B9DA0 18400042 */  blez $v0, 0x800B9EAC
    /* 800B9DA4 00009825 */  or $s3, $zero, $zero
    /* 800B9DA8 00008825 */  or $s1, $zero, $zero
    /* 800B9DAC 241E0001 */  addiu $fp, $zero, 1
    /* 800B9DB0 24170002 */  addiu $s7, $zero, 2
    /* 800B9DB4 27B60050 */  addiu $s6, $sp, 80
    /* 800B9DB8 27B50054 */  addiu $s5, $sp, 84
    /* 800B9DBC 2414FFFF */  addiu $s4, $zero, -1
    /* 800B9DC0 8E4E000C */  lw $t6, 12($s2)
    /* 800B9DC4 02604825 */  or $t1, $s3, $zero
    /* 800B9DC8 240A0001 */  addiu $t2, $zero, 1
    /* 800B9DCC 01D18021 */  addu $s0, $t6, $s1
    /* 800B9DD0 920F0000 */  lbu $t7, 0($s0)
    /* 800B9DD4 02A02025 */  or $a0, $s5, $zero
    /* 800B9DD8 02C04025 */  or $t0, $s6, $zero
    /* 800B9DDC 16EF000C */  bne $s7, $t7, 0x800B9E10
    /* 800B9DE0 00000000 */  nop
    /* 800B9DE4 A2140001 */  sb $s4, 1($s0)
    /* 800B9DE8 8E58000C */  lw $t8, 12($s2)
    /* 800B9DEC 0311C821 */  addu $t9, $t8, $s1
    /* 800B9DF0 A7200002 */  sh $zero, 2($t9)
    /* 800B9DF4 8E4E000C */  lw $t6, 12($s2)
    /* 800B9DF8 01D17821 */  addu $t7, $t6, $s1
    /* 800B9DFC A1F40004 */  sb $s4, 4($t7)
    /* 800B9E00 8E58000C */  lw $t8, 12($s2)
    /* 800B9E04 0311C821 */  addu $t9, $t8, $s1
    /* 800B9E08 10000021 */  beq $zero, $zero, 0x800B9E90
    /* 800B9E0C A73E0006 */  sh $fp, 6($t9)
    /* 800B9E10 0C02E6D9 */  jal 0x800B9B64
    /* 800B9E14 8E07000C */  lw $a3, 12($s0)
    /* 800B9E18 8E4F000C */  lw $t7, 12($s2)
    /* 800B9E1C 8FAE0054 */  lw $t6, 84($sp)
    /* 800B9E20 02604825 */  or $t1, $s3, $zero
    /* 800B9E24 01F1C021 */  addu $t8, $t7, $s1
    /* 800B9E28 A30E0001 */  sb $t6, 1($t8)
    /* 800B9E2C 8E4F000C */  lw $t7, 12($s2)
    /* 800B9E30 8FB90050 */  lw $t9, 80($sp)
    /* 800B9E34 00005025 */  or $t2, $zero, $zero
    /* 800B9E38 01F17021 */  addu $t6, $t7, $s1
    /* 800B9E3C A5D90002 */  sh $t9, 2($t6)
    /* 800B9E40 8E58000C */  lw $t8, 12($s2)
    /* 800B9E44 02A02025 */  or $a0, $s5, $zero
    /* 800B9E48 02C04025 */  or $t0, $s6, $zero
    /* 800B9E4C 03118021 */  addu $s0, $t8, $s1
    /* 800B9E50 9619000A */  lhu $t9, 10($s0)
    /* 800B9E54 8E0F000C */  lw $t7, 12($s0)
    /* 800B9E58 00197080 */  sll $t6, $t9, 2
    /* 800B9E5C 01D97023 */  subu $t6, $t6, $t9
    /* 800B9E60 000E7040 */  sll $t6, $t6, 1
    /* 800B9E64 01EE3821 */  addu $a3, $t7, $t6
    /* 800B9E68 0C02E6D9 */  jal 0x800B9B64
    /* 800B9E6C 24E7FFFA */  addiu $a3, $a3, -6
    /* 800B9E70 8E59000C */  lw $t9, 12($s2)
    /* 800B9E74 8FB80054 */  lw $t8, 84($sp)
    /* 800B9E78 03317821 */  addu $t7, $t9, $s1
    /* 800B9E7C A1F80004 */  sb $t8, 4($t7)
    /* 800B9E80 8E59000C */  lw $t9, 12($s2)
    /* 800B9E84 8FAE0050 */  lw $t6, 80($sp)
    /* 800B9E88 0331C021 */  addu $t8, $t9, $s1
    /* 800B9E8C A70E0006 */  sh $t6, 6($t8)
    /* 800B9E90 92420008 */  lbu $v0, 8($s2)
    /* 800B9E94 26730001 */  addiu $s3, $s3, 1
    /* 800B9E98 26310010 */  addiu $s1, $s1, 16
    /* 800B9E9C 0262082A */  slt $at, $s3, $v0
    /* 800B9EA0 5420FFC8 */  .word 0x5420FFC8
    /* 800B9EA4 8E4E000C */  lw $t6, 12($s2)
    /* 800B9EA8 00009825 */  or $s3, $zero, $zero
    /* 800B9EAC 2414FFFF */  addiu $s4, $zero, -1
    /* 800B9EB0 1840001F */  blez $v0, 0x800B9F30
    /* 800B9EB4 24170002 */  addiu $s7, $zero, 2
    /* 800B9EB8 00008825 */  or $s1, $zero, $zero
    /* 800B9EBC 27B50058 */  addiu $s5, $sp, 88
    /* 800B9EC0 8E4F000C */  lw $t7, 12($s2)
    /* 800B9EC4 02602025 */  or $a0, $s3, $zero
    /* 800B9EC8 00002825 */  or $a1, $zero, $zero
    /* 800B9ECC 01F18021 */  addu $s0, $t7, $s1
    /* 800B9ED0 92190000 */  lbu $t9, 0($s0)
    /* 800B9ED4 02A03025 */  or $a2, $s5, $zero
    /* 800B9ED8 16F90003 */  bne $s7, $t9, 0x800B9EE8
    /* 800B9EDC 00000000 */  nop
    /* 800B9EE0 1000000D */  beq $zero, $zero, 0x800B9F18
    /* 800B9EE4 A2140008 */  sb $s4, 8($s0)
    /* 800B9EE8 0C02E683 */  jal 0x800B9A0C
    /* 800B9EEC 00003825 */  or $a3, $zero, $zero
    /* 800B9EF0 54400006 */  .word 0x54400006
    /* 800B9EF4 8E59000C */  lw $t9, 12($s2)
    /* 800B9EF8 8E4E000C */  lw $t6, 12($s2)
    /* 800B9EFC 01D1C021 */  addu $t8, $t6, $s1
    /* 800B9F00 10000005 */  beq $zero, $zero, 0x800B9F18
    /* 800B9F04 A3140008 */  sb $s4, 8($t8)
    /* 800B9F08 8E59000C */  lw $t9, 12($s2)
    /* 800B9F0C 8FAF0058 */  lw $t7, 88($sp)
    /* 800B9F10 03317021 */  addu $t6, $t9, $s1
    /* 800B9F14 A1CF0008 */  sb $t7, 8($t6)
    /* 800B9F18 92580008 */  lbu $t8, 8($s2)
    /* 800B9F1C 26730001 */  addiu $s3, $s3, 1
    /* 800B9F20 26310010 */  addiu $s1, $s1, 16
    /* 800B9F24 0278082A */  slt $at, $s3, $t8
    /* 800B9F28 5420FFE6 */  .word 0x5420FFE6
    /* 800B9F2C 8E4F000C */  lw $t7, 12($s2)
    /* 800B9F30 8FBF0044 */  lw $ra, 68($sp)
    /* 800B9F34 8FB00020 */  lw $s0, 32($sp)
    /* 800B9F38 8FB10024 */  lw $s1, 36($sp)
    /* 800B9F3C 8FB20028 */  lw $s2, 40($sp)
    /* 800B9F40 8FB3002C */  lw $s3, 44($sp)
    /* 800B9F44 8FB40030 */  lw $s4, 48($sp)
    /* 800B9F48 8FB50034 */  lw $s5, 52($sp)
    /* 800B9F4C 8FB60038 */  lw $s6, 56($sp)
    /* 800B9F50 8FB7003C */  lw $s7, 60($sp)
    /* 800B9F54 8FBE0040 */  lw $fp, 64($sp)
    /* 800B9F58 03E00008 */  jr $ra
    /* 800B9F5C 27BD0060 */  addiu $sp, $sp, 96
