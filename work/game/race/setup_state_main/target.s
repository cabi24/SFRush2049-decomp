# Source: game_code.bin (decompressed)
# Address: 0x800FB2C8

glabel setup_state_main
    /* 800FB2C8 27BDFF00 */  addiu $sp, $sp, -256
    /* 800FB2CC AFBF004C */  sw $ra, 76($sp)
    /* 800FB2D0 AFBE0048 */  sw $fp, 72($sp)
    /* 800FB2D4 AFB70044 */  sw $s7, 68($sp)
    /* 800FB2D8 AFB60040 */  sw $s6, 64($sp)
    /* 800FB2DC AFB5003C */  sw $s5, 60($sp)
    /* 800FB2E0 AFB40038 */  sw $s4, 56($sp)
    /* 800FB2E4 AFB30034 */  sw $s3, 52($sp)
    /* 800FB2E8 AFB20030 */  sw $s2, 48($sp)
    /* 800FB2EC AFB1002C */  sw $s1, 44($sp)
    /* 800FB2F0 AFB00028 */  sw $s0, 40($sp)
    /* 800FB2F4 10A00005 */  beq $a1, $zero, 0x800FB30C
    /* 800FB2F8 AFA40100 */  sw $a0, 256($sp)
    /* 800FB2FC 3C018011 */  lui $at, 0x8011
    /* 800FB300 A02046F4 */  sb $zero, 18164($at)
    /* 800FB304 10000231 */  beq $zero, $zero, 0x800FBBCC
    /* 800FB308 00001025 */  or $v0, $zero, $zero
    /* 800FB30C 3C0E8011 */  lui $t6, 0x8011
    /* 800FB310 81CE46F4 */  lb $t6, 18164($t6)
    /* 800FB314 240F0001 */  addiu $t7, $zero, 1
    /* 800FB318 3C018011 */  lui $at, 0x8011
    /* 800FB31C 15C00111 */  bne $t6, $zero, 0x800FB764
    /* 800FB320 00000000 */  nop
    /* 800FB324 0C0326F8 */  jal 0x800C9BE0
    /* 800FB328 A02F46F4 */  sb $t7, 18164($at)
    /* 800FB32C 3C028011 */  lui $v0, 0x8011
    /* 800FB330 8C4274B4 */  lw $v0, 29876($v0)
    /* 800FB334 3C010200 */  lui $at, 0x0200
    /* 800FB338 30440008 */  andi $a0, $v0, 0x0008
    /* 800FB33C 2C980001 */  sltiu $t8, $a0, 1
    /* 800FB340 13000004 */  beq $t8, $zero, 0x800FB354
    /* 800FB344 03002025 */  or $a0, $t8, $zero
    /* 800FB348 00412024 */  and $a0, $v0, $at
    /* 800FB34C 2C990001 */  sltiu $t9, $a0, 1
    /* 800FB350 03202025 */  or $a0, $t9, $zero
    /* 800FB354 0C03EC8D */  jal 0x800FB234
    /* 800FB358 00000000 */  nop
    /* 800FB35C 3C028011 */  lui $v0, 0x8011
    /* 800FB360 8C4274B4 */  lw $v0, 29876($v0)
    /* 800FB364 3C098015 */  lui $t1, 0x8015
    /* 800FB368 3C0142B4 */  lui $at, 0x42B4
    /* 800FB36C 00027180 */  sll $t6, $v0, 6
    /* 800FB370 05C100B8 */  bgez $t6, 0x800FB654
    /* 800FB374 30580008 */  andi $t8, $v0, 0x0008
    /* 800FB378 3C0A8015 */  lui $t2, 0x8015
    /* 800FB37C 254A274C */  addiu $t2, $t2, 10060
    /* 800FB380 A5400000 */  sh $zero, 0($t2)
    /* 800FB384 85430000 */  lh $v1, 0($t2)
    /* 800FB388 25292768 */  addiu $t1, $t1, 10088
    /* 800FB38C 3C0F8015 */  lui $t7, 0x8015
    /* 800FB390 25EF3FD2 */  addiu $t7, $t7, 16338
    /* 800FB394 A5230000 */  sh $v1, 0($t1)
    /* 800FB398 3C188015 */  lui $t8, 0x8015
    /* 800FB39C A5E30000 */  sh $v1, 0($t7)
    /* 800FB3A0 8718A108 */  lh $t8, -24312($t8)
    /* 800FB3A4 3C108015 */  lui $s0, 0x8015
    /* 800FB3A8 2610A118 */  addiu $s0, $s0, -24296
    /* 800FB3AC 1B00003E */  blez $t8, 0x800FB4A8
    /* 800FB3B0 00008825 */  or $s1, $zero, $zero
    /* 800FB3B4 92030000 */  lbu $v1, 0($s0)
    /* 800FB3B8 3C0E8015 */  lui $t6, 0x8015
    /* 800FB3BC 25CE3E88 */  addiu $t6, $t6, 16008
    /* 800FB3C0 0003C8C0 */  sll $t9, $v1, 3
    /* 800FB3C4 032E4021 */  addu $t0, $t9, $t6
    /* 800FB3C8 910F0007 */  lbu $t7, 7($t0)
    /* 800FB3CC 0003C900 */  sll $t9, $v1, 4
    /* 800FB3D0 0323C823 */  subu $t9, $t9, $v1
    /* 800FB3D4 25F8FFFA */  addiu $t8, $t7, -6
    /* 800FB3D8 A1180007 */  sb $t8, 7($t0)
    /* 800FB3DC 0019C8C0 */  sll $t9, $t9, 3
    /* 800FB3E0 00037A00 */  sll $t7, $v1, 8
    /* 800FB3E4 3C0E8015 */  lui $t6, 0x8015
    /* 800FB3E8 3C188015 */  lui $t8, 0x8015
    /* 800FB3EC 0323C823 */  subu $t9, $t9, $v1
    /* 800FB3F0 01E37821 */  addu $t7, $t7, $v1
    /* 800FB3F4 0019C8C0 */  sll $t9, $t9, 3
    /* 800FB3F8 25CE2818 */  addiu $t6, $t6, 10264
    /* 800FB3FC 000F78C0 */  sll $t7, $t7, 3
    /* 800FB400 2718A250 */  addiu $t8, $t8, -23984
    /* 800FB404 032E3821 */  addu $a3, $t9, $t6
    /* 800FB408 01F81021 */  addu $v0, $t7, $t8
    /* 800FB40C 24060001 */  addiu $a2, $zero, 1
    /* 800FB410 00032400 */  sll $a0, $v1, 16
    /* 800FB414 0004CC03 */  sra $t9, $a0, 16
    /* 800FB418 A0E00359 */  sb $zero, 857($a3)
    /* 800FB41C A0E000EF */  sb $zero, 239($a3)
    /* 800FB420 ACE00380 */  sw $zero, 896($a3)
    /* 800FB424 A04607CC */  sb $a2, 1996($v0)
    /* 800FB428 A44607CA */  sh $a2, 1994($v0)
    /* 800FB42C A046000A */  sb $a2, 10($v0)
    /* 800FB430 AC4007D4 */  sw $zero, 2004($v0)
    /* 800FB434 ACE000E8 */  sw $zero, 232($a3)
    /* 800FB438 03202025 */  or $a0, $t9, $zero
    /* 800FB43C 0C02C803 */  jal 0x800B200C
    /* 800FB440 90450008 */  lbu $a1, 8($v0)
    /* 800FB444 3C098015 */  lui $t1, 0x8015
    /* 800FB448 25292768 */  addiu $t1, $t1, 10088
    /* 800FB44C 85230000 */  lh $v1, 0($t1)
    /* 800FB450 3C018015 */  lui $at, 0x8015
    /* 800FB454 3C0A8015 */  lui $t2, 0x8015
    /* 800FB458 00037040 */  sll $t6, $v1, 1
    /* 800FB45C 002E0821 */  addu $at, $at, $t6
    /* 800FB460 254A274C */  addiu $t2, $t2, 10060
    /* 800FB464 A43127D8 */  sh $s1, 10200($at)
    /* 800FB468 85440000 */  lh $a0, 0($t2)
    /* 800FB46C 3C018015 */  lui $at, 0x8015
    /* 800FB470 246F0001 */  addiu $t7, $v1, 1
    /* 800FB474 0004C040 */  sll $t8, $a0, 1
    /* 800FB478 A52F0000 */  sh $t7, 0($t1)
    /* 800FB47C 00380821 */  addu $at, $at, $t8
    /* 800FB480 A4312808 */  sh $s1, 10248($at)
    /* 800FB484 24990001 */  addiu $t9, $a0, 1
    /* 800FB488 3C0E8015 */  lui $t6, 0x8015
    /* 800FB48C A5590000 */  sh $t9, 0($t2)
    /* 800FB490 85CEA108 */  lh $t6, -24312($t6)
    /* 800FB494 26310001 */  addiu $s1, $s1, 1
    /* 800FB498 2610004C */  addiu $s0, $s0, 76
    /* 800FB49C 022E082A */  slt $at, $s1, $t6
    /* 800FB4A0 5420FFC5 */  .word 0x5420FFC5
    /* 800FB4A4 92030000 */  lbu $v1, 0($s0)
    /* 800FB4A8 3C0F8015 */  lui $t7, 0x8015
    /* 800FB4AC 81EF2744 */  lb $t7, 10052($t7)
    /* 800FB4B0 0011C100 */  sll $t8, $s1, 4
    /* 800FB4B4 0311C023 */  subu $t8, $t8, $s1
    /* 800FB4B8 022F082A */  slt $at, $s1, $t7
    /* 800FB4BC 10200031 */  beq $at, $zero, 0x800FB584
    /* 800FB4C0 0018C0C0 */  sll $t8, $t8, 3
    /* 800FB4C4 00117200 */  sll $t6, $s1, 8
    /* 800FB4C8 01D17021 */  addu $t6, $t6, $s1
    /* 800FB4CC 0311C023 */  subu $t8, $t8, $s1
    /* 800FB4D0 3C198015 */  lui $t9, 0x8015
    /* 800FB4D4 3C0F8015 */  lui $t7, 0x8015
    /* 800FB4D8 25EFA250 */  addiu $t7, $t7, -23984
    /* 800FB4DC 27392818 */  addiu $t9, $t9, 10264
    /* 800FB4E0 0018C0C0 */  sll $t8, $t8, 3
    /* 800FB4E4 000E70C0 */  sll $t6, $t6, 3
    /* 800FB4E8 01CF8021 */  addu $s0, $t6, $t7
    /* 800FB4EC 03191021 */  addu $v0, $t8, $t9
    /* 800FB4F0 00112400 */  sll $a0, $s1, 16
    /* 800FB4F4 0004C403 */  sra $t8, $a0, 16
    /* 800FB4F8 A0400359 */  sb $zero, 857($v0)
    /* 800FB4FC A04000EF */  sb $zero, 239($v0)
    /* 800FB500 AC4000E8 */  sw $zero, 232($v0)
    /* 800FB504 AE0007D4 */  sw $zero, 2004($s0)
    /* 800FB508 03002025 */  or $a0, $t8, $zero
    /* 800FB50C 92050008 */  lbu $a1, 8($s0)
    /* 800FB510 24060001 */  addiu $a2, $zero, 1
    /* 800FB514 0C02C803 */  jal 0x800B200C
    /* 800FB518 AFA20054 */  sw $v0, 84($sp)
    /* 800FB51C 3C098015 */  lui $t1, 0x8015
    /* 800FB520 25292768 */  addiu $t1, $t1, 10088
    /* 800FB524 85230000 */  lh $v1, 0($t1)
    /* 800FB528 3C018015 */  lui $at, 0x8015
    /* 800FB52C 3C0A8015 */  lui $t2, 0x8015
    /* 800FB530 0003C840 */  sll $t9, $v1, 1
    /* 800FB534 00390821 */  addu $at, $at, $t9
    /* 800FB538 254A274C */  addiu $t2, $t2, 10060
    /* 800FB53C A43127D8 */  sh $s1, 10200($at)
    /* 800FB540 85440000 */  lh $a0, 0($t2)
    /* 800FB544 3C018015 */  lui $at, 0x8015
    /* 800FB548 246E0001 */  addiu $t6, $v1, 1
    /* 800FB54C 00047840 */  sll $t7, $a0, 1
    /* 800FB550 A52E0000 */  sh $t6, 0($t1)
    /* 800FB554 002F0821 */  addu $at, $at, $t7
    /* 800FB558 A4312808 */  sh $s1, 10248($at)
    /* 800FB55C 24980001 */  addiu $t8, $a0, 1
    /* 800FB560 3C198015 */  lui $t9, 0x8015
    /* 800FB564 A5580000 */  sh $t8, 0($t2)
    /* 800FB568 83392744 */  lb $t9, 10052($t9)
    /* 800FB56C 8FA20054 */  lw $v0, 84($sp)
    /* 800FB570 26310001 */  addiu $s1, $s1, 1
    /* 800FB574 0239082A */  slt $at, $s1, $t9
    /* 800FB578 26100808 */  addiu $s0, $s0, 2056
    /* 800FB57C 1420FFDC */  bne $at, $zero, 0x800FB4F0
    /* 800FB580 244203B8 */  addiu $v0, $v0, 952
    /* 800FB584 854E0000 */  lh $t6, 0($t2)
    /* 800FB588 3C028015 */  lui $v0, 0x8015
    /* 800FB58C 24423E84 */  addiu $v0, $v0, 16004
    /* 800FB590 A44E0000 */  sh $t6, 0($v0)
    /* 800FB594 844F0000 */  lh $t7, 0($v0)
    /* 800FB598 3C018015 */  lui $at, 0x8015
    /* 800FB59C 2418FFFF */  addiu $t8, $zero, -1
    /* 800FB5A0 A42F3F08 */  sh $t7, 16136($at)
    /* 800FB5A4 3C018015 */  lui $at, 0x8015
    /* 800FB5A8 A4383F24 */  sh $t8, 16164($at)
    /* 800FB5AC 3C018015 */  lui $at, 0x8015
    /* 800FB5B0 A4203F40 */  sh $zero, 16192($at)
    /* 800FB5B4 00008825 */  or $s1, $zero, $zero
    /* 800FB5B8 0C03EBDB */  jal 0x800FAF6C
    /* 800FB5BC AFA000FC */  sw $zero, 252($sp)
    /* 800FB5C0 3C018015 */  lui $at, 0x8015
    /* 800FB5C4 3C198015 */  lui $t9, 0x8015
    /* 800FB5C8 87392734 */  lh $t9, 10036($t9)
    /* 800FB5CC A0202718 */  sb $zero, 10008($at)
    /* 800FB5D0 3C018015 */  lui $at, 0x8015
    /* 800FB5D4 8FB100FC */  lw $s1, 252($sp)
    /* 800FB5D8 0C03EBB9 */  jal 0x800FAEE4
    /* 800FB5DC A0392014 */  sb $t9, 8212($at)
    /* 800FB5E0 3C018014 */  lui $at, 0x8014
    /* 800FB5E4 A020FECB */  sb $zero, -309($at)
    /* 800FB5E8 3C018014 */  lui $at, 0x8014
    /* 800FB5EC A0202699 */  sb $zero, 9881($at)
    /* 800FB5F0 3C018012 */  lui $at, 0x8012
    /* 800FB5F4 3C0E8015 */  lui $t6, 0x8015
    /* 800FB5F8 C42447F0 */  lwc1 $f4, 18416($at)
    /* 800FB5FC 81CE2744 */  lb $t6, 10052($t6)
    /* 800FB600 3C018014 */  lui $at, 0x8014
    /* 800FB604 3C108015 */  lui $s0, 0x8015
    /* 800FB608 19C0000C */  blez $t6, 0x800FB63C
    /* 800FB60C E4242764 */  swc1 $f4, 10084($at)
    /* 800FB610 2610A250 */  addiu $s0, $s0, -23984
    /* 800FB614 00002025 */  or $a0, $zero, $zero
    /* 800FB618 0C03950F */  jal 0x800E543C
    /* 800FB61C 860507C6 */  lh $a1, 1990($s0)
    /* 800FB620 3C0F8015 */  lui $t7, 0x8015
    /* 800FB624 81EF2744 */  lb $t7, 10052($t7)
    /* 800FB628 26310001 */  addiu $s1, $s1, 1
    /* 800FB62C 26100808 */  addiu $s0, $s0, 2056
    /* 800FB630 022F082A */  slt $at, $s1, $t7
    /* 800FB634 5420FFF8 */  .word 0x5420FFF8
    /* 800FB638 00002025 */  or $a0, $zero, $zero
    /* 800FB63C 0C03C040 */  jal 0x800F0100
    /* 800FB640 00000000 */  nop
    /* 800FB644 3C018011 */  lui $at, 0x8011
    /* 800FB648 A02046F4 */  sb $zero, 18164($at)
    /* 800FB64C 1000015F */  beq $zero, $zero, 0x800FBBCC
    /* 800FB650 24020001 */  addiu $v0, $zero, 1
    /* 800FB654 44813000 */  .word 0x44813000
    /* 800FB658 3C018015 */  lui $at, 0x8015
    /* 800FB65C 17000015 */  bne $t8, $zero, 0x800FB6B4
    /* 800FB660 E4264188 */  swc1 $f6, 16776($at)
    /* 800FB664 3C048014 */  lui $a0, 0x8014
    /* 800FB668 24842728 */  addiu $a0, $a0, 10024
    /* 800FB66C 00002825 */  or $a1, $zero, $zero
    /* 800FB670 0C001C9C */  jal 0x80007270
    /* 800FB674 24060001 */  addiu $a2, $zero, 1
    /* 800FB678 0C0246C0 */  jal 0x80091B00
    /* 800FB67C 00000000 */  nop
    /* 800FB680 24190001 */  addiu $t9, $zero, 1
    /* 800FB684 3C048014 */  lui $a0, 0x8014
    /* 800FB688 24842728 */  addiu $a0, $a0, 10024
    /* 800FB68C A0590002 */  sb $t9, 2($v0)
    /* 800FB690 00408025 */  or $s0, $v0, $zero
    /* 800FB694 00002825 */  or $a1, $zero, $zero
    /* 800FB698 0C001D78 */  jal 0x800075E0
    /* 800FB69C 00003025 */  or $a2, $zero, $zero
    /* 800FB6A0 3C048014 */  lui $a0, 0x8014
    /* 800FB6A4 248427A8 */  addiu $a0, $a0, 10152
    /* 800FB6A8 02002825 */  or $a1, $s0, $zero
    /* 800FB6AC 0C001D78 */  jal 0x800075E0
    /* 800FB6B0 00003025 */  or $a2, $zero, $zero
    /* 800FB6B4 3C108011 */  lui $s0, 0x8011
    /* 800FB6B8 26104650 */  addiu $s0, $s0, 18000
    /* 800FB6BC 820E0000 */  lb $t6, 0($s0)
    /* 800FB6C0 3C018015 */  lui $at, 0x8015
    /* 800FB6C4 240FFFFF */  addiu $t7, $zero, -1
    /* 800FB6C8 11C0001A */  beq $t6, $zero, 0x800FB734
    /* 800FB6CC 3C198011 */  lui $t9, 0x8011
    /* 800FB6D0 AC206990 */  sw $zero, 27024($at)
    /* 800FB6D4 3C018015 */  lui $at, 0x8015
    /* 800FB6D8 AC206CE0 */  sw $zero, 27872($at)
    /* 800FB6DC 3C018016 */  lui $at, 0x8016
    /* 800FB6E0 AC2013AC */  sw $zero, 5036($at)
    /* 800FB6E4 3C018015 */  lui $at, 0x8015
    /* 800FB6E8 AC206BAC */  sw $zero, 27564($at)
    /* 800FB6EC 3C018016 */  lui $at, 0x8016
    /* 800FB6F0 AC2013B4 */  sw $zero, 5044($at)
    /* 800FB6F4 3C018016 */  lui $at, 0x8016
    /* 800FB6F8 0C029690 */  jal 0x800A5A40
    /* 800FB6FC A42FB254 */  sh $t7, -19884($at)
    /* 800FB700 3C013F80 */  lui $at, 0x3F80
    /* 800FB704 44814000 */  .word 0x44814000
    /* 800FB708 3C018016 */  lui $at, 0x8016
    /* 800FB70C 3C188012 */  lui $t8, 0x8012
    /* 800FB710 E42813B8 */  swc1 $f8, 5048($at)
    /* 800FB714 3C018014 */  lui $at, 0x8014
    /* 800FB718 A4200618 */  sh $zero, 1560($at)
    /* 800FB71C 3C018014 */  lui $at, 0x8014
    /* 800FB720 2718EA18 */  addiu $t8, $t8, -5608
    /* 800FB724 0C029522 */  jal 0x800A5488
    /* 800FB728 AC3806B8 */  sw $t8, 1720($at)
    /* 800FB72C 10000007 */  beq $zero, $zero, 0x800FB74C
    /* 800FB730 00000000 */  nop
    /* 800FB734 8F3974B4 */  lw $t9, 29876($t9)
    /* 800FB738 332E0008 */  andi $t6, $t9, 0x0008
    /* 800FB73C 15C00003 */  bne $t6, $zero, 0x800FB74C
    /* 800FB740 00000000 */  nop
    /* 800FB744 0C0296EE */  jal 0x800A5BB8
    /* 800FB748 00000000 */  nop
    /* 800FB74C 0C03B245 */  jal 0x800EC914
    /* 800FB750 00000000 */  nop
    /* 800FB754 0C03DAAE */  jal 0x800F6AB8
    /* 800FB758 00000000 */  nop
    /* 800FB75C 0C03DFAC */  jal 0x800F7EB0
    /* 800FB760 00000000 */  nop
    /* 800FB764 3C0F8014 */  lui $t7, 0x8014
    /* 800FB768 81EF27A0 */  lb $t7, 10144($t7)
    /* 800FB76C 3C108011 */  lui $s0, 0x8011
    /* 800FB770 26104650 */  addiu $s0, $s0, 18000
    /* 800FB774 29E10013 */  slti $at, $t7, 19
    /* 800FB778 10200005 */  beq $at, $zero, 0x800FB790
    /* 800FB77C 3C198011 */  lui $t9, 0x8011
    /* 800FB780 3C188015 */  lui $t8, 0x8015
    /* 800FB784 8318978C */  lb $t8, -26740($t8)
    /* 800FB788 3C018014 */  lui $at, 0x8014
    /* 800FB78C A03827A0 */  sb $t8, 10144($at)
    /* 800FB790 8F3974B4 */  lw $t9, 29876($t9)
    /* 800FB794 332E0008 */  andi $t6, $t9, 0x0008
    /* 800FB798 15C00014 */  bne $t6, $zero, 0x800FB7EC
    /* 800FB79C 3C0142B4 */  lui $at, 0x42B4
    /* 800FB7A0 44815000 */  .word 0x44815000
    /* 800FB7A4 3C018015 */  lui $at, 0x8015
    /* 800FB7A8 0C029690 */  jal 0x800A5A40
    /* 800FB7AC E42A4188 */  swc1 $f10, 16776($at)
    /* 800FB7B0 3C0F8015 */  lui $t7, 0x8015
    /* 800FB7B4 8DEFA110 */  lw $t7, -24304($t7)
    /* 800FB7B8 24010002 */  addiu $at, $zero, 2
    /* 800FB7BC 3C198015 */  lui $t9, 0x8015
    /* 800FB7C0 15E10005 */  bne $t7, $at, 0x800FB7D8
    /* 800FB7C4 3C048015 */  lui $a0, 0x8015
    /* 800FB7C8 24180001 */  addiu $t8, $zero, 1
    /* 800FB7CC 3C018015 */  lui $at, 0x8015
    /* 800FB7D0 10000004 */  beq $zero, $zero, 0x800FB7E4
    /* 800FB7D4 A4381AD0 */  sh $t8, 6864($at)
    /* 800FB7D8 8739A108 */  lh $t9, -24312($t9)
    /* 800FB7DC 3C018015 */  lui $at, 0x8015
    /* 800FB7E0 A4391AD0 */  sh $t9, 6864($at)
    /* 800FB7E4 0C029AF9 */  jal 0x800A6BE4
    /* 800FB7E8 84841AD0 */  lh $a0, 6864($a0)
    /* 800FB7EC 820E0000 */  lb $t6, 0($s0)
    /* 800FB7F0 3C048015 */  lui $a0, 0x8015
    /* 800FB7F4 8084978C */  lb $a0, -26740($a0)
    /* 800FB7F8 11C00007 */  beq $t6, $zero, 0x800FB818
    /* 800FB7FC 3C018014 */  lui $at, 0x8014
    /* 800FB800 A0203A10 */  sb $zero, 14864($at)
    /* 800FB804 00002825 */  or $a1, $zero, $zero
    /* 800FB808 0C02EE6C */  jal 0x800BB9B0
    /* 800FB80C 24060001 */  addiu $a2, $zero, 1
    /* 800FB810 10000008 */  beq $zero, $zero, 0x800FB834
    /* 800FB814 00000000 */  nop
    /* 800FB818 24050001 */  addiu $a1, $zero, 1
    /* 800FB81C 0C02EE6C */  jal 0x800BB9B0
    /* 800FB820 00003025 */  or $a2, $zero, $zero
    /* 800FB824 14400003 */  bne $v0, $zero, 0x800FB834
    /* 800FB828 00000000 */  nop
    /* 800FB82C 100000E7 */  beq $zero, $zero, 0x800FBBCC
    /* 800FB830 00001025 */  or $v0, $zero, $zero
    /* 800FB834 0C02F6AA */  jal 0x800BDAA8
    /* 800FB838 00000000 */  nop
    /* 800FB83C A2000000 */  sb $zero, 0($s0)
    /* 800FB840 3C0F8015 */  lui $t7, 0x8015
    /* 800FB844 85EF1AD0 */  lh $t7, 6864($t7)
    /* 800FB848 00008825 */  or $s1, $zero, $zero
    /* 800FB84C 19E0001D */  blez $t7, 0x800FB8C4
    /* 800FB850 3C108015 */  lui $s0, 0x8015
    /* 800FB854 26100B94 */  addiu $s0, $s0, 2964
    /* 800FB858 3C188014 */  lui $t8, 0x8014
    /* 800FB85C 831827A0 */  lb $t8, 10144($t8)
    /* 800FB860 3C0E8011 */  lui $t6, 0x8011
    /* 800FB864 25CE4658 */  addiu $t6, $t6, 18008
    /* 800FB868 0018C880 */  sll $t9, $t8, 2
    /* 800FB86C 0338C823 */  subu $t9, $t9, $t8
    /* 800FB870 032E1021 */  addu $v0, $t9, $t6
    /* 800FB874 904F0002 */  lbu $t7, 2($v0)
    /* 800FB878 241800FF */  addiu $t8, $zero, 255
    /* 800FB87C AFB80014 */  sw $t8, 20($sp)
    /* 800FB880 90460000 */  lbu $a2, 0($v0)
    /* 800FB884 90470001 */  lbu $a3, 1($v0)
    /* 800FB888 240403E8 */  addiu $a0, $zero, 1000
    /* 800FB88C 240503E7 */  addiu $a1, $zero, 999
    /* 800FB890 AFB10018 */  sw $s1, 24($sp)
    /* 800FB894 0C029D20 */  jal 0x800A7480
    /* 800FB898 AFAF0010 */  sw $t7, 16($sp)
    /* 800FB89C 02202025 */  or $a0, $s1, $zero
    /* 800FB8A0 0C02AC63 */  jal 0x800AB18C
    /* 800FB8A4 02002825 */  or $a1, $s0, $zero
    /* 800FB8A8 3C198015 */  lui $t9, 0x8015
    /* 800FB8AC 87391AD0 */  lh $t9, 6864($t9)
    /* 800FB8B0 26310001 */  addiu $s1, $s1, 1
    /* 800FB8B4 26100098 */  addiu $s0, $s0, 152
    /* 800FB8B8 0239082A */  slt $at, $s1, $t9
    /* 800FB8BC 1420FFE6 */  bne $at, $zero, 0x800FB858
    /* 800FB8C0 00000000 */  nop
    /* 800FB8C4 3C0E8011 */  lui $t6, 0x8011
    /* 800FB8C8 8DCE74B4 */  lw $t6, 29876($t6)
    /* 800FB8CC 31CF0008 */  andi $t7, $t6, 0x0008
    /* 800FB8D0 15E00014 */  bne $t7, $zero, 0x800FB924
    /* 800FB8D4 3C048014 */  lui $a0, 0x8014
    /* 800FB8D8 24842728 */  addiu $a0, $a0, 10024
    /* 800FB8DC 00002825 */  or $a1, $zero, $zero
    /* 800FB8E0 0C001C9C */  jal 0x80007270
    /* 800FB8E4 24060001 */  addiu $a2, $zero, 1
    /* 800FB8E8 0C0246C0 */  jal 0x80091B00
    /* 800FB8EC 00000000 */  nop
    /* 800FB8F0 24180007 */  addiu $t8, $zero, 7
    /* 800FB8F4 3C048014 */  lui $a0, 0x8014
    /* 800FB8F8 24842728 */  addiu $a0, $a0, 10024
    /* 800FB8FC A0580002 */  sb $t8, 2($v0)
    /* 800FB900 00408025 */  or $s0, $v0, $zero
    /* 800FB904 00002825 */  or $a1, $zero, $zero
    /* 800FB908 0C001D78 */  jal 0x800075E0
    /* 800FB90C 00003025 */  or $a2, $zero, $zero
    /* 800FB910 3C048014 */  lui $a0, 0x8014
    /* 800FB914 248427A8 */  addiu $a0, $a0, 10152
    /* 800FB918 02002825 */  or $a1, $s0, $zero
    /* 800FB91C 0C001D78 */  jal 0x800075E0
    /* 800FB920 00003025 */  or $a2, $zero, $zero
    /* 800FB924 3C018015 */  lui $at, 0x8015
    /* 800FB928 3C198015 */  lui $t9, 0x8015
    /* 800FB92C 87392734 */  lh $t9, 10036($t9)
    /* 800FB930 A0202718 */  sb $zero, 10008($at)
    /* 800FB934 3C018015 */  lui $at, 0x8015
    /* 800FB938 0C03B0BE */  jal 0x800EC2F8
    /* 800FB93C A0392014 */  sb $t9, 8212($at)
    /* 800FB940 3C028014 */  lui $v0, 0x8014
    /* 800FB944 3C038014 */  lui $v1, 0x8014
    /* 800FB948 246339FE */  addiu $v1, $v1, 14846
    /* 800FB94C 244239F8 */  addiu $v0, $v0, 14840
    /* 800FB950 24420001 */  addiu $v0, $v0, 1
    /* 800FB954 0043082B */  sltu $at, $v0, $v1
    /* 800FB958 1420FFFD */  bne $at, $zero, 0x800FB950
    /* 800FB95C A040FFFF */  sb $zero, -1($v0)
    /* 800FB960 0C03EBB9 */  jal 0x800FAEE4
    /* 800FB964 00000000 */  nop
    /* 800FB968 3C018014 */  lui $at, 0x8014
    /* 800FB96C A020FECB */  sb $zero, -309($at)
    /* 800FB970 3C018014 */  lui $at, 0x8014
    /* 800FB974 3C0E8015 */  lui $t6, 0x8015
    /* 800FB978 81CE43C8 */  lb $t6, 17352($t6)
    /* 800FB97C A0202699 */  sb $zero, 9881($at)
    /* 800FB980 3C018014 */  lui $at, 0x8014
    /* 800FB984 A0202690 */  sb $zero, 9872($at)
    /* 800FB988 3C018012 */  lui $at, 0x8012
    /* 800FB98C 000E7880 */  sll $t7, $t6, 2
    /* 800FB990 002F0821 */  addu $at, $at, $t7
    /* 800FB994 C42402F8 */  lwc1 $f4, 760($at)
    /* 800FB998 3C018012 */  lui $at, 0x8012
    /* 800FB99C C42647F4 */  lwc1 $f6, 18420($at)
    /* 800FB9A0 3C048015 */  lui $a0, 0x8015
    /* 800FB9A4 80842744 */  lb $a0, 10052($a0)
    /* 800FB9A8 46062202 */  .word 0x46062202
    /* 800FB9AC 3C018014 */  lui $at, 0x8014
    /* 800FB9B0 3C108015 */  lui $s0, 0x8015
    /* 800FB9B4 2610A250 */  addiu $s0, $s0, -23984
    /* 800FB9B8 00008825 */  or $s1, $zero, $zero
    /* 800FB9BC 18800016 */  blez $a0, 0x800FBA18
    /* 800FB9C0 E4282764 */  swc1 $f8, 10084($at)
    /* 800FB9C4 860207C6 */  lh $v0, 1990($s0)
    /* 800FB9C8 3C038015 */  lui $v1, 0x8015
    /* 800FB9CC 24010006 */  addiu $at, $zero, 6
    /* 800FB9D0 0002C0C0 */  sll $t8, $v0, 3
    /* 800FB9D4 00781821 */  addu $v1, $v1, $t8
    /* 800FB9D8 90633E8F */  lbu $v1, 16015($v1)
    /* 800FB9DC 00022C00 */  sll $a1, $v0, 16
    /* 800FB9E0 0005CC03 */  sra $t9, $a1, 16
    /* 800FB9E4 10600003 */  beq $v1, $zero, 0x800FB9F4
    /* 800FB9E8 03202825 */  or $a1, $t9, $zero
    /* 800FB9EC 54610006 */  .word 0x54610006
    /* 800FB9F0 26310001 */  addiu $s1, $s1, 1
    /* 800FB9F4 0C03950F */  jal 0x800E543C
    /* 800FB9F8 00002025 */  or $a0, $zero, $zero
    /* 800FB9FC 3C048015 */  lui $a0, 0x8015
    /* 800FBA00 80842744 */  lb $a0, 10052($a0)
    /* 800FBA04 26310001 */  addiu $s1, $s1, 1
    /* 800FBA08 0224082A */  slt $at, $s1, $a0
    /* 800FBA0C 1420FFED */  bne $at, $zero, 0x800FB9C4
    /* 800FBA10 26100808 */  addiu $s0, $s0, 2056
    /* 800FBA14 00008825 */  or $s1, $zero, $zero
    /* 800FBA18 0C02C6D2 */  jal 0x800B1B48
    /* 800FBA1C 00000000 */  nop
    /* 800FBA20 0C03EB78 */  jal 0x800FADE0
    /* 800FBA24 00000000 */  nop
    /* 800FBA28 8FAE0100 */  lw $t6, 256($sp)
    /* 800FBA2C 11C00003 */  beq $t6, $zero, 0x800FBA3C
    /* 800FBA30 00000000 */  nop
    /* 800FBA34 0C03C040 */  jal 0x800F0100
    /* 800FBA38 00000000 */  nop
    /* 800FBA3C 3C038015 */  lui $v1, 0x8015
    /* 800FBA40 8463A108 */  lh $v1, -24312($v1)
    /* 800FBA44 3C108015 */  lui $s0, 0x8015
    /* 800FBA48 2610A118 */  addiu $s0, $s0, -24296
    /* 800FBA4C 1860001B */  blez $v1, 0x800FBABC
    /* 800FBA50 3C028015 */  lui $v0, 0x8015
    /* 800FBA54 24422818 */  addiu $v0, $v0, 10264
    /* 800FBA58 44805000 */  .word 0x44805000
    /* 800FBA5C 240F0320 */  addiu $t7, $zero, 800
    /* 800FBA60 A6000040 */  sh $zero, 64($s0)
    /* 800FBA64 A44F0386 */  sh $t7, 902($v0)
    /* 800FBA68 A04003A3 */  sb $zero, 931($v0)
    /* 800FBA6C 3C188015 */  lui $t8, 0x8015
    /* 800FBA70 E44A03AC */  swc1 $f10, 940($v0)
    /* 800FBA74 8F18A110 */  lw $t8, -24304($t8)
    /* 800FBA78 24010006 */  addiu $at, $zero, 6
    /* 800FBA7C 00112400 */  sll $a0, $s1, 16
    /* 800FBA80 17010007 */  bne $t8, $at, 0x800FBAA0
    /* 800FBA84 0004CC03 */  sra $t9, $a0, 16
    /* 800FBA88 03202025 */  or $a0, $t9, $zero
    /* 800FBA8C 0C0E3289 */  jal 0x8038CA24
    /* 800FBA90 AFA20054 */  sw $v0, 84($sp)
    /* 800FBA94 3C038015 */  lui $v1, 0x8015
