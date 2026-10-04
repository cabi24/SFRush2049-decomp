nonmatching func_80022580, 0x2C

glabel func_80022580
    /* 23180 80022580 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 23184 80022584 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 23188 80022588 8CA50000 */  lw         $a1, 0x0($a1)
    /* 2318C 8002258C 00052A02 */  srl        $a1, $a1, 8
    /* 23190 80022590 0C007BE3 */  jal        func_8001EF8C
    /* 23194 80022594 30A500FF */   andi      $a1, $a1, 0xFF
    /* 23198 80022598 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 2319C 8002259C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 231A0 800225A0 00001025 */  or         $v0, $zero, $zero
    /* 231A4 800225A4 03E00008 */  jr         $ra
    /* 231A8 800225A8 00000000 */   nop
endlabel func_80022580
