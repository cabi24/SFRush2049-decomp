nonmatching func_80019AA8, 0x2C

glabel func_80019AA8
    /* 1A6A8 80019AA8 9082004B */  lbu        $v0, 0x4B($a0)
    /* 1A6AC 80019AAC 240100FF */  addiu      $at, $zero, 0xFF
    /* 1A6B0 80019AB0 14410003 */  bne        $v0, $at, .L80019AC0
    /* 1A6B4 80019AB4 00401825 */   or        $v1, $v0, $zero
    /* 1A6B8 80019AB8 10000001 */  b          .L80019AC0
    /* 1A6BC 80019ABC 24030008 */   addiu     $v1, $zero, 0x8
  .L80019AC0:
    /* 1A6C0 80019AC0 00037080 */  sll        $t6, $v1, 2
    /* 1A6C4 80019AC4 3C028005 */  lui        $v0, %hi(D_8004FA20)
    /* 1A6C8 80019AC8 004E1021 */  addu       $v0, $v0, $t6
    /* 1A6CC 80019ACC 03E00008 */  jr         $ra
    /* 1A6D0 80019AD0 8C42FA20 */   lw        $v0, %lo(D_8004FA20)($v0)
endlabel func_80019AA8
