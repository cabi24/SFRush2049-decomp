# Source: game_code.bin (decompressed)
# Address: 0x800AAB44

glabel car_velocity_clamp
    /* 800AAB44 27BDFF98 */  addiu $sp, $sp, -104
    /* 800AAB48 AFBF0014 */  sw $ra, 20($sp)
    /* 800AAB4C 15C000AC */  bne $t6, $zero, 0x800AAE00
    /* 800AAB50 00805825 */  or $t3, $a0, $zero
    /* 800AAB54 3C078015 */  lui $a3, 0x8015
    /* 800AAB58 3C088012 */  lui $t0, 0x8012
    /* 800AAB5C 3C028015 */  lui $v0, 0x8015
    /* 800AAB60 00001825 */  or $v1, $zero, $zero
    /* 800AAB64 24422468 */  addiu $v0, $v0, 9320
    /* 800AAB68 2508E888 */  addiu $t0, $t0, -6008
    /* 800AAB6C 24E72578 */  addiu $a3, $a3, 9592
    /* 800AAB70 00002025 */  or $a0, $zero, $zero
    /* 800AAB74 00003025 */  or $a2, $zero, $zero
    /* 800AAB78 8D0F0000 */  lw $t7, 0($t0)
    /* 800AAB7C 24180001 */  addiu $t8, $zero, 1
    /* 800AAB80 24E70004 */  addiu $a3, $a3, 4
    /* 800AAB84 01F82804 */  .word 0x01F82804
    /* 800AAB88 18A00008 */  blez $a1, 0x800AABAC
    /* 800AAB8C ACE3FFFC */  sw $v1, -4($a3)
    /* 800AAB90 24C60001 */  addiu $a2, $a2, 1
    /* 800AAB94 00C5082A */  slt $at, $a2, $a1
    /* 800AAB98 A0440000 */  sb $a0, 0($v0)
    /* 800AAB9C 24630001 */  addiu $v1, $v1, 1
    /* 800AABA0 1420FFFB */  bne $at, $zero, 0x800AAB90
    /* 800AABA4 24420001 */  addiu $v0, $v0, 1
    /* 800AABA8 00003025 */  or $a2, $zero, $zero
    /* 800AABAC 24840001 */  addiu $a0, $a0, 1
    /* 800AABB0 2881001C */  slti $at, $a0, 28
    /* 800AABB4 1420FFF0 */  bne $at, $zero, 0x800AAB78
    /* 800AABB8 25080004 */  addiu $t0, $t0, 4
    /* 800AABBC A044FFFF */  sb $a0, -1($v0)
    /* 800AABC0 3C078015 */  lui $a3, 0x8015
    /* 800AABC4 3C088012 */  lui $t0, 0x8012
    /* 800AABC8 00001825 */  or $v1, $zero, $zero
    /* 800AABCC 2508E8FC */  addiu $t0, $t0, -5892
    /* 800AABD0 24E72600 */  addiu $a3, $a3, 9728
    /* 800AABD4 00002025 */  or $a0, $zero, $zero
    /* 800AABD8 24090010 */  addiu $t1, $zero, 16
    /* 800AABDC 8D190000 */  lw $t9, 0($t0)
    /* 800AABE0 240E0001 */  addiu $t6, $zero, 1
    /* 800AABE4 24E70004 */  addiu $a3, $a3, 4
    /* 800AABE8 032E2804 */  .word 0x032E2804
    /* 800AABEC 18A0000B */  blez $a1, 0x800AAC1C
    /* 800AABF0 ACE3FFFC */  sw $v1, -4($a3)
    /* 800AABF4 3C188015 */  lui $t8, 0x8015
    /* 800AABF8 27182260 */  addiu $t8, $t8, 8800
    /* 800AABFC 00781021 */  addu $v0, $v1, $t8
    /* 800AAC00 24C60001 */  addiu $a2, $a2, 1
    /* 800AAC04 00C5082A */  slt $at, $a2, $a1
    /* 800AAC08 A0440000 */  sb $a0, 0($v0)
    /* 800AAC0C 24630001 */  addiu $v1, $v1, 1
    /* 800AAC10 1420FFFB */  bne $at, $zero, 0x800AAC00
    /* 800AAC14 24420001 */  addiu $v0, $v0, 1
    /* 800AAC18 00003025 */  or $a2, $zero, $zero
    /* 800AAC1C 24840001 */  addiu $a0, $a0, 1
    /* 800AAC20 1489FFEE */  bne $a0, $t1, 0x800AABDC
    /* 800AAC24 25080004 */  addiu $t0, $t0, 4
    /* 800AAC28 000379C3 */  sra $t7, $v1, 7
    /* 800AAC2C 2881001E */  slti $at, $a0, 30
    /* 800AAC30 10200017 */  beq $at, $zero, 0x800AAC90
    /* 800AAC34 01E01825 */  or $v1, $t7, $zero
    /* 800AAC38 8D190000 */  lw $t9, 0($t0)
    /* 800AAC3C 240F0001 */  addiu $t7, $zero, 1
    /* 800AAC40 000371C0 */  sll $t6, $v1, 7
    /* 800AAC44 27380019 */  addiu $t8, $t9, 25
    /* 800AAC48 030F2804 */  .word 0x030F2804
    /* 800AAC4C 18A0000B */  blez $a1, 0x800AAC7C
    /* 800AAC50 ACEE0000 */  sw $t6, 0($a3)
    /* 800AAC54 3C0E8015 */  lui $t6, 0x8015
    /* 800AAC58 25CE2260 */  addiu $t6, $t6, 8800
    /* 800AAC5C 006E1021 */  addu $v0, $v1, $t6
    /* 800AAC60 24C60001 */  addiu $a2, $a2, 1
    /* 800AAC64 00C5082A */  slt $at, $a2, $a1
    /* 800AAC68 A0440100 */  sb $a0, 256($v0)
    /* 800AAC6C 24630001 */  addiu $v1, $v1, 1
    /* 800AAC70 1420FFFB */  bne $at, $zero, 0x800AAC60
    /* 800AAC74 24420001 */  addiu $v0, $v0, 1
    /* 800AAC78 00003025 */  or $a2, $zero, $zero
    /* 800AAC7C 24840001 */  addiu $a0, $a0, 1
    /* 800AAC80 2881001E */  slti $at, $a0, 30
    /* 800AAC84 24E70004 */  addiu $a3, $a3, 4
    /* 800AAC88 1420FFEB */  bne $at, $zero, 0x800AAC38
    /* 800AAC8C 25080004 */  addiu $t0, $t0, 4
    /* 800AAC90 27A2002C */  addiu $v0, $sp, 44
    /* 800AAC94 27A3004C */  addiu $v1, $sp, 76
    /* 800AAC98 24420008 */  addiu $v0, $v0, 8
    /* 800AAC9C A440FFF8 */  sh $zero, -8($v0)
    /* 800AACA0 A440FFFA */  sh $zero, -6($v0)
    /* 800AACA4 A440FFFC */  sh $zero, -4($v0)
    /* 800AACA8 1443FFFB */  bne $v0, $v1, 0x800AAC98
    /* 800AACAC A440FFFE */  sh $zero, -2($v0)
    /* 800AACB0 3C028012 */  lui $v0, 0x8012
    /* 800AACB4 3C058012 */  lui $a1, 0x8012
    /* 800AACB8 24A54C30 */  addiu $a1, $a1, 19504
    /* 800AACBC 244249F0 */  addiu $v0, $v0, 18928
    /* 800AACC0 24040008 */  addiu $a0, $zero, 8
    /* 800AACC4 97A3003C */  lhu $v1, 60($sp)
    /* 800AACC8 0060C025 */  or $t8, $v1, $zero
    /* 800AACCC 27030004 */  addiu $v1, $t8, 4
    /* 800AACD0 306EFFFF */  andi $t6, $v1, 0xFFFF
    /* 800AACD4 24420010 */  addiu $v0, $v0, 16
    /* 800AACD8 01C01825 */  or $v1, $t6, $zero
    /* 800AACDC A444FFF2 */  sh $a0, -14($v0)
    /* 800AACE0 A444FFF6 */  sh $a0, -10($v0)
    /* 800AACE4 A444FFFA */  sh $a0, -6($v0)
    /* 800AACE8 1445FFF7 */  bne $v0, $a1, 0x800AACC8
    /* 800AACEC A444FFFE */  sh $a0, -2($v0)
    /* 800AACF0 3C198012 */  lui $t9, 0x8012
    /* 800AACF4 27394DF0 */  addiu $t9, $t9, 19952
    /* 800AACF8 0059082B */  sltu $at, $v0, $t9
    /* 800AACFC 1020000D */  beq $at, $zero, 0x800AAD34
    /* 800AAD00 3C188012 */  lui $t8, 0x8012
    /* 800AAD04 3C078012 */  lui $a3, 0x8012
    /* 800AAD08 24E74DF0 */  addiu $a3, $a3, 19952
    /* 800AAD0C 24060009 */  addiu $a2, $zero, 9
    /* 800AAD10 97A5003E */  lhu $a1, 62($sp)
    /* 800AAD14 24420004 */  addiu $v0, $v0, 4
    /* 800AAD18 24A50001 */  addiu $a1, $a1, 1
    /* 800AAD1C 30AFFFFF */  andi $t7, $a1, 0xFFFF
    /* 800AAD20 0047082B */  sltu $at, $v0, $a3
    /* 800AAD24 01E02825 */  or $a1, $t7, $zero
    /* 800AAD28 1420FFFA */  bne $at, $zero, 0x800AAD14
    /* 800AAD2C A446FFFE */  sh $a2, -2($v0)
    /* 800AAD30 A7AF003E */  sh $t7, 62($sp)
    /* 800AAD34 27184E50 */  addiu $t8, $t8, 20048
    /* 800AAD38 0058082B */  sltu $at, $v0, $t8
    /* 800AAD3C 1020000D */  beq $at, $zero, 0x800AAD74
    /* 800AAD40 3C198012 */  lui $t9, 0x8012
    /* 800AAD44 3C078012 */  lui $a3, 0x8012
    /* 800AAD48 24E74E50 */  addiu $a3, $a3, 20048
    /* 800AAD4C 24060007 */  addiu $a2, $zero, 7
    /* 800AAD50 97A5003A */  lhu $a1, 58($sp)
    /* 800AAD54 24420004 */  addiu $v0, $v0, 4
    /* 800AAD58 24A50001 */  addiu $a1, $a1, 1
    /* 800AAD5C 30AEFFFF */  andi $t6, $a1, 0xFFFF
    /* 800AAD60 0047082B */  sltu $at, $v0, $a3
    /* 800AAD64 01C02825 */  or $a1, $t6, $zero
    /* 800AAD68 1420FFFA */  bne $at, $zero, 0x800AAD54
    /* 800AAD6C A446FFFE */  sh $a2, -2($v0)
    /* 800AAD70 A7AE003A */  sh $t6, 58($sp)
    /* 800AAD74 27394E70 */  addiu $t9, $t9, 20080
    /* 800AAD78 0059082B */  sltu $at, $v0, $t9
    /* 800AAD7C 1020000B */  beq $at, $zero, 0x800AADAC
    /* 800AAD80 A7A3003C */  sh $v1, 60($sp)
    /* 800AAD84 3C058012 */  lui $a1, 0x8012
    /* 800AAD88 24A54E70 */  addiu $a1, $a1, 20080
    /* 800AAD8C 24420004 */  addiu $v0, $v0, 4
    /* 800AAD90 24630001 */  addiu $v1, $v1, 1
    /* 800AAD94 306FFFFF */  andi $t7, $v1, 0xFFFF
    /* 800AAD98 0045082B */  sltu $at, $v0, $a1
    /* 800AAD9C 01E01825 */  or $v1, $t7, $zero
    /* 800AADA0 1420FFFA */  bne $at, $zero, 0x800AAD8C
    /* 800AADA4 A444FFFE */  sh $a0, -2($v0)
    /* 800AADA8 A7AF003C */  sh $t7, 60($sp)
    /* 800AADAC 3C0A8012 */  lui $t2, 0x8012
    /* 800AADB0 254A49F0 */  addiu $t2, $t2, 18928
    /* 800AADB4 2409011F */  addiu $t1, $zero, 287
    /* 800AADB8 0C02A3D9 */  jal 0x800A8F64
    /* 800AADBC 27A7002C */  addiu $a3, $sp, 44
    /* 800AADC0 3C078012 */  lui $a3, 0x8012
    /* 800AADC4 24E74E70 */  addiu $a3, $a3, 20080
    /* 800AADC8 00003025 */  or $a2, $zero, $zero
    /* 800AADCC 2409001E */  addiu $t1, $zero, 30
    /* 800AADD0 24080005 */  addiu $t0, $zero, 5
    /* 800AADD4 A4E80002 */  sh $t0, 2($a3)
    /* 800AADD8 00C02025 */  or $a0, $a2, $zero
    /* 800AADDC 0C02A3CE */  jal 0x800A8F38
    /* 800AADE0 24050005 */  addiu $a1, $zero, 5
    /* 800AADE4 24C60001 */  addiu $a2, $a2, 1
    /* 800AADE8 24E70004 */  addiu $a3, $a3, 4
    /* 800AADEC 14C9FFF9 */  bne $a2, $t1, 0x800AADD4
    /* 800AADF0 A4E2FFFC */  sh $v0, -4($a3)
    /* 800AADF4 24180001 */  addiu $t8, $zero, 1
    /* 800AADF8 3C018012 */  lui $at, 0x8012
    /* 800AADFC AC38EA10 */  sw $t8, -5616($at)
    /* 800AAE00 3C198012 */  lui $t9, 0x8012
    /* 800AAE04 2739E9D4 */  addiu $t9, $t9, -5676
    /* 800AAE08 256E0078 */  addiu $t6, $t3, 120
    /* 800AAE0C AD790B04 */  sw $t9, 2820($t3)
    /* 800AAE10 256F096C */  addiu $t7, $t3, 2412
    /* 800AAE14 AD6E0AFC */  sw $t6, 2812($t3)
    /* 800AAE18 AD6F0B08 */  sw $t7, 2824($t3)
    /* 800AAE1C 3C188012 */  lui $t8, 0x8012
    /* 800AAE20 3C198012 */  lui $t9, 0x8012
    /* 800AAE24 2718E9E8 */  addiu $t8, $t8, -5656
    /* 800AAE28 256E0A60 */  addiu $t6, $t3, 2656
    /* 800AAE2C 2739E9FC */  addiu $t9, $t9, -5636
    /* 800AAE30 240F0008 */  addiu $t7, $zero, 8
    /* 800AAE34 AD601694 */  sw $zero, 5780($t3)
    /* 800AAE38 AD780B10 */  sw $t8, 2832($t3)
    /* 800AAE3C AD6E0B14 */  sw $t6, 2836($t3)
    /* 800AAE40 AD790B1C */  sw $t9, 2844($t3)
    /* 800AAE44 A56016A0 */  sh $zero, 5792($t3)
    /* 800AAE48 AD6016A4 */  sw $zero, 5796($t3)
    /* 800AAE4C AD6F169C */  sw $t7, 5788($t3)
    /* 800AAE50 0C02A07F */  jal 0x800A81FC
    /* 800AAE54 01602025 */  or $a0, $t3, $zero
    /* 800AAE58 8FBF0014 */  lw $ra, 20($sp)
    /* 800AAE5C 27BD0068 */  addiu $sp, $sp, 104
    /* 800AAE60 03E00008 */  jr $ra
    /* 800AAE64 00000000 */  nop
