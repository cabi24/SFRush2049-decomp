nonmatching func_80010D3C, 0x38

glabel func_80010D3C
    /* 1193C 80010D3C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 11940 80010D40 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 11944 80010D44 AFA40018 */  sw         $a0, 0x18($sp)
    /* 11948 80010D48 0C002FC0 */  jal        osAiSetFrequency
    /* 1194C 80010D4C 8C840000 */   lw        $a0, 0x0($a0)
    /* 11950 80010D50 8FB80018 */  lw         $t8, 0x18($sp)
    /* 11954 80010D54 3C038004 */  lui        $v1, %hi(D_8003828C)
    /* 11958 80010D58 2463828C */  addiu      $v1, $v1, %lo(D_8003828C)
    /* 1195C 80010D5C AC620000 */  sw         $v0, 0x0($v1)
    /* 11960 80010D60 AF020000 */  sw         $v0, 0x0($t8)
    /* 11964 80010D64 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 11968 80010D68 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1196C 80010D6C 03E00008 */  jr         $ra
    /* 11970 80010D70 00000000 */   nop
endlabel func_80010D3C
