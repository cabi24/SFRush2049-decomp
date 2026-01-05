# Source: game_code.bin (decompressed)
# Address: 0x800FE08C

glabel stat_race_start
    /* 800FE08C 27BDFF68 */  addiu $sp, $sp, -152
    /* 800FE090 000E7880 */  sll $t7, $t6, 2
    /* 800FE094 01EE7821 */  addu $t7, $t7, $t6
    /* 800FE098 000F7880 */  sll $t7, $t7, 2
    /* 800FE09C 01EE7823 */  subu $t7, $t7, $t6
    /* 800FE0A0 000F7880 */  sll $t7, $t7, 2
    /* 800FE0A4 030FC021 */  addu $t8, $t8, $t7
    /* 800FE0A8 8F18A160 */  lw $t8, -24224($t8)
    /* 800FE0AC AFBF001C */  sw $ra, 28($sp)
    /* 800FE0B0 AFB00018 */  sw $s0, 24($sp)
    /* 800FE0B4 8F190000 */  lw $t9, 0($t8)
    /* 800FE0B8 00808025 */  or $s0, $a0, $zero
    /* 800FE0BC 24040056 */  addiu $a0, $zero, 86
    /* 800FE0C0 8F2D002C */  lw $t5, 44($t9)
    /* 800FE0C4 2405000E */  addiu $a1, $zero, 14
    /* 800FE0C8 8DAE0000 */  lw $t6, 0($t5)
    /* 800FE0CC 25CF06F4 */  addiu $t7, $t6, 1780
    /* 800FE0D0 0C03718A */  jal 0x800DC628
    /* 800FE0D4 AFAF0040 */  sw $t7, 64($sp)
    /* 800FE0D8 0C037092 */  jal 0x800DC248
    /* 800FE0DC 02002025 */  or $a0, $s0, $zero
    /* 800FE0E0 14400003 */  bne $v0, $zero, 0x800FE0F0
    /* 800FE0E4 27A40094 */  addiu $a0, $sp, 148
    /* 800FE0E8 100000EB */  beq $zero, $zero, 0x800FE498
    /* 800FE0EC 00001025 */  or $v0, $zero, $zero
    /* 800FE0F0 0C03715F */  jal 0x800DC57C
    /* 800FE0F4 24050003 */  addiu $a1, $zero, 3
    /* 800FE0F8 27A40090 */  addiu $a0, $sp, 144
    /* 800FE0FC 0C03715F */  jal 0x800DC57C
    /* 800FE100 24050001 */  addiu $a1, $zero, 1
    /* 800FE104 27A4008C */  addiu $a0, $sp, 140
    /* 800FE108 0C03715F */  jal 0x800DC57C
    /* 800FE10C 24050005 */  addiu $a1, $zero, 5
    /* 800FE110 27A40088 */  addiu $a0, $sp, 136
    /* 800FE114 0C03715F */  jal 0x800DC57C
    /* 800FE118 24050014 */  addiu $a1, $zero, 20
    /* 800FE11C 27A40084 */  addiu $a0, $sp, 132
    /* 800FE120 0C03715F */  jal 0x800DC57C
    /* 800FE124 24050003 */  addiu $a1, $zero, 3
    /* 800FE128 27B0006C */  addiu $s0, $sp, 108
    /* 800FE12C 24080001 */  addiu $t0, $zero, 1
    /* 800FE130 02002025 */  or $a0, $s0, $zero
    /* 800FE134 24050009 */  addiu $a1, $zero, 9
    /* 800FE138 0C03715F */  jal 0x800DC57C
    /* 800FE13C AFA80034 */  sw $t0, 52($sp)
    /* 800FE140 8FA80034 */  lw $t0, 52($sp)
    /* 800FE144 26100004 */  addiu $s0, $s0, 4
    /* 800FE148 25080001 */  addiu $t0, $t0, 1
    /* 800FE14C 29010007 */  slti $at, $t0, 7
    /* 800FE150 5420FFF8 */  .word 0x5420FFF8
    /* 800FE154 02002025 */  or $a0, $s0, $zero
    /* 800FE158 8FAB008C */  lw $t3, 140($sp)
    /* 800FE15C 8FAC0094 */  lw $t4, 148($sp)
    /* 800FE160 3C198011 */  lui $t9, 0x8011
    /* 800FE164 11600009 */  beq $t3, $zero, 0x800FE18C
    /* 800FE168 000CC080 */  sll $t8, $t4, 2
    /* 800FE16C 0338C821 */  addu $t9, $t9, $t8
    /* 800FE170 8F391784 */  lw $t9, 6020($t9)
    /* 800FE174 00002825 */  or $a1, $zero, $zero
    /* 800FE178 24080001 */  addiu $t0, $zero, 1
    /* 800FE17C 00196880 */  sll $t5, $t9, 2
    /* 800FE180 016D082B */  sltu $at, $t3, $t5
    /* 800FE184 14200003 */  bne $at, $zero, 0x800FE194
    /* 800FE188 27A3004C */  addiu $v1, $sp, 76
    /* 800FE18C 100000C2 */  beq $zero, $zero, 0x800FE498
    /* 800FE190 00001025 */  or $v0, $zero, $zero
    /* 800FE194 AC650000 */  sw $a1, 0($v1)
    /* 800FE198 01002825 */  or $a1, $t0, $zero
    /* 800FE19C 28A10006 */  slti $at, $a1, 6
    /* 800FE1A0 25080001 */  addiu $t0, $t0, 1
    /* 800FE1A4 1420FFFB */  bne $at, $zero, 0x800FE194
    /* 800FE1A8 24630004 */  addiu $v1, $v1, 4
    /* 800FE1AC 00002825 */  or $a1, $zero, $zero
    /* 800FE1B0 24080001 */  addiu $t0, $zero, 1
    /* 800FE1B4 240A0006 */  addiu $t2, $zero, 6
    /* 800FE1B8 27A90064 */  addiu $t1, $sp, 100
    /* 800FE1BC 27A7006C */  addiu $a3, $sp, 108
    /* 800FE1C0 29010006 */  slti $at, $t0, 6
    /* 800FE1C4 1020004A */  beq $at, $zero, 0x800FE2F0
    /* 800FE1C8 01002025 */  or $a0, $t0, $zero
    /* 800FE1CC 24180006 */  addiu $t8, $zero, 6
    /* 800FE1D0 03083023 */  subu $a2, $t8, $t0
    /* 800FE1D4 30C20001 */  andi $v0, $a2, 0x0001
    /* 800FE1D8 00057080 */  sll $t6, $a1, 2
    /* 800FE1DC 27AF004C */  addiu $t7, $sp, 76
    /* 800FE1E0 10400018 */  beq $v0, $zero, 0x800FE244
    /* 800FE1E4 01CF1821 */  addu $v1, $t6, $t7
    /* 800FE1E8 0008C880 */  sll $t9, $t0, 2
    /* 800FE1EC 8C640000 */  lw $a0, 0($v1)
    /* 800FE1F0 032F1021 */  addu $v0, $t9, $t7
    /* 800FE1F4 8C450000 */  lw $a1, 0($v0)
    /* 800FE1F8 00046880 */  sll $t5, $a0, 2
    /* 800FE1FC 00ED7021 */  addu $t6, $a3, $t5
    /* 800FE200 0005C880 */  sll $t9, $a1, 2
    /* 800FE204 00F97821 */  addu $t7, $a3, $t9
    /* 800FE208 8DED0000 */  lw $t5, 0($t7)
    /* 800FE20C 8DD80000 */  lw $t8, 0($t6)
    /* 800FE210 00857026 */  xor $t6, $a0, $a1
    /* 800FE214 030D082B */  sltu $at, $t8, $t5
    /* 800FE218 50200009 */  .word 0x50200009
    /* 800FE21C 25040001 */  addiu $a0, $t0, 1
    /* 800FE220 AC6E0000 */  sw $t6, 0($v1)
    /* 800FE224 8C590000 */  lw $t9, 0($v0)
    /* 800FE228 032EC026 */  xor $t8, $t9, $t6
    /* 800FE22C AC580000 */  sw $t8, 0($v0)
    /* 800FE230 8C6D0000 */  lw $t5, 0($v1)
    /* 800FE234 01B8C826 */  xor $t9, $t5, $t8
    /* 800FE238 AC790000 */  sw $t9, 0($v1)
    /* 800FE23C 25040001 */  addiu $a0, $t0, 1
    /* 800FE240 108A002B */  beq $a0, $t2, 0x800FE2F0
    /* 800FE244 00047880 */  sll $t7, $a0, 2
    /* 800FE248 27B8004C */  addiu $t8, $sp, 76
    /* 800FE24C 01F81021 */  addu $v0, $t7, $t8
    /* 800FE250 8C640000 */  lw $a0, 0($v1)
    /* 800FE254 8C460000 */  lw $a2, 0($v0)
    /* 800FE258 00046880 */  sll $t5, $a0, 2
    /* 800FE25C 0006C880 */  sll $t9, $a2, 2
    /* 800FE260 00F97821 */  addu $t7, $a3, $t9
    /* 800FE264 00ED7021 */  addu $t6, $a3, $t5
    /* 800FE268 8DC50000 */  lw $a1, 0($t6)
    /* 800FE26C 8DF80000 */  lw $t8, 0($t7)
    /* 800FE270 00866826 */  xor $t5, $a0, $a2
    /* 800FE274 00B8082B */  sltu $at, $a1, $t8
    /* 800FE278 5020000C */  .word 0x5020000C
    /* 800FE27C 8C460004 */  lw $a2, 4($v0)
    /* 800FE280 AC6D0000 */  sw $t5, 0($v1)
    /* 800FE284 8C4E0000 */  lw $t6, 0($v0)
    /* 800FE288 01CD7826 */  xor $t7, $t6, $t5
    /* 800FE28C AC4F0000 */  sw $t7, 0($v0)
    /* 800FE290 8C780000 */  lw $t8, 0($v1)
    /* 800FE294 030F2026 */  xor $a0, $t8, $t7
    /* 800FE298 0004C880 */  sll $t9, $a0, 2
    /* 800FE29C 00F97821 */  addu $t7, $a3, $t9
    /* 800FE2A0 8DE50000 */  lw $a1, 0($t7)
    /* 800FE2A4 AC640000 */  sw $a0, 0($v1)
    /* 800FE2A8 8C460004 */  lw $a2, 4($v0)
    /* 800FE2AC 0006C080 */  sll $t8, $a2, 2
    /* 800FE2B0 00F86821 */  addu $t5, $a3, $t8
    /* 800FE2B4 8DAE0000 */  lw $t6, 0($t5)
    /* 800FE2B8 0086C826 */  xor $t9, $a0, $a2
    /* 800FE2BC 00AE082B */  sltu $at, $a1, $t6
    /* 800FE2C0 50200009 */  .word 0x50200009
    /* 800FE2C4 24420008 */  addiu $v0, $v0, 8
    /* 800FE2C8 AC790000 */  sw $t9, 0($v1)
    /* 800FE2CC 8C4F0004 */  lw $t7, 4($v0)
    /* 800FE2D0 01F96826 */  xor $t5, $t7, $t9
    /* 800FE2D4 AC4D0004 */  sw $t5, 4($v0)
    /* 800FE2D8 8C6E0000 */  lw $t6, 0($v1)
    /* 800FE2DC 01CD7826 */  xor $t7, $t6, $t5
    /* 800FE2E0 AC6F0000 */  sw $t7, 0($v1)
    /* 800FE2E4 24420008 */  addiu $v0, $v0, 8
    /* 800FE2E8 5449FFDA */  .word 0x5449FFDA
    /* 800FE2EC 8C640000 */  lw $a0, 0($v1)
    /* 800FE2F0 01002825 */  or $a1, $t0, $zero
    /* 800FE2F4 28A10005 */  slti $at, $a1, 5
    /* 800FE2F8 1420FFB1 */  bne $at, $zero, 0x800FE1C0
    /* 800FE2FC 25080001 */  addiu $t0, $t0, 1
    /* 800FE300 3C188011 */  lui $t8, 0x8011
    /* 800FE304 8F18176C */  lw $t8, 5996($t8)
    /* 800FE308 8FAE004C */  lw $t6, 76($sp)
    /* 800FE30C 3C028011 */  lui $v0, 0x8011
    /* 800FE310 030B0019 */  multu $t8, $t3
    /* 800FE314 000EC880 */  sll $t9, $t6, 2
    /* 800FE318 00F97821 */  addu $t7, $a3, $t9
    /* 800FE31C 8DF80000 */  lw $t8, 0($t7)
    /* 800FE320 3C038011 */  lui $v1, 0x8011
    /* 800FE324 24631784 */  addiu $v1, $v1, 6020
    /* 800FE328 2442176C */  addiu $v0, $v0, 5996
    /* 800FE32C 00002025 */  or $a0, $zero, $zero
    /* 800FE330 24080001 */  addiu $t0, $zero, 1
    /* 800FE334 27A50064 */  addiu $a1, $sp, 100
    /* 800FE338 00006812 */  mflo $t5
    /* 800FE33C 01B8082B */  sltu $at, $t5, $t8
    /* 800FE340 10200003 */  beq $at, $zero, 0x800FE350
    /* 800FE344 00000000 */  nop
    /* 800FE348 10000053 */  beq $zero, $zero, 0x800FE498
    /* 800FE34C 00001025 */  or $v0, $zero, $zero
    /* 800FE350 8C4E0000 */  lw $t6, 0($v0)
    /* 800FE354 24420004 */  addiu $v0, $v0, 4
    /* 800FE358 0043082B */  sltu $at, $v0, $v1
    /* 800FE35C 25080001 */  addiu $t0, $t0, 1
    /* 800FE360 1420FFFB */  bne $at, $zero, 0x800FE350
    /* 800FE364 008E2021 */  addu $a0, $a0, $t6
    /* 800FE368 00001025 */  or $v0, $zero, $zero
    /* 800FE36C 24080001 */  addiu $t0, $zero, 1
    /* 800FE370 27A3004C */  addiu $v1, $sp, 76
    /* 800FE374 8C790000 */  lw $t9, 0($v1)
    /* 800FE378 24630004 */  addiu $v1, $v1, 4
    /* 800FE37C 0065082B */  sltu $at, $v1, $a1
    /* 800FE380 00197880 */  sll $t7, $t9, 2
    /* 800FE384 00EF6821 */  addu $t5, $a3, $t7
    /* 800FE388 8DB80000 */  lw $t8, 0($t5)
    /* 800FE38C 25080001 */  addiu $t0, $t0, 1
    /* 800FE390 1420FFF8 */  bne $at, $zero, 0x800FE374
    /* 800FE394 00581021 */  addu $v0, $v0, $t8
    /* 800FE398 008B0019 */  multu $a0, $t3
    /* 800FE39C 00007012 */  mflo $t6
    /* 800FE3A0 104E0003 */  beq $v0, $t6, 0x800FE3B0
    /* 800FE3A4 00000000 */  nop
    /* 800FE3A8 1000003B */  beq $zero, $zero, 0x800FE498
    /* 800FE3AC 00001025 */  or $v0, $zero, $zero
    /* 800FE3B0 0C03F7E2 */  jal 0x800FDF88
    /* 800FE3B4 01802025 */  or $a0, $t4, $zero
    /* 800FE3B8 8FA20040 */  lw $v0, 64($sp)
    /* 800FE3BC 24190001 */  addiu $t9, $zero, 1
    /* 800FE3C0 3C068015 */  lui $a2, 0x8015
    /* 800FE3C4 A0590006 */  sb $t9, 6($v0)
    /* 800FE3C8 8FAF0090 */  lw $t7, 144($sp)
    /* 800FE3CC 24C643D4 */  addiu $a2, $a2, 17364
    /* 800FE3D0 2408004C */  addiu $t0, $zero, 76
    /* 800FE3D4 A04F0005 */  sb $t7, 5($v0)
    /* 800FE3D8 8FAD008C */  lw $t5, 140($sp)
    /* 800FE3DC 3C048015 */  lui $a0, 0x8015
    /* 800FE3E0 2484A118 */  addiu $a0, $a0, -24296
    /* 800FE3E4 A04D0009 */  sb $t5, 9($v0)
    /* 800FE3E8 90CE0000 */  lbu $t6, 0($a2)
    /* 800FE3EC 8FB8008C */  lw $t8, 140($sp)
    /* 800FE3F0 3C018015 */  lui $at, 0x8015
    /* 800FE3F4 01C80019 */  multu $t6, $t0
    /* 800FE3F8 8FAE0088 */  lw $t6, 136($sp)
    /* 800FE3FC 24050002 */  addiu $a1, $zero, 2
    /* 800FE400 27A7006C */  addiu $a3, $sp, 108
    /* 800FE404 0000C812 */  mflo $t9
    /* 800FE408 00997821 */  addu $t7, $a0, $t9
    /* 800FE40C 91ED0001 */  lbu $t5, 1($t7)
    /* 800FE410 002D0821 */  addu $at, $at, $t5
    /* 800FE414 A0385140 */  sb $t8, 20800($at)
    /* 800FE418 AC4E000C */  sw $t6, 12($v0)
    /* 800FE41C 8FB90084 */  lw $t9, 132($sp)
    /* 800FE420 A0590007 */  sb $t9, 7($v0)
    /* 800FE424 90CF0000 */  lbu $t7, 0($a2)
    /* 800FE428 01E80019 */  multu $t7, $t0
    /* 800FE42C 3C0F8015 */  lui $t7, 0x8015
    /* 800FE430 25EF5148 */  addiu $t7, $t7, 20808
    /* 800FE434 0000C012 */  mflo $t8
    /* 800FE438 00986821 */  addu $t5, $a0, $t8
    /* 800FE43C 91AE0001 */  lbu $t6, 1($t5)
    /* 800FE440 8FAD0070 */  lw $t5, 112($sp)
    /* 800FE444 8FB8006C */  lw $t8, 108($sp)
    /* 800FE448 000EC880 */  sll $t9, $t6, 2
    /* 800FE44C 032EC823 */  subu $t9, $t9, $t6
    /* 800FE450 0019C880 */  sll $t9, $t9, 2
    /* 800FE454 032F1821 */  addu $v1, $t9, $t7
    /* 800FE458 0005C880 */  sll $t9, $a1, 2
    /* 800FE45C 00F98021 */  addu $s0, $a3, $t9
    /* 800FE460 00057040 */  sll $t6, $a1, 1
    /* 800FE464 006E1021 */  addu $v0, $v1, $t6
    /* 800FE468 A46D0002 */  sh $t5, 2($v1)
    /* 800FE46C A4780000 */  sh $t8, 0($v1)
    /* 800FE470 8E180004 */  lw $t8, 4($s0)
    /* 800FE474 8E0D0008 */  lw $t5, 8($s0)
    /* 800FE478 8E0E000C */  lw $t6, 12($s0)
    /* 800FE47C 8E0F0000 */  lw $t7, 0($s0)
    /* 800FE480 A4580002 */  sh $t8, 2($v0)
    /* 800FE484 A44D0004 */  sh $t5, 4($v0)
    /* 800FE488 A44E0006 */  sh $t6, 6($v0)
    /* 800FE48C 0C03CD36 */  jal 0x800F34D8
    /* 800FE490 A44F0000 */  sh $t7, 0($v0)
    /* 800FE494 24020001 */  addiu $v0, $zero, 1
    /* 800FE498 8FBF001C */  lw $ra, 28($sp)
    /* 800FE49C 8FB00018 */  lw $s0, 24($sp)
    /* 800FE4A0 27BD0098 */  addiu $sp, $sp, 152
    /* 800FE4A4 03E00008 */  jr $ra
    /* 800FE4A8 00000000 */  nop
