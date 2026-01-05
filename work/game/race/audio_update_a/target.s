# Source: game_code.bin (decompressed)
# Address: 0x800F4D94

glabel audio_update_a
    /* 800F4D94 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800F4D98 AFBE0038 */  sw $fp, 56($sp)
    /* 800F4D9C 3C1E8015 */  lui $fp, 0x8015
    /* 800F4DA0 AFB70034 */  sw $s7, 52($sp)
    /* 800F4DA4 3C178015 */  lui $s7, 0x8015
    /* 800F4DA8 27DEA108 */  addiu $fp, $fp, -24312
    /* 800F4DAC 87CE0000 */  lh $t6, 0($fp)
    /* 800F4DB0 26F7978C */  addiu $s7, $s7, -26740
    /* 800F4DB4 AFB60030 */  sw $s6, 48($sp)
    /* 800F4DB8 82F60000 */  lb $s6, 0($s7)
    /* 800F4DBC AFB5002C */  sw $s5, 44($sp)
    /* 800F4DC0 AFBF003C */  sw $ra, 60($sp)
    /* 800F4DC4 AFB40028 */  sw $s4, 40($sp)
    /* 800F4DC8 AFB30024 */  sw $s3, 36($sp)
    /* 800F4DCC AFB20020 */  sw $s2, 32($sp)
    /* 800F4DD0 AFB1001C */  sw $s1, 28($sp)
    /* 800F4DD4 AFB00018 */  sw $s0, 24($sp)
    /* 800F4DD8 0000A825 */  or $s5, $zero, $zero
    /* 800F4DDC 19C00077 */  blez $t6, 0x800F4FBC
    /* 800F4DE0 26D6FFF2 */  addiu $s6, $s6, -14
    /* 800F4DE4 3C118015 */  lui $s1, 0x8015
    /* 800F4DE8 3C148015 */  lui $s4, 0x8015
    /* 800F4DEC 3C128014 */  lui $s2, 0x8014
    /* 800F4DF0 26520804 */  addiu $s2, $s2, 2052
    /* 800F4DF4 26941410 */  addiu $s4, $s4, 5136
    /* 800F4DF8 2631A118 */  addiu $s1, $s1, -24296
    /* 800F4DFC 24130002 */  addiu $s3, $zero, 2
    /* 800F4E00 2410000A */  addiu $s0, $zero, 10
    /* 800F4E04 8E220048 */  lw $v0, 72($s1)
    /* 800F4E08 00004025 */  or $t0, $zero, $zero
    /* 800F4E0C 00164980 */  sll $t1, $s6, 6
    /* 800F4E10 14400008 */  bne $v0, $zero, 0x800F4E34
    /* 800F4E14 00157100 */  sll $t6, $s5, 4
    /* 800F4E18 922F0001 */  lbu $t7, 1($s1)
    /* 800F4E1C 3C198014 */  lui $t9, 0x8014
    /* 800F4E20 27396150 */  addiu $t9, $t9, 24912
    /* 800F4E24 000FC080 */  sll $t8, $t7, 2
    /* 800F4E28 03195821 */  addu $t3, $t8, $t9
    /* 800F4E2C AE2B0048 */  sw $t3, 72($s1)
    /* 800F4E30 01601025 */  or $v0, $t3, $zero
    /* 800F4E34 8C4C0000 */  lw $t4, 0($v0)
    /* 800F4E38 01D57023 */  subu $t6, $t6, $s5
    /* 800F4E3C 3C0F8015 */  lui $t7, 0x8015
    /* 800F4E40 8D8D002C */  lw $t5, 44($t4)
    /* 800F4E44 25EF2038 */  addiu $t7, $t7, 8248
    /* 800F4E48 000E70C0 */  sll $t6, $t6, 3
    /* 800F4E4C 11A0005B */  beq $t5, $zero, 0x800F4FBC
    /* 800F4E50 01CF3821 */  addu $a3, $t6, $t7
    /* 800F4E54 00E05025 */  or $t2, $a3, $zero
    /* 800F4E58 15000008 */  bne $t0, $zero, 0x800F4E7C
    /* 800F4E5C 02893021 */  addu $a2, $s4, $t1
    /* 800F4E60 8E380048 */  lw $t8, 72($s1)
    /* 800F4E64 8F190000 */  lw $t9, 0($t8)
    /* 800F4E68 8F2B002C */  lw $t3, 44($t9)
    /* 800F4E6C 8D6C0000 */  lw $t4, 0($t3)
    /* 800F4E70 01893021 */  addu $a2, $t4, $t1
    /* 800F4E74 10000001 */  beq $zero, $zero, 0x800F4E7C
    /* 800F4E78 24C6050C */  addiu $a2, $a2, 1292
    /* 800F4E7C 8CE20008 */  lw $v0, 8($a3)
    /* 800F4E80 8CCD0004 */  lw $t5, 4($a2)
    /* 800F4E84 25080001 */  addiu $t0, $t0, 1
    /* 800F4E88 01A2082B */  sltu $at, $t5, $v0
    /* 800F4E8C 50200003 */  .word 0x50200003
    /* 800F4E90 8CE20014 */  lw $v0, 20($a3)
    /* 800F4E94 ACC20004 */  sw $v0, 4($a2)
    /* 800F4E98 8CE20014 */  lw $v0, 20($a3)
    /* 800F4E9C 8E4E0000 */  lw $t6, 0($s2)
    /* 800F4EA0 8CCF0008 */  lw $t7, 8($a2)
    /* 800F4EA4 004E001A */  div $v0, $t6
    /* 800F4EA8 15C00002 */  bne $t6, $zero, 0x800F4EB4
    /* 800F4EAC 00000000 */  nop
    /* 800F4EB0 0007000D */  .word 0x0007000D
    /* 800F4EB4 2401FFFF */  addiu $at, $zero, -1
    /* 800F4EB8 15C10004 */  bne $t6, $at, 0x800F4ECC
    /* 800F4EBC 3C018000 */  lui $at, 0x8000
    /* 800F4EC0 14410002 */  bne $v0, $at, 0x800F4ECC
    /* 800F4EC4 00000000 */  nop
    /* 800F4EC8 0006000D */  .word 0x0006000D
    /* 800F4ECC 00001812 */  mflo $v1
    /* 800F4ED0 01E3082B */  sltu $at, $t7, $v1
    /* 800F4ED4 50200004 */  .word 0x50200004
    /* 800F4ED8 8CD8000C */  lw $t8, 12($a2)
    /* 800F4EDC ACC30008 */  sw $v1, 8($a2)
    /* 800F4EE0 8CE20014 */  lw $v0, 20($a3)
    /* 800F4EE4 8CD8000C */  lw $t8, 12($a2)
    /* 800F4EE8 94CB0010 */  lhu $t3, 16($a2)
    /* 800F4EEC 94CE0012 */  lhu $t6, 18($a2)
    /* 800F4EF0 0302C821 */  addu $t9, $t8, $v0
    /* 800F4EF4 ACD9000C */  sw $t9, 12($a2)
    /* 800F4EF8 8E4C0000 */  lw $t4, 0($s2)
    /* 800F4EFC 8CD90014 */  lw $t9, 20($a2)
    /* 800F4F00 24C20008 */  addiu $v0, $a2, 8
    /* 800F4F04 016C6821 */  addu $t5, $t3, $t4
    /* 800F4F08 A4CD0010 */  sh $t5, 16($a2)
    /* 800F4F0C 962F0040 */  lhu $t7, 64($s1)
    /* 800F4F10 8CCD0018 */  lw $t5, 24($a2)
    /* 800F4F14 00C02825 */  or $a1, $a2, $zero
    /* 800F4F18 01CFC021 */  addu $t8, $t6, $t7
    /* 800F4F1C A4D80012 */  sh $t8, 18($a2)
    /* 800F4F20 854B0048 */  lh $t3, 72($t2)
    /* 800F4F24 02602025 */  or $a0, $s3, $zero
    /* 800F4F28 24E30004 */  addiu $v1, $a3, 4
    /* 800F4F2C 032B6021 */  addu $t4, $t9, $t3
    /* 800F4F30 ACCC0014 */  sw $t4, 20($a2)
    /* 800F4F34 854E004A */  lh $t6, 74($t2)
    /* 800F4F38 01AE7821 */  addu $t7, $t5, $t6
    /* 800F4F3C ACCF0018 */  sw $t7, 24($a2)
    /* 800F4F40 8C580014 */  lw $t8, 20($v0)
    /* 800F4F44 84790048 */  lh $t9, 72($v1)
    /* 800F4F48 8C4C0018 */  lw $t4, 24($v0)
    /* 800F4F4C 8C4F001C */  lw $t7, 28($v0)
    /* 800F4F50 03195821 */  addu $t3, $t8, $t9
    /* 800F4F54 AC4B0014 */  sw $t3, 20($v0)
    /* 800F4F58 846D004A */  lh $t5, 74($v1)
    /* 800F4F5C 8C4B0020 */  lw $t3, 32($v0)
    /* 800F4F60 24840004 */  addiu $a0, $a0, 4
    /* 800F4F64 018D7021 */  addu $t6, $t4, $t5
    /* 800F4F68 AC4E0018 */  sw $t6, 24($v0)
    /* 800F4F6C 8478004C */  lh $t8, 76($v1)
    /* 800F4F70 24420010 */  addiu $v0, $v0, 16
    /* 800F4F74 24630008 */  addiu $v1, $v1, 8
    /* 800F4F78 01F8C821 */  addu $t9, $t7, $t8
    /* 800F4F7C AC59000C */  sw $t9, 12($v0)
    /* 800F4F80 846C0046 */  lh $t4, 70($v1)
    /* 800F4F84 016C6821 */  addu $t5, $t3, $t4
    /* 800F4F88 1490FFED */  bne $a0, $s0, 0x800F4F40
    /* 800F4F8C AC4D0010 */  sw $t5, 16($v0)
    /* 800F4F90 1513FFB1 */  bne $t0, $s3, 0x800F4E58
    /* 800F4F94 00000000 */  nop
    /* 800F4F98 8E240048 */  lw $a0, 72($s1)
    /* 800F4F9C 0C03363B */  jal 0x800CD8EC
    /* 800F4FA0 92E50000 */  lbu $a1, 0($s7)
    /* 800F4FA4 87CE0000 */  lh $t6, 0($fp)
    /* 800F4FA8 26B50001 */  addiu $s5, $s5, 1
    /* 800F4FAC 2631004C */  addiu $s1, $s1, 76
    /* 800F4FB0 02AE082A */  slt $at, $s5, $t6
    /* 800F4FB4 5420FF94 */  .word 0x5420FF94
    /* 800F4FB8 8E220048 */  lw $v0, 72($s1)
    /* 800F4FBC 8FBF003C */  lw $ra, 60($sp)
    /* 800F4FC0 8FB00018 */  lw $s0, 24($sp)
    /* 800F4FC4 8FB1001C */  lw $s1, 28($sp)
    /* 800F4FC8 8FB20020 */  lw $s2, 32($sp)
    /* 800F4FCC 8FB30024 */  lw $s3, 36($sp)
    /* 800F4FD0 8FB40028 */  lw $s4, 40($sp)
    /* 800F4FD4 8FB5002C */  lw $s5, 44($sp)
    /* 800F4FD8 8FB60030 */  lw $s6, 48($sp)
    /* 800F4FDC 8FB70034 */  lw $s7, 52($sp)
    /* 800F4FE0 8FBE0038 */  lw $fp, 56($sp)
    /* 800F4FE4 03E00008 */  jr $ra
    /* 800F4FE8 27BD0040 */  addiu $sp, $sp, 64
