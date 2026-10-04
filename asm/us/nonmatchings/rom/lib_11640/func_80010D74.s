nonmatching func_80010D74, 0x64

glabel func_80010D74
    /* 11974 80010D74 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 11978 80010D78 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1197C 80010D7C 240E00FF */  addiu      $t6, $zero, 0xFF
    /* 11980 80010D80 3C048004 */  lui        $a0, %hi(D_800381F8)
    /* 11984 80010D84 A3AE001C */  sb         $t6, 0x1C($sp)
    /* 11988 80010D88 248481F8 */  addiu      $a0, $a0, %lo(D_800381F8)
    /* 1198C 80010D8C 27A5001C */  addiu      $a1, $sp, 0x1C
    /* 11990 80010D90 0C001D78 */  jal        osJamMesg
    /* 11994 80010D94 24060001 */   addiu     $a2, $zero, 0x1
    /* 11998 80010D98 3C198004 */  lui        $t9, %hi(D_8003801C)
    /* 1199C 80010D9C 8F39801C */  lw         $t9, %lo(D_8003801C)($t9)
    /* 119A0 80010DA0 3C048004 */  lui        $a0, %hi(D_80038228)
    /* 119A4 80010DA4 8C848228 */  lw         $a0, %lo(D_80038228)($a0)
    /* 119A8 80010DA8 0320F809 */  jalr       $t9
    /* 119AC 80010DAC 00000000 */   nop
    /* 119B0 80010DB0 3C198004 */  lui        $t9, %hi(D_8003801C)
    /* 119B4 80010DB4 8F39801C */  lw         $t9, %lo(D_8003801C)($t9)
    /* 119B8 80010DB8 3C048004 */  lui        $a0, %hi(D_800381F0)
    /* 119BC 80010DBC 8C8481F0 */  lw         $a0, %lo(D_800381F0)($a0)
    /* 119C0 80010DC0 0320F809 */  jalr       $t9
    /* 119C4 80010DC4 00000000 */   nop
    /* 119C8 80010DC8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 119CC 80010DCC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 119D0 80010DD0 03E00008 */  jr         $ra
    /* 119D4 80010DD4 00000000 */   nop
endlabel func_80010D74
