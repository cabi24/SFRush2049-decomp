# Source: 21F0.s
# Address: 0x80001B44

glabel func_80001B44
    /* 2744 80001B44 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 2748 80001B48 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 274C 80001B4C AFA40018 */  sw         $a0, 0x18($sp)
    /* 2750 80001B50 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 2754 80001B54 AFA60020 */  sw         $a2, 0x20($sp)
    /* 2758 80001B58 AFA70024 */  sw         $a3, 0x24($sp)
    /* 275C 80001B5C 8FAE0018 */  lw         $t6, 0x18($sp)
    /* 2760 80001B60 15C0000A */  bnez       $t6, .L80001B8C
    /* 2764 80001B64 00000000 */   nop
    /* 2768 80001B68 3C014248 */  lui        $at, (0x42480000 >> 16)
    /* 276C 80001B6C 44812000 */  mtc1       $at, $ft0
    /* 2770 80001B70 3C018003 */  lui        $at, %hi(gScaleTicksPerSecond)
    /* 2774 80001B74 E424AFB4 */  swc1       $ft0, %lo(gScaleTicksPerSecond)($at)
    /* 2778 80001B78 3C018003 */  lui        $at, %hi(gViewportFloatC)
    /* 277C 80001B7C C426D4C8 */  lwc1       $ft1, %lo(gViewportFloatC)($at)
    /* 2780 80001B80 3C018003 */  lui        $at, %hi(gScaleSecondsPerTick)
    /* 2784 80001B84 10000009 */  b          .L80001BAC
    /* 2788 80001B88 E426AFB8 */   swc1      $ft1, %lo(gScaleSecondsPerTick)($at)
  .L80001B8C:
    /* 278C 80001B8C 3C014270 */  lui        $at, (0x42700000 >> 16)
    /* 2790 80001B90 44814000 */  mtc1       $at, $ft2
    /* 2794 80001B94 3C018003 */  lui        $at, %hi(gScaleTicksPerSecond)
    /* 2798 80001B98 E428AFB4 */  swc1       $ft2, %lo(gScaleTicksPerSecond)($at)
    /* 279C 80001B9C 3C018003 */  lui        $at, %hi(gViewportFloatD)
    /* 27A0 80001BA0 C42AD4CC */  lwc1       $ft3, %lo(gViewportFloatD)($at)
    /* 27A4 80001BA4 3C018003 */  lui        $at, %hi(gScaleSecondsPerTick)
    /* 27A8 80001BA8 E42AAFB8 */  swc1       $ft3, %lo(gScaleSecondsPerTick)($at)
  .L80001BAC:
    /* 27AC 80001BAC 8FAF0018 */  lw         $t7, 0x18($sp)
    /* 27B0 80001BB0 3C018003 */  lui        $at, %hi(gViewportX)
    /* 27B4 80001BB4 A42FEBF8 */  sh         $t7, %lo(gViewportX)($at)
    /* 27B8 80001BB8 8FB8001C */  lw         $t8, 0x1C($sp)
    /* 27BC 80001BBC 3C018003 */  lui        $at, %hi(gViewportY)
    /* 27C0 80001BC0 A438EBFA */  sh         $t8, %lo(gViewportY)($at)
    /* 27C4 80001BC4 8FB9001C */  lw         $t9, 0x1C($sp)
    /* 27C8 80001BC8 3C098003 */  lui        $t1, %hi(gViModeTableBase)
    /* 27CC 80001BCC 2529B1B0 */  addiu      $t1, $t1, %lo(gViModeTableBase)
    /* 27D0 80001BD0 00194080 */  sll        $t0, $t9, 2
    /* 27D4 80001BD4 01194021 */  addu       $t0, $t0, $t9
    /* 27D8 80001BD8 00084100 */  sll        $t0, $t0, 4
    /* 27DC 80001BDC 01095021 */  addu       $t2, $t0, $t1
    /* 27E0 80001BE0 3C018003 */  lui        $at, %hi(gViewportStruct)
    /* 27E4 80001BE4 AC2AEBFC */  sw         $t2, %lo(gViewportStruct)($at)
    /* 27E8 80001BE8 3C0B8003 */  lui        $t3, %hi(gViewportStruct)
    /* 27EC 80001BEC 8D6BEBFC */  lw         $t3, %lo(gViewportStruct)($t3)
    /* 27F0 80001BF0 3C018003 */  lui        $at, %hi(gViewportLeftEdge)
    /* 27F4 80001BF4 8D6C001C */  lw         $t4, 0x1C($t3)
    /* 27F8 80001BF8 000C6C02 */  srl        $t5, $t4, 16
    /* 27FC 80001BFC 31AE03FF */  andi       $t6, $t5, 0x3FF
    /* 2800 80001C00 A42EECA0 */  sh         $t6, %lo(gViewportLeftEdge)($at)
    /* 2804 80001C04 3C0F8003 */  lui        $t7, %hi(gViewportStruct)
    /* 2808 80001C08 8DEFEBFC */  lw         $t7, %lo(gViewportStruct)($t7)
    /* 280C 80001C0C 3C018003 */  lui        $at, %hi(gViewportRightEdge)
    /* 2810 80001C10 8DF8001C */  lw         $t8, 0x1C($t7)
    /* 2814 80001C14 331903FF */  andi       $t9, $t8, 0x3FF
    /* 2818 80001C18 A439ECA4 */  sh         $t9, %lo(gViewportRightEdge)($at)
    /* 281C 80001C1C 3C088003 */  lui        $t0, %hi(gViewportStruct)
    /* 2820 80001C20 8D08EBFC */  lw         $t0, %lo(gViewportStruct)($t0)
    /* 2824 80001C24 3C018003 */  lui        $at, %hi(gViewportTopEdge)
    /* 2828 80001C28 8D090030 */  lw         $t1, 0x30($t0)
    /* 282C 80001C2C 00095402 */  srl        $t2, $t1, 16
    /* 2830 80001C30 314B03FF */  andi       $t3, $t2, 0x3FF
    /* 2834 80001C34 A42BECA2 */  sh         $t3, %lo(gViewportTopEdge)($at)
    /* 2838 80001C38 3C0C8003 */  lui        $t4, %hi(gViewportStruct)
    /* 283C 80001C3C 8D8CEBFC */  lw         $t4, %lo(gViewportStruct)($t4)
    /* 2840 80001C40 3C018003 */  lui        $at, %hi(gViewportBottomEdge)
    /* 2844 80001C44 8D8D0030 */  lw         $t5, 0x30($t4)
    /* 2848 80001C48 31AE03FF */  andi       $t6, $t5, 0x3FF
    /* 284C 80001C4C A42EECA6 */  sh         $t6, %lo(gViewportBottomEdge)($at)
    /* 2850 80001C50 3C0F8003 */  lui        $t7, %hi(gViewportDataPtr)
    /* 2854 80001C54 8DEFB014 */  lw         $t7, %lo(gViewportDataPtr)($t7)
    /* 2858 80001C58 15E00022 */  bnez       $t7, .L80001CE4
    /* 285C 80001C5C 00000000 */   nop
    /* 2860 80001C60 3C188003 */  lui        $t8, %hi(gViewportX)
    /* 2864 80001C64 8718EBF8 */  lh         $t8, %lo(gViewportX)($t8)
    /* 2868 80001C68 3C088003 */  lui        $t0, %hi(gViewportOffsetXExtra)
    /* 286C 80001C6C 3C018003 */  lui        $at, %hi(gViewportXOverflow)
    /* 2870 80001C70 0018C8C0 */  sll        $t9, $t8, 3
    /* 2874 80001C74 01194021 */  addu       $t0, $t0, $t9
    /* 2878 80001C78 8508AFF8 */  lh         $t0, %lo(gViewportOffsetXExtra)($t0)
    /* 287C 80001C7C A428ECA8 */  sh         $t0, %lo(gViewportXOverflow)($at)
    /* 2880 80001C80 3C098003 */  lui        $t1, %hi(gViewportX)
    /* 2884 80001C84 8529EBF8 */  lh         $t1, %lo(gViewportX)($t1)
    /* 2888 80001C88 3C0B8003 */  lui        $t3, %hi(gViewportOffsetYExtra)
    /* 288C 80001C8C 3C018003 */  lui        $at, %hi(gViewportYOverflow)
    /* 2890 80001C90 000950C0 */  sll        $t2, $t1, 3
    /* 2894 80001C94 016A5821 */  addu       $t3, $t3, $t2
    /* 2898 80001C98 856BAFFA */  lh         $t3, %lo(gViewportOffsetYExtra)($t3)
    /* 289C 80001C9C A42BECAA */  sh         $t3, %lo(gViewportYOverflow)($at)
    /* 28A0 80001CA0 3C0C8003 */  lui        $t4, %hi(gViewportX)
    /* 28A4 80001CA4 858CEBF8 */  lh         $t4, %lo(gViewportX)($t4)
    /* 28A8 80001CA8 3C0E8003 */  lui        $t6, %hi(gViewportOffsetX)
    /* 28AC 80001CAC 3C018003 */  lui        $at, %hi(gViewportScaleX)
    /* 28B0 80001CB0 000C68C0 */  sll        $t5, $t4, 3
    /* 28B4 80001CB4 01CD7021 */  addu       $t6, $t6, $t5
    /* 28B8 80001CB8 85CEAFFC */  lh         $t6, %lo(gViewportOffsetX)($t6)
    /* 28BC 80001CBC A42EECB0 */  sh         $t6, %lo(gViewportScaleX)($at)
    /* 28C0 80001CC0 3C0F8003 */  lui        $t7, %hi(gViewportX)
    /* 28C4 80001CC4 85EFEBF8 */  lh         $t7, %lo(gViewportX)($t7)
    /* 28C8 80001CC8 3C198003 */  lui        $t9, %hi(gViewportOffsetY)
    /* 28CC 80001CCC 3C018003 */  lui        $at, %hi(gViewportScaleY)
    /* 28D0 80001CD0 000FC0C0 */  sll        $t8, $t7, 3
    /* 28D4 80001CD4 0338C821 */  addu       $t9, $t9, $t8
    /* 28D8 80001CD8 8739AFFE */  lh         $t9, %lo(gViewportOffsetY)($t9)
    /* 28DC 80001CDC 10000012 */  b          .L80001D28
    /* 28E0 80001CE0 A439ECB2 */   sh        $t9, %lo(gViewportScaleY)($at)
  .L80001CE4:
    /* 28E4 80001CE4 8FA80020 */  lw         $t0, 0x20($sp)
    /* 28E8 80001CE8 3C098003 */  lui        $t1, %hi(gViewportScale)
    /* 28EC 80001CEC 8529ECAC */  lh         $t1, %lo(gViewportScale)($t1)
    /* 28F0 80001CF0 44888000 */  mtc1       $t0, $ft4
    /* 28F4 80001CF4 3C0B8003 */  lui        $t3, %hi(gViewportScaleYAlt)
    /* 28F8 80001CF8 856BECAE */  lh         $t3, %lo(gViewportScaleYAlt)($t3)
    /* 28FC 80001CFC 8FAA0024 */  lw         $t2, 0x24($sp)
    /* 2900 80001D00 468084A0 */  cvt.s.w    $ft5, $ft4
    /* 2904 80001D04 44892000 */  mtc1       $t1, $ft0
    /* 2908 80001D08 448B8000 */  mtc1       $t3, $ft4
    /* 290C 80001D0C 448A4000 */  mtc1       $t2, $ft2
    /* 2910 80001D10 468021A0 */  cvt.s.w    $ft1, $ft0
    /* 2914 80001D14 46808120 */  cvt.s.w    $ft0, $ft4
    /* 2918 80001D18 468042A0 */  cvt.s.w    $ft3, $ft2
    /* 291C 80001D1C 46069303 */  div.s      $fa0, $ft5, $ft1
    /* 2920 80001D20 0C029D42 */  jal        func_800A7508
    /* 2924 80001D24 46045383 */   div.s     $fa1, $ft3, $ft0
  .L80001D28:
    /* 2928 80001D28 8FAC0020 */  lw         $t4, 0x20($sp)
    /* 292C 80001D2C 3C018003 */  lui        $at, %hi(gViewportScale)
    /* 2930 80001D30 A42CECAC */  sh         $t4, %lo(gViewportScale)($at)
    /* 2934 80001D34 8FAD0024 */  lw         $t5, 0x24($sp)
    /* 2938 80001D38 3C018003 */  lui        $at, %hi(gViewportScaleYAlt)
    /* 293C 80001D3C A42DECAE */  sh         $t5, %lo(gViewportScaleYAlt)($at)
    /* 2940 80001D40 0C00057C */  jal        func_800015F0
    /* 2944 80001D44 00000000 */   nop
    /* 2948 80001D48 10000001 */  b          .L80001D50
    /* 294C 80001D4C 00000000 */   nop
  .L80001D50:
    /* 2950 80001D50 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 2954 80001D54 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 2958 80001D58 03E00008 */  jr         $ra
    /* 295C 80001D5C 00000000 */   nop