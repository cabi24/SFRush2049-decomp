nonmatching func_80022A40, 0x38

glabel func_80022A40
    /* 23640 80022A40 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 23644 80022A44 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 23648 80022A48 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 2364C 80022A4C 00803025 */  or         $a2, $a0, $zero
    /* 23650 80022A50 88840060 */  lwl        $a0, 0x60($a0)
    /* 23654 80022A54 98C40063 */  lwr        $a0, 0x63($a2)
    /* 23658 80022A58 24050001 */  addiu      $a1, $zero, 0x1
    /* 2365C 80022A5C 0C005227 */  jal        func_8001489C
    /* 23660 80022A60 308400FF */   andi      $a0, $a0, 0xFF
    /* 23664 80022A64 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 23668 80022A68 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 2366C 80022A6C 00001025 */  or         $v0, $zero, $zero
    /* 23670 80022A70 03E00008 */  jr         $ra
    /* 23674 80022A74 00000000 */   nop
endlabel func_80022A40
