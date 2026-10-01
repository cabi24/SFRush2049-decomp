nonmatching osViGetCurrentFramebuffer, 0x3C

glabel osViGetCurrentFramebuffer
    /* 8390 80007790 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 8394 80007794 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 8398 80007798 0C00312C */  jal        __osDisableInt
    /* 839C 8000779C 00000000 */   nop
    /* 83A0 800077A0 3C0E8003 */  lui        $t6, %hi(__osViModeInfo)
    /* 83A4 800077A4 8DCEC460 */  lw         $t6, %lo(__osViModeInfo)($t6)
    /* 83A8 800077A8 00402025 */  or         $a0, $v0, $zero
    /* 83AC 800077AC 8DCF0004 */  lw         $t7, 0x4($t6)
    /* 83B0 800077B0 0C003148 */  jal        __osRestoreInt
    /* 83B4 800077B4 AFAF0018 */   sw        $t7, 0x18($sp)
    /* 83B8 800077B8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 83BC 800077BC 8FA20018 */  lw         $v0, 0x18($sp)
    /* 83C0 800077C0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 83C4 800077C4 03E00008 */  jr         $ra
    /* 83C8 800077C8 00000000 */   nop
endlabel osViGetCurrentFramebuffer
    /* 83CC 800077CC 00000000 */  nop
