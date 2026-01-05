# Source: game_code.bin (decompressed)
# Address: 0x800F2A28

glabel net_state_validate
    /* 800F2A28 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800F2A2C 3C0E8011 */  lui $t6, 0x8011
    /* 800F2A30 85CE64C0 */  lh $t6, 25792($t6)
    /* 800F2A34 AFB30024 */  sw $s3, 36($sp)
    /* 800F2A38 24130001 */  addiu $s3, $zero, 1
    /* 800F2A3C AFBF003C */  sw $ra, 60($sp)
    /* 800F2A40 AFBE0038 */  sw $fp, 56($sp)
    /* 800F2A44 AFB70034 */  sw $s7, 52($sp)
    /* 800F2A48 AFB60030 */  sw $s6, 48($sp)
    /* 800F2A4C AFB5002C */  sw $s5, 44($sp)
    /* 800F2A50 AFB40028 */  sw $s4, 40($sp)
    /* 800F2A54 AFB20020 */  sw $s2, 32($sp)
    /* 800F2A58 AFB1001C */  sw $s1, 28($sp)
    /* 800F2A5C 166E001C */  bne $s3, $t6, 0x800F2AD0
    /* 800F2A60 AFB00018 */  sw $s0, 24($sp)
    /* 800F2A64 3C118015 */  lui $s1, 0x8015
    /* 800F2A68 8631A108 */  lh $s1, -24312($s1)
    /* 800F2A6C 0000F025 */  or $fp, $zero, $zero
    /* 800F2A70 1A2000B0 */  blez $s1, 0x800F2D34
    /* 800F2A74 001E7880 */  sll $t7, $fp, 2
    /* 800F2A78 01FE7823 */  subu $t7, $t7, $fp
    /* 800F2A7C 000F7880 */  sll $t7, $t7, 2
    /* 800F2A80 3C188015 */  lui $t8, 0x8015
    /* 800F2A84 27180E30 */  addiu $t8, $t8, 3632
    /* 800F2A88 01FE7821 */  addu $t7, $t7, $fp
    /* 800F2A8C 01F81821 */  addu $v1, $t7, $t8
    /* 800F2A90 00001025 */  or $v0, $zero, $zero
    /* 800F2A94 0062C821 */  addu $t9, $v1, $v0
    /* 800F2A98 24420001 */  addiu $v0, $v0, 1
    /* 800F2A9C 00027400 */  sll $t6, $v0, 16
    /* 800F2AA0 000E1403 */  sra $v0, $t6, 16
    /* 800F2AA4 2841000D */  slti $at, $v0, 13
    /* 800F2AA8 1420FFFA */  bne $at, $zero, 0x800F2A94
    /* 800F2AAC A3330000 */  sb $s3, 0($t9)
    /* 800F2AB0 27DE0001 */  addiu $fp, $fp, 1
    /* 800F2AB4 001EC400 */  sll $t8, $fp, 16
    /* 800F2AB8 0018F403 */  sra $fp, $t8, 16
    /* 800F2ABC 03D1082A */  slt $at, $fp, $s1
    /* 800F2AC0 5420FFED */  .word 0x5420FFED
    /* 800F2AC4 001E7880 */  sll $t7, $fp, 2
    /* 800F2AC8 1000009A */  beq $zero, $zero, 0x800F2D34
    /* 800F2ACC 0000F025 */  or $fp, $zero, $zero
    /* 800F2AD0 3C118015 */  lui $s1, 0x8015
    /* 800F2AD4 8631A108 */  lh $s1, -24312($s1)
    /* 800F2AD8 3C068015 */  lui $a2, 0x8015
    /* 800F2ADC 24C60E30 */  addiu $a2, $a2, 3632
    /* 800F2AE0 1A200094 */  blez $s1, 0x800F2D34
    /* 800F2AE4 0000F025 */  or $fp, $zero, $zero
    /* 800F2AE8 001E7080 */  sll $t6, $fp, 2
    /* 800F2AEC 01DE7021 */  addu $t6, $t6, $fp
    /* 800F2AF0 000E7080 */  sll $t6, $t6, 2
    /* 800F2AF4 01DE7023 */  subu $t6, $t6, $fp
    /* 800F2AF8 3C0F8015 */  lui $t7, 0x8015
    /* 800F2AFC 25EFA118 */  addiu $t7, $t7, -24296
    /* 800F2B00 000E7080 */  sll $t6, $t6, 2
    /* 800F2B04 01CF9021 */  addu $s2, $t6, $t7
    /* 800F2B08 8E580048 */  lw $t8, 72($s2)
    /* 800F2B0C 001E7880 */  sll $t7, $fp, 2
    /* 800F2B10 0000B025 */  or $s6, $zero, $zero
    /* 800F2B14 8F190000 */  lw $t9, 0($t8)
    /* 800F2B18 0000A025 */  or $s4, $zero, $zero
    /* 800F2B1C 0000B825 */  or $s7, $zero, $zero
    /* 800F2B20 8F2E002C */  lw $t6, 44($t9)
    /* 800F2B24 0000A825 */  or $s5, $zero, $zero
    /* 800F2B28 01FE7823 */  subu $t7, $t7, $fp
    /* 800F2B2C 11C0007A */  beq $t6, $zero, 0x800F2D18
    /* 800F2B30 00001025 */  or $v0, $zero, $zero
    /* 800F2B34 000F7880 */  sll $t7, $t7, 2
    /* 800F2B38 01FE7821 */  addu $t7, $t7, $fp
    /* 800F2B3C 00CF1821 */  addu $v1, $a2, $t7
    /* 800F2B40 28410006 */  slti $at, $v0, 6
    /* 800F2B44 10200004 */  beq $at, $zero, 0x800F2B58
    /* 800F2B48 0062C821 */  addu $t9, $v1, $v0
    /* 800F2B4C 0062C021 */  addu $t8, $v1, $v0
    /* 800F2B50 10000002 */  beq $zero, $zero, 0x800F2B5C
    /* 800F2B54 A3130000 */  sb $s3, 0($t8)
    /* 800F2B58 A3200000 */  sb $zero, 0($t9)
    /* 800F2B5C 24420001 */  addiu $v0, $v0, 1
    /* 800F2B60 00027400 */  sll $t6, $v0, 16
    /* 800F2B64 000E1403 */  sra $v0, $t6, 16
    /* 800F2B68 2841000D */  slti $at, $v0, 13
    /* 800F2B6C 5420FFF5 */  .word 0x5420FFF5
    /* 800F2B70 28410006 */  slti $at, $v0, 6
    /* 800F2B74 00008025 */  or $s0, $zero, $zero
    /* 800F2B78 8E580048 */  lw $t8, 72($s2)
    /* 800F2B7C 24050010 */  addiu $a1, $zero, 16
    /* 800F2B80 8F190000 */  lw $t9, 0($t8)
    /* 800F2B84 8F2E002C */  lw $t6, 44($t9)
    /* 800F2B88 8DCF0000 */  lw $t7, 0($t6)
    /* 800F2B8C 01F08821 */  addu $s1, $t7, $s0
    /* 800F2B90 962400E8 */  lhu $a0, 232($s1)
    /* 800F2B94 2631008C */  addiu $s1, $s1, 140
    /* 800F2B98 309800FF */  andi $t8, $a0, 0x00FF
    /* 800F2B9C 0C02DE29 */  jal 0x800B78A4
    /* 800F2BA0 03002025 */  or $a0, $t8, $zero
    /* 800F2BA4 9624005C */  lhu $a0, 92($s1)
    /* 800F2BA8 02C2B021 */  addu $s6, $s6, $v0
    /* 800F2BAC 24050010 */  addiu $a1, $zero, 16
    /* 800F2BB0 3099FF00 */  andi $t9, $a0, 0xFF00
    /* 800F2BB4 0C02DE29 */  jal 0x800B78A4
    /* 800F2BB8 03202025 */  or $a0, $t9, $zero
    /* 800F2BBC 26100060 */  addiu $s0, $s0, 96
    /* 800F2BC0 24010240 */  addiu $at, $zero, 576
    /* 800F2BC4 1601FFEC */  bne $s0, $at, 0x800F2B78
    /* 800F2BC8 0282A021 */  addu $s4, $s4, $v0
    /* 800F2BCC 00008025 */  or $s0, $zero, $zero
    /* 800F2BD0 8E4E0048 */  lw $t6, 72($s2)
    /* 800F2BD4 24050010 */  addiu $a1, $zero, 16
    /* 800F2BD8 8DCF0000 */  lw $t7, 0($t6)
    /* 800F2BDC 8DF8002C */  lw $t8, 44($t7)
    /* 800F2BE0 8F190000 */  lw $t9, 0($t8)
    /* 800F2BE4 03308821 */  addu $s1, $t9, $s0
    /* 800F2BE8 96240548 */  lhu $a0, 1352($s1)
    /* 800F2BEC 2631050C */  addiu $s1, $s1, 1292
    /* 800F2BF0 308E00FF */  andi $t6, $a0, 0x00FF
    /* 800F2BF4 0C02DE29 */  jal 0x800B78A4
    /* 800F2BF8 01C02025 */  or $a0, $t6, $zero
    /* 800F2BFC 9624003C */  lhu $a0, 60($s1)
    /* 800F2C00 02E2B821 */  addu $s7, $s7, $v0
    /* 800F2C04 24050010 */  addiu $a1, $zero, 16
    /* 800F2C08 308FFF00 */  andi $t7, $a0, 0xFF00
    /* 800F2C0C 0C02DE29 */  jal 0x800B78A4
    /* 800F2C10 01E02025 */  or $a0, $t7, $zero
    /* 800F2C14 26100040 */  addiu $s0, $s0, 64
    /* 800F2C18 24010100 */  addiu $at, $zero, 256
    /* 800F2C1C 1601FFEC */  bne $s0, $at, 0x800F2BD0
    /* 800F2C20 02A2A821 */  addu $s5, $s5, $v0
    /* 800F2C24 2AC10030 */  slti $at, $s6, 48
    /* 800F2C28 14200008 */  bne $at, $zero, 0x800F2C4C
    /* 800F2C2C 3C118015 */  lui $s1, 0x8015
    /* 800F2C30 2407000D */  addiu $a3, $zero, 13
    /* 800F2C34 03C70019 */  multu $fp, $a3
    /* 800F2C38 3C068015 */  lui $a2, 0x8015
    /* 800F2C3C 24C60E30 */  addiu $a2, $a2, 3632
    /* 800F2C40 0000C012 */  mflo $t8
    /* 800F2C44 00D8C821 */  addu $t9, $a2, $t8
    /* 800F2C48 A3330006 */  sb $s3, 6($t9)
    /* 800F2C4C 3C068015 */  lui $a2, 0x8015
    /* 800F2C50 2A810018 */  slti $at, $s4, 24
    /* 800F2C54 24C60E30 */  addiu $a2, $a2, 3632
    /* 800F2C58 14200005 */  bne $at, $zero, 0x800F2C70
    /* 800F2C5C 2407000D */  addiu $a3, $zero, 13
    /* 800F2C60 03C70019 */  multu $fp, $a3
    /* 800F2C64 00007012 */  mflo $t6
    /* 800F2C68 00CE7821 */  addu $t7, $a2, $t6
    /* 800F2C6C A1F30007 */  sb $s3, 7($t7)
    /* 800F2C70 2A810024 */  slti $at, $s4, 36
    /* 800F2C74 54200006 */  .word 0x54200006
    /* 800F2C78 2AE10020 */  slti $at, $s7, 32
    /* 800F2C7C 03C70019 */  multu $fp, $a3
    /* 800F2C80 0000C012 */  mflo $t8
    /* 800F2C84 00D8C821 */  addu $t9, $a2, $t8
    /* 800F2C88 A3330008 */  sb $s3, 8($t9)
    /* 800F2C8C 2AE10020 */  slti $at, $s7, 32
    /* 800F2C90 54200006 */  .word 0x54200006
    /* 800F2C94 2AA10010 */  slti $at, $s5, 16
    /* 800F2C98 03C70019 */  multu $fp, $a3
    /* 800F2C9C 00007012 */  mflo $t6
    /* 800F2CA0 00CE7821 */  addu $t7, $a2, $t6
    /* 800F2CA4 A1F30009 */  sb $s3, 9($t7)
    /* 800F2CA8 2AA10010 */  slti $at, $s5, 16
    /* 800F2CAC 54200006 */  .word 0x54200006
    /* 800F2CB0 2AA10018 */  slti $at, $s5, 24
    /* 800F2CB4 03C70019 */  multu $fp, $a3
    /* 800F2CB8 0000C012 */  mflo $t8
    /* 800F2CBC 00D8C821 */  addu $t9, $a2, $t8
    /* 800F2CC0 A333000A */  sb $s3, 10($t9)
    /* 800F2CC4 2AA10018 */  slti $at, $s5, 24
    /* 800F2CC8 54200006 */  .word 0x54200006
    /* 800F2CCC 2AC10030 */  slti $at, $s6, 48
    /* 800F2CD0 03C70019 */  multu $fp, $a3
    /* 800F2CD4 00007012 */  mflo $t6
    /* 800F2CD8 00CE7821 */  addu $t7, $a2, $t6
    /* 800F2CDC A1F3000B */  sb $s3, 11($t7)
    /* 800F2CE0 2AC10030 */  slti $at, $s6, 48
    /* 800F2CE4 1420000B */  bne $at, $zero, 0x800F2D14
    /* 800F2CE8 2A810030 */  slti $at, $s4, 48
    /* 800F2CEC 14200009 */  bne $at, $zero, 0x800F2D14
    /* 800F2CF0 2AE10020 */  slti $at, $s7, 32
    /* 800F2CF4 14200007 */  bne $at, $zero, 0x800F2D14
    /* 800F2CF8 2AA10020 */  slti $at, $s5, 32
    /* 800F2CFC 14200005 */  bne $at, $zero, 0x800F2D14
    /* 800F2D00 00000000 */  nop
    /* 800F2D04 03C70019 */  multu $fp, $a3
    /* 800F2D08 0000C012 */  mflo $t8
    /* 800F2D0C 00D8C821 */  addu $t9, $a2, $t8
    /* 800F2D10 A333000C */  sb $s3, 12($t9)
    /* 800F2D14 8631A108 */  lh $s1, -24312($s1)
    /* 800F2D18 27DE0001 */  addiu $fp, $fp, 1
    /* 800F2D1C 001E7400 */  sll $t6, $fp, 16
    /* 800F2D20 000EF403 */  sra $fp, $t6, 16
    /* 800F2D24 03D1082A */  slt $at, $fp, $s1
    /* 800F2D28 5420FF70 */  .word 0x5420FF70
    /* 800F2D2C 001E7080 */  sll $t6, $fp, 2
    /* 800F2D30 0000F025 */  or $fp, $zero, $zero
    /* 800F2D34 3C188011 */  lui $t8, 0x8011
    /* 800F2D38 871864C4 */  lh $t8, 25796($t8)
    /* 800F2D3C 3C128011 */  lui $s2, 0x8011
    /* 800F2D40 16780052 */  bne $s3, $t8, 0x800F2E8C
    /* 800F2D44 00000000 */  nop
    /* 800F2D48 1A2000CB */  blez $s1, 0x800F3078
    /* 800F2D4C 24080006 */  addiu $t0, $zero, 6
    /* 800F2D50 24040007 */  addiu $a0, $zero, 7
    /* 800F2D54 001EC880 */  sll $t9, $fp, 2
    /* 800F2D58 033EC821 */  addu $t9, $t9, $fp
    /* 800F2D5C 0019C880 */  sll $t9, $t9, 2
    /* 800F2D60 033EC823 */  subu $t9, $t9, $fp
    /* 800F2D64 0019C880 */  sll $t9, $t9, 2
    /* 800F2D68 3C0E8015 */  lui $t6, 0x8015
    /* 800F2D6C 01D97021 */  addu $t6, $t6, $t9
    /* 800F2D70 8DCEA160 */  lw $t6, -24224($t6)
    /* 800F2D74 001EC8C0 */  sll $t9, $fp, 3
    /* 800F2D78 00001025 */  or $v0, $zero, $zero
    /* 800F2D7C 8DCF0000 */  lw $t7, 0($t6)
    /* 800F2D80 3C0E8015 */  lui $t6, 0x8015
    /* 800F2D84 25CE0EB8 */  addiu $t6, $t6, 3768
    /* 800F2D88 8DF8002C */  lw $t8, 44($t7)
    /* 800F2D8C 032E1821 */  addu $v1, $t9, $t6
    /* 800F2D90 53000037 */  .word 0x53000037
    /* 800F2D94 27DE0001 */  addiu $fp, $fp, 1
    /* 800F2D98 10480003 */  beq $v0, $t0, 0x800F2DA8
    /* 800F2D9C 00627821 */  addu $t7, $v1, $v0
    /* 800F2DA0 14440003 */  bne $v0, $a0, 0x800F2DB0
    /* 800F2DA4 0062C021 */  addu $t8, $v1, $v0
    /* 800F2DA8 10000002 */  beq $zero, $zero, 0x800F2DB4
    /* 800F2DAC A1E00000 */  sb $zero, 0($t7)
    /* 800F2DB0 A3130000 */  sb $s3, 0($t8)
    /* 800F2DB4 24420001 */  addiu $v0, $v0, 1
    /* 800F2DB8 0002CC00 */  sll $t9, $v0, 16
    /* 800F2DBC 00191403 */  sra $v0, $t9, 16
    /* 800F2DC0 28410008 */  slti $at, $v0, 8
    /* 800F2DC4 1420FFF4 */  bne $at, $zero, 0x800F2D98
    /* 800F2DC8 00000000 */  nop
    /* 800F2DCC 001E78C0 */  sll $t7, $fp, 3
    /* 800F2DD0 3C188015 */  lui $t8, 0x8015
    /* 800F2DD4 27180ED8 */  addiu $t8, $t8, 3800
    /* 800F2DD8 01FE7821 */  addu $t7, $t7, $fp
    /* 800F2DDC 01F81821 */  addu $v1, $t7, $t8
    /* 800F2DE0 00001025 */  or $v0, $zero, $zero
    /* 800F2DE4 0062C821 */  addu $t9, $v1, $v0
    /* 800F2DE8 24420001 */  addiu $v0, $v0, 1
    /* 800F2DEC 00027400 */  sll $t6, $v0, 16
    /* 800F2DF0 000E1403 */  sra $v0, $t6, 16
    /* 800F2DF4 28410009 */  slti $at, $v0, 9
    /* 800F2DF8 1420FFFA */  bne $at, $zero, 0x800F2DE4
    /* 800F2DFC A3330000 */  sb $s3, 0($t9)
    /* 800F2E00 001EC080 */  sll $t8, $fp, 2
    /* 800F2E04 3C198015 */  lui $t9, 0x8015
    /* 800F2E08 27390F00 */  addiu $t9, $t9, 3840
    /* 800F2E0C 031EC021 */  addu $t8, $t8, $fp
    /* 800F2E10 03191821 */  addu $v1, $t8, $t9
    /* 800F2E14 00001025 */  or $v0, $zero, $zero
    /* 800F2E18 00627021 */  addu $t6, $v1, $v0
    /* 800F2E1C 24420001 */  addiu $v0, $v0, 1
    /* 800F2E20 00027C00 */  sll $t7, $v0, 16
    /* 800F2E24 000F1403 */  sra $v0, $t7, 16
    /* 800F2E28 28410005 */  slti $at, $v0, 5
    /* 800F2E2C 1420FFFA */  bne $at, $zero, 0x800F2E18
    /* 800F2E30 A1D30000 */  sb $s3, 0($t6)
    /* 800F2E34 001EC880 */  sll $t9, $fp, 2
    /* 800F2E38 033EC823 */  subu $t9, $t9, $fp
    /* 800F2E3C 3C0E8015 */  lui $t6, 0x8015
    /* 800F2E40 25CE0F40 */  addiu $t6, $t6, 3904
    /* 800F2E44 0019C840 */  sll $t9, $t9, 1
    /* 800F2E48 032E1821 */  addu $v1, $t9, $t6
    /* 800F2E4C 00001025 */  or $v0, $zero, $zero
    /* 800F2E50 00627821 */  addu $t7, $v1, $v0
    /* 800F2E54 24420001 */  addiu $v0, $v0, 1
    /* 800F2E58 0002C400 */  sll $t8, $v0, 16
    /* 800F2E5C 00181403 */  sra $v0, $t8, 16
    /* 800F2E60 28410006 */  slti $at, $v0, 6
    /* 800F2E64 1420FFFA */  bne $at, $zero, 0x800F2E50
    /* 800F2E68 A1F30000 */  sb $s3, 0($t7)
    /* 800F2E6C 27DE0001 */  addiu $fp, $fp, 1
    /* 800F2E70 001E7400 */  sll $t6, $fp, 16
    /* 800F2E74 000EF403 */  sra $fp, $t6, 16
    /* 800F2E78 03D1082A */  slt $at, $fp, $s1
    /* 800F2E7C 5420FFB6 */  .word 0x5420FFB6
    /* 800F2E80 001EC880 */  sll $t9, $fp, 2
    /* 800F2E84 1000007C */  beq $zero, $zero, 0x800F3078
    /* 800F2E88 0000F025 */  or $fp, $zero, $zero
    /* 800F2E8C 1A20007A */  blez $s1, 0x800F3078
    /* 800F2E90 3C108015 */  lui $s0, 0x8015
    /* 800F2E94 3C0D8015 */  lui $t5, 0x8015
    /* 800F2E98 3C0B8015 */  lui $t3, 0x8015
    /* 800F2E9C 3C0A8015 */  lui $t2, 0x8015
    /* 800F2EA0 254A0EB8 */  addiu $t2, $t2, 3768
    /* 800F2EA4 256B0ED8 */  addiu $t3, $t3, 3800
    /* 800F2EA8 25AD0F00 */  addiu $t5, $t5, 3840
    /* 800F2EAC 26100F40 */  addiu $s0, $s0, 3904
    /* 800F2EB0 241F0005 */  addiu $ra, $zero, 5
    /* 800F2EB4 240C0009 */  addiu $t4, $zero, 9
    /* 800F2EB8 2409000A */  addiu $t1, $zero, 10
    /* 800F2EBC 24080006 */  addiu $t0, $zero, 6
    /* 800F2EC0 2406000C */  addiu $a2, $zero, 12
    /* 800F2EC4 001EC080 */  sll $t8, $fp, 2
    /* 800F2EC8 031EC021 */  addu $t8, $t8, $fp
    /* 800F2ECC 0018C080 */  sll $t8, $t8, 2
    /* 800F2ED0 031EC023 */  subu $t8, $t8, $fp
    /* 800F2ED4 0018C080 */  sll $t8, $t8, 2
    /* 800F2ED8 3C198015 */  lui $t9, 0x8015
    /* 800F2EDC 0338C821 */  addu $t9, $t9, $t8
    /* 800F2EE0 8F39A160 */  lw $t9, -24224($t9)
    /* 800F2EE4 00001025 */  or $v0, $zero, $zero
    /* 800F2EE8 00002025 */  or $a0, $zero, $zero
    /* 800F2EEC 8F2E0000 */  lw $t6, 0($t9)
    /* 800F2EF0 8DC7002C */  lw $a3, 44($t6)
    /* 800F2EF4 50E0005A */  .word 0x50E0005A
    /* 800F2EF8 27DE0001 */  addiu $fp, $fp, 1
    /* 800F2EFC 8CE30000 */  lw $v1, 0($a3)
    /* 800F2F00 8C6F00E4 */  lw $t7, 228($v1)
    /* 800F2F04 8C780144 */  lw $t8, 324($v1)
    /* 800F2F08 8C7901A4 */  lw $t9, 420($v1)
    /* 800F2F0C 004F1021 */  addu $v0, $v0, $t7
    /* 800F2F10 8C6E0204 */  lw $t6, 516($v1)
    /* 800F2F14 00581021 */  addu $v0, $v0, $t8
    /* 800F2F18 24840004 */  addiu $a0, $a0, 4
    /* 800F2F1C 00591021 */  addu $v0, $v0, $t9
    /* 800F2F20 24630180 */  addiu $v1, $v1, 384
    /* 800F2F24 1486FFF6 */  bne $a0, $a2, 0x800F2F00
    /* 800F2F28 004E1021 */  addu $v0, $v0, $t6
    /* 800F2F2C 0049001A */  div $v0, $t1
    /* 800F2F30 15200002 */  bne $t1, $zero, 0x800F2F3C
    /* 800F2F34 00000000 */  nop
    /* 800F2F38 0007000D */  .word 0x0007000D
    /* 800F2F3C 2401FFFF */  addiu $at, $zero, -1
    /* 800F2F40 15210004 */  bne $t1, $at, 0x800F2F54
    /* 800F2F44 3C018000 */  lui $at, 0x8000
    /* 800F2F48 14410002 */  bne $v0, $at, 0x800F2F54
    /* 800F2F4C 00000000 */  nop
    /* 800F2F50 0006000D */  .word 0x0006000D
    /* 800F2F54 00001012 */  mflo $v0
    /* 800F2F58 285900FA */  slti $t9, $v0, 250
    /* 800F2F5C 3B390001 */  xori $t9, $t9, 0x0001
    /* 800F2F60 03CC0019 */  multu $fp, $t4
    /* 800F2F64 001E78C0 */  sll $t7, $fp, 3
    /* 800F2F68 014F1821 */  addu $v1, $t2, $t7
    /* 800F2F6C 284700C8 */  slti $a3, $v0, 200
    /* 800F2F70 284501F4 */  slti $a1, $v0, 500
    /* 800F2F74 38E70001 */  xori $a3, $a3, 0x0001
    /* 800F2F78 38A50001 */  xori $a1, $a1, 0x0001
    /* 800F2F7C A0730000 */  sb $s3, 0($v1)
    /* 800F2F80 A0730001 */  sb $s3, 1($v1)
    /* 800F2F84 A0670002 */  sb $a3, 2($v1)
    /* 800F2F88 0000C012 */  mflo $t8
    /* 800F2F8C 01782021 */  addu $a0, $t3, $t8
    /* 800F2F90 A0990003 */  sb $t9, 3($a0)
    /* 800F2F94 03DF0019 */  multu $fp, $ra
    /* 800F2F98 A0670003 */  sb $a3, 3($v1)
    /* 800F2F9C A0650004 */  sb $a1, 4($v1)
    /* 800F2FA0 A0650005 */  sb $a1, 5($v1)
    /* 800F2FA4 A0600006 */  sb $zero, 6($v1)
    /* 800F2FA8 A0600007 */  sb $zero, 7($v1)
    /* 800F2FAC A0850004 */  sb $a1, 4($a0)
    /* 800F2FB0 284E0320 */  slti $t6, $v0, 800
    /* 800F2FB4 284F0640 */  slti $t7, $v0, 1600
    /* 800F2FB8 285807D0 */  slti $t8, $v0, 2000
    /* 800F2FBC 0000C812 */  mflo $t9
    /* 800F2FC0 01B91821 */  addu $v1, $t5, $t9
    /* 800F2FC4 A0930000 */  sb $s3, 0($a0)
    /* 800F2FC8 03C80019 */  multu $fp, $t0
    /* 800F2FCC A0930001 */  sb $s3, 1($a0)
    /* 800F2FD0 A0930002 */  sb $s3, 2($a0)
    /* 800F2FD4 39CE0001 */  xori $t6, $t6, 0x0001
    /* 800F2FD8 39EF0001 */  xori $t7, $t7, 0x0001
    /* 800F2FDC 3B180001 */  xori $t8, $t8, 0x0001
    /* 800F2FE0 A08E0005 */  sb $t6, 5($a0)
    /* 800F2FE4 A08F0007 */  sb $t7, 7($a0)
    /* 800F2FE8 A0980008 */  sb $t8, 8($a0)
    /* 800F2FEC 284704B0 */  slti $a3, $v0, 1200
    /* 800F2FF0 0000C812 */  mflo $t9
    /* 800F2FF4 284E012C */  slti $t6, $v0, 300
    /* 800F2FF8 284F0064 */  slti $t7, $v0, 100
    /* 800F2FFC 28580258 */  slti $t8, $v0, 600
    /* 800F3000 02192821 */  addu $a1, $s0, $t9
    /* 800F3004 38E70001 */  xori $a3, $a3, 0x0001
    /* 800F3008 39CE0001 */  xori $t6, $t6, 0x0001
    /* 800F300C 39EF0001 */  xori $t7, $t7, 0x0001
    /* 800F3010 3B180001 */  xori $t8, $t8, 0x0001
    /* 800F3014 A06E0001 */  sb $t6, 1($v1)
    /* 800F3018 A06F0003 */  sb $t7, 3($v1)
    /* 800F301C A0780004 */  sb $t8, 4($v1)
    /* 800F3020 A0870006 */  sb $a3, 6($a0)
    /* 800F3024 A0730000 */  sb $s3, 0($v1)
    /* 800F3028 A0670002 */  sb $a3, 2($v1)
    /* 800F302C 284E0096 */  slti $t6, $v0, 150
    /* 800F3030 284F0190 */  slti $t7, $v0, 400
    /* 800F3034 285802BC */  slti $t8, $v0, 700
    /* 800F3038 39CE0001 */  xori $t6, $t6, 0x0001
    /* 800F303C 39EF0001 */  xori $t7, $t7, 0x0001
    /* 800F3040 3B180001 */  xori $t8, $t8, 0x0001
    /* 800F3044 A0B30000 */  sb $s3, 0($a1)
    /* 800F3048 A0B30001 */  sb $s3, 1($a1)
    /* 800F304C A0B30002 */  sb $s3, 2($a1)
    /* 800F3050 A0AE0003 */  sb $t6, 3($a1)
    /* 800F3054 A0AF0004 */  sb $t7, 4($a1)
    /* 800F3058 A0B80005 */  sb $t8, 5($a1)
    /* 800F305C 27DE0001 */  addiu $fp, $fp, 1
    /* 800F3060 001ECC00 */  sll $t9, $fp, 16
    /* 800F3064 0019F403 */  sra $fp, $t9, 16
    /* 800F3068 03D1082A */  slt $at, $fp, $s1
    /* 800F306C 5420FF96 */  .word 0x5420FF96
    /* 800F3070 001EC080 */  sll $t8, $fp, 2
    /* 800F3074 0000F025 */  or $fp, $zero, $zero
    /* 800F3078 865264C2 */  lh $s2, 25794($s2)
    /* 800F307C 16720019 */  bne $s3, $s2, 0x800F30E4
    /* 800F3080 00000000 */  nop
    /* 800F3084 1A200080 */  blez $s1, 0x800F3288
    /* 800F3088 001E7880 */  sll $t7, $fp, 2
    /* 800F308C 01FE7821 */  addu $t7, $t7, $fp
    /* 800F3090 000F7880 */  sll $t7, $t7, 2
    /* 800F3094 3C188015 */  lui $t8, 0x8015
    /* 800F3098 27180DD8 */  addiu $t8, $t8, 3544
    /* 800F309C 01FE7823 */  subu $t7, $t7, $fp
    /* 800F30A0 01F82821 */  addu $a1, $t7, $t8
    /* 800F30A4 00001025 */  or $v0, $zero, $zero
    /* 800F30A8 00A2C821 */  addu $t9, $a1, $v0
    /* 800F30AC 24420001 */  addiu $v0, $v0, 1
    /* 800F30B0 00027400 */  sll $t6, $v0, 16
    /* 800F30B4 000E1403 */  sra $v0, $t6, 16
    /* 800F30B8 28410013 */  slti $at, $v0, 19
    /* 800F30BC 1420FFFA */  bne $at, $zero, 0x800F30A8
    /* 800F30C0 A3330000 */  sb $s3, 0($t9)
    /* 800F30C4 27DE0001 */  addiu $fp, $fp, 1
    /* 800F30C8 001EC400 */  sll $t8, $fp, 16
    /* 800F30CC 0018F403 */  sra $fp, $t8, 16
    /* 800F30D0 03D1082A */  slt $at, $fp, $s1
    /* 800F30D4 5420FFED */  .word 0x5420FFED
    /* 800F30D8 001E7880 */  sll $t7, $fp, 2
    /* 800F30DC 1000006A */  beq $zero, $zero, 0x800F3288
    /* 800F30E0 0000F025 */  or $fp, $zero, $zero
    /* 800F30E4 1A200068 */  blez $s1, 0x800F3288
    /* 800F30E8 24100013 */  addiu $s0, $zero, 19
    /* 800F30EC 3C1F8015 */  lui $ra, 0x8015
    /* 800F30F0 3C0D000F */  lui $t5, 0x000F
    /* 800F30F4 3C0C0007 */  lui $t4, 0x0007
    /* 800F30F8 3C0B0003 */  lui $t3, 0x0003
    /* 800F30FC 3C0A0001 */  lui $t2, 0x0001
    /* 800F3100 354A86A0 */  ori $t2, $t2, 0x86A0
    /* 800F3104 356BD090 */  ori $t3, $t3, 0xD090
    /* 800F3108 358CA120 */  ori $t4, $t4, 0xA120
    /* 800F310C 35AD4240 */  ori $t5, $t5, 0x4240
    /* 800F3110 27FF0DD8 */  addiu $ra, $ra, 3544
    /* 800F3114 24080008 */  addiu $t0, $zero, 8
    /* 800F3118 001E7080 */  sll $t6, $fp, 2
    /* 800F311C 01DE7021 */  addu $t6, $t6, $fp
    /* 800F3120 000E7080 */  sll $t6, $t6, 2
    /* 800F3124 01DE7023 */  subu $t6, $t6, $fp
    /* 800F3128 00002025 */  or $a0, $zero, $zero
    /* 800F312C 03EE2821 */  addu $a1, $ra, $t6
    /* 800F3130 00001025 */  or $v0, $zero, $zero
    /* 800F3134 00A27821 */  addu $t7, $a1, $v0
    /* 800F3138 24420001 */  addiu $v0, $v0, 1
    /* 800F313C 0002C400 */  sll $t8, $v0, 16
    /* 800F3140 00181403 */  sra $v0, $t8, 16
    /* 800F3144 28410013 */  slti $at, $v0, 19
    /* 800F3148 1420FFFA */  bne $at, $zero, 0x800F3134
    /* 800F314C A1E00000 */  sb $zero, 0($t7)
    /* 800F3150 03D00019 */  multu $fp, $s0
    /* 800F3154 001E7880 */  sll $t7, $fp, 2
    /* 800F3158 01FE7821 */  addu $t7, $t7, $fp
    /* 800F315C 000F7880 */  sll $t7, $t7, 2
    /* 800F3160 01FE7823 */  subu $t7, $t7, $fp
    /* 800F3164 000F7880 */  sll $t7, $t7, 2
    /* 800F3168 3C188015 */  lui $t8, 0x8015
    /* 800F316C 030FC021 */  addu $t8, $t8, $t7
    /* 800F3170 27DE0001 */  addiu $fp, $fp, 1
    /* 800F3174 00007012 */  mflo $t6
    /* 800F3178 03EE3021 */  addu $a2, $ra, $t6
    /* 800F317C A0D30000 */  sb $s3, 0($a2)
    /* 800F3180 A0D30001 */  sb $s3, 1($a2)
    /* 800F3184 A0D30002 */  sb $s3, 2($a2)
    /* 800F3188 A0D30003 */  sb $s3, 3($a2)
    /* 800F318C A0D3000E */  sb $s3, 14($a2)
    /* 800F3190 A0D30006 */  sb $s3, 6($a2)
    /* 800F3194 A0D30007 */  sb $s3, 7($a2)
    /* 800F3198 A0D30008 */  sb $s3, 8($a2)
    /* 800F319C A0D30009 */  sb $s3, 9($a2)
    /* 800F31A0 8F18A160 */  lw $t8, -24224($t8)
    /* 800F31A4 8F190000 */  lw $t9, 0($t8)
    /* 800F31A8 8F27002C */  lw $a3, 44($t9)
    /* 800F31AC 50E00031 */  .word 0x50E00031
    /* 800F31B0 001ECC00 */  sll $t9, $fp, 16
    /* 800F31B4 8CE20000 */  lw $v0, 0($a3)
    /* 800F31B8 2445050C */  addiu $a1, $v0, 1292
    /* 800F31BC 8CAE004C */  lw $t6, 76($a1)
    /* 800F31C0 8CA3000C */  lw $v1, 12($a1)
    /* 800F31C4 8CAF008C */  lw $t7, 140($a1)
    /* 800F31C8 8CB800CC */  lw $t8, 204($a1)
    /* 800F31CC 006E1821 */  addu $v1, $v1, $t6
    /* 800F31D0 2449060C */  addiu $t1, $v0, 1548
    /* 800F31D4 006F1821 */  addu $v1, $v1, $t7
    /* 800F31D8 01201025 */  or $v0, $t1, $zero
    /* 800F31DC 00002825 */  or $a1, $zero, $zero
    /* 800F31E0 00781821 */  addu $v1, $v1, $t8
    /* 800F31E4 94590008 */  lhu $t9, 8($v0)
    /* 800F31E8 944E0014 */  lhu $t6, 20($v0)
    /* 800F31EC 944F0020 */  lhu $t7, 32($v0)
    /* 800F31F0 00992021 */  addu $a0, $a0, $t9
    /* 800F31F4 9458002C */  lhu $t8, 44($v0)
