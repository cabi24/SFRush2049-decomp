# Source: game_code.bin (decompressed)
# Address: 0x800DC248

glabel tournament_standings_display
    /* 800DC248 27BDFFD0 */  addiu $sp, $sp, -48
    /* 800DC24C 3C028011 */  lui $v0, 0x8011
    /* 800DC250 944270E8 */  lhu $v0, 28904($v0)
    /* 800DC254 AFB00018 */  sw $s0, 24($sp)
    /* 800DC258 00808025 */  or $s0, $a0, $zero
    /* 800DC25C AFBF002C */  sw $ra, 44($sp)
    /* 800DC260 AFB40028 */  sw $s4, 40($sp)
    /* 800DC264 AFB30024 */  sw $s3, 36($sp)
    /* 800DC268 AFB20020 */  sw $s2, 32($sp)
    /* 800DC26C AFB1001C */  sw $s1, 28($sp)
    /* 800DC270 1840000A */  blez $v0, 0x800DC29C
    /* 800DC274 00004825 */  or $t1, $zero, $zero
    /* 800DC278 3C138013 */  lui $s3, 0x8013
    /* 800DC27C 2673E618 */  addiu $s3, $s3, -6632
    /* 800DC280 02697021 */  addu $t6, $s3, $t1
    /* 800DC284 25290001 */  addiu $t1, $t1, 1
    /* 800DC288 312FFFFF */  andi $t7, $t1, 0xFFFF
    /* 800DC28C 01E2082A */  slt $at, $t7, $v0
    /* 800DC290 01E04825 */  or $t1, $t7, $zero
    /* 800DC294 1420FFFA */  bne $at, $zero, 0x800DC280
    /* 800DC298 A1C00000 */  sb $zero, 0($t6)
    /* 800DC29C 3C148011 */  lui $s4, 0x8011
    /* 800DC2A0 3C018011 */  lui $at, 0x8011
    /* 800DC2A4 269470EC */  addiu $s4, $s4, 28908
    /* 800DC2A8 A42070F4 */  sh $zero, 28916($at)
    /* 800DC2AC 3C0A8011 */  lui $t2, 0x8011
    /* 800DC2B0 954A70F0 */  lhu $t2, 28912($t2)
    /* 800DC2B4 96980000 */  lhu $t8, 0($s4)
    /* 800DC2B8 3C138013 */  lui $s3, 0x8013
    /* 800DC2BC 3C118011 */  lui $s1, 0x8011
    /* 800DC2C0 030AC821 */  addu $t9, $t8, $t2
    /* 800DC2C4 A6990000 */  sh $t9, 0($s4)
    /* 800DC2C8 92020000 */  lbu $v0, 0($s0)
    /* 800DC2CC 2673E618 */  addiu $s3, $s3, -6632
    /* 800DC2D0 26316FE8 */  addiu $s1, $s1, 28648
    /* 800DC2D4 1040001B */  beq $v0, $zero, 0x800DC344
    /* 800DC2D8 241200FF */  addiu $s2, $zero, 255
    /* 800DC2DC 02227021 */  addu $t6, $s1, $v0
    /* 800DC2E0 91C40000 */  lbu $a0, 0($t6)
    /* 800DC2E4 26100001 */  addiu $s0, $s0, 1
    /* 800DC2E8 14920007 */  bne $a0, $s2, 0x800DC308
    /* 800DC2EC 3C188011 */  lui $t8, 0x8011
    /* 800DC2F0 971870F0 */  lhu $t8, 28912($t8)
    /* 800DC2F4 968F0000 */  lhu $t7, 0($s4)
    /* 800DC2F8 00001025 */  or $v0, $zero, $zero
    /* 800DC2FC 01F8C823 */  subu $t9, $t7, $t8
    /* 800DC300 10000035 */  beq $zero, $zero, 0x800DC3D8
    /* 800DC304 A6990000 */  sh $t9, 0($s4)
    /* 800DC308 0C03706B */  jal 0x800DC1AC
    /* 800DC30C 24050005 */  addiu $a1, $zero, 5
    /* 800DC310 14400007 */  bne $v0, $zero, 0x800DC330
    /* 800DC314 3C0F8011 */  lui $t7, 0x8011
    /* 800DC318 95EF70F0 */  lhu $t7, 28912($t7)
    /* 800DC31C 968E0000 */  lhu $t6, 0($s4)
    /* 800DC320 00001025 */  or $v0, $zero, $zero
    /* 800DC324 01CFC023 */  subu $t8, $t6, $t7
    /* 800DC328 1000002B */  beq $zero, $zero, 0x800DC3D8
    /* 800DC32C A6980000 */  sh $t8, 0($s4)
    /* 800DC330 92020000 */  lbu $v0, 0($s0)
    /* 800DC334 5440FFEA */  .word 0x5440FFEA
    /* 800DC338 02227021 */  addu $t6, $s1, $v0
    /* 800DC33C 3C0A8011 */  lui $t2, 0x8011
    /* 800DC340 954A70F0 */  lhu $t2, 28912($t2)
    /* 800DC344 96990000 */  lhu $t9, 0($s4)
    /* 800DC348 00004825 */  or $t1, $zero, $zero
    /* 800DC34C 032A7023 */  subu $t6, $t9, $t2
    /* 800DC350 0C037048 */  jal 0x800DC120
    /* 800DC354 A68E0000 */  sh $t6, 0($s4)
    /* 800DC358 3C0A8011 */  lui $t2, 0x8011
    /* 800DC35C 954A70F0 */  lhu $t2, 28912($t2)
    /* 800DC360 240F0001 */  addiu $t7, $zero, 1
    /* 800DC364 00002025 */  or $a0, $zero, $zero
    /* 800DC368 014FC004 */  .word 0x014FC004
    /* 800DC36C 2719FFFF */  addiu $t9, $t8, -1
    /* 800DC370 00593024 */  and $a2, $v0, $t9
    /* 800DC374 19400012 */  blez $t2, 0x800DC3C0
    /* 800DC378 96850000 */  lhu $a1, 0($s4)
    /* 800DC37C 00001825 */  or $v1, $zero, $zero
    /* 800DC380 000570C3 */  sra $t6, $a1, 3
    /* 800DC384 026E7821 */  addu $t7, $s3, $t6
    /* 800DC388 91F80000 */  lbu $t8, 0($t7)
    /* 800DC38C 30B90007 */  andi $t9, $a1, 0x0007
    /* 800DC390 25290001 */  addiu $t1, $t1, 1
    /* 800DC394 03387007 */  .word 0x03387007
    /* 800DC398 31CF0001 */  andi $t7, $t6, 0x0001
    /* 800DC39C 006FC004 */  .word 0x006FC004
    /* 800DC3A0 3123FFFF */  andi $v1, $t1, 0xFFFF
    /* 800DC3A4 24A50001 */  addiu $a1, $a1, 1
    /* 800DC3A8 30AEFFFF */  andi $t6, $a1, 0xFFFF
    /* 800DC3AC 006A082A */  slt $at, $v1, $t2
    /* 800DC3B0 01C02825 */  or $a1, $t6, $zero
    /* 800DC3B4 00604825 */  or $t1, $v1, $zero
    /* 800DC3B8 1420FFF1 */  bne $at, $zero, 0x800DC380
    /* 800DC3BC 00982025 */  or $a0, $a0, $t8
    /* 800DC3C0 10C40003 */  beq $a2, $a0, 0x800DC3D0
    /* 800DC3C4 3C018011 */  lui $at, 0x8011
    /* 800DC3C8 10000003 */  beq $zero, $zero, 0x800DC3D8
    /* 800DC3CC 00001025 */  or $v0, $zero, $zero
    /* 800DC3D0 A42070F4 */  sh $zero, 28916($at)
    /* 800DC3D4 24020001 */  addiu $v0, $zero, 1
    /* 800DC3D8 8FBF002C */  lw $ra, 44($sp)
    /* 800DC3DC 8FB00018 */  lw $s0, 24($sp)
    /* 800DC3E0 8FB1001C */  lw $s1, 28($sp)
    /* 800DC3E4 8FB20020 */  lw $s2, 32($sp)
    /* 800DC3E8 8FB30024 */  lw $s3, 36($sp)
    /* 800DC3EC 8FB40028 */  lw $s4, 40($sp)
    /* 800DC3F0 03E00008 */  jr $ra
    /* 800DC3F4 27BD0030 */  addiu $sp, $sp, 48
