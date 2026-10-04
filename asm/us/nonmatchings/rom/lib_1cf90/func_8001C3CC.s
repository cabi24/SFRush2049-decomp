nonmatching func_8001C3CC, 0x13C

glabel func_8001C3CC
    /* 1CFCC 8001C3CC 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 1CFD0 8001C3D0 3C028005 */  lui        $v0, %hi(D_8004FA18)
    /* 1CFD4 8001C3D4 9042FA18 */  lbu        $v0, %lo(D_8004FA18)($v0)
    /* 1CFD8 8001C3D8 AFB20024 */  sw         $s2, 0x24($sp)
    /* 1CFDC 8001C3DC AFBF002C */  sw         $ra, 0x2C($sp)
    /* 1CFE0 8001C3E0 AFB30028 */  sw         $s3, 0x28($sp)
    /* 1CFE4 8001C3E4 AFB10020 */  sw         $s1, 0x20($sp)
    /* 1CFE8 8001C3E8 AFB0001C */  sw         $s0, 0x1C($sp)
    /* 1CFEC 8001C3EC 1840003F */  blez       $v0, .L8001C4EC
    /* 1CFF0 8001C3F0 00009025 */   or        $s2, $zero, $zero
    /* 1CFF4 8001C3F4 3C108005 */  lui        $s0, %hi(D_8004FA50)
    /* 1CFF8 8001C3F8 2610FA50 */  addiu      $s0, $s0, %lo(D_8004FA50)
    /* 1CFFC 8001C3FC 24130001 */  addiu      $s3, $zero, 0x1
  .L8001C400:
    /* 1D000 8001C400 920E0000 */  lbu        $t6, 0x0($s0)
    /* 1D004 8001C404 566E0036 */  bnel       $s3, $t6, .L8001C4E0
    /* 1D008 8001C408 26520001 */   addiu     $s2, $s2, 0x1
    /* 1D00C 8001C40C 0C005306 */  jal        func_80014C18
    /* 1D010 8001C410 02402025 */   or        $a0, $s2, $zero
    /* 1D014 8001C414 8E030010 */  lw         $v1, 0x10($s0)
    /* 1D018 8001C418 00408825 */  or         $s1, $v0, $zero
    /* 1D01C 8001C41C 1043002C */  beq        $v0, $v1, .L8001C4D0
    /* 1D020 8001C420 0062082B */   sltu      $at, $v1, $v0
    /* 1D024 8001C424 10200015 */  beqz       $at, .L8001C47C
    /* 1D028 8001C428 00035040 */   sll       $t2, $v1, 1
    /* 1D02C 8001C42C 8E190014 */  lw         $t9, 0x14($s0)
    /* 1D030 8001C430 8E180008 */  lw         $t8, 0x8($s0)
    /* 1D034 8001C434 00037840 */  sll        $t7, $v1, 1
    /* 1D038 8001C438 AFB90010 */  sw         $t9, 0x10($sp)
    /* 1D03C 8001C43C 8E190004 */  lw         $t9, 0x4($s0)
    /* 1D040 8001C440 00432823 */  subu       $a1, $v0, $v1
    /* 1D044 8001C444 00003025 */  or         $a2, $zero, $zero
    /* 1D048 8001C448 00003825 */  or         $a3, $zero, $zero
    /* 1D04C 8001C44C 0320F809 */  jalr       $t9
    /* 1D050 8001C450 01F82021 */   addu      $a0, $t7, $t8
    /* 1D054 8001C454 5040001F */  beql       $v0, $zero, .L8001C4D4
    /* 1D058 8001C458 AE110010 */   sw        $s1, 0x10($s0)
    /* 1D05C 8001C45C 8E030010 */  lw         $v1, 0x10($s0)
    /* 1D060 8001C460 8E090008 */  lw         $t1, 0x8($s0)
    /* 1D064 8001C464 00034040 */  sll        $t0, $v1, 1
    /* 1D068 8001C468 02232823 */  subu       $a1, $s1, $v1
    /* 1D06C 8001C46C 0C005310 */  jal        func_80014C40
    /* 1D070 8001C470 01092021 */   addu      $a0, $t0, $t1
    /* 1D074 8001C474 10000017 */  b          .L8001C4D4
    /* 1D078 8001C478 AE110010 */   sw        $s1, 0x10($s0)
  .L8001C47C:
    /* 1D07C 8001C47C 8E190004 */  lw         $t9, 0x4($s0)
    /* 1D080 8001C480 8E060008 */  lw         $a2, 0x8($s0)
    /* 1D084 8001C484 8E0B000C */  lw         $t3, 0xC($s0)
    /* 1D088 8001C488 8E0C0014 */  lw         $t4, 0x14($s0)
    /* 1D08C 8001C48C 02203825 */  or         $a3, $s1, $zero
    /* 1D090 8001C490 01462021 */  addu       $a0, $t2, $a2
    /* 1D094 8001C494 01632823 */  subu       $a1, $t3, $v1
    /* 1D098 8001C498 0320F809 */  jalr       $t9
    /* 1D09C 8001C49C AFAC0010 */   sw        $t4, 0x10($sp)
    /* 1D0A0 8001C4A0 5040000C */  beql       $v0, $zero, .L8001C4D4
    /* 1D0A4 8001C4A4 AE110010 */   sw        $s1, 0x10($s0)
    /* 1D0A8 8001C4A8 8E030010 */  lw         $v1, 0x10($s0)
    /* 1D0AC 8001C4AC 8E0E0008 */  lw         $t6, 0x8($s0)
    /* 1D0B0 8001C4B0 8E0F000C */  lw         $t7, 0xC($s0)
    /* 1D0B4 8001C4B4 00036840 */  sll        $t5, $v1, 1
    /* 1D0B8 8001C4B8 01AE2021 */  addu       $a0, $t5, $t6
    /* 1D0BC 8001C4BC 0C005310 */  jal        func_80014C40
    /* 1D0C0 8001C4C0 01E32823 */   subu      $a1, $t7, $v1
    /* 1D0C4 8001C4C4 8E040008 */  lw         $a0, 0x8($s0)
    /* 1D0C8 8001C4C8 0C005310 */  jal        func_80014C40
    /* 1D0CC 8001C4CC 02202825 */   or        $a1, $s1, $zero
  .L8001C4D0:
    /* 1D0D0 8001C4D0 AE110010 */  sw         $s1, 0x10($s0)
  .L8001C4D4:
    /* 1D0D4 8001C4D4 3C028005 */  lui        $v0, %hi(D_8004FA18)
    /* 1D0D8 8001C4D8 9042FA18 */  lbu        $v0, %lo(D_8004FA18)($v0)
    /* 1D0DC 8001C4DC 26520001 */  addiu      $s2, $s2, 0x1
  .L8001C4E0:
    /* 1D0E0 8001C4E0 0242082A */  slt        $at, $s2, $v0
    /* 1D0E4 8001C4E4 1420FFC6 */  bnez       $at, .L8001C400
    /* 1D0E8 8001C4E8 26100018 */   addiu     $s0, $s0, 0x18
  .L8001C4EC:
    /* 1D0EC 8001C4EC 8FBF002C */  lw         $ra, 0x2C($sp)
    /* 1D0F0 8001C4F0 8FB0001C */  lw         $s0, 0x1C($sp)
    /* 1D0F4 8001C4F4 8FB10020 */  lw         $s1, 0x20($sp)
    /* 1D0F8 8001C4F8 8FB20024 */  lw         $s2, 0x24($sp)
    /* 1D0FC 8001C4FC 8FB30028 */  lw         $s3, 0x28($sp)
    /* 1D100 8001C500 03E00008 */  jr         $ra
    /* 1D104 8001C504 27BD0030 */   addiu     $sp, $sp, 0x30
endlabel func_8001C3CC
