nonmatching func_80020F4C, 0x4C

glabel func_80020F4C
    /* 21B4C 80020F4C AFA50004 */  sw         $a1, 0x4($sp)
    /* 21B50 80020F50 30A500FF */  andi       $a1, $a1, 0xFF
    /* 21B54 80020F54 AFA40000 */  sw         $a0, 0x0($sp)
    /* 21B58 80020F58 AFA60008 */  sw         $a2, 0x8($sp)
    /* 21B5C 80020F5C 240100FF */  addiu      $at, $zero, 0xFF
    /* 21B60 80020F60 30C600FF */  andi       $a2, $a2, 0xFF
    /* 21B64 80020F64 10A10007 */  beq        $a1, $at, .L80020F84
    /* 21B68 80020F68 308400FF */   andi      $a0, $a0, 0xFF
    /* 21B6C 80020F6C 00057100 */  sll        $t6, $a1, 4
    /* 21B70 80020F70 01C47821 */  addu       $t7, $t6, $a0
    /* 21B74 80020F74 3C018005 */  lui        $at, %hi(D_80050C60)
    /* 21B78 80020F78 002F0821 */  addu       $at, $at, $t7
    /* 21B7C 80020F7C 03E00008 */  jr         $ra
    /* 21B80 80020F80 A0260C60 */   sb        $a2, %lo(D_80050C60)($at)
  .L80020F84:
    /* 21B84 80020F84 3C018005 */  lui        $at, %hi(D_80050CE0)
    /* 21B88 80020F88 00240821 */  addu       $at, $at, $a0
    /* 21B8C 80020F8C A0260CE0 */  sb         $a2, %lo(D_80050CE0)($at)
    /* 21B90 80020F90 03E00008 */  jr         $ra
    /* 21B94 80020F94 00000000 */   nop
endlabel func_80020F4C
