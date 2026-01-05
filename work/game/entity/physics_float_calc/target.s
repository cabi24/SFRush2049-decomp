# Source: game_code.bin (decompressed)
# Address: 0x8009EBC0

glabel physics_float_calc
    /* 8009EBC0 27BDFF78 */  addiu $sp, $sp, -136
    /* 8009EBC4 3C038015 */  lui $v1, 0x8015
    /* 8009EBC8 24639B80 */  addiu $v1, $v1, -25728
    /* 8009EBCC AFA40088 */  sw $a0, 136($sp)
    /* 8009EBD0 8C640000 */  lw $a0, 0($v1)
    /* 8009EBD4 240EFFFF */  addiu $t6, $zero, -1
    /* 8009EBD8 AFBF001C */  sw $ra, 28($sp)
    /* 8009EBDC AFB00018 */  sw $s0, 24($sp)
    /* 8009EBE0 AFA60090 */  sw $a2, 144($sp)
    /* 8009EBE4 AFA00084 */  sw $zero, 132($sp)
    /* 8009EBE8 AFA00080 */  sw $zero, 128($sp)
    /* 8009EBEC 10800115 */  beq $a0, $zero, 0x8009F044
    /* 8009EBF0 AFAE0078 */  sw $t6, 120($sp)
    /* 8009EBF4 3C013F40 */  lui $at, 0x3F40
    /* 8009EBF8 00801025 */  or $v0, $a0, $zero
    /* 8009EBFC 44817000 */  .word 0x44817000
    /* 8009EC00 24040068 */  addiu $a0, $zero, 104
    /* 8009EC04 24060068 */  addiu $a2, $zero, 104
    /* 8009EC08 C4A40000 */  lwc1 $f4, 0($a1)
    /* 8009EC0C C4460034 */  lwc1 $f6, 52($v0)
    /* 8009EC10 46062201 */  .word 0x46062201
    /* 8009EC14 E7A8003C */  swc1 $f8, 60($sp)
    /* 8009EC18 C4500038 */  lwc1 $f16, 56($v0)
    /* 8009EC1C C4AA0004 */  lwc1 $f10, 4($a1)
    /* 8009EC20 46105481 */  .word 0x46105481
    /* 8009EC24 C7AA003C */  lwc1 $f10, 60($sp)
    /* 8009EC28 E7B20040 */  swc1 $f18, 64($sp)
    /* 8009EC2C C446003C */  lwc1 $f6, 60($v0)
    /* 8009EC30 C4A40008 */  lwc1 $f4, 8($a1)
    /* 8009EC34 46062201 */  .word 0x46062201
    /* 8009EC38 E7A80044 */  swc1 $f8, 68($sp)
    /* 8009EC3C C4500050 */  lwc1 $f16, 80($v0)
    /* 8009EC40 460A803E */  .word 0x460A803E
    /* 8009EC44 00000000 */  nop
    /* 8009EC48 4502003D */  .word 0x4502003D
    /* 8009EC4C 844D0044 */  lh $t5, 68($v0)
    /* 8009EC50 C452005C */  lwc1 $f18, 92($v0)
    /* 8009EC54 C7A40040 */  lwc1 $f4, 64($sp)
    /* 8009EC58 4612503E */  .word 0x4612503E
    /* 8009EC5C 00000000 */  nop
    /* 8009EC60 45020037 */  .word 0x45020037
    /* 8009EC64 844D0044 */  lh $t5, 68($v0)
    /* 8009EC68 C4420054 */  lwc1 $f2, 84($v0)
    /* 8009EC6C C7A60044 */  lwc1 $f6, 68($sp)
    /* 8009EC70 4604103E */  .word 0x4604103E
    /* 8009EC74 00000000 */  nop
    /* 8009EC78 45020031 */  .word 0x45020031
    /* 8009EC7C 844D0044 */  lh $t5, 68($v0)
    /* 8009EC80 C4480058 */  lwc1 $f8, 88($v0)
    /* 8009EC84 4606403E */  .word 0x4606403E
    /* 8009EC88 00000000 */  nop
    /* 8009EC8C 4502002C */  .word 0x4502002C
    /* 8009EC90 844D0044 */  lh $t5, 68($v0)
    /* 8009EC94 C4500064 */  lwc1 $f16, 100($v0)
    /* 8009EC98 8FAF0078 */  lw $t7, 120($sp)
    /* 8009EC9C 4610303E */  .word 0x4610303E
    /* 8009ECA0 00000000 */  nop
    /* 8009ECA4 45020026 */  .word 0x45020026
    /* 8009ECA8 844D0044 */  lh $t5, 68($v0)
    /* 8009ECAC 46063482 */  .word 0x46063482
    /* 8009ECB0 C4500060 */  lwc1 $f16, 96($v0)
    /* 8009ECB4 8FB80084 */  lw $t8, 132($sp)
    /* 8009ECB8 460A5202 */  .word 0x460A5202
    /* 8009ECBC 46089000 */  .word 0x46089000
    /* 8009ECC0 460E8182 */  .word 0x460E8182
    /* 8009ECC4 46000004 */  .word 0x46000004
    /* 8009ECC8 4604303C */  .word 0x4604303C
    /* 8009ECCC 46000306 */  .word 0x46000306
    /* 8009ECD0 45000003 */  .word 0x45000003
    /* 8009ECD4 00000000 */  nop
    /* 8009ECD8 46022281 */  .word 0x46022281
    /* 8009ECDC 460A0300 */  .word 0x460A0300
    /* 8009ECE0 05E20007 */  .word 0x05E20007
    /* 8009ECE4 8C790000 */  lw $t9, 0($v1)
    /* 8009ECE8 C7B2007C */  lwc1 $f18, 124($sp)
    /* 8009ECEC 4612603C */  .word 0x4612603C
    /* 8009ECF0 00000000 */  nop
    /* 8009ECF4 45020012 */  .word 0x45020012
    /* 8009ECF8 844D0044 */  lh $t5, 68($v0)
    /* 8009ECFC 8C790000 */  lw $t9, 0($v1)
    /* 8009ED00 AFB80078 */  sw $t8, 120($sp)
    /* 8009ED04 E7AC007C */  swc1 $f12, 124($sp)
    /* 8009ED08 00595823 */  subu $t3, $v0, $t9
    /* 8009ED0C 0164001A */  div $t3, $a0
    /* 8009ED10 00006012 */  mflo $t4
    /* 8009ED14 AFAC0080 */  sw $t4, 128($sp)
    /* 8009ED18 14800002 */  bne $a0, $zero, 0x8009ED24
    /* 8009ED1C 00000000 */  nop
    /* 8009ED20 0007000D */  .word 0x0007000D
    /* 8009ED24 2401FFFF */  addiu $at, $zero, -1
    /* 8009ED28 14810004 */  bne $a0, $at, 0x8009ED3C
    /* 8009ED2C 3C018000 */  lui $at, 0x8000
    /* 8009ED30 15610002 */  bne $t3, $at, 0x8009ED3C
    /* 8009ED34 00000000 */  nop
    /* 8009ED38 0006000D */  .word 0x0006000D
    /* 8009ED3C 844D0044 */  lh $t5, 68($v0)
    /* 8009ED40 8FAE0084 */  lw $t6, 132($sp)
    /* 8009ED44 05A00009 */  bltz $t5, 0x8009ED6C
    /* 8009ED48 25CF0001 */  addiu $t7, $t6, 1
    /* 8009ED4C AFAF0084 */  sw $t7, 132($sp)
    /* 8009ED50 84580044 */  lh $t8, 68($v0)
    /* 8009ED54 8C6B0000 */  lw $t3, 0($v1)
    /* 8009ED58 03060019 */  multu $t8, $a2
    /* 8009ED5C 0000C812 */  mflo $t9
    /* 8009ED60 032B1021 */  addu $v0, $t9, $t3
    /* 8009ED64 1000FFA9 */  beq $zero, $zero, 0x8009EC0C
    /* 8009ED68 C4A40000 */  lwc1 $f4, 0($a1)
    /* 8009ED6C 8FAC0078 */  lw $t4, 120($sp)
    /* 8009ED70 3C0A8012 */  lui $t2, 0x8012
    /* 8009ED74 254AE75C */  addiu $t2, $t2, -6308
    /* 8009ED78 05810004 */  bgez $t4, 0x8009ED8C
    /* 8009ED7C 3C058015 */  lui $a1, 0x8015
    /* 8009ED80 3C058015 */  lui $a1, 0x8015
    /* 8009ED84 1000007D */  beq $zero, $zero, 0x8009EF7C
    /* 8009ED88 80A5978C */  lb $a1, -26740($a1)
    /* 8009ED8C 80A5978C */  lb $a1, -26740($a1)
    /* 8009ED90 8FAA0064 */  lw $t2, 100($sp)
    /* 8009ED94 2CA10013 */  sltiu $at, $a1, 19
    /* 8009ED98 10200078 */  beq $at, $zero, 0x8009EF7C
    /* 8009ED9C 00056880 */  sll $t5, $a1, 2
    /* 8009EDA0 3C018012 */  lui $at, 0x8012
    /* 8009EDA4 002D0821 */  addu $at, $at, $t5
    /* 8009EDA8 8C2D3B14 */  lw $t5, 15124($at)
    /* 8009EDAC 01A00008 */  jr $t5
    /* 8009EDB0 00000000 */  nop
    /* 8009EDB4 8FAE0078 */  lw $t6, 120($sp)
    /* 8009EDB8 3C188012 */  lui $t8, 0x8012
    /* 8009EDBC 2718B898 */  addiu $t8, $t8, -18280
    /* 8009EDC0 000E7900 */  sll $t7, $t6, 4
    /* 8009EDC4 1000006D */  beq $zero, $zero, 0x8009EF7C
    /* 8009EDC8 01F85021 */  addu $t2, $t7, $t8
    /* 8009EDCC 8FB90078 */  lw $t9, 120($sp)
    /* 8009EDD0 3C0C8012 */  lui $t4, 0x8012
    /* 8009EDD4 258CBFE8 */  addiu $t4, $t4, -16408
    /* 8009EDD8 00195900 */  sll $t3, $t9, 4
    /* 8009EDDC 10000067 */  beq $zero, $zero, 0x8009EF7C
    /* 8009EDE0 016C5021 */  addu $t2, $t3, $t4
    /* 8009EDE4 8FAD0078 */  lw $t5, 120($sp)
    /* 8009EDE8 3C0F8012 */  lui $t7, 0x8012
    /* 8009EDEC 25EFC738 */  addiu $t7, $t7, -14536
    /* 8009EDF0 000D7100 */  sll $t6, $t5, 4
    /* 8009EDF4 10000061 */  beq $zero, $zero, 0x8009EF7C
    /* 8009EDF8 01CF5021 */  addu $t2, $t6, $t7
    /* 8009EDFC 8FB80078 */  lw $t8, 120($sp)
    /* 8009EE00 3C0B8012 */  lui $t3, 0x8012
    /* 8009EE04 256BCE88 */  addiu $t3, $t3, -12664
    /* 8009EE08 0018C900 */  sll $t9, $t8, 4
    /* 8009EE0C 1000005B */  beq $zero, $zero, 0x8009EF7C
    /* 8009EE10 032B5021 */  addu $t2, $t9, $t3
    /* 8009EE14 8FAC0078 */  lw $t4, 120($sp)
    /* 8009EE18 3C0E8012 */  lui $t6, 0x8012
    /* 8009EE1C 25CED618 */  addiu $t6, $t6, -10728
    /* 8009EE20 000C6900 */  sll $t5, $t4, 4
    /* 8009EE24 10000055 */  beq $zero, $zero, 0x8009EF7C
    /* 8009EE28 01AE5021 */  addu $t2, $t5, $t6
    /* 8009EE2C 8FAF0078 */  lw $t7, 120($sp)
    /* 8009EE30 3C198012 */  lui $t9, 0x8012
    /* 8009EE34 2739DC88 */  addiu $t9, $t9, -9080
    /* 8009EE38 000FC100 */  sll $t8, $t7, 4
    /* 8009EE3C 1000004F */  beq $zero, $zero, 0x8009EF7C
    /* 8009EE40 03195021 */  addu $t2, $t8, $t9
    /* 8009EE44 8FAB0078 */  lw $t3, 120($sp)
    /* 8009EE48 3C0D8012 */  lui $t5, 0x8012
    /* 8009EE4C 25ADE428 */  addiu $t5, $t5, -7128
    /* 8009EE50 000B6100 */  sll $t4, $t3, 4
    /* 8009EE54 10000049 */  beq $zero, $zero, 0x8009EF7C
    /* 8009EE58 018D5021 */  addu $t2, $t4, $t5
    /* 8009EE5C 8FAE0078 */  lw $t6, 120($sp)
    /* 8009EE60 3C188012 */  lui $t8, 0x8012
    /* 8009EE64 2718E438 */  addiu $t8, $t8, -7112
    /* 8009EE68 000E7900 */  sll $t7, $t6, 4
    /* 8009EE6C 10000043 */  beq $zero, $zero, 0x8009EF7C
    /* 8009EE70 01F85021 */  addu $t2, $t7, $t8
    /* 8009EE74 8FB90078 */  lw $t9, 120($sp)
    /* 8009EE78 3C0C8012 */  lui $t4, 0x8012
    /* 8009EE7C 258CE448 */  addiu $t4, $t4, -7096
    /* 8009EE80 00195900 */  sll $t3, $t9, 4
    /* 8009EE84 1000003D */  beq $zero, $zero, 0x8009EF7C
    /* 8009EE88 016C5021 */  addu $t2, $t3, $t4
    /* 8009EE8C 8FAD0078 */  lw $t5, 120($sp)
    /* 8009EE90 3C0F8012 */  lui $t7, 0x8012
    /* 8009EE94 25EFE458 */  addiu $t7, $t7, -7080
    /* 8009EE98 000D7100 */  sll $t6, $t5, 4
    /* 8009EE9C 10000037 */  beq $zero, $zero, 0x8009EF7C
    /* 8009EEA0 01CF5021 */  addu $t2, $t6, $t7
    /* 8009EEA4 8FB80078 */  lw $t8, 120($sp)
    /* 8009EEA8 3C0B8012 */  lui $t3, 0x8012
    /* 8009EEAC 256BE468 */  addiu $t3, $t3, -7064
    /* 8009EEB0 0018C900 */  sll $t9, $t8, 4
    /* 8009EEB4 10000031 */  beq $zero, $zero, 0x8009EF7C
    /* 8009EEB8 032B5021 */  addu $t2, $t9, $t3
    /* 8009EEBC 8FAC0078 */  lw $t4, 120($sp)
    /* 8009EEC0 3C0E8012 */  lui $t6, 0x8012
    /* 8009EEC4 25CEE548 */  addiu $t6, $t6, -6840
    /* 8009EEC8 000C6900 */  sll $t5, $t4, 4
    /* 8009EECC 1000002B */  beq $zero, $zero, 0x8009EF7C
    /* 8009EED0 01AE5021 */  addu $t2, $t5, $t6
    /* 8009EED4 8FAF0078 */  lw $t7, 120($sp)
    /* 8009EED8 3C198012 */  lui $t9, 0x8012
    /* 8009EEDC 2739E558 */  addiu $t9, $t9, -6824
    /* 8009EEE0 000FC100 */  sll $t8, $t7, 4
    /* 8009EEE4 10000025 */  beq $zero, $zero, 0x8009EF7C
    /* 8009EEE8 03195021 */  addu $t2, $t8, $t9
    /* 8009EEEC 8FAB0078 */  lw $t3, 120($sp)
    /* 8009EEF0 3C0D8012 */  lui $t5, 0x8012
    /* 8009EEF4 25ADE568 */  addiu $t5, $t5, -6808
    /* 8009EEF8 000B6100 */  sll $t4, $t3, 4
    /* 8009EEFC 1000001F */  beq $zero, $zero, 0x8009EF7C
    /* 8009EF00 018D5021 */  addu $t2, $t4, $t5
    /* 8009EF04 8FAE0078 */  lw $t6, 120($sp)
    /* 8009EF08 3C188012 */  lui $t8, 0x8012
    /* 8009EF0C 2718E578 */  addiu $t8, $t8, -6792
    /* 8009EF10 000E7900 */  sll $t7, $t6, 4
    /* 8009EF14 10000019 */  beq $zero, $zero, 0x8009EF7C
    /* 8009EF18 01F85021 */  addu $t2, $t7, $t8
    /* 8009EF1C 8FB90078 */  lw $t9, 120($sp)
    /* 8009EF20 3C0C8012 */  lui $t4, 0x8012
    /* 8009EF24 258CE588 */  addiu $t4, $t4, -6776
    /* 8009EF28 00195900 */  sll $t3, $t9, 4
    /* 8009EF2C 10000013 */  beq $zero, $zero, 0x8009EF7C
    /* 8009EF30 016C5021 */  addu $t2, $t3, $t4
    /* 8009EF34 8FAD0078 */  lw $t5, 120($sp)
    /* 8009EF38 3C0F8012 */  lui $t7, 0x8012
    /* 8009EF3C 25EFE598 */  addiu $t7, $t7, -6760
    /* 8009EF40 000D7100 */  sll $t6, $t5, 4
    /* 8009EF44 1000000D */  beq $zero, $zero, 0x8009EF7C
    /* 8009EF48 01CF5021 */  addu $t2, $t6, $t7
    /* 8009EF4C 8FB80078 */  lw $t8, 120($sp)
    /* 8009EF50 3C0B8012 */  lui $t3, 0x8012
    /* 8009EF54 256BE5A8 */  addiu $t3, $t3, -6744
    /* 8009EF58 0018C900 */  sll $t9, $t8, 4
    /* 8009EF5C 10000007 */  beq $zero, $zero, 0x8009EF7C
    /* 8009EF60 032B5021 */  addu $t2, $t9, $t3
    /* 8009EF64 8FAC0078 */  lw $t4, 120($sp)
    /* 8009EF68 3C0E8012 */  lui $t6, 0x8012
    /* 8009EF6C 25CEE5B8 */  addiu $t6, $t6, -6728
    /* 8009EF70 000C6900 */  sll $t5, $t4, 4
    /* 8009EF74 10000001 */  beq $zero, $zero, 0x8009EF7C
    /* 8009EF78 01AE5021 */  addu $t2, $t5, $t6
    /* 8009EF7C 3C098012 */  lui $t1, 0x8012
    /* 8009EF80 01254821 */  addu $t1, $t1, $a1
    /* 8009EF84 9129E748 */  lbu $t1, -6328($t1)
    /* 8009EF88 24020001 */  addiu $v0, $zero, 1
    /* 8009EF8C 00001825 */  or $v1, $zero, $zero
    /* 8009EF90 1920002C */  blez $t1, 0x8009F044
    /* 8009EF94 00003025 */  or $a2, $zero, $zero
    /* 8009EF98 3C0F8015 */  lui $t7, 0x8015
    /* 8009EF9C 85EF9D90 */  lh $t7, -25200($t7)
    /* 8009EFA0 3C198013 */  lui $t9, 0x8013
    /* 8009EFA4 2739E700 */  addiu $t9, $t9, -6400
    /* 8009EFA8 000FC100 */  sll $t8, $t7, 4
    /* 8009EFAC 030FC021 */  addu $t8, $t8, $t7
    /* 8009EFB0 0018C080 */  sll $t8, $t8, 2
    /* 8009EFB4 03198021 */  addu $s0, $t8, $t9
    /* 8009EFB8 00003825 */  or $a3, $zero, $zero
    /* 8009EFBC 01404025 */  or $t0, $t2, $zero
    /* 8009EFC0 10600007 */  beq $v1, $zero, 0x8009EFE0
    /* 8009EFC4 24E70044 */  addiu $a3, $a3, 68
    /* 8009EFC8 306B001F */  andi $t3, $v1, 0x001F
    /* 8009EFCC 55600005 */  .word 0x55600005
    /* 8009EFD0 8D0C0000 */  lw $t4, 0($t0)
    /* 8009EFD4 24C60004 */  addiu $a2, $a2, 4
    /* 8009EFD8 25080004 */  addiu $t0, $t0, 4
    /* 8009EFDC 24020001 */  addiu $v0, $zero, 1
    /* 8009EFE0 8D0C0000 */  lw $t4, 0($t0)
    /* 8009EFE4 24630001 */  addiu $v1, $v1, 1
    /* 8009EFE8 8FAE0090 */  lw $t6, 144($sp)
    /* 8009EFEC 01826824 */  and $t5, $t4, $v0
    /* 8009EFF0 15A00003 */  bne $t5, $zero, 0x8009F000
    /* 8009EFF4 3C0F8014 */  lui $t7, 0x8014
    /* 8009EFF8 51C0000B */  .word 0x51C0000B
    /* 8009EFFC 8E0B0000 */  lw $t3, 0($s0)
    /* 8009F000 81EF061A */  lb $t7, 1562($t7)
    /* 8009F004 55E00008 */  .word 0x55E00008
    /* 8009F008 8E0B0000 */  lw $t3, 0($s0)
    /* 8009F00C 8E180000 */  lw $t8, 0($s0)
    /* 8009F010 3C017FFF */  lui $at, 0x7FFF
    /* 8009F014 3421FFFF */  ori $at, $at, 0xFFFF
    /* 8009F018 0301C824 */  and $t9, $t8, $at
    /* 8009F01C 10000005 */  beq $zero, $zero, 0x8009F034
    /* 8009F020 AE190000 */  sw $t9, 0($s0)
    /* 8009F024 8E0B0000 */  lw $t3, 0($s0)
    /* 8009F028 3C018000 */  lui $at, 0x8000
    /* 8009F02C 01616025 */  or $t4, $t3, $at
    /* 8009F030 AE0C0000 */  sw $t4, 0($s0)
    /* 8009F034 00026840 */  sll $t5, $v0, 1
    /* 8009F038 01A01025 */  or $v0, $t5, $zero
    /* 8009F03C 1469FFE0 */  bne $v1, $t1, 0x8009EFC0
    /* 8009F040 26100044 */  addiu $s0, $s0, 68
    /* 8009F044 8FBF001C */  lw $ra, 28($sp)
    /* 8009F048 8FB00018 */  lw $s0, 24($sp)
    /* 8009F04C 27BD0088 */  addiu $sp, $sp, 136
    /* 8009F050 03E00008 */  jr $ra
    /* 8009F054 00000000 */  nop
