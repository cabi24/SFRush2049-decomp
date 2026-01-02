# Source: 21F0.s
# Address: 0x800015F0

glabel func_800015F0
    /* 21F0 800015F0 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* 21F4 800015F4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 21F8 800015F8 3C0E8003 */  lui        $t6, %hi(gViewportStruct)
    /* 21FC 800015FC 8DCEEBFC */  lw         $t6, %lo(gViewportStruct)($t6)
    /* 2200 80001600 15C00003 */  bnez       $t6, .L80001610
    /* 2204 80001604 00000000 */   nop
    /* 2208 80001608 1000014A */  b          .L80001B34
    /* 220C 8000160C 00000000 */   nop
  .L80001610:
    /* 2210 80001610 3C0F8003 */  lui        $t7, %hi(gViewportLeftEdge)
    /* 2214 80001614 3C188003 */  lui        $t8, %hi(gViewportScaleX)
    /* 2218 80001618 8718ECB0 */  lh         $t8, %lo(gViewportScaleX)($t8)
    /* 221C 8000161C 85EFECA0 */  lh         $t7, %lo(gViewportLeftEdge)($t7)
    /* 2220 80001620 01F8C821 */  addu       $t9, $t7, $t8
    /* 2224 80001624 AFB90030 */  sw         $t9, 0x30($sp)
    /* 2228 80001628 3C088003 */  lui        $t0, %hi(gViewportRightEdge)
    /* 222C 8000162C 3C098003 */  lui        $t1, %hi(gViewportScaleX)
    /* 2230 80001630 8529ECB0 */  lh         $t1, %lo(gViewportScaleX)($t1)
    /* 2234 80001634 8508ECA4 */  lh         $t0, %lo(gViewportRightEdge)($t0)
    /* 2238 80001638 3C0B8003 */  lui        $t3, %hi(gViewportXOverflow)
    /* 223C 8000163C 856BECA8 */  lh         $t3, %lo(gViewportXOverflow)($t3)
    /* 2240 80001640 01095021 */  addu       $t2, $t0, $t1
    /* 2244 80001644 014B6021 */  addu       $t4, $t2, $t3
    /* 2248 80001648 AFAC0028 */  sw         $t4, 0x28($sp)
    /* 224C 8000164C 3C0D8003 */  lui        $t5, %hi(gViewportTopEdge)
    /* 2250 80001650 3C0E8003 */  lui        $t6, %hi(gViewportScaleY)
    /* 2254 80001654 85CEECB2 */  lh         $t6, %lo(gViewportScaleY)($t6)
    /* 2258 80001658 85ADECA2 */  lh         $t5, %lo(gViewportTopEdge)($t5)
    /* 225C 8000165C 01AE7821 */  addu       $t7, $t5, $t6
    /* 2260 80001660 AFAF002C */  sw         $t7, 0x2C($sp)
    /* 2264 80001664 3C188003 */  lui        $t8, %hi(gViewportBottomEdge)
    /* 2268 80001668 3C198003 */  lui        $t9, %hi(gViewportScaleY)
    /* 226C 8000166C 8739ECB2 */  lh         $t9, %lo(gViewportScaleY)($t9)
    /* 2270 80001670 8718ECA6 */  lh         $t8, %lo(gViewportBottomEdge)($t8)
    /* 2274 80001674 3C098003 */  lui        $t1, %hi(gViewportYOverflow)
    /* 2278 80001678 8529ECAA */  lh         $t1, %lo(gViewportYOverflow)($t1)
    /* 227C 8000167C 03194021 */  addu       $t0, $t8, $t9
    /* 2280 80001680 01095021 */  addu       $t2, $t0, $t1
    /* 2284 80001684 AFAA0024 */  sw         $t2, 0x24($sp)
    /* 2288 80001688 3C0B8003 */  lui        $t3, %hi(gViewportRightEdge)
    /* 228C 8000168C 3C0C8003 */  lui        $t4, %hi(gViewportLeftEdge)
    /* 2290 80001690 858CECA0 */  lh         $t4, %lo(gViewportLeftEdge)($t4)
    /* 2294 80001694 856BECA4 */  lh         $t3, %lo(gViewportRightEdge)($t3)
    /* 2298 80001698 8FAE0028 */  lw         $t6, 0x28($sp)
    /* 229C 8000169C 8FAF0030 */  lw         $t7, 0x30($sp)
    /* 22A0 800016A0 016C6823 */  subu       $t5, $t3, $t4
    /* 22A4 800016A4 448D2000 */  mtc1       $t5, $ft0
    /* 22A8 800016A8 01CFC023 */  subu       $t8, $t6, $t7
    /* 22AC 800016AC 44984000 */  mtc1       $t8, $ft2
    /* 22B0 800016B0 468021A0 */  cvt.s.w    $ft1, $ft0
    /* 22B4 800016B4 468042A0 */  cvt.s.w    $ft3, $ft2
    /* 22B8 800016B8 460A3403 */  div.s      $ft4, $ft1, $ft3
    /* 22BC 800016BC E7B0001C */  swc1       $ft4, 0x1C($sp)
    /* 22C0 800016C0 3C198003 */  lui        $t9, %hi(gViewportBottomEdge)
    /* 22C4 800016C4 3C088003 */  lui        $t0, %hi(gViewportTopEdge)
    /* 22C8 800016C8 8508ECA2 */  lh         $t0, %lo(gViewportTopEdge)($t0)
    /* 22CC 800016CC 8739ECA6 */  lh         $t9, %lo(gViewportBottomEdge)($t9)
    /* 22D0 800016D0 8FAA0024 */  lw         $t2, 0x24($sp)
    /* 22D4 800016D4 8FAB002C */  lw         $t3, 0x2C($sp)
    /* 22D8 800016D8 03284823 */  subu       $t1, $t9, $t0
    /* 22DC 800016DC 44899000 */  mtc1       $t1, $ft5
    /* 22E0 800016E0 014B6023 */  subu       $t4, $t2, $t3
    /* 22E4 800016E4 448C4000 */  mtc1       $t4, $ft2
    /* 22E8 800016E8 46809120 */  cvt.s.w    $ft0, $ft5
    /* 22EC 800016EC 468041A0 */  cvt.s.w    $ft1, $ft2
    /* 22F0 800016F0 46062283 */  div.s      $ft3, $ft0, $ft1
    /* 22F4 800016F4 E7AA0018 */  swc1       $ft3, 0x18($sp)
    /* 22F8 800016F8 0C001C74 */  jal        func_800071D0
    /* 22FC 800016FC 24040001 */   addiu     $a0, $zero, 0x1
    /* 2300 80001700 AFA20034 */  sw         $v0, 0x34($sp)
    /* 2304 80001704 3C0E8003 */  lui        $t6, %hi(gViewportDataPtr)
    /* 2308 80001708 8DCEB014 */  lw         $t6, %lo(gViewportDataPtr)($t6)
    /* 230C 8000170C 3C0D8003 */  lui        $t5, %hi(gViewportBuffer)
    /* 2310 80001710 25ADEC00 */  addiu      $t5, $t5, %lo(gViewportBuffer)
    /* 2314 80001714 15AE0007 */  bne        $t5, $t6, .L80001734
    /* 2318 80001718 00000000 */   nop
    /* 231C 8000171C 3C0F8003 */  lui        $t7, %hi(gViewportBuffer)
    /* 2320 80001720 25EFEC00 */  addiu      $t7, $t7, %lo(gViewportBuffer)
    /* 2324 80001724 25F80050 */  addiu      $t8, $t7, 0x50
    /* 2328 80001728 3C018003 */  lui        $at, %hi(gViewportDataPtr)
    /* 232C 8000172C 10000005 */  b          .L80001744
    /* 2330 80001730 AC38B014 */   sw        $t8, %lo(gViewportDataPtr)($at)
  .L80001734:
    /* 2334 80001734 3C198003 */  lui        $t9, %hi(gViewportBuffer)
    /* 2338 80001738 2739EC00 */  addiu      $t9, $t9, %lo(gViewportBuffer)
    /* 233C 8000173C 3C018003 */  lui        $at, %hi(gViewportDataPtr)
    /* 2340 80001740 AC39B014 */  sw         $t9, %lo(gViewportDataPtr)($at)
  .L80001744:
    /* 2344 80001744 3C048003 */  lui        $a0, %hi(gViewportDataPtr)
    /* 2348 80001748 3C058003 */  lui        $a1, %hi(gViewportStruct)
    /* 234C 8000174C 8CA5EBFC */  lw         $a1, %lo(gViewportStruct)($a1)
    /* 2350 80001750 8C84B014 */  lw         $a0, %lo(gViewportDataPtr)($a0)
    /* 2354 80001754 0C001F1A */  jal        func_80007C68
    /* 2358 80001758 24060050 */   addiu     $a2, $zero, 0x50
    /* 235C 8000175C 3C088003 */  lui        $t0, %hi(gViewportDataPtr)
    /* 2360 80001760 8D08B014 */  lw         $t0, %lo(gViewportDataPtr)($t0)
    /* 2364 80001764 8D090020 */  lw         $t1, 0x20($t0)
    /* 2368 80001768 44898000 */  mtc1       $t1, $ft4
    /* 236C 8000176C 05210005 */  bgez       $t1, .L80001784
    /* 2370 80001770 468084A0 */   cvt.s.w   $ft5, $ft4
    /* 2374 80001774 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* 2378 80001778 44814000 */  mtc1       $at, $ft2
    /* 237C 8000177C 00000000 */  nop
    /* 2380 80001780 46089480 */  add.s      $ft5, $ft5, $ft2
  .L80001784:
    /* 2384 80001784 3C0A8003 */  lui        $t2, %hi(gViewportStruct)
    /* 2388 80001788 8D4AEBFC */  lw         $t2, %lo(gViewportStruct)($t2)
    /* 238C 8000178C 8D4B0020 */  lw         $t3, 0x20($t2)
    /* 2390 80001790 448B2000 */  mtc1       $t3, $ft0
    /* 2394 80001794 05610005 */  bgez       $t3, .L800017AC
    /* 2398 80001798 468021A0 */   cvt.s.w   $ft1, $ft0
    /* 239C 8000179C 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* 23A0 800017A0 44815000 */  mtc1       $at, $ft3
    /* 23A4 800017A4 00000000 */  nop
    /* 23A8 800017A8 460A3180 */  add.s      $ft1, $ft1, $ft3
  .L800017AC:
    /* 23AC 800017AC 46069403 */  div.s      $ft4, $ft5, $ft1
    /* 23B0 800017B0 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 23B4 800017B4 44814000 */  mtc1       $at, $ft2
    /* 23B8 800017B8 8FAC0030 */  lw         $t4, 0x30($sp)
    /* 23BC 800017BC 3C0140A0 */  lui        $at, (0x40A00000 >> 16)
    /* 23C0 800017C0 44815000 */  mtc1       $at, $ft3
    /* 23C4 800017C4 448C3000 */  mtc1       $t4, $ft1
    /* 23C8 800017C8 46104101 */  sub.s      $ft0, $ft2, $ft4
    /* 23CC 800017CC 46803220 */  cvt.s.w    $ft2, $ft1
    /* 23D0 800017D0 460A2482 */  mul.s      $ft5, $ft0, $ft3
    /* 23D4 800017D4 46124401 */  sub.s      $ft4, $ft2, $ft5
    /* 23D8 800017D8 4600810D */  trunc.w.s  $ft0, $ft4
    /* 23DC 800017DC 440E2000 */  mfc1       $t6, $ft0
    /* 23E0 800017E0 00000000 */  nop
    /* 23E4 800017E4 AFAE0030 */  sw         $t6, 0x30($sp)
    /* 23E8 800017E8 8FAF0030 */  lw         $t7, 0x30($sp)
    /* 23EC 800017EC 8FB90028 */  lw         $t9, 0x28($sp)
    /* 23F0 800017F0 3C098003 */  lui        $t1, %hi(gViewportDataPtr)
    /* 23F4 800017F4 8D29B014 */  lw         $t1, %lo(gViewportDataPtr)($t1)
    /* 23F8 800017F8 000FC400 */  sll        $t8, $t7, 16
    /* 23FC 800017FC 03194025 */  or         $t0, $t8, $t9
    /* 2400 80001800 AD28001C */  sw         $t0, 0x1C($t1)
    /* 2404 80001804 3C0A8003 */  lui        $t2, %hi(gViewportScale)
    /* 2408 80001808 854AECAC */  lh         $t2, %lo(gViewportScale)($t2)
    /* 240C 8000180C C7A8001C */  lwc1       $ft2, 0x1C($sp)
    /* 2410 80001810 3C018003 */  lui        $at, %hi(gViewportFloatA)
    /* 2414 80001814 448A5000 */  mtc1       $t2, $ft3
    /* 2418 80001818 C430D4C0 */  lwc1       $ft4, %lo(gViewportFloatA)($at)
    /* 241C 8000181C 240C0001 */  addiu      $t4, $zero, 0x1
    /* 2420 80001820 468051A0 */  cvt.s.w    $ft1, $ft3
    /* 2424 80001824 46083482 */  mul.s      $ft5, $ft1, $ft2
    /* 2428 80001828 00000000 */  nop
    /* 242C 8000182C 46109102 */  mul.s      $ft0, $ft5, $ft4
    /* 2430 80001830 444BF800 */  cfc1       $t3, $31
    /* 2434 80001834 44CCF800 */  ctc1       $t4, $31
    /* 2438 80001838 00000000 */  nop
    /* 243C 8000183C 460022A4 */  cvt.w.s    $ft3, $ft0
    /* 2440 80001840 444CF800 */  cfc1       $t4, $31
    /* 2444 80001844 00000000 */  nop
    /* 2448 80001848 31810004 */  andi       $at, $t4, 0x4
    /* 244C 8000184C 318C0078 */  andi       $t4, $t4, 0x78
    /* 2450 80001850 11800014 */  beqz       $t4, .L800018A4
    /* 2454 80001854 00000000 */   nop
    /* 2458 80001858 3C014F00 */  lui        $at, (0x4F000000 >> 16)
    /* 245C 8000185C 44815000 */  mtc1       $at, $ft3
    /* 2460 80001860 240C0001 */  addiu      $t4, $zero, 0x1
    /* 2464 80001864 460A2281 */  sub.s      $ft3, $ft0, $ft3
    /* 2468 80001868 44CCF800 */  ctc1       $t4, $31
    /* 246C 8000186C 00000000 */  nop
    /* 2470 80001870 460052A4 */  cvt.w.s    $ft3, $ft3
    /* 2474 80001874 444CF800 */  cfc1       $t4, $31
    /* 2478 80001878 00000000 */  nop
    /* 247C 8000187C 31810004 */  andi       $at, $t4, 0x4
    /* 2480 80001880 318C0078 */  andi       $t4, $t4, 0x78
    /* 2484 80001884 15800005 */  bnez       $t4, .L8000189C
    /* 2488 80001888 00000000 */   nop
    /* 248C 8000188C 440C5000 */  mfc1       $t4, $ft3
    /* 2490 80001890 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* 2494 80001894 10000007 */  b          .L800018B4
    /* 2498 80001898 01816025 */   or        $t4, $t4, $at
  .L8000189C:
    /* 249C 8000189C 10000005 */  b          .L800018B4
    /* 24A0 800018A0 240CFFFF */   addiu     $t4, $zero, -0x1
  .L800018A4:
    /* 24A4 800018A4 440C5000 */  mfc1       $t4, $ft3
    /* 24A8 800018A8 00000000 */  nop
    /* 24AC 800018AC 0580FFFB */  bltz       $t4, .L8000189C
    /* 24B0 800018B0 00000000 */   nop
  .L800018B4:
    /* 24B4 800018B4 3C0D8003 */  lui        $t5, %hi(gViewportDataPtr)
    /* 24B8 800018B8 8DADB014 */  lw         $t5, %lo(gViewportDataPtr)($t5)
    /* 24BC 800018BC 44CBF800 */  ctc1       $t3, $31
    /* 24C0 800018C0 ADAC0020 */  sw         $t4, 0x20($t5)
    /* 24C4 800018C4 00000000 */  nop
    /* 24C8 800018C8 8FAE002C */  lw         $t6, 0x2C($sp)
    /* 24CC 800018CC 8FB90024 */  lw         $t9, 0x24($sp)
    /* 24D0 800018D0 3C0A8003 */  lui        $t2, %hi(gViewportDataPtr)
    /* 24D4 800018D4 8D4AB014 */  lw         $t2, %lo(gViewportDataPtr)($t2)
    /* 24D8 800018D8 31CF03FF */  andi       $t7, $t6, 0x3FF
    /* 24DC 800018DC 000FC400 */  sll        $t8, $t7, 16
    /* 24E0 800018E0 332803FF */  andi       $t0, $t9, 0x3FF
    /* 24E4 800018E4 03084825 */  or         $t1, $t8, $t0
    /* 24E8 800018E8 AD490030 */  sw         $t1, 0x30($t2)
    /* 24EC 800018EC 3C0B8003 */  lui        $t3, %hi(gViewportScale)
    /* 24F0 800018F0 856BECAC */  lh         $t3, %lo(gViewportScale)($t3)
    /* 24F4 800018F4 3C0D8003 */  lui        $t5, %hi(gViewportDataPtr)
    /* 24F8 800018F8 8DADB014 */  lw         $t5, %lo(gViewportDataPtr)($t5)
    /* 24FC 800018FC 000B6040 */  sll        $t4, $t3, 1
    /* 2500 80001900 ADAC0028 */  sw         $t4, 0x28($t5)
    /* 2504 80001904 3C0E8003 */  lui        $t6, %hi(gViewportScaleYAlt)
    /* 2508 80001908 85CEECAE */  lh         $t6, %lo(gViewportScaleYAlt)($t6)
    /* 250C 8000190C C7B20018 */  lwc1       $ft5, 0x18($sp)
    /* 2510 80001910 3C018003 */  lui        $at, %hi(gViewportFloatB)
    /* 2514 80001914 448E3000 */  mtc1       $t6, $ft1
    /* 2518 80001918 C424D4C4 */  lwc1       $ft0, %lo(gViewportFloatB)($at)
    /* 251C 8000191C 3C014184 */  lui        $at, (0x41840000 >> 16)
    /* 2520 80001920 46803220 */  cvt.s.w    $ft2, $ft1
    /* 2524 80001924 44813000 */  mtc1       $at, $ft1
    /* 2528 80001928 24190001 */  addiu      $t9, $zero, 0x1
    /* 252C 8000192C 46124402 */  mul.s      $ft4, $ft2, $ft5
    /* 2530 80001930 00000000 */  nop
    /* 2534 80001934 46048282 */  mul.s      $ft3, $ft4, $ft0
    /* 2538 80001938 46065200 */  add.s      $ft2, $ft3, $ft1
    /* 253C 8000193C 444FF800 */  cfc1       $t7, $31
    /* 2540 80001940 44D9F800 */  ctc1       $t9, $31
    /* 2544 80001944 00000000 */  nop
    /* 2548 80001948 460044A4 */  cvt.w.s    $ft5, $ft2
    /* 254C 8000194C 4459F800 */  cfc1       $t9, $31
    /* 2550 80001950 00000000 */  nop
    /* 2554 80001954 33210004 */  andi       $at, $t9, 0x4
    /* 2558 80001958 33390078 */  andi       $t9, $t9, 0x78
    /* 255C 8000195C 13200014 */  beqz       $t9, .L800019B0
    /* 2560 80001960 00000000 */   nop
    /* 2564 80001964 3C014F00 */  lui        $at, (0x4F000000 >> 16)
    /* 2568 80001968 44819000 */  mtc1       $at, $ft5
    /* 256C 8000196C 24190001 */  addiu      $t9, $zero, 0x1
    /* 2570 80001970 46124481 */  sub.s      $ft5, $ft2, $ft5
    /* 2574 80001974 44D9F800 */  ctc1       $t9, $31
    /* 2578 80001978 00000000 */  nop
    /* 257C 8000197C 460094A4 */  cvt.w.s    $ft5, $ft5
    /* 2580 80001980 4459F800 */  cfc1       $t9, $31
    /* 2584 80001984 00000000 */  nop
    /* 2588 80001988 33210004 */  andi       $at, $t9, 0x4
    /* 258C 8000198C 33390078 */  andi       $t9, $t9, 0x78
    /* 2590 80001990 17200005 */  bnez       $t9, .L800019A8
    /* 2594 80001994 00000000 */   nop
    /* 2598 80001998 44199000 */  mfc1       $t9, $ft5
    /* 259C 8000199C 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* 25A0 800019A0 10000007 */  b          .L800019C0
    /* 25A4 800019A4 0321C825 */   or        $t9, $t9, $at
  .L800019A8:
    /* 25A8 800019A8 10000005 */  b          .L800019C0
    /* 25AC 800019AC 2419FFFF */   addiu     $t9, $zero, -0x1
  .L800019B0:
    /* 25B0 800019B0 44199000 */  mfc1       $t9, $ft5
    /* 25B4 800019B4 00000000 */  nop
    /* 25B8 800019B8 0720FFFB */  bltz       $t9, .L800019A8
    /* 25BC 800019BC 00000000 */   nop
  .L800019C0:
    /* 25C0 800019C0 44CFF800 */  ctc1       $t7, $31
    /* 25C4 800019C4 AFB90020 */  sw         $t9, 0x20($sp)
    /* 25C8 800019C8 00000000 */  nop
    /* 25CC 800019CC 3C188003 */  lui        $t8, %hi(gViewportStruct)
    /* 25D0 800019D0 8F18EBFC */  lw         $t8, %lo(gViewportStruct)($t8)
    /* 25D4 800019D4 8F08003C */  lw         $t0, 0x3C($t8)
    /* 25D8 800019D8 8F090028 */  lw         $t1, 0x28($t8)
    /* 25DC 800019DC 11090011 */  beq        $t0, $t1, .L80001A24
    /* 25E0 800019E0 00000000 */   nop
    /* 25E4 800019E4 3C0A8003 */  lui        $t2, %hi(gViewportScale)
    /* 25E8 800019E8 854AECAC */  lh         $t2, %lo(gViewportScale)($t2)
    /* 25EC 800019EC 3C0C8003 */  lui        $t4, %hi(gViewportDataPtr)
    /* 25F0 800019F0 8D8CB014 */  lw         $t4, %lo(gViewportDataPtr)($t4)
    /* 25F4 800019F4 000A5880 */  sll        $t3, $t2, 2
    /* 25F8 800019F8 AD8B003C */  sw         $t3, 0x3C($t4)
    /* 25FC 800019FC 3C0D8003 */  lui        $t5, %hi(gViewportScale)
    /* 2600 80001A00 85ADECAC */  lh         $t5, %lo(gViewportScale)($t5)
    /* 2604 80001A04 3C0F8003 */  lui        $t7, %hi(gViewportDataPtr)
    /* 2608 80001A08 8DEFB014 */  lw         $t7, %lo(gViewportDataPtr)($t7)
    /* 260C 80001A0C 000D7040 */  sll        $t6, $t5, 1
    /* 2610 80001A10 ADEE0008 */  sw         $t6, 0x8($t7)
    /* 2614 80001A14 8FB90020 */  lw         $t9, 0x20($sp)
    /* 2618 80001A18 0019C042 */  srl        $t8, $t9, 1
    /* 261C 80001A1C 1000000C */  b          .L80001A50
    /* 2620 80001A20 AFB80020 */   sw        $t8, 0x20($sp)
  .L80001A24:
    /* 2624 80001A24 3C088003 */  lui        $t0, %hi(gViewportScale)
    /* 2628 80001A28 8508ECAC */  lh         $t0, %lo(gViewportScale)($t0)
    /* 262C 80001A2C 3C0A8003 */  lui        $t2, %hi(gViewportDataPtr)
    /* 2630 80001A30 8D4AB014 */  lw         $t2, %lo(gViewportDataPtr)($t2)
    /* 2634 80001A34 00084840 */  sll        $t1, $t0, 1
    /* 2638 80001A38 AD49003C */  sw         $t1, 0x3C($t2)
    /* 263C 80001A3C 3C0B8003 */  lui        $t3, %hi(gViewportScale)
    /* 2640 80001A40 3C0C8003 */  lui        $t4, %hi(gViewportDataPtr)
    /* 2644 80001A44 8D8CB014 */  lw         $t4, %lo(gViewportDataPtr)($t4)
    /* 2648 80001A48 856BECAC */  lh         $t3, %lo(gViewportScale)($t3)
    /* 264C 80001A4C AD8B0008 */  sw         $t3, 0x8($t4)
  .L80001A50:
    /* 2650 80001A50 3C0D8003 */  lui        $t5, %hi(gViewportDataPtr)
    /* 2654 80001A54 8DADB014 */  lw         $t5, %lo(gViewportDataPtr)($t5)
    /* 2658 80001A58 8DAE0004 */  lw         $t6, 0x4($t5)
    /* 265C 80001A5C 31CF0040 */  andi       $t7, $t6, 0x40
    /* 2660 80001A60 11E0000D */  beqz       $t7, .L80001A98
    /* 2664 80001A64 00000000 */   nop
    /* 2668 80001A68 8FB9002C */  lw         $t9, 0x2C($sp)
    /* 266C 80001A6C 8FAA0024 */  lw         $t2, 0x24($sp)
    /* 2670 80001A70 3C0E8003 */  lui        $t6, %hi(gViewportDataPtr)
    /* 2674 80001A74 27380002 */  addiu      $t8, $t9, 0x2
    /* 2678 80001A78 330803FF */  andi       $t0, $t8, 0x3FF
    /* 267C 80001A7C 254B0002 */  addiu      $t3, $t2, 0x2
    /* 2680 80001A80 8DCEB014 */  lw         $t6, %lo(gViewportDataPtr)($t6)
    /* 2684 80001A84 316C03FF */  andi       $t4, $t3, 0x3FF
    /* 2688 80001A88 00084C00 */  sll        $t1, $t0, 16
    /* 268C 80001A8C 012C6825 */  or         $t5, $t1, $t4
    /* 2690 80001A90 1000000A */  b          .L80001ABC
    /* 2694 80001A94 ADCD0044 */   sw        $t5, 0x44($t6)
  .L80001A98:
    /* 2698 80001A98 8FAF002C */  lw         $t7, 0x2C($sp)
    /* 269C 80001A9C 8FA80024 */  lw         $t0, 0x24($sp)
    /* 26A0 80001AA0 3C098003 */  lui        $t1, %hi(gViewportDataPtr)
    /* 26A4 80001AA4 8D29B014 */  lw         $t1, %lo(gViewportDataPtr)($t1)
    /* 26A8 80001AA8 31F903FF */  andi       $t9, $t7, 0x3FF
    /* 26AC 80001AAC 0019C400 */  sll        $t8, $t9, 16
    /* 26B0 80001AB0 310A03FF */  andi       $t2, $t0, 0x3FF
    /* 26B4 80001AB4 030A5825 */  or         $t3, $t8, $t2
    /* 26B8 80001AB8 AD2B0044 */  sw         $t3, 0x44($t1)
  .L80001ABC:
    /* 26BC 80001ABC 8FAC0020 */  lw         $t4, 0x20($sp)
    /* 26C0 80001AC0 318DFFF0 */  andi       $t5, $t4, 0xFFF0
    /* 26C4 80001AC4 AFAD0020 */  sw         $t5, 0x20($sp)
    /* 26C8 80001AC8 3C0E8003 */  lui        $t6, %hi(gViewportDataPtr)
    /* 26CC 80001ACC 8DCEB014 */  lw         $t6, %lo(gViewportDataPtr)($t6)
    /* 26D0 80001AD0 8FA80020 */  lw         $t0, 0x20($sp)
    /* 26D4 80001AD4 3C01FFFF */  lui        $at, (0xFFFF0000 >> 16)
    /* 26D8 80001AD8 8DCF002C */  lw         $t7, 0x2C($t6)
    /* 26DC 80001ADC 01E1C824 */  and        $t9, $t7, $at
    /* 26E0 80001AE0 0328C025 */  or         $t8, $t9, $t0
    /* 26E4 80001AE4 ADD8002C */  sw         $t8, 0x2C($t6)
    /* 26E8 80001AE8 3C0A8003 */  lui        $t2, %hi(gViewportDataPtr)
    /* 26EC 80001AEC 8D4AB014 */  lw         $t2, %lo(gViewportDataPtr)($t2)
    /* 26F0 80001AF0 8FAC0020 */  lw         $t4, 0x20($sp)
    /* 26F4 80001AF4 3C01FFFF */  lui        $at, (0xFFFF0000 >> 16)
    /* 26F8 80001AF8 8D4B0040 */  lw         $t3, 0x40($t2)
    /* 26FC 80001AFC 01614824 */  and        $t1, $t3, $at
    /* 2700 80001B00 012C6825 */  or         $t5, $t1, $t4
    /* 2704 80001B04 AD4D0040 */  sw         $t5, 0x40($t2)
    /* 2708 80001B08 240F0002 */  addiu      $t7, $zero, 0x2
    /* 270C 80001B0C 3C018003 */  lui        $at, %hi(gViewportPendingFrames)
    /* 2710 80001B10 A42FB010 */  sh         $t7, %lo(gViewportPendingFrames)($at)
    /* 2714 80001B14 3C048003 */  lui        $a0, %hi(gViewportDataPtr)
    /* 2718 80001B18 8C84B014 */  lw         $a0, %lo(gViewportDataPtr)($a0)
    /* 271C 80001B1C 0C001F28 */  jal        func_80007CA0
    /* 2720 80001B20 24050050 */   addiu     $a1, $zero, 0x50
    /* 2724 80001B24 0C001C74 */  jal        func_800071D0
    /* 2728 80001B28 8FA40034 */   lw        $a0, 0x34($sp)
    /* 272C 80001B2C 10000001 */  b          .L80001B34
    /* 2730 80001B30 00000000 */   nop
  .L80001B34:
    /* 2734 80001B34 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 2738 80001B38 27BD0038 */  addiu      $sp, $sp, 0x38
    /* 273C 80001B3C 03E00008 */  jr         $ra
    /* 2740 80001B40 00000000 */   nop