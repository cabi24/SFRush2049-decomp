nonmatching func_8001A270, 0x368

glabel func_8001A270
    /* 1AE70 8001A270 27BDFFA0 */  addiu      $sp, $sp, -0x60
    /* 1AE74 8001A274 87B90086 */  lh         $t9, 0x86($sp)
    /* 1AE78 8001A278 00047A02 */  srl        $t7, $a0, 8
    /* 1AE7C 8001A27C 31F800FF */  andi       $t8, $t7, 0xFF
    /* 1AE80 8001A280 AFB00038 */  sw         $s0, 0x38($sp)
    /* 1AE84 8001A284 00047402 */  srl        $t6, $a0, 16
    /* 1AE88 8001A288 03191021 */  addu       $v0, $t8, $t9
    /* 1AE8C 8001A28C 00808025 */  or         $s0, $a0, $zero
    /* 1AE90 8001A290 AFBF003C */  sw         $ra, 0x3C($sp)
    /* 1AE94 8001A294 AFA50064 */  sw         $a1, 0x64($sp)
    /* 1AE98 8001A298 AFA60068 */  sw         $a2, 0x68($sp)
    /* 1AE9C 8001A29C AFA7006C */  sw         $a3, 0x6C($sp)
    /* 1AEA0 8001A2A0 04410003 */  bgez       $v0, .L8001A2B0
    /* 1AEA4 8001A2A4 A7AE005A */   sh        $t6, 0x5A($sp)
    /* 1AEA8 8001A2A8 10000007 */  b          .L8001A2C8
    /* 1AEAC 8001A2AC 00002825 */   or        $a1, $zero, $zero
  .L8001A2B0:
    /* 1AEB0 8001A2B0 28410100 */  slti       $at, $v0, 0x100
    /* 1AEB4 8001A2B4 14200003 */  bnez       $at, .L8001A2C4
    /* 1AEB8 8001A2B8 00401825 */   or        $v1, $v0, $zero
    /* 1AEBC 8001A2BC 10000001 */  b          .L8001A2C4
    /* 1AEC0 8001A2C0 240300FF */   addiu     $v1, $zero, 0xFF
  .L8001A2C4:
    /* 1AEC4 8001A2C4 00602825 */  or         $a1, $v1, $zero
  .L8001A2C8:
    /* 1AEC8 8001A2C8 97A2005A */  lhu        $v0, 0x5A($sp)
    /* 1AECC 8001A2CC 3C01FFFF */  lui        $at, (0xFFFF00FF >> 16)
    /* 1AED0 8001A2D0 342100FF */  ori        $at, $at, (0xFFFF00FF & 0xFFFF)
    /* 1AED4 8001A2D4 02015824 */  and        $t3, $s0, $at
    /* 1AED8 8001A2D8 00056200 */  sll        $t4, $a1, 8
    /* 1AEDC 8001A2DC 3042C000 */  andi       $v0, $v0, 0xC000
    /* 1AEE0 8001A2E0 10400009 */  beqz       $v0, .L8001A308
    /* 1AEE4 8001A2E4 016C8025 */   or        $s0, $t3, $t4
    /* 1AEE8 8001A2E8 24014000 */  addiu      $at, $zero, 0x4000
    /* 1AEEC 8001A2EC 10410024 */  beq        $v0, $at, .L8001A380
    /* 1AEF0 8001A2F0 97A4005A */   lhu       $a0, 0x5A($sp)
    /* 1AEF4 8001A2F4 34018000 */  ori        $at, $zero, 0x8000
    /* 1AEF8 8001A2F8 1041009E */  beq        $v0, $at, .L8001A574
    /* 1AEFC 8001A2FC 02002025 */   or        $a0, $s0, $zero
    /* 1AF00 8001A300 100000B0 */  b          .L8001A5C4
    /* 1AF04 8001A304 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8001A308:
    /* 1AF08 8001A308 93A40067 */  lbu        $a0, 0x67($sp)
    /* 1AF0C 8001A30C 93A50073 */  lbu        $a1, 0x73($sp)
    /* 1AF10 8001A310 0C0067B4 */  jal        func_80019ED0
    /* 1AF14 8001A314 93A60077 */   lbu       $a2, 0x77($sp)
    /* 1AF18 8001A318 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1AF1C 8001A31C 10410003 */  beq        $v0, $at, .L8001A32C
    /* 1AF20 8001A320 93A9006F */   lbu       $t1, 0x6F($sp)
    /* 1AF24 8001A324 100000A8 */  b          .L8001A5C8
    /* 1AF28 8001A328 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8001A32C:
    /* 1AF2C 8001A32C 93AD0073 */  lbu        $t5, 0x73($sp)
    /* 1AF30 8001A330 93AE0077 */  lbu        $t6, 0x77($sp)
    /* 1AF34 8001A334 97AF007A */  lhu        $t7, 0x7A($sp)
    /* 1AF38 8001A338 97B8007E */  lhu        $t8, 0x7E($sp)
    /* 1AF3C 8001A33C 93AB0083 */  lbu        $t3, 0x83($sp)
    /* 1AF40 8001A340 00102C02 */  srl        $a1, $s0, 16
    /* 1AF44 8001A344 24190001 */  addiu      $t9, $zero, 0x1
    /* 1AF48 8001A348 AFB90024 */  sw         $t9, 0x24($sp)
    /* 1AF4C 8001A34C 30A5FFFF */  andi       $a1, $a1, 0xFFFF
    /* 1AF50 8001A350 02002025 */  or         $a0, $s0, $zero
    /* 1AF54 8001A354 93A60067 */  lbu        $a2, 0x67($sp)
    /* 1AF58 8001A358 93A7006B */  lbu        $a3, 0x6B($sp)
    /* 1AF5C 8001A35C AFA90010 */  sw         $t1, 0x10($sp)
    /* 1AF60 8001A360 AFAD0014 */  sw         $t5, 0x14($sp)
    /* 1AF64 8001A364 AFAE0018 */  sw         $t6, 0x18($sp)
    /* 1AF68 8001A368 AFAF001C */  sw         $t7, 0x1C($sp)
    /* 1AF6C 8001A36C AFB80020 */  sw         $t8, 0x20($sp)
    /* 1AF70 8001A370 0C009262 */  jal        func_80024988
    /* 1AF74 8001A374 AFAB0028 */   sw        $t3, 0x28($sp)
    /* 1AF78 8001A378 10000093 */  b          .L8001A5C8
    /* 1AF7C 8001A37C 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8001A380:
    /* 1AF80 8001A380 0C005BB8 */  jal        func_80016EE0
    /* 1AF84 8001A384 AFA5004C */   sw        $a1, 0x4C($sp)
    /* 1AF88 8001A388 8FA5004C */  lw         $a1, 0x4C($sp)
    /* 1AF8C 8001A38C 1040008C */  beqz       $v0, .L8001A5C0
    /* 1AF90 8001A390 93A9006F */   lbu       $t1, 0x6F($sp)
    /* 1AF94 8001A394 93A80067 */  lbu        $t0, 0x67($sp)
    /* 1AF98 8001A398 321000FF */  andi       $s0, $s0, 0xFF
    /* 1AF9C 8001A39C 3106007F */  andi       $a2, $t0, 0x7F
    /* 1AFA0 8001A3A0 30CC00FF */  andi       $t4, $a2, 0xFF
    /* 1AFA4 8001A3A4 000C68C0 */  sll        $t5, $t4, 3
    /* 1AFA8 8001A3A8 004D2021 */  addu       $a0, $v0, $t5
    /* 1AFAC 8001A3AC 90810000 */  lbu        $at, 0x0($a0)
    /* 1AFB0 8001A3B0 90870001 */  lbu        $a3, 0x1($a0)
    /* 1AFB4 8001A3B4 01005025 */  or         $t2, $t0, $zero
    /* 1AFB8 8001A3B8 00010A00 */  sll        $at, $at, 8
    /* 1AFBC 8001A3BC 00E13825 */  or         $a3, $a3, $at
    /* 1AFC0 8001A3C0 3401FFFF */  ori        $at, $zero, 0xFFFF
    /* 1AFC4 8001A3C4 50E1007F */  beql       $a3, $at, .L8001A5C4
    /* 1AFC8 8001A3C8 2402FFFF */   addiu     $v0, $zero, -0x1
    /* 1AFCC 8001A3CC 908E0003 */  lbu        $t6, 0x3($a0)
    /* 1AFD0 8001A3D0 0008C0C0 */  sll        $t8, $t0, 3
    /* 1AFD4 8001A3D4 0058C821 */  addu       $t9, $v0, $t8
    /* 1AFD8 8001A3D8 31CF0080 */  andi       $t7, $t6, 0x80
    /* 1AFDC 8001A3DC 15E0000E */  bnez       $t7, .L8001A418
    /* 1AFE0 8001A3E0 00077400 */   sll       $t6, $a3, 16
    /* 1AFE4 8001A3E4 93230003 */  lbu        $v1, 0x3($t9)
    /* 1AFE8 8001A3E8 2463FFC0 */  addiu      $v1, $v1, -0x40
    /* 1AFEC 8001A3EC 00691821 */  addu       $v1, $v1, $t1
    /* 1AFF0 8001A3F0 04610003 */  bgez       $v1, .L8001A400
    /* 1AFF4 8001A3F4 28610080 */   slti      $at, $v1, 0x80
    /* 1AFF8 8001A3F8 10000008 */  b          .L8001A41C
    /* 1AFFC 8001A3FC 00004825 */   or        $t1, $zero, $zero
  .L8001A400:
    /* 1B000 8001A400 14200003 */  bnez       $at, .L8001A410
    /* 1B004 8001A404 00000000 */   nop
    /* 1B008 8001A408 10000004 */  b          .L8001A41C
    /* 1B00C 8001A40C 2409007F */   addiu     $t1, $zero, 0x7F
  .L8001A410:
    /* 1B010 8001A410 10000002 */  b          .L8001A41C
    /* 1B014 8001A414 306900FF */   andi      $t1, $v1, 0xFF
  .L8001A418:
    /* 1B018 8001A418 24090080 */  addiu      $t1, $zero, 0x80
  .L8001A41C:
    /* 1B01C 8001A41C 808B0002 */  lb         $t3, 0x2($a0)
    /* 1B020 8001A420 30EFC000 */  andi       $t7, $a3, 0xC000
    /* 1B024 8001A424 93A7006B */  lbu        $a3, 0x6B($sp)
    /* 1B028 8001A428 01664021 */  addu       $t0, $t3, $a2
    /* 1B02C 8001A42C 29010080 */  slti       $at, $t0, 0x80
    /* 1B030 8001A430 14200003 */  bnez       $at, .L8001A440
    /* 1B034 8001A434 31580080 */   andi      $t8, $t2, 0x80
    /* 1B038 8001A438 10000006 */  b          .L8001A454
    /* 1B03C 8001A43C 2408007F */   addiu     $t0, $zero, 0x7F
  .L8001A440:
    /* 1B040 8001A440 05010003 */  bgez       $t0, .L8001A450
    /* 1B044 8001A444 01001825 */   or        $v1, $t0, $zero
    /* 1B048 8001A448 10000001 */  b          .L8001A450
    /* 1B04C 8001A44C 00001825 */   or        $v1, $zero, $zero
  .L8001A450:
    /* 1B050 8001A450 00604025 */  or         $t0, $v1, $zero
  .L8001A454:
    /* 1B054 8001A454 80810004 */  lb         $at, 0x4($a0)
    /* 1B058 8001A458 908C0005 */  lbu        $t4, 0x5($a0)
    /* 1B05C 8001A45C 03083025 */  or         $a2, $t8, $t0
    /* 1B060 8001A460 00010A00 */  sll        $at, $at, 8
    /* 1B064 8001A464 01816025 */  or         $t4, $t4, $at
    /* 1B068 8001A468 00AC2821 */  addu       $a1, $a1, $t4
    /* 1B06C 8001A46C 28A10100 */  slti       $at, $a1, 0x100
    /* 1B070 8001A470 14200003 */  bnez       $at, .L8001A480
    /* 1B074 8001A474 30C600FF */   andi      $a2, $a2, 0xFF
    /* 1B078 8001A478 10000006 */  b          .L8001A494
    /* 1B07C 8001A47C 240500FF */   addiu     $a1, $zero, 0xFF
  .L8001A480:
    /* 1B080 8001A480 04A10003 */  bgez       $a1, .L8001A490
    /* 1B084 8001A484 00A01825 */   or        $v1, $a1, $zero
    /* 1B088 8001A488 10000001 */  b          .L8001A490
    /* 1B08C 8001A48C 00001825 */   or        $v1, $zero, $zero
  .L8001A490:
    /* 1B090 8001A490 00602825 */  or         $a1, $v1, $zero
  .L8001A494:
    /* 1B094 8001A494 00056A00 */  sll        $t5, $a1, 8
    /* 1B098 8001A498 020D8025 */  or         $s0, $s0, $t5
    /* 1B09C 8001A49C 15E00025 */  bnez       $t7, .L8001A534
    /* 1B0A0 8001A4A0 020E8025 */   or        $s0, $s0, $t6
    /* 1B0A4 8001A4A4 310400FF */  andi       $a0, $t0, 0xFF
    /* 1B0A8 8001A4A8 93A50073 */  lbu        $a1, 0x73($sp)
    /* 1B0AC 8001A4AC 93A60077 */  lbu        $a2, 0x77($sp)
    /* 1B0B0 8001A4B0 AFA80048 */  sw         $t0, 0x48($sp)
    /* 1B0B4 8001A4B4 A3A9006F */  sb         $t1, 0x6F($sp)
    /* 1B0B8 8001A4B8 0C0067B4 */  jal        func_80019ED0
    /* 1B0BC 8001A4BC AFAA0040 */   sw        $t2, 0x40($sp)
    /* 1B0C0 8001A4C0 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1B0C4 8001A4C4 8FA80048 */  lw         $t0, 0x48($sp)
    /* 1B0C8 8001A4C8 93A9006F */  lbu        $t1, 0x6F($sp)
    /* 1B0CC 8001A4CC 10410003 */  beq        $v0, $at, .L8001A4DC
    /* 1B0D0 8001A4D0 8FAA0040 */   lw        $t2, 0x40($sp)
    /* 1B0D4 8001A4D4 1000003C */  b          .L8001A5C8
    /* 1B0D8 8001A4D8 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8001A4DC:
    /* 1B0DC 8001A4DC 93B90073 */  lbu        $t9, 0x73($sp)
    /* 1B0E0 8001A4E0 93AB0077 */  lbu        $t3, 0x77($sp)
    /* 1B0E4 8001A4E4 97AC007A */  lhu        $t4, 0x7A($sp)
    /* 1B0E8 8001A4E8 97AD007E */  lhu        $t5, 0x7E($sp)
    /* 1B0EC 8001A4EC 93AF0083 */  lbu        $t7, 0x83($sp)
    /* 1B0F0 8001A4F0 31580080 */  andi       $t8, $t2, 0x80
    /* 1B0F4 8001A4F4 03083025 */  or         $a2, $t8, $t0
    /* 1B0F8 8001A4F8 240E0001 */  addiu      $t6, $zero, 0x1
    /* 1B0FC 8001A4FC AFAE0024 */  sw         $t6, 0x24($sp)
    /* 1B100 8001A500 30C600FF */  andi       $a2, $a2, 0xFF
    /* 1B104 8001A504 02002025 */  or         $a0, $s0, $zero
    /* 1B108 8001A508 97A5005A */  lhu        $a1, 0x5A($sp)
    /* 1B10C 8001A50C 93A7006B */  lbu        $a3, 0x6B($sp)
    /* 1B110 8001A510 AFA90010 */  sw         $t1, 0x10($sp)
    /* 1B114 8001A514 AFB90014 */  sw         $t9, 0x14($sp)
    /* 1B118 8001A518 AFAB0018 */  sw         $t3, 0x18($sp)
    /* 1B11C 8001A51C AFAC001C */  sw         $t4, 0x1C($sp)
    /* 1B120 8001A520 AFAD0020 */  sw         $t5, 0x20($sp)
    /* 1B124 8001A524 0C009262 */  jal        func_80024988
    /* 1B128 8001A528 AFAF0028 */   sw        $t7, 0x28($sp)
    /* 1B12C 8001A52C 10000026 */  b          .L8001A5C8
    /* 1B130 8001A530 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8001A534:
    /* 1B134 8001A534 93B90073 */  lbu        $t9, 0x73($sp)
    /* 1B138 8001A538 93AB0077 */  lbu        $t3, 0x77($sp)
    /* 1B13C 8001A53C 97AC007A */  lhu        $t4, 0x7A($sp)
    /* 1B140 8001A540 97AD007E */  lhu        $t5, 0x7E($sp)
    /* 1B144 8001A544 93AE0083 */  lbu        $t6, 0x83($sp)
    /* 1B148 8001A548 02002025 */  or         $a0, $s0, $zero
    /* 1B14C 8001A54C 97A5005A */  lhu        $a1, 0x5A($sp)
    /* 1B150 8001A550 AFA90010 */  sw         $t1, 0x10($sp)
    /* 1B154 8001A554 AFB90014 */  sw         $t9, 0x14($sp)
    /* 1B158 8001A558 AFAB0018 */  sw         $t3, 0x18($sp)
    /* 1B15C 8001A55C AFAC001C */  sw         $t4, 0x1C($sp)
    /* 1B160 8001A560 AFAD0020 */  sw         $t5, 0x20($sp)
    /* 1B164 8001A564 0C0067D2 */  jal        func_80019F48
    /* 1B168 8001A568 AFAE0024 */   sw        $t6, 0x24($sp)
    /* 1B16C 8001A56C 10000016 */  b          .L8001A5C8
    /* 1B170 8001A570 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8001A574:
    /* 1B174 8001A574 93AF006F */  lbu        $t7, 0x6F($sp)
    /* 1B178 8001A578 93B80073 */  lbu        $t8, 0x73($sp)
    /* 1B17C 8001A57C 93B90077 */  lbu        $t9, 0x77($sp)
    /* 1B180 8001A580 97AB007A */  lhu        $t3, 0x7A($sp)
    /* 1B184 8001A584 97AC007E */  lhu        $t4, 0x7E($sp)
    /* 1B188 8001A588 93AD0083 */  lbu        $t5, 0x83($sp)
    /* 1B18C 8001A58C 00102C02 */  srl        $a1, $s0, 16
    /* 1B190 8001A590 30A5FFFF */  andi       $a1, $a1, 0xFFFF
    /* 1B194 8001A594 93A60067 */  lbu        $a2, 0x67($sp)
    /* 1B198 8001A598 93A7006B */  lbu        $a3, 0x6B($sp)
    /* 1B19C 8001A59C AFAF0010 */  sw         $t7, 0x10($sp)
    /* 1B1A0 8001A5A0 AFB80014 */  sw         $t8, 0x14($sp)
    /* 1B1A4 8001A5A4 AFB90018 */  sw         $t9, 0x18($sp)
    /* 1B1A8 8001A5A8 AFAB001C */  sw         $t3, 0x1C($sp)
    /* 1B1AC 8001A5AC AFAC0020 */  sw         $t4, 0x20($sp)
    /* 1B1B0 8001A5B0 0C0067D2 */  jal        func_80019F48
    /* 1B1B4 8001A5B4 AFAD0024 */   sw        $t5, 0x24($sp)
    /* 1B1B8 8001A5B8 10000003 */  b          .L8001A5C8
    /* 1B1BC 8001A5BC 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8001A5C0:
    /* 1B1C0 8001A5C0 2402FFFF */  addiu      $v0, $zero, -0x1
  .L8001A5C4:
    /* 1B1C4 8001A5C4 8FBF003C */  lw         $ra, 0x3C($sp)
  .L8001A5C8:
    /* 1B1C8 8001A5C8 8FB00038 */  lw         $s0, 0x38($sp)
    /* 1B1CC 8001A5CC 27BD0060 */  addiu      $sp, $sp, 0x60
    /* 1B1D0 8001A5D0 03E00008 */  jr         $ra
    /* 1B1D4 8001A5D4 00000000 */   nop
endlabel func_8001A270
