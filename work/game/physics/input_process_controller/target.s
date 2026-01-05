# Source: game_code.bin (decompressed)
# Address: 0x800AD734

glabel input_process_controller
    /* 800AD734 27BDFF48 */  addiu $sp, $sp, -184
    /* 800AD738 AFBF0014 */  sw $ra, 20($sp)
    /* 800AD73C 24140001 */  addiu $s4, $zero, 1
    /* 800AD740 24E50004 */  addiu $a1, $a3, 4
    /* 800AD744 0C02B594 */  jal 0x800AD650
    /* 800AD748 03C02025 */  or $a0, $fp, $zero
    /* 800AD74C 94F00002 */  lhu $s0, 2($a3)
    /* 800AD750 3C0E8015 */  lui $t6, 0x8015
    /* 800AD754 8DCE2568 */  lw $t6, 9576($t6)
    /* 800AD758 94EF0016 */  lhu $t7, 22($a3)
    /* 800AD75C 3205000F */  andi $a1, $s0, 0x000F
    /* 800AD760 00A08025 */  or $s0, $a1, $zero
    /* 800AD764 27A60090 */  addiu $a2, $sp, 144
    /* 800AD768 0C02B574 */  jal 0x800AD5D0
    /* 800AD76C 01EE2021 */  addu $a0, $t7, $t6
    /* 800AD770 A6A20000 */  sh $v0, 0($s5)
    /* 800AD774 97A70090 */  lhu $a3, 144($sp)
    /* 800AD778 3C198015 */  lui $t9, 0x8015
    /* 800AD77C 8F39201C */  lw $t9, 8220($t9)
    /* 800AD780 0007C0C0 */  sll $t8, $a3, 3
    /* 800AD784 3C013D00 */  lui $at, 0x3D00
    /* 800AD788 03191821 */  addu $v1, $t8, $t9
    /* 800AD78C 846F0000 */  lh $t7, 0($v1)
    /* 800AD790 94780006 */  lhu $t8, 6($v1)
    /* 800AD794 4481A000 */  .word 0x4481A000
    /* 800AD798 000F7140 */  sll $t6, $t7, 5
    /* 800AD79C 33197C00 */  andi $t9, $t8, 0x7C00
    /* 800AD7A0 00197A83 */  sra $t7, $t9, 10
    /* 800AD7A4 01CFC021 */  addu $t8, $t6, $t7
    /* 800AD7A8 44983000 */  .word 0x44983000
    /* 800AD7AC 27A40068 */  addiu $a0, $sp, 104
    /* 800AD7B0 27A50080 */  addiu $a1, $sp, 128
    /* 800AD7B4 468032A0 */  .word 0x468032A0
    /* 800AD7B8 03C03025 */  or $a2, $fp, $zero
    /* 800AD7BC 46145102 */  .word 0x46145102
    /* 800AD7C0 E7A40050 */  swc1 $f4, 80($sp)
    /* 800AD7C4 946F0006 */  lhu $t7, 6($v1)
    /* 800AD7C8 84790002 */  lh $t9, 2($v1)
    /* 800AD7CC 31F803E0 */  andi $t8, $t7, 0x03E0
    /* 800AD7D0 00197140 */  sll $t6, $t9, 5
    /* 800AD7D4 0018C943 */  sra $t9, $t8, 5
    /* 800AD7D8 01D97821 */  addu $t7, $t6, $t9
    /* 800AD7DC 448F4000 */  .word 0x448F4000
    /* 800AD7E0 00000000 */  nop
    /* 800AD7E4 468041A0 */  .word 0x468041A0
    /* 800AD7E8 46143282 */  .word 0x46143282
    /* 800AD7EC E7AA0054 */  swc1 $f10, 84($sp)
    /* 800AD7F0 94790006 */  lhu $t9, 6($v1)
    /* 800AD7F4 84780004 */  lh $t8, 4($v1)
    /* 800AD7F8 332F001F */  andi $t7, $t9, 0x001F
    /* 800AD7FC 00187140 */  sll $t6, $t8, 5
    /* 800AD800 01CFC021 */  addu $t8, $t6, $t7
    /* 800AD804 44984000 */  .word 0x44984000
    /* 800AD808 00000000 */  nop
    /* 800AD80C 468041A0 */  .word 0x468041A0
    /* 800AD810 46143202 */  .word 0x46143202
    /* 800AD814 E7A80058 */  swc1 $f8, 88($sp)
    /* 800AD818 C6260000 */  lwc1 $f6, 0($s1)
    /* 800AD81C 46043181 */  .word 0x46043181
    /* 800AD820 E7A60068 */  swc1 $f6, 104($sp)
    /* 800AD824 C6240004 */  lwc1 $f4, 4($s1)
    /* 800AD828 460A2181 */  .word 0x460A2181
    /* 800AD82C E7A6006C */  swc1 $f6, 108($sp)
    /* 800AD830 C6240008 */  lwc1 $f4, 8($s1)
    /* 800AD834 46082281 */  .word 0x46082281
    /* 800AD838 0C02986C */  jal 0x800A61B0
    /* 800AD83C E7AA0070 */  swc1 $f10, 112($sp)
    /* 800AD840 C7AC0084 */  lwc1 $f12, 132($sp)
    /* 800AD844 4480B000 */  .word 0x4480B000
    /* 800AD848 27B10080 */  addiu $s1, $sp, 128
    /* 800AD84C 27B50068 */  addiu $s5, $sp, 104
    /* 800AD850 4616603C */  .word 0x4616603C
    /* 800AD854 C7A40050 */  lwc1 $f4, 80($sp)
    /* 800AD858 45020004 */  .word 0x45020004
    /* 800AD85C C6460000 */  lwc1 $f6, 0($s2)
    /* 800AD860 1000011B */  beq $zero, $zero, 0x800ADCD0
    /* 800AD864 00001025 */  or $v0, $zero, $zero
    /* 800AD868 C6460000 */  lwc1 $f6, 0($s2)
    /* 800AD86C 02A02025 */  or $a0, $s5, $zero
    /* 800AD870 27A50074 */  addiu $a1, $sp, 116
    /* 800AD874 46043201 */  .word 0x46043201
    /* 800AD878 C7A60054 */  lwc1 $f6, 84($sp)
    /* 800AD87C 03C03025 */  or $a2, $fp, $zero
    /* 800AD880 E7A80068 */  swc1 $f8, 104($sp)
    /* 800AD884 C64A0004 */  lwc1 $f10, 4($s2)
    /* 800AD888 46065101 */  .word 0x46065101
    /* 800AD88C C7AA0058 */  lwc1 $f10, 88($sp)
    /* 800AD890 E7A4006C */  swc1 $f4, 108($sp)
    /* 800AD894 C6480008 */  lwc1 $f8, 8($s2)
    /* 800AD898 460A4181 */  .word 0x460A4181
    /* 800AD89C 0C02986C */  jal 0x800A61B0
    /* 800AD8A0 E7A60070 */  swc1 $f6, 112($sp)
    /* 800AD8A4 C7A00078 */  lwc1 $f0, 120($sp)
    /* 800AD8A8 C7AC0084 */  lwc1 $f12, 132($sp)
    /* 800AD8AC C7AE0080 */  lwc1 $f14, 128($sp)
    /* 800AD8B0 4600B03C */  .word 0x4600B03C
    /* 800AD8B4 C7A40074 */  lwc1 $f4, 116($sp)
    /* 800AD8B8 45020004 */  .word 0x45020004
    /* 800AD8BC 460C0081 */  .word 0x460C0081
    /* 800AD8C0 10000103 */  beq $zero, $zero, 0x800ADCD0
    /* 800AD8C4 00001025 */  or $v0, $zero, $zero
    /* 800AD8C8 460C0081 */  .word 0x460C0081
    /* 800AD8CC C7B00088 */  lwc1 $f16, 136($sp)
    /* 800AD8D0 C7AA007C */  lwc1 $f10, 124($sp)
    /* 800AD8D4 460E2201 */  .word 0x460E2201
    /* 800AD8D8 0010C840 */  sll $t9, $s0, 1
    /* 800AD8DC 03B93821 */  addu $a3, $sp, $t9
    /* 800AD8E0 4602B03C */  .word 0x4602B03C
    /* 800AD8E4 E7A80068 */  swc1 $f8, 104($sp)
    /* 800AD8E8 46105181 */  .word 0x46105181
    /* 800AD8EC 3C058015 */  lui $a1, 0x8015
    /* 800AD8F0 45000003 */  .word 0x45000003
    /* 800AD8F4 E7A60070 */  swc1 $f6, 112($sp)
    /* 800AD8F8 100000F5 */  beq $zero, $zero, 0x800ADCD0
    /* 800AD8FC 00001025 */  or $v0, $zero, $zero
    /* 800AD900 4616103C */  .word 0x4616103C
    /* 800AD904 E7A2006C */  swc1 $f2, 108($sp)
    /* 800AD908 45020011 */  .word 0x45020011
    /* 800AD90C 94E7008E */  lhu $a3, 142($a3)
    /* 800AD910 46026003 */  .word 0x46026003
    /* 800AD914 C7A40068 */  lwc1 $f4, 104($sp)
    /* 800AD918 C7A60070 */  lwc1 $f6, 112($sp)
    /* 800AD91C E7A2006C */  swc1 $f2, 108($sp)
    /* 800AD920 46002202 */  .word 0x46002202
    /* 800AD924 00000000 */  nop
    /* 800AD928 46001282 */  .word 0x46001282
    /* 800AD92C 00000000 */  nop
    /* 800AD930 46003102 */  .word 0x46003102
    /* 800AD934 46087381 */  .word 0x46087381
    /* 800AD938 460A6301 */  .word 0x460A6301
    /* 800AD93C E7AE0080 */  swc1 $f14, 128($sp)
    /* 800AD940 46048401 */  .word 0x46048401
    /* 800AD944 E7AC0084 */  swc1 $f12, 132($sp)
    /* 800AD948 E7B00088 */  swc1 $f16, 136($sp)
    /* 800AD94C 94E7008E */  lhu $a3, 142($a3)
    /* 800AD950 8CA5201C */  lw $a1, 8220($a1)
    /* 800AD954 44801000 */  .word 0x44801000
    /* 800AD958 000770C0 */  sll $t6, $a3, 3
    /* 800AD95C 01C51821 */  addu $v1, $t6, $a1
    /* 800AD960 846F0000 */  lh $t7, 0($v1)
    /* 800AD964 94790006 */  lhu $t9, 6($v1)
    /* 800AD968 97A70092 */  lhu $a3, 146($sp)
    /* 800AD96C 000FC140 */  sll $t8, $t7, 5
    /* 800AD970 332E7C00 */  andi $t6, $t9, 0x7C00
    /* 800AD974 000E7A83 */  sra $t7, $t6, 10
    /* 800AD978 030FC821 */  addu $t9, $t8, $t7
    /* 800AD97C 44994000 */  .word 0x44994000
    /* 800AD980 00000000 */  nop
    /* 800AD984 468042A0 */  .word 0x468042A0
    /* 800AD988 46145182 */  .word 0x46145182
    /* 800AD98C E7A6005C */  swc1 $f6, 92($sp)
    /* 800AD990 946F0006 */  lhu $t7, 6($v1)
    /* 800AD994 846E0002 */  lh $t6, 2($v1)
    /* 800AD998 31F903E0 */  andi $t9, $t7, 0x03E0
    /* 800AD99C 000EC140 */  sll $t8, $t6, 5
    /* 800AD9A0 00197143 */  sra $t6, $t9, 5
    /* 800AD9A4 030E7821 */  addu $t7, $t8, $t6
    /* 800AD9A8 448F2000 */  .word 0x448F2000
    /* 800AD9AC 00000000 */  nop
    /* 800AD9B0 46802220 */  .word 0x46802220
    /* 800AD9B4 46144282 */  .word 0x46144282
    /* 800AD9B8 E7AA0060 */  swc1 $f10, 96($sp)
    /* 800AD9BC 946E0006 */  lhu $t6, 6($v1)
    /* 800AD9C0 84790004 */  lh $t9, 4($v1)
    /* 800AD9C4 31CF001F */  andi $t7, $t6, 0x001F
    /* 800AD9C8 0019C140 */  sll $t8, $t9, 5
    /* 800AD9CC 030FC821 */  addu $t9, $t8, $t7
    /* 800AD9D0 44992000 */  .word 0x44992000
    /* 800AD9D4 000770C0 */  sll $t6, $a3, 3
    /* 800AD9D8 01C51821 */  addu $v1, $t6, $a1
    /* 800AD9DC 46802220 */  .word 0x46802220
    /* 800AD9E0 C7A40088 */  lwc1 $f4, 136($sp)
    /* 800AD9E4 46144282 */  .word 0x46144282
    /* 800AD9E8 46045201 */  .word 0x46045201
    /* 800AD9EC E7AA0064 */  swc1 $f10, 100($sp)
    /* 800AD9F0 46064102 */  .word 0x46064102
    /* 800AD9F4 C7A80080 */  lwc1 $f8, 128($sp)
    /* 800AD9F8 46083181 */  .word 0x46083181
    /* 800AD9FC 46065202 */  .word 0x46065202
    /* 800ADA00 46082001 */  .word 0x46082001
    /* 800ADA04 4602003C */  .word 0x4602003C
    /* 800ADA08 00000000 */  nop
    /* 800ADA0C 45020010 */  .word 0x45020010
    /* 800ADA10 84780000 */  lh $t8, 0($v1)
    /* 800ADA14 1AC00003 */  blez $s6, 0x800ADA24
    /* 800ADA18 27A4005C */  addiu $a0, $sp, 92
    /* 800ADA1C 100000AC */  beq $zero, $zero, 0x800ADCD0
    /* 800ADA20 00001025 */  or $v0, $zero, $zero
    /* 800ADA24 02202825 */  or $a1, $s1, $zero
    /* 800ADA28 02E03025 */  or $a2, $s7, $zero
    /* 800ADA2C 0C02B532 */  jal 0x800AD4C8
    /* 800ADA30 4600C486 */  .word 0x4600C486
    /* 800ADA34 14400003 */  bne $v0, $zero, 0x800ADA44
    /* 800ADA38 00000000 */  nop
    /* 800ADA3C 100000A4 */  beq $zero, $zero, 0x800ADCD0
    /* 800ADA40 00001025 */  or $v0, $zero, $zero
    /* 800ADA44 1000008F */  beq $zero, $zero, 0x800ADC84
    /* 800ADA48 2414FFFF */  addiu $s4, $zero, -1
    /* 800ADA4C 84780000 */  lh $t8, 0($v1)
    /* 800ADA50 94790006 */  lhu $t9, 6($v1)
    /* 800ADA54 2E010003 */  sltiu $at, $s0, 3
    /* 800ADA58 00187940 */  sll $t7, $t8, 5
    /* 800ADA5C 332E7C00 */  andi $t6, $t9, 0x7C00
    /* 800ADA60 000EC283 */  sra $t8, $t6, 10
    /* 800ADA64 01F8C821 */  addu $t9, $t7, $t8
    /* 800ADA68 44995000 */  .word 0x44995000
    /* 800ADA6C 00000000 */  nop
    /* 800ADA70 468051A0 */  .word 0x468051A0
    /* 800ADA74 46143102 */  .word 0x46143102
    /* 800ADA78 E7A4005C */  swc1 $f4, 92($sp)
    /* 800ADA7C 94780006 */  lhu $t8, 6($v1)
    /* 800ADA80 846E0002 */  lh $t6, 2($v1)
    /* 800ADA84 331903E0 */  andi $t9, $t8, 0x03E0
    /* 800ADA88 000E7940 */  sll $t7, $t6, 5
    /* 800ADA8C 00197143 */  sra $t6, $t9, 5
    /* 800ADA90 01EEC021 */  addu $t8, $t7, $t6
    /* 800ADA94 44984000 */  .word 0x44984000
    /* 800ADA98 00000000 */  nop
    /* 800ADA9C 468042A0 */  .word 0x468042A0
    /* 800ADAA0 46145182 */  .word 0x46145182
    /* 800ADAA4 E7A60060 */  swc1 $f6, 96($sp)
    /* 800ADAA8 946E0006 */  lhu $t6, 6($v1)
    /* 800ADAAC 84790004 */  lh $t9, 4($v1)
    /* 800ADAB0 31D8001F */  andi $t8, $t6, 0x001F
    /* 800ADAB4 00197940 */  sll $t7, $t9, 5
    /* 800ADAB8 01F8C821 */  addu $t9, $t7, $t8
    /* 800ADABC 44994000 */  .word 0x44994000
    /* 800ADAC0 00000000 */  nop
    /* 800ADAC4 468042A0 */  .word 0x468042A0
    /* 800ADAC8 C7A80088 */  lwc1 $f8, 136($sp)
    /* 800ADACC 46145182 */  .word 0x46145182
    /* 800ADAD0 00000000 */  nop
    /* 800ADAD4 46044282 */  .word 0x46044282
    /* 800ADAD8 C7A80080 */  lwc1 $f8, 128($sp)
    /* 800ADADC 46083102 */  .word 0x46083102
    /* 800ADAE0 E7A60064 */  swc1 $f6, 100($sp)
    /* 800ADAE4 46045001 */  .word 0x46045001
    /* 800ADAE8 4602003C */  .word 0x4602003C
    /* 800ADAEC 00000000 */  nop
    /* 800ADAF0 4500000F */  .word 0x4500000F
    /* 800ADAF4 00000000 */  nop
    /* 800ADAF8 1AC00003 */  blez $s6, 0x800ADB08
    /* 800ADAFC 27A4005C */  addiu $a0, $sp, 92
    /* 800ADB00 10000073 */  beq $zero, $zero, 0x800ADCD0
    /* 800ADB04 00001025 */  or $v0, $zero, $zero
    /* 800ADB08 02202825 */  or $a1, $s1, $zero
    /* 800ADB0C 02E03025 */  or $a2, $s7, $zero
    /* 800ADB10 0C02B532 */  jal 0x800AD4C8
    /* 800ADB14 4600C486 */  .word 0x4600C486
    /* 800ADB18 14400003 */  bne $v0, $zero, 0x800ADB28
    /* 800ADB1C 00000000 */  nop
    /* 800ADB20 1000006B */  beq $zero, $zero, 0x800ADCD0
    /* 800ADB24 00001025 */  or $v0, $zero, $zero
    /* 800ADB28 10000056 */  beq $zero, $zero, 0x800ADC84
    /* 800ADB2C 2414FFFF */  addiu $s4, $zero, -1
    /* 800ADB30 14200054 */  bne $at, $zero, 0x800ADC84
    /* 800ADB34 24020002 */  addiu $v0, $zero, 2
    /* 800ADB38 27A40094 */  addiu $a0, $sp, 148
    /* 800ADB3C C7A6005C */  lwc1 $f6, 92($sp)
    /* 800ADB40 C7A80064 */  lwc1 $f8, 100($sp)
    /* 800ADB44 24420001 */  addiu $v0, $v0, 1
    /* 800ADB48 E7A60044 */  swc1 $f6, 68($sp)
    /* 800ADB4C E7A8004C */  swc1 $f8, 76($sp)
    /* 800ADB50 94870000 */  lhu $a3, 0($a0)
    /* 800ADB54 0050082B */  sltu $at, $v0, $s0
    /* 800ADB58 000770C0 */  sll $t6, $a3, 3
    /* 800ADB5C 01C51821 */  addu $v1, $t6, $a1
    /* 800ADB60 846F0000 */  lh $t7, 0($v1)
    /* 800ADB64 94790006 */  lhu $t9, 6($v1)
    /* 800ADB68 000FC140 */  sll $t8, $t7, 5
    /* 800ADB6C 332E7C00 */  andi $t6, $t9, 0x7C00
    /* 800ADB70 000E7A83 */  sra $t7, $t6, 10
    /* 800ADB74 030FC821 */  addu $t9, $t8, $t7
    /* 800ADB78 44995000 */  .word 0x44995000
    /* 800ADB7C 00000000 */  nop
    /* 800ADB80 46805120 */  .word 0x46805120
    /* 800ADB84 46142182 */  .word 0x46142182
    /* 800ADB88 E7A6005C */  swc1 $f6, 92($sp)
    /* 800ADB8C 946F0006 */  lhu $t7, 6($v1)
    /* 800ADB90 846E0002 */  lh $t6, 2($v1)
    /* 800ADB94 31F903E0 */  andi $t9, $t7, 0x03E0
    /* 800ADB98 000EC140 */  sll $t8, $t6, 5
    /* 800ADB9C 00197143 */  sra $t6, $t9, 5
    /* 800ADBA0 030E7821 */  addu $t7, $t8, $t6
    /* 800ADBA4 448F4000 */  .word 0x448F4000
    /* 800ADBA8 00000000 */  nop
    /* 800ADBAC 468042A0 */  .word 0x468042A0
    /* 800ADBB0 46145102 */  .word 0x46145102
    /* 800ADBB4 E7A40060 */  swc1 $f4, 96($sp)
    /* 800ADBB8 946E0006 */  lhu $t6, 6($v1)
    /* 800ADBBC 84790004 */  lh $t9, 4($v1)
    /* 800ADBC0 E7B6006C */  swc1 $f22, 108($sp)
    /* 800ADBC4 31CF001F */  andi $t7, $t6, 0x001F
    /* 800ADBC8 0019C140 */  sll $t8, $t9, 5
    /* 800ADBCC 030FC821 */  addu $t9, $t8, $t7
    /* 800ADBD0 44994000 */  .word 0x44994000
    /* 800ADBD4 E7B6002C */  swc1 $f22, 44($sp)
    /* 800ADBD8 468042A0 */  .word 0x468042A0
    /* 800ADBDC C7A80044 */  lwc1 $f8, 68($sp)
    /* 800ADBE0 E7A80018 */  swc1 $f8, 24($sp)
    /* 800ADBE4 46145102 */  .word 0x46145102
    /* 800ADBE8 46083281 */  .word 0x46083281
    /* 800ADBEC C7A80080 */  lwc1 $f8, 128($sp)
    /* 800ADBF0 C7A6004C */  lwc1 $f6, 76($sp)
    /* 800ADBF4 E7AA0068 */  swc1 $f10, 104($sp)
    /* 800ADBF8 E7AA001C */  swc1 $f10, 28($sp)
    /* 800ADBFC C7AA0018 */  lwc1 $f10, 24($sp)
    /* 800ADC00 E7A40064 */  swc1 $f4, 100($sp)
    /* 800ADC04 46062101 */  .word 0x46062101
    /* 800ADC08 460A4201 */  .word 0x460A4201
    /* 800ADC0C C7AA0088 */  lwc1 $f10, 136($sp)
    /* 800ADC10 E7A40070 */  swc1 $f4, 112($sp)
    /* 800ADC14 46065281 */  .word 0x46065281
    /* 800ADC18 46082182 */  .word 0x46082182
    /* 800ADC1C E7A80028 */  swc1 $f8, 40($sp)
    /* 800ADC20 E7AA0030 */  swc1 $f10, 48($sp)
    /* 800ADC24 C7A40030 */  lwc1 $f4, 48($sp)
    /* 800ADC28 C7AA001C */  lwc1 $f10, 28($sp)
    /* 800ADC2C 460A2202 */  .word 0x460A2202
    /* 800ADC30 46064001 */  .word 0x46064001
    /* 800ADC34 4602003C */  .word 0x4602003C
    /* 800ADC38 00000000 */  nop
    /* 800ADC3C 4500000F */  .word 0x4500000F
    /* 800ADC40 00000000 */  nop
    /* 800ADC44 1AC00003 */  blez $s6, 0x800ADC54
    /* 800ADC48 02A02025 */  or $a0, $s5, $zero
    /* 800ADC4C 10000020 */  beq $zero, $zero, 0x800ADCD0
    /* 800ADC50 00001025 */  or $v0, $zero, $zero
    /* 800ADC54 27A50028 */  addiu $a1, $sp, 40
    /* 800ADC58 02E03025 */  or $a2, $s7, $zero
    /* 800ADC5C 0C02B532 */  jal 0x800AD4C8
    /* 800ADC60 4600C486 */  .word 0x4600C486
    /* 800ADC64 14400003 */  bne $v0, $zero, 0x800ADC74
    /* 800ADC68 00000000 */  nop
    /* 800ADC6C 10000018 */  beq $zero, $zero, 0x800ADCD0
    /* 800ADC70 00001025 */  or $v0, $zero, $zero
    /* 800ADC74 10000003 */  beq $zero, $zero, 0x800ADC84
    /* 800ADC78 2414FFFF */  addiu $s4, $zero, -1
    /* 800ADC7C 1420FFAF */  bne $at, $zero, 0x800ADB3C
    /* 800ADC80 24840002 */  addiu $a0, $a0, 2
    /* 800ADC84 02202025 */  or $a0, $s1, $zero
    /* 800ADC88 02602825 */  or $a1, $s3, $zero
    /* 800ADC8C 0C027A08 */  jal 0x8009E820
    /* 800ADC90 03C03025 */  or $a2, $fp, $zero
    /* 800ADC94 C7A40050 */  lwc1 $f4, 80($sp)
    /* 800ADC98 C66A0000 */  lwc1 $f10, 0($s3)
    /* 800ADC9C 00141400 */  sll $v0, $s4, 16
    /* 800ADCA0 00027403 */  sra $t6, $v0, 16
    /* 800ADCA4 460A2200 */  .word 0x460A2200
    /* 800ADCA8 C6640004 */  lwc1 $f4, 4($s3)
    /* 800ADCAC 01C01025 */  or $v0, $t6, $zero
    /* 800ADCB0 E6680000 */  swc1 $f8, 0($s3)
    /* 800ADCB4 C7A60054 */  lwc1 $f6, 84($sp)
    /* 800ADCB8 46043280 */  .word 0x46043280
    /* 800ADCBC C6660008 */  lwc1 $f6, 8($s3)
    /* 800ADCC0 E66A0004 */  swc1 $f10, 4($s3)
    /* 800ADCC4 C7A80058 */  lwc1 $f8, 88($sp)
    /* 800ADCC8 46064100 */  .word 0x46064100
    /* 800ADCCC E6640008 */  swc1 $f4, 8($s3)
    /* 800ADCD0 8FBF0014 */  lw $ra, 20($sp)
    /* 800ADCD4 27BD00B8 */  addiu $sp, $sp, 184
    /* 800ADCD8 03E00008 */  jr $ra
    /* 800ADCDC 00000000 */  nop
