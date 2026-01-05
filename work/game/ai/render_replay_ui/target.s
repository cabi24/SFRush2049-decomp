# Source: game_code.bin (decompressed)
# Address: 0x800D7D40

glabel render_replay_ui
    /* 800D7D40 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800D7D44 AFB40018 */  sw $s4, 24($sp)
    /* 800D7D48 00147600 */  sll $t6, $s4, 24
    /* 800D7D4C 000EA603 */  sra $s4, $t6, 24
    /* 800D7D50 0014C100 */  sll $t8, $s4, 4
    /* 800D7D54 0314C021 */  addu $t8, $t8, $s4
    /* 800D7D58 3C198011 */  lui $t9, 0x8011
    /* 800D7D5C 27391998 */  addiu $t9, $t9, 6552
    /* 800D7D60 0018C180 */  sll $t8, $t8, 6
    /* 800D7D64 AFBF0014 */  sw $ra, 20($sp)
    /* 800D7D68 03198021 */  addu $s0, $t8, $t9
    /* 800D7D6C 00008825 */  or $s1, $zero, $zero
    /* 800D7D70 24130440 */  addiu $s3, $zero, 1088
    /* 800D7D74 2412FFFF */  addiu $s2, $zero, -1
    /* 800D7D78 8E020004 */  lw $v0, 4($s0)
    /* 800D7D7C 00002825 */  or $a1, $zero, $zero
    /* 800D7D80 00003025 */  or $a2, $zero, $zero
    /* 800D7D84 12420005 */  beq $s2, $v0, 0x800D7D9C
    /* 800D7D88 00022400 */  sll $a0, $v0, 16
    /* 800D7D8C 00047403 */  sra $t6, $a0, 16
    /* 800D7D90 0C024022 */  jal 0x80090088
    /* 800D7D94 01C02025 */  or $a0, $t6, $zero
    /* 800D7D98 AE120004 */  sw $s2, 4($s0)
    /* 800D7D9C 26310040 */  addiu $s1, $s1, 64
    /* 800D7DA0 1633FFF5 */  bne $s1, $s3, 0x800D7D78
    /* 800D7DA4 26100040 */  addiu $s0, $s0, 64
    /* 800D7DA8 8FBF0014 */  lw $ra, 20($sp)
    /* 800D7DAC 00147880 */  sll $t7, $s4, 2
    /* 800D7DB0 3C018015 */  lui $at, 0x8015
    /* 800D7DB4 002F0821 */  addu $at, $at, $t7
    /* 800D7DB8 AC204618 */  sw $zero, 17944($at)
    /* 800D7DBC 03E00008 */  jr $ra
    /* 800D7DC0 27BD0018 */  addiu $sp, $sp, 24
