nonmatching __osContDataCrc, 0x94

glabel __osContDataCrc
    /* 106C0 8000FAC0 00802825 */  or         $a1, $a0, $zero
    /* 106C4 8000FAC4 00001825 */  or         $v1, $zero, $zero
    /* 106C8 8000FAC8 24020020 */  addiu      $v0, $zero, 0x20
  .L8000FACC:
    /* 106CC 8000FACC 24040080 */  addiu      $a0, $zero, 0x80
    /* 106D0 8000FAD0 90A60000 */  lbu        $a2, 0x0($a1)
  .L8000FAD4:
    /* 106D4 8000FAD4 00037040 */  sll        $t6, $v1, 1
    /* 106D8 8000FAD8 00C47824 */  and        $t7, $a2, $a0
    /* 106DC 8000FADC 11E00008 */  beqz       $t7, .L8000FB00
    /* 106E0 8000FAE0 01C01825 */   or        $v1, $t6, $zero
    /* 106E4 8000FAE4 31D80100 */  andi       $t8, $t6, 0x100
    /* 106E8 8000FAE8 13000003 */  beqz       $t8, .L8000FAF8
    /* 106EC 8000FAEC 00000000 */   nop
    /* 106F0 8000FAF0 10000007 */  b          .L8000FB10
    /* 106F4 8000FAF4 39C30084 */   xori      $v1, $t6, 0x84
  .L8000FAF8:
    /* 106F8 8000FAF8 10000005 */  b          .L8000FB10
    /* 106FC 8000FAFC 24630001 */   addiu     $v1, $v1, 0x1
  .L8000FB00:
    /* 10700 8000FB00 30680100 */  andi       $t0, $v1, 0x100
    /* 10704 8000FB04 11000002 */  beqz       $t0, .L8000FB10
    /* 10708 8000FB08 38690085 */   xori      $t1, $v1, 0x85
    /* 1070C 8000FB0C 01201825 */  or         $v1, $t1, $zero
  .L8000FB10:
    /* 10710 8000FB10 00045042 */  srl        $t2, $a0, 1
    /* 10714 8000FB14 1540FFEF */  bnez       $t2, .L8000FAD4
    /* 10718 8000FB18 01402025 */   or        $a0, $t2, $zero
    /* 1071C 8000FB1C 2442FFFF */  addiu      $v0, $v0, -0x1
    /* 10720 8000FB20 1440FFEA */  bnez       $v0, .L8000FACC
    /* 10724 8000FB24 24A50001 */   addiu     $a1, $a1, 0x1
    /* 10728 8000FB28 00035840 */  sll        $t3, $v1, 1
  .L8000FB2C:
    /* 1072C 8000FB2C 316C0100 */  andi       $t4, $t3, 0x100
    /* 10730 8000FB30 11800002 */  beqz       $t4, .L8000FB3C
    /* 10734 8000FB34 01601825 */   or        $v1, $t3, $zero
    /* 10738 8000FB38 39630085 */  xori       $v1, $t3, 0x85
  .L8000FB3C:
    /* 1073C 8000FB3C 24420001 */  addiu      $v0, $v0, 0x1
    /* 10740 8000FB40 2C410008 */  sltiu      $at, $v0, 0x8
    /* 10744 8000FB44 5420FFF9 */  bnel       $at, $zero, .L8000FB2C
    /* 10748 8000FB48 00035840 */   sll       $t3, $v1, 1
    /* 1074C 8000FB4C 03E00008 */  jr         $ra
    /* 10750 8000FB50 306200FF */   andi      $v0, $v1, 0xFF
endlabel __osContDataCrc
    /* 10754 8000FB54 00000000 */  nop
    /* 10758 8000FB58 00000000 */  nop
    /* 1075C 8000FB5C 00000000 */  nop
