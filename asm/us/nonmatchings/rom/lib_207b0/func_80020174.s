nonmatching func_80020174, 0x5C

glabel func_80020174
    /* 20D74 80020174 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 20D78 80020178 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 20D7C 8002017C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 20D80 80020180 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 20D84 80020184 AFA40020 */  sw         $a0, 0x20($sp)
    /* 20D88 80020188 AFA50024 */  sw         $a1, 0x24($sp)
    /* 20D8C 8002018C AFA60028 */  sw         $a2, 0x28($sp)
    /* 20D90 80020190 11C0000A */  beqz       $t6, .L800201BC
    /* 20D94 80020194 2403FFFF */   addiu     $v1, $zero, -0x1
    /* 20D98 80020198 0C005165 */  jal        func_80014594
    /* 20D9C 8002019C 00000000 */   nop
    /* 20DA0 800201A0 97A40022 */  lhu        $a0, 0x22($sp)
    /* 20DA4 800201A4 93A50027 */  lbu        $a1, 0x27($sp)
    /* 20DA8 800201A8 0C006C74 */  jal        func_8001B1D0
    /* 20DAC 800201AC 93A6002B */   lbu       $a2, 0x2B($sp)
    /* 20DB0 800201B0 0C005177 */  jal        func_800145DC
    /* 20DB4 800201B4 AFA2001C */   sw        $v0, 0x1C($sp)
    /* 20DB8 800201B8 8FA3001C */  lw         $v1, 0x1C($sp)
  .L800201BC:
    /* 20DBC 800201BC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 20DC0 800201C0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 20DC4 800201C4 00601025 */  or         $v0, $v1, $zero
    /* 20DC8 800201C8 03E00008 */  jr         $ra
    /* 20DCC 800201CC 00000000 */   nop
endlabel func_80020174
