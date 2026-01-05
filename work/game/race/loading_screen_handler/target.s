# Source: game_code.bin (decompressed)
# Address: 0x800FA9E4

glabel loading_screen_handler
    /* 800FA9E4 26D6A110 */  addiu $s6, $s6, -24304
    /* 800FA9E8 8EC30000 */  lw $v1, 0($s6)
    /* 800FA9EC 24150004 */  addiu $s5, $zero, 4
    /* 800FA9F0 24010006 */  addiu $at, $zero, 6
    /* 800FA9F4 16A30005 */  bne $s5, $v1, 0x800FAA0C
    /* 800FA9F8 00000000 */  nop
    /* 800FA9FC 0C030AF8 */  jal 0x800C2BE0
    /* 800FAA00 00000000 */  nop
    /* 800FAA04 10000014 */  beq $zero, $zero, 0x800FAA58
    /* 800FAA08 00000000 */  nop
    /* 800FAA0C 54610008 */  .word 0x54610008
    /* 800FAA10 24010005 */  addiu $at, $zero, 5
    /* 800FAA14 0C0E3F38 */  jal 0x8038FCE0
    /* 800FAA18 00000000 */  nop
    /* 800FAA1C 0C0E43D8 */  jal 0x80390F60
    /* 800FAA20 00000000 */  nop
    /* 800FAA24 1000000C */  beq $zero, $zero, 0x800FAA58
    /* 800FAA28 00000000 */  nop
    /* 800FAA2C 24010005 */  addiu $at, $zero, 5
    /* 800FAA30 14610005 */  bne $v1, $at, 0x800FAA48
    /* 800FAA34 00000000 */  nop
    /* 800FAA38 0C0345A7 */  jal 0x800D169C
    /* 800FAA3C 00000000 */  nop
    /* 800FAA40 10000003 */  beq $zero, $zero, 0x800FAA50
    /* 800FAA44 00000000 */  nop
    /* 800FAA48 0C03E4E8 */  jal 0x800F93A0
    /* 800FAA4C 00000000 */  nop
    /* 800FAA50 0C03E3B2 */  jal 0x800F8EC8
    /* 800FAA54 00000000 */  nop
    /* 800FAA58 0C032B69 */  jal 0x800CADA4
    /* 800FAA5C 00000000 */  nop
    /* 800FAA60 0C02BAD5 */  jal 0x800AEB54
    /* 800FAA64 00000000 */  nop
    /* 800FAA68 3C0E8015 */  lui $t6, 0x8015
    /* 800FAA6C 81CE2744 */  lb $t6, 10052($t6)
    /* 800FAA70 3C108015 */  lui $s0, 0x8015
    /* 800FAA74 26102818 */  addiu $s0, $s0, 10264
    /* 800FAA78 19C000A0 */  blez $t6, 0x800FACFC
    /* 800FAA7C 00009825 */  or $s3, $zero, $zero
    /* 800FAA80 3C118015 */  lui $s1, 0x8015
    /* 800FAA84 3C148011 */  lui $s4, 0x8011
    /* 800FAA88 4480A000 */  .word 0x4480A000
    /* 800FAA8C 2694FFC0 */  addiu $s4, $s4, -64
    /* 800FAA90 2631A250 */  addiu $s1, $s1, -23984
    /* 800FAA94 24120002 */  addiu $s2, $zero, 2
    /* 800FAA98 821800ED */  lb $t8, 237($s0)
    /* 800FAA9C 862F07C6 */  lh $t7, 1990($s1)
    /* 800FAAA0 3C198011 */  lui $t9, 0x8011
    /* 800FAAA4 07010068 */  bgez $t8, 0x800FAC48
    /* 800FAAA8 A7AF00BA */  sh $t7, 186($sp)
    /* 800FAAAC 8F3974B4 */  lw $t9, 29876($t9)
    /* 800FAAB0 8203035C */  lb $v1, 860($s0)
    /* 800FAAB4 27A400BA */  addiu $a0, $sp, 186
    /* 800FAAB8 33280008 */  andi $t0, $t9, 0x0008
    /* 800FAABC 15000011 */  bne $t0, $zero, 0x800FAB04
    /* 800FAAC0 24050001 */  addiu $a1, $zero, 1
    /* 800FAAC4 87A900BA */  lh $t1, 186($sp)
    /* 800FAAC8 3C0A8015 */  lui $t2, 0x8015
    /* 800FAACC 854AA108 */  lh $t2, -24312($t2)
    /* 800FAAD0 00095880 */  sll $t3, $t1, 2
    /* 800FAAD4 01695821 */  addu $t3, $t3, $t1
    /* 800FAAD8 012A082A */  slt $at, $t1, $t2
    /* 800FAADC 10200009 */  beq $at, $zero, 0x800FAB04
    /* 800FAAE0 000B5880 */  sll $t3, $t3, 2
    /* 800FAAE4 01695823 */  subu $t3, $t3, $t1
    /* 800FAAE8 3C0C8015 */  lui $t4, 0x8015
    /* 800FAAEC 258CA118 */  addiu $t4, $t4, -24296
    /* 800FAAF0 000B5880 */  sll $t3, $t3, 2
    /* 800FAAF4 016C1021 */  addu $v0, $t3, $t4
    /* 800FAAF8 944D0040 */  lhu $t5, 64($v0)
    /* 800FAAFC 25AE0001 */  addiu $t6, $t5, 1
    /* 800FAB00 A44E0040 */  sh $t6, 64($v0)
    /* 800FAB04 04600012 */  bltz $v1, 0x800FAB50
    /* 800FAB08 3C063F80 */  lui $a2, 0x3F80
    /* 800FAB0C 0003C080 */  sll $t8, $v1, 2
    /* 800FAB10 0303C021 */  addu $t8, $t8, $v1
    /* 800FAB14 0018C080 */  sll $t8, $t8, 2
    /* 800FAB18 0303C023 */  subu $t8, $t8, $v1
    /* 800FAB1C 3C198015 */  lui $t9, 0x8015
    /* 800FAB20 27390B70 */  addiu $t9, $t9, 2928
    /* 800FAB24 0018C0C0 */  sll $t8, $t8, 3
    /* 800FAB28 03191021 */  addu $v0, $t8, $t9
    /* 800FAB2C 820F035D */  lb $t7, 861($s0)
    /* 800FAB30 C4440024 */  lwc1 $f4, 36($v0)
    /* 800FAB34 C4460028 */  lwc1 $f6, 40($v0)
    /* 800FAB38 C448002C */  lwc1 $f8, 44($v0)
    /* 800FAB3C A215035D */  sb $s5, 861($s0)
    /* 800FAB40 A20F035E */  sb $t7, 862($s0)
    /* 800FAB44 E4440084 */  swc1 $f4, 132($v0)
    /* 800FAB48 E4460088 */  swc1 $f6, 136($v0)
    /* 800FAB4C E448008C */  swc1 $f8, 140($v0)
    /* 800FAB50 0C02BC1B */  jal 0x800AF06C
    /* 800FAB54 00003825 */  or $a3, $zero, $zero
    /* 800FAB58 0C035549 */  jal 0x800D5524
    /* 800FAB5C 02202025 */  or $a0, $s1, $zero
    /* 800FAB60 822807CC */  lb $t0, 1996($s1)
    /* 800FAB64 5648001D */  .word 0x5648001D
    /* 800FAB68 82980000 */  lb $t8, 0($s4)
    /* 800FAB6C 8EC30000 */  lw $v1, 0($s6)
    /* 800FAB70 5643000F */  .word 0x5643000F
    /* 800FAB74 828F0000 */  lb $t7, 0($s4)
    /* 800FAB78 862A07C6 */  lh $t2, 1990($s1)
    /* 800FAB7C 3C028015 */  lui $v0, 0x8015
    /* 800FAB80 000A4880 */  sll $t1, $t2, 2
    /* 800FAB84 00491021 */  addu $v0, $v0, $t1
    /* 800FAB88 8C422698 */  lw $v0, 9880($v0)
    /* 800FAB8C 50400008 */  .word 0x50400008
    /* 800FAB90 828F0000 */  lb $t7, 0($s4)
    /* 800FAB94 8C4B0000 */  lw $t3, 0($v0)
    /* 800FAB98 8D6C0028 */  lw $t4, 40($t3)
    /* 800FAB9C 8D8D0000 */  lw $t5, 0($t4)
    /* 800FABA0 81AE0005 */  lb $t6, 5($t5)
    /* 800FABA4 05C10022 */  bgez $t6, 0x800FAC30
    /* 800FABA8 00000000 */  nop
    /* 800FABAC 828F0000 */  lb $t7, 0($s4)
    /* 800FABB0 862507C6 */  lh $a1, 1990($s1)
    /* 800FABB4 2404002D */  addiu $a0, $zero, 45
    /* 800FABB8 15E00003 */  bne $t7, $zero, 0x800FABC8
    /* 800FABBC 24060001 */  addiu $a2, $zero, 1
    /* 800FABC0 1000001B */  beq $zero, $zero, 0x800FAC30
    /* 800FABC4 00000000 */  nop
    /* 800FABC8 0C0248D8 */  jal 0x80092360
    /* 800FABCC 24070001 */  addiu $a3, $zero, 1
    /* 800FABD0 10000017 */  beq $zero, $zero, 0x800FAC30
    /* 800FABD4 8EC30000 */  lw $v1, 0($s6)
    /* 800FABD8 82980000 */  lb $t8, 0($s4)
    /* 800FABDC 862207C6 */  lh $v0, 1990($s1)
    /* 800FABE0 2624022C */  addiu $a0, $s1, 556
    /* 800FABE4 17000003 */  bne $t8, $zero, 0x800FABF4
    /* 800FABE8 3C058011 */  lui $a1, 0x8011
    /* 800FABEC 10000010 */  beq $zero, $zero, 0x800FAC30
    /* 800FABF0 8EC30000 */  lw $v1, 0($s6)
    /* 800FABF4 3C013F80 */  lui $at, 0x3F80
    /* 800FABF8 44815000 */  .word 0x44815000
    /* 800FABFC 4407A000 */  .word 0x4407A000
    /* 800FAC00 2419002D */  addiu $t9, $zero, 45
    /* 800FAC04 24080080 */  addiu $t0, $zero, 128
    /* 800FAC08 AFA80024 */  sw $t0, 36($sp)
    /* 800FAC0C AFB90018 */  sw $t9, 24($sp)
    /* 800FAC10 24A541B0 */  addiu $a1, $a1, 16816
    /* 800FAC14 3C0643C8 */  lui $a2, 0x43C8
    /* 800FAC18 E7B40014 */  swc1 $f20, 20($sp)
    /* 800FAC1C AFA2001C */  sw $v0, 28($sp)
    /* 800FAC20 AFA00020 */  sw $zero, 32($sp)
    /* 800FAC24 0C02BB59 */  jal 0x800AED64
    /* 800FAC28 E7AA0010 */  swc1 $f10, 16($sp)
    /* 800FAC2C 8EC30000 */  lw $v1, 0($s6)
    /* 800FAC30 16A30003 */  bne $s5, $v1, 0x800FAC40
    /* 800FAC34 00000000 */  nop
    /* 800FAC38 0C030D5E */  jal 0x800C3578
    /* 800FAC3C 87A400BA */  lh $a0, 186($sp)
    /* 800FAC40 10000012 */  beq $zero, $zero, 0x800FAC8C
    /* 800FAC44 A20000ED */  sb $zero, 237($s0)
    /* 800FAC48 820A00ED */  lb $t2, 237($s0)
    /* 800FAC4C 87A900BA */  lh $t1, 186($sp)
    /* 800FAC50 3C0C8014 */  lui $t4, 0x8014
    /* 800FAC54 1940000D */  blez $t2, 0x800FAC8C
    /* 800FAC58 00095880 */  sll $t3, $t1, 2
    /* 800FAC5C 258C92D8 */  addiu $t4, $t4, -27944
    /* 800FAC60 016C1021 */  addu $v0, $t3, $t4
    /* 800FAC64 8C4D0000 */  lw $t5, 0($v0)
    /* 800FAC68 8204035C */  lb $a0, 860($s0)
    /* 800FAC6C 2401FFDF */  addiu $at, $zero, -33
    /* 800FAC70 01A17024 */  and $t6, $t5, $at
    /* 800FAC74 04800004 */  bltz $a0, 0x800FAC88
    /* 800FAC78 AC4E0000 */  sw $t6, 0($v0)
    /* 800FAC7C 820F035E */  lb $t7, 862($s0)
    /* 800FAC80 0C03E3A4 */  jal 0x800F8E90
    /* 800FAC84 A20F035D */  sb $t7, 861($s0)
    /* 800FAC88 A20000ED */  sb $zero, 237($s0)
    /* 800FAC8C 0C02C060 */  jal 0x800B0180
    /* 800FAC90 87A400BA */  lh $a0, 186($sp)
    /* 800FAC94 87B800BA */  lh $t8, 186($sp)
    /* 800FAC98 3C028015 */  lui $v0, 0x8015
    /* 800FAC9C 24010006 */  addiu $at, $zero, 6
    /* 800FACA0 0018C8C0 */  sll $t9, $t8, 3
    /* 800FACA4 00591021 */  addu $v0, $v0, $t9
    /* 800FACA8 90423E8F */  lbu $v0, 16015($v0)
    /* 800FACAC 50400004 */  .word 0x50400004
    /* 800FACB0 8EC30000 */  lw $v1, 0($s6)
    /* 800FACB4 1441000A */  bne $v0, $at, 0x800FACE0
    /* 800FACB8 00000000 */  nop
    /* 800FACBC 8EC30000 */  lw $v1, 0($s6)
    /* 800FACC0 16430005 */  bne $s2, $v1, 0x800FACD8
    /* 800FACC4 00000000 */  nop
    /* 800FACC8 16430005 */  bne $s2, $v1, 0x800FACE0
    /* 800FACCC 87A800BA */  lh $t0, 186($sp)
    /* 800FACD0 15000003 */  bne $t0, $zero, 0x800FACE0
    /* 800FACD4 00000000 */  nop
    /* 800FACD8 0C02FAA8 */  jal 0x800BEAA0
    /* 800FACDC 87A400BA */  lh $a0, 186($sp)
    /* 800FACE0 3C0A8015 */  lui $t2, 0x8015
    /* 800FACE4 814A2744 */  lb $t2, 10052($t2)
    /* 800FACE8 26730001 */  addiu $s3, $s3, 1
    /* 800FACEC 261003B8 */  addiu $s0, $s0, 952
    /* 800FACF0 026A082A */  slt $at, $s3, $t2
    /* 800FACF4 1420FF68 */  bne $at, $zero, 0x800FAA98
    /* 800FACF8 26310808 */  addiu $s1, $s1, 2056
    /* 800FACFC 0C02F4B2 */  jal 0x800BD2C8
    /* 800FAD00 00000000 */  nop
    /* 800FAD04 0C033DA7 */  jal 0x800CF69C
    /* 800FAD08 00000000 */  nop
    /* 800FAD0C 3C098011 */  lui $t1, 0x8011
    /* 800FAD10 81294650 */  lb $t1, 18000($t1)
    /* 800FAD14 55200004 */  .word 0x55200004
    /* 800FAD18 8FBF0054 */  lw $ra, 84($sp)
    /* 800FAD1C 0C03C040 */  jal 0x800F0100
    /* 800FAD20 00000000 */  nop
    /* 800FAD24 8FBF0054 */  lw $ra, 84($sp)
    /* 800FAD28 D7B40030 */  .word 0xD7B40030
    /* 800FAD2C 8FB00038 */  lw $s0, 56($sp)
    /* 800FAD30 8FB1003C */  lw $s1, 60($sp)
    /* 800FAD34 8FB20040 */  lw $s2, 64($sp)
    /* 800FAD38 8FB30044 */  lw $s3, 68($sp)
    /* 800FAD3C 8FB40048 */  lw $s4, 72($sp)
    /* 800FAD40 8FB5004C */  lw $s5, 76($sp)
    /* 800FAD44 8FB60050 */  lw $s6, 80($sp)
    /* 800FAD48 03E00008 */  jr $ra
    /* 800FAD4C 27BD00C0 */  addiu $sp, $sp, 192
