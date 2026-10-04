nonmatching func_8001D518, 0x60

glabel func_8001D518
    /* 1E118 8001D518 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 1E11C 8001D51C 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 1E120 8001D520 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 1E124 8001D524 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1E128 8001D528 11C0000E */  beqz       $t6, .L8001D564
    /* 1E12C 8001D52C 2403FFFF */   addiu     $v1, $zero, -0x1
    /* 1E130 8001D530 AFA3001C */  sw         $v1, 0x1C($sp)
    /* 1E134 8001D534 0C005165 */  jal        func_80014594
    /* 1E138 8001D538 AFA40020 */   sw        $a0, 0x20($sp)
    /* 1E13C 8001D53C 8FA40020 */  lw         $a0, 0x20($sp)
    /* 1E140 8001D540 8FA3001C */  lw         $v1, 0x1C($sp)
    /* 1E144 8001D544 8C8F0008 */  lw         $t7, 0x8($a0)
    /* 1E148 8001D548 000FC3C0 */  sll        $t8, $t7, 15
    /* 1E14C 8001D54C 07010002 */  bgez       $t8, .L8001D558
    /* 1E150 8001D550 00000000 */   nop
    /* 1E154 8001D554 8C830034 */  lw         $v1, 0x34($a0)
  .L8001D558:
    /* 1E158 8001D558 0C005177 */  jal        func_800145DC
    /* 1E15C 8001D55C AFA3001C */   sw        $v1, 0x1C($sp)
    /* 1E160 8001D560 8FA3001C */  lw         $v1, 0x1C($sp)
  .L8001D564:
    /* 1E164 8001D564 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1E168 8001D568 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 1E16C 8001D56C 00601025 */  or         $v0, $v1, $zero
    /* 1E170 8001D570 03E00008 */  jr         $ra
    /* 1E174 8001D574 00000000 */   nop
endlabel func_8001D518
