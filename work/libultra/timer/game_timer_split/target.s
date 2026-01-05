# Source: game_code.bin (decompressed)
# Address: 0x800FEA08

glabel game_timer_split
    /* 800FEA08 27BDFFA0 */  addiu $sp, $sp, -96
    /* 800FEA0C AFA7006C */  sw $a3, 108($sp)
    /* 800FEA10 30EE00FF */  andi $t6, $a3, 0x00FF
    /* 800FEA14 01C03825 */  or $a3, $t6, $zero
    /* 800FEA18 AFBF0024 */  sw $ra, 36($sp)
    /* 800FEA1C 00805025 */  or $t2, $a0, $zero
    /* 800FEA20 00A04825 */  or $t1, $a1, $zero
    /* 800FEA24 15E00003 */  bne $t7, $zero, 0x800FEA34
    /* 800FEA28 00C04025 */  or $t0, $a2, $zero
    /* 800FEA2C 10000088 */  beq $zero, $zero, 0x800FEC50
    /* 800FEA30 2402FFFF */  addiu $v0, $zero, -1
    /* 800FEA34 0009C0C0 */  sll $t8, $t1, 3
    /* 800FEA38 3C198015 */  lui $t9, 0x8015
    /* 800FEA3C 0338C821 */  addu $t9, $t9, $t8
    /* 800FEA40 93393E8F */  lbu $t9, 16015($t9)
    /* 800FEA44 24010006 */  addiu $at, $zero, 6
    /* 800FEA48 00095900 */  sll $t3, $t1, 4
    /* 800FEA4C 13210003 */  beq $t9, $at, 0x800FEA5C
    /* 800FEA50 01695823 */  subu $t3, $t3, $t1
    /* 800FEA54 1000007E */  beq $zero, $zero, 0x800FEC50
    /* 800FEA58 2402FFFF */  addiu $v0, $zero, -1
    /* 800FEA5C 000B58C0 */  sll $t3, $t3, 3
    /* 800FEA60 01695823 */  subu $t3, $t3, $t1
    /* 800FEA64 3C0C8015 */  lui $t4, 0x8015
    /* 800FEA68 258C2818 */  addiu $t4, $t4, 10264
    /* 800FEA6C 000B58C0 */  sll $t3, $t3, 3
    /* 800FEA70 016C1021 */  addu $v0, $t3, $t4
    /* 800FEA74 C4460008 */  lwc1 $f6, 8($v0)
    /* 800FEA78 C5040000 */  lwc1 $f4, 0($t0)
    /* 800FEA7C 2446002C */  addiu $a2, $v0, 44
    /* 800FEA80 27A40044 */  addiu $a0, $sp, 68
    /* 800FEA84 46062201 */  .word 0x46062201
    /* 800FEA88 27A50050 */  addiu $a1, $sp, 80
    /* 800FEA8C E7A80044 */  swc1 $f8, 68($sp)
    /* 800FEA90 C444000C */  lwc1 $f4, 12($v0)
    /* 800FEA94 C50A0004 */  lwc1 $f10, 4($t0)
    /* 800FEA98 46045181 */  .word 0x46045181
    /* 800FEA9C E7A60048 */  swc1 $f6, 72($sp)
    /* 800FEAA0 C44A0010 */  lwc1 $f10, 16($v0)
    /* 800FEAA4 C5080008 */  lwc1 $f8, 8($t0)
    /* 800FEAA8 AFAA0060 */  sw $t2, 96($sp)
    /* 800FEAAC AFA90064 */  sw $t1, 100($sp)
    /* 800FEAB0 460A4101 */  .word 0x460A4101
    /* 800FEAB4 A3A7006F */  sb $a3, 111($sp)
    /* 800FEAB8 0C02986C */  jal 0x800A61B0
    /* 800FEABC E7A4004C */  swc1 $f4, 76($sp)
    /* 800FEAC0 C7AE0058 */  lwc1 $f14, 88($sp)
    /* 800FEAC4 C7B00050 */  lwc1 $f16, 80($sp)
    /* 800FEAC8 3C013F80 */  lui $at, 0x3F80
    /* 800FEACC 460E7302 */  .word 0x460E7302
    /* 800FEAD0 44813000 */  .word 0x44813000
    /* 800FEAD4 8FA90064 */  lw $t1, 100($sp)
    /* 800FEAD8 46108082 */  .word 0x46108082
    /* 800FEADC 8FAA0060 */  lw $t2, 96($sp)
    /* 800FEAE0 93A7006F */  lbu $a3, 111($sp)
    /* 800FEAE4 00001825 */  or $v1, $zero, $zero
    /* 800FEAE8 24060001 */  addiu $a2, $zero, 1
    /* 800FEAEC 3C0F8012 */  lui $t7, 0x8012
    /* 800FEAF0 3C188012 */  lui $t8, 0x8012
    /* 800FEAF4 46026480 */  .word 0x46026480
    /* 800FEAF8 01202825 */  or $a1, $t1, $zero
    /* 800FEAFC 01402025 */  or $a0, $t2, $zero
    /* 800FEB00 4612303C */  .word 0x4612303C
    /* 800FEB04 00000000 */  nop
    /* 800FEB08 45020034 */  .word 0x45020034
    /* 800FEB0C 24010006 */  addiu $at, $zero, 6
    /* 800FEB10 44804000 */  .word 0x44804000
    /* 800FEB14 44803000 */  .word 0x44803000
    /* 800FEB18 2402FFFF */  addiu $v0, $zero, -1
    /* 800FEB1C 460E403E */  .word 0x460E403E
    /* 800FEB20 00000000 */  nop
    /* 800FEB24 45000003 */  .word 0x45000003
    /* 800FEB28 00000000 */  nop
    /* 800FEB2C 10000001 */  beq $zero, $zero, 0x800FEB34
    /* 800FEB30 24020001 */  addiu $v0, $zero, 1
    /* 800FEB34 44825000 */  .word 0x44825000
    /* 800FEB38 4610303E */  .word 0x4610303E
    /* 800FEB3C 2402FFFF */  addiu $v0, $zero, -1
    /* 800FEB40 46805120 */  .word 0x46805120
    /* 800FEB44 46046302 */  .word 0x46046302
    /* 800FEB48 45000003 */  .word 0x45000003
    /* 800FEB4C 00000000 */  nop
    /* 800FEB50 10000001 */  beq $zero, $zero, 0x800FEB58
    /* 800FEB54 24020001 */  addiu $v0, $zero, 1
    /* 800FEB58 44824000 */  .word 0x44824000
    /* 800FEB5C 3C018012 */  lui $at, 0x8012
    /* 800FEB60 C4244824 */  lwc1 $f4, 18468($at)
    /* 800FEB64 468042A0 */  .word 0x468042A0
    /* 800FEB68 460A1082 */  .word 0x460A1082
    /* 800FEB6C 00000000 */  nop
    /* 800FEB70 46122002 */  .word 0x46122002
    /* 800FEB74 4602003C */  .word 0x4602003C
    /* 800FEB78 00000000 */  nop
    /* 800FEB7C 45000003 */  .word 0x45000003
    /* 800FEB80 00000000 */  nop
    /* 800FEB84 10000007 */  beq $zero, $zero, 0x800FEBA4
    /* 800FEB88 24030004 */  addiu $v1, $zero, 4
    /* 800FEB8C 46000187 */  .word 0x46000187
    /* 800FEB90 4606103C */  .word 0x4606103C
    /* 800FEB94 00000000 */  nop
    /* 800FEB98 45020003 */  .word 0x45020003
    /* 800FEB9C 460C003C */  .word 0x460C003C
    /* 800FEBA0 24030008 */  addiu $v1, $zero, 8
    /* 800FEBA4 460C003C */  .word 0x460C003C
    /* 800FEBA8 346D0001 */  ori $t5, $v1, 0x0001
    /* 800FEBAC 45020004 */  .word 0x45020004
    /* 800FEBB0 46000207 */  .word 0x46000207
    /* 800FEBB4 10000008 */  beq $zero, $zero, 0x800FEBD8
    /* 800FEBB8 01A01825 */  or $v1, $t5, $zero
    /* 800FEBBC 46000207 */  .word 0x46000207
    /* 800FEBC0 346E0002 */  ori $t6, $v1, 0x0002
    /* 800FEBC4 4608603C */  .word 0x4608603C
    /* 800FEBC8 00000000 */  nop
    /* 800FEBCC 45020003 */  .word 0x45020003
    /* 800FEBD0 24010006 */  addiu $at, $zero, 6
    /* 800FEBD4 01C01825 */  or $v1, $t6, $zero
    /* 800FEBD8 24010006 */  addiu $at, $zero, 6
    /* 800FEBDC 1541000B */  bne $t2, $at, 0x800FEC0C
    /* 800FEBE0 00031040 */  sll $v0, $v1, 1
    /* 800FEBE4 3C013F80 */  lui $at, 0x3F80
    /* 800FEBE8 44815000 */  .word 0x44815000
    /* 800FEBEC 44802000 */  .word 0x44802000
    /* 800FEBF0 44803000 */  .word 0x44803000
    /* 800FEBF4 E7AA0010 */  swc1 $f10, 16($sp)
    /* 800FEBF8 E7A40014 */  swc1 $f4, 20($sp)
    /* 800FEBFC 0C035724 */  jal 0x800D5C90
    /* 800FEC00 E7A60018 */  swc1 $f6, 24($sp)
    /* 800FEC04 10000013 */  beq $zero, $zero, 0x800FEC54
    /* 800FEC08 8FBF0024 */  lw $ra, 36($sp)
    /* 800FEC0C 01E27821 */  addu $t7, $t7, $v0
    /* 800FEC10 0302C021 */  addu $t8, $t8, $v0
    /* 800FEC14 8718F040 */  lh $t8, -4032($t8)
    /* 800FEC18 85EFF020 */  lh $t7, -4064($t7)
    /* 800FEC1C 3C013F80 */  lui $at, 0x3F80
    /* 800FEC20 44814000 */  .word 0x44814000
    /* 800FEC24 44983000 */  .word 0x44983000
    /* 800FEC28 448F5000 */  .word 0x448F5000
    /* 800FEC2C E7A80010 */  swc1 $f8, 16($sp)
    /* 800FEC30 46803220 */  .word 0x46803220
    /* 800FEC34 01402025 */  or $a0, $t2, $zero
    /* 800FEC38 01202825 */  or $a1, $t1, $zero
    /* 800FEC3C 24060001 */  addiu $a2, $zero, 1
    /* 800FEC40 46805120 */  .word 0x46805120
    /* 800FEC44 E7A80018 */  swc1 $f8, 24($sp)
    /* 800FEC48 0C035724 */  jal 0x800D5C90
    /* 800FEC4C E7A40014 */  swc1 $f4, 20($sp)
    /* 800FEC50 8FBF0024 */  lw $ra, 36($sp)
    /* 800FEC54 27BD0060 */  addiu $sp, $sp, 96
    /* 800FEC58 03E00008 */  jr $ra
    /* 800FEC5C 00000000 */  nop
