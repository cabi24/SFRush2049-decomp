# Source: 21F0.s
# Address: 0x80001D60

glabel func_80001D60
    /* 2960 80001D60 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 2964 80001D64 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 2968 80001D68 3C0E8003 */  lui        $t6, %hi(gViewportPendingFrames)
    /* 296C 80001D6C 85CEB010 */  lh         $t6, %lo(gViewportPendingFrames)($t6)
    /* 2970 80001D70 15C00003 */  bnez       $t6, .L80001D80
    /* 2974 80001D74 00000000 */   nop
    /* 2978 80001D78 1000001C */  b          .L80001DEC
    /* 297C 80001D7C 00000000 */   nop
  .L80001D80:
    /* 2980 80001D80 3C0F8003 */  lui        $t7, %hi(gViewportPendingFrames)
    /* 2984 80001D84 85EFB010 */  lh         $t7, %lo(gViewportPendingFrames)($t7)
    /* 2988 80001D88 3C018003 */  lui        $at, %hi(gViewportPendingFrames)
    /* 298C 80001D8C 25F8FFFF */  addiu      $t8, $t7, -0x1
    /* 2990 80001D90 0018CC00 */  sll        $t9, $t8, 16
    /* 2994 80001D94 00194403 */  sra        $t0, $t9, 16
    /* 2998 80001D98 15000012 */  bnez       $t0, .L80001DE4
    /* 299C 80001D9C A438B010 */   sh        $t8, %lo(gViewportPendingFrames)($at)
    /* 29A0 80001DA0 3C098003 */  lui        $t1, %hi(gViewportStruct)
    /* 29A4 80001DA4 3C0C8003 */  lui        $t4, %hi(gViewportDataPtr)
    /* 29A8 80001DA8 8D8CB014 */  lw         $t4, %lo(gViewportDataPtr)($t4)
    /* 29AC 80001DAC 8D29EBFC */  lw         $t1, %lo(gViewportStruct)($t1)
    /* 29B0 80001DB0 8D8D002C */  lw         $t5, 0x2C($t4)
    /* 29B4 80001DB4 8D2A002C */  lw         $t2, 0x2C($t1)
    /* 29B8 80001DB8 31AEFFFF */  andi       $t6, $t5, 0xFFFF
    /* 29BC 80001DBC 314BFFFF */  andi       $t3, $t2, 0xFFFF
    /* 29C0 80001DC0 156E0003 */  bne        $t3, $t6, .L80001DD0
    /* 29C4 80001DC4 00000000 */   nop
    /* 29C8 80001DC8 0C001B68 */  jal        func_80006DA0
    /* 29CC 80001DCC 00002025 */   or        $a0, $zero, $zero
  .L80001DD0:
    /* 29D0 80001DD0 3C048003 */  lui        $a0, %hi(gViewportDataPtr)
    /* 29D4 80001DD4 0C001B50 */  jal        func_80006D40
    /* 29D8 80001DD8 8C84B014 */   lw        $a0, %lo(gViewportDataPtr)($a0)
    /* 29DC 80001DDC 0C001F48 */  jal        func_80007D20
    /* 29E0 80001DE0 240400AA */   addiu     $a0, $zero, 0xAA
  .L80001DE4:
    /* 29E4 80001DE4 10000001 */  b          .L80001DEC
    /* 29E8 80001DE8 00000000 */   nop
  .L80001DEC:
    /* 29EC 80001DEC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 29F0 80001DF0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 29F4 80001DF4 03E00008 */  jr         $ra
    /* 29F8 80001DF8 00000000 */   nop