nonmatching func_8001FFF4, 0x54

glabel func_8001FFF4
    /* 20BF4 8001FFF4 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 20BF8 8001FFF8 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 20BFC 8001FFFC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 20C00 80020000 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 20C04 80020004 AFA40020 */  sw         $a0, 0x20($sp)
    /* 20C08 80020008 AFA50024 */  sw         $a1, 0x24($sp)
    /* 20C0C 8002000C 11C00009 */  beqz       $t6, .L80020034
    /* 20C10 80020010 2403FFFF */   addiu     $v1, $zero, -0x1
    /* 20C14 80020014 0C005165 */  jal        func_80014594
    /* 20C18 80020018 00000000 */   nop
    /* 20C1C 8002001C 8FA40020 */  lw         $a0, 0x20($sp)
    /* 20C20 80020020 0C006CA7 */  jal        func_8001B29C
    /* 20C24 80020024 93A50027 */   lbu       $a1, 0x27($sp)
    /* 20C28 80020028 0C005177 */  jal        func_800145DC
    /* 20C2C 8002002C AFA2001C */   sw        $v0, 0x1C($sp)
    /* 20C30 80020030 8FA3001C */  lw         $v1, 0x1C($sp)
  .L80020034:
    /* 20C34 80020034 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 20C38 80020038 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 20C3C 8002003C 00601025 */  or         $v0, $v1, $zero
    /* 20C40 80020040 03E00008 */  jr         $ra
    /* 20C44 80020044 00000000 */   nop
endlabel func_8001FFF4
