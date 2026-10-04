nonmatching func_8001FD2C, 0x12C

glabel func_8001FD2C
    /* 2092C 8001FD2C 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 20930 8001FD30 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 20934 8001FD34 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 20938 8001FD38 AFB50028 */  sw         $s5, 0x28($sp)
    /* 2093C 8001FD3C AFB30020 */  sw         $s3, 0x20($sp)
    /* 20940 8001FD40 30B300FF */  andi       $s3, $a1, 0xFF
    /* 20944 8001FD44 AFBF002C */  sw         $ra, 0x2C($sp)
    /* 20948 8001FD48 AFB40024 */  sw         $s4, 0x24($sp)
    /* 2094C 8001FD4C AFB2001C */  sw         $s2, 0x1C($sp)
    /* 20950 8001FD50 AFB10018 */  sw         $s1, 0x18($sp)
    /* 20954 8001FD54 AFB00014 */  sw         $s0, 0x14($sp)
    /* 20958 8001FD58 AFA50034 */  sw         $a1, 0x34($sp)
    /* 2095C 8001FD5C 11C00034 */  beqz       $t6, .L8001FE30
    /* 20960 8001FD60 2415FFFF */   addiu     $s5, $zero, -0x1
    /* 20964 8001FD64 0C005165 */  jal        func_80014594
    /* 20968 8001FD68 AFA40030 */   sw        $a0, 0x30($sp)
    /* 2096C 8001FD6C 0C007B7D */  jal        func_8001EDF4
    /* 20970 8001FD70 8FA40030 */   lw        $a0, 0x30($sp)
    /* 20974 8001FD74 2414FFFF */  addiu      $s4, $zero, -0x1
    /* 20978 8001FD78 1054002B */  beq        $v0, $s4, .L8001FE28
    /* 2097C 8001FD7C 00402025 */   or        $a0, $v0, $zero
    /* 20980 8001FD80 3C118005 */  lui        $s1, %hi(D_8004BEB8)
    /* 20984 8001FD84 2631BEB8 */  addiu      $s1, $s1, %lo(D_8004BEB8)
    /* 20988 8001FD88 241201A0 */  addiu      $s2, $zero, 0x1A0
    /* 2098C 8001FD8C 308200FF */  andi       $v0, $a0, 0xFF
  .L8001FD90:
    /* 20990 8001FD90 00520019 */  multu      $v0, $s2
    /* 20994 8001FD94 00408025 */  or         $s0, $v0, $zero
    /* 20998 8001FD98 304500FF */  andi       $a1, $v0, 0xFF
    /* 2099C 8001FD9C 326700FF */  andi       $a3, $s3, 0xFF
    /* 209A0 8001FDA0 00007812 */  mflo       $t7
    /* 209A4 8001FDA4 022F1821 */  addu       $v1, $s1, $t7
    /* 209A8 8001FDA8 88780060 */  lwl        $t8, 0x60($v1)
    /* 209AC 8001FDAC 98780063 */  lwr        $t8, 0x63($v1)
    /* 209B0 8001FDB0 14980017 */  bne        $a0, $t8, .L8001FE10
    /* 209B4 8001FDB4 00000000 */   nop
    /* 209B8 8001FDB8 88790024 */  lwl        $t9, 0x24($v1)
    /* 209BC 8001FDBC 98790027 */  lwr        $t9, 0x27($v1)
    /* 209C0 8001FDC0 0000A825 */  or         $s5, $zero, $zero
    /* 209C4 8001FDC4 2404005B */  addiu      $a0, $zero, 0x5B
    /* 209C8 8001FDC8 33280002 */  andi       $t0, $t9, 0x2
    /* 209CC 8001FDCC 11000008 */  beqz       $t0, .L8001FDF0
    /* 209D0 8001FDD0 00000000 */   nop
    /* 209D4 8001FDD4 2404005B */  addiu      $a0, $zero, 0x5B
    /* 209D8 8001FDD8 304500FF */  andi       $a1, $v0, 0xFF
    /* 209DC 8001FDDC 90660055 */  lbu        $a2, 0x55($v1)
    /* 209E0 8001FDE0 0C008184 */  jal        func_80020610
    /* 209E4 8001FDE4 326700FF */   andi      $a3, $s3, 0xFF
    /* 209E8 8001FDE8 10000003 */  b          .L8001FDF8
    /* 209EC 8001FDEC 00000000 */   nop
  .L8001FDF0:
    /* 209F0 8001FDF0 0C008184 */  jal        func_80020610
    /* 209F4 8001FDF4 9066004B */   lbu       $a2, 0x4B($v1)
  .L8001FDF8:
    /* 209F8 8001FDF8 02120019 */  multu      $s0, $s2
    /* 209FC 8001FDFC 00004812 */  mflo       $t1
    /* 20A00 8001FE00 02295021 */  addu       $t2, $s1, $t1
    /* 20A04 8001FE04 89440010 */  lwl        $a0, 0x10($t2)
    /* 20A08 8001FE08 10000005 */  b          .L8001FE20
    /* 20A0C 8001FE0C 99440013 */   lwr       $a0, 0x13($t2)
  .L8001FE10:
    /* 20A10 8001FE10 0C005177 */  jal        func_800145DC
    /* 20A14 8001FE14 00000000 */   nop
    /* 20A18 8001FE18 10000006 */  b          .L8001FE34
    /* 20A1C 8001FE1C 02A01025 */   or        $v0, $s5, $zero
  .L8001FE20:
    /* 20A20 8001FE20 5494FFDB */  bnel       $a0, $s4, .L8001FD90
    /* 20A24 8001FE24 308200FF */   andi      $v0, $a0, 0xFF
  .L8001FE28:
    /* 20A28 8001FE28 0C005177 */  jal        func_800145DC
    /* 20A2C 8001FE2C 00000000 */   nop
  .L8001FE30:
    /* 20A30 8001FE30 02A01025 */  or         $v0, $s5, $zero
  .L8001FE34:
    /* 20A34 8001FE34 8FBF002C */  lw         $ra, 0x2C($sp)
    /* 20A38 8001FE38 8FB00014 */  lw         $s0, 0x14($sp)
    /* 20A3C 8001FE3C 8FB10018 */  lw         $s1, 0x18($sp)
    /* 20A40 8001FE40 8FB2001C */  lw         $s2, 0x1C($sp)
    /* 20A44 8001FE44 8FB30020 */  lw         $s3, 0x20($sp)
    /* 20A48 8001FE48 8FB40024 */  lw         $s4, 0x24($sp)
    /* 20A4C 8001FE4C 8FB50028 */  lw         $s5, 0x28($sp)
    /* 20A50 8001FE50 03E00008 */  jr         $ra
    /* 20A54 8001FE54 27BD0030 */   addiu     $sp, $sp, 0x30
endlabel func_8001FD2C
