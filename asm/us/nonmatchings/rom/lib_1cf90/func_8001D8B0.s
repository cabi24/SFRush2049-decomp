nonmatching func_8001D8B0, 0x78

glabel func_8001D8B0
    /* 1E4B0 8001D8B0 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 1E4B4 8001D8B4 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 1E4B8 8001D8B8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1E4BC 8001D8BC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1E4C0 8001D8C0 11C00015 */  beqz       $t6, .L8001D918
    /* 1E4C4 8001D8C4 00001025 */   or        $v0, $zero, $zero
    /* 1E4C8 8001D8C8 0C005165 */  jal        func_80014594
    /* 1E4CC 8001D8CC AFA40018 */   sw        $a0, 0x18($sp)
    /* 1E4D0 8001D8D0 8FA40018 */  lw         $a0, 0x18($sp)
    /* 1E4D4 8001D8D4 3C018005 */  lui        $at, %hi(D_8004FD54)
    /* 1E4D8 8001D8D8 8C820000 */  lw         $v0, 0x0($a0)
    /* 1E4DC 8001D8DC 50400005 */  beql       $v0, $zero, .L8001D8F4
    /* 1E4E0 8001D8E0 8C830004 */   lw        $v1, 0x4($a0)
    /* 1E4E4 8001D8E4 8C8F0004 */  lw         $t7, 0x4($a0)
    /* 1E4E8 8001D8E8 AC4F0004 */  sw         $t7, 0x4($v0)
    /* 1E4EC 8001D8EC 8C820000 */  lw         $v0, 0x0($a0)
    /* 1E4F0 8001D8F0 8C830004 */  lw         $v1, 0x4($a0)
  .L8001D8F4:
    /* 1E4F4 8001D8F4 10600003 */  beqz       $v1, .L8001D904
    /* 1E4F8 8001D8F8 00000000 */   nop
    /* 1E4FC 8001D8FC 10000002 */  b          .L8001D908
    /* 1E500 8001D900 AC620000 */   sw        $v0, 0x0($v1)
  .L8001D904:
    /* 1E504 8001D904 AC22FD54 */  sw         $v0, %lo(D_8004FD54)($at)
  .L8001D908:
    /* 1E508 8001D908 0C005177 */  jal        func_800145DC
    /* 1E50C 8001D90C 00000000 */   nop
    /* 1E510 8001D910 10000001 */  b          .L8001D918
    /* 1E514 8001D914 24020001 */   addiu     $v0, $zero, 0x1
  .L8001D918:
    /* 1E518 8001D918 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1E51C 8001D91C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1E520 8001D920 03E00008 */  jr         $ra
    /* 1E524 8001D924 00000000 */   nop
endlabel func_8001D8B0
