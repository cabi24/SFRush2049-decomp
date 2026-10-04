nonmatching func_8001FF4C, 0x54

glabel func_8001FF4C
    /* 20B4C 8001FF4C 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 20B50 8001FF50 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 20B54 8001FF54 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 20B58 8001FF58 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 20B5C 8001FF5C AFA40020 */  sw         $a0, 0x20($sp)
    /* 20B60 8001FF60 AFA50024 */  sw         $a1, 0x24($sp)
    /* 20B64 8001FF64 11C00009 */  beqz       $t6, .L8001FF8C
    /* 20B68 8001FF68 2403FFFF */   addiu     $v1, $zero, -0x1
    /* 20B6C 8001FF6C 0C005165 */  jal        func_80014594
    /* 20B70 8001FF70 00000000 */   nop
    /* 20B74 8001FF74 8FA40020 */  lw         $a0, 0x20($sp)
    /* 20B78 8001FF78 0C006CE8 */  jal        func_8001B3A0
    /* 20B7C 8001FF7C 93A50027 */   lbu       $a1, 0x27($sp)
    /* 20B80 8001FF80 0C005177 */  jal        func_800145DC
    /* 20B84 8001FF84 AFA2001C */   sw        $v0, 0x1C($sp)
    /* 20B88 8001FF88 8FA3001C */  lw         $v1, 0x1C($sp)
  .L8001FF8C:
    /* 20B8C 8001FF8C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 20B90 8001FF90 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 20B94 8001FF94 00601025 */  or         $v0, $v1, $zero
    /* 20B98 8001FF98 03E00008 */  jr         $ra
    /* 20B9C 8001FF9C 00000000 */   nop
endlabel func_8001FF4C
