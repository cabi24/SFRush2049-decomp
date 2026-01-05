# Source: game_code.bin (decompressed)
# Address: 0x800A8D9C

glabel car_collision_init
    /* 800A8D9C 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800A8DA0 AFBF0014 */  sw $ra, 20($sp)
    /* 800A8DA4 8C8316A4 */  lw $v1, 5796($a0)
    /* 800A8DA8 00A04825 */  or $t1, $a1, $zero
    /* 800A8DAC 00804025 */  or $t0, $a0, $zero
    /* 800A8DB0 2861000E */  slti $at, $v1, 14
    /* 800A8DB4 1420001E */  bne $at, $zero, 0x800A8E30
    /* 800A8DB8 01202825 */  or $a1, $t1, $zero
    /* 800A8DBC 948E16A0 */  lhu $t6, 5792($a0)
    /* 800A8DC0 00677804 */  .word 0x00677804
    /* 800A8DC4 01CFC025 */  or $t8, $t6, $t7
    /* 800A8DC8 A49816A0 */  sh $t8, 5792($a0)
    /* 800A8DCC 03007025 */  or $t6, $t8, $zero
    /* 800A8DD0 8C98000C */  lw $t8, 12($a0)
    /* 800A8DD4 8C8F0004 */  lw $t7, 4($a0)
    /* 800A8DD8 01F8C821 */  addu $t9, $t7, $t8
    /* 800A8DDC A32E0000 */  sb $t6, 0($t9)
    /* 800A8DE0 8C8F000C */  lw $t7, 12($a0)
    /* 800A8DE4 948E16A0 */  lhu $t6, 5792($a0)
    /* 800A8DE8 25F80001 */  addiu $t8, $t7, 1
    /* 800A8DEC 8C8F0004 */  lw $t7, 4($a0)
    /* 800A8DF0 000ECA03 */  sra $t9, $t6, 8
    /* 800A8DF4 AC98000C */  sw $t8, 12($a0)
    /* 800A8DF8 01F87021 */  addu $t6, $t7, $t8
    /* 800A8DFC A1D90000 */  sb $t9, 0($t6)
    /* 800A8E00 8C8F000C */  lw $t7, 12($a0)
    /* 800A8E04 8C8216A4 */  lw $v0, 5796($a0)
    /* 800A8E08 240E0010 */  addiu $t6, $zero, 16
    /* 800A8E0C 25F80001 */  addiu $t8, $t7, 1
    /* 800A8E10 01C27823 */  subu $t7, $t6, $v0
    /* 800A8E14 AC98000C */  sw $t8, 12($a0)
    /* 800A8E18 30F9FFFF */  andi $t9, $a3, 0xFFFF
    /* 800A8E1C 01F9C007 */  .word 0x01F9C007
    /* 800A8E20 244EFFF3 */  addiu $t6, $v0, -13
    /* 800A8E24 AC8E16A4 */  sw $t6, 5796($a0)
    /* 800A8E28 10000007 */  beq $zero, $zero, 0x800A8E48
    /* 800A8E2C A49816A0 */  sh $t8, 5792($a0)
    /* 800A8E30 951916A0 */  lhu $t9, 5792($t0)
    /* 800A8E34 00677804 */  .word 0x00677804
    /* 800A8E38 246E0003 */  addiu $t6, $v1, 3
    /* 800A8E3C 032FC025 */  or $t8, $t9, $t7
    /* 800A8E40 A51816A0 */  sh $t8, 5792($t0)
    /* 800A8E44 AD0E16A4 */  sw $t6, 5796($t0)
    /* 800A8E48 8D191694 */  lw $t9, 5780($t0)
    /* 800A8E4C 2401FFF8 */  addiu $at, $zero, -8
    /* 800A8E50 01002025 */  or $a0, $t0, $zero
    /* 800A8E54 272F000A */  addiu $t7, $t9, 10
    /* 800A8E58 01E1C024 */  and $t8, $t7, $at
    /* 800A8E5C 24D90004 */  addiu $t9, $a2, 4
    /* 800A8E60 AD181694 */  sw $t8, 5780($t0)
    /* 800A8E64 001978C0 */  sll $t7, $t9, 3
    /* 800A8E68 030FC021 */  addu $t8, $t8, $t7
    /* 800A8E6C 0C02A05D */  jal 0x800A8174
    /* 800A8E70 AD181694 */  sw $t8, 5780($t0)
    /* 800A8E74 8D0F0004 */  lw $t7, 4($t0)
    /* 800A8E78 8D18000C */  lw $t8, 12($t0)
    /* 800A8E7C 24190008 */  addiu $t9, $zero, 8
    /* 800A8E80 AD19169C */  sw $t9, 5788($t0)
    /* 800A8E84 00C01025 */  or $v0, $a2, $zero
    /* 800A8E88 01F8C821 */  addu $t9, $t7, $t8
    /* 800A8E8C A3220000 */  sb $v0, 0($t9)
    /* 800A8E90 8D0F000C */  lw $t7, 12($t0)
    /* 800A8E94 8D190004 */  lw $t9, 4($t0)
    /* 800A8E98 00027203 */  sra $t6, $v0, 8
    /* 800A8E9C 25F80001 */  addiu $t8, $t7, 1
    /* 800A8EA0 AD18000C */  sw $t8, 12($t0)
    /* 800A8EA4 0338C021 */  addu $t8, $t9, $t8
    /* 800A8EA8 A30E0000 */  sb $t6, 0($t8)
    /* 800A8EAC 8D19000C */  lw $t9, 12($t0)
    /* 800A8EB0 00C01827 */  nor $v1, $a2, $zero
    /* 800A8EB4 0003C203 */  sra $t8, $v1, 8
    /* 800A8EB8 272F0001 */  addiu $t7, $t9, 1
    /* 800A8EBC 8D190004 */  lw $t9, 4($t0)
    /* 800A8EC0 AD0F000C */  sw $t7, 12($t0)
    /* 800A8EC4 24C2FFFF */  addiu $v0, $a2, -1
    /* 800A8EC8 032F7021 */  addu $t6, $t9, $t7
    /* 800A8ECC A1C30000 */  sb $v1, 0($t6)
    /* 800A8ED0 8D19000C */  lw $t9, 12($t0)
    /* 800A8ED4 8D0E0004 */  lw $t6, 4($t0)
    /* 800A8ED8 272F0001 */  addiu $t7, $t9, 1
    /* 800A8EDC AD0F000C */  sw $t7, 12($t0)
    /* 800A8EE0 01CF7821 */  addu $t7, $t6, $t7
    /* 800A8EE4 A1F80000 */  sb $t8, 0($t7)
    /* 800A8EE8 8D0E000C */  lw $t6, 12($t0)
    /* 800A8EEC 25D90001 */  addiu $t9, $t6, 1
    /* 800A8EF0 10C0000D */  beq $a2, $zero, 0x800A8F28
    /* 800A8EF4 AD19000C */  sw $t9, 12($t0)
    /* 800A8EF8 8D0F0004 */  lw $t7, 4($t0)
    /* 800A8EFC 8D0E000C */  lw $t6, 12($t0)
    /* 800A8F00 90B80000 */  lbu $t8, 0($a1)
    /* 800A8F04 00401825 */  or $v1, $v0, $zero
    /* 800A8F08 01EEC821 */  addu $t9, $t7, $t6
    /* 800A8F0C A3380000 */  sb $t8, 0($t9)
    /* 800A8F10 8D0F000C */  lw $t7, 12($t0)
    /* 800A8F14 24A50001 */  addiu $a1, $a1, 1
    /* 800A8F18 25EE0001 */  addiu $t6, $t7, 1
    /* 800A8F1C AD0E000C */  sw $t6, 12($t0)
    /* 800A8F20 1440FFF5 */  bne $v0, $zero, 0x800A8EF8
    /* 800A8F24 2442FFFF */  addiu $v0, $v0, -1
    /* 800A8F28 8FBF0014 */  lw $ra, 20($sp)
    /* 800A8F2C 27BD0018 */  addiu $sp, $sp, 24
    /* 800A8F30 03E00008 */  jr $ra
    /* 800A8F34 00000000 */  nop
