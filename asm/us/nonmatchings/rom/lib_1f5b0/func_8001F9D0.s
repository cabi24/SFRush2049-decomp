nonmatching func_8001F9D0, 0x48

glabel func_8001F9D0
    /* 205D0 8001F9D0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 205D4 8001F9D4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 205D8 8001F9D8 0C007AC4 */  jal        func_8001EB10
    /* 205DC 8001F9DC AFA40018 */   sw        $a0, 0x18($sp)
    /* 205E0 8001F9E0 8FA40018 */  lw         $a0, 0x18($sp)
    /* 205E4 8001F9E4 2401FFFC */  addiu      $at, $zero, -0x4
    /* 205E8 8001F9E8 888E0024 */  lwl        $t6, 0x24($a0)
    /* 205EC 8001F9EC 988E0027 */  lwr        $t6, 0x27($a0)
    /* 205F0 8001F9F0 A8800028 */  swl        $zero, 0x28($a0)
    /* 205F4 8001F9F4 B880002B */  swr        $zero, 0x2B($a0)
    /* 205F8 8001F9F8 01C17824 */  and        $t7, $t6, $at
    /* 205FC 8001F9FC A88F0024 */  swl        $t7, 0x24($a0)
    /* 20600 8001FA00 0C007DBB */  jal        func_8001F6EC
    /* 20604 8001FA04 B88F0027 */   swr       $t7, 0x27($a0)
    /* 20608 8001FA08 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 2060C 8001FA0C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 20610 8001FA10 03E00008 */  jr         $ra
    /* 20614 8001FA14 00000000 */   nop
endlabel func_8001F9D0
