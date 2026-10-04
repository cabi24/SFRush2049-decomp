nonmatching func_8001B29C, 0x104

glabel func_8001B29C
    /* 1BE9C 8001B29C 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 1BEA0 8001B2A0 AFBF002C */  sw         $ra, 0x2C($sp)
    /* 1BEA4 8001B2A4 AFB40024 */  sw         $s4, 0x24($sp)
    /* 1BEA8 8001B2A8 AFB30020 */  sw         $s3, 0x20($sp)
    /* 1BEAC 8001B2AC 30B300FF */  andi       $s3, $a1, 0xFF
    /* 1BEB0 8001B2B0 AFB50028 */  sw         $s5, 0x28($sp)
    /* 1BEB4 8001B2B4 AFB2001C */  sw         $s2, 0x1C($sp)
    /* 1BEB8 8001B2B8 AFB10018 */  sw         $s1, 0x18($sp)
    /* 1BEBC 8001B2BC AFB00014 */  sw         $s0, 0x14($sp)
    /* 1BEC0 8001B2C0 AFA50034 */  sw         $a1, 0x34($sp)
    /* 1BEC4 8001B2C4 0C007B7D */  jal        func_8001EDF4
    /* 1BEC8 8001B2C8 2414FFFF */   addiu     $s4, $zero, -0x1
    /* 1BECC 8001B2CC 2415FFFF */  addiu      $s5, $zero, -0x1
    /* 1BED0 8001B2D0 10550029 */  beq        $v0, $s5, .L8001B378
    /* 1BED4 8001B2D4 00402025 */   or        $a0, $v0, $zero
    /* 1BED8 8001B2D8 3C118005 */  lui        $s1, %hi(D_8004BEB8)
    /* 1BEDC 8001B2DC 2631BEB8 */  addiu      $s1, $s1, %lo(D_8004BEB8)
    /* 1BEE0 8001B2E0 241201A0 */  addiu      $s2, $zero, 0x1A0
    /* 1BEE4 8001B2E4 308200FF */  andi       $v0, $a0, 0xFF
  .L8001B2E8:
    /* 1BEE8 8001B2E8 00520019 */  multu      $v0, $s2
    /* 1BEEC 8001B2EC 00408025 */  or         $s0, $v0, $zero
    /* 1BEF0 8001B2F0 304500FF */  andi       $a1, $v0, 0xFF
    /* 1BEF4 8001B2F4 326700FF */  andi       $a3, $s3, 0xFF
    /* 1BEF8 8001B2F8 00007012 */  mflo       $t6
    /* 1BEFC 8001B2FC 022E1821 */  addu       $v1, $s1, $t6
    /* 1BF00 8001B300 886F0060 */  lwl        $t7, 0x60($v1)
    /* 1BF04 8001B304 986F0063 */  lwr        $t7, 0x63($v1)
    /* 1BF08 8001B308 148F0017 */  bne        $a0, $t7, .L8001B368
    /* 1BF0C 8001B30C 00000000 */   nop
    /* 1BF10 8001B310 88780024 */  lwl        $t8, 0x24($v1)
    /* 1BF14 8001B314 98780027 */  lwr        $t8, 0x27($v1)
    /* 1BF18 8001B318 0000A025 */  or         $s4, $zero, $zero
    /* 1BF1C 8001B31C 2404000A */  addiu      $a0, $zero, 0xA
    /* 1BF20 8001B320 33190002 */  andi       $t9, $t8, 0x2
    /* 1BF24 8001B324 13200008 */  beqz       $t9, .L8001B348
    /* 1BF28 8001B328 00000000 */   nop
    /* 1BF2C 8001B32C 2404000A */  addiu      $a0, $zero, 0xA
    /* 1BF30 8001B330 304500FF */  andi       $a1, $v0, 0xFF
    /* 1BF34 8001B334 90660055 */  lbu        $a2, 0x55($v1)
    /* 1BF38 8001B338 0C008184 */  jal        func_80020610
    /* 1BF3C 8001B33C 326700FF */   andi      $a3, $s3, 0xFF
    /* 1BF40 8001B340 10000003 */  b          .L8001B350
    /* 1BF44 8001B344 00000000 */   nop
  .L8001B348:
    /* 1BF48 8001B348 0C008184 */  jal        func_80020610
    /* 1BF4C 8001B34C 9066004B */   lbu       $a2, 0x4B($v1)
  .L8001B350:
    /* 1BF50 8001B350 02120019 */  multu      $s0, $s2
    /* 1BF54 8001B354 00004012 */  mflo       $t0
    /* 1BF58 8001B358 02284821 */  addu       $t1, $s1, $t0
    /* 1BF5C 8001B35C 89240010 */  lwl        $a0, 0x10($t1)
    /* 1BF60 8001B360 10000003 */  b          .L8001B370
    /* 1BF64 8001B364 99240013 */   lwr       $a0, 0x13($t1)
  .L8001B368:
    /* 1BF68 8001B368 10000004 */  b          .L8001B37C
    /* 1BF6C 8001B36C 02801025 */   or        $v0, $s4, $zero
  .L8001B370:
    /* 1BF70 8001B370 5495FFDD */  bnel       $a0, $s5, .L8001B2E8
    /* 1BF74 8001B374 308200FF */   andi      $v0, $a0, 0xFF
  .L8001B378:
    /* 1BF78 8001B378 02801025 */  or         $v0, $s4, $zero
  .L8001B37C:
    /* 1BF7C 8001B37C 8FBF002C */  lw         $ra, 0x2C($sp)
    /* 1BF80 8001B380 8FB00014 */  lw         $s0, 0x14($sp)
    /* 1BF84 8001B384 8FB10018 */  lw         $s1, 0x18($sp)
    /* 1BF88 8001B388 8FB2001C */  lw         $s2, 0x1C($sp)
    /* 1BF8C 8001B38C 8FB30020 */  lw         $s3, 0x20($sp)
    /* 1BF90 8001B390 8FB40024 */  lw         $s4, 0x24($sp)
    /* 1BF94 8001B394 8FB50028 */  lw         $s5, 0x28($sp)
    /* 1BF98 8001B398 03E00008 */  jr         $ra
    /* 1BF9C 8001B39C 27BD0030 */   addiu     $sp, $sp, 0x30
endlabel func_8001B29C
