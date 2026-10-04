nonmatching func_800177EC, 0x38

glabel func_800177EC
    /* 183EC 800177EC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 183F0 800177F0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 183F4 800177F4 AFA40018 */  sw         $a0, 0x18($sp)
    /* 183F8 800177F8 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 183FC 800177FC 308700FF */  andi       $a3, $a0, 0xFF
    /* 18400 80017800 3C068005 */  lui        $a2, %hi(D_8004BE7B)
    /* 18404 80017804 30A500FF */  andi       $a1, $a1, 0xFF
    /* 18408 80017808 90C6BE7B */  lbu        $a2, %lo(D_8004BE7B)($a2)
    /* 1840C 8001780C 0C008184 */  jal        func_80020610
    /* 18410 80017810 24040082 */   addiu     $a0, $zero, 0x82
    /* 18414 80017814 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 18418 80017818 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1841C 8001781C 03E00008 */  jr         $ra
    /* 18420 80017820 00000000 */   nop
endlabel func_800177EC
