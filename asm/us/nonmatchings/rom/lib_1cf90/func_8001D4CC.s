nonmatching func_8001D4CC, 0x4C

glabel func_8001D4CC
    /* 1E0CC 8001D4CC 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 1E0D0 8001D4D0 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 1E0D4 8001D4D4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1E0D8 8001D4D8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1E0DC 8001D4DC 11C00009 */  beqz       $t6, .L8001D504
    /* 1E0E0 8001D4E0 AFA40018 */   sw        $a0, 0x18($sp)
    /* 1E0E4 8001D4E4 0C005165 */  jal        func_80014594
    /* 1E0E8 8001D4E8 00000000 */   nop
    /* 1E0EC 8001D4EC 0C007421 */  jal        func_8001D084
    /* 1E0F0 8001D4F0 8FA40018 */   lw        $a0, 0x18($sp)
    /* 1E0F4 8001D4F4 0C005177 */  jal        func_800145DC
    /* 1E0F8 8001D4F8 00000000 */   nop
    /* 1E0FC 8001D4FC 10000002 */  b          .L8001D508
    /* 1E100 8001D500 24020001 */   addiu     $v0, $zero, 0x1
  .L8001D504:
    /* 1E104 8001D504 00001025 */  or         $v0, $zero, $zero
  .L8001D508:
    /* 1E108 8001D508 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1E10C 8001D50C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1E110 8001D510 03E00008 */  jr         $ra
    /* 1E114 8001D514 00000000 */   nop
endlabel func_8001D4CC
