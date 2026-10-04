nonmatching func_800118C0, 0x50

glabel func_800118C0
    /* 124C0 800118C0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 124C4 800118C4 3C0E8004 */  lui        $t6, %hi(D_800382CD)
    /* 124C8 800118C8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 124CC 800118CC 25CE82CD */  addiu      $t6, $t6, %lo(D_800382CD)
    /* 124D0 800118D0 91CF0000 */  lbu        $t7, 0x0($t6)
    /* 124D4 800118D4 3C198004 */  lui        $t9, %hi(D_80038004)
    /* 124D8 800118D8 51E0000A */  beql       $t7, $zero, .L80011904
    /* 124DC 800118DC 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 124E0 800118E0 8F398004 */  lw         $t9, %lo(D_80038004)($t9)
    /* 124E4 800118E4 0320F809 */  jalr       $t9
    /* 124E8 800118E8 00000000 */   nop
    /* 124EC 800118EC 0C004044 */  jal        osYieldThread
    /* 124F0 800118F0 00000000 */   nop
    /* 124F4 800118F4 3C188004 */  lui        $t8, %hi(D_800382CD)
    /* 124F8 800118F8 271882CD */  addiu      $t8, $t8, %lo(D_800382CD)
    /* 124FC 800118FC A3000000 */  sb         $zero, 0x0($t8)
    /* 12500 80011900 8FBF0014 */  lw         $ra, 0x14($sp)
  .L80011904:
    /* 12504 80011904 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 12508 80011908 03E00008 */  jr         $ra
    /* 1250C 8001190C 00000000 */   nop
endlabel func_800118C0
