# Source: game_code.bin (decompressed)
# Address: 0x800D7E88

glabel drone_target_update
    /* 800D7E88 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800D7E8C 3C0E8015 */  lui $t6, 0x8015
    /* 800D7E90 85CEA108 */  lh $t6, -24312($t6)
    /* 800D7E94 AFB70034 */  sw $s7, 52($sp)
    /* 800D7E98 0080B825 */  or $s7, $a0, $zero
    /* 800D7E9C AFBF003C */  sw $ra, 60($sp)
    /* 800D7EA0 AFBE0038 */  sw $fp, 56($sp)
    /* 800D7EA4 AFB60030 */  sw $s6, 48($sp)
    /* 800D7EA8 AFB5002C */  sw $s5, 44($sp)
    /* 800D7EAC AFB40028 */  sw $s4, 40($sp)
    /* 800D7EB0 AFB30024 */  sw $s3, 36($sp)
    /* 800D7EB4 AFB20020 */  sw $s2, 32($sp)
    /* 800D7EB8 AFB1001C */  sw $s1, 28($sp)
    /* 800D7EBC 19C00062 */  blez $t6, 0x800D8048
    /* 800D7EC0 AFB00018 */  sw $s0, 24($sp)
    /* 800D7EC4 3C0F8015 */  lui $t7, 0x8015
    /* 800D7EC8 25F5A118 */  addiu $s5, $t7, -24296
    /* 800D7ECC 3C1E8014 */  lui $fp, 0x8014
    /* 800D7ED0 3C108011 */  lui $s0, 0x8011
    /* 800D7ED4 261018E4 */  addiu $s0, $s0, 6372
    /* 800D7ED8 27DEC228 */  addiu $fp, $fp, -15832
    /* 800D7EDC 02A0A025 */  or $s4, $s5, $zero
    /* 800D7EE0 02A0B025 */  or $s6, $s5, $zero
    /* 800D7EE4 2412000A */  addiu $s2, $zero, 10
    /* 800D7EE8 12E00004 */  beq $s7, $zero, 0x800D7EFC
    /* 800D7EEC 24010005 */  addiu $at, $zero, 5
    /* 800D7EF0 03C09825 */  or $s3, $fp, $zero
    /* 800D7EF4 10000008 */  beq $zero, $zero, 0x800D7F18
    /* 800D7EF8 92820001 */  lbu $v0, 1($s4)
    /* 800D7EFC 92820001 */  lbu $v0, 1($s4)
    /* 800D7F00 3C198014 */  lui $t9, 0x8014
    /* 800D7F04 2739C068 */  addiu $t9, $t9, -16280
    /* 800D7F08 0002C080 */  sll $t8, $v0, 2
    /* 800D7F0C 0302C021 */  addu $t8, $t8, $v0
    /* 800D7F10 0018C040 */  sll $t8, $t8, 1
    /* 800D7F14 03199821 */  addu $s3, $t8, $t9
    /* 800D7F18 10410017 */  beq $v0, $at, 0x800D7F78
    /* 800D7F1C 3C088014 */  lui $t0, 0x8014
    /* 800D7F20 25089300 */  addiu $t0, $t0, -27904
    /* 800D7F24 00481821 */  addu $v1, $v0, $t0
    /* 800D7F28 80690000 */  lb $t1, 0($v1)
    /* 800D7F2C 51200013 */  .word 0x51200013
    /* 800D7F30 826F0000 */  lb $t7, 0($s3)
    /* 800D7F34 8E910048 */  lw $s1, 72($s4)
    /* 800D7F38 A0600000 */  sb $zero, 0($v1)
    /* 800D7F3C 12200004 */  beq $s1, $zero, 0x800D7F50
    /* 800D7F40 02202025 */  or $a0, $s1, $zero
    /* 800D7F44 0C033397 */  jal 0x800CCE5C
    /* 800D7F48 02602825 */  or $a1, $s3, $zero
    /* 800D7F4C 8E910048 */  lw $s1, 72($s4)
    /* 800D7F50 12200003 */  beq $s1, $zero, 0x800D7F60
    /* 800D7F54 3C0C8014 */  lui $t4, 0x8014
    /* 800D7F58 52E00008 */  .word 0x52E00008
    /* 800D7F5C 826F0000 */  lb $t7, 0($s3)
    /* 800D7F60 928A0001 */  lbu $t2, 1($s4)
    /* 800D7F64 258C6150 */  addiu $t4, $t4, 24912
    /* 800D7F68 02602825 */  or $a1, $s3, $zero
    /* 800D7F6C 000A5880 */  sll $t3, $t2, 2
    /* 800D7F70 0C033397 */  jal 0x800CCE5C
    /* 800D7F74 016C2021 */  addu $a0, $t3, $t4
    /* 800D7F78 826F0000 */  lb $t7, 0($s3)
    /* 800D7F7C 3C028015 */  lui $v0, 0x8015
    /* 800D7F80 8442A108 */  lh $v0, -24312($v0)
    /* 800D7F84 000FC080 */  sll $t8, $t7, 2
    /* 800D7F88 0218C821 */  addu $t9, $s0, $t8
    /* 800D7F8C 8F280000 */  lw $t0, 0($t9)
    /* 800D7F90 00026880 */  sll $t5, $v0, 2
    /* 800D7F94 01A26821 */  addu $t5, $t5, $v0
    /* 800D7F98 AEA80018 */  sw $t0, 24($s5)
    /* 800D7F9C 82690001 */  lb $t1, 1($s3)
    /* 800D7FA0 000D6880 */  sll $t5, $t5, 2
    /* 800D7FA4 3C0E8015 */  lui $t6, 0x8015
    /* 800D7FA8 00095080 */  sll $t2, $t1, 2
    /* 800D7FAC 020A5821 */  addu $t3, $s0, $t2
    /* 800D7FB0 8D6C0000 */  lw $t4, 0($t3)
    /* 800D7FB4 01A26823 */  subu $t5, $t5, $v0
    /* 800D7FB8 000D6880 */  sll $t5, $t5, 2
    /* 800D7FBC 25CEA118 */  addiu $t6, $t6, -24296
    /* 800D7FC0 01AE3021 */  addu $a2, $t5, $t6
    /* 800D7FC4 26C20008 */  addiu $v0, $s6, 8
    /* 800D7FC8 24040002 */  addiu $a0, $zero, 2
    /* 800D7FCC 26630002 */  addiu $v1, $s3, 2
    /* 800D7FD0 AEAC001C */  sw $t4, 28($s5)
    /* 800D7FD4 806D0000 */  lb $t5, 0($v1)
    /* 800D7FD8 24840004 */  addiu $a0, $a0, 4
    /* 800D7FDC 24420010 */  addiu $v0, $v0, 16
    /* 800D7FE0 000D7080 */  sll $t6, $t5, 2
    /* 800D7FE4 020E7821 */  addu $t7, $s0, $t6
    /* 800D7FE8 8DF80000 */  lw $t8, 0($t7)
    /* 800D7FEC 24630004 */  addiu $v1, $v1, 4
    /* 800D7FF0 AC580008 */  sw $t8, 8($v0)
    /* 800D7FF4 8079FFFD */  lb $t9, -3($v1)
    /* 800D7FF8 00194080 */  sll $t0, $t9, 2
    /* 800D7FFC 02084821 */  addu $t1, $s0, $t0
    /* 800D8000 8D2A0000 */  lw $t2, 0($t1)
    /* 800D8004 AC4A000C */  sw $t2, 12($v0)
    /* 800D8008 806BFFFE */  lb $t3, -2($v1)
    /* 800D800C 000B6080 */  sll $t4, $t3, 2
    /* 800D8010 020C6821 */  addu $t5, $s0, $t4
    /* 800D8014 8DAE0000 */  lw $t6, 0($t5)
    /* 800D8018 AC4E0010 */  sw $t6, 16($v0)
    /* 800D801C 806FFFFF */  lb $t7, -1($v1)
    /* 800D8020 000FC080 */  sll $t8, $t7, 2
    /* 800D8024 0218C821 */  addu $t9, $s0, $t8
    /* 800D8028 8F280000 */  lw $t0, 0($t9)
    /* 800D802C 1492FFE9 */  bne $a0, $s2, 0x800D7FD4
    /* 800D8030 AC480014 */  sw $t0, 20($v0)
    /* 800D8034 26B5004C */  addiu $s5, $s5, 76
    /* 800D8038 02A6082B */  sltu $at, $s5, $a2
    /* 800D803C 2694004C */  addiu $s4, $s4, 76
    /* 800D8040 1420FFA9 */  bne $at, $zero, 0x800D7EE8
    /* 800D8044 26D6004C */  addiu $s6, $s6, 76
    /* 800D8048 8FBF003C */  lw $ra, 60($sp)
    /* 800D804C 8FB00018 */  lw $s0, 24($sp)
    /* 800D8050 8FB1001C */  lw $s1, 28($sp)
    /* 800D8054 8FB20020 */  lw $s2, 32($sp)
    /* 800D8058 8FB30024 */  lw $s3, 36($sp)
    /* 800D805C 8FB40028 */  lw $s4, 40($sp)
    /* 800D8060 8FB5002C */  lw $s5, 44($sp)
    /* 800D8064 8FB60030 */  lw $s6, 48($sp)
    /* 800D8068 8FB70034 */  lw $s7, 52($sp)
    /* 800D806C 8FBE0038 */  lw $fp, 56($sp)
    /* 800D8070 03E00008 */  jr $ra
    /* 800D8074 27BD0040 */  addiu $sp, $sp, 64
