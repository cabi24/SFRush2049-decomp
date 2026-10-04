nonmatching func_8001989C, 0x2C

glabel func_8001989C
    /* 1A49C 8001989C 8C830000 */  lw         $v1, 0x0($a0)
    /* 1A4A0 800198A0 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1A4A4 800198A4 24020001 */  addiu      $v0, $zero, 0x1
    /* 1A4A8 800198A8 10610005 */  beq        $v1, $at, .L800198C0
    /* 1A4AC 800198AC 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1A4B0 800198B0 00611024 */  and        $v0, $v1, $at
    /* 1A4B4 800198B4 2C420001 */  sltiu      $v0, $v0, 0x1
    /* 1A4B8 800198B8 03E00008 */  jr         $ra
    /* 1A4BC 800198BC 304200FF */   andi      $v0, $v0, 0xFF
  .L800198C0:
    /* 1A4C0 800198C0 03E00008 */  jr         $ra
    /* 1A4C4 800198C4 00000000 */   nop
endlabel func_8001989C
