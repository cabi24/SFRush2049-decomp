nonmatching func_8001CCDC, 0x3A8

glabel func_8001CCDC
    /* 1D8DC 8001CCDC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 1D8E0 8001CCE0 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 1D8E4 8001CCE4 AFB00018 */  sw         $s0, 0x18($sp)
    /* 1D8E8 8001CCE8 AFA40020 */  sw         $a0, 0x20($sp)
    /* 1D8EC 8001CCEC AFA60028 */  sw         $a2, 0x28($sp)
    /* 1D8F0 8001CCF0 AFA7002C */  sw         $a3, 0x2C($sp)
    /* 1D8F4 8001CCF4 8C8E0008 */  lw         $t6, 0x8($a0)
    /* 1D8F8 8001CCF8 44856000 */  mtc1       $a1, $fa0
    /* 1D8FC 8001CCFC 8C900034 */  lw         $s0, 0x34($a0)
    /* 1D900 8001CD00 000E7AC0 */  sll        $t7, $t6, 11
    /* 1D904 8001CD04 05E1002E */  bgez       $t7, .L8001CDC0
    /* 1D908 8001CD08 3C0142FE */   lui       $at, (0x42FE0000 >> 16)
    /* 1D90C 8001CD0C C4840040 */  lwc1       $ft0, 0x40($a0)
    /* 1D910 8001CD10 3C0142FE */  lui        $at, (0x42FE0000 >> 16)
    /* 1D914 8001CD14 44814000 */  mtc1       $at, $ft2
    /* 1D918 8001CD18 460C2182 */  mul.s      $ft1, $ft0, $fa0
    /* 1D91C 8001CD1C 24040001 */  addiu      $a0, $zero, 0x1
    /* 1D920 8001CD20 3C014F00 */  lui        $at, (0x4F000000 >> 16)
    /* 1D924 8001CD24 46083282 */  mul.s      $ft3, $ft1, $ft2
    /* 1D928 8001CD28 4458F800 */  cfc1       $t8, $31
    /* 1D92C 8001CD2C 44C4F800 */  ctc1       $a0, $31
    /* 1D930 8001CD30 00000000 */  nop
    /* 1D934 8001CD34 46005424 */  cvt.w.s    $ft4, $ft3
    /* 1D938 8001CD38 4444F800 */  cfc1       $a0, $31
    /* 1D93C 8001CD3C 00000000 */  nop
    /* 1D940 8001CD40 30840078 */  andi       $a0, $a0, 0x78
    /* 1D944 8001CD44 50800013 */  beql       $a0, $zero, .L8001CD94
    /* 1D948 8001CD48 44048000 */   mfc1      $a0, $ft4
    /* 1D94C 8001CD4C 44818000 */  mtc1       $at, $ft4
    /* 1D950 8001CD50 24040001 */  addiu      $a0, $zero, 0x1
    /* 1D954 8001CD54 46105401 */  sub.s      $ft4, $ft3, $ft4
    /* 1D958 8001CD58 44C4F800 */  ctc1       $a0, $31
    /* 1D95C 8001CD5C 00000000 */  nop
    /* 1D960 8001CD60 46008424 */  cvt.w.s    $ft4, $ft4
    /* 1D964 8001CD64 4444F800 */  cfc1       $a0, $31
    /* 1D968 8001CD68 00000000 */  nop
    /* 1D96C 8001CD6C 30840078 */  andi       $a0, $a0, 0x78
    /* 1D970 8001CD70 14800005 */  bnez       $a0, .L8001CD88
    /* 1D974 8001CD74 00000000 */   nop
    /* 1D978 8001CD78 44048000 */  mfc1       $a0, $ft4
    /* 1D97C 8001CD7C 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* 1D980 8001CD80 10000007 */  b          .L8001CDA0
    /* 1D984 8001CD84 00812025 */   or        $a0, $a0, $at
  .L8001CD88:
    /* 1D988 8001CD88 10000005 */  b          .L8001CDA0
    /* 1D98C 8001CD8C 2404FFFF */   addiu     $a0, $zero, -0x1
    /* 1D990 8001CD90 44048000 */  mfc1       $a0, $ft4
  .L8001CD94:
    /* 1D994 8001CD94 00000000 */  nop
    /* 1D998 8001CD98 0480FFFB */  bltz       $a0, .L8001CD88
    /* 1D99C 8001CD9C 00000000 */   nop
  .L8001CDA0:
    /* 1D9A0 8001CDA0 44D8F800 */  ctc1       $t8, $31
    /* 1D9A4 8001CDA4 0C007327 */  jal        func_8001CC9C
    /* 1D9A8 8001CDA8 308400FF */   andi      $a0, $a0, 0xFF
    /* 1D9AC 8001CDAC 02002025 */  or         $a0, $s0, $zero
    /* 1D9B0 8001CDB0 0C006DF0 */  jal        func_8001B7C0
    /* 1D9B4 8001CDB4 304500FF */   andi      $a1, $v0, 0xFF
    /* 1D9B8 8001CDB8 1000002A */  b          .L8001CE64
    /* 1D9BC 8001CDBC 3C013F80 */   lui       $at, (0x3F800000 >> 16)
  .L8001CDC0:
    /* 1D9C0 8001CDC0 44819000 */  mtc1       $at, $ft5
    /* 1D9C4 8001CDC4 24040001 */  addiu      $a0, $zero, 0x1
    /* 1D9C8 8001CDC8 3C014F00 */  lui        $at, (0x4F000000 >> 16)
    /* 1D9CC 8001CDCC 46126102 */  mul.s      $ft0, $fa0, $ft5
    /* 1D9D0 8001CDD0 4459F800 */  cfc1       $t9, $31
    /* 1D9D4 8001CDD4 44C4F800 */  ctc1       $a0, $31
    /* 1D9D8 8001CDD8 00000000 */  nop
    /* 1D9DC 8001CDDC 460021A4 */  cvt.w.s    $ft1, $ft0
    /* 1D9E0 8001CDE0 4444F800 */  cfc1       $a0, $31
    /* 1D9E4 8001CDE4 00000000 */  nop
    /* 1D9E8 8001CDE8 30840078 */  andi       $a0, $a0, 0x78
    /* 1D9EC 8001CDEC 50800013 */  beql       $a0, $zero, .L8001CE3C
    /* 1D9F0 8001CDF0 44043000 */   mfc1      $a0, $ft1
    /* 1D9F4 8001CDF4 44813000 */  mtc1       $at, $ft1
    /* 1D9F8 8001CDF8 24040001 */  addiu      $a0, $zero, 0x1
    /* 1D9FC 8001CDFC 46062181 */  sub.s      $ft1, $ft0, $ft1
    /* 1DA00 8001CE00 44C4F800 */  ctc1       $a0, $31
    /* 1DA04 8001CE04 00000000 */  nop
    /* 1DA08 8001CE08 460031A4 */  cvt.w.s    $ft1, $ft1
    /* 1DA0C 8001CE0C 4444F800 */  cfc1       $a0, $31
    /* 1DA10 8001CE10 00000000 */  nop
    /* 1DA14 8001CE14 30840078 */  andi       $a0, $a0, 0x78
    /* 1DA18 8001CE18 14800005 */  bnez       $a0, .L8001CE30
    /* 1DA1C 8001CE1C 00000000 */   nop
    /* 1DA20 8001CE20 44043000 */  mfc1       $a0, $ft1
    /* 1DA24 8001CE24 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* 1DA28 8001CE28 10000007 */  b          .L8001CE48
    /* 1DA2C 8001CE2C 00812025 */   or        $a0, $a0, $at
  .L8001CE30:
    /* 1DA30 8001CE30 10000005 */  b          .L8001CE48
    /* 1DA34 8001CE34 2404FFFF */   addiu     $a0, $zero, -0x1
    /* 1DA38 8001CE38 44043000 */  mfc1       $a0, $ft1
  .L8001CE3C:
    /* 1DA3C 8001CE3C 00000000 */  nop
    /* 1DA40 8001CE40 0480FFFB */  bltz       $a0, .L8001CE30
    /* 1DA44 8001CE44 00000000 */   nop
  .L8001CE48:
    /* 1DA48 8001CE48 44D9F800 */  ctc1       $t9, $31
    /* 1DA4C 8001CE4C 0C007327 */  jal        func_8001CC9C
    /* 1DA50 8001CE50 308400FF */   andi      $a0, $a0, 0xFF
    /* 1DA54 8001CE54 02002025 */  or         $a0, $s0, $zero
    /* 1DA58 8001CE58 0C006DF0 */  jal        func_8001B7C0
    /* 1DA5C 8001CE5C 304500FF */   andi      $a1, $v0, 0xFF
    /* 1DA60 8001CE60 3C013F80 */  lui        $at, (0x3F800000 >> 16)
  .L8001CE64:
    /* 1DA64 8001CE64 44814000 */  mtc1       $at, $ft2
    /* 1DA68 8001CE68 C7AA0028 */  lwc1       $ft3, 0x28($sp)
    /* 1DA6C 8001CE6C 3C014280 */  lui        $at, (0x42800000 >> 16)
    /* 1DA70 8001CE70 44819000 */  mtc1       $at, $ft5
    /* 1DA74 8001CE74 460A4400 */  add.s      $ft4, $ft2, $ft3
    /* 1DA78 8001CE78 24040001 */  addiu      $a0, $zero, 0x1
    /* 1DA7C 8001CE7C 3C014F00 */  lui        $at, (0x4F000000 >> 16)
    /* 1DA80 8001CE80 46128102 */  mul.s      $ft0, $ft4, $ft5
    /* 1DA84 8001CE84 4448F800 */  cfc1       $t0, $31
    /* 1DA88 8001CE88 44C4F800 */  ctc1       $a0, $31
    /* 1DA8C 8001CE8C 00000000 */  nop
    /* 1DA90 8001CE90 460021A4 */  cvt.w.s    $ft1, $ft0
    /* 1DA94 8001CE94 4444F800 */  cfc1       $a0, $31
    /* 1DA98 8001CE98 00000000 */  nop
    /* 1DA9C 8001CE9C 30840078 */  andi       $a0, $a0, 0x78
    /* 1DAA0 8001CEA0 50800013 */  beql       $a0, $zero, .L8001CEF0
    /* 1DAA4 8001CEA4 44043000 */   mfc1      $a0, $ft1
    /* 1DAA8 8001CEA8 44813000 */  mtc1       $at, $ft1
    /* 1DAAC 8001CEAC 24040001 */  addiu      $a0, $zero, 0x1
    /* 1DAB0 8001CEB0 46062181 */  sub.s      $ft1, $ft0, $ft1
    /* 1DAB4 8001CEB4 44C4F800 */  ctc1       $a0, $31
    /* 1DAB8 8001CEB8 00000000 */  nop
    /* 1DABC 8001CEBC 460031A4 */  cvt.w.s    $ft1, $ft1
    /* 1DAC0 8001CEC0 4444F800 */  cfc1       $a0, $31
    /* 1DAC4 8001CEC4 00000000 */  nop
    /* 1DAC8 8001CEC8 30840078 */  andi       $a0, $a0, 0x78
    /* 1DACC 8001CECC 14800005 */  bnez       $a0, .L8001CEE4
    /* 1DAD0 8001CED0 00000000 */   nop
    /* 1DAD4 8001CED4 44043000 */  mfc1       $a0, $ft1
    /* 1DAD8 8001CED8 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* 1DADC 8001CEDC 10000007 */  b          .L8001CEFC
    /* 1DAE0 8001CEE0 00812025 */   or        $a0, $a0, $at
  .L8001CEE4:
    /* 1DAE4 8001CEE4 10000005 */  b          .L8001CEFC
    /* 1DAE8 8001CEE8 2404FFFF */   addiu     $a0, $zero, -0x1
    /* 1DAEC 8001CEEC 44043000 */  mfc1       $a0, $ft1
  .L8001CEF0:
    /* 1DAF0 8001CEF0 00000000 */  nop
    /* 1DAF4 8001CEF4 0480FFFB */  bltz       $a0, .L8001CEE4
    /* 1DAF8 8001CEF8 00000000 */   nop
  .L8001CEFC:
    /* 1DAFC 8001CEFC 44C8F800 */  ctc1       $t0, $31
    /* 1DB00 8001CF00 0C007327 */  jal        func_8001CC9C
    /* 1DB04 8001CF04 308400FF */   andi      $a0, $a0, 0xFF
    /* 1DB08 8001CF08 02002025 */  or         $a0, $s0, $zero
    /* 1DB0C 8001CF0C 0C006CA7 */  jal        func_8001B29C
    /* 1DB10 8001CF10 304500FF */   andi      $a1, $v0, 0xFF
    /* 1DB14 8001CF14 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 1DB18 8001CF18 44814000 */  mtc1       $at, $ft2
    /* 1DB1C 8001CF1C C7AA0030 */  lwc1       $ft3, 0x30($sp)
    /* 1DB20 8001CF20 3C014280 */  lui        $at, (0x42800000 >> 16)
    /* 1DB24 8001CF24 44819000 */  mtc1       $at, $ft5
    /* 1DB28 8001CF28 460A4401 */  sub.s      $ft4, $ft2, $ft3
    /* 1DB2C 8001CF2C 24040001 */  addiu      $a0, $zero, 0x1
    /* 1DB30 8001CF30 3C014F00 */  lui        $at, (0x4F000000 >> 16)
    /* 1DB34 8001CF34 46128102 */  mul.s      $ft0, $ft4, $ft5
    /* 1DB38 8001CF38 4449F800 */  cfc1       $t1, $31
    /* 1DB3C 8001CF3C 44C4F800 */  ctc1       $a0, $31
    /* 1DB40 8001CF40 00000000 */  nop
    /* 1DB44 8001CF44 460021A4 */  cvt.w.s    $ft1, $ft0
    /* 1DB48 8001CF48 4444F800 */  cfc1       $a0, $31
    /* 1DB4C 8001CF4C 00000000 */  nop
    /* 1DB50 8001CF50 30840078 */  andi       $a0, $a0, 0x78
    /* 1DB54 8001CF54 50800013 */  beql       $a0, $zero, .L8001CFA4
    /* 1DB58 8001CF58 44043000 */   mfc1      $a0, $ft1
    /* 1DB5C 8001CF5C 44813000 */  mtc1       $at, $ft1
    /* 1DB60 8001CF60 24040001 */  addiu      $a0, $zero, 0x1
    /* 1DB64 8001CF64 46062181 */  sub.s      $ft1, $ft0, $ft1
    /* 1DB68 8001CF68 44C4F800 */  ctc1       $a0, $31
    /* 1DB6C 8001CF6C 00000000 */  nop
    /* 1DB70 8001CF70 460031A4 */  cvt.w.s    $ft1, $ft1
    /* 1DB74 8001CF74 4444F800 */  cfc1       $a0, $31
    /* 1DB78 8001CF78 00000000 */  nop
    /* 1DB7C 8001CF7C 30840078 */  andi       $a0, $a0, 0x78
    /* 1DB80 8001CF80 14800005 */  bnez       $a0, .L8001CF98
    /* 1DB84 8001CF84 00000000 */   nop
    /* 1DB88 8001CF88 44043000 */  mfc1       $a0, $ft1
    /* 1DB8C 8001CF8C 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* 1DB90 8001CF90 10000007 */  b          .L8001CFB0
    /* 1DB94 8001CF94 00812025 */   or        $a0, $a0, $at
  .L8001CF98:
    /* 1DB98 8001CF98 10000005 */  b          .L8001CFB0
    /* 1DB9C 8001CF9C 2404FFFF */   addiu     $a0, $zero, -0x1
    /* 1DBA0 8001CFA0 44043000 */  mfc1       $a0, $ft1
  .L8001CFA4:
    /* 1DBA4 8001CFA4 00000000 */  nop
    /* 1DBA8 8001CFA8 0480FFFB */  bltz       $a0, .L8001CF98
    /* 1DBAC 8001CFAC 00000000 */   nop
  .L8001CFB0:
    /* 1DBB0 8001CFB0 44C9F800 */  ctc1       $t1, $31
    /* 1DBB4 8001CFB4 0C007327 */  jal        func_8001CC9C
    /* 1DBB8 8001CFB8 308400FF */   andi      $a0, $a0, 0xFF
    /* 1DBBC 8001CFBC 02002025 */  or         $a0, $s0, $zero
    /* 1DBC0 8001CFC0 0C006CE8 */  jal        func_8001B3A0
    /* 1DBC4 8001CFC4 304500FF */   andi      $a1, $v0, 0xFF
    /* 1DBC8 8001CFC8 3C014600 */  lui        $at, (0x46000000 >> 16)
    /* 1DBCC 8001CFCC 44815000 */  mtc1       $at, $ft3
    /* 1DBD0 8001CFD0 C7A80034 */  lwc1       $ft2, 0x34($sp)
    /* 1DBD4 8001CFD4 24040001 */  addiu      $a0, $zero, 0x1
    /* 1DBD8 8001CFD8 3C014F00 */  lui        $at, (0x4F000000 >> 16)
    /* 1DBDC 8001CFDC 460A4402 */  mul.s      $ft4, $ft2, $ft3
    /* 1DBE0 8001CFE0 444AF800 */  cfc1       $t2, $31
    /* 1DBE4 8001CFE4 44C4F800 */  ctc1       $a0, $31
    /* 1DBE8 8001CFE8 00000000 */  nop
    /* 1DBEC 8001CFEC 460084A4 */  cvt.w.s    $ft5, $ft4
    /* 1DBF0 8001CFF0 4444F800 */  cfc1       $a0, $31
    /* 1DBF4 8001CFF4 00000000 */  nop
    /* 1DBF8 8001CFF8 30840078 */  andi       $a0, $a0, 0x78
    /* 1DBFC 8001CFFC 50800013 */  beql       $a0, $zero, .L8001D04C
    /* 1DC00 8001D000 44049000 */   mfc1      $a0, $ft5
    /* 1DC04 8001D004 44819000 */  mtc1       $at, $ft5
    /* 1DC08 8001D008 24040001 */  addiu      $a0, $zero, 0x1
    /* 1DC0C 8001D00C 46128481 */  sub.s      $ft5, $ft4, $ft5
    /* 1DC10 8001D010 44C4F800 */  ctc1       $a0, $31
    /* 1DC14 8001D014 00000000 */  nop
    /* 1DC18 8001D018 460094A4 */  cvt.w.s    $ft5, $ft5
    /* 1DC1C 8001D01C 4444F800 */  cfc1       $a0, $31
    /* 1DC20 8001D020 00000000 */  nop
    /* 1DC24 8001D024 30840078 */  andi       $a0, $a0, 0x78
    /* 1DC28 8001D028 14800005 */  bnez       $a0, .L8001D040
    /* 1DC2C 8001D02C 00000000 */   nop
    /* 1DC30 8001D030 44049000 */  mfc1       $a0, $ft5
    /* 1DC34 8001D034 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* 1DC38 8001D038 10000007 */  b          .L8001D058
    /* 1DC3C 8001D03C 00812025 */   or        $a0, $a0, $at
  .L8001D040:
    /* 1DC40 8001D040 10000005 */  b          .L8001D058
    /* 1DC44 8001D044 2404FFFF */   addiu     $a0, $zero, -0x1
    /* 1DC48 8001D048 44049000 */  mfc1       $a0, $ft5
  .L8001D04C:
    /* 1DC4C 8001D04C 00000000 */  nop
    /* 1DC50 8001D050 0480FFFB */  bltz       $a0, .L8001D040
    /* 1DC54 8001D054 00000000 */   nop
  .L8001D058:
    /* 1DC58 8001D058 44CAF800 */  ctc1       $t2, $31
    /* 1DC5C 8001D05C 0C007330 */  jal        func_8001CCC0
    /* 1DC60 8001D060 00000000 */   nop
    /* 1DC64 8001D064 02002025 */  or         $a0, $s0, $zero
    /* 1DC68 8001D068 0C006D29 */  jal        func_8001B4A4
    /* 1DC6C 8001D06C 3045FFFF */   andi      $a1, $v0, 0xFFFF
    /* 1DC70 8001D070 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 1DC74 8001D074 8FB00018 */  lw         $s0, 0x18($sp)
    /* 1DC78 8001D078 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 1DC7C 8001D07C 03E00008 */  jr         $ra
    /* 1DC80 8001D080 00000000 */   nop
endlabel func_8001CCDC
