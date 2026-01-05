# Source: game_code.bin (decompressed)
# Address: 0x800C997C

glabel process_inputs
    /* 800C997C 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800C9980 AFBF0014 */  sw $ra, 20($sp)
    /* 800C9984 0C032577 */  jal 0x800C95DC
    /* 800C9988 00000000 */  nop
    /* 800C998C 3C038015 */  lui $v1, 0x8015
    /* 800C9990 3C048015 */  lui $a0, 0x8015
    /* 800C9994 3C0D8015 */  lui $t5, 0x8015
    /* 800C9998 3C0C8014 */  lui $t4, 0x8014
    /* 800C999C 3C0B8015 */  lui $t3, 0x8015
    /* 800C99A0 3C0A8015 */  lui $t2, 0x8015
    /* 800C99A4 3C098015 */  lui $t1, 0x8015
    /* 800C99A8 44800000 */  .word 0x44800000
    /* 800C99AC 2529A108 */  addiu $t1, $t1, -24312
    /* 800C99B0 254A6978 */  addiu $t2, $t2, 27000
    /* 800C99B4 256B6998 */  addiu $t3, $t3, 27032
    /* 800C99B8 258C3A00 */  addiu $t4, $t4, 14848
    /* 800C99BC 25AD6958 */  addiu $t5, $t5, 26968
    /* 800C99C0 2484A164 */  addiu $a0, $a0, -24220
    /* 800C99C4 2463A118 */  addiu $v1, $v1, -24296
    /* 800C99C8 00003825 */  or $a3, $zero, $zero
    /* 800C99CC 241F0004 */  addiu $ra, $zero, 4
    /* 800C99D0 24080005 */  addiu $t0, $zero, 5
    /* 800C99D4 90660001 */  lbu $a2, 1($v1)
    /* 800C99D8 51060008 */  .word 0x51060008
    /* 800C99DC AC600008 */  sw $zero, 8($v1)
    /* 800C99E0 852E0000 */  lh $t6, 0($t1)
    /* 800C99E4 00061080 */  sll $v0, $a2, 2
    /* 800C99E8 01427821 */  addu $t7, $t2, $v0
    /* 800C99EC 00EE082A */  slt $at, $a3, $t6
    /* 800C99F0 14200007 */  bne $at, $zero, 0x800C9A10
    /* 800C99F4 0162C821 */  addu $t9, $t3, $v0
    /* 800C99F8 AC600008 */  sw $zero, 8($v1)
    /* 800C99FC AC600004 */  sw $zero, 4($v1)
    /* 800C9A00 AC60000C */  sw $zero, 12($v1)
    /* 800C9A04 E4600010 */  swc1 $f0, 16($v1)
    /* 800C9A08 1000000E */  beq $zero, $zero, 0x800C9A44
    /* 800C9A0C E4600014 */  swc1 $f0, 20($v1)
    /* 800C9A10 8DF80000 */  lw $t8, 0($t7)
    /* 800C9A14 8F2E0000 */  lw $t6, 0($t9)
    /* 800C9A18 0006C8C0 */  sll $t9, $a2, 3
    /* 800C9A1C 01B92821 */  addu $a1, $t5, $t9
    /* 800C9A20 01827821 */  addu $t7, $t4, $v0
    /* 800C9A24 AC780008 */  sw $t8, 8($v1)
    /* 800C9A28 8DF80000 */  lw $t8, 0($t7)
    /* 800C9A2C C4A40000 */  lwc1 $f4, 0($a1)
    /* 800C9A30 C4A60004 */  lwc1 $f6, 4($a1)
    /* 800C9A34 AC6E0004 */  sw $t6, 4($v1)
    /* 800C9A38 AC78000C */  sw $t8, 12($v1)
    /* 800C9A3C E4640010 */  swc1 $f4, 16($v1)
    /* 800C9A40 E4660014 */  swc1 $f6, 20($v1)
    /* 800C9A44 906E004D */  lbu $t6, 77($v1)
    /* 800C9A48 24630098 */  addiu $v1, $v1, 152
    /* 800C9A4C 510E0007 */  .word 0x510E0007
    /* 800C9A50 AC800008 */  sw $zero, 8($a0)
    /* 800C9A54 852F0000 */  lh $t7, 0($t1)
    /* 800C9A58 25F8FFFF */  addiu $t8, $t7, -1
    /* 800C9A5C 00F8082A */  slt $at, $a3, $t8
    /* 800C9A60 54200008 */  .word 0x54200008
    /* 800C9A64 90850001 */  lbu $a1, 1($a0)
    /* 800C9A68 AC800008 */  sw $zero, 8($a0)
    /* 800C9A6C AC800004 */  sw $zero, 4($a0)
    /* 800C9A70 AC80000C */  sw $zero, 12($a0)
    /* 800C9A74 E4800010 */  swc1 $f0, 16($a0)
    /* 800C9A78 10000012 */  beq $zero, $zero, 0x800C9AC4
    /* 800C9A7C E4800014 */  swc1 $f0, 20($a0)
    /* 800C9A80 90850001 */  lbu $a1, 1($a0)
    /* 800C9A84 00051080 */  sll $v0, $a1, 2
    /* 800C9A88 0142C821 */  addu $t9, $t2, $v0
    /* 800C9A8C 8F2E0000 */  lw $t6, 0($t9)
    /* 800C9A90 01627821 */  addu $t7, $t3, $v0
    /* 800C9A94 0182C821 */  addu $t9, $t4, $v0
    /* 800C9A98 AC8E0008 */  sw $t6, 8($a0)
    /* 800C9A9C 8DF80000 */  lw $t8, 0($t7)
    /* 800C9AA0 000578C0 */  sll $t7, $a1, 3
    /* 800C9AA4 01AF3021 */  addu $a2, $t5, $t7
    /* 800C9AA8 AC980004 */  sw $t8, 4($a0)
    /* 800C9AAC 8F2E0000 */  lw $t6, 0($t9)
    /* 800C9AB0 AC8E000C */  sw $t6, 12($a0)
    /* 800C9AB4 C4C80000 */  lwc1 $f8, 0($a2)
    /* 800C9AB8 E4880010 */  swc1 $f8, 16($a0)
    /* 800C9ABC C4CA0004 */  lwc1 $f10, 4($a2)
    /* 800C9AC0 E48A0014 */  swc1 $f10, 20($a0)
    /* 800C9AC4 24E70002 */  addiu $a3, $a3, 2
    /* 800C9AC8 14FFFFC2 */  bne $a3, $ra, 0x800C99D4
    /* 800C9ACC 24840098 */  addiu $a0, $a0, 152
    /* 800C9AD0 8FBF0014 */  lw $ra, 20($sp)
    /* 800C9AD4 27BD0018 */  addiu $sp, $sp, 24
    /* 800C9AD8 03E00008 */  jr $ra
    /* 800C9ADC 00000000 */  nop
