# Source: game_code.bin (decompressed)
# Address: 0x800B5694

glabel sound_priority_mgr
    /* 800B5694 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800B5698 AFBF0014 */  sw $ra, 20($sp)
    /* 800B569C 51C00028 */  .word 0x51C00028
    /* 800B56A0 8FBF0014 */  lw $ra, 20($sp)
    /* 800B56A4 0C02D57F */  jal 0x800B55FC
    /* 800B56A8 24040001 */  addiu $a0, $zero, 1
    /* 800B56AC 3C108012 */  lui $s0, 0x8012
    /* 800B56B0 3C128012 */  lui $s2, 0x8012
    /* 800B56B4 2652AC94 */  addiu $s2, $s2, -21356
    /* 800B56B8 2610A994 */  addiu $s0, $s0, -22124
    /* 800B56BC 2411FFFF */  addiu $s1, $zero, -1
    /* 800B56C0 8E020004 */  lw $v0, 4($s0)
    /* 800B56C4 00002825 */  or $a1, $zero, $zero
    /* 800B56C8 00003025 */  or $a2, $zero, $zero
    /* 800B56CC 12220005 */  beq $s1, $v0, 0x800B56E4
    /* 800B56D0 00022400 */  sll $a0, $v0, 16
    /* 800B56D4 00047C03 */  sra $t7, $a0, 16
    /* 800B56D8 0C024022 */  jal 0x80090088
    /* 800B56DC 01E02025 */  or $a0, $t7, $zero
    /* 800B56E0 AE110004 */  sw $s1, 4($s0)
    /* 800B56E4 26100040 */  addiu $s0, $s0, 64
    /* 800B56E8 5612FFF6 */  .word 0x5612FFF6
    /* 800B56EC 8E020004 */  lw $v0, 4($s0)
    /* 800B56F0 3C018012 */  lui $at, 0x8012
    /* 800B56F4 00002025 */  or $a0, $zero, $zero
    /* 800B56F8 0C02D16F */  jal 0x800B45BC
    /* 800B56FC AC20AC94 */  sw $zero, -21356($at)
    /* 800B5700 3C108012 */  lui $s0, 0x8012
    /* 800B5704 3C018012 */  lui $at, 0x8012
    /* 800B5708 2610AD34 */  addiu $s0, $s0, -21196
    /* 800B570C AC20AD68 */  sw $zero, -21144($at)
    /* 800B5710 8E040000 */  lw $a0, 0($s0)
    /* 800B5714 10800004 */  beq $a0, $zero, 0x800B5728
    /* 800B5718 00000000 */  nop
    /* 800B571C 0C02CD63 */  jal 0x800B358C
    /* 800B5720 00000000 */  nop
    /* 800B5724 AE000000 */  sw $zero, 0($s0)
    /* 800B5728 0C02D55F */  jal 0x800B557C
    /* 800B572C 00000000 */  nop
    /* 800B5730 3C018012 */  lui $at, 0x8012
    /* 800B5734 A020AD38 */  sb $zero, -21192($at)
    /* 800B5738 A2600000 */  sb $zero, 0($s3)
    /* 800B573C 8FBF0014 */  lw $ra, 20($sp)
    /* 800B5740 27BD0018 */  addiu $sp, $sp, 24
    /* 800B5744 03E00008 */  jr $ra
    /* 800B5748 00000000 */  nop
