nonmatching func_8001D3F0, 0x70

glabel func_8001D3F0
    /* 1DFF0 8001D3F0 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 1DFF4 8001D3F4 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 1DFF8 8001D3F8 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 1DFFC 8001D3FC 44876000 */  mtc1       $a3, $fa0
    /* 1E000 8001D400 11C00012 */  beqz       $t6, .L8001D44C
    /* 1E004 8001D404 AFBF002C */   sw        $ra, 0x2C($sp)
    /* 1E008 8001D408 97A2004A */  lhu        $v0, 0x4A($sp)
    /* 1E00C 8001D40C C7A40040 */  lwc1       $ft0, 0x40($sp)
    /* 1E010 8001D410 8FAF0044 */  lw         $t7, 0x44($sp)
    /* 1E014 8001D414 93B9004F */  lbu        $t9, 0x4F($sp)
    /* 1E018 8001D418 93A80053 */  lbu        $t0, 0x53($sp)
    /* 1E01C 8001D41C 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* 1E020 8001D420 44076000 */  mfc1       $a3, $fa0
    /* 1E024 8001D424 0041C025 */  or         $t8, $v0, $at
    /* 1E028 8001D428 AFB8001C */  sw         $t8, 0x1C($sp)
    /* 1E02C 8001D42C AFA20018 */  sw         $v0, 0x18($sp)
    /* 1E030 8001D430 E7A40010 */  swc1       $ft0, 0x10($sp)
    /* 1E034 8001D434 AFAF0014 */  sw         $t7, 0x14($sp)
    /* 1E038 8001D438 AFB90020 */  sw         $t9, 0x20($sp)
    /* 1E03C 8001D43C 0C00747D */  jal        func_8001D1F4
    /* 1E040 8001D440 AFA80024 */   sw        $t0, 0x24($sp)
    /* 1E044 8001D444 10000003 */  b          .L8001D454
    /* 1E048 8001D448 8FBF002C */   lw        $ra, 0x2C($sp)
  .L8001D44C:
    /* 1E04C 8001D44C 2402FFFF */  addiu      $v0, $zero, -0x1
    /* 1E050 8001D450 8FBF002C */  lw         $ra, 0x2C($sp)
  .L8001D454:
    /* 1E054 8001D454 27BD0030 */  addiu      $sp, $sp, 0x30
    /* 1E058 8001D458 03E00008 */  jr         $ra
    /* 1E05C 8001D45C 00000000 */   nop
endlabel func_8001D3F0
