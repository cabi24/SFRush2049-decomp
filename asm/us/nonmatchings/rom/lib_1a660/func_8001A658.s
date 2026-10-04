nonmatching func_8001A658, 0xAFC

glabel func_8001A658
    /* 1B258 8001A658 27BDFF78 */  addiu      $sp, $sp, -0x88
    /* 1B25C 8001A65C 3C0E8005 */  lui        $t6, %hi(D_8004FA18)
    /* 1B260 8001A660 91CEFA18 */  lbu        $t6, %lo(D_8004FA18)($t6)
    /* 1B264 8001A664 AFB20028 */  sw         $s2, 0x28($sp)
    /* 1B268 8001A668 AFB60038 */  sw         $s6, 0x38($sp)
    /* 1B26C 8001A66C 3C128005 */  lui        $s2, %hi(D_8004BEB8)
    /* 1B270 8001A670 AFBF0044 */  sw         $ra, 0x44($sp)
    /* 1B274 8001A674 AFBE0040 */  sw         $fp, 0x40($sp)
    /* 1B278 8001A678 AFB7003C */  sw         $s7, 0x3C($sp)
    /* 1B27C 8001A67C AFB50034 */  sw         $s5, 0x34($sp)
    /* 1B280 8001A680 AFB40030 */  sw         $s4, 0x30($sp)
    /* 1B284 8001A684 AFB3002C */  sw         $s3, 0x2C($sp)
    /* 1B288 8001A688 AFB10024 */  sw         $s1, 0x24($sp)
    /* 1B28C 8001A68C AFB00020 */  sw         $s0, 0x20($sp)
    /* 1B290 8001A690 2652BEB8 */  addiu      $s2, $s2, %lo(D_8004BEB8)
    /* 1B294 8001A694 19C0025A */  blez       $t6, .L8001B000
    /* 1B298 8001A698 0000B025 */   or        $s6, $zero, $zero
    /* 1B29C 8001A69C 3C17007F */  lui        $s7, (0x7F0001 >> 16)
    /* 1B2A0 8001A6A0 3C138005 */  lui        $s3, %hi(D_8004BE90)
    /* 1B2A4 8001A6A4 2673BE90 */  addiu      $s3, $s3, %lo(D_8004BE90)
    /* 1B2A8 8001A6A8 36F70001 */  ori        $s7, $s7, (0x7F0001 & 0xFFFF)
    /* 1B2AC 8001A6AC 3C1E007F */  lui        $fp, (0x7F0000 >> 16)
    /* 1B2B0 8001A6B0 241500FF */  addiu      $s5, $zero, 0xFF
    /* 1B2B4 8001A6B4 24140002 */  addiu      $s4, $zero, 0x2
    /* 1B2B8 8001A6B8 8A4F0000 */  lwl        $t7, 0x0($s2)
  .L8001A6BC:
    /* 1B2BC 8001A6BC 9A4F0003 */  lwr        $t7, 0x3($s2)
    /* 1B2C0 8001A6C0 11E00005 */  beqz       $t7, .L8001A6D8
    /* 1B2C4 8001A6C4 00000000 */   nop
    /* 1B2C8 8001A6C8 0C008FA7 */  jal        func_80023E9C
    /* 1B2CC 8001A6CC 02402025 */   or        $a0, $s2, $zero
    /* 1B2D0 8001A6D0 10000006 */  b          .L8001A6EC
    /* 1B2D4 8001A6D4 925800BD */   lbu       $t8, 0xBD($s2)
  .L8001A6D8:
    /* 1B2D8 8001A6D8 0C00519F */  jal        func_8001467C
    /* 1B2DC 8001A6DC 02C02025 */   or        $a0, $s6, $zero
    /* 1B2E0 8001A6E0 10400240 */  beqz       $v0, .L8001AFE4
    /* 1B2E4 8001A6E4 00000000 */   nop
    /* 1B2E8 8001A6E8 925800BD */  lbu        $t8, 0xBD($s2)
  .L8001A6EC:
    /* 1B2EC 8001A6EC 00002025 */  or         $a0, $zero, $zero
    /* 1B2F0 8001A6F0 00008825 */  or         $s1, $zero, $zero
    /* 1B2F4 8001A6F4 1700023B */  bnez       $t8, .L8001AFE4
    /* 1B2F8 8001A6F8 02408025 */   or        $s0, $s2, $zero
  .L8001A6FC:
    /* 1B2FC 8001A6FC 8A02016C */  lwl        $v0, 0x16C($s0)
    /* 1B300 8001A700 9A02016F */  lwr        $v0, 0x16F($s0)
    /* 1B304 8001A704 5040001C */  beql       $v0, $zero, .L8001A778
    /* 1B308 8001A708 2631000C */   addiu     $s1, $s1, 0xC
    /* 1B30C 8001A70C 8A190168 */  lwl        $t9, 0x168($s0)
    /* 1B310 8001A710 8E680000 */  lw         $t0, 0x0($s3)
    /* 1B314 8001A714 9A19016B */  lwr        $t9, 0x16B($s0)
    /* 1B318 8001A718 00026A02 */  srl        $t5, $v0, 8
    /* 1B31C 8001A71C 03284821 */  addu       $t1, $t9, $t0
    /* 1B320 8001A720 AA090168 */  swl        $t1, 0x168($s0)
    /* 1B324 8001A724 BA09016B */  swr        $t1, 0x16B($s0)
    /* 1B328 8001A728 8A0A0168 */  lwl        $t2, 0x168($s0)
    /* 1B32C 8001A72C 9A0A016B */  lwr        $t2, 0x16B($s0)
    /* 1B330 8001A730 0142001B */  divu       $zero, $t2, $v0
    /* 1B334 8001A734 00005810 */  mfhi       $t3
    /* 1B338 8001A738 000B6100 */  sll        $t4, $t3, 4
    /* 1B33C 8001A73C 14400002 */  bnez       $v0, .L8001A748
    /* 1B340 8001A740 00000000 */   nop
    /* 1B344 8001A744 0007000D */  break      7
  .L8001A748:
    /* 1B348 8001A748 018D001B */  divu       $zero, $t4, $t5
    /* 1B34C 8001A74C 00002012 */  mflo       $a0
    /* 1B350 8001A750 3084FFFF */  andi       $a0, $a0, 0xFFFF
    /* 1B354 8001A754 15A00002 */  bnez       $t5, .L8001A760
    /* 1B358 8001A758 00000000 */   nop
    /* 1B35C 8001A75C 0007000D */  break      7
  .L8001A760:
    /* 1B360 8001A760 0C0079EF */  jal        func_8001E7BC
    /* 1B364 8001A764 00000000 */   nop
    /* 1B368 8001A768 00020A02 */  srl        $at, $v0, 8
    /* 1B36C 8001A76C A2010170 */  sb         $at, 0x170($s0)
    /* 1B370 8001A770 A2020171 */  sb         $v0, 0x171($s0)
    /* 1B374 8001A774 2631000C */  addiu      $s1, $s1, 0xC
  .L8001A778:
    /* 1B378 8001A778 2A210018 */  slti       $at, $s1, 0x18
    /* 1B37C 8001A77C 1420FFDF */  bnez       $at, .L8001A6FC
    /* 1B380 8001A780 2610000C */   addiu     $s0, $s0, 0xC
    /* 1B384 8001A784 8A4E0024 */  lwl        $t6, 0x24($s2)
    /* 1B388 8001A788 9A4E0027 */  lwr        $t6, 0x27($s2)
    /* 1B38C 8001A78C 31CF4000 */  andi       $t7, $t6, 0x4000
    /* 1B390 8001A790 51E0001D */  beql       $t7, $zero, .L8001A808
    /* 1B394 8001A794 00002025 */   or        $a0, $zero, $zero
    /* 1B398 8001A798 8A58006C */  lwl        $t8, 0x6C($s2)
    /* 1B39C 8001A79C 8E790000 */  lw         $t9, 0x0($s3)
    /* 1B3A0 8001A7A0 9A58006F */  lwr        $t8, 0x6F($s2)
    /* 1B3A4 8001A7A4 8A420070 */  lwl        $v0, 0x70($s2)
    /* 1B3A8 8001A7A8 9A420073 */  lwr        $v0, 0x73($s2)
    /* 1B3AC 8001A7AC 03194021 */  addu       $t0, $t8, $t9
    /* 1B3B0 8001A7B0 AA48006C */  swl        $t0, 0x6C($s2)
    /* 1B3B4 8001A7B4 BA48006F */  swr        $t0, 0x6F($s2)
    /* 1B3B8 8001A7B8 8A49006C */  lwl        $t1, 0x6C($s2)
    /* 1B3BC 8001A7BC 9A49006F */  lwr        $t1, 0x6F($s2)
    /* 1B3C0 8001A7C0 00026202 */  srl        $t4, $v0, 8
    /* 1B3C4 8001A7C4 0122001B */  divu       $zero, $t1, $v0
    /* 1B3C8 8001A7C8 00005010 */  mfhi       $t2
    /* 1B3CC 8001A7CC 000A5900 */  sll        $t3, $t2, 4
    /* 1B3D0 8001A7D0 14400002 */  bnez       $v0, .L8001A7DC
    /* 1B3D4 8001A7D4 00000000 */   nop
    /* 1B3D8 8001A7D8 0007000D */  break      7
  .L8001A7DC:
    /* 1B3DC 8001A7DC 016C001B */  divu       $zero, $t3, $t4
    /* 1B3E0 8001A7E0 00002012 */  mflo       $a0
    /* 1B3E4 8001A7E4 3084FFFF */  andi       $a0, $a0, 0xFFFF
    /* 1B3E8 8001A7E8 15800002 */  bnez       $t4, .L8001A7F4
    /* 1B3EC 8001A7EC 00000000 */   nop
    /* 1B3F0 8001A7F0 0007000D */  break      7
  .L8001A7F4:
    /* 1B3F4 8001A7F4 0C0079EF */  jal        func_8001E7BC
    /* 1B3F8 8001A7F8 00000000 */   nop
    /* 1B3FC 8001A7FC AA420074 */  swl        $v0, 0x74($s2)
    /* 1B400 8001A800 BA420077 */  swr        $v0, 0x77($s2)
    /* 1B404 8001A804 00002025 */  or         $a0, $zero, $zero
  .L8001A808:
    /* 1B408 8001A808 02401025 */  or         $v0, $s2, $zero
  .L8001A80C:
    /* 1B40C 8001A80C 904D0048 */  lbu        $t5, 0x48($v0)
    /* 1B410 8001A810 51A00011 */  beql       $t5, $zero, .L8001A858
    /* 1B414 8001A814 24840001 */   addiu     $a0, $a0, 0x1
    /* 1B418 8001A818 904E00B0 */  lbu        $t6, 0xB0($v0)
    /* 1B41C 8001A81C 0004C080 */  sll        $t8, $a0, 2
    /* 1B420 8001A820 02581821 */  addu       $v1, $s2, $t8
    /* 1B424 8001A824 25CFFFFF */  addiu      $t7, $t6, -0x1
    /* 1B428 8001A828 31F900FF */  andi       $t9, $t7, 0xFF
    /* 1B42C 8001A82C 17200005 */  bnez       $t9, .L8001A844
    /* 1B430 8001A830 A04F00B0 */   sb        $t7, 0xB0($v0)
    /* 1B434 8001A834 90480048 */  lbu        $t0, 0x48($v0)
    /* 1B438 8001A838 A04800B0 */  sb         $t0, 0xB0($v0)
    /* 1B43C 8001A83C 10000005 */  b          .L8001A854
    /* 1B440 8001A840 AC600040 */   sw        $zero, 0x40($v1)
  .L8001A844:
    /* 1B444 8001A844 8C690040 */  lw         $t1, 0x40($v1)
    /* 1B448 8001A848 8C6A007C */  lw         $t2, 0x7C($v1)
    /* 1B44C 8001A84C 012A5821 */  addu       $t3, $t1, $t2
    /* 1B450 8001A850 AC6B0040 */  sw         $t3, 0x40($v1)
  .L8001A854:
    /* 1B454 8001A854 24840001 */  addiu      $a0, $a0, 0x1
  .L8001A858:
    /* 1B458 8001A858 1494FFEC */  bne        $a0, $s4, .L8001A80C
    /* 1B45C 8001A85C 24420001 */   addiu     $v0, $v0, 0x1
    /* 1B460 8001A860 924C004A */  lbu        $t4, 0x4A($s2)
    /* 1B464 8001A864 52AC0016 */  beql       $s5, $t4, .L8001A8C0
    /* 1B468 8001A868 8A590024 */   lwl       $t9, 0x24($s2)
    /* 1B46C 8001A86C 0C00853A */  jal        func_800214E8
    /* 1B470 8001A870 02402025 */   or        $a0, $s2, $zero
    /* 1B474 8001A874 28411F81 */  slti       $at, $v0, 0x1F81
    /* 1B478 8001A878 5020000A */  beql       $at, $zero, .L8001A8A4
    /* 1B47C 8001A87C 8A4F0024 */   lwl       $t7, 0x24($s2)
    /* 1B480 8001A880 8A4D0024 */  lwl        $t5, 0x24($s2)
    /* 1B484 8001A884 9A4D0027 */  lwr        $t5, 0x27($s2)
    /* 1B488 8001A888 3C01BFFF */  lui        $at, (0xBFFFFFFF >> 16)
    /* 1B48C 8001A88C 3421FFFF */  ori        $at, $at, (0xBFFFFFFF & 0xFFFF)
    /* 1B490 8001A890 01A17024 */  and        $t6, $t5, $at
    /* 1B494 8001A894 AA4E0024 */  swl        $t6, 0x24($s2)
    /* 1B498 8001A898 1000000F */  b          .L8001A8D8
    /* 1B49C 8001A89C BA4E0027 */   swr       $t6, 0x27($s2)
    /* 1B4A0 8001A8A0 8A4F0024 */  lwl        $t7, 0x24($s2)
  .L8001A8A4:
    /* 1B4A4 8001A8A4 9A4F0027 */  lwr        $t7, 0x27($s2)
    /* 1B4A8 8001A8A8 3C014000 */  lui        $at, (0x40000000 >> 16)
    /* 1B4AC 8001A8AC 01E1C025 */  or         $t8, $t7, $at
    /* 1B4B0 8001A8B0 AA580024 */  swl        $t8, 0x24($s2)
    /* 1B4B4 8001A8B4 10000008 */  b          .L8001A8D8
    /* 1B4B8 8001A8B8 BA580027 */   swr       $t8, 0x27($s2)
    /* 1B4BC 8001A8BC 8A590024 */  lwl        $t9, 0x24($s2)
  .L8001A8C0:
    /* 1B4C0 8001A8C0 9A590027 */  lwr        $t9, 0x27($s2)
    /* 1B4C4 8001A8C4 3C01BFFF */  lui        $at, (0xBFFFFFFF >> 16)
    /* 1B4C8 8001A8C8 3421FFFF */  ori        $at, $at, (0xBFFFFFFF & 0xFFFF)
    /* 1B4CC 8001A8CC 03214024 */  and        $t0, $t9, $at
    /* 1B4D0 8001A8D0 AA480024 */  swl        $t0, 0x24($s2)
    /* 1B4D4 8001A8D4 BA480027 */  swr        $t0, 0x27($s2)
  .L8001A8D8:
    /* 1B4D8 8001A8D8 8A500024 */  lwl        $s0, 0x24($s2)
    /* 1B4DC 8001A8DC 9A500027 */  lwr        $s0, 0x27($s2)
    /* 1B4E0 8001A8E0 32091000 */  andi       $t1, $s0, 0x1000
    /* 1B4E4 8001A8E4 55200029 */  bnel       $t1, $zero, .L8001A98C
    /* 1B4E8 8001A8E8 32180020 */   andi      $t8, $s0, 0x20
    /* 1B4EC 8001A8EC 924A004A */  lbu        $t2, 0x4A($s2)
    /* 1B4F0 8001A8F0 52AA0021 */  beql       $s5, $t2, .L8001A978
    /* 1B4F4 8001A8F4 360F2000 */   ori       $t7, $s0, 0x2000
    /* 1B4F8 8001A8F8 0C008542 */  jal        func_80021508
    /* 1B4FC 8001A8FC 02402025 */   or        $a0, $s2, $zero
    /* 1B500 8001A900 28411F81 */  slti       $at, $v0, 0x1F81
    /* 1B504 8001A904 1020000D */  beqz       $at, .L8001A93C
    /* 1B508 8001A908 00003025 */   or        $a2, $zero, $zero
    /* 1B50C 8001A90C 8A4B0024 */  lwl        $t3, 0x24($s2)
    /* 1B510 8001A910 9A4B0027 */  lwr        $t3, 0x27($s2)
    /* 1B514 8001A914 2401F7FF */  addiu      $at, $zero, -0x801
    /* 1B518 8001A918 9244004A */  lbu        $a0, 0x4A($s2)
    /* 1B51C 8001A91C 01616024 */  and        $t4, $t3, $at
    /* 1B520 8001A920 AA4C0024 */  swl        $t4, 0x24($s2)
    /* 1B524 8001A924 BA4C0027 */  swr        $t4, 0x27($s2)
    /* 1B528 8001A928 0C0083F7 */  jal        func_80020FDC
    /* 1B52C 8001A92C 9245004B */   lbu       $a1, 0x4B($s2)
    /* 1B530 8001A930 8A500024 */  lwl        $s0, 0x24($s2)
    /* 1B534 8001A934 1000000F */  b          .L8001A974
    /* 1B538 8001A938 9A500027 */   lwr       $s0, 0x27($s2)
  .L8001A93C:
    /* 1B53C 8001A93C 8A500024 */  lwl        $s0, 0x24($s2)
    /* 1B540 8001A940 9A500027 */  lwr        $s0, 0x27($s2)
    /* 1B544 8001A944 320D0800 */  andi       $t5, $s0, 0x800
    /* 1B548 8001A948 55A00006 */  bnel       $t5, $zero, .L8001A964
    /* 1B54C 8001A94C 360E0800 */   ori       $t6, $s0, 0x800
    /* 1B550 8001A950 0C0066F9 */  jal        func_80019BE4
    /* 1B554 8001A954 02402025 */   or        $a0, $s2, $zero
    /* 1B558 8001A958 8A500024 */  lwl        $s0, 0x24($s2)
    /* 1B55C 8001A95C 9A500027 */  lwr        $s0, 0x27($s2)
    /* 1B560 8001A960 360E0800 */  ori        $t6, $s0, 0x800
  .L8001A964:
    /* 1B564 8001A964 AA4E0024 */  swl        $t6, 0x24($s2)
    /* 1B568 8001A968 BA4E0027 */  swr        $t6, 0x27($s2)
    /* 1B56C 8001A96C 8A500024 */  lwl        $s0, 0x24($s2)
    /* 1B570 8001A970 9A500027 */  lwr        $s0, 0x27($s2)
  .L8001A974:
    /* 1B574 8001A974 360F2000 */  ori        $t7, $s0, 0x2000
  .L8001A978:
    /* 1B578 8001A978 AA4F0024 */  swl        $t7, 0x24($s2)
    /* 1B57C 8001A97C BA4F0027 */  swr        $t7, 0x27($s2)
    /* 1B580 8001A980 8A500024 */  lwl        $s0, 0x24($s2)
    /* 1B584 8001A984 9A500027 */  lwr        $s0, 0x27($s2)
    /* 1B588 8001A988 32180020 */  andi       $t8, $s0, 0x20
  .L8001A98C:
    /* 1B58C 8001A98C 1300000D */  beqz       $t8, .L8001A9C4
    /* 1B590 8001A990 2401FFDF */   addiu     $at, $zero, -0x21
    /* 1B594 8001A994 0201C824 */  and        $t9, $s0, $at
    /* 1B598 8001A998 AA590024 */  swl        $t9, 0x24($s2)
    /* 1B59C 8001A99C BA590027 */  swr        $t9, 0x27($s2)
    /* 1B5A0 8001A9A0 8A480024 */  lwl        $t0, 0x24($s2)
    /* 1B5A4 8001A9A4 9A480027 */  lwr        $t0, 0x27($s2)
    /* 1B5A8 8001A9A8 02C02025 */  or         $a0, $s6, $zero
    /* 1B5AC 8001A9AC 35090010 */  ori        $t1, $t0, 0x10
    /* 1B5B0 8001A9B0 AA490024 */  swl        $t1, 0x24($s2)
    /* 1B5B4 8001A9B4 0C00526F */  jal        func_800149BC
    /* 1B5B8 8001A9B8 BA490027 */   swr       $t1, 0x27($s2)
    /* 1B5BC 8001A9BC 8A500024 */  lwl        $s0, 0x24($s2)
    /* 1B5C0 8001A9C0 9A500027 */  lwr        $s0, 0x27($s2)
  .L8001A9C4:
    /* 1B5C4 8001A9C4 320A0080 */  andi       $t2, $s0, 0x80
    /* 1B5C8 8001A9C8 11400008 */  beqz       $t2, .L8001A9EC
    /* 1B5CC 8001A9CC 00105840 */   sll       $t3, $s0, 1
    /* 1B5D0 8001A9D0 05600006 */  bltz       $t3, .L8001A9EC
    /* 1B5D4 8001A9D4 2401FF6F */   addiu     $at, $zero, -0x91
    /* 1B5D8 8001A9D8 02016024 */  and        $t4, $s0, $at
    /* 1B5DC 8001A9DC AA4C0024 */  swl        $t4, 0x24($s2)
    /* 1B5E0 8001A9E0 BA4C0027 */  swr        $t4, 0x27($s2)
    /* 1B5E4 8001A9E4 0C005277 */  jal        func_800149DC
    /* 1B5E8 8001A9E8 02C02025 */   or        $a0, $s6, $zero
  .L8001A9EC:
    /* 1B5EC 8001A9EC 8A420028 */  lwl        $v0, 0x28($s2)
    /* 1B5F0 8001A9F0 9A42002B */  lwr        $v0, 0x2B($s2)
    /* 1B5F4 8001A9F4 50400012 */  beql       $v0, $zero, .L8001AA40
    /* 1B5F8 8001A9F8 8A480024 */   lwl       $t0, 0x24($s2)
    /* 1B5FC 8001A9FC 9241002C */  lbu        $at, 0x2C($s2)
    /* 1B600 8001AA00 924D002D */  lbu        $t5, 0x2D($s2)
    /* 1B604 8001AA04 8E6E0000 */  lw         $t6, 0x0($s3)
    /* 1B608 8001AA08 00010A00 */  sll        $at, $at, 8
    /* 1B60C 8001AA0C 01A16825 */  or         $t5, $t5, $at
    /* 1B610 8001AA10 01AE0019 */  multu      $t5, $t6
    /* 1B614 8001AA14 00007812 */  mflo       $t7
    /* 1B618 8001AA18 004FC023 */  subu       $t8, $v0, $t7
    /* 1B61C 8001AA1C AA580028 */  swl        $t8, 0x28($s2)
    /* 1B620 8001AA20 BA58002B */  swr        $t8, 0x2B($s2)
    /* 1B624 8001AA24 8A590028 */  lwl        $t9, 0x28($s2)
    /* 1B628 8001AA28 9A59002B */  lwr        $t9, 0x2B($s2)
    /* 1B62C 8001AA2C 07230004 */  bgezl      $t9, .L8001AA40
    /* 1B630 8001AA30 8A480024 */   lwl       $t0, 0x24($s2)
    /* 1B634 8001AA34 AA400028 */  swl        $zero, 0x28($s2)
    /* 1B638 8001AA38 BA40002B */  swr        $zero, 0x2B($s2)
    /* 1B63C 8001AA3C 8A480024 */  lwl        $t0, 0x24($s2)
  .L8001AA40:
    /* 1B640 8001AA40 9A480027 */  lwr        $t0, 0x27($s2)
    /* 1B644 8001AA44 00084BC0 */  sll        $t1, $t0, 15
    /* 1B648 8001AA48 05230032 */  bgezl      $t1, .L8001AB14
    /* 1B64C 8001AA4C 92410050 */   lbu       $at, 0x50($s2)
    /* 1B650 8001AA50 8A4A00A8 */  lwl        $t2, 0xA8($s2)
    /* 1B654 8001AA54 9A4A00AB */  lwr        $t2, 0xAB($s2)
    /* 1B658 8001AA58 8A4C003C */  lwl        $t4, 0x3C($s2)
    /* 1B65C 8001AA5C 9A4C003F */  lwr        $t4, 0x3F($s2)
    /* 1B660 8001AA60 000A5A02 */  srl        $t3, $t2, 8
    /* 1B664 8001AA64 8A4E00AC */  lwl        $t6, 0xAC($s2)
    /* 1B668 8001AA68 016C0019 */  multu      $t3, $t4
    /* 1B66C 8001AA6C 9A4E00AF */  lwr        $t6, 0xAF($s2)
    /* 1B670 8001AA70 00006812 */  mflo       $t5
    /* 1B674 8001AA74 01CD1023 */  subu       $v0, $t6, $t5
    /* 1B678 8001AA78 AA420038 */  swl        $v0, 0x38($s2)
    /* 1B67C 8001AA7C 04410004 */  bgez       $v0, .L8001AA90
    /* 1B680 8001AA80 BA42003B */   swr       $v0, 0x3B($s2)
    /* 1B684 8001AA84 AA400038 */  swl        $zero, 0x38($s2)
    /* 1B688 8001AA88 1000000C */  b          .L8001AABC
    /* 1B68C 8001AA8C BA40003B */   swr       $zero, 0x3B($s2)
  .L8001AA90:
    /* 1B690 8001AA90 8A430038 */  lwl        $v1, 0x38($s2)
    /* 1B694 8001AA94 9A43003B */  lwr        $v1, 0x3B($s2)
    /* 1B698 8001AA98 3C01007F */  lui        $at, (0x7F0001 >> 16)
    /* 1B69C 8001AA9C 34210001 */  ori        $at, $at, (0x7F0001 & 0xFFFF)
    /* 1B6A0 8001AAA0 0061082B */  sltu       $at, $v1, $at
    /* 1B6A4 8001AAA4 14200003 */  bnez       $at, .L8001AAB4
    /* 1B6A8 8001AAA8 00601025 */   or        $v0, $v1, $zero
    /* 1B6AC 8001AAAC 10000001 */  b          .L8001AAB4
    /* 1B6B0 8001AAB0 3C02007F */   lui       $v0, (0x7F0000 >> 16)
  .L8001AAB4:
    /* 1B6B4 8001AAB4 AA420038 */  swl        $v0, 0x38($s2)
    /* 1B6B8 8001AAB8 BA42003B */  swr        $v0, 0x3B($s2)
  .L8001AABC:
    /* 1B6BC 8001AABC 8A4F00A8 */  lwl        $t7, 0xA8($s2)
    /* 1B6C0 8001AAC0 8E780000 */  lw         $t8, 0x0($s3)
    /* 1B6C4 8001AAC4 9A4F00AB */  lwr        $t7, 0xAB($s2)
    /* 1B6C8 8001AAC8 3C01FFFE */  lui        $at, (0xFFFEFFFF >> 16)
    /* 1B6CC 8001AACC 3421FFFF */  ori        $at, $at, (0xFFFEFFFF & 0xFFFF)
    /* 1B6D0 8001AAD0 01F8C823 */  subu       $t9, $t7, $t8
    /* 1B6D4 8001AAD4 AA5900A8 */  swl        $t9, 0xA8($s2)
    /* 1B6D8 8001AAD8 BA5900AB */  swr        $t9, 0xAB($s2)
    /* 1B6DC 8001AADC 8A4800A8 */  lwl        $t0, 0xA8($s2)
    /* 1B6E0 8001AAE0 9A4800AB */  lwr        $t0, 0xAB($s2)
    /* 1B6E4 8001AAE4 5500000B */  bnel       $t0, $zero, .L8001AB14
    /* 1B6E8 8001AAE8 92410050 */   lbu       $at, 0x50($s2)
    /* 1B6EC 8001AAEC 8A490024 */  lwl        $t1, 0x24($s2)
    /* 1B6F0 8001AAF0 9A490027 */  lwr        $t1, 0x27($s2)
    /* 1B6F4 8001AAF4 8A4B00AC */  lwl        $t3, 0xAC($s2)
    /* 1B6F8 8001AAF8 9A4B00AF */  lwr        $t3, 0xAF($s2)
    /* 1B6FC 8001AAFC 01215024 */  and        $t2, $t1, $at
    /* 1B700 8001AB00 AA4A0024 */  swl        $t2, 0x24($s2)
    /* 1B704 8001AB04 AA4B0038 */  swl        $t3, 0x38($s2)
    /* 1B708 8001AB08 BA4A0027 */  swr        $t2, 0x27($s2)
    /* 1B70C 8001AB0C BA4B003B */  swr        $t3, 0x3B($s2)
    /* 1B710 8001AB10 92410050 */  lbu        $at, 0x50($s2)
  .L8001AB14:
    /* 1B714 8001AB14 924C0051 */  lbu        $t4, 0x51($s2)
    /* 1B718 8001AB18 824D00C0 */  lb         $t5, 0xC0($s2)
    /* 1B71C 8001AB1C 00010A00 */  sll        $at, $at, 8
    /* 1B720 8001AB20 01816025 */  or         $t4, $t4, $at
    /* 1B724 8001AB24 24010064 */  addiu      $at, $zero, 0x64
    /* 1B728 8001AB28 000D7C00 */  sll        $t7, $t5, 16
    /* 1B72C 8001AB2C 01E1001A */  div        $zero, $t7, $at
    /* 1B730 8001AB30 8A500024 */  lwl        $s0, 0x24($s2)
    /* 1B734 8001AB34 9A500027 */  lwr        $s0, 0x27($s2)
    /* 1B738 8001AB38 3C010004 */  lui        $at, (0x40010 >> 16)
    /* 1B73C 8001AB3C 34210010 */  ori        $at, $at, (0x40010 & 0xFFFF)
    /* 1B740 8001AB40 0000C012 */  mflo       $t8
    /* 1B744 8001AB44 000C7400 */  sll        $t6, $t4, 16
    /* 1B748 8001AB48 0201C824 */  and        $t9, $s0, $at
    /* 1B74C 8001AB4C 1320000D */  beqz       $t9, .L8001AB84
    /* 1B750 8001AB50 01D88821 */   addu      $s1, $t6, $t8
    /* 1B754 8001AB54 9248004A */  lbu        $t0, 0x4A($s2)
    /* 1B758 8001AB58 52A8001F */  beql       $s5, $t0, .L8001ABD8
    /* 1B75C 8001AB5C 320E4000 */   andi      $t6, $s0, 0x4000
    /* 1B760 8001AB60 0C008522 */  jal        func_80021488
    /* 1B764 8001AB64 02402025 */   or        $a0, $s2, $zero
    /* 1B768 8001AB68 00020A02 */  srl        $at, $v0, 8
    /* 1B76C 8001AB6C 8A500024 */  lwl        $s0, 0x24($s2)
    /* 1B770 8001AB70 00402025 */  or         $a0, $v0, $zero
    /* 1B774 8001AB74 A24100B8 */  sb         $at, 0xB8($s2)
    /* 1B778 8001AB78 A24200B9 */  sb         $v0, 0xB9($s2)
    /* 1B77C 8001AB7C 10000005 */  b          .L8001AB94
    /* 1B780 8001AB80 9A500027 */   lwr       $s0, 0x27($s2)
  .L8001AB84:
    /* 1B784 8001AB84 924100B8 */  lbu        $at, 0xB8($s2)
    /* 1B788 8001AB88 924400B9 */  lbu        $a0, 0xB9($s2)
    /* 1B78C 8001AB8C 00010A00 */  sll        $at, $at, 8
    /* 1B790 8001AB90 00812025 */  or         $a0, $a0, $at
  .L8001AB94:
    /* 1B794 8001AB94 24012000 */  addiu      $at, $zero, 0x2000
    /* 1B798 8001AB98 1081000E */  beq        $a0, $at, .L8001ABD4
    /* 1B79C 8001AB9C 2484E000 */   addiu     $a0, $a0, -0x2000
    /* 1B7A0 8001ABA0 04830008 */  bgezl      $a0, .L8001ABC4
    /* 1B7A4 8001ABA4 924C006B */   lbu       $t4, 0x6B($s2)
    /* 1B7A8 8001ABA8 9249006A */  lbu        $t1, 0x6A($s2)
    /* 1B7AC 8001ABAC 01240019 */  multu      $t1, $a0
    /* 1B7B0 8001ABB0 00005012 */  mflo       $t2
    /* 1B7B4 8001ABB4 000A58C0 */  sll        $t3, $t2, 3
    /* 1B7B8 8001ABB8 10000006 */  b          .L8001ABD4
    /* 1B7BC 8001ABBC 022B8821 */   addu      $s1, $s1, $t3
    /* 1B7C0 8001ABC0 924C006B */  lbu        $t4, 0x6B($s2)
  .L8001ABC4:
    /* 1B7C4 8001ABC4 01840019 */  multu      $t4, $a0
    /* 1B7C8 8001ABC8 00006812 */  mflo       $t5
    /* 1B7CC 8001ABCC 000D78C0 */  sll        $t7, $t5, 3
    /* 1B7D0 8001ABD0 022F8821 */  addu       $s1, $s1, $t7
  .L8001ABD4:
    /* 1B7D4 8001ABD4 320E4000 */  andi       $t6, $s0, 0x4000
  .L8001ABD8:
    /* 1B7D8 8001ABD8 11C0001F */  beqz       $t6, .L8001AC58
    /* 1B7DC 8001ABDC 24010064 */   addiu     $at, $zero, 0x64
    /* 1B7E0 8001ABE0 92480079 */  lbu        $t0, 0x79($s2)
    /* 1B7E4 8001ABE4 92580078 */  lbu        $t8, 0x78($s2)
    /* 1B7E8 8001ABE8 320B8000 */  andi       $t3, $s0, 0x8000
    /* 1B7EC 8001ABEC 00084A00 */  sll        $t1, $t0, 8
    /* 1B7F0 8001ABF0 0121001A */  div        $zero, $t1, $at
    /* 1B7F4 8001ABF4 00005012 */  mflo       $t2
    /* 1B7F8 8001ABF8 0018CA00 */  sll        $t9, $t8, 8
    /* 1B7FC 8001ABFC 11600010 */  beqz       $t3, .L8001AC40
    /* 1B800 8001AC00 032A2821 */   addu      $a1, $t9, $t2
    /* 1B804 8001AC04 924C004A */  lbu        $t4, 0x4A($s2)
    /* 1B808 8001AC08 02402025 */  or         $a0, $s2, $zero
    /* 1B80C 8001AC0C 52AC000D */  beql       $s5, $t4, .L8001AC44
    /* 1B810 8001AC10 8A430074 */   lwl       $v1, 0x74($s2)
    /* 1B814 8001AC14 0C008532 */  jal        func_800214C8
    /* 1B818 8001AC18 AFA50058 */   sw        $a1, 0x58($sp)
    /* 1B81C 8001AC1C 8A4F0074 */  lwl        $t7, 0x74($s2)
    /* 1B820 8001AC20 9A4F0077 */  lwr        $t7, 0x77($s2)
    /* 1B824 8001AC24 000269C3 */  sra        $t5, $v0, 7
    /* 1B828 8001AC28 8FA50058 */  lw         $a1, 0x58($sp)
    /* 1B82C 8001AC2C 01AF0019 */  multu      $t5, $t7
    /* 1B830 8001AC30 00001812 */  mflo       $v1
    /* 1B834 8001AC34 000319C3 */  sra        $v1, $v1, 7
    /* 1B838 8001AC38 10000003 */  b          .L8001AC48
    /* 1B83C 8001AC3C 00000000 */   nop
  .L8001AC40:
    /* 1B840 8001AC40 8A430074 */  lwl        $v1, 0x74($s2)
  .L8001AC44:
    /* 1B844 8001AC44 9A430077 */  lwr        $v1, 0x77($s2)
  .L8001AC48:
    /* 1B848 8001AC48 00A30019 */  multu      $a1, $v1
    /* 1B84C 8001AC4C 00007012 */  mflo       $t6
    /* 1B850 8001AC50 000EC103 */  sra        $t8, $t6, 4
    /* 1B854 8001AC54 02388821 */  addu       $s1, $s1, $t8
  .L8001AC58:
    /* 1B858 8001AC58 02402025 */  or         $a0, $s2, $zero
    /* 1B85C 8001AC5C 0C0066B5 */  jal        func_80019AD4
    /* 1B860 8001AC60 02202825 */   or        $a1, $s1, $zero
    /* 1B864 8001AC64 02402025 */  or         $a0, $s2, $zero
    /* 1B868 8001AC68 0C006976 */  jal        func_8001A5D8
    /* 1B86C 8001AC6C 00402825 */   or        $a1, $v0, $zero
    /* 1B870 8001AC70 8E480040 */  lw         $t0, 0x40($s2)
    /* 1B874 8001AC74 924A004A */  lbu        $t2, 0x4A($s2)
    /* 1B878 8001AC78 8E590044 */  lw         $t9, 0x44($s2)
    /* 1B87C 8001AC7C 00484821 */  addu       $t1, $v0, $t0
    /* 1B880 8001AC80 12AA0009 */  beq        $s5, $t2, .L8001ACA8
    /* 1B884 8001AC84 01398021 */   addu      $s0, $t1, $t9
    /* 1B888 8001AC88 0C00852A */  jal        func_800214A8
    /* 1B88C 8001AC8C 02402025 */   or        $a0, $s2, $zero
    /* 1B890 8001AC90 00105C02 */  srl        $t3, $s0, 16
    /* 1B894 8001AC94 004B0019 */  multu      $v0, $t3
    /* 1B898 8001AC98 00008012 */  mflo       $s0
    /* 1B89C 8001AC9C 00108342 */  srl        $s0, $s0, 13
    /* 1B8A0 8001ACA0 10000003 */  b          .L8001ACB0
    /* 1B8A4 8001ACA4 02C02025 */   or        $a0, $s6, $zero
  .L8001ACA8:
    /* 1B8A8 8001ACA8 00108402 */  srl        $s0, $s0, 16
    /* 1B8AC 8001ACAC 02C02025 */  or         $a0, $s6, $zero
  .L8001ACB0:
    /* 1B8B0 8001ACB0 0C005283 */  jal        func_80014A0C
    /* 1B8B4 8001ACB4 3205FFFF */   andi      $a1, $s0, 0xFFFF
    /* 1B8B8 8001ACB8 8A4C0024 */  lwl        $t4, 0x24($s2)
    /* 1B8BC 8001ACBC 9A4C0027 */  lwr        $t4, 0x27($s2)
    /* 1B8C0 8001ACC0 00003825 */  or         $a3, $zero, $zero
    /* 1B8C4 8001ACC4 000C6B80 */  sll        $t5, $t4, 14
    /* 1B8C8 8001ACC8 05A30032 */  bgezl      $t5, .L8001AD94
    /* 1B8CC 8001ACCC 925800BE */   lbu       $t8, 0xBE($s2)
    /* 1B8D0 8001ACD0 8A4F00B4 */  lwl        $t7, 0xB4($s2)
    /* 1B8D4 8001ACD4 9A4F00B7 */  lwr        $t7, 0xB7($s2)
    /* 1B8D8 8001ACD8 8A580084 */  lwl        $t8, 0x84($s2)
    /* 1B8DC 8001ACDC 9A580087 */  lwr        $t8, 0x87($s2)
    /* 1B8E0 8001ACE0 000F7202 */  srl        $t6, $t7, 8
    /* 1B8E4 8001ACE4 8A490088 */  lwl        $t1, 0x88($s2)
    /* 1B8E8 8001ACE8 01D80019 */  multu      $t6, $t8
    /* 1B8EC 8001ACEC 9A49008B */  lwr        $t1, 0x8B($s2)
    /* 1B8F0 8001ACF0 00004012 */  mflo       $t0
    /* 1B8F4 8001ACF4 01281023 */  subu       $v0, $t1, $t0
    /* 1B8F8 8001ACF8 AA420030 */  swl        $v0, 0x30($s2)
    /* 1B8FC 8001ACFC 04410004 */  bgez       $v0, .L8001AD10
    /* 1B900 8001AD00 BA420033 */   swr       $v0, 0x33($s2)
    /* 1B904 8001AD04 AA400030 */  swl        $zero, 0x30($s2)
    /* 1B908 8001AD08 1000000C */  b          .L8001AD3C
    /* 1B90C 8001AD0C BA400033 */   swr       $zero, 0x33($s2)
  .L8001AD10:
    /* 1B910 8001AD10 8A430030 */  lwl        $v1, 0x30($s2)
    /* 1B914 8001AD14 9A430033 */  lwr        $v1, 0x33($s2)
    /* 1B918 8001AD18 3C01007F */  lui        $at, (0x7F0001 >> 16)
    /* 1B91C 8001AD1C 34210001 */  ori        $at, $at, (0x7F0001 & 0xFFFF)
    /* 1B920 8001AD20 0061082B */  sltu       $at, $v1, $at
    /* 1B924 8001AD24 14200003 */  bnez       $at, .L8001AD34
    /* 1B928 8001AD28 00601025 */   or        $v0, $v1, $zero
    /* 1B92C 8001AD2C 10000001 */  b          .L8001AD34
    /* 1B930 8001AD30 3C02007F */   lui       $v0, (0x7F0000 >> 16)
  .L8001AD34:
    /* 1B934 8001AD34 AA420030 */  swl        $v0, 0x30($s2)
    /* 1B938 8001AD38 BA420033 */  swr        $v0, 0x33($s2)
  .L8001AD3C:
    /* 1B93C 8001AD3C 8A5900B4 */  lwl        $t9, 0xB4($s2)
    /* 1B940 8001AD40 8E6A0000 */  lw         $t2, 0x0($s3)
    /* 1B944 8001AD44 9A5900B7 */  lwr        $t9, 0xB7($s2)
    /* 1B948 8001AD48 3C01FFFD */  lui        $at, (0xFFFDFFFF >> 16)
    /* 1B94C 8001AD4C 3421FFFF */  ori        $at, $at, (0xFFFDFFFF & 0xFFFF)
    /* 1B950 8001AD50 032A5823 */  subu       $t3, $t9, $t2
    /* 1B954 8001AD54 AA4B00B4 */  swl        $t3, 0xB4($s2)
    /* 1B958 8001AD58 BA4B00B7 */  swr        $t3, 0xB7($s2)
    /* 1B95C 8001AD5C 8A4C00B4 */  lwl        $t4, 0xB4($s2)
    /* 1B960 8001AD60 9A4C00B7 */  lwr        $t4, 0xB7($s2)
    /* 1B964 8001AD64 5D80000B */  bgtzl      $t4, .L8001AD94
    /* 1B968 8001AD68 925800BE */   lbu       $t8, 0xBE($s2)
    /* 1B96C 8001AD6C 8A4D0024 */  lwl        $t5, 0x24($s2)
    /* 1B970 8001AD70 9A4D0027 */  lwr        $t5, 0x27($s2)
    /* 1B974 8001AD74 8A4E0088 */  lwl        $t6, 0x88($s2)
    /* 1B978 8001AD78 9A4E008B */  lwr        $t6, 0x8B($s2)
    /* 1B97C 8001AD7C 01A17824 */  and        $t7, $t5, $at
    /* 1B980 8001AD80 AA4F0024 */  swl        $t7, 0x24($s2)
    /* 1B984 8001AD84 AA4E0030 */  swl        $t6, 0x30($s2)
    /* 1B988 8001AD88 BA4F0027 */  swr        $t7, 0x27($s2)
    /* 1B98C 8001AD8C BA4E0033 */  swr        $t6, 0x33($s2)
    /* 1B990 8001AD90 925800BE */  lbu        $t8, 0xBE($s2)
  .L8001AD94:
    /* 1B994 8001AD94 3C088005 */  lui        $t0, %hi(D_8004F300)
    /* 1B998 8001AD98 2508F300 */  addiu      $t0, $t0, %lo(D_8004F300)
    /* 1B99C 8001AD9C 00184880 */  sll        $t1, $t8, 2
    /* 1B9A0 8001ADA0 01384821 */  addu       $t1, $t1, $t8
    /* 1B9A4 8001ADA4 000948C0 */  sll        $t1, $t1, 3
    /* 1B9A8 8001ADA8 8A500030 */  lwl        $s0, 0x30($s2)
    /* 1B9AC 8001ADAC 01281021 */  addu       $v0, $t1, $t0
    /* 1B9B0 8001ADB0 8C590018 */  lw         $t9, 0x18($v0)
    /* 1B9B4 8001ADB4 9A500033 */  lwr        $s0, 0x33($s2)
    /* 1B9B8 8001ADB8 8C4C0000 */  lw         $t4, 0x0($v0)
    /* 1B9BC 8001ADBC 00195403 */  sra        $t2, $t9, 16
    /* 1B9C0 8001ADC0 001059C3 */  sra        $t3, $s0, 7
    /* 1B9C4 8001ADC4 014B0019 */  multu      $t2, $t3
    /* 1B9C8 8001ADC8 000C6C03 */  sra        $t5, $t4, 16
    /* 1B9CC 8001ADCC 924E004C */  lbu        $t6, 0x4C($s2)
    /* 1B9D0 8001ADD0 8A510038 */  lwl        $s1, 0x38($s2)
    /* 1B9D4 8001ADD4 9244004A */  lbu        $a0, 0x4A($s2)
    /* 1B9D8 8001ADD8 24020015 */  addiu      $v0, $zero, 0x15
    /* 1B9DC 8001ADDC 3C098005 */  lui        $t1, %hi(D_8004F300)
    /* 1B9E0 8001ADE0 3C0A8005 */  lui        $t2, %hi(D_8004F2B8)
    /* 1B9E4 8001ADE4 3C0C8005 */  lui        $t4, %hi(D_8004F2F8)
    /* 1B9E8 8001ADE8 9A51003B */  lwr        $s1, 0x3B($s2)
    /* 1B9EC 8001ADEC 00008012 */  mflo       $s0
    /* 1B9F0 8001ADF0 001079C3 */  sra        $t7, $s0, 7
    /* 1B9F4 8001ADF4 00000000 */  nop
    /* 1B9F8 8001ADF8 01AF0019 */  multu      $t5, $t7
    /* 1B9FC 8001ADFC 00008012 */  mflo       $s0
    /* 1BA00 8001AE00 11C00003 */  beqz       $t6, .L8001AE10
    /* 1BA04 8001AE04 00000000 */   nop
    /* 1BA08 8001AE08 10000001 */  b          .L8001AE10
    /* 1BA0C 8001AE0C 24020016 */   addiu     $v0, $zero, 0x16
  .L8001AE10:
    /* 1BA10 8001AE10 0002C080 */  sll        $t8, $v0, 2
    /* 1BA14 8001AE14 0302C021 */  addu       $t8, $t8, $v0
    /* 1BA18 8001AE18 0018C0C0 */  sll        $t8, $t8, 3
    /* 1BA1C 8001AE1C 01384821 */  addu       $t1, $t1, $t8
    /* 1BA20 8001AE20 8D29F300 */  lw         $t1, %lo(D_8004F300)($t1)
    /* 1BA24 8001AE24 0010C9C3 */  sra        $t9, $s0, 7
    /* 1BA28 8001AE28 9243002F */  lbu        $v1, 0x2F($s2)
    /* 1BA2C 8001AE2C 00094403 */  sra        $t0, $t1, 16
    /* 1BA30 8001AE30 01190019 */  multu      $t0, $t9
    /* 1BA34 8001AE34 01435021 */  addu       $t2, $t2, $v1
    /* 1BA38 8001AE38 00008012 */  mflo       $s0
    /* 1BA3C 8001AE3C 12A30007 */  beq        $s5, $v1, .L8001AE5C
    /* 1BA40 8001AE40 00000000 */   nop
    /* 1BA44 8001AE44 914AF2B8 */  lbu        $t2, %lo(D_8004F2B8)($t2)
    /* 1BA48 8001AE48 001059C3 */  sra        $t3, $s0, 7
    /* 1BA4C 8001AE4C 014B0019 */  multu      $t2, $t3
    /* 1BA50 8001AE50 00008012 */  mflo       $s0
    /* 1BA54 8001AE54 00000000 */  nop
    /* 1BA58 8001AE58 00000000 */  nop
  .L8001AE5C:
    /* 1BA5C 8001AE5C 918CF2F8 */  lbu        $t4, %lo(D_8004F2F8)($t4)
    /* 1BA60 8001AE60 1580002A */  bnez       $t4, .L8001AF0C
    /* 1BA64 8001AE64 00000000 */   nop
    /* 1BA68 8001AE68 12A40026 */  beq        $s5, $a0, .L8001AF04
    /* 1BA6C 8001AE6C 00802825 */   or        $a1, $a0, $zero
    /* 1BA70 8001AE70 02402025 */  or         $a0, $s2, $zero
    /* 1BA74 8001AE74 0C00850A */  jal        func_80021428
    /* 1BA78 8001AE78 AFA50048 */   sw        $a1, 0x48($sp)
    /* 1BA7C 8001AE7C 000269C3 */  sra        $t5, $v0, 7
    /* 1BA80 8001AE80 001079C3 */  sra        $t7, $s0, 7
    /* 1BA84 8001AE84 01AF0019 */  multu      $t5, $t7
    /* 1BA88 8001AE88 3C010080 */  lui        $at, (0x800000 >> 16)
    /* 1BA8C 8001AE8C 8FA50048 */  lw         $a1, 0x48($sp)
    /* 1BA90 8001AE90 02402025 */  or         $a0, $s2, $zero
    /* 1BA94 8001AE94 00008012 */  mflo       $s0
    /* 1BA98 8001AE98 52210012 */  beql       $s1, $at, .L8001AEE4
    /* 1BA9C 8001AE9C 02402025 */   or        $a0, $s2, $zero
    /* 1BAA0 8001AEA0 0C008512 */  jal        func_80021448
    /* 1BAA4 8001AEA4 AFA50048 */   sw        $a1, 0x48($sp)
    /* 1BAA8 8001AEA8 000271C3 */  sra        $t6, $v0, 7
    /* 1BAAC 8001AEAC 25D8FFC0 */  addiu      $t8, $t6, -0x40
    /* 1BAB0 8001AEB0 00184C00 */  sll        $t1, $t8, 16
    /* 1BAB4 8001AEB4 02298821 */  addu       $s1, $s1, $t1
    /* 1BAB8 8001AEB8 06210003 */  bgez       $s1, .L8001AEC8
    /* 1BABC 8001AEBC 8FA50048 */   lw        $a1, 0x48($sp)
    /* 1BAC0 8001AEC0 10000007 */  b          .L8001AEE0
    /* 1BAC4 8001AEC4 00008825 */   or        $s1, $zero, $zero
  .L8001AEC8:
    /* 1BAC8 8001AEC8 0237082A */  slt        $at, $s1, $s7
    /* 1BACC 8001AECC 14200003 */  bnez       $at, .L8001AEDC
    /* 1BAD0 8001AED0 02201025 */   or        $v0, $s1, $zero
    /* 1BAD4 8001AED4 10000001 */  b          .L8001AEDC
    /* 1BAD8 8001AED8 03C01025 */   or        $v0, $fp, $zero
  .L8001AEDC:
    /* 1BADC 8001AEDC 00408825 */  or         $s1, $v0, $zero
  .L8001AEE0:
    /* 1BAE0 8001AEE0 02402025 */  or         $a0, $s2, $zero
  .L8001AEE4:
    /* 1BAE4 8001AEE4 0C00851A */  jal        func_80021468
    /* 1BAE8 8001AEE8 AFA50048 */   sw        $a1, 0x48($sp)
    /* 1BAEC 8001AEEC 00023A40 */  sll        $a3, $v0, 9
    /* 1BAF0 8001AEF0 00F7082A */  slt        $at, $a3, $s7
    /* 1BAF4 8001AEF4 14200014 */  bnez       $at, .L8001AF48
    /* 1BAF8 8001AEF8 8FA50048 */   lw        $a1, 0x48($sp)
    /* 1BAFC 8001AEFC 10000012 */  b          .L8001AF48
    /* 1BB00 8001AF00 03C03825 */   or        $a3, $fp, $zero
  .L8001AF04:
    /* 1BB04 8001AF04 10000010 */  b          .L8001AF48
    /* 1BB08 8001AF08 00003825 */   or        $a3, $zero, $zero
  .L8001AF0C:
    /* 1BB0C 8001AF0C 12A4000D */  beq        $s5, $a0, .L8001AF44
    /* 1BB10 8001AF10 00802825 */   or        $a1, $a0, $zero
    /* 1BB14 8001AF14 02402025 */  or         $a0, $s2, $zero
    /* 1BB18 8001AF18 AFA50048 */  sw         $a1, 0x48($sp)
    /* 1BB1C 8001AF1C 0C00850A */  jal        func_80021428
    /* 1BB20 8001AF20 AFA70068 */   sw        $a3, 0x68($sp)
    /* 1BB24 8001AF24 000241C3 */  sra        $t0, $v0, 7
    /* 1BB28 8001AF28 0010C9C3 */  sra        $t9, $s0, 7
    /* 1BB2C 8001AF2C 01190019 */  multu      $t0, $t9
    /* 1BB30 8001AF30 8FA50048 */  lw         $a1, 0x48($sp)
    /* 1BB34 8001AF34 8FA70068 */  lw         $a3, 0x68($sp)
    /* 1BB38 8001AF38 00008012 */  mflo       $s0
    /* 1BB3C 8001AF3C 00000000 */  nop
    /* 1BB40 8001AF40 00000000 */  nop
  .L8001AF44:
    /* 1BB44 8001AF44 3C110040 */  lui        $s1, (0x400000 >> 16)
  .L8001AF48:
    /* 1BB48 8001AF48 12A5001D */  beq        $s5, $a1, .L8001AFC0
    /* 1BB4C 8001AF4C 00001825 */   or        $v1, $zero, $zero
    /* 1BB50 8001AF50 02402025 */  or         $a0, $s2, $zero
    /* 1BB54 8001AF54 0C00854A */  jal        func_80021528
    /* 1BB58 8001AF58 AFA70068 */   sw        $a3, 0x68($sp)
    /* 1BB5C 8001AF5C 000251C3 */  sra        $t2, $v0, 7
    /* 1BB60 8001AF60 01500019 */  multu      $t2, $s0
    /* 1BB64 8001AF64 8FA70068 */  lw         $a3, 0x68($sp)
    /* 1BB68 8001AF68 00002012 */  mflo       $a0
    /* 1BB6C 8001AF6C 000419C3 */  sra        $v1, $a0, 7
    /* 1BB70 8001AF70 0077082A */  slt        $at, $v1, $s7
    /* 1BB74 8001AF74 54200003 */  bnel       $at, $zero, .L8001AF84
    /* 1BB78 8001AF78 924C0099 */   lbu       $t4, 0x99($s2)
    /* 1BB7C 8001AF7C 03C01825 */  or         $v1, $fp, $zero
    /* 1BB80 8001AF80 924C0099 */  lbu        $t4, 0x99($s2)
  .L8001AF84:
    /* 1BB84 8001AF84 000359C3 */  sra        $t3, $v1, 7
    /* 1BB88 8001AF88 016C0019 */  multu      $t3, $t4
    /* 1BB8C 8001AF8C 00001812 */  mflo       $v1
    /* 1BB90 8001AF90 0077082A */  slt        $at, $v1, $s7
    /* 1BB94 8001AF94 54200003 */  bnel       $at, $zero, .L8001AFA4
    /* 1BB98 8001AF98 924D009A */   lbu       $t5, 0x9A($s2)
    /* 1BB9C 8001AF9C 03C01825 */  or         $v1, $fp, $zero
    /* 1BBA0 8001AFA0 924D009A */  lbu        $t5, 0x9A($s2)
  .L8001AFA4:
    /* 1BBA4 8001AFA4 000D7C00 */  sll        $t7, $t5, 16
    /* 1BBA8 8001AFA8 01E31821 */  addu       $v1, $t7, $v1
    /* 1BBAC 8001AFAC 0077082A */  slt        $at, $v1, $s7
    /* 1BBB0 8001AFB0 54200004 */  bnel       $at, $zero, .L8001AFC4
    /* 1BBB4 8001AFB4 00107203 */   sra       $t6, $s0, 8
    /* 1BBB8 8001AFB8 10000001 */  b          .L8001AFC0
    /* 1BBBC 8001AFBC 03C01825 */   or        $v1, $fp, $zero
  .L8001AFC0:
    /* 1BBC0 8001AFC0 00107203 */  sra        $t6, $s0, 8
  .L8001AFC4:
    /* 1BBC4 8001AFC4 000E0A02 */  srl        $at, $t6, 8
    /* 1BBC8 8001AFC8 A24100C2 */  sb         $at, 0xC2($s2)
    /* 1BBCC 8001AFCC A24E00C3 */  sb         $t6, 0xC3($s2)
    /* 1BBD0 8001AFD0 AFA30010 */  sw         $v1, 0x10($sp)
    /* 1BBD4 8001AFD4 02C02025 */  or         $a0, $s6, $zero
    /* 1BBD8 8001AFD8 02002825 */  or         $a1, $s0, $zero
    /* 1BBDC 8001AFDC 0C00529D */  jal        func_80014A74
    /* 1BBE0 8001AFE0 02203025 */   or        $a2, $s1, $zero
  .L8001AFE4:
    /* 1BBE4 8001AFE4 3C188005 */  lui        $t8, %hi(D_8004FA18)
    /* 1BBE8 8001AFE8 9318FA18 */  lbu        $t8, %lo(D_8004FA18)($t8)
    /* 1BBEC 8001AFEC 26D60001 */  addiu      $s6, $s6, 0x1
    /* 1BBF0 8001AFF0 265201A0 */  addiu      $s2, $s2, 0x1A0
    /* 1BBF4 8001AFF4 02D8082A */  slt        $at, $s6, $t8
    /* 1BBF8 8001AFF8 5420FDB0 */  bnel       $at, $zero, .L8001A6BC
    /* 1BBFC 8001AFFC 8A4F0000 */   lwl       $t7, 0x0($s2)
  .L8001B000:
    /* 1BC00 8001B000 3C108005 */  lui        $s0, %hi(D_8004F300)
    /* 1BC04 8001B004 3C138005 */  lui        $s3, %hi(D_8004F800)
    /* 1BC08 8001B008 24140002 */  addiu      $s4, $zero, 0x2
    /* 1BC0C 8001B00C 2673F800 */  addiu      $s3, $s3, %lo(D_8004F800)
    /* 1BC10 8001B010 2610F300 */  addiu      $s0, $s0, %lo(D_8004F300)
    /* 1BC14 8001B014 24120003 */  addiu      $s2, $zero, 0x3
    /* 1BC18 8001B018 24110001 */  addiu      $s1, $zero, 0x1
    /* 1BC1C 8001B01C 8E03000C */  lw         $v1, 0xC($s0)
  .L8001B020:
    /* 1BC20 8001B020 50600027 */  beql       $v1, $zero, .L8001B0C0
    /* 1BC24 8001B024 8E030024 */   lw        $v1, 0x24($s0)
    /* 1BC28 8001B028 8E090008 */  lw         $t1, 0x8($s0)
    /* 1BC2C 8001B02C 00034203 */  sra        $t0, $v1, 8
    /* 1BC30 8001B030 8E0A0004 */  lw         $t2, 0x4($s0)
    /* 1BC34 8001B034 01280019 */  multu      $t1, $t0
    /* 1BC38 8001B038 3C048005 */  lui        $a0, %hi(D_8004BE90)
    /* 1BC3C 8001B03C 0000C812 */  mflo       $t9
    /* 1BC40 8001B040 01591023 */  subu       $v0, $t2, $t9
    /* 1BC44 8001B044 AE020000 */  sw         $v0, 0x0($s0)
    /* 1BC48 8001B048 04410003 */  bgez       $v0, .L8001B058
    /* 1BC4C 8001B04C 8C84BE90 */   lw        $a0, %lo(D_8004BE90)($a0)
    /* 1BC50 8001B050 AE000000 */  sw         $zero, 0x0($s0)
    /* 1BC54 8001B054 8E03000C */  lw         $v1, 0xC($s0)
  .L8001B058:
    /* 1BC58 8001B058 00645823 */  subu       $t3, $v1, $a0
    /* 1BC5C 8001B05C 1D600017 */  bgtz       $t3, .L8001B0BC
    /* 1BC60 8001B060 AE0B000C */   sw        $t3, 0xC($s0)
    /* 1BC64 8001B064 92020015 */  lbu        $v0, 0x15($s0)
    /* 1BC68 8001B068 8E0D0004 */  lw         $t5, 0x4($s0)
    /* 1BC6C 8001B06C AE00000C */  sw         $zero, 0xC($s0)
    /* 1BC70 8001B070 10510007 */  beq        $v0, $s1, .L8001B090
    /* 1BC74 8001B074 AE0D0000 */   sw        $t5, 0x0($s0)
    /* 1BC78 8001B078 10540009 */  beq        $v0, $s4, .L8001B0A0
    /* 1BC7C 8001B07C 00000000 */   nop
    /* 1BC80 8001B080 1052000B */  beq        $v0, $s2, .L8001B0B0
    /* 1BC84 8001B084 00002825 */   or        $a1, $zero, $zero
    /* 1BC88 8001B088 1000000D */  b          .L8001B0C0
    /* 1BC8C 8001B08C 8E030024 */   lw        $v1, 0x24($s0)
  .L8001B090:
    /* 1BC90 8001B090 0C006350 */  jal        func_80018D40
    /* 1BC94 8001B094 8E040010 */   lw        $a0, 0x10($s0)
    /* 1BC98 8001B098 10000009 */  b          .L8001B0C0
    /* 1BC9C 8001B09C 8E030024 */   lw        $v1, 0x24($s0)
  .L8001B0A0:
    /* 1BCA0 8001B0A0 0C00630B */  jal        func_80018C2C
    /* 1BCA4 8001B0A4 8E040010 */   lw        $a0, 0x10($s0)
    /* 1BCA8 8001B0A8 10000005 */  b          .L8001B0C0
    /* 1BCAC 8001B0AC 8E030024 */   lw        $v1, 0x24($s0)
  .L8001B0B0:
    /* 1BCB0 8001B0B0 8E040010 */  lw         $a0, 0x10($s0)
    /* 1BCB4 8001B0B4 0C00642B */  jal        func_800190AC
    /* 1BCB8 8001B0B8 00003025 */   or        $a2, $zero, $zero
  .L8001B0BC:
    /* 1BCBC 8001B0BC 8E030024 */  lw         $v1, 0x24($s0)
  .L8001B0C0:
    /* 1BCC0 8001B0C0 50600014 */  beql       $v1, $zero, .L8001B114
    /* 1BCC4 8001B0C4 26100028 */   addiu     $s0, $s0, 0x28
    /* 1BCC8 8001B0C8 8E0F0020 */  lw         $t7, 0x20($s0)
    /* 1BCCC 8001B0CC 00037203 */  sra        $t6, $v1, 8
    /* 1BCD0 8001B0D0 8E09001C */  lw         $t1, 0x1C($s0)
    /* 1BCD4 8001B0D4 01EE0019 */  multu      $t7, $t6
    /* 1BCD8 8001B0D8 3C048005 */  lui        $a0, %hi(D_8004BE90)
    /* 1BCDC 8001B0DC 0000C012 */  mflo       $t8
    /* 1BCE0 8001B0E0 01381023 */  subu       $v0, $t1, $t8
    /* 1BCE4 8001B0E4 AE020018 */  sw         $v0, 0x18($s0)
    /* 1BCE8 8001B0E8 04410003 */  bgez       $v0, .L8001B0F8
    /* 1BCEC 8001B0EC 8C84BE90 */   lw        $a0, %lo(D_8004BE90)($a0)
    /* 1BCF0 8001B0F0 AE000018 */  sw         $zero, 0x18($s0)
    /* 1BCF4 8001B0F4 8E030024 */  lw         $v1, 0x24($s0)
  .L8001B0F8:
    /* 1BCF8 8001B0F8 00644023 */  subu       $t0, $v1, $a0
    /* 1BCFC 8001B0FC 1D000004 */  bgtz       $t0, .L8001B110
    /* 1BD00 8001B100 AE080024 */   sw        $t0, 0x24($s0)
    /* 1BD04 8001B104 8E19001C */  lw         $t9, 0x1C($s0)
    /* 1BD08 8001B108 AE000024 */  sw         $zero, 0x24($s0)
    /* 1BD0C 8001B10C AE190018 */  sw         $t9, 0x18($s0)
  .L8001B110:
    /* 1BD10 8001B110 26100028 */  addiu      $s0, $s0, 0x28
  .L8001B114:
    /* 1BD14 8001B114 5613FFC2 */  bnel       $s0, $s3, .L8001B020
    /* 1BD18 8001B118 8E03000C */   lw        $v1, 0xC($s0)
    /* 1BD1C 8001B11C 0C005345 */  jal        func_80014D14
    /* 1BD20 8001B120 00000000 */   nop
    /* 1BD24 8001B124 8FBF0044 */  lw         $ra, 0x44($sp)
    /* 1BD28 8001B128 8FB00020 */  lw         $s0, 0x20($sp)
    /* 1BD2C 8001B12C 8FB10024 */  lw         $s1, 0x24($sp)
    /* 1BD30 8001B130 8FB20028 */  lw         $s2, 0x28($sp)
    /* 1BD34 8001B134 8FB3002C */  lw         $s3, 0x2C($sp)
    /* 1BD38 8001B138 8FB40030 */  lw         $s4, 0x30($sp)
    /* 1BD3C 8001B13C 8FB50034 */  lw         $s5, 0x34($sp)
    /* 1BD40 8001B140 8FB60038 */  lw         $s6, 0x38($sp)
    /* 1BD44 8001B144 8FB7003C */  lw         $s7, 0x3C($sp)
    /* 1BD48 8001B148 8FBE0040 */  lw         $fp, 0x40($sp)
    /* 1BD4C 8001B14C 03E00008 */  jr         $ra
    /* 1BD50 8001B150 27BD0088 */   addiu     $sp, $sp, 0x88
endlabel func_8001A658
