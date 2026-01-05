# Source: game_code.bin (decompressed)
# Address: 0x800B34C0

glabel collision_force_check
    /* 800B34C0 3C048012 */  lui $a0, 0x8012
    /* 800B34C4 3C0F8014 */  lui $t7, 0x8014
    /* 800B34C8 15C00004 */  bne $t6, $zero, 0x800B34DC
    /* 800B34CC 2484325C */  addiu $a0, $a0, 12892
    /* 800B34D0 81EFFECD */  lb $t7, -307($t7)
    /* 800B34D4 11E0000B */  beq $t7, $zero, 0x800B3504
    /* 800B34D8 00000000 */  nop
    /* 800B34DC 92660000 */  lbu $a2, 0($s3)
    /* 800B34E0 00002825 */  or $a1, $zero, $zero
    /* 800B34E4 00003825 */  or $a3, $zero, $zero
    /* 800B34E8 24C6FFFF */  addiu $a2, $a2, -1
    /* 800B34EC 0006C600 */  sll $t8, $a2, 24
    /* 800B34F0 0C024B8B */  jal 0x80092E2C
    /* 800B34F4 00183603 */  sra $a2, $t8, 24
    /* 800B34F8 A6220000 */  sh $v0, 0($s1)
    /* 800B34FC 26520004 */  addiu $s2, $s2, 4
    /* 800B3500 26310002 */  addiu $s1, $s1, 2
    /* 800B3504 3C088014 */  lui $t0, 0x8014
    /* 800B3508 25082AC4 */  addiu $t0, $t0, 10948
    /* 800B350C 0228082B */  sltu $at, $s1, $t0
    /* 800B3510 10200012 */  beq $at, $zero, 0x800B355C
    /* 800B3514 3C098012 */  lui $t1, 0x8012
    /* 800B3518 2529B118 */  addiu $t1, $t1, -20200
    /* 800B351C 02498021 */  addu $s0, $s2, $t1
    /* 800B3520 3C128012 */  lui $s2, 0x8012
    /* 800B3524 2652B720 */  addiu $s2, $s2, -18656
    /* 800B3528 92660000 */  lbu $a2, 0($s3)
    /* 800B352C 8E04FC50 */  lw $a0, -944($s0)
    /* 800B3530 00002825 */  or $a1, $zero, $zero
    /* 800B3534 24C6FFFF */  addiu $a2, $a2, -1
    /* 800B3538 00065600 */  sll $t2, $a2, 24
    /* 800B353C 000A3603 */  sra $a2, $t2, 24
    /* 800B3540 0C024B8B */  jal 0x80092E2C
    /* 800B3544 00003825 */  or $a3, $zero, $zero
    /* 800B3548 26100004 */  addiu $s0, $s0, 4
    /* 800B354C 0212082B */  sltu $at, $s0, $s2
    /* 800B3550 26310002 */  addiu $s1, $s1, 2
    /* 800B3554 1420FFF4 */  bne $at, $zero, 0x800B3528
    /* 800B3558 A622FFFE */  sh $v0, -2($s1)
    /* 800B355C 0C02CB7E */  jal 0x800B2DF8
    /* 800B3560 00000000 */  nop
    /* 800B3564 8FBF002C */  lw $ra, 44($sp)
    /* 800B3568 8FB00018 */  lw $s0, 24($sp)
    /* 800B356C 8FB1001C */  lw $s1, 28($sp)
    /* 800B3570 8FB20020 */  lw $s2, 32($sp)
    /* 800B3574 8FB30024 */  lw $s3, 36($sp)
    /* 800B3578 8FB40028 */  lw $s4, 40($sp)
    /* 800B357C 03E00008 */  jr $ra
    /* 800B3580 27BD0030 */  addiu $sp, $sp, 48
