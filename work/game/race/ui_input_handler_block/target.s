# Source: game_code.bin (decompressed)
# Address: 0x800FC0EC

glabel ui_input_handler_block
    /* 800FC0EC 0002CAC0 */  sll $t9, $v0, 11
    /* 800FC0F0 0723005B */  .word 0x0723005B
    /* 800FC0F4 0002CA40 */  sll $t9, $v0, 9
    /* 800FC0F8 3C168015 */  lui $s6, 0x8015
    /* 800FC0FC 26D6A110 */  addiu $s6, $s6, -24304
    /* 800FC100 8EC20000 */  lw $v0, 0($s6)
    /* 800FC104 24140001 */  addiu $s4, $zero, 1
    /* 800FC108 24010004 */  addiu $at, $zero, 4
    /* 800FC10C 16820007 */  bne $s4, $v0, 0x800FC12C
    /* 800FC110 00000000 */  nop
    /* 800FC114 3C0144E1 */  lui $at, 0x44E1
    /* 800FC118 44816000 */  .word 0x44816000
    /* 800FC11C 0C00053C */  jal 0x800014F0
    /* 800FC120 00000000 */  nop
    /* 800FC124 10000039 */  beq $zero, $zero, 0x800FC20C
    /* 800FC128 2404004E */  addiu $a0, $zero, 78
    /* 800FC12C 14410014 */  bne $v0, $at, 0x800FC180
    /* 800FC130 3C0E8014 */  lui $t6, 0x8014
    /* 800FC134 8DCE07BC */  lw $t6, 1980($t6)
    /* 800FC138 3C0F8014 */  lui $t7, 0x8014
    /* 800FC13C 3C014416 */  lui $at, 0x4416
    /* 800FC140 55C0000B */  .word 0x55C0000B
    /* 800FC144 44816000 */  .word 0x44816000
    /* 800FC148 8DEF0804 */  lw $t7, 2052($t7)
    /* 800FC14C 000FC100 */  sll $t8, $t7, 4
    /* 800FC150 030FC023 */  subu $t8, $t8, $t7
    /* 800FC154 0018C080 */  sll $t8, $t8, 2
    /* 800FC158 44982000 */  .word 0x44982000
    /* 800FC15C 0C00053C */  jal 0x800014F0
    /* 800FC160 46802320 */  .word 0x46802320
    /* 800FC164 10000029 */  beq $zero, $zero, 0x800FC20C
    /* 800FC168 2404004E */  addiu $a0, $zero, 78
    /* 800FC16C 44816000 */  .word 0x44816000
    /* 800FC170 0C00053C */  jal 0x800014F0
    /* 800FC174 00000000 */  nop
    /* 800FC178 10000024 */  beq $zero, $zero, 0x800FC20C
    /* 800FC17C 2404004E */  addiu $a0, $zero, 78
    /* 800FC180 24010005 */  addiu $at, $zero, 5
    /* 800FC184 14410006 */  bne $v0, $at, 0x800FC1A0
    /* 800FC188 3C014396 */  lui $at, 0x4396
    /* 800FC18C 44816000 */  .word 0x44816000
    /* 800FC190 0C00053C */  jal 0x800014F0
    /* 800FC194 00000000 */  nop
    /* 800FC198 1000001C */  beq $zero, $zero, 0x800FC20C
    /* 800FC19C 2404004E */  addiu $a0, $zero, 78
    /* 800FC1A0 24010006 */  addiu $at, $zero, 6
    /* 800FC1A4 14410014 */  bne $v0, $at, 0x800FC1F8
    /* 800FC1A8 3C198014 */  lui $t9, 0x8014
    /* 800FC1AC 8F390B08 */  lw $t9, 2824($t9)
    /* 800FC1B0 3C0E8014 */  lui $t6, 0x8014
    /* 800FC1B4 3C014496 */  lui $at, 0x4496
    /* 800FC1B8 5699000B */  .word 0x5699000B
    /* 800FC1BC 44816000 */  .word 0x44816000
    /* 800FC1C0 8DCE0BD8 */  lw $t6, 3032($t6)
    /* 800FC1C4 000E7900 */  sll $t7, $t6, 4
    /* 800FC1C8 01EE7823 */  subu $t7, $t7, $t6
    /* 800FC1CC 000F7880 */  sll $t7, $t7, 2
    /* 800FC1D0 448F3000 */  .word 0x448F3000
    /* 800FC1D4 0C00053C */  jal 0x800014F0
    /* 800FC1D8 46803320 */  .word 0x46803320
    /* 800FC1DC 1000000B */  beq $zero, $zero, 0x800FC20C
    /* 800FC1E0 2404004E */  addiu $a0, $zero, 78
    /* 800FC1E4 44816000 */  .word 0x44816000
    /* 800FC1E8 0C00053C */  jal 0x800014F0
    /* 800FC1EC 00000000 */  nop
    /* 800FC1F0 10000006 */  beq $zero, $zero, 0x800FC20C
    /* 800FC1F4 2404004E */  addiu $a0, $zero, 78
    /* 800FC1F8 0C03EFB6 */  jal 0x800FBED8
    /* 800FC1FC 00000000 */  nop
    /* 800FC200 0C00053C */  jal 0x800014F0
    /* 800FC204 46000306 */  .word 0x46000306
    /* 800FC208 2404004E */  addiu $a0, $zero, 78
    /* 800FC20C 00002825 */  or $a1, $zero, $zero
    /* 800FC210 24060001 */  addiu $a2, $zero, 1
    /* 800FC214 0C02D86A */  jal 0x800B61A8
    /* 800FC218 00003825 */  or $a3, $zero, $zero
    /* 800FC21C 0C035858 */  jal 0x800D6160
    /* 800FC220 00004025 */  or $t0, $zero, $zero
    /* 800FC224 0C03EF98 */  jal 0x800FBE60
    /* 800FC228 00000000 */  nop
    /* 800FC22C 3C118015 */  lui $s1, 0x8015
    /* 800FC230 3C180040 */  lui $t8, 0x0040
    /* 800FC234 3C018011 */  lui $at, 0x8011
    /* 800FC238 263143CC */  addiu $s1, $s1, 17356
    /* 800FC23C AC3874B8 */  sw $t8, 29880($at)
    /* 800FC240 C6280000 */  lwc1 $f8, 0($s1)
    /* 800FC244 3C108014 */  lui $s0, 0x8014
    /* 800FC248 2610401C */  addiu $s0, $s0, 16412
    /* 800FC24C 3C028011 */  lui $v0, 0x8011
    /* 800FC250 E6080000 */  swc1 $f8, 0($s0)
    /* 800FC254 100001CF */  beq $zero, $zero, 0x800FC994
    /* 800FC258 8C4274B4 */  lw $v0, 29876($v0)
    /* 800FC25C 0002CA40 */  sll $t9, $v0, 9
    /* 800FC260 0723018E */  .word 0x0723018E
    /* 800FC264 000271C0 */  sll $t6, $v0, 7
    /* 800FC268 3C108014 */  lui $s0, 0x8014
    /* 800FC26C 2610401C */  addiu $s0, $s0, 16412
    /* 800FC270 3C014040 */  lui $at, 0x4040
    /* 800FC274 4481B000 */  .word 0x4481B000
    /* 800FC278 C60A0000 */  lwc1 $f10, 0($s0)
    /* 800FC27C 3C118015 */  lui $s1, 0x8015
    /* 800FC280 263143CC */  addiu $s1, $s1, 17356
    /* 800FC284 46165100 */  .word 0x46165100
    /* 800FC288 C6260000 */  lwc1 $f6, 0($s1)
    /* 800FC28C 00002025 */  or $a0, $zero, $zero
    /* 800FC290 4606203C */  .word 0x4606203C
    /* 800FC294 00000000 */  nop
    /* 800FC298 45000005 */  .word 0x45000005
    /* 800FC29C 00000000 */  nop
    /* 800FC2A0 0C03204F */  jal 0x800C813C
    /* 800FC2A4 24050001 */  addiu $a1, $zero, 1
    /* 800FC2A8 C6280000 */  lwc1 $f8, 0($s1)
    /* 800FC2AC E6080000 */  swc1 $f8, 0($s0)
    /* 800FC2B0 0C03EA6D */  jal 0x800FA9B4
    /* 800FC2B4 00000000 */  nop
    /* 800FC2B8 0C03EF8C */  jal 0x800FBE30
    /* 800FC2BC 00000000 */  nop
    /* 800FC2C0 3C0E8014 */  lui $t6, 0x8014
    /* 800FC2C4 81CE2699 */  lb $t6, 9881($t6)
    /* 800FC2C8 11C0000B */  beq $t6, $zero, 0x800FC2F8
    /* 800FC2CC 00000000 */  nop
    /* 800FC2D0 0C03DFCF */  jal 0x800F7F3C
    /* 800FC2D4 00000000 */  nop
    /* 800FC2D8 3C0F0100 */  lui $t7, 0x0100
    /* 800FC2DC 3C018011 */  lui $at, 0x8011
    /* 800FC2E0 0C035443 */  jal 0x800D510C
    /* 800FC2E4 AC2F74B8 */  sw $t7, 29880($at)
    /* 800FC2E8 0C035633 */  jal 0x800D58CC
    /* 800FC2EC 00000000 */  nop
    /* 800FC2F0 10000166 */  beq $zero, $zero, 0x800FC88C
    /* 800FC2F4 00000000 */  nop
    /* 800FC2F8 3C158014 */  lui $s5, 0x8014
    /* 800FC2FC 26B5FECB */  addiu $s5, $s5, -309
    /* 800FC300 82A70000 */  lb $a3, 0($s5)
    /* 800FC304 3C168015 */  lui $s6, 0x8015
    /* 800FC308 3C058015 */  lui $a1, 0x8015
    /* 800FC30C 10E0003C */  beq $a3, $zero, 0x800FC400
    /* 800FC310 26D6A110 */  addiu $s6, $s6, -24304
    /* 800FC314 84A5A108 */  lh $a1, -24312($a1)
    /* 800FC318 24120001 */  addiu $s2, $zero, 1
    /* 800FC31C 00008025 */  or $s0, $zero, $zero
    /* 800FC320 18A00012 */  blez $a1, 0x800FC36C
    /* 800FC324 00000000 */  nop
    /* 800FC328 3C118014 */  lui $s1, 0x8014
    /* 800FC32C 26312760 */  addiu $s1, $s1, 10080
    /* 800FC330 82380000 */  lb $t8, 0($s1)
    /* 800FC334 00102400 */  sll $a0, $s0, 16
    /* 800FC338 0004CC03 */  sra $t9, $a0, 16
    /* 800FC33C 57000008 */  .word 0x57000008
    /* 800FC340 26100001 */  addiu $s0, $s0, 1
    /* 800FC344 0C033D81 */  jal 0x800CF604
    /* 800FC348 03202025 */  or $a0, $t9, $zero
    /* 800FC34C 14400002 */  bne $v0, $zero, 0x800FC358
    /* 800FC350 3C058015 */  lui $a1, 0x8015
    /* 800FC354 00009025 */  or $s2, $zero, $zero
    /* 800FC358 84A5A108 */  lh $a1, -24312($a1)
    /* 800FC35C 26100001 */  addiu $s0, $s0, 1
    /* 800FC360 0205082A */  slt $at, $s0, $a1
    /* 800FC364 5420FFF3 */  .word 0x5420FFF3
    /* 800FC368 82380000 */  lb $t8, 0($s1)
    /* 800FC36C 0C039F74 */  jal 0x800E7DD0
    /* 800FC370 00000000 */  nop
    /* 800FC374 10400010 */  beq $v0, $zero, 0x800FC3B8
    /* 800FC378 00000000 */  nop
    /* 800FC37C 1240000E */  beq $s2, $zero, 0x800FC3B8
    /* 800FC380 3C168015 */  lui $s6, 0x8015
    /* 800FC384 26D6A110 */  addiu $s6, $s6, -24304
    /* 800FC388 8ECE0000 */  lw $t6, 0($s6)
    /* 800FC38C 24010004 */  addiu $at, $zero, 4
    /* 800FC390 55C10004 */  .word 0x55C10004
    /* 800FC394 3C0F0100 */  lui $t7, 0x0100
    /* 800FC398 0C03DFCF */  jal 0x800F7F3C
    /* 800FC39C 00000000 */  nop
    /* 800FC3A0 3C0F0100 */  lui $t7, 0x0100
    /* 800FC3A4 3C018011 */  lui $at, 0x8011
    /* 800FC3A8 0C035443 */  jal 0x800D510C
    /* 800FC3AC AC2F74B8 */  sw $t7, 29880($at)
    /* 800FC3B0 0C035633 */  jal 0x800D58CC
    /* 800FC3B4 00000000 */  nop
    /* 800FC3B8 3C168015 */  lui $s6, 0x8015
    /* 800FC3BC 0C00056F */  jal 0x800015BC
    /* 800FC3C0 26D6A110 */  addiu $s6, $s6, -24304
    /* 800FC3C4 14400131 */  bne $v0, $zero, 0x800FC88C
    /* 800FC3C8 00000000 */  nop
    /* 800FC3CC 8EC20000 */  lw $v0, 0($s6)
    /* 800FC3D0 24010004 */  addiu $at, $zero, 4
    /* 800FC3D4 1041012D */  beq $v0, $at, 0x800FC88C
    /* 800FC3D8 24010005 */  addiu $at, $zero, 5
    /* 800FC3DC 1041012B */  beq $v0, $at, 0x800FC88C
    /* 800FC3E0 24010006 */  addiu $at, $zero, 6
    /* 800FC3E4 10410129 */  beq $v0, $at, 0x800FC88C
    /* 800FC3E8 00000000 */  nop
    /* 800FC3EC 3C128014 */  lui $s2, 0x8014
    /* 800FC3F0 26522690 */  addiu $s2, $s2, 9872
    /* 800FC3F4 A2A00000 */  sb $zero, 0($s5)
    /* 800FC3F8 10000124 */  beq $zero, $zero, 0x800FC88C
    /* 800FC3FC A2400000 */  sb $zero, 0($s2)
    /* 800FC400 8EC20000 */  lw $v0, 0($s6)
    /* 800FC404 24010004 */  addiu $at, $zero, 4
    /* 800FC408 54410051 */  .word 0x54410051
    /* 800FC40C 24010006 */  addiu $at, $zero, 6
    /* 800FC410 3C028014 */  lui $v0, 0x8014
    /* 800FC414 8C4207BC */  lw $v0, 1980($v0)
    /* 800FC418 10400009 */  beq $v0, $zero, 0x800FC440
    /* 800FC41C 00000000 */  nop
    /* 800FC420 24140001 */  addiu $s4, $zero, 1
    /* 800FC424 10540010 */  beq $v0, $s4, 0x800FC468
    /* 800FC428 3C058015 */  lui $a1, 0x8015
    /* 800FC42C 24010002 */  addiu $at, $zero, 2
    /* 800FC430 10410026 */  beq $v0, $at, 0x800FC4CC
    /* 800FC434 3C058015 */  lui $a1, 0x8015
    /* 800FC438 10000114 */  beq $zero, $zero, 0x800FC88C
    /* 800FC43C 00000000 */  nop
    /* 800FC440 0C00056F */  jal 0x800015BC
    /* 800FC444 00000000 */  nop
    /* 800FC448 10400110 */  beq $v0, $zero, 0x800FC88C
    /* 800FC44C 00000000 */  nop
    /* 800FC450 3C128014 */  lui $s2, 0x8014
    /* 800FC454 26522690 */  addiu $s2, $s2, 9872
    /* 800FC458 24140001 */  addiu $s4, $zero, 1
    /* 800FC45C A2B40000 */  sb $s4, 0($s5)
    /* 800FC460 1000010A */  beq $zero, $zero, 0x800FC88C
    /* 800FC464 A2540000 */  sb $s4, 0($s2)
    /* 800FC468 84A5A108 */  lh $a1, -24312($a1)
    /* 800FC46C 3C188015 */  lui $t8, 0x8015
    /* 800FC470 27022038 */  addiu $v0, $t8, 8248
    /* 800FC474 18A00105 */  blez $a1, 0x800FC88C
    /* 800FC478 00008025 */  or $s0, $zero, $zero
    /* 800FC47C 0005C900 */  sll $t9, $a1, 4
    /* 800FC480 0325C823 */  subu $t9, $t9, $a1
    /* 800FC484 0019C8C0 */  sll $t9, $t9, 3
    /* 800FC488 3C038014 */  lui $v1, 0x8014
    /* 800FC48C 3C128014 */  lui $s2, 0x8014
    /* 800FC490 26522690 */  addiu $s2, $s2, 9872
    /* 800FC494 8C630A00 */  lw $v1, 2560($v1)
    /* 800FC498 03222021 */  addu $a0, $t9, $v0
    /* 800FC49C 8C4E0014 */  lw $t6, 20($v0)
    /* 800FC4A0 24420078 */  addiu $v0, $v0, 120
    /* 800FC4A4 01C3082A */  slt $at, $t6, $v1
    /* 800FC4A8 54200004 */  .word 0x54200004
    /* 800FC4AC 0044082B */  sltu $at, $v0, $a0
    /* 800FC4B0 A2B40000 */  sb $s4, 0($s5)
    /* 800FC4B4 A2540000 */  sb $s4, 0($s2)
    /* 800FC4B8 0044082B */  sltu $at, $v0, $a0
    /* 800FC4BC 5420FFF8 */  .word 0x5420FFF8
    /* 800FC4C0 8C4E0014 */  lw $t6, 20($v0)
    /* 800FC4C4 100000F1 */  beq $zero, $zero, 0x800FC88C
    /* 800FC4C8 00000000 */  nop
    /* 800FC4CC 84A5A108 */  lh $a1, -24312($a1)
    /* 800FC4D0 00001025 */  or $v0, $zero, $zero
    /* 800FC4D4 00008025 */  or $s0, $zero, $zero
    /* 800FC4D8 18A00015 */  blez $a1, 0x800FC530
    /* 800FC4DC 3C0F8015 */  lui $t7, 0x8015
    /* 800FC4E0 0005C900 */  sll $t9, $a1, 4
    /* 800FC4E4 0325C823 */  subu $t9, $t9, $a1
    /* 800FC4E8 0019C8C0 */  sll $t9, $t9, 3
    /* 800FC4EC 0325C823 */  subu $t9, $t9, $a1
    /* 800FC4F0 0019C8C0 */  sll $t9, $t9, 3
    /* 800FC4F4 25E32818 */  addiu $v1, $t7, 10264
    /* 800FC4F8 3C048014 */  lui $a0, 0x8014
    /* 800FC4FC 8C840AD8 */  lw $a0, 2776($a0)
    /* 800FC500 03233021 */  addu $a2, $t9, $v1
    /* 800FC504 8C780380 */  lw $t8, 896($v1)
    /* 800FC508 970E0040 */  lhu $t6, 64($t8)
    /* 800FC50C 01C4082A */  slt $at, $t6, $a0
    /* 800FC510 54200004 */  .word 0x54200004
    /* 800FC514 246303B8 */  addiu $v1, $v1, 952
    /* 800FC518 A0740359 */  sb $s4, 857($v1)
    /* 800FC51C 24420001 */  addiu $v0, $v0, 1
    /* 800FC520 246303B8 */  addiu $v1, $v1, 952
    /* 800FC524 0066082B */  sltu $at, $v1, $a2
    /* 800FC528 5420FFF7 */  .word 0x5420FFF7
    /* 800FC52C 8C780380 */  lw $t8, 896($v1)
    /* 800FC530 144500D6 */  bne $v0, $a1, 0x800FC88C
    /* 800FC534 00000000 */  nop
    /* 800FC538 3C128014 */  lui $s2, 0x8014
    /* 800FC53C 26522690 */  addiu $s2, $s2, 9872
    /* 800FC540 A2B40000 */  sb $s4, 0($s5)
    /* 800FC544 100000D1 */  beq $zero, $zero, 0x800FC88C
    /* 800FC548 A2540000 */  sb $s4, 0($s2)
    /* 800FC54C 24010006 */  addiu $at, $zero, 6
    /* 800FC550 14410050 */  bne $v0, $at, 0x800FC694
    /* 800FC554 00000000 */  nop
    /* 800FC558 3C028014 */  lui $v0, 0x8014
    /* 800FC55C 8C420B08 */  lw $v0, 2824($v0)
    /* 800FC560 24140001 */  addiu $s4, $zero, 1
    /* 800FC564 00002825 */  or $a1, $zero, $zero
    /* 800FC568 1040000E */  beq $v0, $zero, 0x800FC5A4
    /* 800FC56C 24060004 */  addiu $a2, $zero, 4
    /* 800FC570 145400C6 */  bne $v0, $s4, 0x800FC88C
    /* 800FC574 00000000 */  nop
    /* 800FC578 0C00056F */  jal 0x800015BC
    /* 800FC57C 00000000 */  nop
    /* 800FC580 104000C2 */  beq $v0, $zero, 0x800FC88C
    /* 800FC584 00000000 */  nop
    /* 800FC588 0C03DFCF */  jal 0x800F7F3C
    /* 800FC58C 00000000 */  nop
    /* 800FC590 3C128014 */  lui $s2, 0x8014
    /* 800FC594 26522690 */  addiu $s2, $s2, 9872
    /* 800FC598 A2B40000 */  sb $s4, 0($s5)
    /* 800FC59C 100000BB */  beq $zero, $zero, 0x800FC88C
    /* 800FC5A0 A2540000 */  sb $s4, 0($s2)
    /* 800FC5A4 3C118015 */  lui $s1, 0x8015
    /* 800FC5A8 2631256C */  addiu $s1, $s1, 9580
    /* 800FC5AC 0C0009E4 */  jal 0x80002790
    /* 800FC5B0 02202025 */  or $a0, $s1, $zero
    /* 800FC5B4 3C058015 */  lui $a1, 0x8015
    /* 800FC5B8 84A5A108 */  lh $a1, -24312($a1)
    /* 800FC5BC 3C108015 */  lui $s0, 0x8015
    /* 800FC5C0 2610256C */  addiu $s0, $s0, 9580
    /* 800FC5C4 18A00010 */  blez $a1, 0x800FC608
    /* 800FC5C8 24140001 */  addiu $s4, $zero, 1
    /* 800FC5CC 3C198013 */  lui $t9, 0x8013
    /* 800FC5D0 2724E67C */  addiu $a0, $t9, -6532
    /* 800FC5D4 3C038015 */  lui $v1, 0x8015
    /* 800FC5D8 24632818 */  addiu $v1, $v1, 10264
    /* 800FC5DC 00A43021 */  addu $a2, $a1, $a0
    /* 800FC5E0 808F0000 */  lb $t7, 0($a0)
    /* 800FC5E4 806E03A3 */  lb $t6, 931($v1)
    /* 800FC5E8 24840001 */  addiu $a0, $a0, 1
    /* 800FC5EC 022F1021 */  addu $v0, $s1, $t7
    /* 800FC5F0 80580000 */  lb $t8, 0($v0)
    /* 800FC5F4 0086082B */  sltu $at, $a0, $a2
    /* 800FC5F8 246303B8 */  addiu $v1, $v1, 952
    /* 800FC5FC 030EC821 */  addu $t9, $t8, $t6
    /* 800FC600 1420FFF7 */  bne $at, $zero, 0x800FC5E0
    /* 800FC604 A0590000 */  sb $t9, 0($v0)
    /* 800FC608 3C138015 */  lui $s3, 0x8015
    /* 800FC60C 3C128014 */  lui $s2, 0x8014
    /* 800FC610 3C118011 */  lui $s1, 0x8011
    /* 800FC614 26314654 */  addiu $s1, $s1, 18004
    /* 800FC618 26522690 */  addiu $s2, $s2, 9872
    /* 800FC61C 26732570 */  addiu $s3, $s3, 9584
    /* 800FC620 822F0000 */  lb $t7, 0($s1)
    /* 800FC624 3C038014 */  lui $v1, 0x8014
    /* 800FC628 8C632510 */  lw $v1, 9488($v1)
    /* 800FC62C 55E0000D */  .word 0x55E0000D
    /* 800FC630 82190000 */  lb $t9, 0($s0)
    /* 800FC634 82180000 */  lb $t8, 0($s0)
    /* 800FC638 24040002 */  addiu $a0, $zero, 2
    /* 800FC63C 00002825 */  or $a1, $zero, $zero
    /* 800FC640 270E0001 */  addiu $t6, $t8, 1
    /* 800FC644 146E0006 */  bne $v1, $t6, 0x800FC660
    /* 800FC648 24060002 */  addiu $a2, $zero, 2
    /* 800FC64C A2340000 */  sb $s4, 0($s1)
    /* 800FC650 0C02D86A */  jal 0x800B61A8
    /* 800FC654 00003825 */  or $a3, $zero, $zero
    /* 800FC658 3C038014 */  lui $v1, 0x8014
    /* 800FC65C 8C632510 */  lw $v1, 9488($v1)
    /* 800FC660 82190000 */  lb $t9, 0($s0)
    /* 800FC664 0323082A */  slt $at, $t9, $v1
    /* 800FC668 54200006 */  .word 0x54200006
    /* 800FC66C 26100001 */  addiu $s0, $s0, 1
    /* 800FC670 0C03DFCF */  jal 0x800F7F3C
    /* 800FC674 00000000 */  nop
    /* 800FC678 A2B40000 */  sb $s4, 0($s5)
    /* 800FC67C A2540000 */  sb $s4, 0($s2)
    /* 800FC680 26100001 */  addiu $s0, $s0, 1
    /* 800FC684 5613FFE7 */  .word 0x5613FFE7
    /* 800FC688 822F0000 */  lb $t7, 0($s1)
    /* 800FC68C 1000007F */  beq $zero, $zero, 0x800FC88C
    /* 800FC690 00000000 */  nop
    /* 800FC694 10400004 */  beq $v0, $zero, 0x800FC6A8
    /* 800FC698 24140001 */  addiu $s4, $zero, 1
    /* 800FC69C 12820002 */  beq $s4, $v0, 0x800FC6A8
    /* 800FC6A0 24010003 */  addiu $at, $zero, 3
    /* 800FC6A4 1441003F */  bne $v0, $at, 0x800FC7A4
    /* 800FC6A8 3C058015 */  lui $a1, 0x8015
    /* 800FC6AC 84A5A108 */  lh $a1, -24312($a1)
    /* 800FC6B0 24140001 */  addiu $s4, $zero, 1
    /* 800FC6B4 00008025 */  or $s0, $zero, $zero
    /* 800FC6B8 18A00015 */  blez $a1, 0x800FC710
    /* 800FC6BC 3C068014 */  lui $a2, 0x8014
    /* 800FC6C0 3C028015 */  lui $v0, 0x8015
    /* 800FC6C4 3C118015 */  lui $s1, 0x8015
    /* 800FC6C8 26312818 */  addiu $s1, $s1, 10264
    /* 800FC6CC 2442A118 */  addiu $v0, $v0, -24296
    /* 800FC6D0 240303B8 */  addiu $v1, $zero, 952
    /* 800FC6D4 904F0000 */  lbu $t7, 0($v0)
    /* 800FC6D8 01E30019 */  multu $t7, $v1
    /* 800FC6DC 0000C012 */  mflo $t8
    /* 800FC6E0 02387021 */  addu $t6, $s1, $t8
    /* 800FC6E4 81D900EF */  lb $t9, 239($t6)
    /* 800FC6E8 57200005 */  .word 0x57200005
    /* 800FC6EC 26100001 */  addiu $s0, $s0, 1
    /* 800FC6F0 3C028011 */  lui $v0, 0x8011
    /* 800FC6F4 100000A7 */  beq $zero, $zero, 0x800FC994
    /* 800FC6F8 8C4274B4 */  lw $v0, 29876($v0)
    /* 800FC6FC 26100001 */  addiu $s0, $s0, 1
    /* 800FC700 0205082A */  slt $at, $s0, $a1
    /* 800FC704 1420FFF3 */  bne $at, $zero, 0x800FC6D4
    /* 800FC708 2442004C */  addiu $v0, $v0, 76
    /* 800FC70C 00008025 */  or $s0, $zero, $zero
    /* 800FC710 24C63F10 */  addiu $a2, $a2, 16144
    /* 800FC714 18A00011 */  blez $a1, 0x800FC75C
    /* 800FC718 ACC00000 */  sw $zero, 0($a2)
    /* 800FC71C 0005C200 */  sll $t8, $a1, 8
    /* 800FC720 0305C021 */  addu $t8, $t8, $a1
    /* 800FC724 3C0F8015 */  lui $t7, 0x8015
    /* 800FC728 25E2A250 */  addiu $v0, $t7, -23984
    /* 800FC72C 0018C0C0 */  sll $t8, $t8, 3
    /* 800FC730 3C048015 */  lui $a0, 0x8015
    /* 800FC734 84842734 */  lh $a0, 10036($a0)
    /* 800FC738 03021821 */  addu $v1, $t8, $v0
    /* 800FC73C 804E07E8 */  lb $t6, 2024($v0)
    /* 800FC740 24420808 */  addiu $v0, $v0, 2056
    /* 800FC744 0043082B */  sltu $at, $v0, $v1
    /* 800FC748 148E0002 */  bne $a0, $t6, 0x800FC754
    /* 800FC74C 00000000 */  nop
    /* 800FC750 ACD40000 */  sw $s4, 0($a2)
    /* 800FC754 5420FFFA */  .word 0x5420FFFA
    /* 800FC758 804E07E8 */  lb $t6, 2024($v0)
    /* 800FC75C 14E0004B */  bne $a3, $zero, 0x800FC88C
    /* 800FC760 00000000 */  nop
    /* 800FC764 3C118014 */  lui $s1, 0x8014
    /* 800FC768 26312760 */  addiu $s1, $s1, 10080
    /* 800FC76C 82390000 */  lb $t9, 0($s1)
    /* 800FC770 13200004 */  beq $t9, $zero, 0x800FC784
    /* 800FC774 00000000 */  nop
    /* 800FC778 44805000 */  .word 0x44805000
    /* 800FC77C 3C018015 */  lui $at, 0x8015
    /* 800FC780 E42A25F4 */  swc1 $f10, 9716($at)
    /* 800FC784 0C03DFCF */  jal 0x800F7F3C
    /* 800FC788 A2B40000 */  sb $s4, 0($s5)
    /* 800FC78C 4480A000 */  .word 0x4480A000
    /* 800FC790 24110001 */  addiu $s1, $zero, 1
    /* 800FC794 0C032484 */  jal 0x800C9210
    /* 800FC798 00009025 */  or $s2, $zero, $zero
    /* 800FC79C 1000003B */  beq $zero, $zero, 0x800FC88C
    /* 800FC7A0 00000000 */  nop
    /* 800FC7A4 0C00056F */  jal 0x800015BC
    /* 800FC7A8 00000000 */  nop
    /* 800FC7AC 10400037 */  beq $v0, $zero, 0x800FC88C
    /* 800FC7B0 00000000 */  nop
    /* 800FC7B4 82B80000 */  lb $t8, 0($s5)
    /* 800FC7B8 17000007 */  bne $t8, $zero, 0x800FC7D8
    /* 800FC7BC 00000000 */  nop
    /* 800FC7C0 0C03DFCF */  jal 0x800F7F3C
    /* 800FC7C4 A2B40000 */  sb $s4, 0($s5)
    /* 800FC7C8 4480A000 */  .word 0x4480A000
    /* 800FC7CC 02808825 */  or $s1, $s4, $zero
    /* 800FC7D0 0C032484 */  jal 0x800C9210
    /* 800FC7D4 00009025 */  or $s2, $zero, $zero
    /* 800FC7D8 3C058015 */  lui $a1, 0x8015
    /* 800FC7DC 84A5A108 */  lh $a1, -24312($a1)
    /* 800FC7E0 3C028015 */  lui $v0, 0x8015
    /* 800FC7E4 2442A118 */  addiu $v0, $v0, -24296
    /* 800FC7E8 18A00028 */  blez $a1, 0x800FC88C
    /* 800FC7EC 00008025 */  or $s0, $zero, $zero
    /* 800FC7F0 3C048015 */  lui $a0, 0x8015
    /* 800FC7F4 3C128015 */  lui $s2, 0x8015
    /* 800FC7F8 2652A250 */  addiu $s2, $s2, -23984
    /* 800FC7FC 84842734 */  lh $a0, 10036($a0)
    /* 800FC800 24060808 */  addiu $a2, $zero, 2056
    /* 800FC804 90430000 */  lbu $v1, 0($v0)
    /* 800FC808 00660019 */  multu $v1, $a2
    /* 800FC80C 00007812 */  mflo $t7
    /* 800FC810 024F7021 */  addu $t6, $s2, $t7
    /* 800FC814 81D907E8 */  lb $t9, 2024($t6)
    /* 800FC818 50990019 */  .word 0x50990019
    /* 800FC81C 26100001 */  addiu $s0, $s0, 1
    /* 800FC820 8ED80000 */  lw $t8, 0($s6)
    /* 800FC824 24010002 */  addiu $at, $zero, 2
    /* 800FC828 00037880 */  sll $t7, $v1, 2
    /* 800FC82C 1701000A */  bne $t8, $at, 0x800FC858
    /* 800FC830 3C028015 */  lui $v0, 0x8015
    /* 800FC834 004F1021 */  addu $v0, $v0, $t7
    /* 800FC838 8C422698 */  lw $v0, 9880($v0)
    /* 800FC83C 50400007 */  .word 0x50400007
    /* 800FC840 24040016 */  addiu $a0, $zero, 22
    /* 800FC844 8C4E0000 */  lw $t6, 0($v0)
    /* 800FC848 8DD90028 */  lw $t9, 40($t6)
    /* 800FC84C 8F380000 */  lw $t8, 0($t9)
    /* 800FC850 830F0005 */  lb $t7, 5($t8)
    /* 800FC854 05E10005 */  bgez $t7, 0x800FC86C
    /* 800FC858 24040016 */  addiu $a0, $zero, 22
    /* 800FC85C 00002825 */  or $a1, $zero, $zero
    /* 800FC860 02803025 */  or $a2, $s4, $zero
    /* 800FC864 0C02D86A */  jal 0x800B61A8
    /* 800FC868 00003825 */  or $a3, $zero, $zero
    /* 800FC86C 3C128014 */  lui $s2, 0x8014
    /* 800FC870 26522690 */  addiu $s2, $s2, 9872
    /* 800FC874 10000005 */  beq $zero, $zero, 0x800FC88C
    /* 800FC878 A2540000 */  sb $s4, 0($s2)
    /* 800FC87C 26100001 */  addiu $s0, $s0, 1
    /* 800FC880 0205082A */  slt $at, $s0, $a1
    /* 800FC884 1420FFDF */  bne $at, $zero, 0x800FC804
    /* 800FC888 2442004C */  addiu $v0, $v0, 76
    /* 800FC88C 3C028011 */  lui $v0, 0x8011
    /* 800FC890 10000040 */  beq $zero, $zero, 0x800FC994
    /* 800FC894 8C4274B4 */  lw $v0, 29876($v0)
    /* 800FC898 000271C0 */  sll $t6, $v0, 7
    /* 800FC89C 05C10030 */  bgez $t6, 0x800FC960
    /* 800FC8A0 00027980 */  sll $t7, $v0, 6
    /* 800FC8A4 3C138015 */  lui $s3, 0x8015
    /* 800FC8A8 26732744 */  addiu $s3, $s3, 10052
    /* 800FC8AC 82790000 */  lb $t9, 0($s3)
    /* 800FC8B0 00008025 */  or $s0, $zero, $zero
    /* 800FC8B4 1B20001C */  blez $t9, 0x800FC928
    /* 800FC8B8 00000000 */  nop
