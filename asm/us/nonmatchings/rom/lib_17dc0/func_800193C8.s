nonmatching func_800193C8, 0x58

glabel func_800193C8
    /* 19FC8 800193C8 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 19FCC 800193CC 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 19FD0 800193D0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 19FD4 800193D4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 19FD8 800193D8 51C0000D */  beql       $t6, $zero, .L80019410
    /* 19FDC 800193DC 00001025 */   or        $v0, $zero, $zero
    /* 19FE0 800193E0 0C005D91 */  jal        func_80017644
    /* 19FE4 800193E4 00000000 */   nop
    /* 19FE8 800193E8 2401FFFF */  addiu      $at, $zero, -0x1
    /* 19FEC 800193EC 10410007 */  beq        $v0, $at, .L8001940C
    /* 19FF0 800193F0 00027A40 */   sll       $t7, $v0, 9
    /* 19FF4 800193F4 01E27823 */  subu       $t7, $t7, $v0
    /* 19FF8 800193F8 000F78C0 */  sll        $t7, $t7, 3
    /* 19FFC 800193FC 3C028004 */  lui        $v0, %hi(D_80043EB8 + 0xFC4)
    /* 1A000 80019400 004F1021 */  addu       $v0, $v0, $t7
    /* 1A004 80019404 10000002 */  b          .L80019410
    /* 1A008 80019408 90424E7C */   lbu       $v0, %lo(D_80043EB8 + 0xFC4)($v0)
  .L8001940C:
    /* 1A00C 8001940C 00001025 */  or         $v0, $zero, $zero
  .L80019410:
    /* 1A010 80019410 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1A014 80019414 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1A018 80019418 03E00008 */  jr         $ra
    /* 1A01C 8001941C 00000000 */   nop
endlabel func_800193C8
