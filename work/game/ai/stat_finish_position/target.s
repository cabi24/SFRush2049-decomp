# Source: game_code.bin (decompressed)
# Address: 0x800DA0CC

glabel stat_finish_position
    /* 800DA0CC 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800DA0D0 01C17824 */  and $t7, $t6, $at
    /* 800DA0D4 15E00005 */  bne $t7, $zero, 0x800DA0EC
    /* 800DA0D8 AFBF0014 */  sw $ra, 20($sp)
    /* 800DA0DC 0C02D16F */  jal 0x800B45BC
    /* 800DA0E0 00002025 */  or $a0, $zero, $zero
    /* 800DA0E4 3C018011 */  lui $at, 0x8011
    /* 800DA0E8 AC206DA4 */  sw $zero, 28068($at)
    /* 800DA0EC 3C108011 */  lui $s0, 0x8011
    /* 800DA0F0 26106D98 */  addiu $s0, $s0, 28056
    /* 800DA0F4 8E040000 */  lw $a0, 0($s0)
    /* 800DA0F8 10800004 */  beq $a0, $zero, 0x800DA10C
    /* 800DA0FC 00000000 */  nop
    /* 800DA100 0C02CD63 */  jal 0x800B358C
    /* 800DA104 00000000 */  nop
    /* 800DA108 AE000000 */  sw $zero, 0($s0)
    /* 800DA10C 0C02D55F */  jal 0x800B557C
    /* 800DA110 00000000 */  nop
    /* 800DA114 3C108011 */  lui $s0, 0x8011
    /* 800DA118 3C128011 */  lui $s2, 0x8011
    /* 800DA11C 26526D0C */  addiu $s2, $s2, 27916
    /* 800DA120 2610650C */  addiu $s0, $s0, 25868
    /* 800DA124 2411FFFF */  addiu $s1, $zero, -1
    /* 800DA128 8E020004 */  lw $v0, 4($s0)
    /* 800DA12C 00002825 */  or $a1, $zero, $zero
    /* 800DA130 00003025 */  or $a2, $zero, $zero
    /* 800DA134 12220005 */  beq $s1, $v0, 0x800DA14C
    /* 800DA138 00022400 */  sll $a0, $v0, 16
    /* 800DA13C 0004C403 */  sra $t8, $a0, 16
    /* 800DA140 0C024022 */  jal 0x80090088
    /* 800DA144 03002025 */  or $a0, $t8, $zero
    /* 800DA148 AE110004 */  sw $s1, 4($s0)
    /* 800DA14C 26100040 */  addiu $s0, $s0, 64
    /* 800DA150 5612FFF6 */  .word 0x5612FFF6
    /* 800DA154 8E020004 */  lw $v0, 4($s0)
    /* 800DA158 8FBF0014 */  lw $ra, 20($sp)
    /* 800DA15C 3C018011 */  lui $at, 0x8011
    /* 800DA160 AC206D0C */  sw $zero, 27916($at)
    /* 800DA164 3C018011 */  lui $at, 0x8011
    /* 800DA168 A0206D94 */  sb $zero, 28052($at)
    /* 800DA16C 03E00008 */  jr $ra
    /* 800DA170 27BD0018 */  addiu $sp, $sp, 24
