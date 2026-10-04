nonmatching func_8001FEF8, 0x54

glabel func_8001FEF8
    /* 20AF8 8001FEF8 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 20AFC 8001FEFC 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 20B00 8001FF00 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 20B04 8001FF04 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 20B08 8001FF08 AFA40020 */  sw         $a0, 0x20($sp)
    /* 20B0C 8001FF0C AFA50024 */  sw         $a1, 0x24($sp)
    /* 20B10 8001FF10 11C00009 */  beqz       $t6, .L8001FF38
    /* 20B14 8001FF14 2403FFFF */   addiu     $v1, $zero, -0x1
    /* 20B18 8001FF18 0C005165 */  jal        func_80014594
    /* 20B1C 8001FF1C 00000000 */   nop
    /* 20B20 8001FF20 8FA40020 */  lw         $a0, 0x20($sp)
    /* 20B24 8001FF24 0C006D7D */  jal        func_8001B5F4
    /* 20B28 8001FF28 97A50026 */   lhu       $a1, 0x26($sp)
    /* 20B2C 8001FF2C 0C005177 */  jal        func_800145DC
    /* 20B30 8001FF30 AFA2001C */   sw        $v0, 0x1C($sp)
    /* 20B34 8001FF34 8FA3001C */  lw         $v1, 0x1C($sp)
  .L8001FF38:
    /* 20B38 8001FF38 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 20B3C 8001FF3C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 20B40 8001FF40 00601025 */  or         $v0, $v1, $zero
    /* 20B44 8001FF44 03E00008 */  jr         $ra
    /* 20B48 8001FF48 00000000 */   nop
endlabel func_8001FEF8
