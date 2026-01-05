# Source: game_code.bin (decompressed)
# Address: 0x800EEDB0

glabel emitter_update
    /* 800EEDB0 27BDFF28 */  addiu $sp, $sp, -216
    /* 800EEDB4 3C0E8011 */  lui $t6, 0x8011
    /* 800EEDB8 8DCE74B4 */  lw $t6, 29876($t6)
    /* 800EEDBC AFBF002C */  sw $ra, 44($sp)
    /* 800EEDC0 AFB50028 */  sw $s5, 40($sp)
    /* 800EEDC4 000E7A40 */  sll $t7, $t6, 9
    /* 800EEDC8 AFB40024 */  sw $s4, 36($sp)
    /* 800EEDCC AFB30020 */  sw $s3, 32($sp)
    /* 800EEDD0 AFB2001C */  sw $s2, 28($sp)
    /* 800EEDD4 AFB10018 */  sw $s1, 24($sp)
    /* 800EEDD8 AFB00014 */  sw $s0, 20($sp)
    /* 800EEDDC 05E1001A */  bgez $t7, 0x800EEE48
    /* 800EEDE0 AFA400D8 */  sw $a0, 216($sp)
    /* 800EEDE4 3C188016 */  lui $t8, 0x8016
    /* 800EEDE8 8318B24C */  lb $t8, -19892($t8)
    /* 800EEDEC 3C028015 */  lui $v0, 0x8015
    /* 800EEDF0 13000015 */  beq $t8, $zero, 0x800EEE48
    /* 800EEDF4 00000000 */  nop
    /* 800EEDF8 8C42A110 */  lw $v0, -24304($v0)
    /* 800EEDFC 24010006 */  addiu $at, $zero, 6
    /* 800EEE00 3C198014 */  lui $t9, 0x8014
    /* 800EEE04 14410004 */  bne $v0, $at, 0x800EEE18
    /* 800EEE08 24130004 */  addiu $s3, $zero, 4
    /* 800EEE0C 8F390B08 */  lw $t9, 2824($t9)
    /* 800EEE10 1320000D */  beq $t9, $zero, 0x800EEE48
    /* 800EEE14 00000000 */  nop
    /* 800EEE18 16620005 */  bne $s3, $v0, 0x800EEE30
    /* 800EEE1C 3C0E8014 */  lui $t6, 0x8014
    /* 800EEE20 8DCE07BC */  lw $t6, 1980($t6)
    /* 800EEE24 24120001 */  addiu $s2, $zero, 1
    /* 800EEE28 124E0007 */  beq $s2, $t6, 0x800EEE48
    /* 800EEE2C 00000000 */  nop
    /* 800EEE30 10400005 */  beq $v0, $zero, 0x800EEE48
    /* 800EEE34 24120001 */  addiu $s2, $zero, 1
    /* 800EEE38 12420003 */  beq $s2, $v0, 0x800EEE48
    /* 800EEE3C 24010003 */  addiu $at, $zero, 3
    /* 800EEE40 14410003 */  bne $v0, $at, 0x800EEE50
    /* 800EEE44 00000000 */  nop
    /* 800EEE48 10000106 */  beq $zero, $zero, 0x800EF264
    /* 800EEE4C 24020001 */  addiu $v0, $zero, 1
    /* 800EEE50 0C00055E */  jal 0x80001578
    /* 800EEE54 00000000 */  nop
    /* 800EEE58 4600010D */  .word 0x4600010D
    /* 800EEE5C 3C018012 */  lui $at, 0x8012
    /* 800EEE60 2402001E */  addiu $v0, $zero, 30
    /* 800EEE64 44102000 */  .word 0x44102000
    /* 800EEE68 00000000 */  nop
    /* 800EEE6C 06010002 */  bgez $s0, 0x800EEE78
    /* 800EEE70 00000000 */  nop
    /* 800EEE74 00008025 */  or $s0, $zero, $zero
    /* 800EEE78 AC304F74 */  sw $s0, 20340($at)
    /* 800EEE7C 2A01001F */  slti $at, $s0, 31
    /* 800EEE80 10200026 */  beq $at, $zero, 0x800EEF1C
    /* 800EEE84 00000000 */  nop
    /* 800EEE88 3C028011 */  lui $v0, 0x8011
    /* 800EEE8C 8C4261B8 */  lw $v0, 25016($v0)
    /* 800EEE90 3C188011 */  lui $t8, 0x8011
    /* 800EEE94 0050082A */  slt $at, $v0, $s0
    /* 800EEE98 14200013 */  bne $at, $zero, 0x800EEEE8
    /* 800EEE9C 28410006 */  slti $at, $v0, 6
    /* 800EEEA0 14200005 */  bne $at, $zero, 0x800EEEB8
    /* 800EEEA4 2442FFFF */  addiu $v0, $v0, -1
    /* 800EEEA8 24020005 */  addiu $v0, $zero, 5
    /* 800EEEAC 3C018011 */  lui $at, 0x8011
    /* 800EEEB0 10000003 */  beq $zero, $zero, 0x800EEEC0
    /* 800EEEB4 AC2261B8 */  sw $v0, 25016($at)
    /* 800EEEB8 3C018011 */  lui $at, 0x8011
    /* 800EEEBC AC2261B8 */  sw $v0, 25016($at)
    /* 800EEEC0 8318FFC0 */  lb $t8, -64($t8)
    /* 800EEEC4 00003825 */  or $a3, $zero, $zero
    /* 800EEEC8 57000004 */  .word 0x57000004
    /* 800EEECC 24040002 */  addiu $a0, $zero, 2
    /* 800EEED0 10000005 */  beq $zero, $zero, 0x800EEEE8
    /* 800EEED4 00000000 */  nop
    /* 800EEED8 24040002 */  addiu $a0, $zero, 2
    /* 800EEEDC 00002825 */  or $a1, $zero, $zero
    /* 800EEEE0 0C0248D8 */  jal 0x80092360
    /* 800EEEE4 24060002 */  addiu $a2, $zero, 2
    /* 800EEEE8 3C198012 */  lui $t9, 0x8012
    /* 800EEEEC 8F394F74 */  lw $t9, 20340($t9)
    /* 800EEEF0 3C0E8003 */  lui $t6, 0x8003
    /* 800EEEF4 25CEE8E8 */  addiu $t6, $t6, -5912
    /* 800EEEF8 2B210006 */  slti $at, $t9, 6
    /* 800EEEFC 10200009 */  beq $at, $zero, 0x800EEF24
    /* 800EEF00 00000000 */  nop
    /* 800EEF04 8DCF027C */  lw $t7, 636($t6)
    /* 800EEF08 31F80008 */  andi $t8, $t7, 0x0008
    /* 800EEF0C 13000005 */  beq $t8, $zero, 0x800EEF24
    /* 800EEF10 00000000 */  nop
    /* 800EEF14 100000D3 */  beq $zero, $zero, 0x800EF264
    /* 800EEF18 24020001 */  addiu $v0, $zero, 1
    /* 800EEF1C 3C018011 */  lui $at, 0x8011
    /* 800EEF20 AC2261B8 */  sw $v0, 25016($at)
    /* 800EEF24 3C118015 */  lui $s1, 0x8015
    /* 800EEF28 26311AD0 */  addiu $s1, $s1, 6864
    /* 800EEF2C 86390000 */  lh $t9, 0($s1)
    /* 800EEF30 3C0F8011 */  lui $t7, 0x8011
    /* 800EEF34 25EF5C68 */  addiu $t7, $t7, 23656
    /* 800EEF38 001970C0 */  sll $t6, $t9, 3
    /* 800EEF3C 01CF1021 */  addu $v0, $t6, $t7
    /* 800EEF40 44806000 */  .word 0x44806000
    /* 800EEF44 8C54FFF8 */  lw $s4, -8($v0)
    /* 800EEF48 0C02D96D */  jal 0x800B65B4
    /* 800EEF4C 8C55FFFC */  lw $s5, -4($v0)
    /* 800EEF50 3C078012 */  lui $a3, 0x8012
    /* 800EEF54 24E78E20 */  addiu $a3, $a3, -29152
    /* 800EEF58 24180003 */  addiu $t8, $zero, 3
    /* 800EEF5C ACF80004 */  sw $t8, 4($a3)
    /* 800EEF60 ACF20000 */  sw $s2, 0($a3)
    /* 800EEF64 3C198015 */  lui $t9, 0x8015
    /* 800EEF68 8F39A110 */  lw $t9, -24304($t9)
    /* 800EEF6C 3C048014 */  lui $a0, 0x8014
    /* 800EEF70 248461D0 */  addiu $a0, $a0, 25040
    /* 800EEF74 1679000B */  bne $s3, $t9, 0x800EEFA4
    /* 800EEF78 00002825 */  or $a1, $zero, $zero
    /* 800EEF7C 86220000 */  lh $v0, 0($s1)
    /* 800EEF80 28410002 */  slti $at, $v0, 2
    /* 800EEF84 14200007 */  bne $at, $zero, 0x800EEFA4
    /* 800EEF88 000270C0 */  sll $t6, $v0, 3
    /* 800EEF8C 3C0F8011 */  lui $t7, 0x8011
    /* 800EEF90 25EF60A8 */  addiu $t7, $t7, 24744
    /* 800EEF94 01CF1821 */  addu $v1, $t6, $t7
    /* 800EEF98 8C74FFF8 */  lw $s4, -8($v1)
    /* 800EEF9C 8C75FFFC */  lw $s5, -4($v1)
    /* 800EEFA0 ACF20004 */  sw $s2, 4($a3)
    /* 800EEFA4 24010258 */  addiu $at, $zero, 600
    /* 800EEFA8 0201001A */  div $s0, $at
    /* 800EEFAC 0000C012 */  mflo $t8
    /* 800EEFB0 2401003C */  addiu $at, $zero, 60
    /* 800EEFB4 2403000A */  addiu $v1, $zero, 10
    /* 800EEFB8 0201001A */  div $s0, $at
    /* 800EEFBC 00007012 */  mflo $t6
    /* 800EEFC0 27190030 */  addiu $t9, $t8, 48
    /* 800EEFC4 A3B900CC */  sb $t9, 204($sp)
    /* 800EEFC8 01C3001A */  div $t6, $v1
    /* 800EEFCC 00007810 */  mfhi $t7
    /* 800EEFD0 25F80030 */  addiu $t8, $t7, 48
    /* 800EEFD4 14600002 */  bne $v1, $zero, 0x800EEFE0
    /* 800EEFD8 00000000 */  nop
    /* 800EEFDC 0007000D */  .word 0x0007000D
    /* 800EEFE0 2401FFFF */  addiu $at, $zero, -1
    /* 800EEFE4 14610004 */  bne $v1, $at, 0x800EEFF8
    /* 800EEFE8 3C018000 */  lui $at, 0x8000
    /* 800EEFEC 15C10002 */  bne $t6, $at, 0x800EEFF8
    /* 800EEFF0 00000000 */  nop
    /* 800EEFF4 0006000D */  .word 0x0006000D
    /* 800EEFF8 0203001A */  div $s0, $v1
    /* 800EEFFC 00007012 */  mflo $t6
    /* 800EF000 2419003A */  addiu $t9, $zero, 58
    /* 800EF004 86220000 */  lh $v0, 0($s1)
    /* 800EF008 A3B900CE */  sb $t9, 206($sp)
    /* 800EF00C A3B800CD */  sb $t8, 205($sp)
    /* 800EF010 14600002 */  bne $v1, $zero, 0x800EF01C
    /* 800EF014 00000000 */  nop
    /* 800EF018 0007000D */  .word 0x0007000D
    /* 800EF01C 2401FFFF */  addiu $at, $zero, -1
    /* 800EF020 14610004 */  bne $v1, $at, 0x800EF034
    /* 800EF024 3C018000 */  lui $at, 0x8000
    /* 800EF028 16010002 */  bne $s0, $at, 0x800EF034
    /* 800EF02C 00000000 */  nop
    /* 800EF030 0006000D */  .word 0x0006000D
    /* 800EF034 24010006 */  addiu $at, $zero, 6
    /* 800EF038 01C1001A */  div $t6, $at
    /* 800EF03C 00007810 */  mfhi $t7
    /* 800EF040 25F80030 */  addiu $t8, $t7, 48
    /* 800EF044 A3B800CF */  sb $t8, 207($sp)
    /* 800EF048 0203001A */  div $s0, $v1
    /* 800EF04C 14600002 */  bne $v1, $zero, 0x800EF058
    /* 800EF050 00000000 */  nop
    /* 800EF054 0007000D */  .word 0x0007000D
    /* 800EF058 2401FFFF */  addiu $at, $zero, -1
    /* 800EF05C 14610004 */  bne $v1, $at, 0x800EF070
    /* 800EF060 3C018000 */  lui $at, 0x8000
    /* 800EF064 16010002 */  bne $s0, $at, 0x800EF070
    /* 800EF068 00000000 */  nop
    /* 800EF06C 0006000D */  .word 0x0006000D
    /* 800EF070 0000C810 */  mfhi $t9
    /* 800EF074 272E0030 */  addiu $t6, $t9, 48
    /* 800EF078 A3AE00D0 */  sb $t6, 208($sp)
    /* 800EF07C 1642000D */  bne $s2, $v0, 0x800EF0B4
    /* 800EF080 28410004 */  slti $at, $v0, 4
    /* 800EF084 0C001C9C */  jal 0x80007270
    /* 800EF088 24060001 */  addiu $a2, $zero, 1
    /* 800EF08C 0C02D080 */  jal 0x800B4200
    /* 800EF090 24120009 */  addiu $s2, $zero, 9
    /* 800EF094 3C048014 */  lui $a0, 0x8014
    /* 800EF098 248461D0 */  addiu $a0, $a0, 25040
    /* 800EF09C 00408025 */  or $s0, $v0, $zero
    /* 800EF0A0 00002825 */  or $a1, $zero, $zero
    /* 800EF0A4 0C001D78 */  jal 0x800075E0
    /* 800EF0A8 00003025 */  or $a2, $zero, $zero
    /* 800EF0AC 1000001E */  beq $zero, $zero, 0x800EF128
    /* 800EF0B0 00000000 */  nop
    /* 800EF0B4 10200010 */  beq $at, $zero, 0x800EF0F8
    /* 800EF0B8 3C048014 */  lui $a0, 0x8014
    /* 800EF0BC 3C048014 */  lui $a0, 0x8014
    /* 800EF0C0 248461D0 */  addiu $a0, $a0, 25040
    /* 800EF0C4 00002825 */  or $a1, $zero, $zero
    /* 800EF0C8 0C001C9C */  jal 0x80007270
    /* 800EF0CC 24060001 */  addiu $a2, $zero, 1
    /* 800EF0D0 0C02D080 */  jal 0x800B4200
    /* 800EF0D4 24120008 */  addiu $s2, $zero, 8
    /* 800EF0D8 3C048014 */  lui $a0, 0x8014
    /* 800EF0DC 248461D0 */  addiu $a0, $a0, 25040
    /* 800EF0E0 00408025 */  or $s0, $v0, $zero
    /* 800EF0E4 00002825 */  or $a1, $zero, $zero
    /* 800EF0E8 0C001D78 */  jal 0x800075E0
    /* 800EF0EC 00003025 */  or $a2, $zero, $zero
    /* 800EF0F0 1000000D */  beq $zero, $zero, 0x800EF128
    /* 800EF0F4 00000000 */  nop
    /* 800EF0F8 248461D0 */  addiu $a0, $a0, 25040
    /* 800EF0FC 00002825 */  or $a1, $zero, $zero
    /* 800EF100 0C001C9C */  jal 0x80007270
    /* 800EF104 24060001 */  addiu $a2, $zero, 1
    /* 800EF108 0C02D080 */  jal 0x800B4200
    /* 800EF10C 24120005 */  addiu $s2, $zero, 5
    /* 800EF110 3C048014 */  lui $a0, 0x8014
    /* 800EF114 248461D0 */  addiu $a0, $a0, 25040
    /* 800EF118 00408025 */  or $s0, $v0, $zero
    /* 800EF11C 00002825 */  or $a1, $zero, $zero
    /* 800EF120 0C001D78 */  jal 0x800075E0
    /* 800EF124 00003025 */  or $a2, $zero, $zero
    /* 800EF128 0C02F77F */  jal 0x800BDDFC
    /* 800EF12C 24040038 */  addiu $a0, $zero, 56
    /* 800EF130 24530001 */  addiu $s3, $v0, 1
    /* 800EF134 0C02F77F */  jal 0x800BDDFC
    /* 800EF138 2404003A */  addiu $a0, $zero, 58
    /* 800EF13C 0013C080 */  sll $t8, $s3, 2
    /* 800EF140 0313C023 */  subu $t8, $t8, $s3
    /* 800EF144 244F0001 */  addiu $t7, $v0, 1
    /* 800EF148 0302C821 */  addu $t9, $t8, $v0
    /* 800EF14C AFAF00C0 */  sw $t7, 192($sp)
    /* 800EF150 272E0001 */  addiu $t6, $t9, 1
    /* 800EF154 05C10003 */  bgez $t6, 0x800EF164
    /* 800EF158 000E7843 */  sra $t7, $t6, 1
    /* 800EF15C 25C10001 */  addiu $at, $t6, 1
    /* 800EF160 00017843 */  sra $t7, $at, 1
    /* 800EF164 028FA023 */  subu $s4, $s4, $t7
    /* 800EF168 27B000CC */  addiu $s0, $sp, 204
    /* 800EF16C 92180000 */  lbu $t8, 0($s0)
    /* 800EF170 27B900CD */  addiu $t9, $sp, 205
    /* 800EF174 0219082B */  sltu $at, $s0, $t9
    /* 800EF178 A3A000C9 */  sb $zero, 201($sp)
    /* 800EF17C 10200004 */  beq $at, $zero, 0x800EF190
    /* 800EF180 A3B800C8 */  sb $t8, 200($sp)
    /* 800EF184 330E00FF */  andi $t6, $t8, 0x00FF
    /* 800EF188 24010030 */  addiu $at, $zero, 48
    /* 800EF18C 11C1001A */  beq $t6, $at, 0x800EF1F8
    /* 800EF190 26B10001 */  addiu $s1, $s5, 1
    /* 800EF194 00159400 */  sll $s2, $s5, 16
    /* 800EF198 0012CC03 */  sra $t9, $s2, 16
    /* 800EF19C 00117C00 */  sll $t7, $s1, 16
    /* 800EF1A0 000F8C03 */  sra $s1, $t7, 16
    /* 800EF1A4 03209025 */  or $s2, $t9, $zero
    /* 800EF1A8 0C02DD28 */  jal 0x800B74A0
    /* 800EF1AC 00002025 */  or $a0, $zero, $zero
    /* 800EF1B0 26840001 */  addiu $a0, $s4, 1
    /* 800EF1B4 00112C00 */  sll $a1, $s1, 16
    /* 800EF1B8 0005C403 */  sra $t8, $a1, 16
    /* 800EF1BC 00047400 */  sll $t6, $a0, 16
    /* 800EF1C0 000E2403 */  sra $a0, $t6, 16
    /* 800EF1C4 03002825 */  or $a1, $t8, $zero
    /* 800EF1C8 0C02DC75 */  jal 0x800B71D4
    /* 800EF1CC 27A600C8 */  addiu $a2, $sp, 200
    /* 800EF1D0 0C02DD28 */  jal 0x800B74A0
    /* 800EF1D4 2404000E */  addiu $a0, $zero, 14
    /* 800EF1D8 00142400 */  sll $a0, $s4, 16
    /* 800EF1DC 00122C00 */  sll $a1, $s2, 16
    /* 800EF1E0 00057403 */  sra $t6, $a1, 16
    /* 800EF1E4 0004CC03 */  sra $t9, $a0, 16
    /* 800EF1E8 03202025 */  or $a0, $t9, $zero
    /* 800EF1EC 01C02825 */  or $a1, $t6, $zero
    /* 800EF1F0 0C02DC75 */  jal 0x800B71D4
    /* 800EF1F4 27A600C8 */  addiu $a2, $sp, 200
    /* 800EF1F8 27AF00CD */  addiu $t7, $sp, 205
    /* 800EF1FC 120F0003 */  beq $s0, $t7, 0x800EF20C
    /* 800EF200 27B800CE */  addiu $t8, $sp, 206
    /* 800EF204 5618000A */  .word 0x5618000A
    /* 800EF208 0293A021 */  addu $s4, $s4, $s3
    /* 800EF20C 8FB900C0 */  lw $t9, 192($sp)
    /* 800EF210 02797021 */  addu $t6, $s3, $t9
    /* 800EF214 05C10003 */  bgez $t6, 0x800EF224
    /* 800EF218 000E7843 */  sra $t7, $t6, 1
    /* 800EF21C 25C10001 */  addiu $at, $t6, 1
    /* 800EF220 00017843 */  sra $t7, $at, 1
    /* 800EF224 10000002 */  beq $zero, $zero, 0x800EF230
    /* 800EF228 028FA021 */  addu $s4, $s4, $t7
    /* 800EF22C 0293A021 */  addu $s4, $s4, $s3
    /* 800EF230 26100001 */  addiu $s0, $s0, 1
    /* 800EF234 27B800D1 */  addiu $t8, $sp, 209
    /* 800EF238 5618FFCD */  .word 0x5618FFCD
    /* 800EF23C 92180000 */  lbu $t8, 0($s0)
    /* 800EF240 3C018012 */  lui $at, 0x8012
    /* 800EF244 24190003 */  addiu $t9, $zero, 3
    /* 800EF248 AC398E24 */  sw $t9, -29148($at)
    /* 800EF24C AC208E20 */  sw $zero, -29152($at)
    /* 800EF250 3C01BF80 */  lui $at, 0xBF80
    /* 800EF254 44816000 */  .word 0x44816000
    /* 800EF258 0C02D96D */  jal 0x800B65B4
    /* 800EF25C 00000000 */  nop
    /* 800EF260 24020001 */  addiu $v0, $zero, 1
    /* 800EF264 8FBF002C */  lw $ra, 44($sp)
    /* 800EF268 8FB00014 */  lw $s0, 20($sp)
    /* 800EF26C 8FB10018 */  lw $s1, 24($sp)
    /* 800EF270 8FB2001C */  lw $s2, 28($sp)
    /* 800EF274 8FB30020 */  lw $s3, 32($sp)
    /* 800EF278 8FB40024 */  lw $s4, 36($sp)
    /* 800EF27C 8FB50028 */  lw $s5, 40($sp)
    /* 800EF280 03E00008 */  jr $ra
    /* 800EF284 27BD00D8 */  addiu $sp, $sp, 216
