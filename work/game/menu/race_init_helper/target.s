# Source: game_code.bin (decompressed)
# Address: 0x800EC0DC

glabel race_init_helper
    /* 800EC0DC 27BDFFD0 */  addiu $sp, $sp, -48
    /* 800EC0E0 AFBF002C */  sw $ra, 44($sp)
    /* 800EC0E4 AFB40028 */  sw $s4, 40($sp)
    /* 800EC0E8 AFB30024 */  sw $s3, 36($sp)
    /* 800EC0EC AFB20020 */  sw $s2, 32($sp)
    /* 800EC0F0 AFB1001C */  sw $s1, 28($sp)
    /* 800EC0F4 0C03A11F */  jal 0x800E847C
    /* 800EC0F8 AFB00018 */  sw $s0, 24($sp)
    /* 800EC0FC 3C0E8015 */  lui $t6, 0x8015
    /* 800EC100 8DCEA110 */  lw $t6, -24304($t6)
    /* 800EC104 24010002 */  addiu $at, $zero, 2
    /* 800EC108 3C148015 */  lui $s4, 0x8015
    /* 800EC10C 15C10016 */  bne $t6, $at, 0x800EC168
    /* 800EC110 2694A108 */  addiu $s4, $s4, -24312
    /* 800EC114 868F0000 */  lh $t7, 0($s4)
    /* 800EC118 3C118015 */  lui $s1, 0x8015
    /* 800EC11C 26312BD0 */  addiu $s1, $s1, 11216
    /* 800EC120 29E10002 */  slti $at, $t7, 2
    /* 800EC124 14200010 */  bne $at, $zero, 0x800EC168
    /* 800EC128 24100001 */  addiu $s0, $zero, 1
    /* 800EC12C 3C128015 */  lui $s2, 0x8015
    /* 800EC130 3C138015 */  lui $s3, 0x8015
    /* 800EC134 26732C20 */  addiu $s3, $s3, 11296
    /* 800EC138 26522BD8 */  addiu $s2, $s2, 11224
    /* 800EC13C 862400F6 */  lh $a0, 246($s1)
    /* 800EC140 02402825 */  or $a1, $s2, $zero
    /* 800EC144 0C0235BF */  jal 0x8008D6FC
    /* 800EC148 02603025 */  or $a2, $s3, $zero
    /* 800EC14C 86980000 */  lh $t8, 0($s4)
    /* 800EC150 26100001 */  addiu $s0, $s0, 1
    /* 800EC154 263103B8 */  addiu $s1, $s1, 952
    /* 800EC158 0218082A */  slt $at, $s0, $t8
    /* 800EC15C 265203B8 */  addiu $s2, $s2, 952
    /* 800EC160 1420FFF6 */  bne $at, $zero, 0x800EC13C
    /* 800EC164 267303B8 */  addiu $s3, $s3, 952
    /* 800EC168 0C03AE43 */  jal 0x800EB90C
    /* 800EC16C 00000000 */  nop
    /* 800EC170 8FBF002C */  lw $ra, 44($sp)
    /* 800EC174 8FB00018 */  lw $s0, 24($sp)
    /* 800EC178 8FB1001C */  lw $s1, 28($sp)
    /* 800EC17C 8FB20020 */  lw $s2, 32($sp)
    /* 800EC180 8FB30024 */  lw $s3, 36($sp)
    /* 800EC184 8FB40028 */  lw $s4, 40($sp)
    /* 800EC188 03E00008 */  jr $ra
    /* 800EC18C 27BD0030 */  addiu $sp, $sp, 48
