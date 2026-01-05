# Source: game_code.bin (decompressed)
# Address: 0x800D8C58

glabel drone_throttle_calc
    /* 800D8C58 27BDFF90 */  addiu $sp, $sp, -112
    /* 800D8C5C 83AF0073 */  lb $t7, 115($sp)
    /* 800D8C60 3C0E8011 */  lui $t6, 0x8011
    /* 800D8C64 8DCE40EC */  lw $t6, 16620($t6)
    /* 800D8C68 000FC100 */  sll $t8, $t7, 4
    /* 800D8C6C 3C198011 */  lui $t9, 0x8011
    /* 800D8C70 030FC021 */  addu $t8, $t8, $t7
    /* 800D8C74 0018C180 */  sll $t8, $t8, 6
    /* 800D8C78 27391998 */  addiu $t9, $t9, 6552
    /* 800D8C7C 3C158011 */  lui $s5, 0x8011
    /* 800D8C80 AFBF001C */  sw $ra, 28($sp)
    /* 800D8C84 26B5196C */  addiu $s5, $s5, 6508
    /* 800D8C88 03198021 */  addu $s0, $t8, $t9
    /* 800D8C8C 00009025 */  or $s2, $zero, $zero
    /* 800D8C90 241E000A */  addiu $fp, $zero, 10
    /* 800D8C94 24170009 */  addiu $s7, $zero, 9
    /* 800D8C98 24160002 */  addiu $s6, $zero, 2
    /* 800D8C9C 2414FFFF */  addiu $s4, $zero, -1
    /* 800D8CA0 AFAE006C */  sw $t6, 108($sp)
    /* 800D8CA4 8E0E0004 */  lw $t6, 4($s0)
    /* 800D8CA8 3C048011 */  lui $a0, 0x8011
    /* 800D8CAC 2484418C */  addiu $a0, $a0, 16780
    /* 800D8CB0 168E0037 */  bne $s4, $t6, 0x800D8D90
    /* 800D8CB4 2611000C */  addiu $s1, $s0, 12
    /* 800D8CB8 83AF0073 */  lb $t7, 115($sp)
    /* 800D8CBC 24180001 */  addiu $t8, $zero, 1
    /* 800D8CC0 02202825 */  or $a1, $s1, $zero
    /* 800D8CC4 0C0235AC */  jal 0x8008D6B0
    /* 800D8CC8 01F89804 */  .word 0x01F89804
    /* 800D8CCC 3C0F8014 */  lui $t7, 0x8014
    /* 800D8CD0 8E190000 */  lw $t9, 0($s0)
    /* 800D8CD4 25EF0BDC */  addiu $t7, $t7, 3036
    /* 800D8CD8 91E60000 */  lbu $a2, 0($t7)
    /* 800D8CDC 00197080 */  sll $t6, $t9, 2
    /* 800D8CE0 02AEC021 */  addu $t8, $s5, $t6
    /* 800D8CE4 24C6FFFF */  addiu $a2, $a2, -1
    /* 800D8CE8 0006CE00 */  sll $t9, $a2, 24
    /* 800D8CEC 00193603 */  sra $a2, $t9, 24
    /* 800D8CF0 8F040000 */  lw $a0, 0($t8)
    /* 800D8CF4 00002825 */  or $a1, $zero, $zero
    /* 800D8CF8 0C024B8B */  jal 0x80092E2C
    /* 800D8CFC 24070001 */  addiu $a3, $zero, 1
    /* 800D8D00 8E030000 */  lw $v1, 0($s0)
    /* 800D8D04 00402025 */  or $a0, $v0, $zero
    /* 800D8D08 02202825 */  or $a1, $s1, $zero
    /* 800D8D0C 1060000A */  beq $v1, $zero, 0x800D8D38
    /* 800D8D10 00143400 */  sll $a2, $s4, 16
    /* 800D8D14 24010001 */  addiu $at, $zero, 1
    /* 800D8D18 50610008 */  .word 0x50610008
    /* 800D8D1C 3C070004 */  lui $a3, 0x0004
    /* 800D8D20 52C30006 */  .word 0x52C30006
    /* 800D8D24 3C070004 */  lui $a3, 0x0004
    /* 800D8D28 52E30004 */  .word 0x52E30004
    /* 800D8D2C 3C070004 */  lui $a3, 0x0004
    /* 800D8D30 17C30004 */  bne $fp, $v1, 0x800D8D44
    /* 800D8D34 00003825 */  or $a3, $zero, $zero
    /* 800D8D38 3C070004 */  lui $a3, 0x0004
    /* 800D8D3C 10000001 */  beq $zero, $zero, 0x800D8D44
    /* 800D8D40 34E72000 */  ori $a3, $a3, 0x2000
    /* 800D8D44 0006C403 */  sra $t8, $a2, 16
    /* 800D8D48 0C02389B */  jal 0x8008E26C
    /* 800D8D4C 03003025 */  or $a2, $t8, $zero
    /* 800D8D50 0002C900 */  sll $t9, $v0, 4
    /* 800D8D54 0322C821 */  addu $t9, $t9, $v0
    /* 800D8D58 8FAF006C */  lw $t7, 108($sp)
    /* 800D8D5C 0019C880 */  sll $t9, $t9, 2
    /* 800D8D60 3C018013 */  lui $at, 0x8013
    /* 800D8D64 AE020004 */  sw $v0, 4($s0)
    /* 800D8D68 00390821 */  addu $at, $at, $t9
    /* 800D8D6C 00402025 */  or $a0, $v0, $zero
    /* 800D8D70 24050001 */  addiu $a1, $zero, 1
    /* 800D8D74 2406000F */  addiu $a2, $zero, 15
    /* 800D8D78 0C022BA3 */  jal 0x8008AE8C
    /* 800D8D7C AC2FE73C */  sw $t7, -6340($at)
    /* 800D8D80 8E040004 */  lw $a0, 4($s0)
    /* 800D8D84 00002825 */  or $a1, $zero, $zero
    /* 800D8D88 0C022C36 */  jal 0x8008B0D8
    /* 800D8D8C 02603025 */  or $a2, $s3, $zero
    /* 800D8D90 26520040 */  addiu $s2, $s2, 64
    /* 800D8D94 24010440 */  addiu $at, $zero, 1088
    /* 800D8D98 1641FFC2 */  bne $s2, $at, 0x800D8CA4
    /* 800D8D9C 26100040 */  addiu $s0, $s0, 64
    /* 800D8DA0 3C0E8014 */  lui $t6, 0x8014
    /* 800D8DA4 25CE0BDC */  addiu $t6, $t6, 3036
    /* 800D8DA8 91C70000 */  lbu $a3, 0($t6)
    /* 800D8DAC 27B20066 */  addiu $s2, $sp, 102
    /* 800D8DB0 3C048012 */  lui $a0, 0x8012
    /* 800D8DB4 24E7FFFF */  addiu $a3, $a3, -1
    /* 800D8DB8 0007C600 */  sll $t8, $a3, 24
    /* 800D8DBC 24190001 */  addiu $t9, $zero, 1
    /* 800D8DC0 AFB90010 */  sw $t9, 16($sp)
    /* 800D8DC4 00183E03 */  sra $a3, $t8, 24
    /* 800D8DC8 24840254 */  addiu $a0, $a0, 596
    /* 800D8DCC 02402825 */  or $a1, $s2, $zero
    /* 800D8DD0 0C02C93B */  jal 0x800B24EC
    /* 800D8DD4 00003025 */  or $a2, $zero, $zero
    /* 800D8DD8 83AE0073 */  lb $t6, 115($sp)
    /* 800D8DDC 3C0F8011 */  lui $t7, 0x8011
    /* 800D8DE0 25EF1998 */  addiu $t7, $t7, 6552
    /* 800D8DE4 000EC100 */  sll $t8, $t6, 4
    /* 800D8DE8 030EC021 */  addu $t8, $t8, $t6
    /* 800D8DEC 0018C180 */  sll $t8, $t8, 6
    /* 800D8DF0 030F8021 */  addu $s0, $t8, $t7
    /* 800D8DF4 86040346 */  lh $a0, 838($s0)
    /* 800D8DF8 00402825 */  or $a1, $v0, $zero
    /* 800D8DFC 0C02361C */  jal 0x8008D870
    /* 800D8E00 2406FFFF */  addiu $a2, $zero, -1
    /* 800D8E04 3C048011 */  lui $a0, 0x8011
    /* 800D8E08 2611034C */  addiu $s1, $s0, 844
    /* 800D8E0C 02202825 */  or $a1, $s1, $zero
    /* 800D8E10 0C0235AC */  jal 0x8008D6B0
    /* 800D8E14 2484418C */  addiu $a0, $a0, 16780
    /* 800D8E18 3C018012 */  lui $at, 0x8012
    /* 800D8E1C C43841FC */  lwc1 $f24, 16892($at)
    /* 800D8E20 02202825 */  or $a1, $s1, $zero
    /* 800D8E24 0C02D626 */  jal 0x800B5898
    /* 800D8E28 4600C306 */  .word 0x4600C306
    /* 800D8E2C 3C138015 */  lui $s3, 0x8015
    /* 800D8E30 26731AD0 */  addiu $s3, $s3, 6864
    /* 800D8E34 86790000 */  lh $t9, 0($s3)
    /* 800D8E38 44802000 */  .word 0x44802000
    /* 800D8E3C 2B210002 */  slti $at, $t9, 2
    /* 800D8E40 14200005 */  bne $at, $zero, 0x800D8E58
    /* 800D8E44 E6040370 */  swc1 $f4, 880($s0)
    /* 800D8E48 3C0142D2 */  lui $at, 0x42D2
    /* 800D8E4C 44813000 */  .word 0x44813000
    /* 800D8E50 10000005 */  beq $zero, $zero, 0x800D8E68
    /* 800D8E54 E6060374 */  swc1 $f6, 884($s0)
    /* 800D8E58 3C0142E6 */  lui $at, 0x42E6
    /* 800D8E5C 44814000 */  .word 0x44814000
    /* 800D8E60 00000000 */  nop
    /* 800D8E64 E6080374 */  swc1 $f8, 884($s0)
    /* 800D8E68 3C013F80 */  lui $at, 0x3F80
    /* 800D8E6C 4481A000 */  .word 0x4481A000
    /* 800D8E70 C60A0370 */  lwc1 $f10, 880($s0)
    /* 800D8E74 C6060374 */  lwc1 $f6, 884($s0)
    /* 800D8E78 3C014348 */  lui $at, 0x4348
    /* 800D8E7C 46145102 */  .word 0x46145102
    /* 800D8E80 44815000 */  .word 0x44815000
    /* 800D8E84 3C0E8014 */  lui $t6, 0x8014
    /* 800D8E88 46143202 */  .word 0x46143202
    /* 800D8E8C 25CE0BDC */  addiu $t6, $t6, 3036
    /* 800D8E90 3C048012 */  lui $a0, 0x8012
    /* 800D8E94 24190001 */  addiu $t9, $zero, 1
    /* 800D8E98 E6040370 */  swc1 $f4, 880($s0)
    /* 800D8E9C 46145102 */  .word 0x46145102
    /* 800D8EA0 24840264 */  addiu $a0, $a0, 612
    /* 800D8EA4 E6080374 */  swc1 $f8, 884($s0)
    /* 800D8EA8 02402825 */  or $a1, $s2, $zero
    /* 800D8EAC 00003025 */  or $a2, $zero, $zero
    /* 800D8EB0 E6040378 */  swc1 $f4, 888($s0)
    /* 800D8EB4 91C70000 */  lbu $a3, 0($t6)
    /* 800D8EB8 AFB90010 */  sw $t9, 16($sp)
    /* 800D8EBC 24E7FFFF */  addiu $a3, $a3, -1
    /* 800D8EC0 0007C600 */  sll $t8, $a3, 24
    /* 800D8EC4 0C02C93B */  jal 0x800B24EC
    /* 800D8EC8 00183E03 */  sra $a3, $t8, 24
    /* 800D8ECC 86040386 */  lh $a0, 902($s0)
    /* 800D8ED0 00402825 */  or $a1, $v0, $zero
    /* 800D8ED4 0C02361C */  jal 0x8008D870
    /* 800D8ED8 2406FFFF */  addiu $a2, $zero, -1
    /* 800D8EDC 3C0E8014 */  lui $t6, 0x8014
    /* 800D8EE0 25CE0BDC */  addiu $t6, $t6, 3036
    /* 800D8EE4 91C70000 */  lbu $a3, 0($t6)
    /* 800D8EE8 3C048012 */  lui $a0, 0x8012
    /* 800D8EEC 24190001 */  addiu $t9, $zero, 1
    /* 800D8EF0 24E7FFFF */  addiu $a3, $a3, -1
    /* 800D8EF4 0007C600 */  sll $t8, $a3, 24
    /* 800D8EF8 00183E03 */  sra $a3, $t8, 24
    /* 800D8EFC AFB90010 */  sw $t9, 16($sp)
    /* 800D8F00 2484026C */  addiu $a0, $a0, 620
    /* 800D8F04 02402825 */  or $a1, $s2, $zero
    /* 800D8F08 0C02C93B */  jal 0x800B24EC
    /* 800D8F0C 00003025 */  or $a2, $zero, $zero
    /* 800D8F10 860403C6 */  lh $a0, 966($s0)
    /* 800D8F14 00402825 */  or $a1, $v0, $zero
    /* 800D8F18 0C02361C */  jal 0x8008D870
    /* 800D8F1C 2406FFFF */  addiu $a2, $zero, -1
    /* 800D8F20 3C048011 */  lui $a0, 0x8011
    /* 800D8F24 261103CC */  addiu $s1, $s0, 972
    /* 800D8F28 02202825 */  or $a1, $s1, $zero
    /* 800D8F2C 0C0235AC */  jal 0x8008D6B0
    /* 800D8F30 2484418C */  addiu $a0, $a0, 16780
    /* 800D8F34 4600C306 */  .word 0x4600C306
    /* 800D8F38 0C02D626 */  jal 0x800B5898
    /* 800D8F3C 02202825 */  or $a1, $s1, $zero
    /* 800D8F40 866E0000 */  lh $t6, 0($s3)
    /* 800D8F44 3C018012 */  lui $at, 0x8012
    /* 800D8F48 C4264200 */  lwc1 $f6, 16896($at)
    /* 800D8F4C 29C10002 */  slti $at, $t6, 2
    /* 800D8F50 14200005 */  bne $at, $zero, 0x800D8F68
    /* 800D8F54 E60603F0 */  swc1 $f6, 1008($s0)
    /* 800D8F58 3C018012 */  lui $at, 0x8012
    /* 800D8F5C C4284204 */  lwc1 $f8, 16900($at)
    /* 800D8F60 10000004 */  beq $zero, $zero, 0x800D8F74
    /* 800D8F64 E60803F4 */  swc1 $f8, 1012($s0)
    /* 800D8F68 3C018012 */  lui $at, 0x8012
    /* 800D8F6C C42A4208 */  lwc1 $f10, 16904($at)
    /* 800D8F70 E60A03F4 */  swc1 $f10, 1012($s0)
    /* 800D8F74 C60403F0 */  lwc1 $f4, 1008($s0)
    /* 800D8F78 C60803F4 */  lwc1 $f8, 1012($s0)
    /* 800D8F7C 3C0142C8 */  lui $at, 0x42C8
    /* 800D8F80 46142182 */  .word 0x46142182
    /* 800D8F84 44812000 */  .word 0x44812000
    /* 800D8F88 02202025 */  or $a0, $s1, $zero
    /* 800D8F8C 46144282 */  .word 0x46144282
    /* 800D8F90 02202825 */  or $a1, $s1, $zero
    /* 800D8F94 3C063F00 */  lui $a2, 0x3F00
    /* 800D8F98 46142582 */  .word 0x46142582
    /* 800D8F9C E60603F0 */  swc1 $f6, 1008($s0)
    /* 800D8FA0 E60A03F4 */  swc1 $f10, 1012($s0)
    /* 800D8FA4 0C022CCB */  jal 0x8008B32C
    /* 800D8FA8 E61603F8 */  swc1 $f22, 1016($s0)
    /* 800D8FAC 3C048011 */  lui $a0, 0x8011
    /* 800D8FB0 2611040C */  addiu $s1, $s0, 1036
    /* 800D8FB4 02202825 */  or $a1, $s1, $zero
    /* 800D8FB8 0C0235AC */  jal 0x8008D6B0
    /* 800D8FBC 2484418C */  addiu $a0, $a0, 16780
    /* 800D8FC0 4600C306 */  .word 0x4600C306
    /* 800D8FC4 0C02D626 */  jal 0x800B5898
    /* 800D8FC8 02202825 */  or $a1, $s1, $zero
    /* 800D8FCC 86780000 */  lh $t8, 0($s3)
    /* 800D8FD0 3C018012 */  lui $at, 0x8012
    /* 800D8FD4 C426420C */  lwc1 $f6, 16908($at)
    /* 800D8FD8 2B010002 */  slti $at, $t8, 2
    /* 800D8FDC 14200005 */  bne $at, $zero, 0x800D8FF4
    /* 800D8FE0 E6060430 */  swc1 $f6, 1072($s0)
    /* 800D8FE4 3C018012 */  lui $at, 0x8012
    /* 800D8FE8 C4284210 */  lwc1 $f8, 16912($at)
    /* 800D8FEC 10000004 */  beq $zero, $zero, 0x800D9000
    /* 800D8FF0 E6080434 */  swc1 $f8, 1076($s0)
    /* 800D8FF4 3C018012 */  lui $at, 0x8012
    /* 800D8FF8 C42A4214 */  lwc1 $f10, 16916($at)
    /* 800D8FFC E60A0434 */  swc1 $f10, 1076($s0)
    /* 800D9000 C6040430 */  lwc1 $f4, 1072($s0)
    /* 800D9004 C6080434 */  lwc1 $f8, 1076($s0)
    /* 800D9008 83A20073 */  lb $v0, 115($sp)
    /* 800D900C 46142182 */  .word 0x46142182
    /* 800D9010 3C018015 */  lui $at, 0x8015
    /* 800D9014 00027880 */  sll $t7, $v0, 2
    /* 800D9018 46144282 */  .word 0x46144282
    /* 800D901C E6160438 */  swc1 $f22, 1080($s0)
    /* 800D9020 002F0821 */  addu $at, $at, $t7
    /* 800D9024 24190001 */  addiu $t9, $zero, 1
    /* 800D9028 E6060430 */  swc1 $f6, 1072($s0)
    /* 800D902C 240E0001 */  addiu $t6, $zero, 1
    /* 800D9030 E60A0434 */  swc1 $f10, 1076($s0)
    /* 800D9034 AC394618 */  sw $t9, 17944($at)
    /* 800D9038 3C018015 */  lui $at, 0x8015
    /* 800D903C 002F0821 */  addu $at, $at, $t7
    /* 800D9040 0C02E0B2 */  jal 0x800B82C8
    /* 800D9044 AC2E4630 */  sw $t6, 17968($at)
    /* 800D9048 8FBF001C */  lw $ra, 28($sp)
    /* 800D904C 27BD0070 */  addiu $sp, $sp, 112
    /* 800D9050 03E00008 */  jr $ra
    /* 800D9054 00000000 */  nop
