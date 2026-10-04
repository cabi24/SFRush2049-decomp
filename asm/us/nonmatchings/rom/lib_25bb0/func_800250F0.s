nonmatching func_800250F0, 0x30

glabel func_800250F0
    /* 25CF0 800250F0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 25CF4 800250F4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 25CF8 800250F8 3C048006 */  lui        $a0, %hi(D_800586A8)
    /* 25CFC 800250FC 248486A8 */  addiu      $a0, $a0, %lo(D_800586A8)
    /* 25D00 80025100 00002825 */  or         $a1, $zero, $zero
    /* 25D04 80025104 0C001C9C */  jal        osRecvMesg
    /* 25D08 80025108 24060001 */   addiu     $a2, $zero, 0x1
    /* 25D0C 8002510C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 25D10 80025110 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 25D14 80025114 00001025 */  or         $v0, $zero, $zero
    /* 25D18 80025118 03E00008 */  jr         $ra
    /* 25D1C 8002511C 00000000 */   nop
endlabel func_800250F0
