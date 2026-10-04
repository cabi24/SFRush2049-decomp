nonmatching func_80018E6C, 0x48

glabel func_80018E6C
    /* 19A6C 80018E6C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 19A70 80018E70 AFB10018 */  sw         $s1, 0x18($sp)
    /* 19A74 80018E74 AFB00014 */  sw         $s0, 0x14($sp)
    /* 19A78 80018E78 3C108004 */  lui        $s0, %hi(D_80043EB8)
    /* 19A7C 80018E7C 3C118005 */  lui        $s1, %hi(D_8004BE78)
    /* 19A80 80018E80 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 19A84 80018E84 2631BE78 */  addiu      $s1, $s1, %lo(D_8004BE78)
    /* 19A88 80018E88 26103EB8 */  addiu      $s0, $s0, %lo(D_80043EB8)
  .L80018E8C:
    /* 19A8C 80018E8C 0C006350 */  jal        func_80018D40
    /* 19A90 80018E90 8E040000 */   lw        $a0, 0x0($s0)
    /* 19A94 80018E94 26100FF8 */  addiu      $s0, $s0, 0xFF8
    /* 19A98 80018E98 1611FFFC */  bne        $s0, $s1, .L80018E8C
    /* 19A9C 80018E9C 00000000 */   nop
    /* 19AA0 80018EA0 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 19AA4 80018EA4 8FB00014 */  lw         $s0, 0x14($sp)
    /* 19AA8 80018EA8 8FB10018 */  lw         $s1, 0x18($sp)
    /* 19AAC 80018EAC 03E00008 */  jr         $ra
    /* 19AB0 80018EB0 27BD0020 */   addiu     $sp, $sp, 0x20
endlabel func_80018E6C
