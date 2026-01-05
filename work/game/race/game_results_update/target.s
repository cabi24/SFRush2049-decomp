# Source: game_code.bin (decompressed)
# Address: 0x800FEE04

glabel game_results_update
    /* 800FEE04 27BDFF10 */  addiu $sp, $sp, -240
    /* 800FEE08 AFBF001C */  sw $ra, 28($sp)
    /* 800FEE0C 44806000 */  .word 0x44806000
    /* 800FEE10 0C02D96D */  jal 0x800B65B4
    /* 800FEE14 00000000 */  nop
    /* 800FEE18 3C028012 */  lui $v0, 0x8012
    /* 800FEE1C 3C148014 */  lui $s4, 0x8014
    /* 800FEE20 269461D0 */  addiu $s4, $s4, 25040
    /* 800FEE24 24428E20 */  addiu $v0, $v0, -29152
    /* 800FEE28 240E0001 */  addiu $t6, $zero, 1
    /* 800FEE2C AC4E0004 */  sw $t6, 4($v0)
    /* 800FEE30 AC400000 */  sw $zero, 0($v0)
    /* 800FEE34 02802025 */  or $a0, $s4, $zero
    /* 800FEE38 00002825 */  or $a1, $zero, $zero
    /* 800FEE3C 0C001C9C */  jal 0x80007270
    /* 800FEE40 24060001 */  addiu $a2, $zero, 1
    /* 800FEE44 0C02D080 */  jal 0x800B4200
    /* 800FEE48 2412000D */  addiu $s2, $zero, 13
    /* 800FEE4C 00408025 */  or $s0, $v0, $zero
    /* 800FEE50 02802025 */  or $a0, $s4, $zero
    /* 800FEE54 00002825 */  or $a1, $zero, $zero
    /* 800FEE58 0C001D78 */  jal 0x800075E0
    /* 800FEE5C 00003025 */  or $a2, $zero, $zero
    /* 800FEE60 0C02DD28 */  jal 0x800B74A0
    /* 800FEE64 24040001 */  addiu $a0, $zero, 1
    /* 800FEE68 3C058011 */  lui $a1, 0x8011
    /* 800FEE6C 3C068015 */  lui $a2, 0x8015
    /* 800FEE70 27AF00B8 */  addiu $t7, $sp, 184
    /* 800FEE74 AFAF0010 */  sw $t7, 16($sp)
    /* 800FEE78 24C60B70 */  addiu $a2, $a2, 2928
    /* 800FEE7C 24A505A4 */  addiu $a1, $a1, 1444
    /* 800FEE80 00002025 */  or $a0, $zero, $zero
    /* 800FEE84 0C029891 */  jal 0x800A6244
    /* 800FEE88 00003825 */  or $a3, $zero, $zero
    /* 800FEE8C 3C168018 */  lui $s6, 0x8018
    /* 800FEE90 26D6A4E0 */  addiu $s6, $s6, -23328
    /* 800FEE94 8ED80004 */  lw $t8, 4($s6)
    /* 800FEE98 87B100B8 */  lh $s1, 184($sp)
    /* 800FEE9C 87B200BA */  lh $s2, 186($sp)
    /* 800FEEA0 2405FFFF */  addiu $a1, $zero, -1
    /* 800FEEA4 0C02CFE9 */  jal 0x800B3FA4
    /* 800FEEA8 8F0400D0 */  lw $a0, 208($t8)
    /* 800FEEAC 00027842 */  srl $t7, $v0, 1
    /* 800FEEB0 022FC823 */  subu $t9, $s1, $t7
    /* 800FEEB4 8ED80004 */  lw $t8, 4($s6)
    /* 800FEEB8 00192400 */  sll $a0, $t9, 16
    /* 800FEEBC 00122C00 */  sll $a1, $s2, 16
    /* 800FEEC0 00057C03 */  sra $t7, $a1, 16
    /* 800FEEC4 00047403 */  sra $t6, $a0, 16
    /* 800FEEC8 01C02025 */  or $a0, $t6, $zero
    /* 800FEECC 01E02825 */  or $a1, $t7, $zero
    /* 800FEED0 0C02DC75 */  jal 0x800B71D4
    /* 800FEED4 8F0600D0 */  lw $a2, 208($t8)
    /* 800FEED8 02802025 */  or $a0, $s4, $zero
    /* 800FEEDC 00002825 */  or $a1, $zero, $zero
    /* 800FEEE0 0C001C9C */  jal 0x80007270
    /* 800FEEE4 24060001 */  addiu $a2, $zero, 1
    /* 800FEEE8 0C02D080 */  jal 0x800B4200
    /* 800FEEEC 2412000B */  addiu $s2, $zero, 11
    /* 800FEEF0 00408025 */  or $s0, $v0, $zero
    /* 800FEEF4 02802025 */  or $a0, $s4, $zero
    /* 800FEEF8 00002825 */  or $a1, $zero, $zero
    /* 800FEEFC 0C001D78 */  jal 0x800075E0
    /* 800FEF00 00003025 */  or $a2, $zero, $zero
    /* 800FEF04 3C014300 */  lui $at, 0x4300
    /* 800FEF08 4481F000 */  .word 0x4481F000
    /* 800FEF0C 3C018012 */  lui $at, 0x8012
    /* 800FEF10 C43C4828 */  lwc1 $f28, 18472($at)
    /* 800FEF14 3C018012 */  lui $at, 0x8012
    /* 800FEF18 C43A482C */  lwc1 $f26, 18476($at)
    /* 800FEF1C 3C018012 */  lui $at, 0x8012
    /* 800FEF20 C4384830 */  lwc1 $f24, 18480($at)
    /* 800FEF24 3C018012 */  lui $at, 0x8012
    /* 800FEF28 C4364834 */  lwc1 $f22, 18484($at)
    /* 800FEF2C 3C01BF80 */  lui $at, 0xBF80
    /* 800FEF30 3C178011 */  lui $s7, 0x8011
    /* 800FEF34 3C1E8014 */  lui $fp, 0x8014
    /* 800FEF38 4481A000 */  .word 0x4481A000
    /* 800FEF3C 27DE6108 */  addiu $fp, $fp, 24840
    /* 800FEF40 26F702B4 */  addiu $s7, $s7, 692
    /* 800FEF44 00009825 */  or $s3, $zero, $zero
    /* 800FEF48 27B500BC */  addiu $s5, $sp, 188
    /* 800FEF4C C6E00008 */  lwc1 $f0, 8($s7)
    /* 800FEF50 26F90130 */  addiu $t9, $s7, 304
    /* 800FEF54 4600B03C */  .word 0x4600B03C
    /* 800FEF58 00000000 */  nop
    /* 800FEF5C 45020006 */  .word 0x45020006
    /* 800FEF60 4600D03C */  .word 0x4600D03C
    /* 800FEF64 4618003C */  .word 0x4618003C
    /* 800FEF68 00000000 */  nop
    /* 800FEF6C 450300B0 */  .word 0x450300B0
    /* 800FEF70 26730001 */  addiu $s3, $s3, 1
    /* 800FEF74 4600D03C */  .word 0x4600D03C
    /* 800FEF78 26F10030 */  addiu $s1, $s7, 48
    /* 800FEF7C 0013A080 */  sll $s4, $s3, 2
    /* 800FEF80 02A08025 */  or $s0, $s5, $zero
    /* 800FEF84 45000009 */  .word 0x45000009
    /* 800FEF88 AFB90024 */  sw $t9, 36($sp)
    /* 800FEF8C 461C003C */  .word 0x461C003C
    /* 800FEF90 00000000 */  nop
    /* 800FEF94 45000005 */  .word 0x45000005
    /* 800FEF98 00000000 */  nop
    /* 800FEF9C 0C03B59B */  jal 0x800ED66C
    /* 800FEFA0 4600F306 */  .word 0x4600F306
    /* 800FEFA4 10000003 */  beq $zero, $zero, 0x800FEFB4
    /* 800FEFA8 00000000 */  nop
    /* 800FEFAC 0C03B59B */  jal 0x800ED66C
    /* 800FEFB0 4600A306 */  .word 0x4600A306
    /* 800FEFB4 3C018012 */  lui $at, 0x8012
    /* 800FEFB8 C4264838 */  lwc1 $f6, 18488($at)
    /* 800FEFBC C6E40008 */  lwc1 $f4, 8($s7)
    /* 800FEFC0 24040001 */  addiu $a0, $zero, 1
    /* 800FEFC4 3C018012 */  lui $at, 0x8012
    /* 800FEFC8 4606203C */  .word 0x4606203C
    /* 800FEFCC 3C0E8011 */  lui $t6, 0x8011
    /* 800FEFD0 3C0F8011 */  lui $t7, 0x8011
    /* 800FEFD4 4500000E */  .word 0x4500000E
    /* 800FEFD8 00000000 */  nop
    /* 800FEFDC 3C0E8011 */  lui $t6, 0x8011
    /* 800FEFE0 8DCE46BC */  lw $t6, 18108($t6)
    /* 800FEFE4 3C0F8011 */  lui $t7, 0x8011
    /* 800FEFE8 8DEF46C0 */  lw $t7, 18112($t7)
    /* 800FEFEC 3C018012 */  lui $at, 0x8012
    /* 800FEFF0 AC2E8E28 */  sw $t6, -29144($at)
    /* 800FEFF4 3C018012 */  lui $at, 0x8012
    /* 800FEFF8 AFAE0050 */  sw $t6, 80($sp)
    /* 800FEFFC AC2F8E2C */  sw $t7, -29140($at)
    /* 800FF000 0C02DD28 */  jal 0x800B74A0
    /* 800FF004 AFAF0054 */  sw $t7, 84($sp)
    /* 800FF008 1000000A */  beq $zero, $zero, 0x800FF034
    /* 800FF00C 00000000 */  nop
    /* 800FF010 8DCE46BC */  lw $t6, 18108($t6)
    /* 800FF014 8DEF46C0 */  lw $t7, 18112($t7)
    /* 800FF018 24040016 */  addiu $a0, $zero, 22
    /* 800FF01C AC2E8E28 */  sw $t6, -29144($at)
    /* 800FF020 3C018012 */  lui $at, 0x8012
    /* 800FF024 AFAE0048 */  sw $t6, 72($sp)
    /* 800FF028 AC2F8E2C */  sw $t7, -29140($at)
    /* 800FF02C 0C02DD28 */  jal 0x800B74A0
    /* 800FF030 AFAF004C */  sw $t7, 76($sp)
    /* 800FF034 3C068015 */  lui $a2, 0x8015
    /* 800FF038 27AE00B8 */  addiu $t6, $sp, 184
    /* 800FF03C AFAE0010 */  sw $t6, 16($sp)
    /* 800FF040 24C60B70 */  addiu $a2, $a2, 2928
    /* 800FF044 00002025 */  or $a0, $zero, $zero
    /* 800FF048 02202825 */  or $a1, $s1, $zero
    /* 800FF04C 0C029891 */  jal 0x800A6244
    /* 800FF050 00003825 */  or $a3, $zero, $zero
    /* 800FF054 87B100B8 */  lh $s1, 184($sp)
    /* 800FF058 87B200BA */  lh $s2, 186($sp)
    /* 800FF05C 8ECE000C */  lw $t6, 12($s6)
    /* 800FF060 2631FFC9 */  addiu $s1, $s1, -55
    /* 800FF064 00112400 */  sll $a0, $s1, 16
    /* 800FF068 00047C03 */  sra $t7, $a0, 16
    /* 800FF06C 01E02025 */  or $a0, $t7, $zero
    /* 800FF070 95CF004E */  lhu $t7, 78($t6)
    /* 800FF074 00122C00 */  sll $a1, $s2, 16
    /* 800FF078 8ED90010 */  lw $t9, 16($s6)
    /* 800FF07C 0005C403 */  sra $t8, $a1, 16
    /* 800FF080 03002825 */  or $a1, $t8, $zero
    /* 800FF084 000FC080 */  sll $t8, $t7, 2
    /* 800FF088 03387021 */  addu $t6, $t9, $t8
    /* 800FF08C 01D47821 */  addu $t7, $t6, $s4
    /* 800FF090 0C02DC75 */  jal 0x800B71D4
    /* 800FF094 8DE60000 */  lw $a2, 0($t7)
    /* 800FF098 1260000C */  beq $s3, $zero, 0x800FF0CC
    /* 800FF09C 02A02025 */  or $a0, $s5, $zero
    /* 800FF0A0 24010001 */  addiu $at, $zero, 1
    /* 800FF0A4 1261000F */  beq $s3, $at, 0x800FF0E4
    /* 800FF0A8 02A02025 */  or $a0, $s5, $zero
    /* 800FF0AC 24010002 */  addiu $at, $zero, 2
    /* 800FF0B0 12610012 */  beq $s3, $at, 0x800FF0FC
    /* 800FF0B4 3C028011 */  lui $v0, 0x8011
    /* 800FF0B8 24010003 */  addiu $at, $zero, 3
    /* 800FF0BC 5261002E */  .word 0x5261002E
    /* 800FF0C0 83D9000F */  lb $t9, 15($fp)
    /* 800FF0C4 10000037 */  beq $zero, $zero, 0x800FF1A4
    /* 800FF0C8 93B800BC */  lbu $t8, 188($sp)
    /* 800FF0CC 3C058012 */  lui $a1, 0x8012
    /* 800FF0D0 24A5F6E0 */  addiu $a1, $a1, -2336
    /* 800FF0D4 0C02D0D8 */  jal 0x800B4360
    /* 800FF0D8 83C6000D */  lb $a2, 13($fp)
    /* 800FF0DC 10000031 */  beq $zero, $zero, 0x800FF1A4
    /* 800FF0E0 93B800BC */  lbu $t8, 188($sp)
    /* 800FF0E4 3C058012 */  lui $a1, 0x8012
    /* 800FF0E8 24A5F6E4 */  addiu $a1, $a1, -2332
    /* 800FF0EC 0C02D0D8 */  jal 0x800B4360
    /* 800FF0F0 83C6000C */  lb $a2, 12($fp)
    /* 800FF0F4 1000002B */  beq $zero, $zero, 0x800FF1A4
    /* 800FF0F8 93B800BC */  lbu $t8, 188($sp)
    /* 800FF0FC 8C420640 */  lw $v0, 1600($v0)
    /* 800FF100 2401000C */  addiu $at, $zero, 12
    /* 800FF104 54410008 */  .word 0x54410008
    /* 800FF108 2401000D */  addiu $at, $zero, 13
    /* 800FF10C 8ED90004 */  lw $t9, 4($s6)
    /* 800FF110 02A02025 */  or $a0, $s5, $zero
    /* 800FF114 0C02F9A9 */  jal 0x800BE6A4
    /* 800FF118 8F250088 */  lw $a1, 136($t9)
    /* 800FF11C 10000021 */  beq $zero, $zero, 0x800FF1A4
    /* 800FF120 93B800BC */  lbu $t8, 188($sp)
    /* 800FF124 2401000D */  addiu $at, $zero, 13
    /* 800FF128 1441000B */  bne $v0, $at, 0x800FF158
    /* 800FF12C 02A02025 */  or $a0, $s5, $zero
    /* 800FF130 8ECE000C */  lw $t6, 12($s6)
    /* 800FF134 8ED80010 */  lw $t8, 16($s6)
    /* 800FF138 02A02025 */  or $a0, $s5, $zero
    /* 800FF13C 95CF0020 */  lhu $t7, 32($t6)
    /* 800FF140 000FC880 */  sll $t9, $t7, 2
    /* 800FF144 03197021 */  addu $t6, $t8, $t9
    /* 800FF148 0C02F9A9 */  jal 0x800BE6A4
    /* 800FF14C 8DC50000 */  lw $a1, 0($t6)
    /* 800FF150 10000014 */  beq $zero, $zero, 0x800FF1A4
    /* 800FF154 93B800BC */  lbu $t8, 188($sp)
    /* 800FF158 00027880 */  sll $t7, $v0, 2
    /* 800FF15C 3C058011 */  lui $a1, 0x8011
    /* 800FF160 00AF2821 */  addu $a1, $a1, $t7
    /* 800FF164 0C02F9A9 */  jal 0x800BE6A4
    /* 800FF168 8CA50030 */  lw $a1, 48($a1)
    /* 800FF16C 1000000D */  beq $zero, $zero, 0x800FF1A4
    /* 800FF170 93B800BC */  lbu $t8, 188($sp)
    /* 800FF174 83D9000F */  lb $t9, 15($fp)
    /* 800FF178 8ED80010 */  lw $t8, 16($s6)
    /* 800FF17C 02A02025 */  or $a0, $s5, $zero
    /* 800FF180 00197080 */  sll $t6, $t9, 2
    /* 800FF184 8ED9000C */  lw $t9, 12($s6)
    /* 800FF188 030E7821 */  addu $t7, $t8, $t6
    /* 800FF18C 97380050 */  lhu $t8, 80($t9)
    /* 800FF190 00187080 */  sll $t6, $t8, 2
    /* 800FF194 01EEC821 */  addu $t9, $t7, $t6
    /* 800FF198 0C02F9A9 */  jal 0x800BE6A4
    /* 800FF19C 8F250000 */  lw $a1, 0($t9)
    /* 800FF1A0 93B800BC */  lbu $t8, 188($sp)
    /* 800FF1A4 3C068015 */  lui $a2, 0x8015
    /* 800FF1A8 24C60B70 */  addiu $a2, $a2, 2928
    /* 800FF1AC 1300000F */  beq $t8, $zero, 0x800FF1EC
    /* 800FF1B0 00002025 */  or $a0, $zero, $zero
    /* 800FF1B4 27AF00BC */  addiu $t7, $sp, 188
    /* 800FF1B8 91E20000 */  lbu $v0, 0($t7)
    /* 800FF1BC 28410061 */  slti $at, $v0, 97
    /* 800FF1C0 14200005 */  bne $at, $zero, 0x800FF1D8
    /* 800FF1C4 2841007B */  slti $at, $v0, 123
    /* 800FF1C8 10200003 */  beq $at, $zero, 0x800FF1D8
    /* 800FF1CC 384E0020 */  xori $t6, $v0, 0x0020
    /* 800FF1D0 10000002 */  beq $zero, $zero, 0x800FF1DC
    /* 800FF1D4 A20E0000 */  sb $t6, 0($s0)
    /* 800FF1D8 A2020000 */  sb $v0, 0($s0)
    /* 800FF1DC 92020001 */  lbu $v0, 1($s0)
    /* 800FF1E0 26100001 */  addiu $s0, $s0, 1
    /* 800FF1E4 5440FFF6 */  .word 0x5440FFF6
    /* 800FF1E8 28410061 */  slti $at, $v0, 97
    /* 800FF1EC 27B900B8 */  addiu $t9, $sp, 184
    /* 800FF1F0 AFB90010 */  sw $t9, 16($sp)
    /* 800FF1F4 8FA50024 */  lw $a1, 36($sp)
    /* 800FF1F8 0C029891 */  jal 0x800A6244
    /* 800FF1FC 00003825 */  or $a3, $zero, $zero
    /* 800FF200 87B100B8 */  lh $s1, 184($sp)
    /* 800FF204 87B200BA */  lh $s2, 186($sp)
    /* 800FF208 02A03025 */  or $a2, $s5, $zero
    /* 800FF20C 2631FFDD */  addiu $s1, $s1, -35
    /* 800FF210 00112400 */  sll $a0, $s1, 16
    /* 800FF214 00122C00 */  sll $a1, $s2, 16
    /* 800FF218 00057C03 */  sra $t7, $a1, 16
    /* 800FF21C 0004C403 */  sra $t8, $a0, 16
    /* 800FF220 03002025 */  or $a0, $t8, $zero
    /* 800FF224 0C02DC75 */  jal 0x800B71D4
    /* 800FF228 01E02825 */  or $a1, $t7, $zero
    /* 800FF22C 26730001 */  addiu $s3, $s3, 1
    /* 800FF230 24010004 */  addiu $at, $zero, 4
    /* 800FF234 1661FF45 */  bne $s3, $at, 0x800FEF4C
    /* 800FF238 26F70040 */  addiu $s7, $s7, 64
    /* 800FF23C 3C028012 */  lui $v0, 0x8012
    /* 800FF240 24428E20 */  addiu $v0, $v0, -29152
    /* 800FF244 240E0003 */  addiu $t6, $zero, 3
    /* 800FF248 AC4E0004 */  sw $t6, 4($v0)
    /* 800FF24C AC400000 */  sw $zero, 0($v0)
    /* 800FF250 0C03B59B */  jal 0x800ED66C
    /* 800FF254 4600A306 */  .word 0x4600A306
    /* 800FF258 3C198011 */  lui $t9, 0x8011
    /* 800FF25C 8F3946BC */  lw $t9, 18108($t9)
    /* 800FF260 3C188011 */  lui $t8, 0x8011
    /* 800FF264 8F1846C0 */  lw $t8, 18112($t8)
    /* 800FF268 3C018012 */  lui $at, 0x8012
    /* 800FF26C AC398E28 */  sw $t9, -29144($at)
    /* 800FF270 3C018012 */  lui $at, 0x8012
    /* 800FF274 4600A306 */  .word 0x4600A306
    /* 800FF278 AFB90038 */  sw $t9, 56($sp)
    /* 800FF27C AC388E2C */  sw $t8, -29140($at)
    /* 800FF280 0C02D96D */  jal 0x800B65B4
    /* 800FF284 AFB8003C */  sw $t8, 60($sp)
    /* 800FF288 8FBF001C */  lw $ra, 28($sp)
    /* 800FF28C 27BD00F0 */  addiu $sp, $sp, 240
    /* 800FF290 03E00008 */  jr $ra
    /* 800FF294 00000000 */  nop
