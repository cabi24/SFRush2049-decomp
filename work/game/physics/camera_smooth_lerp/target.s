# Source: game_code.bin (decompressed)
# Address: 0x800AF51C

glabel camera_smooth_lerp
    /* 800AF51C 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800AF520 AFB10018 */  sw $s1, 24($sp)
    /* 800AF524 AFB00014 */  sw $s0, 20($sp)
    /* 800AF528 3C118014 */  lui $s1, 0x8014
    /* 800AF52C AFBF001C */  sw $ra, 28($sp)
    /* 800AF530 2631C238 */  addiu $s1, $s1, -15816
    /* 800AF534 00008025 */  or $s0, $zero, $zero
    /* 800AF538 0C029F5B */  jal 0x800A7D6C
    /* 800AF53C 00000000 */  nop
    /* 800AF540 00107080 */  sll $t6, $s0, 2
    /* 800AF544 26100001 */  addiu $s0, $s0, 1
    /* 800AF548 0010C400 */  sll $t8, $s0, 16
    /* 800AF54C 00188403 */  sra $s0, $t8, 16
    /* 800AF550 2A010032 */  slti $at, $s0, 50
    /* 800AF554 022E7821 */  addu $t7, $s1, $t6
    /* 800AF558 1420FFF7 */  bne $at, $zero, 0x800AF538
    /* 800AF55C ADE20000 */  sw $v0, 0($t7)
    /* 800AF560 3C068015 */  lui $a2, 0x8015
    /* 800AF564 3C048015 */  lui $a0, 0x8015
    /* 800AF568 24844FD8 */  addiu $a0, $a0, 20440
    /* 800AF56C 24C64660 */  addiu $a2, $a2, 18016
    /* 800AF570 00008025 */  or $s0, $zero, $zero
    /* 800AF574 24070190 */  addiu $a3, $zero, 400
    /* 800AF578 2405003C */  addiu $a1, $zero, 60
    /* 800AF57C 2403FFFF */  addiu $v1, $zero, -1
    /* 800AF580 02050019 */  multu $s0, $a1
    /* 800AF584 00004012 */  mflo $t0
    /* 800AF588 00884821 */  addu $t1, $a0, $t0
    /* 800AF58C AD230000 */  sw $v1, 0($t1)
    /* 800AF590 02070019 */  multu $s0, $a3
    /* 800AF594 26100001 */  addiu $s0, $s0, 1
    /* 800AF598 00105C00 */  sll $t3, $s0, 16
    /* 800AF59C 000B8403 */  sra $s0, $t3, 16
    /* 800AF5A0 2A010006 */  slti $at, $s0, 6
    /* 800AF5A4 00005012 */  mflo $t2
    /* 800AF5A8 00CA1021 */  addu $v0, $a2, $t2
    /* 800AF5AC AC430150 */  sw $v1, 336($v0)
    /* 800AF5B0 AC430000 */  sw $v1, 0($v0)
    /* 800AF5B4 AC430054 */  sw $v1, 84($v0)
    /* 800AF5B8 AC4300A8 */  sw $v1, 168($v0)
    /* 800AF5BC 1420FFF0 */  bne $at, $zero, 0x800AF580
    /* 800AF5C0 AC4300FC */  sw $v1, 252($v0)
    /* 800AF5C4 8FBF001C */  lw $ra, 28($sp)
    /* 800AF5C8 3C018014 */  lui $at, 0x8014
    /* 800AF5CC 8FB00014 */  lw $s0, 20($sp)
    /* 800AF5D0 8FB10018 */  lw $s1, 24($sp)
    /* 800AF5D4 A420C094 */  sh $zero, -16236($at)
    /* 800AF5D8 03E00008 */  jr $ra
    /* 800AF5DC 27BD0020 */  addiu $sp, $sp, 32
