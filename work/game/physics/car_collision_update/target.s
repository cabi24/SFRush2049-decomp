# Source: game_code.bin (decompressed)
# Address: 0x800A8F64

glabel car_collision_update
    /* 800A8F64 27BDFFA8 */  addiu $sp, $sp, -88
    /* 800A8F68 AFBF0014 */  sw $ra, 20($sp)
    /* 800A8F6C 24E50002 */  addiu $a1, $a3, 2
    /* 800A8F70 94A2FFFE */  lhu $v0, -2($a1)
    /* 800A8F74 24E40008 */  addiu $a0, $a3, 8
    /* 800A8F78 27A30040 */  addiu $v1, $sp, 64
    /* 800A8F7C 00027840 */  sll $t7, $v0, 1
    /* 800A8F80 A7AF003A */  sh $t7, 58($sp)
    /* 800A8F84 94B80000 */  lhu $t8, 0($a1)
    /* 800A8F88 27A70038 */  addiu $a3, $sp, 56
    /* 800A8F8C 01403025 */  or $a2, $t2, $zero
    /* 800A8F90 030F1021 */  addu $v0, $t8, $t7
    /* 800A8F94 00027040 */  sll $t6, $v0, 1
    /* 800A8F98 A7AE003C */  sh $t6, 60($sp)
    /* 800A8F9C 94AF0002 */  lhu $t7, 2($a1)
    /* 800A8FA0 27A50058 */  addiu $a1, $sp, 88
    /* 800A8FA4 01EE1021 */  addu $v0, $t7, $t6
    /* 800A8FA8 0002C040 */  sll $t8, $v0, 1
    /* 800A8FAC 3302FFFF */  andi $v0, $t8, 0xFFFF
    /* 800A8FB0 A7A2003E */  sh $v0, 62($sp)
    /* 800A8FB4 948EFFFE */  lhu $t6, -2($a0)
    /* 800A8FB8 24630008 */  addiu $v1, $v1, 8
    /* 800A8FBC 24840008 */  addiu $a0, $a0, 8
    /* 800A8FC0 01C21021 */  addu $v0, $t6, $v0
    /* 800A8FC4 0002C040 */  sll $t8, $v0, 1
    /* 800A8FC8 A478FFF8 */  sh $t8, -8($v1)
    /* 800A8FCC 9499FFF8 */  lhu $t9, -8($a0)
    /* 800A8FD0 03381021 */  addu $v0, $t9, $t8
    /* 800A8FD4 00027840 */  sll $t7, $v0, 1
    /* 800A8FD8 A46FFFFA */  sh $t7, -6($v1)
    /* 800A8FDC 9498FFFA */  lhu $t8, -6($a0)
    /* 800A8FE0 030F1021 */  addu $v0, $t8, $t7
    /* 800A8FE4 00027040 */  sll $t6, $v0, 1
    /* 800A8FE8 A46EFFFC */  sh $t6, -4($v1)
    /* 800A8FEC 948FFFFC */  lhu $t7, -4($a0)
    /* 800A8FF0 01EE1021 */  addu $v0, $t7, $t6
    /* 800A8FF4 0002C040 */  sll $t8, $v0, 1
    /* 800A8FF8 3302FFFF */  andi $v0, $t8, 0xFFFF
    /* 800A8FFC 1465FFED */  bne $v1, $a1, 0x800A8FB4
    /* 800A9000 A462FFFE */  sh $v0, -2($v1)
    /* 800A9004 0520000F */  bltz $t1, 0x800A9044
    /* 800A9008 00097080 */  sll $t6, $t1, 2
    /* 800A900C 01CA4021 */  addu $t0, $t6, $t2
    /* 800A9010 25080004 */  addiu $t0, $t0, 4
    /* 800A9014 94C50002 */  lhu $a1, 2($a2)
    /* 800A9018 10A00007 */  beq $a1, $zero, 0x800A9038
    /* 800A901C 00057840 */  sll $t7, $a1, 1
    /* 800A9020 00EF1021 */  addu $v0, $a3, $t7
    /* 800A9024 94440000 */  lhu $a0, 0($v0)
    /* 800A9028 24980001 */  addiu $t8, $a0, 1
    /* 800A902C 0C02A3CE */  jal 0x800A8F38
    /* 800A9030 A4580000 */  sh $t8, 0($v0)
    /* 800A9034 A4C20000 */  sh $v0, 0($a2)
    /* 800A9038 24C60004 */  addiu $a2, $a2, 4
    /* 800A903C 5506FFF6 */  .word 0x5506FFF6
    /* 800A9040 94C50002 */  lhu $a1, 2($a2)
    /* 800A9044 8FBF0014 */  lw $ra, 20($sp)
    /* 800A9048 27BD0058 */  addiu $sp, $sp, 88
    /* 800A904C 03E00008 */  jr $ra
    /* 800A9050 00000000 */  nop
