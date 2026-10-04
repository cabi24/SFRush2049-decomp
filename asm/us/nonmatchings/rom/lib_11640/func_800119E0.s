nonmatching func_800119E0, 0x30

glabel func_800119E0
    /* 125E0 800119E0 3C0E8004 */  lui        $t6, %hi(D_800382D4)
    /* 125E4 800119E4 8DCE82D4 */  lw         $t6, %lo(D_800382D4)($t6)
    /* 125E8 800119E8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 125EC 800119EC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 125F0 800119F0 3C048004 */  lui        $a0, %hi(D_800382D0)
    /* 125F4 800119F4 8C8482D0 */  lw         $a0, %lo(D_800382D0)($a0)
    /* 125F8 800119F8 0C004644 */  jal        func_80011910
    /* 125FC 800119FC 95C50000 */   lhu       $a1, 0x0($t6)
    /* 12600 80011A00 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 12604 80011A04 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 12608 80011A08 03E00008 */  jr         $ra
    /* 1260C 80011A0C 00000000 */   nop
endlabel func_800119E0
