nonmatching func_8001FEA4, 0x54

glabel func_8001FEA4
    /* 20AA4 8001FEA4 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 20AA8 8001FEA8 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 20AAC 8001FEAC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 20AB0 8001FEB0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 20AB4 8001FEB4 AFA40020 */  sw         $a0, 0x20($sp)
    /* 20AB8 8001FEB8 AFA50024 */  sw         $a1, 0x24($sp)
    /* 20ABC 8001FEBC 11C00009 */  beqz       $t6, .L8001FEE4
    /* 20AC0 8001FEC0 2403FFFF */   addiu     $v1, $zero, -0x1
    /* 20AC4 8001FEC4 0C005165 */  jal        func_80014594
    /* 20AC8 8001FEC8 00000000 */   nop
    /* 20ACC 8001FECC 8FA40020 */  lw         $a0, 0x20($sp)
    /* 20AD0 8001FED0 0C006DF0 */  jal        func_8001B7C0
    /* 20AD4 8001FED4 93A50027 */   lbu       $a1, 0x27($sp)
    /* 20AD8 8001FED8 0C005177 */  jal        func_800145DC
    /* 20ADC 8001FEDC AFA2001C */   sw        $v0, 0x1C($sp)
    /* 20AE0 8001FEE0 8FA3001C */  lw         $v1, 0x1C($sp)
  .L8001FEE4:
    /* 20AE4 8001FEE4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 20AE8 8001FEE8 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 20AEC 8001FEEC 00601025 */  or         $v0, $v1, $zero
    /* 20AF0 8001FEF0 03E00008 */  jr         $ra
    /* 20AF4 8001FEF4 00000000 */   nop
endlabel func_8001FEA4
