nonmatching func_8001C19C, 0x3C

glabel func_8001C19C
    /* 1CD9C 8001C19C 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 1CDA0 8001C1A0 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 1CDA4 8001C1A4 AFA40000 */  sw         $a0, 0x0($sp)
    /* 1CDA8 8001C1A8 AFA50004 */  sw         $a1, 0x4($sp)
    /* 1CDAC 8001C1AC 30A500FF */  andi       $a1, $a1, 0xFF
    /* 1CDB0 8001C1B0 11C00007 */  beqz       $t6, .L8001C1D0
    /* 1CDB4 8001C1B4 308400FF */   andi      $a0, $a0, 0xFF
    /* 1CDB8 8001C1B8 00047880 */  sll        $t7, $a0, 2
    /* 1CDBC 8001C1BC 01E47821 */  addu       $t7, $t7, $a0
    /* 1CDC0 8001C1C0 000F78C0 */  sll        $t7, $t7, 3
    /* 1CDC4 8001C1C4 3C018005 */  lui        $at, %hi(D_8004F314)
    /* 1CDC8 8001C1C8 002F0821 */  addu       $at, $at, $t7
    /* 1CDCC 8001C1CC A025F314 */  sb         $a1, %lo(D_8004F314)($at)
  .L8001C1D0:
    /* 1CDD0 8001C1D0 03E00008 */  jr         $ra
    /* 1CDD4 8001C1D4 00000000 */   nop
endlabel func_8001C19C
