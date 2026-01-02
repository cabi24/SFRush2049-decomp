# Source: CC50.s
# Address: 0x8000C308

glabel func_8000C308
    /* CF08 8000C308 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* CF0C 8000C30C AFBF0014 */  sw         $ra, 0x14($sp)
    /* CF10 8000C310 0C00312C */  jal        func_8000C4B0
    /* CF14 8000C314 AFA40038 */   sw        $a0, 0x38($sp)
    /* CF18 8000C318 3C0E8003 */  lui        $t6, %hi(D_8002C3F0)
    /* CF1C 8000C31C 8DCEC3F0 */  lw         $t6, %lo(D_8002C3F0)($t6)
    /* CF20 8000C320 AFA20024 */  sw         $v0, 0x24($sp)
    /* CF24 8000C324 8FB80038 */  lw         $t8, 0x38($sp)
    /* CF28 8000C328 8DCF0000 */  lw         $t7, 0x0($t6)
    /* CF2C 8000C32C AFAF0034 */  sw         $t7, 0x34($sp)
    /* CF30 8000C330 8F090014 */  lw         $t1, 0x14($t8)
    /* CF34 8000C334 8F080010 */  lw         $t0, 0x10($t8)
    /* CF38 8000C338 AFA9002C */  sw         $t1, 0x2C($sp)
    /* CF3C 8000C33C 11EE0029 */  beq        $t7, $t6, .L8000C3E4
    /* CF40 8000C340 AFA80028 */   sw        $t0, 0x28($sp)
    /* CF44 8000C344 8DEA0010 */  lw         $t2, 0x10($t7)
    /* CF48 8000C348 8DEB0014 */  lw         $t3, 0x14($t7)
    /* CF4C 8000C34C 010A082B */  sltu       $at, $t0, $t2
    /* CF50 8000C350 14200024 */  bnez       $at, .L8000C3E4
    /* CF54 8000C354 0148082B */   sltu      $at, $t2, $t0
    /* CF58 8000C358 14200003 */  bnez       $at, .L8000C368
    /* CF5C 8000C35C 0169082B */   sltu      $at, $t3, $t1
    /* CF60 8000C360 10200020 */  beqz       $at, .L8000C3E4
    /* CF64 8000C364 00000000 */   nop
  .L8000C368:
    /* CF68 8000C368 27B90028 */  addiu      $t9, $sp, 0x28
    /* CF6C 8000C36C 8FB80034 */  lw         $t8, 0x34($sp)
    /* CF70 8000C370 AFB90020 */  sw         $t9, 0x20($sp)
    /* CF74 8000C374 8F2D0004 */  lw         $t5, 0x4($t9)
    /* CF78 8000C378 8F2C0000 */  lw         $t4, 0x0($t9)
    /* CF7C 8000C37C 8F0F0014 */  lw         $t7, 0x14($t8)
    /* CF80 8000C380 8F0E0010 */  lw         $t6, 0x10($t8)
    /* CF84 8000C384 3C188003 */  lui        $t8, %hi(D_8002C3F0)
    /* CF88 8000C388 01AF082B */  sltu       $at, $t5, $t7
    /* CF8C 8000C38C 018E4023 */  subu       $t0, $t4, $t6
    /* CF90 8000C390 01014023 */  subu       $t0, $t0, $at
    /* CF94 8000C394 01AF4823 */  subu       $t1, $t5, $t7
    /* CF98 8000C398 AF290004 */  sw         $t1, 0x4($t9)
    /* CF9C 8000C39C AF280000 */  sw         $t0, 0x0($t9)
    /* CFA0 8000C3A0 8FAA0034 */  lw         $t2, 0x34($sp)
    /* CFA4 8000C3A4 8F18C3F0 */  lw         $t8, %lo(D_8002C3F0)($t8)
    /* CFA8 8000C3A8 8D4B0000 */  lw         $t3, 0x0($t2)
    /* CFAC 8000C3AC 1178000D */  beq        $t3, $t8, .L8000C3E4
    /* CFB0 8000C3B0 AFAB0034 */   sw        $t3, 0x34($sp)
    /* CFB4 8000C3B4 8FAC0028 */  lw         $t4, 0x28($sp)
    /* CFB8 8000C3B8 8D6E0010 */  lw         $t6, 0x10($t3)
    /* CFBC 8000C3BC 8FAD002C */  lw         $t5, 0x2C($sp)
    /* CFC0 8000C3C0 8D6F0014 */  lw         $t7, 0x14($t3)
    /* CFC4 8000C3C4 01CC082B */  sltu       $at, $t6, $t4
    /* CFC8 8000C3C8 1420FFE7 */  bnez       $at, .L8000C368
    /* CFCC 8000C3CC 00000000 */   nop
    /* CFD0 8000C3D0 018E082B */  sltu       $at, $t4, $t6
    /* CFD4 8000C3D4 14200003 */  bnez       $at, .L8000C3E4
    /* CFD8 8000C3D8 01ED082B */   sltu      $at, $t7, $t5
    /* CFDC 8000C3DC 1420FFE2 */  bnez       $at, .L8000C368
    /* CFE0 8000C3E0 00000000 */   nop
  .L8000C3E4:
    /* CFE4 8000C3E4 8FB90038 */  lw         $t9, 0x38($sp)
    /* CFE8 8000C3E8 8FA80028 */  lw         $t0, 0x28($sp)
    /* CFEC 8000C3EC 8FA9002C */  lw         $t1, 0x2C($sp)
    /* CFF0 8000C3F0 3C188003 */  lui        $t8, %hi(D_8002C3F0)
    /* CFF4 8000C3F4 AF280010 */  sw         $t0, 0x10($t9)
    /* CFF8 8000C3F8 AF290014 */  sw         $t1, 0x14($t9)
    /* CFFC 8000C3FC 8F18C3F0 */  lw         $t8, %lo(D_8002C3F0)($t8)
    /* D000 8000C400 8FAA0034 */  lw         $t2, 0x34($sp)
    /* D004 8000C404 1158000D */  beq        $t2, $t8, .L8000C43C
    /* D008 8000C408 00000000 */   nop
    /* D00C 8000C40C 254B0010 */  addiu      $t3, $t2, 0x10
    /* D010 8000C410 AFAB001C */  sw         $t3, 0x1C($sp)
    /* D014 8000C414 8D4D0014 */  lw         $t5, 0x14($t2)
    /* D018 8000C418 8D4C0010 */  lw         $t4, 0x10($t2)
    /* D01C 8000C41C 8FAE0028 */  lw         $t6, 0x28($sp)
    /* D020 8000C420 8FAF002C */  lw         $t7, 0x2C($sp)
    /* D024 8000C424 018E4023 */  subu       $t0, $t4, $t6
    /* D028 8000C428 01AF082B */  sltu       $at, $t5, $t7
    /* D02C 8000C42C 01014023 */  subu       $t0, $t0, $at
    /* D030 8000C430 01AF4823 */  subu       $t1, $t5, $t7
    /* D034 8000C434 AD490014 */  sw         $t1, 0x14($t2)
    /* D038 8000C438 AD480010 */  sw         $t0, 0x10($t2)
  .L8000C43C:
    /* D03C 8000C43C 8FB90034 */  lw         $t9, 0x34($sp)
    /* D040 8000C440 8FB80038 */  lw         $t8, 0x38($sp)
    /* D044 8000C444 AF190000 */  sw         $t9, 0x0($t8)
    /* D048 8000C448 8FAB0034 */  lw         $t3, 0x34($sp)
    /* D04C 8000C44C 8FAD0038 */  lw         $t5, 0x38($sp)
    /* D050 8000C450 8D6C0004 */  lw         $t4, 0x4($t3)
    /* D054 8000C454 ADAC0004 */  sw         $t4, 0x4($t5)
    /* D058 8000C458 8FAF0034 */  lw         $t7, 0x34($sp)
    /* D05C 8000C45C 8FAE0038 */  lw         $t6, 0x38($sp)
    /* D060 8000C460 8DE80004 */  lw         $t0, 0x4($t7)
    /* D064 8000C464 AD0E0000 */  sw         $t6, 0x0($t0)
    /* D068 8000C468 8FAA0034 */  lw         $t2, 0x34($sp)
    /* D06C 8000C46C 8FA90038 */  lw         $t1, 0x38($sp)
    /* D070 8000C470 AD490004 */  sw         $t1, 0x4($t2)
    /* D074 8000C474 0C003148 */  jal        func_8000C520
    /* D078 8000C478 8FA40024 */   lw        $a0, 0x24($sp)
    /* D07C 8000C47C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* D080 8000C480 8FA20028 */  lw         $v0, 0x28($sp)
    /* D084 8000C484 8FA3002C */  lw         $v1, 0x2C($sp)
    /* D088 8000C488 03E00008 */  jr         $ra
    /* D08C 8000C48C 27BD0038 */   addiu     $sp, $sp, 0x38