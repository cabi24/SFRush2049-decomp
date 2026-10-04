nonmatching func_8001B744, 0x7C

glabel func_8001B744
    /* 1C344 8001B744 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 1C348 8001B748 AFB10018 */  sw         $s1, 0x18($sp)
    /* 1C34C 8001B74C AFB00014 */  sw         $s0, 0x14($sp)
    /* 1C350 8001B750 00808025 */  or         $s0, $a0, $zero
    /* 1C354 8001B754 00A08825 */  or         $s1, $a1, $zero
    /* 1C358 8001B758 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 1C35C 8001B75C 02203025 */  or         $a2, $s1, $zero
    /* 1C360 8001B760 02002825 */  or         $a1, $s0, $zero
    /* 1C364 8001B764 0C00836A */  jal        func_80020DA8
    /* 1C368 8001B768 24040007 */   addiu     $a0, $zero, 0x7
    /* 1C36C 8001B76C 2404000A */  addiu      $a0, $zero, 0xA
    /* 1C370 8001B770 02002825 */  or         $a1, $s0, $zero
    /* 1C374 8001B774 0C00836A */  jal        func_80020DA8
    /* 1C378 8001B778 02203025 */   or        $a2, $s1, $zero
    /* 1C37C 8001B77C 2404005B */  addiu      $a0, $zero, 0x5B
    /* 1C380 8001B780 02002825 */  or         $a1, $s0, $zero
    /* 1C384 8001B784 0C00836A */  jal        func_80020DA8
    /* 1C388 8001B788 02203025 */   or        $a2, $s1, $zero
    /* 1C38C 8001B78C 24040080 */  addiu      $a0, $zero, 0x80
    /* 1C390 8001B790 02002825 */  or         $a1, $s0, $zero
    /* 1C394 8001B794 0C00836A */  jal        func_80020DA8
    /* 1C398 8001B798 02203025 */   or        $a2, $s1, $zero
    /* 1C39C 8001B79C 24040084 */  addiu      $a0, $zero, 0x84
    /* 1C3A0 8001B7A0 02002825 */  or         $a1, $s0, $zero
    /* 1C3A4 8001B7A4 0C00836A */  jal        func_80020DA8
    /* 1C3A8 8001B7A8 02203025 */   or        $a2, $s1, $zero
    /* 1C3AC 8001B7AC 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 1C3B0 8001B7B0 8FB00014 */  lw         $s0, 0x14($sp)
    /* 1C3B4 8001B7B4 8FB10018 */  lw         $s1, 0x18($sp)
    /* 1C3B8 8001B7B8 03E00008 */  jr         $ra
    /* 1C3BC 8001B7BC 27BD0020 */   addiu     $sp, $sp, 0x20
endlabel func_8001B744
