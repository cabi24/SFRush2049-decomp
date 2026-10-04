nonmatching func_8001FFA0, 0x54

glabel func_8001FFA0
    /* 20BA0 8001FFA0 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 20BA4 8001FFA4 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 20BA8 8001FFA8 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 20BAC 8001FFAC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 20BB0 8001FFB0 AFA40020 */  sw         $a0, 0x20($sp)
    /* 20BB4 8001FFB4 AFA50024 */  sw         $a1, 0x24($sp)
    /* 20BB8 8001FFB8 11C00009 */  beqz       $t6, .L8001FFE0
    /* 20BBC 8001FFBC 2403FFFF */   addiu     $v1, $zero, -0x1
    /* 20BC0 8001FFC0 0C005165 */  jal        func_80014594
    /* 20BC4 8001FFC4 00000000 */   nop
    /* 20BC8 8001FFC8 8FA40020 */  lw         $a0, 0x20($sp)
    /* 20BCC 8001FFCC 0C006D29 */  jal        func_8001B4A4
    /* 20BD0 8001FFD0 97A50026 */   lhu       $a1, 0x26($sp)
    /* 20BD4 8001FFD4 0C005177 */  jal        func_800145DC
    /* 20BD8 8001FFD8 AFA2001C */   sw        $v0, 0x1C($sp)
    /* 20BDC 8001FFDC 8FA3001C */  lw         $v1, 0x1C($sp)
  .L8001FFE0:
    /* 20BE0 8001FFE0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 20BE4 8001FFE4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 20BE8 8001FFE8 00601025 */  or         $v0, $v1, $zero
    /* 20BEC 8001FFEC 03E00008 */  jr         $ra
    /* 20BF0 8001FFF0 00000000 */   nop
endlabel func_8001FFA0
