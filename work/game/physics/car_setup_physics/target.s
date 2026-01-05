# Source: game_code.bin (decompressed)
# Address: 0x800A78C8

glabel car_setup_physics
    /* 800A78C8 27BDFFC8 */  addiu $sp, $sp, -56
    /* 800A78CC AFBF0024 */  sw $ra, 36($sp)
    /* 800A78D0 AFA60040 */  sw $a2, 64($sp)
    /* 800A78D4 00805025 */  or $t2, $a0, $zero
    /* 800A78D8 00A05825 */  or $t3, $a1, $zero
    /* 800A78DC 30C8FFFF */  andi $t0, $a2, 0xFFFF
    /* 800A78E0 00E06025 */  or $t4, $a3, $zero
    /* 800A78E4 1860000A */  blez $v1, 0x800A7910
    /* 800A78E8 00001025 */  or $v0, $zero, $zero
    /* 800A78EC 3C048016 */  lui $a0, 0x8016
    /* 800A78F0 2484B268 */  addiu $a0, $a0, -19864
    /* 800A78F4 848E0000 */  lh $t6, 0($a0)
    /* 800A78F8 51C00006 */  .word 0x51C00006
    /* 800A78FC 284100C8 */  slti $at, $v0, 200
    /* 800A7900 24420001 */  addiu $v0, $v0, 1
    /* 800A7904 0043082A */  slt $at, $v0, $v1
    /* 800A7908 1420FFFA */  bne $at, $zero, 0x800A78F4
    /* 800A790C 24840058 */  addiu $a0, $a0, 88
    /* 800A7910 284100C8 */  slti $at, $v0, 200
    /* 800A7914 14200003 */  bne $at, $zero, 0x800A7924
    /* 800A7918 00027880 */  sll $t7, $v0, 2
    /* 800A791C 10000022 */  beq $zero, $zero, 0x800A79A8
    /* 800A7920 00001025 */  or $v0, $zero, $zero
    /* 800A7924 01E27823 */  subu $t7, $t7, $v0
    /* 800A7928 000F7880 */  sll $t7, $t7, 2
    /* 800A792C 01E27823 */  subu $t7, $t7, $v0
    /* 800A7930 3C188016 */  lui $t8, 0x8016
    /* 800A7934 2718B268 */  addiu $t8, $t8, -19864
    /* 800A7938 000F78C0 */  sll $t7, $t7, 3
    /* 800A793C 0043082A */  slt $at, $v0, $v1
    /* 800A7940 14200003 */  bne $at, $zero, 0x800A7950
    /* 800A7944 01F82021 */  addu $a0, $t7, $t8
    /* 800A7948 24430001 */  addiu $v1, $v0, 1
    /* 800A794C AD230000 */  sw $v1, 0($t1)
    /* 800A7950 3C028014 */  lui $v0, 0x8014
    /* 800A7954 2442269C */  addiu $v0, $v0, 9884
    /* 800A7958 8C4D0000 */  lw $t5, 0($v0)
    /* 800A795C 01402825 */  or $a1, $t2, $zero
    /* 800A7960 01603025 */  or $a2, $t3, $zero
    /* 800A7964 01A3082A */  slt $at, $t5, $v1
    /* 800A7968 10200002 */  beq $at, $zero, 0x800A7974
    /* 800A796C 3107FFFF */  andi $a3, $t0, 0xFFFF
    /* 800A7970 AC430000 */  sw $v1, 0($v0)
    /* 800A7974 8FA2004C */  lw $v0, 76($sp)
    /* 800A7978 97AE004A */  lhu $t6, 74($sp)
    /* 800A797C 10400004 */  beq $v0, $zero, 0x800A7990
    /* 800A7980 A48E0002 */  sh $t6, 2($a0)
    /* 800A7984 35D80400 */  ori $t8, $t6, 0x0400
    /* 800A7988 A4980002 */  sh $t8, 2($a0)
    /* 800A798C A4880004 */  sh $t0, 4($a0)
    /* 800A7990 AFAC0010 */  sw $t4, 16($sp)
    /* 800A7994 AFA00014 */  sw $zero, 20($sp)
    /* 800A7998 AFA20018 */  sw $v0, 24($sp)
    /* 800A799C 0C02301D */  jal 0x8008C074
    /* 800A79A0 AFA40028 */  sw $a0, 40($sp)
    /* 800A79A4 8FA20028 */  lw $v0, 40($sp)
    /* 800A79A8 8FBF0024 */  lw $ra, 36($sp)
    /* 800A79AC 27BD0038 */  addiu $sp, $sp, 56
    /* 800A79B0 03E00008 */  jr $ra
    /* 800A79B4 00000000 */  nop
