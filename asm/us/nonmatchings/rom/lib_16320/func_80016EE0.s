nonmatching func_80016EE0, 0x78

glabel func_80016EE0
    /* 17AE0 80016EE0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 17AE4 80016EE4 AFA40020 */  sw         $a0, 0x20($sp)
    /* 17AE8 80016EE8 97AE0022 */  lhu        $t6, 0x22($sp)
    /* 17AEC 80016EEC 3C0F8001 */  lui        $t7, %hi(func_80016E40)
    /* 17AF0 80016EF0 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 17AF4 80016EF4 3C018004 */  lui        $at, %hi(D_80042684)
    /* 17AF8 80016EF8 25EF6E40 */  addiu      $t7, $t7, %lo(func_80016E40)
    /* 17AFC 80016EFC 3C048004 */  lui        $a0, %hi(D_80042680)
    /* 17B00 80016F00 3C058004 */  lui        $a1, %hi(D_8003C618)
    /* 17B04 80016F04 3C068004 */  lui        $a2, %hi(D_8003C610)
    /* 17B08 80016F08 8CC6C610 */  lw         $a2, %lo(D_8003C610)($a2)
    /* 17B0C 80016F0C 24A5C618 */  addiu      $a1, $a1, %lo(D_8003C618)
    /* 17B10 80016F10 24842680 */  addiu      $a0, $a0, %lo(D_80042680)
    /* 17B14 80016F14 AFAF0010 */  sw         $t7, 0x10($sp)
    /* 17B18 80016F18 24070008 */  addiu      $a3, $zero, 0x8
    /* 17B1C 80016F1C 0C007A19 */  jal        func_8001E864
    /* 17B20 80016F20 A42E2684 */   sh        $t6, %lo(D_80042684)($at)
    /* 17B24 80016F24 3C018004 */  lui        $at, %hi(D_80042688)
    /* 17B28 80016F28 10400006 */  beqz       $v0, .L80016F44
    /* 17B2C 80016F2C AC222688 */   sw        $v0, %lo(D_80042688)($at)
    /* 17B30 80016F30 3C188004 */  lui        $t8, %hi(D_80042688)
    /* 17B34 80016F34 8F182688 */  lw         $t8, %lo(D_80042688)($t8)
    /* 17B38 80016F38 8B020000 */  lwl        $v0, 0x0($t8)
    /* 17B3C 80016F3C 10000002 */  b          .L80016F48
    /* 17B40 80016F40 9B020003 */   lwr       $v0, 0x3($t8)
  .L80016F44:
    /* 17B44 80016F44 00001025 */  or         $v0, $zero, $zero
  .L80016F48:
    /* 17B48 80016F48 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 17B4C 80016F4C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 17B50 80016F50 03E00008 */  jr         $ra
    /* 17B54 80016F54 00000000 */   nop
endlabel func_80016EE0
