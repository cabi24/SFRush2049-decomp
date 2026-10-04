nonmatching func_8001C7F4, 0x6C

glabel func_8001C7F4
    /* 1D3F4 8001C7F4 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 1D3F8 8001C7F8 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 1D3FC 8001C7FC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1D400 8001C800 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1D404 8001C804 51C00013 */  beql       $t6, $zero, .L8001C854
    /* 1D408 8001C808 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 1D40C 8001C80C 0C005165 */  jal        func_80014594
    /* 1D410 8001C810 AFA40018 */   sw        $a0, 0x18($sp)
    /* 1D414 8001C814 8FA40018 */  lw         $a0, 0x18($sp)
    /* 1D418 8001C818 3C188005 */  lui        $t8, %hi(D_8004FA50)
    /* 1D41C 8001C81C 2718FA50 */  addiu      $t8, $t8, %lo(D_8004FA50)
    /* 1D420 8001C820 00047880 */  sll        $t7, $a0, 2
    /* 1D424 8001C824 01E47823 */  subu       $t7, $t7, $a0
    /* 1D428 8001C828 000F78C0 */  sll        $t7, $t7, 3
    /* 1D42C 8001C82C 01F81021 */  addu       $v0, $t7, $t8
    /* 1D430 8001C830 90590000 */  lbu        $t9, 0x0($v0)
    /* 1D434 8001C834 24010001 */  addiu      $at, $zero, 0x1
    /* 1D438 8001C838 17210003 */  bne        $t9, $at, .L8001C848
    /* 1D43C 8001C83C 00000000 */   nop
    /* 1D440 8001C840 0C007E55 */  jal        func_8001F954
    /* 1D444 8001C844 A0400000 */   sb        $zero, 0x0($v0)
  .L8001C848:
    /* 1D448 8001C848 0C005177 */  jal        func_800145DC
    /* 1D44C 8001C84C 00000000 */   nop
    /* 1D450 8001C850 8FBF0014 */  lw         $ra, 0x14($sp)
  .L8001C854:
    /* 1D454 8001C854 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1D458 8001C858 03E00008 */  jr         $ra
    /* 1D45C 8001C85C 00000000 */   nop
endlabel func_8001C7F4
