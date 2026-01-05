# Source: game_code.bin (decompressed)
# Address: 0x800A7AE4

glabel car_mass_set
    /* 800A7AE4 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800A7AE8 AFA40018 */  sw $a0, 24($sp)
    /* 800A7AEC 00047400 */  sll $t6, $a0, 16
    /* 800A7AF0 000E2403 */  sra $a0, $t6, 16
    /* 800A7AF4 AFBF0014 */  sw $ra, 20($sp)
    /* 800A7AF8 AFA5001C */  sw $a1, 28($sp)
    /* 800A7AFC 0C02400B */  jal 0x8009002C
    /* 800A7B00 A7A4001A */  sh $a0, 26($sp)
    /* 800A7B04 2401FFFF */  addiu $at, $zero, -1
    /* 800A7B08 10410004 */  beq $v0, $at, 0x800A7B1C
    /* 800A7B0C 87A4001A */  lh $a0, 26($sp)
    /* 800A7B10 87B8001E */  lh $t8, 30($sp)
    /* 800A7B14 50580035 */  .word 0x50580035
    /* 800A7B18 8FBF0014 */  lw $ra, 20($sp)
    /* 800A7B1C 0440000C */  bltz $v0, 0x800A7B50
    /* 800A7B20 24070044 */  addiu $a3, $zero, 68
    /* 800A7B24 00870019 */  multu $a0, $a3
    /* 800A7B28 3C068013 */  lui $a2, 0x8013
    /* 800A7B2C 24C6E700 */  addiu $a2, $a2, -6400
    /* 800A7B30 0000C812 */  mflo $t9
    /* 800A7B34 00D94021 */  addu $t0, $a2, $t9
    /* 800A7B38 85030018 */  lh $v1, 24($t0)
    /* 800A7B3C 00470019 */  multu $v0, $a3
    /* 800A7B40 00004812 */  mflo $t1
    /* 800A7B44 00C95021 */  addu $t2, $a2, $t1
    /* 800A7B48 1000001D */  beq $zero, $zero, 0x800A7BC0
    /* 800A7B4C A5430016 */  sh $v1, 22($t2)
    /* 800A7B50 3C028016 */  lui $v0, 0x8016
    /* 800A7B54 2442B254 */  addiu $v0, $v0, -19884
    /* 800A7B58 844B0000 */  lh $t3, 0($v0)
    /* 800A7B5C 148B0009 */  bne $a0, $t3, 0x800A7B84
    /* 800A7B60 24070044 */  addiu $a3, $zero, 68
    /* 800A7B64 00870019 */  multu $a0, $a3
    /* 800A7B68 3C068013 */  lui $a2, 0x8013
    /* 800A7B6C 24C6E700 */  addiu $a2, $a2, -6400
    /* 800A7B70 00006012 */  mflo $t4
    /* 800A7B74 00CC6821 */  addu $t5, $a2, $t4
    /* 800A7B78 85AE0018 */  lh $t6, 24($t5)
    /* 800A7B7C 10000010 */  beq $zero, $zero, 0x800A7BC0
    /* 800A7B80 A44E0000 */  sh $t6, 0($v0)
    /* 800A7B84 0C023FF4 */  jal 0x8008FFD0
    /* 800A7B88 A7A4001A */  sh $a0, 26($sp)
    /* 800A7B8C 0440000C */  bltz $v0, 0x800A7BC0
    /* 800A7B90 87A4001A */  lh $a0, 26($sp)
    /* 800A7B94 24070044 */  addiu $a3, $zero, 68
    /* 800A7B98 00870019 */  multu $a0, $a3
    /* 800A7B9C 3C068013 */  lui $a2, 0x8013
    /* 800A7BA0 24C6E700 */  addiu $a2, $a2, -6400
    /* 800A7BA4 00007812 */  mflo $t7
    /* 800A7BA8 00CFC021 */  addu $t8, $a2, $t7
    /* 800A7BAC 87190018 */  lh $t9, 24($t8)
    /* 800A7BB0 00470019 */  multu $v0, $a3
    /* 800A7BB4 00004012 */  mflo $t0
    /* 800A7BB8 00C84821 */  addu $t1, $a2, $t0
    /* 800A7BBC A5390018 */  sh $t9, 24($t1)
    /* 800A7BC0 24070044 */  addiu $a3, $zero, 68
    /* 800A7BC4 00870019 */  multu $a0, $a3
    /* 800A7BC8 3C068013 */  lui $a2, 0x8013
    /* 800A7BCC 24C6E700 */  addiu $a2, $a2, -6400
    /* 800A7BD0 240AFFFF */  addiu $t2, $zero, -1
    /* 800A7BD4 00005812 */  mflo $t3
    /* 800A7BD8 00CB6021 */  addu $t4, $a2, $t3
    /* 800A7BDC A58A0018 */  sh $t2, 24($t4)
    /* 800A7BE0 0C023867 */  jal 0x8008E19C
    /* 800A7BE4 87A5001E */  lh $a1, 30($sp)
    /* 800A7BE8 8FBF0014 */  lw $ra, 20($sp)
    /* 800A7BEC 27BD0018 */  addiu $sp, $sp, 24
    /* 800A7BF0 03E00008 */  jr $ra
    /* 800A7BF4 00000000 */  nop
