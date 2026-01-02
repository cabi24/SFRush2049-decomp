# Source: EFD0.s
# Address: 0x8000E3D0

glabel func_8000E3D0
    /* EFD0 8000E3D0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* EFD4 8000E3D4 AFA40020 */  sw         $a0, 0x20($sp)
    /* EFD8 8000E3D8 8FAE0020 */  lw         $t6, 0x20($sp)
    /* EFDC 8000E3DC AFBF0014 */  sw         $ra, 0x14($sp)
    /* EFE0 8000E3E0 AFA60028 */  sw         $a2, 0x28($sp)
    /* EFE4 8000E3E4 AFA7002C */  sw         $a3, 0x2C($sp)
    /* EFE8 8000E3E8 ADC00000 */  sw         $zero, 0x0($t6)
    /* EFEC 8000E3EC 8FAF0020 */  lw         $t7, 0x20($sp)
    /* EFF0 8000E3F0 ADE00004 */  sw         $zero, 0x4($t7)
    /* EFF4 8000E3F4 8FA80020 */  lw         $t0, 0x20($sp)
    /* EFF8 8000E3F8 8FB90034 */  lw         $t9, 0x34($sp)
    /* EFFC 8000E3FC 8FB80030 */  lw         $t8, 0x30($sp)
    /* F000 8000E400 AD19000C */  sw         $t9, 0xC($t0)
    /* F004 8000E404 AD180008 */  sw         $t8, 0x8($t0)
    /* F008 8000E408 8FAA0028 */  lw         $t2, 0x28($sp)
    /* F00C 8000E40C 8FAB002C */  lw         $t3, 0x2C($sp)
    /* F010 8000E410 15400003 */  bnez       $t2, .L8000E420
    /* F014 8000E414 00000000 */   nop
    /* F018 8000E418 11600005 */  beqz       $t3, .L8000E430
    /* F01C 8000E41C 00000000 */   nop
  .L8000E420:
    /* F020 8000E420 8FA90020 */  lw         $t1, 0x20($sp)
    /* F024 8000E424 AD2A0010 */  sw         $t2, 0x10($t1)
    /* F028 8000E428 10000006 */  b          .L8000E444
    /* F02C 8000E42C AD2B0014 */   sw        $t3, 0x14($t1)
  .L8000E430:
    /* F030 8000E430 8FAE0020 */  lw         $t6, 0x20($sp)
    /* F034 8000E434 8FAC0030 */  lw         $t4, 0x30($sp)
    /* F038 8000E438 8FAD0034 */  lw         $t5, 0x34($sp)
    /* F03C 8000E43C ADCC0010 */  sw         $t4, 0x10($t6)
    /* F040 8000E440 ADCD0014 */  sw         $t5, 0x14($t6)
  .L8000E444:
    /* F044 8000E444 8FAF0038 */  lw         $t7, 0x38($sp)
    /* F048 8000E448 8FB80020 */  lw         $t8, 0x20($sp)
    /* F04C 8000E44C AF0F0018 */  sw         $t7, 0x18($t8)
    /* F050 8000E450 8FA80020 */  lw         $t0, 0x20($sp)
    /* F054 8000E454 8FB9003C */  lw         $t9, 0x3C($sp)
    /* F058 8000E458 AD19001C */  sw         $t9, 0x1C($t0)
    /* F05C 8000E45C 0C0030C2 */  jal        func_8000C308
    /* F060 8000E460 8FA40020 */   lw        $a0, 0x20($sp)
    /* F064 8000E464 3C0A8003 */  lui        $t2, %hi(D_8002C3F0)
    /* F068 8000E468 8D4AC3F0 */  lw         $t2, %lo(D_8002C3F0)($t2)
    /* F06C 8000E46C AFA20018 */  sw         $v0, 0x18($sp)
    /* F070 8000E470 AFA3001C */  sw         $v1, 0x1C($sp)
    /* F074 8000E474 8FA90020 */  lw         $t1, 0x20($sp)
    /* F078 8000E478 8D4B0000 */  lw         $t3, 0x0($t2)
    /* F07C 8000E47C 15690004 */  bne        $t3, $t1, .L8000E490
    /* F080 8000E480 00000000 */   nop
    /* F084 8000E484 8FA40018 */  lw         $a0, 0x18($sp)
    /* F088 8000E488 0C0030A5 */  jal        func_8000C294
    /* F08C 8000E48C 8FA5001C */   lw        $a1, 0x1C($sp)
  .L8000E490:
    /* F090 8000E490 8FBF0014 */  lw         $ra, 0x14($sp)
    /* F094 8000E494 27BD0020 */  addiu      $sp, $sp, 0x20
    /* F098 8000E498 00001025 */  or         $v0, $zero, $zero
    /* F09C 8000E49C 03E00008 */  jr         $ra
    /* F0A0 8000E4A0 00000000 */   nop