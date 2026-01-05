# Source: game_code.bin (decompressed)
# Address: 0x800FF298

glabel game_results_render
    /* 800FF298 27BDFEE0 */  addiu $sp, $sp, -288
    /* 800FF29C 3C0E8011 */  lui $t6, 0x8011
    /* 800FF2A0 8DCE05B4 */  lw $t6, 1460($t6)
    /* 800FF2A4 24010001 */  addiu $at, $zero, 1
    /* 800FF2A8 AFBF006C */  sw $ra, 108($sp)
    /* 800FF2AC AFBE0068 */  sw $fp, 104($sp)
    /* 800FF2B0 AFB70064 */  sw $s7, 100($sp)
    /* 800FF2B4 AFB60060 */  sw $s6, 96($sp)
    /* 800FF2B8 AFB5005C */  sw $s5, 92($sp)
    /* 800FF2BC AFB40058 */  sw $s4, 88($sp)
    /* 800FF2C0 AFB30054 */  sw $s3, 84($sp)
    /* 800FF2C4 AFB20050 */  sw $s2, 80($sp)
    /* 800FF2C8 AFB1004C */  sw $s1, 76($sp)
    /* 800FF2CC AFB00048 */  sw $s0, 72($sp)
    /* 800FF2D0 F7BE0040 */  .word 0xF7BE0040
    /* 800FF2D4 F7BC0038 */  .word 0xF7BC0038
    /* 800FF2D8 F7BA0030 */  .word 0xF7BA0030
    /* 800FF2DC F7B80028 */  .word 0xF7B80028
    /* 800FF2E0 F7B60020 */  .word 0xF7B60020
    /* 800FF2E4 F7B40018 */  .word 0xF7B40018
    /* 800FF2E8 15C10005 */  bne $t6, $at, 0x800FF300
    /* 800FF2EC AFA40120 */  sw $a0, 288($sp)
    /* 800FF2F0 0C03FB81 */  jal 0x800FEE04
    /* 800FF2F4 00000000 */  nop
    /* 800FF2F8 100000F8 */  beq $zero, $zero, 0x800FF6DC
    /* 800FF2FC 24020001 */  addiu $v0, $zero, 1
    /* 800FF300 44806000 */  .word 0x44806000
    /* 800FF304 0C02D96D */  jal 0x800B65B4
    /* 800FF308 00000000 */  nop
    /* 800FF30C 3C168014 */  lui $s6, 0x8014
    /* 800FF310 26D661D0 */  addiu $s6, $s6, 25040
    /* 800FF314 02C02025 */  or $a0, $s6, $zero
    /* 800FF318 00002825 */  or $a1, $zero, $zero
    /* 800FF31C 0C001C9C */  jal 0x80007270
    /* 800FF320 24060001 */  addiu $a2, $zero, 1
    /* 800FF324 0C02D080 */  jal 0x800B4200
    /* 800FF328 2412000D */  addiu $s2, $zero, 13
    /* 800FF32C 00408025 */  or $s0, $v0, $zero
    /* 800FF330 02C02025 */  or $a0, $s6, $zero
    /* 800FF334 00002825 */  or $a1, $zero, $zero
    /* 800FF338 0C001D78 */  jal 0x800075E0
    /* 800FF33C 00003025 */  or $a2, $zero, $zero
    /* 800FF340 0C02DD28 */  jal 0x800B74A0
    /* 800FF344 24040001 */  addiu $a0, $zero, 1
    /* 800FF348 3C0F8011 */  lui $t7, 0x8011
    /* 800FF34C 8DEF74B4 */  lw $t7, 29876($t7)
    /* 800FF350 3C01007C */  lui $at, 0x007C
    /* 800FF354 3C198011 */  lui $t9, 0x8011
    /* 800FF358 01E1C024 */  and $t8, $t7, $at
    /* 800FF35C 1300001D */  beq $t8, $zero, 0x800FF3D4
    /* 800FF360 3C148018 */  lui $s4, 0x8018
    /* 800FF364 8339064C */  lb $t9, 1612($t9)
    /* 800FF368 27B500EC */  addiu $s5, $sp, 236
    /* 800FF36C 02A02025 */  or $a0, $s5, $zero
    /* 800FF370 13200018 */  beq $t9, $zero, 0x800FF3D4
    /* 800FF374 3C058012 */  lui $a1, 0x8012
    /* 800FF378 3C148018 */  lui $s4, 0x8018
    /* 800FF37C 2694A4E0 */  addiu $s4, $s4, -23328
    /* 800FF380 3C068015 */  lui $a2, 0x8015
    /* 800FF384 8CC6698C */  lw $a2, 27020($a2)
    /* 800FF388 8E8E0004 */  lw $t6, 4($s4)
    /* 800FF38C 24A5F6C4 */  addiu $a1, $a1, -2364
    /* 800FF390 24C60001 */  addiu $a2, $a2, 1
    /* 800FF394 0C02D0D8 */  jal 0x800B4360
    /* 800FF398 8DC703A4 */  lw $a3, 932($t6)
    /* 800FF39C 02A02025 */  or $a0, $s5, $zero
    /* 800FF3A0 0C02CFE9 */  jal 0x800B3FA4
    /* 800FF3A4 2405FFFF */  addiu $a1, $zero, -1
    /* 800FF3A8 00027842 */  srl $t7, $v0, 1
    /* 800FF3AC 241800A0 */  addiu $t8, $zero, 160
    /* 800FF3B0 030F7023 */  subu $t6, $t8, $t7
    /* 800FF3B4 000E2400 */  sll $a0, $t6, 16
    /* 800FF3B8 0004C403 */  sra $t8, $a0, 16
    /* 800FF3BC 03002025 */  or $a0, $t8, $zero
    /* 800FF3C0 2405000A */  addiu $a1, $zero, 10
    /* 800FF3C4 0C02DC75 */  jal 0x800B71D4
    /* 800FF3C8 02A03025 */  or $a2, $s5, $zero
    /* 800FF3CC 10000011 */  beq $zero, $zero, 0x800FF414
    /* 800FF3D0 00000000 */  nop
    /* 800FF3D4 2694A4E0 */  addiu $s4, $s4, -23328
    /* 800FF3D8 8E8F0004 */  lw $t7, 4($s4)
    /* 800FF3DC 2405FFFF */  addiu $a1, $zero, -1
    /* 800FF3E0 0C02CFE9 */  jal 0x800B3FA4
    /* 800FF3E4 8DE400D0 */  lw $a0, 208($t7)
    /* 800FF3E8 0002C842 */  srl $t9, $v0, 1
    /* 800FF3EC 240E00A0 */  addiu $t6, $zero, 160
    /* 800FF3F0 01D97823 */  subu $t7, $t6, $t9
    /* 800FF3F4 8E990004 */  lw $t9, 4($s4)
    /* 800FF3F8 000F2400 */  sll $a0, $t7, 16
    /* 800FF3FC 00047403 */  sra $t6, $a0, 16
    /* 800FF400 01C02025 */  or $a0, $t6, $zero
    /* 800FF404 2405000A */  addiu $a1, $zero, 10
    /* 800FF408 0C02DC75 */  jal 0x800B71D4
    /* 800FF40C 8F2600D0 */  lw $a2, 208($t9)
    /* 800FF410 27B500EC */  addiu $s5, $sp, 236
    /* 800FF414 0C02CFD4 */  jal 0x800B3F50
    /* 800FF418 00000000 */  nop
    /* 800FF41C 0002B840 */  sll $s7, $v0, 1
    /* 800FF420 26F7000A */  addiu $s7, $s7, 10
    /* 800FF424 02C02025 */  or $a0, $s6, $zero
    /* 800FF428 00002825 */  or $a1, $zero, $zero
    /* 800FF42C 0C001C9C */  jal 0x80007270
    /* 800FF430 24060001 */  addiu $a2, $zero, 1
    /* 800FF434 0C02D080 */  jal 0x800B4200
    /* 800FF438 2412000A */  addiu $s2, $zero, 10
    /* 800FF43C 02C02025 */  or $a0, $s6, $zero
    /* 800FF440 00002825 */  or $a1, $zero, $zero
    /* 800FF444 0C001D78 */  jal 0x800075E0
    /* 800FF448 00003025 */  or $a2, $zero, $zero
    /* 800FF44C 3C188011 */  lui $t8, 0x8011
    /* 800FF450 8F18065C */  lw $t8, 1628($t8)
    /* 800FF454 3C168014 */  lui $s6, 0x8014
    /* 800FF458 26D66108 */  addiu $s6, $s6, 24840
    /* 800FF45C 00009025 */  or $s2, $zero, $zero
    /* 800FF460 AFB80118 */  sw $t8, 280($sp)
    /* 800FF464 8FAF0118 */  lw $t7, 280($sp)
    /* 800FF468 3C198011 */  lui $t9, 0x8011
    /* 800FF46C 8F390638 */  lw $t9, 1592($t9)
    /* 800FF470 25FE0005 */  addiu $fp, $t7, 5
    /* 800FF474 25F3FFFB */  addiu $s3, $t7, -5
    /* 800FF478 0013C400 */  sll $t8, $s3, 16
    /* 800FF47C 001E7400 */  sll $t6, $fp, 16
    /* 800FF480 000EF403 */  sra $fp, $t6, 16
    /* 800FF484 00189C03 */  sra $s3, $t8, 16
    /* 800FF488 00128880 */  sll $s1, $s2, 2
    /* 800FF48C 16590003 */  bne $s2, $t9, 0x800FF49C
    /* 800FF490 02A08025 */  or $s0, $s5, $zero
    /* 800FF494 10000002 */  beq $zero, $zero, 0x800FF4A0
    /* 800FF498 24040016 */  addiu $a0, $zero, 22
    /* 800FF49C 24040001 */  addiu $a0, $zero, 1
    /* 800FF4A0 0C02DD28 */  jal 0x800B74A0
    /* 800FF4A4 00000000 */  nop
    /* 800FF4A8 8E98000C */  lw $t8, 12($s4)
    /* 800FF4AC 8E8F0010 */  lw $t7, 16($s4)
    /* 800FF4B0 2405FFFF */  addiu $a1, $zero, -1
    /* 800FF4B4 970E004E */  lhu $t6, 78($t8)
    /* 800FF4B8 000EC880 */  sll $t9, $t6, 2
    /* 800FF4BC 01F9C021 */  addu $t8, $t7, $t9
    /* 800FF4C0 03117021 */  addu $t6, $t8, $s1
    /* 800FF4C4 0C02CFE9 */  jal 0x800B3FA4
    /* 800FF4C8 8DC40000 */  lw $a0, 0($t6)
    /* 800FF4CC 0262C823 */  subu $t9, $s3, $v0
    /* 800FF4D0 00192400 */  sll $a0, $t9, 16
    /* 800FF4D4 0004C403 */  sra $t8, $a0, 16
    /* 800FF4D8 03002025 */  or $a0, $t8, $zero
    /* 800FF4DC 00179C00 */  sll $s3, $s7, 16
    /* 800FF4E0 8E98000C */  lw $t8, 12($s4)
    /* 800FF4E4 00137403 */  sra $t6, $s3, 16
    /* 800FF4E8 01C09825 */  or $s3, $t6, $zero
    /* 800FF4EC 000E2C00 */  sll $a1, $t6, 16
    /* 800FF4F0 970E004E */  lhu $t6, 78($t8)
    /* 800FF4F4 8E990010 */  lw $t9, 16($s4)
    /* 800FF4F8 00057C03 */  sra $t7, $a1, 16
    /* 800FF4FC 01E02825 */  or $a1, $t7, $zero
    /* 800FF500 000E7880 */  sll $t7, $t6, 2
    /* 800FF504 032FC021 */  addu $t8, $t9, $t7
    /* 800FF508 03117021 */  addu $t6, $t8, $s1
    /* 800FF50C 0C02DC75 */  jal 0x800B71D4
    /* 800FF510 8DC60000 */  lw $a2, 0($t6)
    /* 800FF514 1240000C */  beq $s2, $zero, 0x800FF548
    /* 800FF518 02A02025 */  or $a0, $s5, $zero
    /* 800FF51C 24010001 */  addiu $at, $zero, 1
    /* 800FF520 1241000F */  beq $s2, $at, 0x800FF560
    /* 800FF524 02A02025 */  or $a0, $s5, $zero
    /* 800FF528 24010002 */  addiu $at, $zero, 2
    /* 800FF52C 12410012 */  beq $s2, $at, 0x800FF578
    /* 800FF530 3C028011 */  lui $v0, 0x8011
    /* 800FF534 24010003 */  addiu $at, $zero, 3
    /* 800FF538 5241002E */  .word 0x5241002E
    /* 800FF53C 82D9000F */  lb $t9, 15($s6)
    /* 800FF540 10000037 */  beq $zero, $zero, 0x800FF620
    /* 800FF544 93AF00EC */  lbu $t7, 236($sp)
    /* 800FF548 3C058012 */  lui $a1, 0x8012
    /* 800FF54C 24A5F6D8 */  addiu $a1, $a1, -2344
    /* 800FF550 0C02D0D8 */  jal 0x800B4360
    /* 800FF554 82C6000D */  lb $a2, 13($s6)
    /* 800FF558 10000031 */  beq $zero, $zero, 0x800FF620
    /* 800FF55C 93AF00EC */  lbu $t7, 236($sp)
    /* 800FF560 3C058012 */  lui $a1, 0x8012
    /* 800FF564 24A5F6DC */  addiu $a1, $a1, -2340
    /* 800FF568 0C02D0D8 */  jal 0x800B4360
    /* 800FF56C 82C6000C */  lb $a2, 12($s6)
    /* 800FF570 1000002B */  beq $zero, $zero, 0x800FF620
    /* 800FF574 93AF00EC */  lbu $t7, 236($sp)
    /* 800FF578 8C420640 */  lw $v0, 1600($v0)
    /* 800FF57C 2401000C */  addiu $at, $zero, 12
    /* 800FF580 54410008 */  .word 0x54410008
    /* 800FF584 2401000D */  addiu $at, $zero, 13
    /* 800FF588 8E990004 */  lw $t9, 4($s4)
    /* 800FF58C 02A02025 */  or $a0, $s5, $zero
    /* 800FF590 0C02F9A9 */  jal 0x800BE6A4
    /* 800FF594 8F250088 */  lw $a1, 136($t9)
    /* 800FF598 10000021 */  beq $zero, $zero, 0x800FF620
    /* 800FF59C 93AF00EC */  lbu $t7, 236($sp)
    /* 800FF5A0 2401000D */  addiu $at, $zero, 13
    /* 800FF5A4 1441000B */  bne $v0, $at, 0x800FF5D4
    /* 800FF5A8 02A02025 */  or $a0, $s5, $zero
    /* 800FF5AC 8E98000C */  lw $t8, 12($s4)
    /* 800FF5B0 8E8F0010 */  lw $t7, 16($s4)
    /* 800FF5B4 02A02025 */  or $a0, $s5, $zero
    /* 800FF5B8 970E0020 */  lhu $t6, 32($t8)
    /* 800FF5BC 000EC880 */  sll $t9, $t6, 2
    /* 800FF5C0 01F9C021 */  addu $t8, $t7, $t9
    /* 800FF5C4 0C02F9A9 */  jal 0x800BE6A4
    /* 800FF5C8 8F050000 */  lw $a1, 0($t8)
    /* 800FF5CC 10000014 */  beq $zero, $zero, 0x800FF620
    /* 800FF5D0 93AF00EC */  lbu $t7, 236($sp)
    /* 800FF5D4 00027080 */  sll $t6, $v0, 2
    /* 800FF5D8 3C058011 */  lui $a1, 0x8011
    /* 800FF5DC 00AE2821 */  addu $a1, $a1, $t6
    /* 800FF5E0 0C02F9A9 */  jal 0x800BE6A4
    /* 800FF5E4 8CA50030 */  lw $a1, 48($a1)
    /* 800FF5E8 1000000D */  beq $zero, $zero, 0x800FF620
    /* 800FF5EC 93AF00EC */  lbu $t7, 236($sp)
    /* 800FF5F0 82D9000F */  lb $t9, 15($s6)
    /* 800FF5F4 8E8F0010 */  lw $t7, 16($s4)
    /* 800FF5F8 02A02025 */  or $a0, $s5, $zero
    /* 800FF5FC 0019C080 */  sll $t8, $t9, 2
    /* 800FF600 8E99000C */  lw $t9, 12($s4)
    /* 800FF604 01F87021 */  addu $t6, $t7, $t8
    /* 800FF608 972F0050 */  lhu $t7, 80($t9)
    /* 800FF60C 000FC080 */  sll $t8, $t7, 2
    /* 800FF610 01D8C821 */  addu $t9, $t6, $t8
    /* 800FF614 0C02F9A9 */  jal 0x800BE6A4
    /* 800FF618 8F250000 */  lw $a1, 0($t9)
    /* 800FF61C 93AF00EC */  lbu $t7, 236($sp)
    /* 800FF620 001E2400 */  sll $a0, $fp, 16
    /* 800FF624 0004CC03 */  sra $t9, $a0, 16
    /* 800FF628 11E0000F */  beq $t7, $zero, 0x800FF668
    /* 800FF62C 03202025 */  or $a0, $t9, $zero
    /* 800FF630 27AE00EC */  addiu $t6, $sp, 236
    /* 800FF634 91C20000 */  lbu $v0, 0($t6)
    /* 800FF638 28410061 */  slti $at, $v0, 97
    /* 800FF63C 14200005 */  bne $at, $zero, 0x800FF654
    /* 800FF640 2841007B */  slti $at, $v0, 123
    /* 800FF644 10200003 */  beq $at, $zero, 0x800FF654
    /* 800FF648 38580020 */  xori $t8, $v0, 0x0020
    /* 800FF64C 10000002 */  beq $zero, $zero, 0x800FF658
    /* 800FF650 A2180000 */  sb $t8, 0($s0)
    /* 800FF654 A2020000 */  sb $v0, 0($s0)
    /* 800FF658 92020001 */  lbu $v0, 1($s0)
    /* 800FF65C 26100001 */  addiu $s0, $s0, 1
    /* 800FF660 5440FFF6 */  .word 0x5440FFF6
    /* 800FF664 28410061 */  slti $at, $v0, 97
    /* 800FF668 00132C00 */  sll $a1, $s3, 16
    /* 800FF66C 00057C03 */  sra $t7, $a1, 16
    /* 800FF670 01E02825 */  or $a1, $t7, $zero
    /* 800FF674 0C02DC75 */  jal 0x800B71D4
    /* 800FF678 02A03025 */  or $a2, $s5, $zero
    /* 800FF67C 0C02CFD4 */  jal 0x800B3F50
    /* 800FF680 00000000 */  nop
    /* 800FF684 26520001 */  addiu $s2, $s2, 1
    /* 800FF688 24010004 */  addiu $at, $zero, 4
    /* 800FF68C 1641FF75 */  bne $s2, $at, 0x800FF464
    /* 800FF690 02E2B821 */  addu $s7, $s7, $v0
    /* 800FF694 3C01BF80 */  lui $at, 0xBF80
    /* 800FF698 44816000 */  .word 0x44816000
    /* 800FF69C 0C03B59B */  jal 0x800ED66C
    /* 800FF6A0 00000000 */  nop
    /* 800FF6A4 3C0E8011 */  lui $t6, 0x8011
    /* 800FF6A8 8DCE46BC */  lw $t6, 18108($t6)
    /* 800FF6AC 3C188011 */  lui $t8, 0x8011
    /* 800FF6B0 8F1846C0 */  lw $t8, 18112($t8)
    /* 800FF6B4 3C018012 */  lui $at, 0x8012
    /* 800FF6B8 AC2E8E28 */  sw $t6, -29144($at)
    /* 800FF6BC 3C018012 */  lui $at, 0x8012
    /* 800FF6C0 AC388E2C */  sw $t8, -29140($at)
    /* 800FF6C4 3C01BF80 */  lui $at, 0xBF80
    /* 800FF6C8 44816000 */  .word 0x44816000
    /* 800FF6CC AFAE0080 */  sw $t6, 128($sp)
    /* 800FF6D0 0C02D96D */  jal 0x800B65B4
    /* 800FF6D4 AFB80084 */  sw $t8, 132($sp)
    /* 800FF6D8 24020001 */  addiu $v0, $zero, 1
    /* 800FF6DC 8FBF006C */  lw $ra, 108($sp)
    /* 800FF6E0 D7B40018 */  .word 0xD7B40018
    /* 800FF6E4 D7B60020 */  .word 0xD7B60020
    /* 800FF6E8 D7B80028 */  .word 0xD7B80028
    /* 800FF6EC D7BA0030 */  .word 0xD7BA0030
    /* 800FF6F0 D7BC0038 */  .word 0xD7BC0038
    /* 800FF6F4 D7BE0040 */  .word 0xD7BE0040
    /* 800FF6F8 8FB00048 */  lw $s0, 72($sp)
    /* 800FF6FC 8FB1004C */  lw $s1, 76($sp)
    /* 800FF700 8FB20050 */  lw $s2, 80($sp)
    /* 800FF704 8FB30054 */  lw $s3, 84($sp)
    /* 800FF708 8FB40058 */  lw $s4, 88($sp)
    /* 800FF70C 8FB5005C */  lw $s5, 92($sp)
    /* 800FF710 8FB60060 */  lw $s6, 96($sp)
    /* 800FF714 8FB70064 */  lw $s7, 100($sp)
    /* 800FF718 8FBE0068 */  lw $fp, 104($sp)
    /* 800FF71C 03E00008 */  jr $ra
    /* 800FF720 27BD0120 */  addiu $sp, $sp, 288
