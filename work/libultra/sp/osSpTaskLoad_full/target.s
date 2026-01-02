# Source: E9A0.s
# Address: 0x8000DF80

glabel func_8000DF80
    /* EB80 8000DF80 27BDFF88 */  addiu      $sp, $sp, -0x78
    /* EB84 8000DF84 AFB3002C */  sw         $s3, 0x2C($sp)
    /* EB88 8000DF88 AFBE0040 */  sw         $fp, 0x40($sp)
    /* EB8C 8000DF8C AFB7003C */  sw         $s7, 0x3C($sp)
    /* EB90 8000DF90 AFB60038 */  sw         $s6, 0x38($sp)
    /* EB94 8000DF94 AFB50034 */  sw         $s5, 0x34($sp)
    /* EB98 8000DF98 AFB40030 */  sw         $s4, 0x30($sp)
    /* EB9C 8000DF9C 3C130500 */  lui        $s3, (0x5000510 >> 16)
    /* EBA0 8000DFA0 0080A025 */  or         $s4, $a0, $zero
    /* EBA4 8000DFA4 AFBF0044 */  sw         $ra, 0x44($sp)
    /* EBA8 8000DFA8 AFB20028 */  sw         $s2, 0x28($sp)
    /* EBAC 8000DFAC AFB10024 */  sw         $s1, 0x24($sp)
    /* EBB0 8000DFB0 AFB00020 */  sw         $s0, 0x20($sp)
    /* EBB4 8000DFB4 AFA00074 */  sw         $zero, 0x74($sp)
    /* EBB8 8000DFB8 36730510 */  ori        $s3, $s3, (0x5000510 & 0xFFFF)
    /* EBBC 8000DFBC 24150024 */  addiu      $s5, $zero, 0x24
    /* EBC0 8000DFC0 27B60070 */  addiu      $s6, $sp, 0x70
    /* EBC4 8000DFC4 24170001 */  addiu      $s7, $zero, 0x1
    /* EBC8 8000DFC8 241E001D */  addiu      $fp, $zero, 0x1D
  .L8000DFCC:
    /* EBCC 8000DFCC 8E840008 */  lw         $a0, 0x8($s4)
  .L8000DFD0:
    /* EBD0 8000DFD0 27A50074 */  addiu      $a1, $sp, 0x74
    /* EBD4 8000DFD4 0C001C9C */  jal        func_80007270
    /* EBD8 8000DFD8 02E03025 */   or        $a2, $s7, $zero
    /* EBDC 8000DFDC 8FAE0074 */  lw         $t6, 0x74($sp)
    /* EBE0 8000DFE0 8FA80074 */  lw         $t0, 0x74($sp)
    /* EBE4 8000DFE4 8DC40014 */  lw         $a0, 0x14($t6)
    /* EBE8 8000DFE8 50800081 */  beql       $a0, $zero, .L8000E1F0
    /* EBEC 8000DFEC 95090000 */   lhu       $t1, 0x0($t0)
    /* EBF0 8000DFF0 908F0004 */  lbu        $t7, 0x4($a0)
    /* EBF4 8000DFF4 24010002 */  addiu      $at, $zero, 0x2
    /* EBF8 8000DFF8 55E1007D */  bnel       $t7, $at, .L8000E1F0
    /* EBFC 8000DFFC 95090000 */   lhu       $t1, 0x0($t0)
    /* EC00 8000E000 8C820014 */  lw         $v0, 0x14($a0)
    /* EC04 8000E004 24010001 */  addiu      $at, $zero, 0x1
    /* EC08 8000E008 24900014 */  addiu      $s0, $a0, 0x14
    /* EC0C 8000E00C 10400003 */  beqz       $v0, .L8000E01C
    /* EC10 8000E010 27A5006C */   addiu     $a1, $sp, 0x6C
    /* EC14 8000E014 54410076 */  bnel       $v0, $at, .L8000E1F0
    /* EC18 8000E018 95090000 */   lhu       $t1, 0x0($t0)
  .L8000E01C:
    /* EC1C 8000E01C 96180006 */  lhu        $t8, 0x6($s0)
    /* EC20 8000E020 96020004 */  lhu        $v0, 0x4($s0)
    /* EC24 8000E024 2408FFFF */  addiu      $t0, $zero, -0x1
    /* EC28 8000E028 03150019 */  multu      $t8, $s5
    /* EC2C 8000E02C 24010003 */  addiu      $at, $zero, 0x3
    /* EC30 8000E030 AE080008 */  sw         $t0, 0x8($s0)
    /* EC34 8000E034 00009025 */  or         $s2, $zero, $zero
    /* EC38 8000E038 02E03025 */  or         $a2, $s7, $zero
    /* EC3C 8000E03C 0000C812 */  mflo       $t9
    /* EC40 8000E040 02198821 */  addu       $s1, $s0, $t9
    /* EC44 8000E044 10410006 */  beq        $v0, $at, .L8000E060
    /* EC48 8000E048 26310018 */   addiu     $s1, $s1, 0x18
    /* EC4C 8000E04C 8E290004 */  lw         $t1, 0x4($s1)
    /* EC50 8000E050 8E2A000C */  lw         $t2, 0xC($s1)
    /* EC54 8000E054 012A5823 */  subu       $t3, $t1, $t2
    /* EC58 8000E058 AE2B0004 */  sw         $t3, 0x4($s1)
    /* EC5C 8000E05C 96020004 */  lhu        $v0, 0x4($s0)
  .L8000E060:
    /* EC60 8000E060 24010002 */  addiu      $at, $zero, 0x2
    /* EC64 8000E064 14410007 */  bne        $v0, $at, .L8000E084
    /* EC68 8000E068 8FAC0074 */   lw        $t4, 0x74($sp)
    /* EC6C 8000E06C 8D8D0014 */  lw         $t5, 0x14($t4)
    /* EC70 8000E070 8DAE0014 */  lw         $t6, 0x14($t5)
    /* EC74 8000E074 15C00003 */  bnez       $t6, .L8000E084
    /* EC78 8000E078 00000000 */   nop
    /* EC7C 8000E07C 10000001 */  b          .L8000E084
    /* EC80 8000E080 02E09025 */   or        $s2, $s7, $zero
  .L8000E084:
    /* EC84 8000E084 0C001C9C */  jal        func_80007270
    /* EC88 8000E088 8E840010 */   lw        $a0, 0x10($s4)
    /* EC8C 8000E08C 3C040010 */  lui        $a0, (0x100401 >> 16)
    /* EC90 8000E090 0C003F68 */  jal        func_8000FDA0
    /* EC94 8000E094 34840401 */   ori       $a0, $a0, (0x100401 & 0xFFFF)
    /* EC98 8000E098 8E060010 */  lw         $a2, 0x10($s0)
    /* EC9C 8000E09C 8FAF0074 */  lw         $t7, 0x74($sp)
    /* ECA0 8000E0A0 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* ECA4 8000E0A4 00C1C025 */  or         $t8, $a2, $at
    /* ECA8 8000E0A8 03003025 */  or         $a2, $t8, $zero
    /* ECAC 8000E0AC 02602825 */  or         $a1, $s3, $zero
    /* ECB0 8000E0B0 0C003F80 */  jal        func_8000FE00
    /* ECB4 8000E0B4 8DE40014 */   lw        $a0, 0x14($t7)
  .L8000E0B8:
    /* ECB8 8000E0B8 8E84000C */  lw         $a0, 0xC($s4)
    /* ECBC 8000E0BC 02C02825 */  or         $a1, $s6, $zero
    /* ECC0 8000E0C0 0C001C9C */  jal        func_80007270
    /* ECC4 8000E0C4 02E03025 */   or        $a2, $s7, $zero
    /* ECC8 8000E0C8 8FB90074 */  lw         $t9, 0x74($sp)
    /* ECCC 8000E0CC 8F240014 */  lw         $a0, 0x14($t9)
    /* ECD0 8000E0D0 24900014 */  addiu      $s0, $a0, 0x14
    /* ECD4 8000E0D4 96080006 */  lhu        $t0, 0x6($s0)
    /* ECD8 8000E0D8 01150019 */  multu      $t0, $s5
    /* ECDC 8000E0DC 00004812 */  mflo       $t1
    /* ECE0 8000E0E0 02098821 */  addu       $s1, $s0, $t1
    /* ECE4 8000E0E4 8E2A0018 */  lw         $t2, 0x18($s1)
    /* ECE8 8000E0E8 26310018 */  addiu      $s1, $s1, 0x18
    /* ECEC 8000E0EC 57CA0027 */  bnel       $fp, $t2, .L8000E18C
    /* ECF0 8000E0F0 8FA50074 */   lw        $a1, 0x74($sp)
    /* ECF4 8000E0F4 8E060010 */  lw         $a2, 0x10($s0)
    /* ECF8 8000E0F8 3C011000 */  lui        $at, (0x10000000 >> 16)
    /* ECFC 8000E0FC 02602825 */  or         $a1, $s3, $zero
    /* ED00 8000E100 00C15825 */  or         $t3, $a2, $at
    /* ED04 8000E104 0C003F80 */  jal        func_8000FE00
    /* ED08 8000E108 01603025 */   or        $a2, $t3, $zero
    /* ED0C 8000E10C 8FAC0074 */  lw         $t4, 0x74($sp)
    /* ED10 8000E110 02602825 */  or         $a1, $s3, $zero
    /* ED14 8000E114 8E060010 */  lw         $a2, 0x10($s0)
    /* ED18 8000E118 0C003F80 */  jal        func_8000FE00
    /* ED1C 8000E11C 8D840014 */   lw        $a0, 0x14($t4)
    /* ED20 8000E120 8FAD0074 */  lw         $t5, 0x74($sp)
    /* ED24 8000E124 3C050500 */  lui        $a1, (0x5000508 >> 16)
    /* ED28 8000E128 34A50508 */  ori        $a1, $a1, (0x5000508 & 0xFFFF)
    /* ED2C 8000E12C 27A60054 */  addiu      $a2, $sp, 0x54
    /* ED30 8000E130 0C003FD8 */  jal        func_8000FF60
    /* ED34 8000E134 8DA40014 */   lw        $a0, 0x14($t5)
    /* ED38 8000E138 8FAE0054 */  lw         $t6, 0x54($sp)
    /* ED3C 8000E13C 8FB80074 */  lw         $t8, 0x74($sp)
    /* ED40 8000E140 02602825 */  or         $a1, $s3, $zero
    /* ED44 8000E144 000E7980 */  sll        $t7, $t6, 6
    /* ED48 8000E148 05E30008 */  bgezl      $t7, .L8000E16C
    /* ED4C 8000E14C 24080004 */   addiu     $t0, $zero, 0x4
    /* ED50 8000E150 8E060010 */  lw         $a2, 0x10($s0)
    /* ED54 8000E154 3C010100 */  lui        $at, (0x1000000 >> 16)
    /* ED58 8000E158 8F040014 */  lw         $a0, 0x14($t8)
    /* ED5C 8000E15C 00C1C825 */  or         $t9, $a2, $at
    /* ED60 8000E160 0C003F80 */  jal        func_8000FE00
    /* ED64 8000E164 03203025 */   or        $a2, $t9, $zero
    /* ED68 8000E168 24080004 */  addiu      $t0, $zero, 0x4
  .L8000E16C:
    /* ED6C 8000E16C AE280000 */  sw         $t0, 0x0($s1)
    /* ED70 8000E170 24090002 */  addiu      $t1, $zero, 0x2
    /* ED74 8000E174 3C0AA460 */  lui        $t2, %hi(PI_STATUS_REG)
    /* ED78 8000E178 3C040010 */  lui        $a0, (0x100C01 >> 16)
    /* ED7C 8000E17C AD490010 */  sw         $t1, %lo(PI_STATUS_REG)($t2)
    /* ED80 8000E180 0C004030 */  jal        func_800100C0
    /* ED84 8000E184 34840C01 */   ori       $a0, $a0, (0x100C01 & 0xFFFF)
    /* ED88 8000E188 8FA50074 */  lw         $a1, 0x74($sp)
  .L8000E18C:
    /* ED8C 8000E18C 00003025 */  or         $a2, $zero, $zero
    /* ED90 8000E190 0C001D78 */  jal        func_800075E0
    /* ED94 8000E194 8CA40004 */   lw        $a0, 0x4($a1)
    /* ED98 8000E198 16570007 */  bne        $s2, $s7, .L8000E1B8
    /* ED9C 8000E19C 8FAC0074 */   lw        $t4, 0x74($sp)
    /* EDA0 8000E1A0 8D8D0014 */  lw         $t5, 0x14($t4)
    /* EDA4 8000E1A4 8DAE002C */  lw         $t6, 0x2C($t5)
    /* EDA8 8000E1A8 55C00004 */  bnel       $t6, $zero, .L8000E1BC
    /* EDAC 8000E1AC 8E840010 */   lw        $a0, 0x10($s4)
    /* EDB0 8000E1B0 1000FFC1 */  b          .L8000E0B8
    /* EDB4 8000E1B4 00009025 */   or        $s2, $zero, $zero
  .L8000E1B8:
    /* EDB8 8000E1B8 8E840010 */  lw         $a0, 0x10($s4)
  .L8000E1BC:
    /* EDBC 8000E1BC 00002825 */  or         $a1, $zero, $zero
    /* EDC0 8000E1C0 0C001D78 */  jal        func_800075E0
    /* EDC4 8000E1C4 00003025 */   or        $a2, $zero, $zero
    /* EDC8 8000E1C8 8FAF0074 */  lw         $t7, 0x74($sp)
    /* EDCC 8000E1CC 8DF80014 */  lw         $t8, 0x14($t7)
    /* EDD0 8000E1D0 9719001A */  lhu        $t9, 0x1A($t8)
    /* EDD4 8000E1D4 56F9FF7E */  bnel       $s7, $t9, .L8000DFD0
    /* EDD8 8000E1D8 8E840008 */   lw        $a0, 0x8($s4)
    /* EDDC 8000E1DC 0C004044 */  jal        func_80010110
    /* EDE0 8000E1E0 00000000 */   nop
    /* EDE4 8000E1E4 1000FF7A */  b          .L8000DFD0
    /* EDE8 8000E1E8 8E840008 */   lw        $a0, 0x8($s4)
    /* EDEC 8000E1EC 95090000 */  lhu        $t1, 0x0($t0)
  .L8000E1F0:
    /* EDF0 8000E1F0 252AFFF6 */  addiu      $t2, $t1, -0xA
    /* EDF4 8000E1F4 2D410007 */  sltiu      $at, $t2, 0x7
    /* EDF8 8000E1F8 10200047 */  beqz       $at, .L8000E318
    /* EDFC 8000E1FC 000A5080 */   sll       $t2, $t2, 2
    /* EE00 8000E200 3C018003 */  lui        $at, %hi(jtbl_8002D860_main)
    /* EE04 8000E204 002A0821 */  addu       $at, $at, $t2
    /* EE08 8000E208 8C2AD860 */  lw         $t2, %lo(jtbl_8002D860_main)($at)
    /* EE0C 8000E20C 01400008 */  jr         $t2
    /* EE10 8000E210 00000000 */   nop
    /* EE14 8000E214 8E840010 */  lw         $a0, 0x10($s4)
    /* EE18 8000E218 27A5006C */  addiu      $a1, $sp, 0x6C
    /* EE1C 8000E21C 0C001C9C */  jal        func_80007270
    /* EE20 8000E220 02E03025 */   or        $a2, $s7, $zero
    /* EE24 8000E224 8FAB0074 */  lw         $t3, 0x74($sp)
    /* EE28 8000E228 8E990014 */  lw         $t9, 0x14($s4)
    /* EE2C 8000E22C 00002025 */  or         $a0, $zero, $zero
    /* EE30 8000E230 8D65000C */  lw         $a1, 0xC($t3)
    /* EE34 8000E234 8D660008 */  lw         $a2, 0x8($t3)
    /* EE38 8000E238 0320F809 */  jalr       $t9
    /* EE3C 8000E23C 8D670010 */   lw        $a3, 0x10($t3)
    /* EE40 8000E240 10000036 */  b          .L8000E31C
    /* EE44 8000E244 00408025 */   or        $s0, $v0, $zero
    /* EE48 8000E248 8E840010 */  lw         $a0, 0x10($s4)
    /* EE4C 8000E24C 27A5006C */  addiu      $a1, $sp, 0x6C
    /* EE50 8000E250 0C001C9C */  jal        func_80007270
    /* EE54 8000E254 02E03025 */   or        $a2, $s7, $zero
    /* EE58 8000E258 8FAC0074 */  lw         $t4, 0x74($sp)
    /* EE5C 8000E25C 8E990014 */  lw         $t9, 0x14($s4)
    /* EE60 8000E260 02E02025 */  or         $a0, $s7, $zero
    /* EE64 8000E264 8D85000C */  lw         $a1, 0xC($t4)
    /* EE68 8000E268 8D860008 */  lw         $a2, 0x8($t4)
    /* EE6C 8000E26C 0320F809 */  jalr       $t9
    /* EE70 8000E270 8D870010 */   lw        $a3, 0x10($t4)
    /* EE74 8000E274 10000029 */  b          .L8000E31C
    /* EE78 8000E278 00408025 */   or        $s0, $v0, $zero
    /* EE7C 8000E27C 8E840010 */  lw         $a0, 0x10($s4)
    /* EE80 8000E280 27A5006C */  addiu      $a1, $sp, 0x6C
    /* EE84 8000E284 0C001C9C */  jal        func_80007270
    /* EE88 8000E288 02E03025 */   or        $a2, $s7, $zero
    /* EE8C 8000E28C 8FAD0074 */  lw         $t5, 0x74($sp)
    /* EE90 8000E290 00002825 */  or         $a1, $zero, $zero
    /* EE94 8000E294 8DAE0010 */  lw         $t6, 0x10($t5)
    /* EE98 8000E298 8DA40014 */  lw         $a0, 0x14($t5)
    /* EE9C 8000E29C 8DA6000C */  lw         $a2, 0xC($t5)
    /* EEA0 8000E2A0 8DA70008 */  lw         $a3, 0x8($t5)
    /* EEA4 8000E2A4 AFAE0010 */  sw         $t6, 0x10($sp)
    /* EEA8 8000E2A8 8E990018 */  lw         $t9, 0x18($s4)
    /* EEAC 8000E2AC 0320F809 */  jalr       $t9
    /* EEB0 8000E2B0 00000000 */   nop
    /* EEB4 8000E2B4 10000019 */  b          .L8000E31C
    /* EEB8 8000E2B8 00408025 */   or        $s0, $v0, $zero
    /* EEBC 8000E2BC 8E840010 */  lw         $a0, 0x10($s4)
    /* EEC0 8000E2C0 27A5006C */  addiu      $a1, $sp, 0x6C
    /* EEC4 8000E2C4 0C001C9C */  jal        func_80007270
    /* EEC8 8000E2C8 02E03025 */   or        $a2, $s7, $zero
    /* EECC 8000E2CC 8FAF0074 */  lw         $t7, 0x74($sp)
    /* EED0 8000E2D0 02E02825 */  or         $a1, $s7, $zero
    /* EED4 8000E2D4 8DF80010 */  lw         $t8, 0x10($t7)
    /* EED8 8000E2D8 8DE40014 */  lw         $a0, 0x14($t7)
    /* EEDC 8000E2DC 8DE6000C */  lw         $a2, 0xC($t7)
    /* EEE0 8000E2E0 8DE70008 */  lw         $a3, 0x8($t7)
    /* EEE4 8000E2E4 AFB80010 */  sw         $t8, 0x10($sp)
    /* EEE8 8000E2E8 8E990018 */  lw         $t9, 0x18($s4)
    /* EEEC 8000E2EC 0320F809 */  jalr       $t9
    /* EEF0 8000E2F0 00000000 */   nop
    /* EEF4 8000E2F4 10000009 */  b          .L8000E31C
    /* EEF8 8000E2F8 00408025 */   or        $s0, $v0, $zero
    /* EEFC 8000E2FC 8FA50074 */  lw         $a1, 0x74($sp)
    /* EF00 8000E300 00003025 */  or         $a2, $zero, $zero
    /* EF04 8000E304 2410FFFF */  addiu      $s0, $zero, -0x1
    /* EF08 8000E308 0C001D78 */  jal        func_800075E0
    /* EF0C 8000E30C 8CA40004 */   lw        $a0, 0x4($a1)
    /* EF10 8000E310 10000002 */  b          .L8000E31C
    /* EF14 8000E314 00000000 */   nop
  .L8000E318:
    /* EF18 8000E318 2410FFFF */  addiu      $s0, $zero, -0x1
  .L8000E31C:
    /* EF1C 8000E31C 1600FF2B */  bnez       $s0, .L8000DFCC
    /* EF20 8000E320 02C02825 */   or        $a1, $s6, $zero
    /* EF24 8000E324 8E84000C */  lw         $a0, 0xC($s4)
    /* EF28 8000E328 0C001C9C */  jal        func_80007270
    /* EF2C 8000E32C 02E03025 */   or        $a2, $s7, $zero
    /* EF30 8000E330 8FA50074 */  lw         $a1, 0x74($sp)
    /* EF34 8000E334 00003025 */  or         $a2, $zero, $zero
    /* EF38 8000E338 0C001D78 */  jal        func_800075E0
    /* EF3C 8000E33C 8CA40004 */   lw        $a0, 0x4($a1)
    /* EF40 8000E340 8E840010 */  lw         $a0, 0x10($s4)
    /* EF44 8000E344 00002825 */  or         $a1, $zero, $zero
    /* EF48 8000E348 0C001D78 */  jal        func_800075E0
    /* EF4C 8000E34C 00003025 */   or        $a2, $zero, $zero
    /* EF50 8000E350 1000FF1F */  b          .L8000DFD0
    /* EF54 8000E354 8E840008 */   lw        $a0, 0x8($s4)
    /* EF58 8000E358 00000000 */  nop
    /* EF5C 8000E35C 00000000 */  nop
    /* EF60 8000E360 8FBF0044 */  lw         $ra, 0x44($sp)
    /* EF64 8000E364 8FB00020 */  lw         $s0, 0x20($sp)
    /* EF68 8000E368 8FB10024 */  lw         $s1, 0x24($sp)
    /* EF6C 8000E36C 8FB20028 */  lw         $s2, 0x28($sp)
    /* EF70 8000E370 8FB3002C */  lw         $s3, 0x2C($sp)
    /* EF74 8000E374 8FB40030 */  lw         $s4, 0x30($sp)
    /* EF78 8000E378 8FB50034 */  lw         $s5, 0x34($sp)
    /* EF7C 8000E37C 8FB60038 */  lw         $s6, 0x38($sp)
    /* EF80 8000E380 8FB7003C */  lw         $s7, 0x3C($sp)
    /* EF84 8000E384 8FBE0040 */  lw         $fp, 0x40($sp)
    /* EF88 8000E388 03E00008 */  jr         $ra
    /* EF8C 8000E38C 27BD0078 */   addiu     $sp, $sp, 0x78