# Source: game_code.bin (decompressed)
# Address: 0x800B3700

glabel skid_sounds_init
    /* 800B3700 00000000 */  nop
    /* 800B3704 3C038015 */  lui $v1, 0x8015
    /* 800B3708 24639788 */  addiu $v1, $v1, -26744
    /* 800B370C 8C620000 */  lw $v0, 0($v1)
    /* 800B3710 27BDFFD0 */  addiu $sp, $sp, -48
    /* 800B3714 AFBF002C */  sw $ra, 44($sp)
    /* 800B3718 284100C8 */  slti $at, $v0, 200
    /* 800B371C AFB00028 */  sw $s0, 40($sp)
    /* 800B3720 AFA50034 */  sw $a1, 52($sp)
    /* 800B3724 AFA60038 */  sw $a2, 56($sp)
    /* 800B3728 14200003 */  bne $at, $zero, 0x800B3738
    /* 800B372C 00804025 */  or $t0, $a0, $zero
    /* 800B3730 10000028 */  beq $zero, $zero, 0x800B37D4
    /* 800B3734 00001025 */  or $v0, $zero, $zero
    /* 800B3738 00027080 */  sll $t6, $v0, 2
    /* 800B373C 3C108015 */  lui $s0, 0x8015
    /* 800B3740 020E8021 */  addu $s0, $s0, $t6
    /* 800B3744 8E109450 */  lw $s0, -27568($s0)
    /* 800B3748 244F0001 */  addiu $t7, $v0, 1
    /* 800B374C AC6F0000 */  sw $t7, 0($v1)
    /* 800B3750 AE080000 */  sw $t0, 0($s0)
    /* 800B3754 8FB80034 */  lw $t8, 52($sp)
    /* 800B3758 24090001 */  addiu $t1, $zero, 1
    /* 800B375C 240AFFFF */  addiu $t2, $zero, -1
    /* 800B3760 A618000E */  sh $t8, 14($s0)
    /* 800B3764 8FB90038 */  lw $t9, 56($sp)
    /* 800B3768 240BFFFF */  addiu $t3, $zero, -1
    /* 800B376C A200001A */  sb $zero, 26($s0)
    /* 800B3770 A2000019 */  sb $zero, 25($s0)
    /* 800B3774 A209001B */  sb $t1, 27($s0)
    /* 800B3778 AE00003C */  sw $zero, 60($s0)
    /* 800B377C AE000028 */  sw $zero, 40($s0)
    /* 800B3780 AE0A002C */  sw $t2, 44($s0)
    /* 800B3784 AE0B0030 */  sw $t3, 48($s0)
    /* 800B3788 AE000038 */  sw $zero, 56($s0)
    /* 800B378C 02002025 */  or $a0, $s0, $zero
    /* 800B3790 0C02CD8B */  jal 0x800B362C
    /* 800B3794 A6190010 */  sh $t9, 16($s0)
    /* 800B3798 8FAC0038 */  lw $t4, 56($sp)
    /* 800B379C 9604000C */  lhu $a0, 12($s0)
    /* 800B37A0 240DFFFF */  addiu $t5, $zero, -1
    /* 800B37A4 240EFFFF */  addiu $t6, $zero, -1
    /* 800B37A8 AFAE0018 */  sw $t6, 24($sp)
    /* 800B37AC AFAD0014 */  sw $t5, 20($sp)
    /* 800B37B0 00002825 */  or $a1, $zero, $zero
    /* 800B37B4 00003025 */  or $a2, $zero, $zero
    /* 800B37B8 8FA70034 */  lw $a3, 52($sp)
    /* 800B37BC 0C029E7D */  jal 0x800A79F4
    /* 800B37C0 AFAC0010 */  sw $t4, 16($sp)
    /* 800B37C4 A6020034 */  sh $v0, 52($s0)
    /* 800B37C8 0C0253B2 */  jal 0x80094EC8
    /* 800B37CC 02002025 */  or $a0, $s0, $zero
    /* 800B37D0 02001025 */  or $v0, $s0, $zero
    /* 800B37D4 8FBF002C */  lw $ra, 44($sp)
    /* 800B37D8 8FB00028 */  lw $s0, 40($sp)
    /* 800B37DC 27BD0030 */  addiu $sp, $sp, 48
    /* 800B37E0 03E00008 */  jr $ra
    /* 800B37E4 00000000 */  nop
