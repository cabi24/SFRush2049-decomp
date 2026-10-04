nonmatching func_80014624, 0x2C

glabel func_80014624
    /* 15224 80014624 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 15228 80014628 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1522C 8001462C 3C048004 */  lui        $a0, %hi(D_80038368)
    /* 15230 80014630 24848368 */  addiu      $a0, $a0, %lo(D_80038368)
    /* 15234 80014634 00002825 */  or         $a1, $zero, $zero
    /* 15238 80014638 0C001C9C */  jal        osRecvMesg
    /* 1523C 8001463C 24060001 */   addiu     $a2, $zero, 0x1
    /* 15240 80014640 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 15244 80014644 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 15248 80014648 03E00008 */  jr         $ra
    /* 1524C 8001464C 00000000 */   nop
endlabel func_80014624
