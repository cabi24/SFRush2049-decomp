nonmatching func_8001D460, 0x6C

glabel func_8001D460
    /* 1E060 8001D460 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 1E064 8001D464 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 1E068 8001D468 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 1E06C 8001D46C 44876000 */  mtc1       $a3, $fa0
    /* 1E070 8001D470 11C00011 */  beqz       $t6, .L8001D4B8
    /* 1E074 8001D474 AFBF002C */   sw        $ra, 0x2C($sp)
    /* 1E078 8001D478 C7A40040 */  lwc1       $ft0, 0x40($sp)
    /* 1E07C 8001D47C 8FAF0044 */  lw         $t7, 0x44($sp)
    /* 1E080 8001D480 97B8004A */  lhu        $t8, 0x4A($sp)
    /* 1E084 8001D484 97B9004E */  lhu        $t9, 0x4E($sp)
    /* 1E088 8001D488 93A80053 */  lbu        $t0, 0x53($sp)
    /* 1E08C 8001D48C 93A90057 */  lbu        $t1, 0x57($sp)
    /* 1E090 8001D490 44076000 */  mfc1       $a3, $fa0
    /* 1E094 8001D494 E7A40010 */  swc1       $ft0, 0x10($sp)
    /* 1E098 8001D498 AFAF0014 */  sw         $t7, 0x14($sp)
    /* 1E09C 8001D49C AFB80018 */  sw         $t8, 0x18($sp)
    /* 1E0A0 8001D4A0 AFB9001C */  sw         $t9, 0x1C($sp)
    /* 1E0A4 8001D4A4 AFA80020 */  sw         $t0, 0x20($sp)
    /* 1E0A8 8001D4A8 0C00747D */  jal        func_8001D1F4
    /* 1E0AC 8001D4AC AFA90024 */   sw        $t1, 0x24($sp)
    /* 1E0B0 8001D4B0 10000003 */  b          .L8001D4C0
    /* 1E0B4 8001D4B4 8FBF002C */   lw        $ra, 0x2C($sp)
  .L8001D4B8:
    /* 1E0B8 8001D4B8 2402FFFF */  addiu      $v0, $zero, -0x1
    /* 1E0BC 8001D4BC 8FBF002C */  lw         $ra, 0x2C($sp)
  .L8001D4C0:
    /* 1E0C0 8001D4C0 27BD0030 */  addiu      $sp, $sp, 0x30
    /* 1E0C4 8001D4C4 03E00008 */  jr         $ra
    /* 1E0C8 8001D4C8 00000000 */   nop
endlabel func_8001D460
