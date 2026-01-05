# Source: game_code.bin (decompressed)
# Address: 0x80094A54

glabel audio_channel_priority
    /* 80094A54 27BDFFE0 */  addiu $sp, $sp, -32
    /* 80094A58 AFB5001C */  sw $s5, 28($sp)
    /* 80094A5C AFB40018 */  sw $s4, 24($sp)
    /* 80094A60 AFB30014 */  sw $s3, 20($sp)
    /* 80094A64 AFB20010 */  sw $s2, 16($sp)
    /* 80094A68 3C058015 */  lui $a1, 0x8015
    /* 80094A6C 44800000 */  .word 0x44800000
    /* 80094A70 AFB1000C */  sw $s1, 12($sp)
    /* 80094A74 AFB00008 */  sw $s0, 8($sp)
    /* 80094A78 00804025 */  or $t0, $a0, $zero
    /* 80094A7C 24A51690 */  addiu $a1, $a1, 5776
    /* 80094A80 24120004 */  addiu $s2, $zero, 4
    /* 80094A84 2413FFFF */  addiu $s3, $zero, -1
    /* 80094A88 2414003C */  addiu $s4, $zero, 60
    /* 80094A8C 2415000C */  addiu $s5, $zero, 12
    /* 80094A90 00001025 */  or $v0, $zero, $zero
    /* 80094A94 00002025 */  or $a0, $zero, $zero
    /* 80094A98 00A03025 */  or $a2, $a1, $zero
    /* 80094A9C 00003825 */  or $a3, $zero, $zero
    /* 80094AA0 00C08825 */  or $s1, $a2, $zero
    /* 80094AA4 8E2E0000 */  lw $t6, 0($s1)
    /* 80094AA8 00027880 */  sll $t7, $v0, 2
    /* 80094AAC 01E27823 */  subu $t7, $t7, $v0
    /* 80094AB0 150E004B */  bne $t0, $t6, 0x80094BE0
    /* 80094AB4 00E04825 */  or $t1, $a3, $zero
    /* 80094AB8 000F7940 */  sll $t7, $t7, 5
    /* 80094ABC 01E4C021 */  addu $t8, $t7, $a0
    /* 80094AC0 3C0E8015 */  lui $t6, 0x8015
    /* 80094AC4 25CE0F88 */  addiu $t6, $t6, 3976
    /* 80094AC8 27190004 */  addiu $t9, $t8, 4
    /* 80094ACC 28E10004 */  slti $at, $a3, 4
    /* 80094AD0 1020003C */  beq $at, $zero, 0x80094BC4
    /* 80094AD4 032E1821 */  addu $v1, $t9, $t6
    /* 80094AD8 02478023 */  subu $s0, $s2, $a3
    /* 80094ADC 320F0003 */  andi $t7, $s0, 0x0003
    /* 80094AE0 11E0000D */  beq $t7, $zero, 0x80094B18
    /* 80094AE4 01E75821 */  addu $t3, $t7, $a3
    /* 80094AE8 00095080 */  sll $t2, $t1, 2
    /* 80094AEC 006A6021 */  addu $t4, $v1, $t2
    /* 80094AF0 00CA6821 */  addu $t5, $a2, $t2
    /* 80094AF4 C5840004 */  lwc1 $f4, 4($t4)
    /* 80094AF8 25290001 */  addiu $t1, $t1, 1
    /* 80094AFC 258C0004 */  addiu $t4, $t4, 4
    /* 80094B00 E584FFFC */  swc1 $f4, -4($t4)
    /* 80094B04 8DB80004 */  lw $t8, 4($t5)
    /* 80094B08 25AD0004 */  addiu $t5, $t5, 4
    /* 80094B0C 1569FFF9 */  bne $t3, $t1, 0x80094AF4
    /* 80094B10 ADB8FFFC */  sw $t8, -4($t5)
    /* 80094B14 1132002B */  beq $t1, $s2, 0x80094BC4
    /* 80094B18 00095080 */  sll $t2, $t1, 2
    /* 80094B1C 006A6021 */  addu $t4, $v1, $t2
    /* 80094B20 00CA6821 */  addu $t5, $a2, $t2
    /* 80094B24 C5920004 */  lwc1 $f18, 4($t4)
    /* 80094B28 25290004 */  addiu $t1, $t1, 4
    /* 80094B2C E5920000 */  swc1 $f18, 0($t4)
    /* 80094B30 11320015 */  beq $t1, $s2, 0x80094B88
    /* 80094B34 8DB90004 */  lw $t9, 4($t5)
    /* 80094B38 ADB90000 */  sw $t9, 0($t5)
    /* 80094B3C C5920008 */  lwc1 $f18, 8($t4)
    /* 80094B40 25290004 */  addiu $t1, $t1, 4
    /* 80094B44 258C0010 */  addiu $t4, $t4, 16
    /* 80094B48 E592FFF4 */  swc1 $f18, -12($t4)
    /* 80094B4C 8DAE0008 */  lw $t6, 8($t5)
    /* 80094B50 25AD0010 */  addiu $t5, $t5, 16
    /* 80094B54 ADAEFFF4 */  sw $t6, -12($t5)
    /* 80094B58 C592FFFC */  lwc1 $f18, -4($t4)
    /* 80094B5C E592FFF8 */  swc1 $f18, -8($t4)
    /* 80094B60 8DAFFFFC */  lw $t7, -4($t5)
    /* 80094B64 ADAFFFF8 */  sw $t7, -8($t5)
    /* 80094B68 C5920000 */  lwc1 $f18, 0($t4)
    /* 80094B6C E592FFFC */  swc1 $f18, -4($t4)
    /* 80094B70 8DB80000 */  lw $t8, 0($t5)
    /* 80094B74 ADB8FFFC */  sw $t8, -4($t5)
    /* 80094B78 C5920004 */  lwc1 $f18, 4($t4)
    /* 80094B7C E5920000 */  swc1 $f18, 0($t4)
    /* 80094B80 1532FFED */  bne $t1, $s2, 0x80094B38
    /* 80094B84 8DB90004 */  lw $t9, 4($t5)
    /* 80094B88 ADB90000 */  sw $t9, 0($t5)
    /* 80094B8C C5920008 */  lwc1 $f18, 8($t4)
    /* 80094B90 258C0010 */  addiu $t4, $t4, 16
    /* 80094B94 25AD0010 */  addiu $t5, $t5, 16
    /* 80094B98 E592FFF4 */  swc1 $f18, -12($t4)
    /* 80094B9C 8DAEFFF8 */  lw $t6, -8($t5)
    /* 80094BA0 ADAEFFF4 */  sw $t6, -12($t5)
    /* 80094BA4 C592FFFC */  lwc1 $f18, -4($t4)
    /* 80094BA8 E592FFF8 */  swc1 $f18, -8($t4)
    /* 80094BAC 8DAFFFFC */  lw $t7, -4($t5)
    /* 80094BB0 ADAFFFF8 */  sw $t7, -8($t5)
    /* 80094BB4 C5920000 */  lwc1 $f18, 0($t4)
    /* 80094BB8 E592FFFC */  swc1 $f18, -4($t4)
    /* 80094BBC 8DB80000 */  lw $t8, 0($t5)
    /* 80094BC0 ADB8FFFC */  sw $t8, -4($t5)
    /* 80094BC4 00095080 */  sll $t2, $t1, 2
    /* 80094BC8 006AC821 */  addu $t9, $v1, $t2
    /* 80094BCC E7200000 */  swc1 $f0, 0($t9)
    /* 80094BD0 00CA7021 */  addu $t6, $a2, $t2
    /* 80094BD4 ADD30000 */  sw $s3, 0($t6)
    /* 80094BD8 24E7FFFF */  addiu $a3, $a3, -1
    /* 80094BDC 2631FFFC */  addiu $s1, $s1, -4
    /* 80094BE0 24E70001 */  addiu $a3, $a3, 1
    /* 80094BE4 28E10005 */  slti $at, $a3, 5
    /* 80094BE8 1420FFAE */  bne $at, $zero, 0x80094AA4
    /* 80094BEC 26310004 */  addiu $s1, $s1, 4
    /* 80094BF0 24840014 */  addiu $a0, $a0, 20
    /* 80094BF4 1494FFA9 */  bne $a0, $s4, 0x80094A9C
    /* 80094BF8 24C60014 */  addiu $a2, $a2, 20
    /* 80094BFC 24420001 */  addiu $v0, $v0, 1
    /* 80094C00 1455FFA4 */  bne $v0, $s5, 0x80094A94
    /* 80094C04 24A5003C */  addiu $a1, $a1, 60
    /* 80094C08 8FB00008 */  lw $s0, 8($sp)
    /* 80094C0C 8FB1000C */  lw $s1, 12($sp)
    /* 80094C10 8FB20010 */  lw $s2, 16($sp)
    /* 80094C14 8FB30014 */  lw $s3, 20($sp)
    /* 80094C18 8FB40018 */  lw $s4, 24($sp)
    /* 80094C1C 8FB5001C */  lw $s5, 28($sp)
    /* 80094C20 03E00008 */  jr $ra
    /* 80094C24 27BD0020 */  addiu $sp, $sp, 32
