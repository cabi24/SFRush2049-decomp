nonmatching func_800110C4, 0x40

glabel func_800110C4
    /* 11CC4 800110C4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 11CC8 800110C8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 11CCC 800110CC 0C00441D */  jal        func_80011074
    /* 11CD0 800110D0 00000000 */   nop
    /* 11CD4 800110D4 3C048004 */  lui        $a0, %hi(D_800382F0)
    /* 11CD8 800110D8 8C8482F0 */  lw         $a0, %lo(D_800382F0)($a0)
    /* 11CDC 800110DC 3C198004 */  lui        $t9, %hi(D_8003801C)
    /* 11CE0 800110E0 50800005 */  beql       $a0, $zero, .L800110F8
    /* 11CE4 800110E4 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 11CE8 800110E8 8F39801C */  lw         $t9, %lo(D_8003801C)($t9)
    /* 11CEC 800110EC 0320F809 */  jalr       $t9
    /* 11CF0 800110F0 00000000 */   nop
    /* 11CF4 800110F4 8FBF0014 */  lw         $ra, 0x14($sp)
  .L800110F8:
    /* 11CF8 800110F8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 11CFC 800110FC 03E00008 */  jr         $ra
    /* 11D00 80011100 00000000 */   nop
endlabel func_800110C4
