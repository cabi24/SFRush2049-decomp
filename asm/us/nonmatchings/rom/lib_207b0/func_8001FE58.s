nonmatching func_8001FE58, 0x4C

glabel func_8001FE58
    /* 20A58 8001FE58 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 20A5C 8001FE5C 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 20A60 8001FE60 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 20A64 8001FE64 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 20A68 8001FE68 AFA40020 */  sw         $a0, 0x20($sp)
    /* 20A6C 8001FE6C 11C00008 */  beqz       $t6, .L8001FE90
    /* 20A70 8001FE70 2403FFFF */   addiu     $v1, $zero, -0x1
    /* 20A74 8001FE74 0C005165 */  jal        func_80014594
    /* 20A78 8001FE78 00000000 */   nop
    /* 20A7C 8001FE7C 0C006E31 */  jal        func_8001B8C4
    /* 20A80 8001FE80 8FA40020 */   lw        $a0, 0x20($sp)
    /* 20A84 8001FE84 0C005177 */  jal        func_800145DC
    /* 20A88 8001FE88 AFA2001C */   sw        $v0, 0x1C($sp)
    /* 20A8C 8001FE8C 8FA3001C */  lw         $v1, 0x1C($sp)
  .L8001FE90:
    /* 20A90 8001FE90 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 20A94 8001FE94 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 20A98 8001FE98 00601025 */  or         $v0, $v1, $zero
    /* 20A9C 8001FE9C 03E00008 */  jr         $ra
    /* 20AA0 8001FEA0 00000000 */   nop
endlabel func_8001FE58
