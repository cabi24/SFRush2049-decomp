# Source: game_code.bin (decompressed)
# Address: 0x800D6E7C

glabel drone_collision_avoid
    /* 800D6E7C 27BDFF70 */  addiu $sp, $sp, -144
    /* 800D6E80 3C148014 */  lui $s4, 0x8014
    /* 800D6E84 269461D0 */  addiu $s4, $s4, 25040
    /* 800D6E88 AFBF0014 */  sw $ra, 20($sp)
    /* 800D6E8C 02802025 */  or $a0, $s4, $zero
    /* 800D6E90 00002825 */  or $a1, $zero, $zero
    /* 800D6E94 0C001C9C */  jal 0x80007270
    /* 800D6E98 24060001 */  addiu $a2, $zero, 1
    /* 800D6E9C 0C02D080 */  jal 0x800B4200
    /* 800D6EA0 2412000A */  addiu $s2, $zero, 10
    /* 800D6EA4 00408025 */  or $s0, $v0, $zero
    /* 800D6EA8 02802025 */  or $a0, $s4, $zero
    /* 800D6EAC 00002825 */  or $a1, $zero, $zero
    /* 800D6EB0 0C001D78 */  jal 0x800075E0
    /* 800D6EB4 00003025 */  or $a2, $zero, $zero
    /* 800D6EB8 3C158011 */  lui $s5, 0x8011
    /* 800D6EBC 3C138018 */  lui $s3, 0x8018
    /* 800D6EC0 AFA00080 */  sw $zero, 128($sp)
    /* 800D6EC4 0000A025 */  or $s4, $zero, $zero
    /* 800D6EC8 AFA00084 */  sw $zero, 132($sp)
    /* 800D6ECC 2673A4E0 */  addiu $s3, $s3, -23328
    /* 800D6ED0 26B50030 */  addiu $s5, $s5, 48
    /* 800D6ED4 AFA0008C */  sw $zero, 140($sp)
    /* 800D6ED8 AFA00020 */  sw $zero, 32($sp)
    /* 800D6EDC 241E000D */  addiu $fp, $zero, 13
    /* 800D6EE0 2417000C */  addiu $s7, $zero, 12
    /* 800D6EE4 2416000E */  addiu $s6, $zero, 14
    /* 800D6EE8 27B20054 */  addiu $s2, $sp, 84
    /* 800D6EEC 8E6F000C */  lw $t7, 12($s3)
    /* 800D6EF0 8E6E0010 */  lw $t6, 16($s3)
    /* 800D6EF4 8FA20020 */  lw $v0, 32($sp)
    /* 800D6EF8 95F8004E */  lhu $t8, 78($t7)
    /* 800D6EFC 2405FFFF */  addiu $a1, $zero, -1
    /* 800D6F00 0018C880 */  sll $t9, $t8, 2
    /* 800D6F04 01D97821 */  addu $t7, $t6, $t9
    /* 800D6F08 01E2C021 */  addu $t8, $t7, $v0
    /* 800D6F0C 0C02CFE9 */  jal 0x800B3FA4
    /* 800D6F10 8F040000 */  lw $a0, 0($t8)
    /* 800D6F14 8FAE0080 */  lw $t6, 128($sp)
    /* 800D6F18 01C2082B */  sltu $at, $t6, $v0
    /* 800D6F1C 5020000D */  .word 0x5020000D
    /* 800D6F20 8FAE008C */  lw $t6, 140($sp)
    /* 800D6F24 8E6F000C */  lw $t7, 12($s3)
    /* 800D6F28 8E790010 */  lw $t9, 16($s3)
    /* 800D6F2C 2405FFFF */  addiu $a1, $zero, -1
    /* 800D6F30 95F8004E */  lhu $t8, 78($t7)
    /* 800D6F34 00187080 */  sll $t6, $t8, 2
    /* 800D6F38 8FB80020 */  lw $t8, 32($sp)
    /* 800D6F3C 032E7821 */  addu $t7, $t9, $t6
    /* 800D6F40 01F8C821 */  addu $t9, $t7, $t8
    /* 800D6F44 0C02CFE9 */  jal 0x800B3FA4
    /* 800D6F48 8F240000 */  lw $a0, 0($t9)
    /* 800D6F4C AFA20080 */  sw $v0, 128($sp)
    /* 800D6F50 8FAE008C */  lw $t6, 140($sp)
    /* 800D6F54 3C048012 */  lui $a0, 0x8012
    /* 800D6F58 24010001 */  addiu $at, $zero, 1
    /* 800D6F5C 11C00009 */  beq $t6, $zero, 0x800D6F84
    /* 800D6F60 2484F6E8 */  addiu $a0, $a0, -2328
    /* 800D6F64 11C10007 */  beq $t6, $at, 0x800D6F84
    /* 800D6F68 24010002 */  addiu $at, $zero, 2
    /* 800D6F6C 11C1000F */  beq $t6, $at, 0x800D6FAC
    /* 800D6F70 24010003 */  addiu $at, $zero, 3
    /* 800D6F74 51C1004A */  .word 0x51C1004A
    /* 800D6F78 8E78000C */  lw $t8, 12($s3)
    /* 800D6F7C 10000070 */  beq $zero, $zero, 0x800D7140
    /* 800D6F80 8FAF0084 */  lw $t7, 132($sp)
    /* 800D6F84 0C02CFE9 */  jal 0x800B3FA4
    /* 800D6F88 2405FFFF */  addiu $a1, $zero, -1
    /* 800D6F8C 0282082B */  sltu $at, $s4, $v0
    /* 800D6F90 1020006A */  beq $at, $zero, 0x800D713C
    /* 800D6F94 3C048012 */  lui $a0, 0x8012
    /* 800D6F98 2484F6EC */  addiu $a0, $a0, -2324
    /* 800D6F9C 0C02CFE9 */  jal 0x800B3FA4
    /* 800D6FA0 2405FFFF */  addiu $a1, $zero, -1
    /* 800D6FA4 10000065 */  beq $zero, $zero, 0x800D713C
    /* 800D6FA8 0040A025 */  or $s4, $v0, $zero
    /* 800D6FAC 00008825 */  or $s1, $zero, $zero
    /* 800D6FB0 2A21000C */  slti $at, $s1, 12
    /* 800D6FB4 10200008 */  beq $at, $zero, 0x800D6FD8
    /* 800D6FB8 02408025 */  or $s0, $s2, $zero
    /* 800D6FBC 00117880 */  sll $t7, $s1, 2
    /* 800D6FC0 02AFC021 */  addu $t8, $s5, $t7
    /* 800D6FC4 8F050000 */  lw $a1, 0($t8)
    /* 800D6FC8 0C02F9A9 */  jal 0x800BE6A4
    /* 800D6FCC 02402025 */  or $a0, $s2, $zero
    /* 800D6FD0 10000014 */  beq $zero, $zero, 0x800D7024
    /* 800D6FD4 93B80054 */  lbu $t8, 84($sp)
    /* 800D6FD8 16370007 */  bne $s1, $s7, 0x800D6FF8
    /* 800D6FDC 00000000 */  nop
    /* 800D6FE0 8E790004 */  lw $t9, 4($s3)
    /* 800D6FE4 02402025 */  or $a0, $s2, $zero
    /* 800D6FE8 0C02F9A9 */  jal 0x800BE6A4
    /* 800D6FEC 8F250088 */  lw $a1, 136($t9)
    /* 800D6FF0 1000000C */  beq $zero, $zero, 0x800D7024
    /* 800D6FF4 93B80054 */  lbu $t8, 84($sp)
    /* 800D6FF8 563E000A */  .word 0x563E000A
    /* 800D6FFC 93B80054 */  lbu $t8, 84($sp)
    /* 800D7000 8E6F000C */  lw $t7, 12($s3)
    /* 800D7004 8E6E0010 */  lw $t6, 16($s3)
    /* 800D7008 02402025 */  or $a0, $s2, $zero
    /* 800D700C 95F80020 */  lhu $t8, 32($t7)
    /* 800D7010 0018C880 */  sll $t9, $t8, 2
    /* 800D7014 01D97821 */  addu $t7, $t6, $t9
    /* 800D7018 0C02F9A9 */  jal 0x800BE6A4
    /* 800D701C 8DE50000 */  lw $a1, 0($t7)
    /* 800D7020 93B80054 */  lbu $t8, 84($sp)
    /* 800D7024 27AE0054 */  addiu $t6, $sp, 84
    /* 800D7028 02402025 */  or $a0, $s2, $zero
    /* 800D702C 1300000E */  beq $t8, $zero, 0x800D7068
    /* 800D7030 00000000 */  nop
    /* 800D7034 91C20000 */  lbu $v0, 0($t6)
    /* 800D7038 28410061 */  slti $at, $v0, 97
    /* 800D703C 14200005 */  bne $at, $zero, 0x800D7054
    /* 800D7040 2841007B */  slti $at, $v0, 123
    /* 800D7044 10200003 */  beq $at, $zero, 0x800D7054
    /* 800D7048 38590020 */  xori $t9, $v0, 0x0020
    /* 800D704C 10000002 */  beq $zero, $zero, 0x800D7058
    /* 800D7050 A2190000 */  sb $t9, 0($s0)
    /* 800D7054 A2020000 */  sb $v0, 0($s0)
    /* 800D7058 92020001 */  lbu $v0, 1($s0)
    /* 800D705C 26100001 */  addiu $s0, $s0, 1
    /* 800D7060 5440FFF6 */  .word 0x5440FFF6
    /* 800D7064 28410061 */  slti $at, $v0, 97
    /* 800D7068 0C02CFE9 */  jal 0x800B3FA4
    /* 800D706C 2405FFFF */  addiu $a1, $zero, -1
    /* 800D7070 0282082B */  sltu $at, $s4, $v0
    /* 800D7074 10200004 */  beq $at, $zero, 0x800D7088
    /* 800D7078 02402025 */  or $a0, $s2, $zero
    /* 800D707C 0C02CFE9 */  jal 0x800B3FA4
    /* 800D7080 2405FFFF */  addiu $a1, $zero, -1
    /* 800D7084 0040A025 */  or $s4, $v0, $zero
    /* 800D7088 26310001 */  addiu $s1, $s1, 1
    /* 800D708C 5636FFC9 */  .word 0x5636FFC9
    /* 800D7090 2A21000C */  slti $at, $s1, 12
    /* 800D7094 1000002A */  beq $zero, $zero, 0x800D7140
    /* 800D7098 8FAF0084 */  lw $t7, 132($sp)
    /* 800D709C 8E78000C */  lw $t8, 12($s3)
    /* 800D70A0 8E6F0010 */  lw $t7, 16($s3)
    /* 800D70A4 2405FFFF */  addiu $a1, $zero, -1
    /* 800D70A8 970E0050 */  lhu $t6, 80($t8)
    /* 800D70AC 000EC880 */  sll $t9, $t6, 2
    /* 800D70B0 01F9C021 */  addu $t8, $t7, $t9
    /* 800D70B4 0C02CFE9 */  jal 0x800B3FA4
    /* 800D70B8 8F040000 */  lw $a0, 0($t8)
    /* 800D70BC 0282082B */  sltu $at, $s4, $v0
    /* 800D70C0 5020000B */  .word 0x5020000B
    /* 800D70C4 8E6E000C */  lw $t6, 12($s3)
    /* 800D70C8 8E6F000C */  lw $t7, 12($s3)
    /* 800D70CC 8E6E0010 */  lw $t6, 16($s3)
    /* 800D70D0 2405FFFF */  addiu $a1, $zero, -1
    /* 800D70D4 95F90050 */  lhu $t9, 80($t7)
    /* 800D70D8 0019C080 */  sll $t8, $t9, 2
    /* 800D70DC 01D87821 */  addu $t7, $t6, $t8
    /* 800D70E0 0C02CFE9 */  jal 0x800B3FA4
    /* 800D70E4 8DE40000 */  lw $a0, 0($t7)
    /* 800D70E8 0040A025 */  or $s4, $v0, $zero
    /* 800D70EC 8E6E000C */  lw $t6, 12($s3)
    /* 800D70F0 8E790010 */  lw $t9, 16($s3)
    /* 800D70F4 2405FFFF */  addiu $a1, $zero, -1
    /* 800D70F8 95D80050 */  lhu $t8, 80($t6)
    /* 800D70FC 00187880 */  sll $t7, $t8, 2
    /* 800D7100 032F7021 */  addu $t6, $t9, $t7
    /* 800D7104 0C02CFE9 */  jal 0x800B3FA4
    /* 800D7108 8DC40004 */  lw $a0, 4($t6)
    /* 800D710C 0282082B */  sltu $at, $s4, $v0
    /* 800D7110 5020000B */  .word 0x5020000B
    /* 800D7114 8FAF0084 */  lw $t7, 132($sp)
    /* 800D7118 8E79000C */  lw $t9, 12($s3)
    /* 800D711C 8E780010 */  lw $t8, 16($s3)
    /* 800D7120 2405FFFF */  addiu $a1, $zero, -1
    /* 800D7124 972F0050 */  lhu $t7, 80($t9)
    /* 800D7128 000F7080 */  sll $t6, $t7, 2
    /* 800D712C 030EC821 */  addu $t9, $t8, $t6
    /* 800D7130 0C02CFE9 */  jal 0x800B3FA4
    /* 800D7134 8F240004 */  lw $a0, 4($t9)
    /* 800D7138 0040A025 */  or $s4, $v0, $zero
    /* 800D713C 8FAF0084 */  lw $t7, 132($sp)
    /* 800D7140 8FAE008C */  lw $t6, 140($sp)
    /* 800D7144 24010004 */  addiu $at, $zero, 4
    /* 800D7148 25F80001 */  addiu $t8, $t7, 1
    /* 800D714C 8FAF0020 */  lw $t7, 32($sp)
    /* 800D7150 AFB80084 */  sw $t8, 132($sp)
    /* 800D7154 25D90001 */  addiu $t9, $t6, 1
    /* 800D7158 25F80004 */  addiu $t8, $t7, 4
    /* 800D715C AFB80020 */  sw $t8, 32($sp)
    /* 800D7160 1721FF62 */  bne $t9, $at, 0x800D6EEC
    /* 800D7164 AFB9008C */  sw $t9, 140($sp)
    /* 800D7168 8FAE0080 */  lw $t6, 128($sp)
    /* 800D716C 3C108011 */  lui $s0, 0x8011
    /* 800D7170 26100650 */  addiu $s0, $s0, 1616
    /* 800D7174 01D47821 */  addu $t7, $t6, $s4
    /* 800D7178 25F8000A */  addiu $t8, $t7, 10
    /* 800D717C 0C02CFD4 */  jal 0x800B3F50
    /* 800D7180 AE180004 */  sw $t8, 4($s0)
    /* 800D7184 8FB90084 */  lw $t9, 132($sp)
    /* 800D7188 8E0F0004 */  lw $t7, 4($s0)
    /* 800D718C 8FBF0014 */  lw $ra, 20($sp)
    /* 800D7190 00590019 */  multu $v0, $t9
    /* 800D7194 241900A0 */  addiu $t9, $zero, 160
    /* 800D7198 00007012 */  mflo $t6
    /* 800D719C AE0E0008 */  sw $t6, 8($s0)
    /* 800D71A0 05E10003 */  bgez $t7, 0x800D71B0
    /* 800D71A4 000FC043 */  sra $t8, $t7, 1
    /* 800D71A8 25E10001 */  addiu $at, $t7, 1
    /* 800D71AC 0001C043 */  sra $t8, $at, 1
    /* 800D71B0 03387023 */  subu $t6, $t9, $t8
    /* 800D71B4 8FB90080 */  lw $t9, 128($sp)
    /* 800D71B8 AE0E0000 */  sw $t6, 0($s0)
    /* 800D71BC 27BD0090 */  addiu $sp, $sp, 144
    /* 800D71C0 01D9C021 */  addu $t8, $t6, $t9
    /* 800D71C4 270E0005 */  addiu $t6, $t8, 5
    /* 800D71C8 03E00008 */  jr $ra
    /* 800D71CC AE0E000C */  sw $t6, 12($s0)
