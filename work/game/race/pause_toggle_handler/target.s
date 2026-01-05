# Source: game_code.bin (decompressed)
# Address: 0x800FB5F4

glabel pause_toggle_handler
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
    /* 800FBA98 8463A108 */  lh $v1, -24312($v1)
    /* 800FBA9C 8FA20054 */  lw $v0, 84($sp)
    /* 800FBAA0 3C018011 */  lui $at, 0x8011
    /* 800FBAA4 26310001 */  addiu $s1, $s1, 1
    /* 800FBAA8 A0204654 */  sb $zero, 18004($at)
    /* 800FBAAC 0223082A */  slt $at, $s1, $v1
    /* 800FBAB0 2610004C */  addiu $s0, $s0, 76
    /* 800FBAB4 1420FFE8 */  bne $at, $zero, 0x800FBA58
    /* 800FBAB8 244203B8 */  addiu $v0, $v0, 952
    /* 800FBABC 3C0E8011 */  lui $t6, 0x8011
    /* 800FBAC0 8DCE74B4 */  lw $t6, 29876($t6)
    /* 800FBAC4 31CF0008 */  andi $t7, $t6, 0x0008
    /* 800FBAC8 15E00003 */  bne $t7, $zero, 0x800FBAD8
    /* 800FBACC 00000000 */  nop
    /* 800FBAD0 0C02D84E */  jal 0x800B6138
    /* 800FBAD4 00000000 */  nop
    /* 800FBAD8 0C03594C */  jal 0x800D6530
    /* 800FBADC 00000000 */  nop
    /* 800FBAE0 3C028015 */  lui $v0, 0x8015
    /* 800FBAE4 8C42A110 */  lw $v0, -24304($v0)
    /* 800FBAE8 24010006 */  addiu $at, $zero, 6
    /* 800FBAEC 3C048015 */  lui $a0, 0x8015
    /* 800FBAF0 1441000D */  bne $v0, $at, 0x800FBB28
    /* 800FBAF4 24849428 */  addiu $a0, $a0, -27608
    /* 800FBAF8 00002825 */  or $a1, $zero, $zero
    /* 800FBAFC 0C0009E4 */  jal 0x80002790
    /* 800FBB00 24060010 */  addiu $a2, $zero, 16
    /* 800FBB04 3C048015 */  lui $a0, 0x8015
    /* 800FBB08 2484256C */  addiu $a0, $a0, 9580
    /* 800FBB0C 00002825 */  or $a1, $zero, $zero
    /* 800FBB10 0C0009E4 */  jal 0x80002790
    /* 800FBB14 24060004 */  addiu $a2, $zero, 4
    /* 800FBB18 0C0E4234 */  jal 0x803908D0
    /* 800FBB1C 00000000 */  nop
    /* 800FBB20 3C028015 */  lui $v0, 0x8015
    /* 800FBB24 8C42A110 */  lw $v0, -24304($v0)
    /* 800FBB28 24010004 */  addiu $at, $zero, 4
    /* 800FBB2C 14410019 */  bne $v0, $at, 0x800FBB94
    /* 800FBB30 3C048015 */  lui $a0, 0x8015
    /* 800FBB34 24842038 */  addiu $a0, $a0, 8248
    /* 800FBB38 00002825 */  or $a1, $zero, $zero
    /* 800FBB3C 0C0009E4 */  jal 0x80002790
    /* 800FBB40 240601E0 */  addiu $a2, $zero, 480
    /* 800FBB44 3C048015 */  lui $a0, 0x8015
    /* 800FBB48 3C118015 */  lui $s1, 0x8015
    /* 800FBB4C 26316BA8 */  addiu $s1, $s1, 27560
    /* 800FBB50 248469B8 */  addiu $a0, $a0, 27064
    /* 800FBB54 3C101500 */  lui $s0, 0x1500
    /* 800FBB58 00002825 */  or $a1, $zero, $zero
    /* 800FBB5C 2406007C */  addiu $a2, $zero, 124
    /* 800FBB60 0C0009E4 */  jal 0x80002790
    /* 800FBB64 AFA4005C */  sw $a0, 92($sp)
    /* 800FBB68 8FA4005C */  lw $a0, 92($sp)
    /* 800FBB6C 8C980000 */  lw $t8, 0($a0)
    /* 800FBB70 2484007C */  addiu $a0, $a0, 124
    /* 800FBB74 0310C825 */  or $t9, $t8, $s0
    /* 800FBB78 1491FFF7 */  bne $a0, $s1, 0x800FBB58
    /* 800FBB7C AC99FF84 */  sw $t9, -124($a0)
    /* 800FBB80 44802000 */  .word 0x44802000
    /* 800FBB84 3C018016 */  lui $at, 0x8016
    /* 800FBB88 E424139C */  swc1 $f4, 5020($at)
    /* 800FBB8C 3C018015 */  lui $at, 0x8015
    /* 800FBB90 AC202738 */  sw $zero, 10040($at)
    /* 800FBB94 3C0E8011 */  lui $t6, 0x8011
    /* 800FBB98 8DCE74B4 */  lw $t6, 29876($t6)
    /* 800FBB9C 24180001 */  addiu $t8, $zero, 1
    /* 800FBBA0 3C018012 */  lui $at, 0x8012
    /* 800FBBA4 31CF0008 */  andi $t7, $t6, 0x0008
    /* 800FBBA8 15E00002 */  bne $t7, $zero, 0x800FBBB4
    /* 800FBBAC 3C198011 */  lui $t9, 0x8011
    /* 800FBBB0 A038EAE4 */  sb $t8, -5404($at)
    /* 800FBBB4 873964AA */  lh $t9, 25770($t9)
    /* 800FBBB8 3C018015 */  lui $at, 0x8015
    /* 800FBBBC 24020001 */  addiu $v0, $zero, 1
    /* 800FBBC0 A0391AD8 */  sb $t9, 6872($at)
    /* 800FBBC4 3C018011 */  lui $at, 0x8011
    /* 800FBBC8 A02046F4 */  sb $zero, 18164($at)
    /* 800FBBCC 8FBF004C */  lw $ra, 76($sp)
    /* 800FBBD0 8FB00028 */  lw $s0, 40($sp)
    /* 800FBBD4 8FB1002C */  lw $s1, 44($sp)
    /* 800FBBD8 8FB20030 */  lw $s2, 48($sp)
    /* 800FBBDC 8FB30034 */  lw $s3, 52($sp)
    /* 800FBBE0 8FB40038 */  lw $s4, 56($sp)
    /* 800FBBE4 8FB5003C */  lw $s5, 60($sp)
    /* 800FBBE8 8FB60040 */  lw $s6, 64($sp)
    /* 800FBBEC 8FB70044 */  lw $s7, 68($sp)
    /* 800FBBF0 8FBE0048 */  lw $fp, 72($sp)
    /* 800FBBF4 03E00008 */  jr $ra
    /* 800FBBF8 27BD0100 */  addiu $sp, $sp, 256
