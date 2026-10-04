nonmatching func_8001B3A0, 0x104

glabel func_8001B3A0
    /* 1BFA0 8001B3A0 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 1BFA4 8001B3A4 AFBF002C */  sw         $ra, 0x2C($sp)
    /* 1BFA8 8001B3A8 AFB40024 */  sw         $s4, 0x24($sp)
    /* 1BFAC 8001B3AC AFB30020 */  sw         $s3, 0x20($sp)
    /* 1BFB0 8001B3B0 30B300FF */  andi       $s3, $a1, 0xFF
    /* 1BFB4 8001B3B4 AFB50028 */  sw         $s5, 0x28($sp)
    /* 1BFB8 8001B3B8 AFB2001C */  sw         $s2, 0x1C($sp)
    /* 1BFBC 8001B3BC AFB10018 */  sw         $s1, 0x18($sp)
    /* 1BFC0 8001B3C0 AFB00014 */  sw         $s0, 0x14($sp)
    /* 1BFC4 8001B3C4 AFA50034 */  sw         $a1, 0x34($sp)
    /* 1BFC8 8001B3C8 0C007B7D */  jal        func_8001EDF4
    /* 1BFCC 8001B3CC 2414FFFF */   addiu     $s4, $zero, -0x1
    /* 1BFD0 8001B3D0 2415FFFF */  addiu      $s5, $zero, -0x1
    /* 1BFD4 8001B3D4 10550029 */  beq        $v0, $s5, .L8001B47C
    /* 1BFD8 8001B3D8 00402025 */   or        $a0, $v0, $zero
    /* 1BFDC 8001B3DC 3C118005 */  lui        $s1, %hi(D_8004BEB8)
    /* 1BFE0 8001B3E0 2631BEB8 */  addiu      $s1, $s1, %lo(D_8004BEB8)
    /* 1BFE4 8001B3E4 241201A0 */  addiu      $s2, $zero, 0x1A0
    /* 1BFE8 8001B3E8 308200FF */  andi       $v0, $a0, 0xFF
  .L8001B3EC:
    /* 1BFEC 8001B3EC 00520019 */  multu      $v0, $s2
    /* 1BFF0 8001B3F0 00408025 */  or         $s0, $v0, $zero
    /* 1BFF4 8001B3F4 304500FF */  andi       $a1, $v0, 0xFF
    /* 1BFF8 8001B3F8 326700FF */  andi       $a3, $s3, 0xFF
    /* 1BFFC 8001B3FC 00007012 */  mflo       $t6
    /* 1C000 8001B400 022E1821 */  addu       $v1, $s1, $t6
    /* 1C004 8001B404 886F0060 */  lwl        $t7, 0x60($v1)
    /* 1C008 8001B408 986F0063 */  lwr        $t7, 0x63($v1)
    /* 1C00C 8001B40C 148F0017 */  bne        $a0, $t7, .L8001B46C
    /* 1C010 8001B410 00000000 */   nop
    /* 1C014 8001B414 88780024 */  lwl        $t8, 0x24($v1)
    /* 1C018 8001B418 98780027 */  lwr        $t8, 0x27($v1)
    /* 1C01C 8001B41C 0000A025 */  or         $s4, $zero, $zero
    /* 1C020 8001B420 24040083 */  addiu      $a0, $zero, 0x83
    /* 1C024 8001B424 33190002 */  andi       $t9, $t8, 0x2
    /* 1C028 8001B428 13200008 */  beqz       $t9, .L8001B44C
    /* 1C02C 8001B42C 00000000 */   nop
    /* 1C030 8001B430 24040083 */  addiu      $a0, $zero, 0x83
    /* 1C034 8001B434 304500FF */  andi       $a1, $v0, 0xFF
    /* 1C038 8001B438 90660055 */  lbu        $a2, 0x55($v1)
    /* 1C03C 8001B43C 0C008184 */  jal        func_80020610
    /* 1C040 8001B440 326700FF */   andi      $a3, $s3, 0xFF
    /* 1C044 8001B444 10000003 */  b          .L8001B454
    /* 1C048 8001B448 00000000 */   nop
  .L8001B44C:
    /* 1C04C 8001B44C 0C008184 */  jal        func_80020610
    /* 1C050 8001B450 9066004B */   lbu       $a2, 0x4B($v1)
  .L8001B454:
    /* 1C054 8001B454 02120019 */  multu      $s0, $s2
    /* 1C058 8001B458 00004012 */  mflo       $t0
    /* 1C05C 8001B45C 02284821 */  addu       $t1, $s1, $t0
    /* 1C060 8001B460 89240010 */  lwl        $a0, 0x10($t1)
    /* 1C064 8001B464 10000003 */  b          .L8001B474
    /* 1C068 8001B468 99240013 */   lwr       $a0, 0x13($t1)
  .L8001B46C:
    /* 1C06C 8001B46C 10000004 */  b          .L8001B480
    /* 1C070 8001B470 02801025 */   or        $v0, $s4, $zero
  .L8001B474:
    /* 1C074 8001B474 5495FFDD */  bnel       $a0, $s5, .L8001B3EC
    /* 1C078 8001B478 308200FF */   andi      $v0, $a0, 0xFF
  .L8001B47C:
    /* 1C07C 8001B47C 02801025 */  or         $v0, $s4, $zero
  .L8001B480:
    /* 1C080 8001B480 8FBF002C */  lw         $ra, 0x2C($sp)
    /* 1C084 8001B484 8FB00014 */  lw         $s0, 0x14($sp)
    /* 1C088 8001B488 8FB10018 */  lw         $s1, 0x18($sp)
    /* 1C08C 8001B48C 8FB2001C */  lw         $s2, 0x1C($sp)
    /* 1C090 8001B490 8FB30020 */  lw         $s3, 0x20($sp)
    /* 1C094 8001B494 8FB40024 */  lw         $s4, 0x24($sp)
    /* 1C098 8001B498 8FB50028 */  lw         $s5, 0x28($sp)
    /* 1C09C 8001B49C 03E00008 */  jr         $ra
    /* 1C0A0 8001B4A0 27BD0030 */   addiu     $sp, $sp, 0x30
endlabel func_8001B3A0
