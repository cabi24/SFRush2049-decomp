nonmatching func_80015178, 0x58

glabel func_80015178
    /* 15D78 80015178 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 15D7C 8001517C AFBF0024 */  sw         $ra, 0x24($sp)
    /* 15D80 80015180 AFB20020 */  sw         $s2, 0x20($sp)
    /* 15D84 80015184 AFB1001C */  sw         $s1, 0x1C($sp)
    /* 15D88 80015188 AFB00018 */  sw         $s0, 0x18($sp)
    /* 15D8C 8001518C 94900000 */  lhu        $s0, 0x0($a0)
    /* 15D90 80015190 3412FFFF */  ori        $s2, $zero, 0xFFFF
    /* 15D94 80015194 00808825 */  or         $s1, $a0, $zero
    /* 15D98 80015198 52500008 */  beql       $s2, $s0, .L800151BC
    /* 15D9C 8001519C 8FBF0024 */   lw        $ra, 0x24($sp)
  .L800151A0:
    /* 15DA0 800151A0 0C005636 */  jal        func_800158D8
    /* 15DA4 800151A4 3204FFFF */   andi      $a0, $s0, 0xFFFF
    /* 15DA8 800151A8 96300002 */  lhu        $s0, 0x2($s1)
    /* 15DAC 800151AC 26310002 */  addiu      $s1, $s1, 0x2
    /* 15DB0 800151B0 1650FFFB */  bne        $s2, $s0, .L800151A0
    /* 15DB4 800151B4 00000000 */   nop
    /* 15DB8 800151B8 8FBF0024 */  lw         $ra, 0x24($sp)
  .L800151BC:
    /* 15DBC 800151BC 8FB00018 */  lw         $s0, 0x18($sp)
    /* 15DC0 800151C0 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 15DC4 800151C4 8FB20020 */  lw         $s2, 0x20($sp)
    /* 15DC8 800151C8 03E00008 */  jr         $ra
    /* 15DCC 800151CC 27BD0028 */   addiu     $sp, $sp, 0x28
endlabel func_80015178
