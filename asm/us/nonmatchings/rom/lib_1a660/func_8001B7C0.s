nonmatching func_8001B7C0, 0x104

glabel func_8001B7C0
    /* 1C3C0 8001B7C0 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 1C3C4 8001B7C4 AFBF002C */  sw         $ra, 0x2C($sp)
    /* 1C3C8 8001B7C8 AFB40024 */  sw         $s4, 0x24($sp)
    /* 1C3CC 8001B7CC AFB30020 */  sw         $s3, 0x20($sp)
    /* 1C3D0 8001B7D0 30B300FF */  andi       $s3, $a1, 0xFF
    /* 1C3D4 8001B7D4 AFB50028 */  sw         $s5, 0x28($sp)
    /* 1C3D8 8001B7D8 AFB2001C */  sw         $s2, 0x1C($sp)
    /* 1C3DC 8001B7DC AFB10018 */  sw         $s1, 0x18($sp)
    /* 1C3E0 8001B7E0 AFB00014 */  sw         $s0, 0x14($sp)
    /* 1C3E4 8001B7E4 AFA50034 */  sw         $a1, 0x34($sp)
    /* 1C3E8 8001B7E8 0C007B7D */  jal        func_8001EDF4
    /* 1C3EC 8001B7EC 2414FFFF */   addiu     $s4, $zero, -0x1
    /* 1C3F0 8001B7F0 2415FFFF */  addiu      $s5, $zero, -0x1
    /* 1C3F4 8001B7F4 10550029 */  beq        $v0, $s5, .L8001B89C
    /* 1C3F8 8001B7F8 00402025 */   or        $a0, $v0, $zero
    /* 1C3FC 8001B7FC 3C118005 */  lui        $s1, %hi(D_8004BEB8)
    /* 1C400 8001B800 2631BEB8 */  addiu      $s1, $s1, %lo(D_8004BEB8)
    /* 1C404 8001B804 241201A0 */  addiu      $s2, $zero, 0x1A0
    /* 1C408 8001B808 308200FF */  andi       $v0, $a0, 0xFF
  .L8001B80C:
    /* 1C40C 8001B80C 00520019 */  multu      $v0, $s2
    /* 1C410 8001B810 00408025 */  or         $s0, $v0, $zero
    /* 1C414 8001B814 304500FF */  andi       $a1, $v0, 0xFF
    /* 1C418 8001B818 326700FF */  andi       $a3, $s3, 0xFF
    /* 1C41C 8001B81C 00007012 */  mflo       $t6
    /* 1C420 8001B820 022E1821 */  addu       $v1, $s1, $t6
    /* 1C424 8001B824 886F0060 */  lwl        $t7, 0x60($v1)
    /* 1C428 8001B828 986F0063 */  lwr        $t7, 0x63($v1)
    /* 1C42C 8001B82C 148F0017 */  bne        $a0, $t7, .L8001B88C
    /* 1C430 8001B830 00000000 */   nop
    /* 1C434 8001B834 88780024 */  lwl        $t8, 0x24($v1)
    /* 1C438 8001B838 98780027 */  lwr        $t8, 0x27($v1)
    /* 1C43C 8001B83C 0000A025 */  or         $s4, $zero, $zero
    /* 1C440 8001B840 24040007 */  addiu      $a0, $zero, 0x7
    /* 1C444 8001B844 33190002 */  andi       $t9, $t8, 0x2
    /* 1C448 8001B848 13200008 */  beqz       $t9, .L8001B86C
    /* 1C44C 8001B84C 00000000 */   nop
    /* 1C450 8001B850 24040007 */  addiu      $a0, $zero, 0x7
    /* 1C454 8001B854 304500FF */  andi       $a1, $v0, 0xFF
    /* 1C458 8001B858 90660055 */  lbu        $a2, 0x55($v1)
    /* 1C45C 8001B85C 0C008184 */  jal        func_80020610
    /* 1C460 8001B860 326700FF */   andi      $a3, $s3, 0xFF
    /* 1C464 8001B864 10000003 */  b          .L8001B874
    /* 1C468 8001B868 00000000 */   nop
  .L8001B86C:
    /* 1C46C 8001B86C 0C008184 */  jal        func_80020610
    /* 1C470 8001B870 9066004B */   lbu       $a2, 0x4B($v1)
  .L8001B874:
    /* 1C474 8001B874 02120019 */  multu      $s0, $s2
    /* 1C478 8001B878 00004012 */  mflo       $t0
    /* 1C47C 8001B87C 02284821 */  addu       $t1, $s1, $t0
    /* 1C480 8001B880 89240010 */  lwl        $a0, 0x10($t1)
    /* 1C484 8001B884 10000003 */  b          .L8001B894
    /* 1C488 8001B888 99240013 */   lwr       $a0, 0x13($t1)
  .L8001B88C:
    /* 1C48C 8001B88C 10000004 */  b          .L8001B8A0
    /* 1C490 8001B890 02801025 */   or        $v0, $s4, $zero
  .L8001B894:
    /* 1C494 8001B894 5495FFDD */  bnel       $a0, $s5, .L8001B80C
    /* 1C498 8001B898 308200FF */   andi      $v0, $a0, 0xFF
  .L8001B89C:
    /* 1C49C 8001B89C 02801025 */  or         $v0, $s4, $zero
  .L8001B8A0:
    /* 1C4A0 8001B8A0 8FBF002C */  lw         $ra, 0x2C($sp)
    /* 1C4A4 8001B8A4 8FB00014 */  lw         $s0, 0x14($sp)
    /* 1C4A8 8001B8A8 8FB10018 */  lw         $s1, 0x18($sp)
    /* 1C4AC 8001B8AC 8FB2001C */  lw         $s2, 0x1C($sp)
    /* 1C4B0 8001B8B0 8FB30020 */  lw         $s3, 0x20($sp)
    /* 1C4B4 8001B8B4 8FB40024 */  lw         $s4, 0x24($sp)
    /* 1C4B8 8001B8B8 8FB50028 */  lw         $s5, 0x28($sp)
    /* 1C4BC 8001B8BC 03E00008 */  jr         $ra
    /* 1C4C0 8001B8C0 27BD0030 */   addiu     $sp, $sp, 0x30
endlabel func_8001B7C0
