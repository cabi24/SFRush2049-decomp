# Source: game_code.bin (decompressed)
# Address: 0x800BEAA0

glabel race_setup_2
    /* 800BEAA0 27BDFF78 */  addiu $sp, $sp, -136
    /* 800BEAA4 AFA40088 */  sw $a0, 136($sp)
    /* 800BEAA8 87AE008A */  lh $t6, 138($sp)
    /* 800BEAAC 3C088011 */  lui $t0, 0x8011
    /* 800BEAB0 8D0874B4 */  lw $t0, 29876($t0)
    /* 800BEAB4 000E7900 */  sll $t7, $t6, 4
    /* 800BEAB8 01EE7823 */  subu $t7, $t7, $t6
    /* 800BEABC 000F78C0 */  sll $t7, $t7, 3
    /* 800BEAC0 3C188015 */  lui $t8, 0x8015
    /* 800BEAC4 01EE7823 */  subu $t7, $t7, $t6
    /* 800BEAC8 000F78C0 */  sll $t7, $t7, 3
    /* 800BEACC 27182818 */  addiu $t8, $t8, 10264
    /* 800BEAD0 01F8C821 */  addu $t9, $t7, $t8
    /* 800BEAD4 31090008 */  andi $t1, $t0, 0x0008
    /* 800BEAD8 AFBF0034 */  sw $ra, 52($sp)
    /* 800BEADC AFB70030 */  sw $s7, 48($sp)
    /* 800BEAE0 AFB6002C */  sw $s6, 44($sp)
    /* 800BEAE4 AFB50028 */  sw $s5, 40($sp)
    /* 800BEAE8 AFB40024 */  sw $s4, 36($sp)
    /* 800BEAEC AFB30020 */  sw $s3, 32($sp)
    /* 800BEAF0 AFB2001C */  sw $s2, 28($sp)
    /* 800BEAF4 AFB10018 */  sw $s1, 24($sp)
    /* 800BEAF8 AFB00014 */  sw $s0, 20($sp)
    /* 800BEAFC 11200004 */  beq $t1, $zero, 0x800BEB10
    /* 800BEB00 AFB9003C */  sw $t9, 60($sp)
    /* 800BEB04 3C0A8015 */  lui $t2, 0x8015
    /* 800BEB08 814A6994 */  lb $t2, 27028($t2)
    /* 800BEB0C 114000BC */  beq $t2, $zero, 0x800BEE00
    /* 800BEB10 3C0B8015 */  lui $t3, 0x8015
    /* 800BEB14 8D6BA110 */  lw $t3, -24304($t3)
    /* 800BEB18 24010006 */  addiu $at, $zero, 6
    /* 800BEB1C 3C128015 */  lui $s2, 0x8015
    /* 800BEB20 5561002C */  .word 0x5561002C
    /* 800BEB24 8FAB003C */  lw $t3, 60($sp)
    /* 800BEB28 8E521AB8 */  lw $s2, 6840($s2)
    /* 800BEB2C 3C178011 */  lui $s7, 0x8011
    /* 800BEB30 26F77518 */  addiu $s7, $s7, 29976
    /* 800BEB34 12400026 */  beq $s2, $zero, 0x800BEBD0
    /* 800BEB38 24160030 */  addiu $s6, $zero, 48
    /* 800BEB3C 3C158011 */  lui $s5, 0x8011
    /* 800BEB40 26B57530 */  addiu $s5, $s5, 30000
    /* 800BEB44 27B4008A */  addiu $s4, $sp, 138
    /* 800BEB48 8E500004 */  lw $s0, 4($s2)
    /* 800BEB4C 8E530000 */  lw $s3, 0($s2)
    /* 800BEB50 02802025 */  or $a0, $s4, $zero
    /* 800BEB54 860C0010 */  lh $t4, 16($s0)
    /* 800BEB58 00003825 */  or $a3, $zero, $zero
    /* 800BEB5C 26050044 */  addiu $a1, $s0, 68
    /* 800BEB60 01960019 */  multu $t4, $s6
    /* 800BEB64 26060054 */  addiu $a2, $s0, 84
    /* 800BEB68 00006812 */  mflo $t5
    /* 800BEB6C 02AD8821 */  addu $s1, $s5, $t5
    /* 800BEB70 862E0010 */  lh $t6, 16($s1)
    /* 800BEB74 000E7880 */  sll $t7, $t6, 2
    /* 800BEB78 02EFC021 */  addu $t8, $s7, $t7
    /* 800BEB7C 8F190000 */  lw $t9, 0($t8)
    /* 800BEB80 0320F809 */  jalr $ra, $t9
    /* 800BEB84 00000000 */  nop
    /* 800BEB88 1040000F */  beq $v0, $zero, 0x800BEBC8
    /* 800BEB8C 00000000 */  nop
    /* 800BEB90 92080004 */  lbu $t0, 4($s0)
    /* 800BEB94 35090004 */  ori $t1, $t0, 0x0004
    /* 800BEB98 A2090004 */  sb $t1, 4($s0)
    /* 800BEB9C 87AA008A */  lh $t2, 138($sp)
    /* 800BEBA0 A20A005C */  sb $t2, 92($s0)
    /* 800BEBA4 8E220008 */  lw $v0, 8($s1)
    /* 800BEBA8 10400003 */  beq $v0, $zero, 0x800BEBB8
    /* 800BEBAC 00000000 */  nop
    /* 800BEBB0 0040F809 */  jalr $ra, $v0
    /* 800BEBB4 02002025 */  or $a0, $s0, $zero
    /* 800BEBB8 3C048015 */  lui $a0, 0x8015
    /* 800BEBBC 24841AA8 */  addiu $a0, $a0, 6824
    /* 800BEBC0 0C02BEA1 */  jal 0x800AFA84
    /* 800BEBC4 02402825 */  or $a1, $s2, $zero
    /* 800BEBC8 1660FFDF */  bne $s3, $zero, 0x800BEB48
    /* 800BEBCC 02609025 */  or $s2, $s3, $zero
    /* 800BEBD0 8FAB003C */  lw $t3, 60($sp)
    /* 800BEBD4 3C158011 */  lui $s5, 0x8011
    /* 800BEBD8 3C178011 */  lui $s7, 0x8011
    /* 800BEBDC C5640008 */  lwc1 $f4, 8($t3)
    /* 800BEBE0 3C048015 */  lui $a0, 0x8015
    /* 800BEBE4 26F77518 */  addiu $s7, $s7, 29976
    /* 800BEBE8 E7A40064 */  swc1 $f4, 100($sp)
    /* 800BEBEC C566000C */  lwc1 $f6, 12($t3)
    /* 800BEBF0 26B57530 */  addiu $s5, $s5, 30000
    /* 800BEBF4 27B4008A */  addiu $s4, $sp, 138
    /* 800BEBF8 E7A60068 */  swc1 $f6, 104($sp)
    /* 800BEBFC C5680010 */  lwc1 $f8, 16($t3)
    /* 800BEC00 24160030 */  addiu $s6, $zero, 48
    /* 800BEC04 00001025 */  or $v0, $zero, $zero
    /* 800BEC08 8C849770 */  lw $a0, -26768($a0)
    /* 800BEC0C 2406001C */  addiu $a2, $zero, 28
    /* 800BEC10 2405001C */  addiu $a1, $zero, 28
    /* 800BEC14 E7A8006C */  swc1 $f8, 108($sp)
    /* 800BEC18 00450019 */  multu $v0, $a1
    /* 800BEC1C C7AA0064 */  lwc1 $f10, 100($sp)
    /* 800BEC20 C7A4006C */  lwc1 $f4, 108($sp)
    /* 800BEC24 00006012 */  mflo $t4
    /* 800BEC28 01849021 */  addu $s2, $t4, $a0
    /* 800BEC2C C6500004 */  lwc1 $f16, 4($s2)
    /* 800BEC30 460A803E */  .word 0x460A803E
    /* 800BEC34 00000000 */  nop
    /* 800BEC38 45020072 */  .word 0x45020072
    /* 800BEC3C 8FBF0034 */  lw $ra, 52($sp)
    /* 800BEC40 C6520008 */  lwc1 $f18, 8($s2)
    /* 800BEC44 4612503C */  .word 0x4612503C
    /* 800BEC48 00000000 */  nop
    /* 800BEC4C 4502006D */  .word 0x4502006D
    /* 800BEC50 8FBF0034 */  lw $ra, 52($sp)
    /* 800BEC54 C646000C */  lwc1 $f6, 12($s2)
    /* 800BEC58 4604303E */  .word 0x4604303E
    /* 800BEC5C 00000000 */  nop
    /* 800BEC60 45020068 */  .word 0x45020068
    /* 800BEC64 8FBF0034 */  lw $ra, 52($sp)
    /* 800BEC68 C6480010 */  lwc1 $f8, 16($s2)
    /* 800BEC6C C7B00064 */  lwc1 $f16, 100($sp)
    /* 800BEC70 4608203C */  .word 0x4608203C
    /* 800BEC74 00000000 */  nop
    /* 800BEC78 45020062 */  .word 0x45020062
    /* 800BEC7C 8FBF0034 */  lw $ra, 52($sp)
    /* 800BEC80 924D0003 */  lbu $t5, 3($s2)
    /* 800BEC84 31AE0001 */  andi $t6, $t5, 0x0001
    /* 800BEC88 51C0001C */  .word 0x51C0001C
    /* 800BEC8C 96430014 */  lhu $v1, 20($s2)
    /* 800BEC90 96430018 */  lhu $v1, 24($s2)
    /* 800BEC94 00660019 */  multu $v1, $a2
    /* 800BEC98 00007812 */  mflo $t7
    /* 800BEC9C 008F1021 */  addu $v0, $a0, $t7
    /* 800BECA0 C4400010 */  lwc1 $f0, 16($v0)
    /* 800BECA4 C4420008 */  lwc1 $f2, 8($v0)
    /* 800BECA8 4604003E */  .word 0x4604003E
    /* 800BECAC 00000000 */  nop
    /* 800BECB0 4502000A */  .word 0x4502000A
    /* 800BECB4 4602803C */  .word 0x4602803C
    /* 800BECB8 4602503C */  .word 0x4602503C
    /* 800BECBC 00000000 */  nop
    /* 800BECC0 45000003 */  .word 0x45000003
    /* 800BECC4 00000000 */  nop
    /* 800BECC8 1000FFD3 */  beq $zero, $zero, 0x800BEC18
    /* 800BECCC 96420014 */  lhu $v0, 20($s2)
    /* 800BECD0 1000FFD1 */  beq $zero, $zero, 0x800BEC18
    /* 800BECD4 96420016 */  lhu $v0, 22($s2)
    /* 800BECD8 4602803C */  .word 0x4602803C
    /* 800BECDC 00000000 */  nop
    /* 800BECE0 45000003 */  .word 0x45000003
    /* 800BECE4 00000000 */  nop
    /* 800BECE8 1000FFCB */  beq $zero, $zero, 0x800BEC18
    /* 800BECEC 00601025 */  or $v0, $v1, $zero
    /* 800BECF0 1000FFC9 */  beq $zero, $zero, 0x800BEC18
    /* 800BECF4 9642001A */  lhu $v0, 26($s2)
    /* 800BECF8 96430014 */  lhu $v1, 20($s2)
    /* 800BECFC 00009825 */  or $s3, $zero, $zero
    /* 800BED00 58600040 */  .word 0x58600040
    /* 800BED04 8FBF0034 */  lw $ra, 52($sp)
    /* 800BED08 96590016 */  lhu $t9, 22($s2)
    /* 800BED0C 3C188015 */  lui $t8, 0x8015
    /* 800BED10 8F1897C0 */  lw $t8, -26688($t8)
    /* 800BED14 00194080 */  sll $t0, $t9, 2
    /* 800BED18 03084821 */  addu $t1, $t8, $t0
    /* 800BED1C 01335021 */  addu $t2, $t1, $s3
    /* 800BED20 8D500000 */  lw $s0, 0($t2)
    /* 800BED24 52000031 */  .word 0x52000031
    /* 800BED28 96430014 */  lhu $v1, 20($s2)
    /* 800BED2C 92020004 */  lbu $v0, 4($s0)
    /* 800BED30 304B0002 */  andi $t3, $v0, 0x0002
    /* 800BED34 1160002C */  beq $t3, $zero, 0x800BEDE8
    /* 800BED38 304C0008 */  andi $t4, $v0, 0x0008
    /* 800BED3C 5180002B */  .word 0x5180002B
    /* 800BED40 96430014 */  lhu $v1, 20($s2)
    /* 800BED44 860D0010 */  lh $t5, 16($s0)
    /* 800BED48 01B60019 */  multu $t5, $s6
    /* 800BED4C 00007012 */  mflo $t6
    /* 800BED50 02AE8821 */  addu $s1, $s5, $t6
    /* 800BED54 86220010 */  lh $v0, 16($s1)
    /* 800BED58 2C410006 */  sltiu $at, $v0, 6
    /* 800BED5C 1020000E */  beq $at, $zero, 0x800BED98
    /* 800BED60 00027880 */  sll $t7, $v0, 2
    /* 800BED64 3C018012 */  lui $at, 0x8012
    /* 800BED68 002F0821 */  addu $at, $at, $t7
    /* 800BED6C 8C2F3E5C */  lw $t7, 15964($at)
    /* 800BED70 01E00008 */  jr $t7
    /* 800BED74 00000000 */  nop
    /* 800BED78 02802025 */  or $a0, $s4, $zero
    /* 800BED7C 26050044 */  addiu $a1, $s0, 68
    /* 800BED80 10000007 */  beq $zero, $zero, 0x800BEDA0
    /* 800BED84 26060054 */  addiu $a2, $s0, 84
    /* 800BED88 02002025 */  or $a0, $s0, $zero
    /* 800BED8C 02802825 */  or $a1, $s4, $zero
    /* 800BED90 10000003 */  beq $zero, $zero, 0x800BEDA0
    /* 800BED94 00003025 */  or $a2, $zero, $zero
    /* 800BED98 10000015 */  beq $zero, $zero, 0x800BEDF0
    /* 800BED9C 00031080 */  sll $v0, $v1, 2
    /* 800BEDA0 0002C880 */  sll $t9, $v0, 2
    /* 800BEDA4 02F9C021 */  addu $t8, $s7, $t9
    /* 800BEDA8 8F190000 */  lw $t9, 0($t8)
    /* 800BEDAC 00003825 */  or $a3, $zero, $zero
    /* 800BEDB0 0320F809 */  jalr $ra, $t9
    /* 800BEDB4 00000000 */  nop
    /* 800BEDB8 5040000C */  .word 0x5040000C
    /* 800BEDBC 96430014 */  lhu $v1, 20($s2)
    /* 800BEDC0 92080004 */  lbu $t0, 4($s0)
    /* 800BEDC4 35090004 */  ori $t1, $t0, 0x0004
    /* 800BEDC8 A2090004 */  sb $t1, 4($s0)
    /* 800BEDCC 87AA008A */  lh $t2, 138($sp)
    /* 800BEDD0 A20A005C */  sb $t2, 92($s0)
    /* 800BEDD4 8E220008 */  lw $v0, 8($s1)
    /* 800BEDD8 50400004 */  .word 0x50400004
    /* 800BEDDC 96430014 */  lhu $v1, 20($s2)
    /* 800BEDE0 0040F809 */  jalr $ra, $v0
    /* 800BEDE4 02002025 */  or $a0, $s0, $zero
    /* 800BEDE8 96430014 */  lhu $v1, 20($s2)
    /* 800BEDEC 00031080 */  sll $v0, $v1, 2
    /* 800BEDF0 26730004 */  addiu $s3, $s3, 4
    /* 800BEDF4 0262082A */  slt $at, $s3, $v0
    /* 800BEDF8 5420FFC4 */  .word 0x5420FFC4
    /* 800BEDFC 96590016 */  lhu $t9, 22($s2)
    /* 800BEE00 8FBF0034 */  lw $ra, 52($sp)
    /* 800BEE04 8FB00014 */  lw $s0, 20($sp)
    /* 800BEE08 8FB10018 */  lw $s1, 24($sp)
    /* 800BEE0C 8FB2001C */  lw $s2, 28($sp)
    /* 800BEE10 8FB30020 */  lw $s3, 32($sp)
    /* 800BEE14 8FB40024 */  lw $s4, 36($sp)
    /* 800BEE18 8FB50028 */  lw $s5, 40($sp)
    /* 800BEE1C 8FB6002C */  lw $s6, 44($sp)
    /* 800BEE20 8FB70030 */  lw $s7, 48($sp)
    /* 800BEE24 03E00008 */  jr $ra
    /* 800BEE28 27BD0088 */  addiu $sp, $sp, 136
