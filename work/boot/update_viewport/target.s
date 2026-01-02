# Source: 21F0.s
# Address: 0x80001F2C

glabel func_80001F2C
    /* 2B2C 80001F2C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 2B30 80001F30 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 2B34 80001F34 AFA40028 */  sw         $a0, 0x28($sp)
    /* 2B38 80001F38 AFA5002C */  sw         $a1, 0x2C($sp)
    /* 2B3C 80001F3C 3C0E8003 */  lui        $t6, %hi(gViewportDataPtr)
    /* 2B40 80001F40 8DCEB014 */  lw         $t6, %lo(gViewportDataPtr)($t6)
    /* 2B44 80001F44 15C00003 */  bnez       $t6, .L80001F54
    /* 2B48 80001F48 00000000 */   nop
    /* 2B4C 80001F4C 10000063 */  b          .L800020DC
    /* 2B50 80001F50 00000000 */   nop
  .L80001F54:
    /* 2B54 80001F54 3C0F8003 */  lui        $t7, %hi(gViewportLeftEdge)
    /* 2B58 80001F58 85EFECA0 */  lh         $t7, %lo(gViewportLeftEdge)($t7)
    /* 2B5C 80001F5C 8FB80028 */  lw         $t8, 0x28($sp)
    /* 2B60 80001F60 01F8C821 */  addu       $t9, $t7, $t8
    /* 2B64 80001F64 AFB90024 */  sw         $t9, 0x24($sp)
    /* 2B68 80001F68 3C088003 */  lui        $t0, %hi(gViewportTopEdge)
    /* 2B6C 80001F6C 8508ECA2 */  lh         $t0, %lo(gViewportTopEdge)($t0)
    /* 2B70 80001F70 8FA9002C */  lw         $t1, 0x2C($sp)
    /* 2B74 80001F74 01095021 */  addu       $t2, $t0, $t1
    /* 2B78 80001F78 AFAA0020 */  sw         $t2, 0x20($sp)
    /* 2B7C 80001F7C 3C0B8003 */  lui        $t3, %hi(gViewportRightEdge)
    /* 2B80 80001F80 856BECA4 */  lh         $t3, %lo(gViewportRightEdge)($t3)
    /* 2B84 80001F84 8FAC0028 */  lw         $t4, 0x28($sp)
    /* 2B88 80001F88 3C0E8003 */  lui        $t6, %hi(gViewportXOverflow)
    /* 2B8C 80001F8C 85CEECA8 */  lh         $t6, %lo(gViewportXOverflow)($t6)
    /* 2B90 80001F90 016C6821 */  addu       $t5, $t3, $t4
    /* 2B94 80001F94 01AE7821 */  addu       $t7, $t5, $t6
    /* 2B98 80001F98 AFAF001C */  sw         $t7, 0x1C($sp)
    /* 2B9C 80001F9C 3C188003 */  lui        $t8, %hi(gViewportBottomEdge)
    /* 2BA0 80001FA0 8718ECA6 */  lh         $t8, %lo(gViewportBottomEdge)($t8)
    /* 2BA4 80001FA4 8FB9002C */  lw         $t9, 0x2C($sp)
    /* 2BA8 80001FA8 3C098003 */  lui        $t1, %hi(gViewportYOverflow)
    /* 2BAC 80001FAC 8529ECAA */  lh         $t1, %lo(gViewportYOverflow)($t1)
    /* 2BB0 80001FB0 03194021 */  addu       $t0, $t8, $t9
    /* 2BB4 80001FB4 01095021 */  addu       $t2, $t0, $t1
    /* 2BB8 80001FB8 AFAA0018 */  sw         $t2, 0x18($sp)
    /* 2BBC 80001FBC 3C0C8003 */  lui        $t4, %hi(gViewportX)
    /* 2BC0 80001FC0 858CEBF8 */  lh         $t4, %lo(gViewportX)($t4)
    /* 2BC4 80001FC4 3C0E8003 */  lui        $t6, %hi(gViewportBoundsTable)
    /* 2BC8 80001FC8 25CEAFC8 */  addiu      $t6, $t6, %lo(gViewportBoundsTable)
    /* 2BCC 80001FCC 000C6900 */  sll        $t5, $t4, 4
    /* 2BD0 80001FD0 01AE7821 */  addu       $t7, $t5, $t6
    /* 2BD4 80001FD4 85F80000 */  lh         $t8, 0x0($t7)
    /* 2BD8 80001FD8 8FAB0024 */  lw         $t3, 0x24($sp)
    /* 2BDC 80001FDC 030B082A */  slt        $at, $t8, $t3
    /* 2BE0 80001FE0 10200018 */  beqz       $at, .L80002044
    /* 2BE4 80001FE4 00000000 */   nop
    /* 2BE8 80001FE8 85F90002 */  lh         $t9, 0x2($t7)
    /* 2BEC 80001FEC 0179082A */  slt        $at, $t3, $t9
    /* 2BF0 80001FF0 10200014 */  beqz       $at, .L80002044
    /* 2BF4 80001FF4 00000000 */   nop
    /* 2BF8 80001FF8 8FA8001C */  lw         $t0, 0x1C($sp)
    /* 2BFC 80001FFC 85E90004 */  lh         $t1, 0x4($t7)
    /* 2C00 80002000 0128082A */  slt        $at, $t1, $t0
    /* 2C04 80002004 1020000F */  beqz       $at, .L80002044
    /* 2C08 80002008 00000000 */   nop
    /* 2C0C 8000200C 85EA0006 */  lh         $t2, 0x6($t7)
    /* 2C10 80002010 010A082A */  slt        $at, $t0, $t2
    /* 2C14 80002014 1020000B */  beqz       $at, .L80002044
    /* 2C18 80002018 00000000 */   nop
    /* 2C1C 8000201C 3C0C8003 */  lui        $t4, %hi(gViewportX)
    /* 2C20 80002020 858CEBF8 */  lh         $t4, %lo(gViewportX)($t4)
    /* 2C24 80002024 3C0E8003 */  lui        $t6, %hi(gViewportOffsetX)
    /* 2C28 80002028 8FB80028 */  lw         $t8, 0x28($sp)
    /* 2C2C 8000202C 000C68C0 */  sll        $t5, $t4, 3
    /* 2C30 80002030 01CD7021 */  addu       $t6, $t6, $t5
    /* 2C34 80002034 85CEAFFC */  lh         $t6, %lo(gViewportOffsetX)($t6)
    /* 2C38 80002038 3C018003 */  lui        $at, %hi(gViewportScaleX)
    /* 2C3C 8000203C 01D85821 */  addu       $t3, $t6, $t8
    /* 2C40 80002040 A42BECB0 */  sh         $t3, %lo(gViewportScaleX)($at)
  .L80002044:
    /* 2C44 80002044 3C098003 */  lui        $t1, %hi(gViewportX)
    /* 2C48 80002048 8529EBF8 */  lh         $t1, %lo(gViewportX)($t1)
    /* 2C4C 8000204C 3C088003 */  lui        $t0, %hi(gViewportBoundsTable)
    /* 2C50 80002050 2508AFC8 */  addiu      $t0, $t0, %lo(gViewportBoundsTable)
    /* 2C54 80002054 00097900 */  sll        $t7, $t1, 4
    /* 2C58 80002058 01E85021 */  addu       $t2, $t7, $t0
    /* 2C5C 8000205C 854C0008 */  lh         $t4, 0x8($t2)
    /* 2C60 80002060 8FB90020 */  lw         $t9, 0x20($sp)
    /* 2C64 80002064 0199082A */  slt        $at, $t4, $t9
    /* 2C68 80002068 10200018 */  beqz       $at, .L800020CC
    /* 2C6C 8000206C 00000000 */   nop
    /* 2C70 80002070 854D000A */  lh         $t5, 0xA($t2)
    /* 2C74 80002074 032D082A */  slt        $at, $t9, $t5
    /* 2C78 80002078 10200014 */  beqz       $at, .L800020CC
    /* 2C7C 8000207C 00000000 */   nop
    /* 2C80 80002080 8FAE0018 */  lw         $t6, 0x18($sp)
    /* 2C84 80002084 8558000C */  lh         $t8, 0xC($t2)
    /* 2C88 80002088 030E082A */  slt        $at, $t8, $t6
    /* 2C8C 8000208C 1020000F */  beqz       $at, .L800020CC
    /* 2C90 80002090 00000000 */   nop
    /* 2C94 80002094 854B000E */  lh         $t3, 0xE($t2)
    /* 2C98 80002098 01CB082A */  slt        $at, $t6, $t3
    /* 2C9C 8000209C 1020000B */  beqz       $at, .L800020CC
    /* 2CA0 800020A0 00000000 */   nop
    /* 2CA4 800020A4 3C098003 */  lui        $t1, %hi(gViewportX)
    /* 2CA8 800020A8 8529EBF8 */  lh         $t1, %lo(gViewportX)($t1)
    /* 2CAC 800020AC 3C088003 */  lui        $t0, %hi(gViewportOffsetY)
    /* 2CB0 800020B0 8FAC002C */  lw         $t4, 0x2C($sp)
    /* 2CB4 800020B4 000978C0 */  sll        $t7, $t1, 3
    /* 2CB8 800020B8 010F4021 */  addu       $t0, $t0, $t7
    /* 2CBC 800020BC 8508AFFE */  lh         $t0, %lo(gViewportOffsetY)($t0)
    /* 2CC0 800020C0 3C018003 */  lui        $at, %hi(gViewportScaleY)
    /* 2CC4 800020C4 010CC821 */  addu       $t9, $t0, $t4
    /* 2CC8 800020C8 A439ECB2 */  sh         $t9, %lo(gViewportScaleY)($at)
  .L800020CC:
    /* 2CCC 800020CC 0C00057C */  jal        func_800015F0
    /* 2CD0 800020D0 00000000 */   nop
    /* 2CD4 800020D4 10000001 */  b          .L800020DC
    /* 2CD8 800020D8 00000000 */   nop
  .L800020DC:
    /* 2CDC 800020DC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 2CE0 800020E0 27BD0028 */  addiu      $sp, $sp, 0x28
    /* 2CE4 800020E4 03E00008 */  jr         $ra
    /* 2CE8 800020E8 00000000 */   nop