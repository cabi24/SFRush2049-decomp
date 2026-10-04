nonmatching func_8001EDF4, 0x40

glabel func_8001EDF4
    /* 1F9F4 8001EDF4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1F9F8 8001EDF8 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1F9FC 8001EDFC 10810008 */  beq        $a0, $at, .L8001EE20
    /* 1FA00 8001EE00 AFBF0014 */   sw        $ra, 0x14($sp)
    /* 1FA04 8001EE04 0C007AA8 */  jal        func_8001EAA0
    /* 1FA08 8001EE08 00000000 */   nop
    /* 1FA0C 8001EE0C 10400004 */  beqz       $v0, .L8001EE20
    /* 1FA10 8001EE10 00401825 */   or        $v1, $v0, $zero
    /* 1FA14 8001EE14 8842000C */  lwl        $v0, 0xC($v0)
    /* 1FA18 8001EE18 10000002 */  b          .L8001EE24
    /* 1FA1C 8001EE1C 9862000F */   lwr       $v0, 0xF($v1)
  .L8001EE20:
    /* 1FA20 8001EE20 2402FFFF */  addiu      $v0, $zero, -0x1
  .L8001EE24:
    /* 1FA24 8001EE24 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1FA28 8001EE28 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1FA2C 8001EE2C 03E00008 */  jr         $ra
    /* 1FA30 8001EE30 00000000 */   nop
endlabel func_8001EDF4
