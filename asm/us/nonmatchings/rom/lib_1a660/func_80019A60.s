nonmatching func_80019A60, 0x48

glabel func_80019A60
    /* 1A660 80019A60 AFA50004 */  sw         $a1, 0x4($sp)
    /* 1A664 80019A64 30A500FF */  andi       $a1, $a1, 0xFF
    /* 1A668 80019A68 240100FF */  addiu      $at, $zero, 0xFF
    /* 1A66C 80019A6C 14A10002 */  bne        $a1, $at, .L80019A78
    /* 1A670 80019A70 000470C0 */   sll       $t6, $a0, 3
    /* 1A674 80019A74 24050008 */  addiu      $a1, $zero, 0x8
  .L80019A78:
    /* 1A678 80019A78 000E7880 */  sll        $t7, $t6, 2
    /* 1A67C 80019A7C 01EE7823 */  subu       $t7, $t7, $t6
    /* 1A680 80019A80 240100F0 */  addiu      $at, $zero, 0xF0
    /* 1A684 80019A84 000F7A40 */  sll        $t7, $t7, 9
    /* 1A688 80019A88 01E1001B */  divu       $zero, $t7, $at
    /* 1A68C 80019A8C 3C018005 */  lui        $at, %hi(D_8004FA20)
    /* 1A690 80019A90 0005C880 */  sll        $t9, $a1, 2
    /* 1A694 80019A94 00390821 */  addu       $at, $at, $t9
    /* 1A698 80019A98 0000C012 */  mflo       $t8
    /* 1A69C 80019A9C AC38FA20 */  sw         $t8, %lo(D_8004FA20)($at)
    /* 1A6A0 80019AA0 03E00008 */  jr         $ra
    /* 1A6A4 80019AA4 00000000 */   nop
endlabel func_80019A60
