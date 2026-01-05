# Source: game_code.bin (decompressed)
# Address: 0x800D67C0

glabel drone_position_calc
    /* 800D67C0 27BDFF70 */  addiu $sp, $sp, -144
    /* 800D67C4 3C148014 */  lui $s4, 0x8014
    /* 800D67C8 269461D0 */  addiu $s4, $s4, 25040
    /* 800D67CC AFBF0024 */  sw $ra, 36($sp)
    /* 800D67D0 02802025 */  or $a0, $s4, $zero
    /* 800D67D4 00002825 */  or $a1, $zero, $zero
    /* 800D67D8 0C001C9C */  jal 0x80007270
    /* 800D67DC 24060001 */  addiu $a2, $zero, 1
    /* 800D67E0 0C02D080 */  jal 0x800B4200
    /* 800D67E4 2412000D */  addiu $s2, $zero, 13
    /* 800D67E8 00408025 */  or $s0, $v0, $zero
    /* 800D67EC 02802025 */  or $a0, $s4, $zero
    /* 800D67F0 00002825 */  or $a1, $zero, $zero
    /* 800D67F4 0C001D78 */  jal 0x800075E0
    /* 800D67F8 00003025 */  or $a2, $zero, $zero
    /* 800D67FC 3C0E8011 */  lui $t6, 0x8011
    /* 800D6800 81CE064C */  lb $t6, 1612($t6)
    /* 800D6804 27B00044 */  addiu $s0, $sp, 68
    /* 800D6808 02002025 */  or $a0, $s0, $zero
    /* 800D680C 11C0000F */  beq $t6, $zero, 0x800D684C
    /* 800D6810 3C188018 */  lui $t8, 0x8018
    /* 800D6814 3C068015 */  lui $a2, 0x8015
    /* 800D6818 3C0F8018 */  lui $t7, 0x8018
    /* 800D681C 8DEFA4E4 */  lw $t7, -23324($t7)
    /* 800D6820 8CC6698C */  lw $a2, 27020($a2)
    /* 800D6824 3C058012 */  lui $a1, 0x8012
    /* 800D6828 24A5F6B0 */  addiu $a1, $a1, -2384
    /* 800D682C 8DE703A4 */  lw $a3, 932($t7)
    /* 800D6830 0C02D0D8 */  jal 0x800B4360
    /* 800D6834 24C60001 */  addiu $a2, $a2, 1
    /* 800D6838 02002025 */  or $a0, $s0, $zero
    /* 800D683C 0C02CFE9 */  jal 0x800B3FA4
    /* 800D6840 2405FFFF */  addiu $a1, $zero, -1
    /* 800D6844 10000006 */  beq $zero, $zero, 0x800D6860
    /* 800D6848 00408025 */  or $s0, $v0, $zero
    /* 800D684C 8F18A4E4 */  lw $t8, -23324($t8)
    /* 800D6850 2405FFFF */  addiu $a1, $zero, -1
    /* 800D6854 0C02CFE9 */  jal 0x800B3FA4
    /* 800D6858 8F0400D0 */  lw $a0, 208($t8)
    /* 800D685C 00408025 */  or $s0, $v0, $zero
    /* 800D6860 3C118011 */  lui $s1, 0x8011
    /* 800D6864 26310648 */  addiu $s1, $s1, 1608
    /* 800D6868 8E390000 */  lw $t9, 0($s1)
    /* 800D686C 13200011 */  beq $t9, $zero, 0x800D68B4
    /* 800D6870 00000000 */  nop
    /* 800D6874 0C02CFD4 */  jal 0x800B3F50
    /* 800D6878 00000000 */  nop
    /* 800D687C 240E0098 */  addiu $t6, $zero, 152
    /* 800D6880 244F000E */  addiu $t7, $v0, 14
    /* 800D6884 AFAF0010 */  sw $t7, 16($sp)
    /* 800D6888 8E240000 */  lw $a0, 0($s1)
    /* 800D688C 06010003 */  bgez $s0, 0x800D689C
    /* 800D6890 00101843 */  sra $v1, $s0, 1
    /* 800D6894 26010001 */  addiu $at, $s0, 1
    /* 800D6898 00011843 */  sra $v1, $at, 1
    /* 800D689C 01C32823 */  subu $a1, $t6, $v1
    /* 800D68A0 246700A8 */  addiu $a3, $v1, 168
    /* 800D68A4 0C02CE6F */  jal 0x800B39BC
    /* 800D68A8 24060006 */  addiu $a2, $zero, 6
    /* 800D68AC 10000014 */  beq $zero, $zero, 0x800D6900
    /* 800D68B0 8FBF0024 */  lw $ra, 36($sp)
    /* 800D68B4 0C02CFD4 */  jal 0x800B3F50
    /* 800D68B8 00000000 */  nop
    /* 800D68BC 06010003 */  bgez $s0, 0x800D68CC
    /* 800D68C0 00101843 */  sra $v1, $s0, 1
    /* 800D68C4 26010001 */  addiu $at, $s0, 1
    /* 800D68C8 00011843 */  sra $v1, $at, 1
    /* 800D68CC 24180098 */  addiu $t8, $zero, 152
    /* 800D68D0 241900B0 */  addiu $t9, $zero, 176
    /* 800D68D4 AFB90010 */  sw $t9, 16($sp)
    /* 800D68D8 03032023 */  subu $a0, $t8, $v1
    /* 800D68DC 246600A8 */  addiu $a2, $v1, 168
    /* 800D68E0 24050006 */  addiu $a1, $zero, 6
    /* 800D68E4 2447000E */  addiu $a3, $v0, 14
    /* 800D68E8 AFA00014 */  sw $zero, 20($sp)
    /* 800D68EC AFA00018 */  sw $zero, 24($sp)
    /* 800D68F0 0C02CED3 */  jal 0x800B3B4C
    /* 800D68F4 AFA0001C */  sw $zero, 28($sp)
    /* 800D68F8 AE220000 */  sw $v0, 0($s1)
    /* 800D68FC 8FBF0024 */  lw $ra, 36($sp)
    /* 800D6900 27BD0090 */  addiu $sp, $sp, 144
    /* 800D6904 03E00008 */  jr $ra
    /* 800D6908 00000000 */  nop
