nonmatching func_80011CD8, 0x4C

glabel func_80011CD8
    /* 128D8 80011CD8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 128DC 80011CDC 3C0E8004 */  lui        $t6, %hi(D_800382CC)
    /* 128E0 80011CE0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 128E4 80011CE4 25CE82CC */  addiu      $t6, $t6, %lo(D_800382CC)
    /* 128E8 80011CE8 91CF0000 */  lbu        $t7, 0x0($t6)
    /* 128EC 80011CEC 3C048004 */  lui        $a0, %hi(D_800382B0)
    /* 128F0 80011CF0 248482B0 */  addiu      $a0, $a0, %lo(D_800382B0)
    /* 128F4 80011CF4 11E00007 */  beqz       $t7, .L80011D14
    /* 128F8 80011CF8 00002825 */   or        $a1, $zero, $zero
    /* 128FC 80011CFC 0C001C9C */  jal        osRecvMesg
    /* 12900 80011D00 24060001 */   addiu     $a2, $zero, 0x1
    /* 12904 80011D04 3C188004 */  lui        $t8, %hi(D_800382CC)
    /* 12908 80011D08 271882CC */  addiu      $t8, $t8, %lo(D_800382CC)
    /* 1290C 80011D0C 0C004044 */  jal        osYieldThread
    /* 12910 80011D10 A3000000 */   sb        $zero, 0x0($t8)
  .L80011D14:
    /* 12914 80011D14 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 12918 80011D18 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1291C 80011D1C 03E00008 */  jr         $ra
    /* 12920 80011D20 00000000 */   nop
endlabel func_80011CD8
