nonmatching func_8002517C, 0x2C

glabel func_8002517C
    /* 25D7C 8002517C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 25D80 80025180 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 25D84 80025184 3C048006 */  lui        $a0, %hi(D_800586A8)
    /* 25D88 80025188 248486A8 */  addiu      $a0, $a0, %lo(D_800586A8)
    /* 25D8C 8002518C 00002825 */  or         $a1, $zero, $zero
    /* 25D90 80025190 0C001D78 */  jal        osJamMesg
    /* 25D94 80025194 00003025 */   or        $a2, $zero, $zero
    /* 25D98 80025198 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 25D9C 8002519C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 25DA0 800251A0 03E00008 */  jr         $ra
    /* 25DA4 800251A4 00000000 */   nop
endlabel func_8002517C
