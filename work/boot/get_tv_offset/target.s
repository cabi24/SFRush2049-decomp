# Source: 21F0.s
# Address: 0x80001DFC

glabel func_80001DFC
    /* 29FC 80001DFC 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* 2A00 80001E00 3C0E8000 */  lui        $t6, %hi(osTvType)
    /* 2A04 80001E04 8DCE0300 */  lw         $t6, %lo(osTvType)($t6)
    /* 2A08 80001E08 24010001 */  addiu      $at, $zero, 0x1
    /* 2A0C 80001E0C 15C10003 */  bne        $t6, $at, .L80001E1C
    /* 2A10 80001E10 00000000 */   nop
    /* 2A14 80001E14 1000000A */  b          .L80001E40
    /* 2A18 80001E18 AFA00004 */   sw        $zero, 0x4($sp)
  .L80001E1C:
    /* 2A1C 80001E1C 3C0F8000 */  lui        $t7, %hi(osTvType)
    /* 2A20 80001E20 8DEF0300 */  lw         $t7, %lo(osTvType)($t7)
    /* 2A24 80001E24 15E00004 */  bnez       $t7, .L80001E38
    /* 2A28 80001E28 00000000 */   nop
    /* 2A2C 80001E2C 2418000E */  addiu      $t8, $zero, 0xE
    /* 2A30 80001E30 10000003 */  b          .L80001E40
    /* 2A34 80001E34 AFB80004 */   sw        $t8, 0x4($sp)
  .L80001E38:
    /* 2A38 80001E38 2419001C */  addiu      $t9, $zero, 0x1C
    /* 2A3C 80001E3C AFB90004 */  sw         $t9, 0x4($sp)
  .L80001E40:
    /* 2A40 80001E40 10000003 */  b          .L80001E50
    /* 2A44 80001E44 8FA20004 */   lw        $v0, 0x4($sp)
    /* 2A48 80001E48 10000001 */  b          .L80001E50
    /* 2A4C 80001E4C 00000000 */   nop
  .L80001E50:
    /* 2A50 80001E50 03E00008 */  jr         $ra
    /* 2A54 80001E54 27BD0008 */   addiu     $sp, $sp, 0x8