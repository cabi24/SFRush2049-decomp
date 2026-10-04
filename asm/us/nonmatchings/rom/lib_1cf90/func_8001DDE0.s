nonmatching func_8001DDE0, 0x2E0

glabel func_8001DDE0
    /* 1E9E0 8001DDE0 27BDFF90 */  addiu      $sp, $sp, -0x70
    /* 1E9E4 8001DDE4 AFBF0044 */  sw         $ra, 0x44($sp)
    /* 1E9E8 8001DDE8 AFB60040 */  sw         $s6, 0x40($sp)
    /* 1E9EC 8001DDEC AFB5003C */  sw         $s5, 0x3C($sp)
    /* 1E9F0 8001DDF0 AFB40038 */  sw         $s4, 0x38($sp)
    /* 1E9F4 8001DDF4 AFB30034 */  sw         $s3, 0x34($sp)
    /* 1E9F8 8001DDF8 AFB20030 */  sw         $s2, 0x30($sp)
    /* 1E9FC 8001DDFC AFB1002C */  sw         $s1, 0x2C($sp)
    /* 1EA00 8001DE00 AFB00028 */  sw         $s0, 0x28($sp)
    /* 1EA04 8001DE04 0C00764A */  jal        func_8001D928
    /* 1EA08 8001DE08 F7B40020 */   sdc1      $fs0, 0x20($sp)
    /* 1EA0C 8001DE0C 3C108005 */  lui        $s0, %hi(D_8004FD50)
    /* 1EA10 8001DE10 8E10FD50 */  lw         $s0, %lo(D_8004FD50)($s0)
    /* 1EA14 8001DE14 3C16FFFD */  lui        $s6, (0xFFFDFFFF >> 16)
    /* 1EA18 8001DE18 36D6FFFF */  ori        $s6, $s6, (0xFFFDFFFF & 0xFFFF)
    /* 1EA1C 8001DE1C 1200009B */  beqz       $s0, .L8001E08C
    /* 1EA20 8001DE20 3C150002 */   lui       $s5, (0x20000 >> 16)
    /* 1EA24 8001DE24 4480A000 */  mtc1       $zero, $fs0
    /* 1EA28 8001DE28 3C140008 */  lui        $s4, (0x80000 >> 16)
    /* 1EA2C 8001DE2C 2412FFFF */  addiu      $s2, $zero, -0x1
    /* 1EA30 8001DE30 3C110004 */  lui        $s1, (0x40000 >> 16)
  .L8001DE34:
    /* 1EA34 8001DE34 8E030008 */  lw         $v1, 0x8($s0)
    /* 1EA38 8001DE38 3C010002 */  lui        $at, (0x20001 >> 16)
    /* 1EA3C 8001DE3C 8E130000 */  lw         $s3, 0x0($s0)
    /* 1EA40 8001DE40 00717024 */  and        $t6, $v1, $s1
    /* 1EA44 8001DE44 11C00005 */  beqz       $t6, .L8001DE5C
    /* 1EA48 8001DE48 34210001 */   ori       $at, $at, (0x20001 & 0xFFFF)
    /* 1EA4C 8001DE4C 0C007421 */  jal        func_8001D084
    /* 1EA50 8001DE50 02002025 */   or        $a0, $s0, $zero
    /* 1EA54 8001DE54 1000008B */  b          .L8001E084
    /* 1EA58 8001DE58 00000000 */   nop
  .L8001DE5C:
    /* 1EA5C 8001DE5C 00617824 */  and        $t7, $v1, $at
    /* 1EA60 8001DE60 11E0000A */  beqz       $t7, .L8001DE8C
    /* 1EA64 8001DE64 02002025 */   or        $a0, $s0, $zero
    /* 1EA68 8001DE68 27B8005C */  addiu      $t8, $sp, 0x5C
    /* 1EA6C 8001DE6C 27B90058 */  addiu      $t9, $sp, 0x58
    /* 1EA70 8001DE70 AFB90014 */  sw         $t9, 0x14($sp)
    /* 1EA74 8001DE74 AFB80010 */  sw         $t8, 0x10($sp)
    /* 1EA78 8001DE78 27A50064 */  addiu      $a1, $sp, 0x64
    /* 1EA7C 8001DE7C 27A60054 */  addiu      $a2, $sp, 0x54
    /* 1EA80 8001DE80 0C007218 */  jal        func_8001C860
    /* 1EA84 8001DE84 27A70060 */   addiu     $a3, $sp, 0x60
    /* 1EA88 8001DE88 8E030008 */  lw         $v1, 0x8($s0)
  .L8001DE8C:
    /* 1EA8C 8001DE8C 00744024 */  and        $t0, $v1, $s4
    /* 1EA90 8001DE90 15000074 */  bnez       $t0, .L8001E064
    /* 1EA94 8001DE94 C7B20064 */   lwc1      $ft5, 0x64($sp)
    /* 1EA98 8001DE98 00754824 */  and        $t1, $v1, $s5
    /* 1EA9C 8001DE9C 11200028 */  beqz       $t1, .L8001DF40
    /* 1EAA0 8001DEA0 C7A40064 */   lwc1      $ft0, 0x64($sp)
    /* 1EAA4 8001DEA4 46142032 */  c.eq.s     $ft0, $fs0
    /* 1EAA8 8001DEA8 306A0004 */  andi       $t2, $v1, 0x4
    /* 1EAAC 8001DEAC 306E0001 */  andi       $t6, $v1, 0x1
    /* 1EAB0 8001DEB0 45000007 */  bc1f       .L8001DED0
    /* 1EAB4 8001DEB4 00000000 */   nop
    /* 1EAB8 8001DEB8 11400005 */  beqz       $t2, .L8001DED0
    /* 1EABC 8001DEBC 00745825 */   or        $t3, $v1, $s4
    /* 1EAC0 8001DEC0 AE0B0008 */  sw         $t3, 0x8($s0)
    /* 1EAC4 8001DEC4 01766824 */  and        $t5, $t3, $s6
    /* 1EAC8 8001DEC8 10000029 */  b          .L8001DF70
    /* 1EACC 8001DECC AE0D0008 */   sw        $t5, 0x8($s0)
  .L8001DED0:
    /* 1EAD0 8001DED0 11C0000E */  beqz       $t6, .L8001DF0C
    /* 1EAD4 8001DED4 2405007F */   addiu     $a1, $zero, 0x7F
    /* 1EAD8 8001DED8 C7A60058 */  lwc1       $ft1, 0x58($sp)
    /* 1EADC 8001DEDC C7A80054 */  lwc1       $ft2, 0x54($sp)
    /* 1EAE0 8001DEE0 02002025 */  or         $a0, $s0, $zero
    /* 1EAE4 8001DEE4 8FA50064 */  lw         $a1, 0x64($sp)
    /* 1EAE8 8001DEE8 8FA60060 */  lw         $a2, 0x60($sp)
    /* 1EAEC 8001DEEC 8FA7005C */  lw         $a3, 0x5C($sp)
    /* 1EAF0 8001DEF0 E7A60010 */  swc1       $ft1, 0x10($sp)
    /* 1EAF4 8001DEF4 0C00769D */  jal        func_8001DA74
    /* 1EAF8 8001DEF8 E7A80014 */   swc1      $ft2, 0x14($sp)
    /* 1EAFC 8001DEFC 5040001D */  beql       $v0, $zero, .L8001DF74
    /* 1EB00 8001DF00 8E0C0034 */   lw        $t4, 0x34($s0)
    /* 1EB04 8001DF04 1000005F */  b          .L8001E084
    /* 1EB08 8001DF08 00000000 */   nop
  .L8001DF0C:
    /* 1EB0C 8001DF0C 9604003C */  lhu        $a0, 0x3C($s0)
    /* 1EB10 8001DF10 0C006C74 */  jal        func_8001B1D0
    /* 1EB14 8001DF14 24060040 */   addiu     $a2, $zero, 0x40
    /* 1EB18 8001DF18 14520015 */  bne        $v0, $s2, .L8001DF70
    /* 1EB1C 8001DF1C AE020034 */   sw        $v0, 0x34($s0)
    /* 1EB20 8001DF20 8E030008 */  lw         $v1, 0x8($s0)
    /* 1EB24 8001DF24 306F0002 */  andi       $t7, $v1, 0x2
    /* 1EB28 8001DF28 15E00056 */  bnez       $t7, .L8001E084
    /* 1EB2C 8001DF2C 0071C025 */   or        $t8, $v1, $s1
    /* 1EB30 8001DF30 AE180008 */  sw         $t8, 0x8($s0)
    /* 1EB34 8001DF34 03164024 */  and        $t0, $t8, $s6
    /* 1EB38 8001DF38 1000000D */  b          .L8001DF70
    /* 1EB3C 8001DF3C AE080008 */   sw        $t0, 0x8($s0)
  .L8001DF40:
    /* 1EB40 8001DF40 0C008074 */  jal        func_800201D0
    /* 1EB44 8001DF44 8E040034 */   lw        $a0, 0x34($s0)
    /* 1EB48 8001DF48 14520009 */  bne        $v0, $s2, .L8001DF70
    /* 1EB4C 8001DF4C AE020034 */   sw        $v0, 0x34($s0)
    /* 1EB50 8001DF50 8E030008 */  lw         $v1, 0x8($s0)
    /* 1EB54 8001DF54 30690002 */  andi       $t1, $v1, 0x2
    /* 1EB58 8001DF58 11200004 */  beqz       $t1, .L8001DF6C
    /* 1EB5C 8001DF5C 00715825 */   or        $t3, $v1, $s1
    /* 1EB60 8001DF60 00755025 */  or         $t2, $v1, $s5
    /* 1EB64 8001DF64 10000002 */  b          .L8001DF70
    /* 1EB68 8001DF68 AE0A0008 */   sw        $t2, 0x8($s0)
  .L8001DF6C:
    /* 1EB6C 8001DF6C AE0B0008 */  sw         $t3, 0x8($s0)
  .L8001DF70:
    /* 1EB70 8001DF70 8E0C0034 */  lw         $t4, 0x34($s0)
  .L8001DF74:
    /* 1EB74 8001DF74 524C0026 */  beql       $s2, $t4, .L8001E010
    /* 1EB78 8001DF78 8E0A0008 */   lw        $t2, 0x8($s0)
    /* 1EB7C 8001DF7C 8E0D0008 */  lw         $t5, 0x8($s0)
    /* 1EB80 8001DF80 02002025 */  or         $a0, $s0, $zero
    /* 1EB84 8001DF84 31AE0001 */  andi       $t6, $t5, 0x1
    /* 1EB88 8001DF88 51C00004 */  beql       $t6, $zero, .L8001DF9C
    /* 1EB8C 8001DF8C C7AA0064 */   lwc1      $ft3, 0x64($sp)
    /* 1EB90 8001DF90 0C007651 */  jal        func_8001D944
    /* 1EB94 8001DF94 8FA50064 */   lw        $a1, 0x64($sp)
    /* 1EB98 8001DF98 C7AA0064 */  lwc1       $ft3, 0x64($sp)
  .L8001DF9C:
    /* 1EB9C 8001DF9C 02002025 */  or         $a0, $s0, $zero
    /* 1EBA0 8001DFA0 8FA50064 */  lw         $a1, 0x64($sp)
    /* 1EBA4 8001DFA4 46145032 */  c.eq.s     $ft3, $fs0
    /* 1EBA8 8001DFA8 8FA60060 */  lw         $a2, 0x60($sp)
    /* 1EBAC 8001DFAC C7B00058 */  lwc1       $ft4, 0x58($sp)
    /* 1EBB0 8001DFB0 45020012 */  bc1fl      .L8001DFFC
    /* 1EBB4 8001DFB4 C7B20054 */   lwc1      $ft5, 0x54($sp)
    /* 1EBB8 8001DFB8 8E0F0008 */  lw         $t7, 0x8($s0)
    /* 1EBBC 8001DFBC 31F80004 */  andi       $t8, $t7, 0x4
    /* 1EBC0 8001DFC0 5300000E */  beql       $t8, $zero, .L8001DFFC
    /* 1EBC4 8001DFC4 C7B20054 */   lwc1      $ft5, 0x54($sp)
    /* 1EBC8 8001DFC8 0C006E31 */  jal        func_8001B8C4
    /* 1EBCC 8001DFCC 8E040034 */   lw        $a0, 0x34($s0)
    /* 1EBD0 8001DFD0 8E030008 */  lw         $v1, 0x8($s0)
    /* 1EBD4 8001DFD4 AE120034 */  sw         $s2, 0x34($s0)
    /* 1EBD8 8001DFD8 30790002 */  andi       $t9, $v1, 0x2
    /* 1EBDC 8001DFDC 13200004 */  beqz       $t9, .L8001DFF0
    /* 1EBE0 8001DFE0 00714825 */   or        $t1, $v1, $s1
    /* 1EBE4 8001DFE4 00744025 */  or         $t0, $v1, $s4
    /* 1EBE8 8001DFE8 10000008 */  b          .L8001E00C
    /* 1EBEC 8001DFEC AE080008 */   sw        $t0, 0x8($s0)
  .L8001DFF0:
    /* 1EBF0 8001DFF0 10000006 */  b          .L8001E00C
    /* 1EBF4 8001DFF4 AE090008 */   sw        $t1, 0x8($s0)
    /* 1EBF8 8001DFF8 C7B20054 */  lwc1       $ft5, 0x54($sp)
  .L8001DFFC:
    /* 1EBFC 8001DFFC 8FA7005C */  lw         $a3, 0x5C($sp)
    /* 1EC00 8001E000 E7B00010 */  swc1       $ft4, 0x10($sp)
    /* 1EC04 8001E004 0C007337 */  jal        func_8001CCDC
    /* 1EC08 8001E008 E7B20014 */   swc1      $ft5, 0x14($sp)
  .L8001E00C:
    /* 1EC0C 8001E00C 8E0A0008 */  lw         $t2, 0x8($s0)
  .L8001E010:
    /* 1EC10 8001E010 3C018003 */  lui        $at, %hi(D_8002D90C)
    /* 1EC14 8001E014 000A5AC0 */  sll        $t3, $t2, 11
    /* 1EC18 8001E018 0561001A */  bgez       $t3, .L8001E084
    /* 1EC1C 8001E01C 00000000 */   nop
    /* 1EC20 8001E020 C6040040 */  lwc1       $ft0, 0x40($s0)
    /* 1EC24 8001E024 C426D90C */  lwc1       $ft1, %lo(D_8002D90C)($at)
    /* 1EC28 8001E028 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 1EC2C 8001E02C 44818000 */  mtc1       $at, $ft4
    /* 1EC30 8001E030 46062200 */  add.s      $ft2, $ft0, $ft1
    /* 1EC34 8001E034 E6080040 */  swc1       $ft2, 0x40($s0)
    /* 1EC38 8001E038 C60A0040 */  lwc1       $ft3, 0x40($s0)
    /* 1EC3C 8001E03C 460A803E */  c.le.s     $ft4, $ft3
    /* 1EC40 8001E040 00000000 */  nop
    /* 1EC44 8001E044 4500000F */  bc1f       .L8001E084
    /* 1EC48 8001E048 00000000 */   nop
    /* 1EC4C 8001E04C 8E0C0008 */  lw         $t4, 0x8($s0)
    /* 1EC50 8001E050 3C01FFEF */  lui        $at, (0xFFEFFFFF >> 16)
    /* 1EC54 8001E054 3421FFFF */  ori        $at, $at, (0xFFEFFFFF & 0xFFFF)
    /* 1EC58 8001E058 01816824 */  and        $t5, $t4, $at
    /* 1EC5C 8001E05C 10000009 */  b          .L8001E084
    /* 1EC60 8001E060 AE0D0008 */   sw        $t5, 0x8($s0)
  .L8001E064:
    /* 1EC64 8001E064 46149032 */  c.eq.s     $ft5, $fs0
    /* 1EC68 8001E068 3C01FFF7 */  lui        $at, (0xFFF7FFFF >> 16)
    /* 1EC6C 8001E06C 3421FFFF */  ori        $at, $at, (0xFFF7FFFF & 0xFFFF)
    /* 1EC70 8001E070 00617024 */  and        $t6, $v1, $at
    /* 1EC74 8001E074 45010003 */  bc1t       .L8001E084
    /* 1EC78 8001E078 01D5C025 */   or        $t8, $t6, $s5
    /* 1EC7C 8001E07C AE0E0008 */  sw         $t6, 0x8($s0)
    /* 1EC80 8001E080 AE180008 */  sw         $t8, 0x8($s0)
  .L8001E084:
    /* 1EC84 8001E084 1660FF6B */  bnez       $s3, .L8001DE34
    /* 1EC88 8001E088 02608025 */   or        $s0, $s3, $zero
  .L8001E08C:
    /* 1EC8C 8001E08C 0C007702 */  jal        func_8001DC08
    /* 1EC90 8001E090 00000000 */   nop
    /* 1EC94 8001E094 8FBF0044 */  lw         $ra, 0x44($sp)
    /* 1EC98 8001E098 D7B40020 */  ldc1       $fs0, 0x20($sp)
    /* 1EC9C 8001E09C 8FB00028 */  lw         $s0, 0x28($sp)
    /* 1ECA0 8001E0A0 8FB1002C */  lw         $s1, 0x2C($sp)
    /* 1ECA4 8001E0A4 8FB20030 */  lw         $s2, 0x30($sp)
    /* 1ECA8 8001E0A8 8FB30034 */  lw         $s3, 0x34($sp)
    /* 1ECAC 8001E0AC 8FB40038 */  lw         $s4, 0x38($sp)
    /* 1ECB0 8001E0B0 8FB5003C */  lw         $s5, 0x3C($sp)
    /* 1ECB4 8001E0B4 8FB60040 */  lw         $s6, 0x40($sp)
    /* 1ECB8 8001E0B8 03E00008 */  jr         $ra
    /* 1ECBC 8001E0BC 27BD0070 */   addiu     $sp, $sp, 0x70
endlabel func_8001DDE0
