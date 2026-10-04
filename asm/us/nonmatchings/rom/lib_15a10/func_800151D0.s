nonmatching func_800151D0, 0x58

glabel func_800151D0
    /* 15DD0 800151D0 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 15DD4 800151D4 AFBF0024 */  sw         $ra, 0x24($sp)
    /* 15DD8 800151D8 AFB20020 */  sw         $s2, 0x20($sp)
    /* 15DDC 800151DC AFB1001C */  sw         $s1, 0x1C($sp)
    /* 15DE0 800151E0 AFB00018 */  sw         $s0, 0x18($sp)
    /* 15DE4 800151E4 94900000 */  lhu        $s0, 0x0($a0)
    /* 15DE8 800151E8 3412FFFF */  ori        $s2, $zero, 0xFFFF
    /* 15DEC 800151EC 00808825 */  or         $s1, $a0, $zero
    /* 15DF0 800151F0 52500008 */  beql       $s2, $s0, .L80015214
    /* 15DF4 800151F4 8FBF0024 */   lw        $ra, 0x24($sp)
  .L800151F8:
    /* 15DF8 800151F8 0C005703 */  jal        func_80015C0C
    /* 15DFC 800151FC 3204FFFF */   andi      $a0, $s0, 0xFFFF
    /* 15E00 80015200 96300002 */  lhu        $s0, 0x2($s1)
    /* 15E04 80015204 26310002 */  addiu      $s1, $s1, 0x2
    /* 15E08 80015208 1650FFFB */  bne        $s2, $s0, .L800151F8
    /* 15E0C 8001520C 00000000 */   nop
    /* 15E10 80015210 8FBF0024 */  lw         $ra, 0x24($sp)
  .L80015214:
    /* 15E14 80015214 8FB00018 */  lw         $s0, 0x18($sp)
    /* 15E18 80015218 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 15E1C 8001521C 8FB20020 */  lw         $s2, 0x20($sp)
    /* 15E20 80015220 03E00008 */  jr         $ra
    /* 15E24 80015224 27BD0028 */   addiu     $sp, $sp, 0x28
endlabel func_800151D0
