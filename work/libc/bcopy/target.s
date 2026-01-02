# Source: DEB0.s
# Address: 0x8000D2B0

glabel func_8000D2B0
    /* DEB0 8000D2B0 10C0001A */  beqz       $a2, .L8000D31C
    /* DEB4 8000D2B4 00A03825 */   or        $a3, $a1, $zero
    /* DEB8 8000D2B8 10850018 */  beq        $a0, $a1, .L8000D31C
    /* DEBC 8000D2BC 00A4082A */   slt       $at, $a1, $a0
    /* DEC0 8000D2C0 54200008 */  bnel       $at, $zero, .L8000D2E4
    /* DEC4 8000D2C4 28C10010 */   slti      $at, $a2, 0x10
    /* DEC8 8000D2C8 00861020 */  add        $v0, $a0, $a2 /* handwritten instruction */
    /* DECC 8000D2CC 00A2082A */  slt        $at, $a1, $v0
    /* DED0 8000D2D0 50200004 */  beql       $at, $zero, .L8000D2E4
    /* DED4 8000D2D4 28C10010 */   slti      $at, $a2, 0x10
    /* DED8 8000D2D8 1000005B */  b          .L8000D448
    /* DEDC 8000D2DC 28C10010 */   slti      $at, $a2, 0x10
    /* DEE0 8000D2E0 28C10010 */  slti       $at, $a2, 0x10
  .L8000D2E4:
    /* DEE4 8000D2E4 14200005 */  bnez       $at, .L8000D2FC
    /* DEE8 8000D2E8 00000000 */   nop
    /* DEEC 8000D2EC 30820003 */  andi       $v0, $a0, 0x3
    /* DEF0 8000D2F0 30A30003 */  andi       $v1, $a1, 0x3
    /* DEF4 8000D2F4 1043000B */  beq        $v0, $v1, .L8000D324
    /* DEF8 8000D2F8 00000000 */   nop
  .L8000D2FC:
    /* DEFC 8000D2FC 10C00007 */  beqz       $a2, .L8000D31C
    /* DF00 8000D300 00000000 */   nop
    /* DF04 8000D304 00861821 */  addu       $v1, $a0, $a2
  .L8000D308:
    /* DF08 8000D308 80820000 */  lb         $v0, 0x0($a0)
    /* DF0C 8000D30C 24840001 */  addiu      $a0, $a0, 0x1
    /* DF10 8000D310 24A50001 */  addiu      $a1, $a1, 0x1
    /* DF14 8000D314 1483FFFC */  bne        $a0, $v1, .L8000D308
    /* DF18 8000D318 A0A2FFFF */   sb        $v0, -0x1($a1)
  .L8000D31C:
    /* DF1C 8000D31C 03E00008 */  jr         $ra
    /* DF20 8000D320 00E01025 */   or        $v0, $a3, $zero
  .L8000D324:
    /* DF24 8000D324 10400018 */  beqz       $v0, .L8000D388
    /* DF28 8000D328 24010001 */   addiu     $at, $zero, 0x1
    /* DF2C 8000D32C 1041000F */  beq        $v0, $at, .L8000D36C
    /* DF30 8000D330 24010002 */   addiu     $at, $zero, 0x2
    /* DF34 8000D334 50410008 */  beql       $v0, $at, .L8000D358
    /* DF38 8000D338 84820000 */   lh        $v0, 0x0($a0)
    /* DF3C 8000D33C 80820000 */  lb         $v0, 0x0($a0)
    /* DF40 8000D340 24840001 */  addiu      $a0, $a0, 0x1
    /* DF44 8000D344 24A50001 */  addiu      $a1, $a1, 0x1
    /* DF48 8000D348 24C6FFFF */  addiu      $a2, $a2, -0x1
    /* DF4C 8000D34C 1000000E */  b          .L8000D388
    /* DF50 8000D350 A0A2FFFF */   sb        $v0, -0x1($a1)
    /* DF54 8000D354 84820000 */  lh         $v0, 0x0($a0)
  .L8000D358:
    /* DF58 8000D358 24840002 */  addiu      $a0, $a0, 0x2
    /* DF5C 8000D35C 24A50002 */  addiu      $a1, $a1, 0x2
    /* DF60 8000D360 24C6FFFE */  addiu      $a2, $a2, -0x2
    /* DF64 8000D364 10000008 */  b          .L8000D388
    /* DF68 8000D368 A4A2FFFE */   sh        $v0, -0x2($a1)
  .L8000D36C:
    /* DF6C 8000D36C 80820000 */  lb         $v0, 0x0($a0)
    /* DF70 8000D370 84830001 */  lh         $v1, 0x1($a0)
    /* DF74 8000D374 24840003 */  addiu      $a0, $a0, 0x3
    /* DF78 8000D378 24A50003 */  addiu      $a1, $a1, 0x3
    /* DF7C 8000D37C 24C6FFFD */  addiu      $a2, $a2, -0x3
    /* DF80 8000D380 A0A2FFFD */  sb         $v0, -0x3($a1)
    /* DF84 8000D384 A4A3FFFE */  sh         $v1, -0x2($a1)
  .L8000D388:
    /* DF88 8000D388 28C10020 */  slti       $at, $a2, 0x20
    /* DF8C 8000D38C 54200016 */  bnel       $at, $zero, .L8000D3E8
    /* DF90 8000D390 28C10010 */   slti      $at, $a2, 0x10
    /* DF94 8000D394 8C820000 */  lw         $v0, 0x0($a0)
    /* DF98 8000D398 8C830004 */  lw         $v1, 0x4($a0)
    /* DF9C 8000D39C 8C880008 */  lw         $t0, 0x8($a0)
    /* DFA0 8000D3A0 8C89000C */  lw         $t1, 0xC($a0)
    /* DFA4 8000D3A4 8C8A0010 */  lw         $t2, 0x10($a0)
    /* DFA8 8000D3A8 8C8B0014 */  lw         $t3, 0x14($a0)
    /* DFAC 8000D3AC 8C8C0018 */  lw         $t4, 0x18($a0)
    /* DFB0 8000D3B0 8C8D001C */  lw         $t5, 0x1C($a0)
    /* DFB4 8000D3B4 24840020 */  addiu      $a0, $a0, 0x20
    /* DFB8 8000D3B8 24A50020 */  addiu      $a1, $a1, 0x20
    /* DFBC 8000D3BC 24C6FFE0 */  addiu      $a2, $a2, -0x20
    /* DFC0 8000D3C0 ACA2FFE0 */  sw         $v0, -0x20($a1)
    /* DFC4 8000D3C4 ACA3FFE4 */  sw         $v1, -0x1C($a1)
    /* DFC8 8000D3C8 ACA8FFE8 */  sw         $t0, -0x18($a1)
    /* DFCC 8000D3CC ACA9FFEC */  sw         $t1, -0x14($a1)
    /* DFD0 8000D3D0 ACAAFFF0 */  sw         $t2, -0x10($a1)
    /* DFD4 8000D3D4 ACABFFF4 */  sw         $t3, -0xC($a1)
    /* DFD8 8000D3D8 ACACFFF8 */  sw         $t4, -0x8($a1)
    /* DFDC 8000D3DC 1000FFEA */  b          .L8000D388
    /* DFE0 8000D3E0 ACADFFFC */   sw        $t5, -0x4($a1)
  .L8000D3E4:
    /* DFE4 8000D3E4 28C10010 */  slti       $at, $a2, 0x10
  .L8000D3E8:
    /* DFE8 8000D3E8 5420000E */  bnel       $at, $zero, .L8000D424
    /* DFEC 8000D3EC 28C10004 */   slti      $at, $a2, 0x4
    /* DFF0 8000D3F0 8C820000 */  lw         $v0, 0x0($a0)
    /* DFF4 8000D3F4 8C830004 */  lw         $v1, 0x4($a0)
    /* DFF8 8000D3F8 8C880008 */  lw         $t0, 0x8($a0)
    /* DFFC 8000D3FC 8C89000C */  lw         $t1, 0xC($a0)
    /* E000 8000D400 24840010 */  addiu      $a0, $a0, 0x10
    /* E004 8000D404 24A50010 */  addiu      $a1, $a1, 0x10
    /* E008 8000D408 24C6FFF0 */  addiu      $a2, $a2, -0x10
    /* E00C 8000D40C ACA2FFF0 */  sw         $v0, -0x10($a1)
    /* E010 8000D410 ACA3FFF4 */  sw         $v1, -0xC($a1)
    /* E014 8000D414 ACA8FFF8 */  sw         $t0, -0x8($a1)
    /* E018 8000D418 1000FFF2 */  b          .L8000D3E4
    /* E01C 8000D41C ACA9FFFC */   sw        $t1, -0x4($a1)
  .L8000D420:
    /* E020 8000D420 28C10004 */  slti       $at, $a2, 0x4
  .L8000D424:
    /* E024 8000D424 1420FFB5 */  bnez       $at, .L8000D2FC
    /* E028 8000D428 00000000 */   nop
    /* E02C 8000D42C 8C820000 */  lw         $v0, 0x0($a0)
    /* E030 8000D430 24840004 */  addiu      $a0, $a0, 0x4
    /* E034 8000D434 24A50004 */  addiu      $a1, $a1, 0x4
    /* E038 8000D438 24C6FFFC */  addiu      $a2, $a2, -0x4
    /* E03C 8000D43C 1000FFF8 */  b          .L8000D420
    /* E040 8000D440 ACA2FFFC */   sw        $v0, -0x4($a1)
    /* E044 8000D444 28C10010 */  slti       $at, $a2, 0x10
  .L8000D448:
    /* E048 8000D448 00862020 */  add        $a0, $a0, $a2 /* handwritten instruction */
    /* E04C 8000D44C 14200005 */  bnez       $at, .L8000D464
    /* E050 8000D450 00A62820 */   add       $a1, $a1, $a2 /* handwritten instruction */
    /* E054 8000D454 30820003 */  andi       $v0, $a0, 0x3
    /* E058 8000D458 30A30003 */  andi       $v1, $a1, 0x3
    /* E05C 8000D45C 1043000D */  beq        $v0, $v1, .L8000D494
    /* E060 8000D460 00000000 */   nop
  .L8000D464:
    /* E064 8000D464 10C0FFAD */  beqz       $a2, .L8000D31C
    /* E068 8000D468 00000000 */   nop
    /* E06C 8000D46C 2484FFFF */  addiu      $a0, $a0, -0x1
    /* E070 8000D470 24A5FFFF */  addiu      $a1, $a1, -0x1
    /* E074 8000D474 00861823 */  subu       $v1, $a0, $a2
  .L8000D478:
    /* E078 8000D478 80820000 */  lb         $v0, 0x0($a0)
    /* E07C 8000D47C 2484FFFF */  addiu      $a0, $a0, -0x1
    /* E080 8000D480 24A5FFFF */  addiu      $a1, $a1, -0x1
    /* E084 8000D484 1483FFFC */  bne        $a0, $v1, .L8000D478
    /* E088 8000D488 A0A20001 */   sb        $v0, 0x1($a1)
    /* E08C 8000D48C 03E00008 */  jr         $ra
    /* E090 8000D490 00E01025 */   or        $v0, $a3, $zero
  .L8000D494:
    /* E094 8000D494 10400018 */  beqz       $v0, .L8000D4F8
    /* E098 8000D498 24010003 */   addiu     $at, $zero, 0x3
    /* E09C 8000D49C 1041000F */  beq        $v0, $at, .L8000D4DC
    /* E0A0 8000D4A0 24010002 */   addiu     $at, $zero, 0x2
    /* E0A4 8000D4A4 50410008 */  beql       $v0, $at, .L8000D4C8
    /* E0A8 8000D4A8 8482FFFE */   lh        $v0, -0x2($a0)
    /* E0AC 8000D4AC 8082FFFF */  lb         $v0, -0x1($a0)
    /* E0B0 8000D4B0 2484FFFF */  addiu      $a0, $a0, -0x1
    /* E0B4 8000D4B4 24A5FFFF */  addiu      $a1, $a1, -0x1
    /* E0B8 8000D4B8 24C6FFFF */  addiu      $a2, $a2, -0x1
    /* E0BC 8000D4BC 1000000E */  b          .L8000D4F8
    /* E0C0 8000D4C0 A0A20000 */   sb        $v0, 0x0($a1)
    /* E0C4 8000D4C4 8482FFFE */  lh         $v0, -0x2($a0)
  .L8000D4C8:
    /* E0C8 8000D4C8 2484FFFE */  addiu      $a0, $a0, -0x2
    /* E0CC 8000D4CC 24A5FFFE */  addiu      $a1, $a1, -0x2
    /* E0D0 8000D4D0 24C6FFFE */  addiu      $a2, $a2, -0x2
    /* E0D4 8000D4D4 10000008 */  b          .L8000D4F8
    /* E0D8 8000D4D8 A4A20000 */   sh        $v0, 0x0($a1)
  .L8000D4DC:
    /* E0DC 8000D4DC 8082FFFF */  lb         $v0, -0x1($a0)
    /* E0E0 8000D4E0 8483FFFD */  lh         $v1, -0x3($a0)
    /* E0E4 8000D4E4 2484FFFD */  addiu      $a0, $a0, -0x3
    /* E0E8 8000D4E8 24A5FFFD */  addiu      $a1, $a1, -0x3
    /* E0EC 8000D4EC 24C6FFFD */  addiu      $a2, $a2, -0x3
    /* E0F0 8000D4F0 A0A20002 */  sb         $v0, 0x2($a1)
    /* E0F4 8000D4F4 A4A30000 */  sh         $v1, 0x0($a1)
  .L8000D4F8:
    /* E0F8 8000D4F8 28C10020 */  slti       $at, $a2, 0x20
    /* E0FC 8000D4FC 54200016 */  bnel       $at, $zero, .L8000D558
    /* E100 8000D500 28C10010 */   slti      $at, $a2, 0x10
    /* E104 8000D504 8C82FFFC */  lw         $v0, -0x4($a0)
    /* E108 8000D508 8C83FFF8 */  lw         $v1, -0x8($a0)
    /* E10C 8000D50C 8C88FFF4 */  lw         $t0, -0xC($a0)
    /* E110 8000D510 8C89FFF0 */  lw         $t1, -0x10($a0)
    /* E114 8000D514 8C8AFFEC */  lw         $t2, -0x14($a0)
    /* E118 8000D518 8C8BFFE8 */  lw         $t3, -0x18($a0)
    /* E11C 8000D51C 8C8CFFE4 */  lw         $t4, -0x1C($a0)
    /* E120 8000D520 8C8DFFE0 */  lw         $t5, -0x20($a0)
    /* E124 8000D524 2484FFE0 */  addiu      $a0, $a0, -0x20
    /* E128 8000D528 24A5FFE0 */  addiu      $a1, $a1, -0x20
    /* E12C 8000D52C 24C6FFE0 */  addiu      $a2, $a2, -0x20
    /* E130 8000D530 ACA2001C */  sw         $v0, 0x1C($a1)
    /* E134 8000D534 ACA30018 */  sw         $v1, 0x18($a1)
    /* E138 8000D538 ACA80014 */  sw         $t0, 0x14($a1)
    /* E13C 8000D53C ACA90010 */  sw         $t1, 0x10($a1)
    /* E140 8000D540 ACAA000C */  sw         $t2, 0xC($a1)
    /* E144 8000D544 ACAB0008 */  sw         $t3, 0x8($a1)
    /* E148 8000D548 ACAC0004 */  sw         $t4, 0x4($a1)
    /* E14C 8000D54C 1000FFEA */  b          .L8000D4F8
    /* E150 8000D550 ACAD0000 */   sw        $t5, 0x0($a1)
  .L8000D554:
    /* E154 8000D554 28C10010 */  slti       $at, $a2, 0x10
  .L8000D558:
    /* E158 8000D558 5420000E */  bnel       $at, $zero, .L8000D594
    /* E15C 8000D55C 28C10004 */   slti      $at, $a2, 0x4
    /* E160 8000D560 8C82FFFC */  lw         $v0, -0x4($a0)
    /* E164 8000D564 8C83FFF8 */  lw         $v1, -0x8($a0)
    /* E168 8000D568 8C88FFF4 */  lw         $t0, -0xC($a0)
    /* E16C 8000D56C 8C89FFF0 */  lw         $t1, -0x10($a0)
    /* E170 8000D570 2484FFF0 */  addiu      $a0, $a0, -0x10
    /* E174 8000D574 24A5FFF0 */  addiu      $a1, $a1, -0x10
    /* E178 8000D578 24C6FFF0 */  addiu      $a2, $a2, -0x10
    /* E17C 8000D57C ACA2000C */  sw         $v0, 0xC($a1)
    /* E180 8000D580 ACA30008 */  sw         $v1, 0x8($a1)
    /* E184 8000D584 ACA80004 */  sw         $t0, 0x4($a1)
    /* E188 8000D588 1000FFF2 */  b          .L8000D554
    /* E18C 8000D58C ACA90000 */   sw        $t1, 0x0($a1)
  .L8000D590:
    /* E190 8000D590 28C10004 */  slti       $at, $a2, 0x4
  .L8000D594:
    /* E194 8000D594 1420FFB3 */  bnez       $at, .L8000D464
    /* E198 8000D598 00000000 */   nop
    /* E19C 8000D59C 8C82FFFC */  lw         $v0, -0x4($a0)
    /* E1A0 8000D5A0 2484FFFC */  addiu      $a0, $a0, -0x4
    /* E1A4 8000D5A4 24A5FFFC */  addiu      $a1, $a1, -0x4
    /* E1A8 8000D5A8 24C6FFFC */  addiu      $a2, $a2, -0x4
    /* E1AC 8000D5AC 1000FFF8 */  b          .L8000D590
    /* E1B0 8000D5B0 ACA20000 */   sw        $v0, 0x0($a1)