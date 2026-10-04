nonmatching func_80025120, 0x30

glabel func_80025120
    /* 25D20 80025120 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 25D24 80025124 AFA40018 */  sw         $a0, 0x18($sp)
    /* 25D28 80025128 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 25D2C 8002512C 3C048006 */  lui        $a0, %hi(D_800586A8)
    /* 25D30 80025130 248486A8 */  addiu      $a0, $a0, %lo(D_800586A8)
    /* 25D34 80025134 00002825 */  or         $a1, $zero, $zero
    /* 25D38 80025138 0C001D78 */  jal        osJamMesg
    /* 25D3C 8002513C 00003025 */   or        $a2, $zero, $zero
    /* 25D40 80025140 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 25D44 80025144 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 25D48 80025148 03E00008 */  jr         $ra
    /* 25D4C 8002514C 00000000 */   nop
endlabel func_80025120
