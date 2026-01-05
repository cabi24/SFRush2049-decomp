# Source: game_code.bin (decompressed)
# Address: 0x800FB928

glabel hud_timer_display
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
