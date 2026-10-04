nonmatching func_8001CC9C, 0x24

glabel func_8001CC9C
    /* 1D89C 8001CC9C AFA40000 */  sw         $a0, 0x0($sp)
    /* 1D8A0 8001CCA0 308400FF */  andi       $a0, $a0, 0xFF
    /* 1D8A4 8001CCA4 28810080 */  slti       $at, $a0, 0x80
    /* 1D8A8 8001CCA8 14200003 */  bnez       $at, .L8001CCB8
    /* 1D8AC 8001CCAC 00801025 */   or        $v0, $a0, $zero
    /* 1D8B0 8001CCB0 03E00008 */  jr         $ra
    /* 1D8B4 8001CCB4 2402007F */   addiu     $v0, $zero, 0x7F
  .L8001CCB8:
    /* 1D8B8 8001CCB8 03E00008 */  jr         $ra
    /* 1D8BC 8001CCBC 00000000 */   nop
endlabel func_8001CC9C
