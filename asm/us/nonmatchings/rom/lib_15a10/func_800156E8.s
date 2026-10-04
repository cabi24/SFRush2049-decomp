nonmatching func_800156E8, 0x30

glabel func_800156E8
    /* 162E8 800156E8 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 162EC 800156EC AFBF001C */  sw         $ra, 0x1C($sp)
    /* 162F0 800156F0 AFA40020 */  sw         $a0, 0x20($sp)
    /* 162F4 800156F4 AFA50024 */  sw         $a1, 0x24($sp)
    /* 162F8 800156F8 30A5FFFF */  andi       $a1, $a1, 0xFFFF
    /* 162FC 800156FC 3084FFFF */  andi       $a0, $a0, 0xFFFF
    /* 16300 80015700 0C005563 */  jal        func_8001558C
    /* 16304 80015704 AFA00010 */   sw        $zero, 0x10($sp)
    /* 16308 80015708 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 1630C 8001570C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 16310 80015710 03E00008 */  jr         $ra
    /* 16314 80015714 00000000 */   nop
endlabel func_800156E8
    /* 16318 80015718 00000000 */  nop
    /* 1631C 8001571C 00000000 */  nop
