# Source: game_code.bin (decompressed)
# Address: 0x800B8820

glabel particle_collision
    /* 800B8820 27BDFF78 */  addiu $sp, $sp, -136
    /* 800B8824 AFBF001C */  sw $ra, 28($sp)
    /* 800B8828 240E0001 */  addiu $t6, $zero, 1
    /* 800B882C 3C018011 */  lui $at, 0x8011
    /* 800B8830 0C02E194 */  jal 0x800B8650
    /* 800B8834 A02E7428 */  sb $t6, 29736($at)
    /* 800B8838 3C0F8011 */  lui $t7, 0x8011
    /* 800B883C 8DEF74B4 */  lw $t7, 29876($t7)
    /* 800B8840 3C017C03 */  lui $at, 0x7C03
    /* 800B8844 3421FFFE */  ori $at, $at, 0xFFFE
    /* 800B8848 01E1C024 */  and $t8, $t7, $at
    /* 800B884C 130000C0 */  beq $t8, $zero, 0x800B8B50
    /* 800B8850 00002025 */  or $a0, $zero, $zero
    /* 800B8854 0C02E0DD */  jal 0x800B8374
    /* 800B8858 00000000 */  nop
    /* 800B885C 3C198012 */  lui $t9, 0x8012
    /* 800B8860 8F39AD88 */  lw $t9, -21112($t9)
    /* 800B8864 3C108012 */  lui $s0, 0x8012
    /* 800B8868 3C1E8012 */  lui $fp, 0x8012
    /* 800B886C 3C168013 */  lui $s6, 0x8013
    /* 800B8870 3C150004 */  lui $s5, 0x0004
    /* 800B8874 3C138012 */  lui $s3, 0x8012
    /* 800B8878 2673A970 */  addiu $s3, $s3, -22160
    /* 800B887C 36B52000 */  ori $s5, $s5, 0x2000
    /* 800B8880 26D6E700 */  addiu $s6, $s6, -6400
    /* 800B8884 27DEAC94 */  addiu $fp, $fp, -21356
    /* 800B8888 2610A994 */  addiu $s0, $s0, -22124
    /* 800B888C 24170044 */  addiu $s7, $zero, 68
    /* 800B8890 24140002 */  addiu $s4, $zero, 2
    /* 800B8894 2412FFFF */  addiu $s2, $zero, -1
    /* 800B8898 AFB90074 */  sw $t9, 116($sp)
    /* 800B889C 8E0E0004 */  lw $t6, 4($s0)
    /* 800B88A0 2611000C */  addiu $s1, $s0, 12
    /* 800B88A4 02202025 */  or $a0, $s1, $zero
    /* 800B88A8 164E0026 */  bne $s2, $t6, 0x800B8944
    /* 800B88AC 3C058011 */  lui $a1, 0x8011
    /* 800B88B0 24A5418C */  addiu $a1, $a1, 16780
    /* 800B88B4 0C001F1A */  jal 0x80007C68
    /* 800B88B8 24060024 */  addiu $a2, $zero, 36
    /* 800B88BC 3C0E8014 */  lui $t6, 0x8014
    /* 800B88C0 8E0F0000 */  lw $t7, 0($s0)
    /* 800B88C4 25CE0BDC */  addiu $t6, $t6, 3036
    /* 800B88C8 91C60000 */  lbu $a2, 0($t6)
    /* 800B88CC 000FC080 */  sll $t8, $t7, 2
    /* 800B88D0 0278C821 */  addu $t9, $s3, $t8
    /* 800B88D4 24C6FFFF */  addiu $a2, $a2, -1
    /* 800B88D8 00067E00 */  sll $t7, $a2, 24
    /* 800B88DC 000F3603 */  sra $a2, $t7, 24
    /* 800B88E0 8F240000 */  lw $a0, 0($t9)
    /* 800B88E4 00002825 */  or $a1, $zero, $zero
    /* 800B88E8 0C024B8B */  jal 0x80092E2C
    /* 800B88EC 24070001 */  addiu $a3, $zero, 1
    /* 800B88F0 8E030000 */  lw $v1, 0($s0)
    /* 800B88F4 00402025 */  or $a0, $v0, $zero
    /* 800B88F8 02202825 */  or $a1, $s1, $zero
    /* 800B88FC 10600006 */  beq $v1, $zero, 0x800B8918
    /* 800B8900 00123400 */  sll $a2, $s2, 16
    /* 800B8904 24010001 */  addiu $at, $zero, 1
    /* 800B8908 10610003 */  beq $v1, $at, 0x800B8918
    /* 800B890C 00000000 */  nop
    /* 800B8910 16830003 */  bne $s4, $v1, 0x800B8920
    /* 800B8914 00003825 */  or $a3, $zero, $zero
    /* 800B8918 10000001 */  beq $zero, $zero, 0x800B8920
    /* 800B891C 02A03825 */  or $a3, $s5, $zero
    /* 800B8920 0006CC03 */  sra $t9, $a2, 16
    /* 800B8924 0C02389B */  jal 0x8008E26C
    /* 800B8928 03203025 */  or $a2, $t9, $zero
    /* 800B892C 00570019 */  multu $v0, $s7
    /* 800B8930 8FAE0074 */  lw $t6, 116($sp)
    /* 800B8934 AE020004 */  sw $v0, 4($s0)
    /* 800B8938 00007812 */  mflo $t7
    /* 800B893C 02CFC021 */  addu $t8, $s6, $t7
    /* 800B8940 AF0E003C */  sw $t6, 60($t8)
    /* 800B8944 26100040 */  addiu $s0, $s0, 64
    /* 800B8948 561EFFD5 */  .word 0x561EFFD5
    /* 800B894C 8E0E0004 */  lw $t6, 4($s0)
    /* 800B8950 3C198014 */  lui $t9, 0x8014
    /* 800B8954 27390BDC */  addiu $t9, $t9, 3036
    /* 800B8958 93270000 */  lbu $a3, 0($t9)
    /* 800B895C 27B2006E */  addiu $s2, $sp, 110
    /* 800B8960 3C048012 */  lui $a0, 0x8012
    /* 800B8964 24E7FFFF */  addiu $a3, $a3, -1
    /* 800B8968 00077E00 */  sll $t7, $a3, 24
    /* 800B896C 24180001 */  addiu $t8, $zero, 1
    /* 800B8970 AFB80010 */  sw $t8, 16($sp)
    /* 800B8974 000F3E03 */  sra $a3, $t7, 24
    /* 800B8978 2484268C */  addiu $a0, $a0, 9868
    /* 800B897C 02402825 */  or $a1, $s2, $zero
    /* 800B8980 0C02C93B */  jal 0x800B24EC
    /* 800B8984 00003025 */  or $a2, $zero, $zero
    /* 800B8988 3C108012 */  lui $s0, 0x8012
    /* 800B898C 2610A994 */  addiu $s0, $s0, -22124
    /* 800B8990 86040206 */  lh $a0, 518($s0)
    /* 800B8994 00402825 */  or $a1, $v0, $zero
    /* 800B8998 0C02361C */  jal 0x8008D870
    /* 800B899C 2406FFFF */  addiu $a2, $zero, -1
    /* 800B89A0 3C198014 */  lui $t9, 0x8014
    /* 800B89A4 27390BDC */  addiu $t9, $t9, 3036
    /* 800B89A8 93270000 */  lbu $a3, 0($t9)
    /* 800B89AC 3C048012 */  lui $a0, 0x8012
    /* 800B89B0 24180001 */  addiu $t8, $zero, 1
    /* 800B89B4 24E7FFFF */  addiu $a3, $a3, -1
    /* 800B89B8 00077E00 */  sll $t7, $a3, 24
    /* 800B89BC 000F3E03 */  sra $a3, $t7, 24
    /* 800B89C0 AFB80010 */  sw $t8, 16($sp)
    /* 800B89C4 24842694 */  addiu $a0, $a0, 9876
    /* 800B89C8 02402825 */  or $a1, $s2, $zero
    /* 800B89CC 0C02C93B */  jal 0x800B24EC
    /* 800B89D0 00003025 */  or $a2, $zero, $zero
    /* 800B89D4 86040246 */  lh $a0, 582($s0)
    /* 800B89D8 00402825 */  or $a1, $v0, $zero
    /* 800B89DC 0C02361C */  jal 0x8008D870
    /* 800B89E0 2406FFFF */  addiu $a2, $zero, -1
    /* 800B89E4 3C118012 */  lui $s1, 0x8012
    /* 800B89E8 2631ABE0 */  addiu $s1, $s1, -21536
    /* 800B89EC 3C058011 */  lui $a1, 0x8011
    /* 800B89F0 24A5418C */  addiu $a1, $a1, 16780
    /* 800B89F4 02202025 */  or $a0, $s1, $zero
    /* 800B89F8 0C001F1A */  jal 0x80007C68
    /* 800B89FC 24060024 */  addiu $a2, $zero, 36
    /* 800B8A00 3C018012 */  lui $at, 0x8012
    /* 800B8A04 C4343DE4 */  lwc1 $f20, 15844($at)
    /* 800B8A08 02202825 */  or $a1, $s1, $zero
    /* 800B8A0C 0C02D626 */  jal 0x800B5898
    /* 800B8A10 4600A306 */  .word 0x4600A306
    /* 800B8A14 3C018012 */  lui $at, 0x8012
    /* 800B8A18 C4363DE8 */  lwc1 $f22, 15848($at)
    /* 800B8A1C 3C0142C8 */  lui $at, 0x42C8
    /* 800B8A20 4481C000 */  .word 0x4481C000
    /* 800B8A24 3C018012 */  lui $at, 0x8012
    /* 800B8A28 C4243DEC */  lwc1 $f4, 15852($at)
    /* 800B8A2C 02202025 */  or $a0, $s1, $zero
    /* 800B8A30 02202825 */  or $a1, $s1, $zero
    /* 800B8A34 3C063F00 */  lui $a2, 0x3F00
    /* 800B8A38 E6160274 */  swc1 $f22, 628($s0)
    /* 800B8A3C E6180278 */  swc1 $f24, 632($s0)
    /* 800B8A40 0C022CCB */  jal 0x8008B32C
    /* 800B8A44 E6040270 */  swc1 $f4, 624($s0)
    /* 800B8A48 3C118012 */  lui $s1, 0x8012
    /* 800B8A4C 2631AC20 */  addiu $s1, $s1, -21472
    /* 800B8A50 3C058011 */  lui $a1, 0x8011
    /* 800B8A54 24A5418C */  addiu $a1, $a1, 16780
    /* 800B8A58 02202025 */  or $a0, $s1, $zero
    /* 800B8A5C 0C001F1A */  jal 0x80007C68
    /* 800B8A60 24060024 */  addiu $a2, $zero, 36
    /* 800B8A64 4600A306 */  .word 0x4600A306
    /* 800B8A68 0C02D626 */  jal 0x800B5898
    /* 800B8A6C 02202825 */  or $a1, $s1, $zero
    /* 800B8A70 3C018012 */  lui $at, 0x8012
    /* 800B8A74 C4263DF0 */  lwc1 $f6, 15856($at)
    /* 800B8A78 3C198014 */  lui $t9, 0x8014
    /* 800B8A7C E61602B4 */  swc1 $f22, 692($s0)
    /* 800B8A80 E61802B8 */  swc1 $f24, 696($s0)
    /* 800B8A84 27390BDC */  addiu $t9, $t9, 3036
    /* 800B8A88 E60602B0 */  swc1 $f6, 688($s0)
    /* 800B8A8C 93270000 */  lbu $a3, 0($t9)
    /* 800B8A90 3C048012 */  lui $a0, 0x8012
    /* 800B8A94 24180001 */  addiu $t8, $zero, 1
    /* 800B8A98 24E7FFFF */  addiu $a3, $a3, -1
    /* 800B8A9C 00077E00 */  sll $t7, $a3, 24
    /* 800B8AA0 000F3E03 */  sra $a3, $t7, 24
    /* 800B8AA4 AFB80010 */  sw $t8, 16($sp)
    /* 800B8AA8 24842698 */  addiu $a0, $a0, 9880
    /* 800B8AAC 02402825 */  or $a1, $s2, $zero
    /* 800B8AB0 0C02C93B */  jal 0x800B24EC
    /* 800B8AB4 00003025 */  or $a2, $zero, $zero
    /* 800B8AB8 860402C6 */  lh $a0, 710($s0)
    /* 800B8ABC 00402825 */  or $a1, $v0, $zero
    /* 800B8AC0 0C02361C */  jal 0x8008D870
    /* 800B8AC4 2406FFFF */  addiu $a2, $zero, -1
    /* 800B8AC8 3C118012 */  lui $s1, 0x8012
    /* 800B8ACC 2631AC60 */  addiu $s1, $s1, -21408
    /* 800B8AD0 3C058011 */  lui $a1, 0x8011
    /* 800B8AD4 24A5418C */  addiu $a1, $a1, 16780
    /* 800B8AD8 02202025 */  or $a0, $s1, $zero
    /* 800B8ADC 0C001F1A */  jal 0x80007C68
    /* 800B8AE0 24060024 */  addiu $a2, $zero, 36
    /* 800B8AE4 4600A306 */  .word 0x4600A306
    /* 800B8AE8 0C02D626 */  jal 0x800B5898
    /* 800B8AEC 02202825 */  or $a1, $s1, $zero
    /* 800B8AF0 3C0142E6 */  lui $at, 0x42E6
    /* 800B8AF4 44815000 */  .word 0x44815000
    /* 800B8AF8 3C014348 */  lui $at, 0x4348
    /* 800B8AFC 44804000 */  .word 0x44804000
    /* 800B8B00 44812000 */  .word 0x44812000
    /* 800B8B04 3C018012 */  lui $at, 0x8012
    /* 800B8B08 24190001 */  addiu $t9, $zero, 1
    /* 800B8B0C E60A02F4 */  swc1 $f10, 756($s0)
    /* 800B8B10 E60802F0 */  swc1 $f8, 752($s0)
    /* 800B8B14 E60402F8 */  swc1 $f4, 760($s0)
    /* 800B8B18 AC39AC94 */  sw $t9, -21356($at)
    /* 800B8B1C 3C018012 */  lui $at, 0x8012
    /* 800B8B20 240F0001 */  addiu $t7, $zero, 1
    /* 800B8B24 0C02E0B2 */  jal 0x800B82C8
    /* 800B8B28 AC2FAC98 */  sw $t7, -21352($at)
    /* 800B8B2C 3C068012 */  lui $a2, 0x8012
    /* 800B8B30 24C6ACA0 */  addiu $a2, $a2, -21344
    /* 800B8B34 00002025 */  or $a0, $zero, $zero
    /* 800B8B38 00002825 */  or $a1, $zero, $zero
    /* 800B8B3C 0C02CDFA */  jal 0x800B37E8
    /* 800B8B40 24070002 */  addiu $a3, $zero, 2
    /* 800B8B44 3C018012 */  lui $at, 0x8012
    /* 800B8B48 10000008 */  beq $zero, $zero, 0x800B8B6C
    /* 800B8B4C AC22AD34 */  sw $v0, -21196($at)
    /* 800B8B50 3C068012 */  lui $a2, 0x8012
    /* 800B8B54 24C6ACE8 */  addiu $a2, $a2, -21272
    /* 800B8B58 00002825 */  or $a1, $zero, $zero
    /* 800B8B5C 0C02CDFA */  jal 0x800B37E8
    /* 800B8B60 24070002 */  addiu $a3, $zero, 2
    /* 800B8B64 3C018012 */  lui $at, 0x8012
    /* 800B8B68 AC22AD34 */  sw $v0, -21196($at)
    /* 800B8B6C 3C028014 */  lui $v0, 0x8014
    /* 800B8B70 24426108 */  addiu $v0, $v0, 24840
    /* 800B8B74 804E0014 */  lb $t6, 20($v0)
    /* 800B8B78 3C018012 */  lui $at, 0x8012
    /* 800B8B7C AC2EAD4C */  sw $t6, -21172($at)
    /* 800B8B80 80580013 */  lb $t8, 19($v0)
    /* 800B8B84 3C018012 */  lui $at, 0x8012
    /* 800B8B88 0C02D84E */  jal 0x800B6138
    /* 800B8B8C AC38AD50 */  sw $t8, -21168($at)
    /* 800B8B90 3C068012 */  lui $a2, 0x8012
    /* 800B8B94 24C6AD44 */  addiu $a2, $a2, -21180
    /* 800B8B98 8CC50000 */  lw $a1, 0($a2)
    /* 800B8B9C 3C078012 */  lui $a3, 0x8012
    /* 800B8BA0 24E7AD40 */  addiu $a3, $a3, -21184
    /* 800B8BA4 00E5C821 */  addu $t9, $a3, $a1
    /* 800B8BA8 83240000 */  lb $a0, 0($t9)
    /* 800B8BAC 24180001 */  addiu $t8, $zero, 1
    /* 800B8BB0 1480000B */  bne $a0, $zero, 0x800B8BE0
    /* 800B8BB4 24AF0001 */  addiu $t7, $a1, 1
    /* 800B8BB8 29E10004 */  slti $at, $t7, 4
    /* 800B8BBC ACCF0000 */  sw $t7, 0($a2)
    /* 800B8BC0 14200003 */  bne $at, $zero, 0x800B8BD0
    /* 800B8BC4 01E02825 */  or $a1, $t7, $zero
    /* 800B8BC8 10000005 */  beq $zero, $zero, 0x800B8BE0
    /* 800B8BCC ACC00000 */  sw $zero, 0($a2)
    /* 800B8BD0 00E57021 */  addu $t6, $a3, $a1
    /* 800B8BD4 81C40000 */  lb $a0, 0($t6)
    /* 800B8BD8 5080FFF7 */  .word 0x5080FFF7
    /* 800B8BDC 24AF0001 */  addiu $t7, $a1, 1
    /* 800B8BE0 3C018012 */  lui $at, 0x8012
    /* 800B8BE4 A020AD54 */  sb $zero, -21164($at)
    /* 800B8BE8 3C018012 */  lui $at, 0x8012
    /* 800B8BEC A020AD58 */  sb $zero, -21160($at)
    /* 800B8BF0 3C018012 */  lui $at, 0x8012
    /* 800B8BF4 A020AD5C */  sb $zero, -21156($at)
    /* 800B8BF8 8FBF001C */  lw $ra, 28($sp)
    /* 800B8BFC 3C018012 */  lui $at, 0x8012
    /* 800B8C00 AC20AD3C */  sw $zero, -21188($at)
    /* 800B8C04 3C018012 */  lui $at, 0x8012
    /* 800B8C08 A038AD30 */  sb $t8, -21200($at)
    /* 800B8C0C 03E00008 */  jr $ra
    /* 800B8C10 27BD0088 */  addiu $sp, $sp, 136
