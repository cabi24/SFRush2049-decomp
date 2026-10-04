nonmatching func_800150C8, 0x58

glabel func_800150C8
    /* 15CC8 800150C8 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 15CCC 800150CC AFBF0024 */  sw         $ra, 0x24($sp)
    /* 15CD0 800150D0 AFB20020 */  sw         $s2, 0x20($sp)
    /* 15CD4 800150D4 AFB1001C */  sw         $s1, 0x1C($sp)
    /* 15CD8 800150D8 AFB00018 */  sw         $s0, 0x18($sp)
    /* 15CDC 800150DC 94900000 */  lhu        $s0, 0x0($a0)
    /* 15CE0 800150E0 3412FFFF */  ori        $s2, $zero, 0xFFFF
    /* 15CE4 800150E4 00808825 */  or         $s1, $a0, $zero
    /* 15CE8 800150E8 52500008 */  beql       $s2, $s0, .L8001510C
    /* 15CEC 800150EC 8FBF0024 */   lw        $ra, 0x24($sp)
  .L800150F0:
    /* 15CF0 800150F0 0C005A66 */  jal        func_80016998
    /* 15CF4 800150F4 3204FFFF */   andi      $a0, $s0, 0xFFFF
    /* 15CF8 800150F8 96300002 */  lhu        $s0, 0x2($s1)
    /* 15CFC 800150FC 26310002 */  addiu      $s1, $s1, 0x2
    /* 15D00 80015100 1650FFFB */  bne        $s2, $s0, .L800150F0
    /* 15D04 80015104 00000000 */   nop
    /* 15D08 80015108 8FBF0024 */  lw         $ra, 0x24($sp)
  .L8001510C:
    /* 15D0C 8001510C 8FB00018 */  lw         $s0, 0x18($sp)
    /* 15D10 80015110 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 15D14 80015114 8FB20020 */  lw         $s2, 0x20($sp)
    /* 15D18 80015118 03E00008 */  jr         $ra
    /* 15D1C 8001511C 27BD0028 */   addiu     $sp, $sp, 0x28
endlabel func_800150C8
