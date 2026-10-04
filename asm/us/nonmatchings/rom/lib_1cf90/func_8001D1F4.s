nonmatching func_8001D1F4, 0x1FC

glabel func_8001D1F4
    /* 1DDF4 8001D1F4 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* 1DDF8 8001D1F8 AFBF0024 */  sw         $ra, 0x24($sp)
    /* 1DDFC 8001D1FC AFB00020 */  sw         $s0, 0x20($sp)
    /* 1DE00 8001D200 AFA5004C */  sw         $a1, 0x4C($sp)
    /* 1DE04 8001D204 AFA60050 */  sw         $a2, 0x50($sp)
    /* 1DE08 8001D208 AFA70054 */  sw         $a3, 0x54($sp)
    /* 1DE0C 8001D20C 0C005165 */  jal        func_80014594
    /* 1DE10 8001D210 AFA40048 */   sw        $a0, 0x48($sp)
    /* 1DE14 8001D214 8FA40048 */  lw         $a0, 0x48($sp)
    /* 1DE18 8001D218 8FAE005C */  lw         $t6, 0x5C($sp)
    /* 1DE1C 8001D21C 3C028005 */  lui        $v0, %hi(D_8004FD50)
    /* 1DE20 8001D220 14800004 */  bnez       $a0, .L8001D234
    /* 1DE24 8001D224 00808025 */   or        $s0, $a0, $zero
    /* 1DE28 8001D228 3C108005 */  lui        $s0, %hi(D_8004FD58)
    /* 1DE2C 8001D22C 10000001 */  b          .L8001D234
    /* 1DE30 8001D230 2610FD58 */   addiu     $s0, $s0, %lo(D_8004FD58)
  .L8001D234:
    /* 1DE34 8001D234 AE0E0008 */  sw         $t6, 0x8($s0)
    /* 1DE38 8001D238 8FAF004C */  lw         $t7, 0x4C($sp)
    /* 1DE3C 8001D23C 3C0142FE */  lui        $at, (0x42FE0000 >> 16)
    /* 1DE40 8001D240 44810000 */  mtc1       $at, $fv0
    /* 1DE44 8001D244 8DF90000 */  lw         $t9, 0x0($t7)
    /* 1DE48 8001D248 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* 1DE4C 8001D24C 27A50034 */  addiu      $a1, $sp, 0x34
    /* 1DE50 8001D250 AE19000C */  sw         $t9, 0xC($s0)
    /* 1DE54 8001D254 8DF80004 */  lw         $t8, 0x4($t7)
    /* 1DE58 8001D258 27A60030 */  addiu      $a2, $sp, 0x30
    /* 1DE5C 8001D25C 27A70040 */  addiu      $a3, $sp, 0x40
    /* 1DE60 8001D260 AE180010 */  sw         $t8, 0x10($s0)
    /* 1DE64 8001D264 8DF90008 */  lw         $t9, 0x8($t7)
    /* 1DE68 8001D268 27AF003C */  addiu      $t7, $sp, 0x3C
    /* 1DE6C 8001D26C 27B80038 */  addiu      $t8, $sp, 0x38
    /* 1DE70 8001D270 AE190014 */  sw         $t9, 0x14($s0)
    /* 1DE74 8001D274 8FA80050 */  lw         $t0, 0x50($sp)
    /* 1DE78 8001D278 2442FD50 */  addiu      $v0, $v0, %lo(D_8004FD50)
    /* 1DE7C 8001D27C 8D0A0000 */  lw         $t2, 0x0($t0)
    /* 1DE80 8001D280 AE0A0018 */  sw         $t2, 0x18($s0)
    /* 1DE84 8001D284 8D090004 */  lw         $t1, 0x4($t0)
    /* 1DE88 8001D288 AE09001C */  sw         $t1, 0x1C($s0)
    /* 1DE8C 8001D28C 8D0A0008 */  lw         $t2, 0x8($t0)
    /* 1DE90 8001D290 AE0A0020 */  sw         $t2, 0x20($s0)
    /* 1DE94 8001D294 C7A40054 */  lwc1       $ft0, 0x54($sp)
    /* 1DE98 8001D298 240AFFFF */  addiu      $t2, $zero, -0x1
    /* 1DE9C 8001D29C E6040024 */  swc1       $ft0, 0x24($s0)
    /* 1DEA0 8001D2A0 97AB0062 */  lhu        $t3, 0x62($sp)
    /* 1DEA4 8001D2A4 A60B003C */  sh         $t3, 0x3C($s0)
    /* 1DEA8 8001D2A8 93AC006B */  lbu        $t4, 0x6B($sp)
    /* 1DEAC 8001D2AC 448C3000 */  mtc1       $t4, $ft1
    /* 1DEB0 8001D2B0 05810004 */  bgez       $t4, .L8001D2C4
    /* 1DEB4 8001D2B4 46803220 */   cvt.s.w   $ft2, $ft1
    /* 1DEB8 8001D2B8 44815000 */  mtc1       $at, $ft3
    /* 1DEBC 8001D2BC 00000000 */  nop
    /* 1DEC0 8001D2C0 460A4200 */  add.s      $ft2, $ft2, $ft3
  .L8001D2C4:
    /* 1DEC4 8001D2C4 46004403 */  div.s      $ft4, $ft2, $fv0
    /* 1DEC8 8001D2C8 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* 1DECC 8001D2CC E6100028 */  swc1       $ft4, 0x28($s0)
    /* 1DED0 8001D2D0 93AD006F */  lbu        $t5, 0x6F($sp)
    /* 1DED4 8001D2D4 448D9000 */  mtc1       $t5, $ft5
    /* 1DED8 8001D2D8 05A10004 */  bgez       $t5, .L8001D2EC
    /* 1DEDC 8001D2DC 46809120 */   cvt.s.w   $ft0, $ft5
    /* 1DEE0 8001D2E0 44813000 */  mtc1       $at, $ft1
    /* 1DEE4 8001D2E4 00000000 */  nop
    /* 1DEE8 8001D2E8 46062100 */  add.s      $ft0, $ft0, $ft1
  .L8001D2EC:
    /* 1DEEC 8001D2EC 46002283 */  div.s      $ft3, $ft0, $fv0
    /* 1DEF0 8001D2F0 3C010003 */  lui        $at, (0x30000 >> 16)
    /* 1DEF4 8001D2F4 E60A002C */  swc1       $ft3, 0x2C($s0)
    /* 1DEF8 8001D2F8 C7A80058 */  lwc1       $ft2, 0x58($sp)
    /* 1DEFC 8001D2FC E6080030 */  swc1       $ft2, 0x30($s0)
    /* 1DF00 8001D300 8FAE0064 */  lw         $t6, 0x64($sp)
    /* 1DF04 8001D304 14800027 */  bnez       $a0, .L8001D3A4
    /* 1DF08 8001D308 AE0E0038 */   sw        $t6, 0x38($s0)
    /* 1DF0C 8001D30C 02002025 */  or         $a0, $s0, $zero
    /* 1DF10 8001D310 AFAF0010 */  sw         $t7, 0x10($sp)
    /* 1DF14 8001D314 0C007218 */  jal        func_8001C860
    /* 1DF18 8001D318 AFB80014 */   sw        $t8, 0x14($sp)
    /* 1DF1C 8001D31C C7B00034 */  lwc1       $ft4, 0x34($sp)
    /* 1DF20 8001D320 44809000 */  mtc1       $zero, $ft5
    /* 1DF24 8001D324 2405007F */  addiu      $a1, $zero, 0x7F
    /* 1DF28 8001D328 24060040 */  addiu      $a2, $zero, 0x40
    /* 1DF2C 8001D32C 46128032 */  c.eq.s     $ft4, $ft5
    /* 1DF30 8001D330 00000000 */  nop
    /* 1DF34 8001D334 45000005 */  bc1f       .L8001D34C
    /* 1DF38 8001D338 00000000 */   nop
    /* 1DF3C 8001D33C 0C005177 */  jal        func_800145DC
    /* 1DF40 8001D340 00000000 */   nop
    /* 1DF44 8001D344 10000025 */  b          .L8001D3DC
    /* 1DF48 8001D348 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8001D34C:
    /* 1DF4C 8001D34C 0C00805D */  jal        func_80020174
    /* 1DF50 8001D350 9604003C */   lhu       $a0, 0x3C($s0)
    /* 1DF54 8001D354 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1DF58 8001D358 14410005 */  bne        $v0, $at, .L8001D370
    /* 1DF5C 8001D35C AE020034 */   sw        $v0, 0x34($s0)
    /* 1DF60 8001D360 0C005177 */  jal        func_800145DC
    /* 1DF64 8001D364 00000000 */   nop
    /* 1DF68 8001D368 1000001C */  b          .L8001D3DC
    /* 1DF6C 8001D36C 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8001D370:
    /* 1DF70 8001D370 C7A60038 */  lwc1       $ft1, 0x38($sp)
    /* 1DF74 8001D374 C7A40030 */  lwc1       $ft0, 0x30($sp)
    /* 1DF78 8001D378 02002025 */  or         $a0, $s0, $zero
    /* 1DF7C 8001D37C 8FA50034 */  lw         $a1, 0x34($sp)
    /* 1DF80 8001D380 8FA60040 */  lw         $a2, 0x40($sp)
    /* 1DF84 8001D384 8FA7003C */  lw         $a3, 0x3C($sp)
    /* 1DF88 8001D388 E7A60010 */  swc1       $ft1, 0x10($sp)
    /* 1DF8C 8001D38C 0C007337 */  jal        func_8001CCDC
    /* 1DF90 8001D390 E7A40014 */   swc1      $ft0, 0x14($sp)
    /* 1DF94 8001D394 0C005177 */  jal        func_800145DC
    /* 1DF98 8001D398 00000000 */   nop
    /* 1DF9C 8001D39C 1000000F */  b          .L8001D3DC
    /* 1DFA0 8001D3A0 8E020034 */   lw        $v0, 0x34($s0)
  .L8001D3A4:
    /* 1DFA4 8001D3A4 8C590000 */  lw         $t9, 0x0($v0)
    /* 1DFA8 8001D3A8 13200003 */  beqz       $t9, .L8001D3B8
    /* 1DFAC 8001D3AC AE190000 */   sw        $t9, 0x0($s0)
    /* 1DFB0 8001D3B0 8C490000 */  lw         $t1, 0x0($v0)
    /* 1DFB4 8001D3B4 AD300004 */  sw         $s0, 0x4($t1)
  .L8001D3B8:
    /* 1DFB8 8001D3B8 AE000004 */  sw         $zero, 0x4($s0)
    /* 1DFBC 8001D3BC AC500000 */  sw         $s0, 0x0($v0)
    /* 1DFC0 8001D3C0 8E0B0008 */  lw         $t3, 0x8($s0)
    /* 1DFC4 8001D3C4 AE0A0034 */  sw         $t2, 0x34($s0)
    /* 1DFC8 8001D3C8 A600003E */  sh         $zero, 0x3E($s0)
    /* 1DFCC 8001D3CC 01616025 */  or         $t4, $t3, $at
    /* 1DFD0 8001D3D0 0C005177 */  jal        func_800145DC
    /* 1DFD4 8001D3D4 AE0C0008 */   sw        $t4, 0x8($s0)
    /* 1DFD8 8001D3D8 2402FFFF */  addiu      $v0, $zero, -0x1
  .L8001D3DC:
    /* 1DFDC 8001D3DC 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 1DFE0 8001D3E0 8FB00020 */  lw         $s0, 0x20($sp)
    /* 1DFE4 8001D3E4 27BD0048 */  addiu      $sp, $sp, 0x48
    /* 1DFE8 8001D3E8 03E00008 */  jr         $ra
    /* 1DFEC 8001D3EC 00000000 */   nop
endlabel func_8001D1F4
