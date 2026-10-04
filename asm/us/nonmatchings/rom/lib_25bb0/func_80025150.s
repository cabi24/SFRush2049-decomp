nonmatching func_80025150, 0x2C

glabel func_80025150
    /* 25D50 80025150 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 25D54 80025154 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 25D58 80025158 3C048006 */  lui        $a0, %hi(D_800586A8)
    /* 25D5C 8002515C 248486A8 */  addiu      $a0, $a0, %lo(D_800586A8)
    /* 25D60 80025160 00002825 */  or         $a1, $zero, $zero
    /* 25D64 80025164 0C001C9C */  jal        osRecvMesg
    /* 25D68 80025168 24060001 */   addiu     $a2, $zero, 0x1
    /* 25D6C 8002516C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 25D70 80025170 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 25D74 80025174 03E00008 */  jr         $ra
    /* 25D78 80025178 00000000 */   nop
endlabel func_80025150
