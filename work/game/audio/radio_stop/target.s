# Source: game_code.bin (decompressed)
# Address: 0x800B1B90

glabel radio_stop
    /* 800B1B90 00122400 */  sll $a0, $s2, 16
    /* 800B1B94 0004C403 */  sra $t8, $a0, 16
    /* 800B1B98 11E00008 */  beq $t7, $zero, 0x800B1BBC
    /* 800B1B9C 03002025 */  or $a0, $t8, $zero
    /* 800B1BA0 2405000F */  addiu $a1, $zero, 15
    /* 800B1BA4 0C02C56D */  jal 0x800B15B4
    /* 800B1BA8 24060001 */  addiu $a2, $zero, 1
    /* 800B1BAC 00122400 */  sll $a0, $s2, 16
    /* 800B1BB0 0004CC03 */  sra $t9, $a0, 16
    /* 800B1BB4 0C02C312 */  jal 0x800B0C48
    /* 800B1BB8 03202025 */  or $a0, $t9, $zero
    /* 800B1BBC 26520001 */  addiu $s2, $s2, 1
    /* 800B1BC0 00124400 */  sll $t0, $s2, 16
    /* 800B1BC4 00089403 */  sra $s2, $t0, 16
    /* 800B1BC8 2A410006 */  slti $at, $s2, 6
    /* 800B1BCC 5420FFEB */  .word 0x5420FFEB
    /* 800B1BD0 00127200 */  sll $t6, $s2, 8
    /* 800B1BD4 3C178016 */  lui $s7, 0x8016
    /* 800B1BD8 3C148016 */  lui $s4, 0x8016
    /* 800B1BDC 3C138012 */  lui $s3, 0x8012
    /* 800B1BE0 2673AD90 */  addiu $s3, $s3, -21104
    /* 800B1BE4 26941368 */  addiu $s4, $s4, 4968
    /* 800B1BE8 26F7B268 */  addiu $s7, $s7, -19864
    /* 800B1BEC 00009025 */  or $s2, $zero, $zero
    /* 800B1BF0 241E0058 */  addiu $fp, $zero, 88
    /* 800B1BF4 24160006 */  addiu $s6, $zero, 6
    /* 800B1BF8 27B50060 */  addiu $s5, $sp, 96
    /* 800B1BFC 00125200 */  sll $t2, $s2, 8
    /* 800B1C00 01525021 */  addu $t2, $t2, $s2
    /* 800B1C04 000A50C0 */  sll $t2, $t2, 3
    /* 800B1C08 3C0B8015 */  lui $t3, 0x8015
    /* 800B1C0C 016A5821 */  addu $t3, $t3, $t2
    /* 800B1C10 856BAA18 */  lh $t3, -21992($t3)
    /* 800B1C14 116000B3 */  beq $t3, $zero, 0x800B1EE4
    /* 800B1C18 00126100 */  sll $t4, $s2, 4
    /* 800B1C1C 01926023 */  subu $t4, $t4, $s2
    /* 800B1C20 000C60C0 */  sll $t4, $t4, 3
    /* 800B1C24 01926023 */  subu $t4, $t4, $s2
    /* 800B1C28 3C0D8015 */  lui $t5, 0x8015
    /* 800B1C2C 25AD2818 */  addiu $t5, $t5, 10264
    /* 800B1C30 000C60C0 */  sll $t4, $t4, 3
    /* 800B1C34 018D1821 */  addu $v1, $t4, $t5
    /* 800B1C38 3C0E8009 */  lui $t6, 0x8009
    /* 800B1C3C 25CED120 */  addiu $t6, $t6, -12000
    /* 800B1C40 AC6E0214 */  sw $t6, 532($v1)
    /* 800B1C44 A4720208 */  sh $s2, 520($v1)
    /* 800B1C48 3C018015 */  lui $at, 0x8015
    /* 800B1C4C C42443CC */  lwc1 $f4, 17356($at)
    /* 800B1C50 3C188011 */  lui $t8, 0x8011
    /* 800B1C54 3C018012 */  lui $at, 0x8012
    /* 800B1C58 E4640210 */  swc1 $f4, 528($v1)
    /* 800B1C5C 8F1874B4 */  lw $t8, 29876($t8)
    /* 800B1C60 240F00C0 */  addiu $t7, $zero, 192
    /* 800B1C64 A02FAD8F */  sb $t7, -21105($at)
    /* 800B1C68 33190100 */  andi $t9, $t8, 0x0100
    /* 800B1C6C 13200004 */  beq $t9, $zero, 0x800B1C80
    /* 800B1C70 24040004 */  addiu $a0, $zero, 4
    /* 800B1C74 24020001 */  addiu $v0, $zero, 1
    /* 800B1C78 10000003 */  beq $zero, $zero, 0x800B1C88
    /* 800B1C7C AFA3004C */  sw $v1, 76($sp)
    /* 800B1C80 2402000F */  addiu $v0, $zero, 15
    /* 800B1C84 AFA3004C */  sw $v1, 76($sp)
    /* 800B1C88 34480200 */  ori $t0, $v0, 0x0200
    /* 800B1C8C 35098000 */  ori $t1, $t0, 0x8000
    /* 800B1C90 3C078012 */  lui $a3, 0x8012
    /* 800B1C94 240A0001 */  addiu $t2, $zero, 1
    /* 800B1C98 AFAA0014 */  sw $t2, 20($sp)
    /* 800B1C9C 24E7AD8C */  addiu $a3, $a3, -21108
    /* 800B1CA0 AFA90010 */  sw $t1, 16($sp)
    /* 800B1CA4 02602825 */  or $a1, $s3, $zero
    /* 800B1CA8 0C029E2F */  jal 0x800A78BC
    /* 800B1CAC 9686000C */  lhu $a2, 12($s4)
    /* 800B1CB0 00575823 */  subu $t3, $v0, $s7
    /* 800B1CB4 017E001A */  div $t3, $fp
    /* 800B1CB8 8FA3004C */  lw $v1, 76($sp)
    /* 800B1CBC 00006012 */  mflo $t4
    /* 800B1CC0 3C0D8015 */  lui $t5, 0x8015
    /* 800B1CC4 17C00002 */  bne $fp, $zero, 0x800B1CD0
    /* 800B1CC8 00000000 */  nop
    /* 800B1CCC 0007000D */  .word 0x0007000D
    /* 800B1CD0 2401FFFF */  addiu $at, $zero, -1
    /* 800B1CD4 17C10004 */  bne $fp, $at, 0x800B1CE8
    /* 800B1CD8 3C018000 */  lui $at, 0x8000
    /* 800B1CDC 15610002 */  bne $t3, $at, 0x800B1CE8
    /* 800B1CE0 00000000 */  nop
    /* 800B1CE4 0006000D */  .word 0x0006000D
    /* 800B1CE8 A46C0206 */  sh $t4, 518($v1)
    /* 800B1CEC 85ADA108 */  lh $t5, -24312($t5)
    /* 800B1CF0 001270C0 */  sll $t6, $s2, 3
    /* 800B1CF4 3C0F8015 */  lui $t7, 0x8015
    /* 800B1CF8 29A10002 */  slti $at, $t5, 2
    /* 800B1CFC 14200079 */  bne $at, $zero, 0x800B1EE4
    /* 800B1D00 01EE7821 */  addu $t7, $t7, $t6
    /* 800B1D04 91EF3E8F */  lbu $t7, 16015($t7)
    /* 800B1D08 3C188012 */  lui $t8, 0x8012
    /* 800B1D0C 2718B574 */  addiu $t8, $t8, -19084
    /* 800B1D10 56CF0075 */  .word 0x56CF0075
    /* 800B1D14 26520001 */  addiu $s2, $s2, 1
    /* 800B1D18 A7B20072 */  sh $s2, 114($sp)
    /* 800B1D1C 8F010000 */  lw $at, 0($t8)
    /* 800B1D20 3C0A8015 */  lui $t2, 0x8015
    /* 800B1D24 3C0B8013 */  lui $t3, 0x8013
    /* 800B1D28 AEA10000 */  sw $at, 0($s5)
    /* 800B1D2C 8D4AA110 */  lw $t2, -24304($t2)
    /* 800B1D30 24700218 */  addiu $s0, $v1, 536
    /* 800B1D34 00008825 */  or $s1, $zero, $zero
    /* 800B1D38 16CA000F */  bne $s6, $t2, 0x800B1D78
    /* 800B1D3C 01725821 */  addu $t3, $t3, $s2
    /* 800B1D40 816BE67C */  lb $t3, -6532($t3)
    /* 800B1D44 3C0D8012 */  lui $t5, 0x8012
    /* 800B1D48 25ADB558 */  addiu $t5, $t5, -19112
    /* 800B1D4C 000B6080 */  sll $t4, $t3, 2
    /* 800B1D50 018D1021 */  addu $v0, $t4, $t5
    /* 800B1D54 904E0000 */  lbu $t6, 0($v0)
    /* 800B1D58 904F0001 */  lbu $t7, 1($v0)
    /* 800B1D5C 90480002 */  lbu $t0, 2($v0)
    /* 800B1D60 90590003 */  lbu $t9, 3($v0)
    /* 800B1D64 A3AE0060 */  sb $t6, 96($sp)
    /* 800B1D68 A3AF0061 */  sb $t7, 97($sp)
    /* 800B1D6C A3A80062 */  sb $t0, 98($sp)
    /* 800B1D70 1000000D */  beq $zero, $zero, 0x800B1DA8
    /* 800B1D74 A3B90063 */  sb $t9, 99($sp)
    /* 800B1D78 3C098012 */  lui $t1, 0x8012
    /* 800B1D7C 2529B558 */  addiu $t1, $t1, -19112
    /* 800B1D80 0012C080 */  sll $t8, $s2, 2
    /* 800B1D84 03091021 */  addu $v0, $t8, $t1
    /* 800B1D88 904A0000 */  lbu $t2, 0($v0)
    /* 800B1D8C 904B0001 */  lbu $t3, 1($v0)
    /* 800B1D90 904C0002 */  lbu $t4, 2($v0)
    /* 800B1D94 904D0003 */  lbu $t5, 3($v0)
    /* 800B1D98 A3AA0060 */  sb $t2, 96($sp)
    /* 800B1D9C A3AB0061 */  sb $t3, 97($sp)
    /* 800B1DA0 A3AC0062 */  sb $t4, 98($sp)
    /* 800B1DA4 A3AD0063 */  sb $t5, 99($sp)
    /* 800B1DA8 3C038015 */  lui $v1, 0x8015
    /* 800B1DAC 84631AD0 */  lh $v1, 6864($v1)
    /* 800B1DB0 58600039 */  .word 0x58600039
    /* 800B1DB4 8FB9004C */  lw $t9, 76($sp)
    /* 800B1DB8 12510032 */  beq $s2, $s1, 0x800B1E84
    /* 800B1DBC 3C0E8009 */  lui $t6, 0x8009
    /* 800B1DC0 25CEC884 */  addiu $t6, $t6, -14204
    /* 800B1DC4 AE0E0014 */  sw $t6, 20($s0)
    /* 800B1DC8 A6120008 */  sh $s2, 8($s0)
    /* 800B1DCC A6110004 */  sh $s1, 4($s0)
    /* 800B1DD0 3C0F8015 */  lui $t7, 0x8015
    /* 800B1DD4 8DEFA110 */  lw $t7, -24304($t7)
    /* 800B1DD8 24040004 */  addiu $a0, $zero, 4
    /* 800B1DDC 02602825 */  or $a1, $s3, $zero
    /* 800B1DE0 16CF000E */  bne $s6, $t7, 0x800B1E1C
    /* 800B1DE4 02A03825 */  or $a3, $s5, $zero
    /* 800B1DE8 24080001 */  addiu $t0, $zero, 1
    /* 800B1DEC 0228C804 */  .word 0x0228C804
    /* 800B1DF0 37383200 */  ori $t8, $t9, 0x3200
    /* 800B1DF4 24090001 */  addiu $t1, $zero, 1
    /* 800B1DF8 AFA90014 */  sw $t1, 20($sp)
    /* 800B1DFC AFB80010 */  sw $t8, 16($sp)
    /* 800B1E00 24040004 */  addiu $a0, $zero, 4
    /* 800B1E04 02602825 */  or $a1, $s3, $zero
    /* 800B1E08 9686000E */  lhu $a2, 14($s4)
    /* 800B1E0C 0C029E2F */  jal 0x800A78BC
    /* 800B1E10 02A03825 */  or $a3, $s5, $zero
    /* 800B1E14 1000000A */  beq $zero, $zero, 0x800B1E40
    /* 800B1E18 00577823 */  subu $t7, $v0, $s7
    /* 800B1E1C 240A0001 */  addiu $t2, $zero, 1
    /* 800B1E20 022A5804 */  .word 0x022A5804
    /* 800B1E24 356C1200 */  ori $t4, $t3, 0x1200
    /* 800B1E28 240D0001 */  addiu $t5, $zero, 1
    /* 800B1E2C AFAD0014 */  sw $t5, 20($sp)
    /* 800B1E30 AFAC0010 */  sw $t4, 16($sp)
    /* 800B1E34 0C029E2F */  jal 0x800A78BC
    /* 800B1E38 9686000E */  lhu $a2, 14($s4)
    /* 800B1E3C 00577823 */  subu $t7, $v0, $s7
    /* 800B1E40 01FE001A */  div $t7, $fp
    /* 800B1E44 240E0001 */  addiu $t6, $zero, 1
    /* 800B1E48 A44E0006 */  sh $t6, 6($v0)
    /* 800B1E4C 00004012 */  mflo $t0
    /* 800B1E50 3C038015 */  lui $v1, 0x8015
    /* 800B1E54 A6080006 */  sh $t0, 6($s0)
    /* 800B1E58 84631AD0 */  lh $v1, 6864($v1)
    /* 800B1E5C 17C00002 */  bne $fp, $zero, 0x800B1E68
    /* 800B1E60 00000000 */  nop
    /* 800B1E64 0007000D */  .word 0x0007000D
    /* 800B1E68 2401FFFF */  addiu $at, $zero, -1
    /* 800B1E6C 17C10004 */  bne $fp, $at, 0x800B1E80
    /* 800B1E70 3C018000 */  lui $at, 0x8000
    /* 800B1E74 15E10002 */  bne $t7, $at, 0x800B1E80
    /* 800B1E78 00000000 */  nop
    /* 800B1E7C 0006000D */  .word 0x0006000D
    /* 800B1E80 26100018 */  addiu $s0, $s0, 24
    /* 800B1E84 26310001 */  addiu $s1, $s1, 1
    /* 800B1E88 0223082A */  slt $at, $s1, $v1
    /* 800B1E8C 1420FFCA */  bne $at, $zero, 0x800B1DB8
    /* 800B1E90 00000000 */  nop
    /* 800B1E94 8FB9004C */  lw $t9, 76($sp)
    /* 800B1E98 87A90072 */  lh $t1, 114($sp)
    /* 800B1E9C 3C0C8015 */  lui $t4, 0x8015
    /* 800B1EA0 27380248 */  addiu $t8, $t9, 584
    /* 800B1EA4 0218082B */  sltu $at, $s0, $t8
    /* 800B1EA8 1020000E */  beq $at, $zero, 0x800B1EE4
    /* 800B1EAC 00095100 */  sll $t2, $t1, 4
    /* 800B1EB0 01495023 */  subu $t2, $t2, $t1
    /* 800B1EB4 000A50C0 */  sll $t2, $t2, 3
    /* 800B1EB8 01495023 */  subu $t2, $t2, $t1
    /* 800B1EBC 000A50C0 */  sll $t2, $t2, 3
    /* 800B1EC0 254B0248 */  addiu $t3, $t2, 584
    /* 800B1EC4 258C2818 */  addiu $t4, $t4, 10264
    /* 800B1EC8 016C1021 */  addu $v0, $t3, $t4
    /* 800B1ECC 2403FFFF */  addiu $v1, $zero, -1
    /* 800B1ED0 26100018 */  addiu $s0, $s0, 24
    /* 800B1ED4 0202082B */  sltu $at, $s0, $v0
    /* 800B1ED8 AE00FFFC */  sw $zero, -4($s0)
    /* 800B1EDC 1420FFFC */  bne $at, $zero, 0x800B1ED0
    /* 800B1EE0 A603FFEE */  sh $v1, -18($s0)
    /* 800B1EE4 26520001 */  addiu $s2, $s2, 1
    /* 800B1EE8 00126C00 */  sll $t5, $s2, 16
    /* 800B1EEC 000D9403 */  sra $s2, $t5, 16
    /* 800B1EF0 2A410006 */  slti $at, $s2, 6
    /* 800B1EF4 1420FF41 */  bne $at, $zero, 0x800B1BFC
    /* 800B1EF8 8FBF0044 */  lw $ra, 68($sp)
    /* 800B1EFC 8FB00020 */  lw $s0, 32($sp)
    /* 800B1F00 8FB10024 */  lw $s1, 36($sp)
    /* 800B1F04 8FB20028 */  lw $s2, 40($sp)
    /* 800B1F08 8FB3002C */  lw $s3, 44($sp)
    /* 800B1F0C 8FB40030 */  lw $s4, 48($sp)
    /* 800B1F10 8FB50034 */  lw $s5, 52($sp)
    /* 800B1F14 8FB60038 */  lw $s6, 56($sp)
    /* 800B1F18 8FB7003C */  lw $s7, 60($sp)
    /* 800B1F1C 8FBE0040 */  lw $fp, 64($sp)
    /* 800B1F20 03E00008 */  jr $ra
    /* 800B1F24 27BD00A0 */  addiu $sp, $sp, 160
