# Source: game_code.bin (decompressed)
# Address: 0x800F49C0

glabel track_section_render
    /* 800F49C0 19C00066 */  blez $t6, 0x800F4B5C
    /* 800F49C4 254AFFFA */  addiu $t2, $t2, -6
    /* 800F49C8 3C148015 */  lui $s4, 0x8015
    /* 800F49CC 3C178015 */  lui $s7, 0x8015
    /* 800F49D0 3C158014 */  lui $s5, 0x8014
    /* 800F49D4 3C138013 */  lui $s3, 0x8013
    /* 800F49D8 3C118015 */  lui $s1, 0x8015
    /* 800F49DC 3C098015 */  lui $t1, 0x8015
    /* 800F49E0 25291578 */  addiu $t1, $t1, 5496
    /* 800F49E4 26319428 */  addiu $s1, $s1, -27608
    /* 800F49E8 2673E67C */  addiu $s3, $s3, -6532
    /* 800F49EC 26B53F54 */  addiu $s5, $s5, 16212
    /* 800F49F0 26F70B68 */  addiu $s7, $s7, 2920
    /* 800F49F4 2694A118 */  addiu $s4, $s4, -24296
    /* 800F49F8 241E0002 */  addiu $fp, $zero, 2
    /* 800F49FC 24160001 */  addiu $s6, $zero, 1
    /* 800F4A00 8E820048 */  lw $v0, 72($s4)
    /* 800F4A04 000A4080 */  sll $t0, $t2, 2
    /* 800F4A08 010A4023 */  subu $t0, $t0, $t2
    /* 800F4A0C 14400007 */  bne $v0, $zero, 0x800F4A2C
    /* 800F4A10 00003825 */  or $a3, $zero, $zero
    /* 800F4A14 928F0001 */  lbu $t7, 1($s4)
    /* 800F4A18 3C198014 */  lui $t9, 0x8014
    /* 800F4A1C 27396150 */  addiu $t9, $t9, 24912
    /* 800F4A20 000FC080 */  sll $t8, $t7, 2
    /* 800F4A24 03191021 */  addu $v0, $t8, $t9
    /* 800F4A28 AE820048 */  sw $v0, 72($s4)
    /* 800F4A2C 8C4C0000 */  lw $t4, 0($v0)
    /* 800F4A30 00084080 */  sll $t0, $t0, 2
    /* 800F4A34 8D8D002C */  lw $t5, 44($t4)
    /* 800F4A38 51A00049 */  .word 0x51A00049
    /* 800F4A3C 8FBF003C */  lw $ra, 60($sp)
    /* 800F4A40 86420000 */  lh $v0, 0($s2)
    /* 800F4A44 14E00008 */  bne $a3, $zero, 0x800F4A68
    /* 800F4A48 00002025 */  or $a0, $zero, $zero
    /* 800F4A4C 8E8E0048 */  lw $t6, 72($s4)
    /* 800F4A50 8DCF0000 */  lw $t7, 0($t6)
    /* 800F4A54 8DF8002C */  lw $t8, 44($t7)
    /* 800F4A58 8F190000 */  lw $t9, 0($t8)
    /* 800F4A5C 03283021 */  addu $a2, $t9, $t0
    /* 800F4A60 10000002 */  beq $zero, $zero, 0x800F4A6C
    /* 800F4A64 24C6060C */  addiu $a2, $a2, 1548
    /* 800F4A68 01283021 */  addu $a2, $t1, $t0
    /* 800F4A6C 1840001A */  blez $v0, 0x800F4AD8
    /* 800F4A70 24E70001 */  addiu $a3, $a3, 1
    /* 800F4A74 3C0B8015 */  lui $t3, 0x8015
    /* 800F4A78 256B9428 */  addiu $t3, $t3, -27608
    /* 800F4A7C 020B2821 */  addu $a1, $s0, $t3
    /* 800F4A80 10900007 */  beq $a0, $s0, 0x800F4AA0
    /* 800F4A84 00106880 */  sll $t5, $s0, 2
    /* 800F4A88 022D7021 */  addu $t6, $s1, $t5
    /* 800F4A8C 01C47821 */  addu $t7, $t6, $a0
    /* 800F4A90 81F80000 */  lb $t8, 0($t7)
    /* 800F4A94 94CC0008 */  lhu $t4, 8($a2)
    /* 800F4A98 0198C821 */  addu $t9, $t4, $t8
    /* 800F4A9C A4D90008 */  sh $t9, 8($a2)
    /* 800F4AA0 80A20000 */  lb $v0, 0($a1)
    /* 800F4AA4 04400003 */  bltz $v0, 0x800F4AB4
    /* 800F4AA8 00021823 */  subu $v1, $zero, $v0
    /* 800F4AAC 10000001 */  beq $zero, $zero, 0x800F4AB4
    /* 800F4AB0 00401825 */  or $v1, $v0, $zero
    /* 800F4AB4 94CB000A */  lhu $t3, 10($a2)
    /* 800F4AB8 24840001 */  addiu $a0, $a0, 1
    /* 800F4ABC 24A50004 */  addiu $a1, $a1, 4
    /* 800F4AC0 01636821 */  addu $t5, $t3, $v1
    /* 800F4AC4 A4CD000A */  sh $t5, 10($a2)
    /* 800F4AC8 864E0000 */  lh $t6, 0($s2)
    /* 800F4ACC 008E082A */  slt $at, $a0, $t6
    /* 800F4AD0 1420FFEB */  bne $at, $zero, 0x800F4A80
    /* 800F4AD4 00000000 */  nop
    /* 800F4AD8 94CF0004 */  lhu $t7, 4($a2)
    /* 800F4ADC 0270C021 */  addu $t8, $s3, $s0
    /* 800F4AE0 25EC0001 */  addiu $t4, $t7, 1
    /* 800F4AE4 A4CC0004 */  sh $t4, 4($a2)
    /* 800F4AE8 82A20000 */  lb $v0, 0($s5)
    /* 800F4AEC 12020009 */  beq $s0, $v0, 0x800F4B14
    /* 800F4AF0 02625821 */  addu $t3, $s3, $v0
    /* 800F4AF4 83190000 */  lb $t9, 0($t8)
    /* 800F4AF8 816D0000 */  lb $t5, 0($t3)
    /* 800F4AFC 02F07021 */  addu $t6, $s7, $s0
    /* 800F4B00 532D0005 */  .word 0x532D0005
    /* 800F4B04 94CC0006 */  lhu $t4, 6($a2)
    /* 800F4B08 81CF0000 */  lb $t7, 0($t6)
    /* 800F4B0C 16CF0004 */  bne $s6, $t7, 0x800F4B20
    /* 800F4B10 00000000 */  nop
    /* 800F4B14 94CC0006 */  lhu $t4, 6($a2)
    /* 800F4B18 25980001 */  addiu $t8, $t4, 1
    /* 800F4B1C A4D80006 */  sh $t8, 6($a2)
    /* 800F4B20 54FEFFC8 */  .word 0x54FEFFC8
    /* 800F4B24 86420000 */  lh $v0, 0($s2)
    /* 800F4B28 3C058015 */  lui $a1, 0x8015
    /* 800F4B2C 90A5978C */  lbu $a1, -26740($a1)
    /* 800F4B30 8E840048 */  lw $a0, 72($s4)
    /* 800F4B34 0C03363B */  jal 0x800CD8EC
    /* 800F4B38 AFAA0044 */  sw $t2, 68($sp)
    /* 800F4B3C 864B0000 */  lh $t3, 0($s2)
    /* 800F4B40 26100001 */  addiu $s0, $s0, 1
    /* 800F4B44 3C098015 */  lui $t1, 0x8015
    /* 800F4B48 020B082A */  slt $at, $s0, $t3
    /* 800F4B4C 2694004C */  addiu $s4, $s4, 76
    /* 800F4B50 25291578 */  addiu $t1, $t1, 5496
    /* 800F4B54 1420FFAA */  bne $at, $zero, 0x800F4A00
    /* 800F4B58 8FAA0044 */  lw $t2, 68($sp)
    /* 800F4B5C 8FBF003C */  lw $ra, 60($sp)
    /* 800F4B60 8FB00018 */  lw $s0, 24($sp)
    /* 800F4B64 8FB1001C */  lw $s1, 28($sp)
    /* 800F4B68 8FB20020 */  lw $s2, 32($sp)
    /* 800F4B6C 8FB30024 */  lw $s3, 36($sp)
    /* 800F4B70 8FB40028 */  lw $s4, 40($sp)
    /* 800F4B74 8FB5002C */  lw $s5, 44($sp)
    /* 800F4B78 8FB60030 */  lw $s6, 48($sp)
    /* 800F4B7C 8FB70034 */  lw $s7, 52($sp)
    /* 800F4B80 8FBE0038 */  lw $fp, 56($sp)
    /* 800F4B84 03E00008 */  jr $ra
    /* 800F4B88 27BD0048 */  addiu $sp, $sp, 72
