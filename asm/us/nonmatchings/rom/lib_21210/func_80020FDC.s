nonmatching func_80020FDC, 0x4C

glabel func_80020FDC
    /* 21BDC 80020FDC AFA50004 */  sw         $a1, 0x4($sp)
    /* 21BE0 80020FE0 30A500FF */  andi       $a1, $a1, 0xFF
    /* 21BE4 80020FE4 AFA40000 */  sw         $a0, 0x0($sp)
    /* 21BE8 80020FE8 AFA60008 */  sw         $a2, 0x8($sp)
    /* 21BEC 80020FEC 240100FF */  addiu      $at, $zero, 0xFF
    /* 21BF0 80020FF0 30C600FF */  andi       $a2, $a2, 0xFF
    /* 21BF4 80020FF4 10A10007 */  beq        $a1, $at, .L80021014
    /* 21BF8 80020FF8 308400FF */   andi      $a0, $a0, 0xFF
    /* 21BFC 80020FFC 00057100 */  sll        $t6, $a1, 4
    /* 21C00 80021000 01C47821 */  addu       $t7, $t6, $a0
    /* 21C04 80021004 3C018005 */  lui        $at, %hi(D_800560C0)
    /* 21C08 80021008 002F0821 */  addu       $at, $at, $t7
    /* 21C0C 8002100C 03E00008 */  jr         $ra
    /* 21C10 80021010 A02660C0 */   sb        $a2, %lo(D_800560C0)($at)
  .L80021014:
    /* 21C14 80021014 3C018005 */  lui        $at, %hi(D_80056140)
    /* 21C18 80021018 00240821 */  addu       $at, $at, $a0
    /* 21C1C 8002101C A0266140 */  sb         $a2, %lo(D_80056140)($at)
    /* 21C20 80021020 03E00008 */  jr         $ra
    /* 21C24 80021024 00000000 */   nop
endlabel func_80020FDC
