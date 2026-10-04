nonmatching func_80018E2C, 0x40

glabel func_80018E2C
    /* 19A2C 80018E2C 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 19A30 80018E30 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 19A34 80018E34 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 19A38 80018E38 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 19A3C 80018E3C 11C00007 */  beqz       $t6, .L80018E5C
    /* 19A40 80018E40 AFA40018 */   sw        $a0, 0x18($sp)
    /* 19A44 80018E44 0C005165 */  jal        func_80014594
    /* 19A48 80018E48 00000000 */   nop
    /* 19A4C 80018E4C 0C006350 */  jal        func_80018D40
    /* 19A50 80018E50 8FA40018 */   lw        $a0, 0x18($sp)
    /* 19A54 80018E54 0C005177 */  jal        func_800145DC
    /* 19A58 80018E58 00000000 */   nop
  .L80018E5C:
    /* 19A5C 80018E5C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 19A60 80018E60 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 19A64 80018E64 03E00008 */  jr         $ra
    /* 19A68 80018E68 00000000 */   nop
endlabel func_80018E2C
