# Source: game_code.bin (decompressed)
# Address: 0x800F8588

glabel update_mpath_index
    /* 800F8588 27BDFFD8 */  addiu $sp, $sp, -40
    /* 800F858C AFBF0024 */  sw $ra, 36($sp)
    /* 800F8590 5202006B */  .word 0x5202006B
    /* 800F8594 8FBF0024 */  lw $ra, 36($sp)
    /* 800F8598 24150001 */  addiu $s5, $zero, 1
    /* 800F859C 16A20003 */  bne $s5, $v0, 0x800F85AC
    /* 800F85A0 00000000 */  nop
    /* 800F85A4 0C02D16F */  jal 0x800B45BC
    /* 800F85A8 00002025 */  or $a0, $zero, $zero
    /* 800F85AC 16B00063 */  bne $s5, $s0, 0x800F873C
    /* 800F85B0 AE300000 */  sw $s0, 0($s1)
    /* 800F85B4 3C178014 */  lui $s7, 0x8014
    /* 800F85B8 26F761D0 */  addiu $s7, $s7, 25040
    /* 800F85BC 02E02025 */  or $a0, $s7, $zero
    /* 800F85C0 00002825 */  or $a1, $zero, $zero
    /* 800F85C4 0C001C9C */  jal 0x80007270
    /* 800F85C8 02A03025 */  or $a2, $s5, $zero
    /* 800F85CC 0C02D080 */  jal 0x800B4200
    /* 800F85D0 2412000D */  addiu $s2, $zero, 13
    /* 800F85D4 00408025 */  or $s0, $v0, $zero
    /* 800F85D8 02E02025 */  or $a0, $s7, $zero
    /* 800F85DC 00002825 */  or $a1, $zero, $zero
    /* 800F85E0 0C001D78 */  jal 0x800075E0
    /* 800F85E4 00003025 */  or $a2, $zero, $zero
    /* 800F85E8 3C148018 */  lui $s4, 0x8018
    /* 800F85EC 2694A4E0 */  addiu $s4, $s4, -23328
    /* 800F85F0 8E8F0004 */  lw $t7, 4($s4)
    /* 800F85F4 2405FFFF */  addiu $a1, $zero, -1
    /* 800F85F8 0C02CFE9 */  jal 0x800B3FA4
    /* 800F85FC 8DE400FC */  lw $a0, 252($t7)
    /* 800F8600 3C038011 */  lui $v1, 0x8011
    /* 800F8604 24634724 */  addiu $v1, $v1, 18212
    /* 800F8608 00408825 */  or $s1, $v0, $zero
    /* 800F860C 90760000 */  lbu $s6, 0($v1)
    /* 800F8610 0C02CFD4 */  jal 0x800B3F50
    /* 800F8614 90700001 */  lbu $s0, 1($v1)
    /* 800F8618 06210003 */  bgez $s1, 0x800F8628
    /* 800F861C 00111843 */  sra $v1, $s1, 1
    /* 800F8620 26210001 */  addiu $at, $s1, 1
    /* 800F8624 00011843 */  sra $v1, $at, 1
    /* 800F8628 02C32023 */  subu $a0, $s6, $v1
    /* 800F862C 00763021 */  addu $a2, $v1, $s6
    /* 800F8630 00503821 */  addu $a3, $v0, $s0
    /* 800F8634 241800B0 */  addiu $t8, $zero, 176
    /* 800F8638 AFB80010 */  sw $t8, 16($sp)
    /* 800F863C 24E70004 */  addiu $a3, $a3, 4
    /* 800F8640 24C60008 */  addiu $a2, $a2, 8
    /* 800F8644 2484FFF8 */  addiu $a0, $a0, -8
    /* 800F8648 2605FFFC */  addiu $a1, $s0, -4
    /* 800F864C AFA00014 */  sw $zero, 20($sp)
    /* 800F8650 AFA00018 */  sw $zero, 24($sp)
    /* 800F8654 0C02CED3 */  jal 0x800B3B4C
    /* 800F8658 AFA0001C */  sw $zero, 28($sp)
    /* 800F865C 02E02025 */  or $a0, $s7, $zero
    /* 800F8660 00002825 */  or $a1, $zero, $zero
    /* 800F8664 0C001C9C */  jal 0x80007270
    /* 800F8668 02A03025 */  or $a2, $s5, $zero
    /* 800F866C 0C02D080 */  jal 0x800B4200
    /* 800F8670 2412000A */  addiu $s2, $zero, 10
    /* 800F8674 00408025 */  or $s0, $v0, $zero
    /* 800F8678 02E02025 */  or $a0, $s7, $zero
    /* 800F867C 00002825 */  or $a1, $zero, $zero
    /* 800F8680 0C001D78 */  jal 0x800075E0
    /* 800F8684 00003025 */  or $a2, $zero, $zero
    /* 800F8688 00008825 */  or $s1, $zero, $zero
    /* 800F868C 00009025 */  or $s2, $zero, $zero
    /* 800F8690 00008025 */  or $s0, $zero, $zero
    /* 800F8694 24130007 */  addiu $s3, $zero, 7
    /* 800F8698 0C03E12C */  jal 0x800F84B0
    /* 800F869C 02002025 */  or $a0, $s0, $zero
    /* 800F86A0 54550010 */  .word 0x54550010
    /* 800F86A4 26100001 */  addiu $s0, $s0, 1
    /* 800F86A8 8E8E000C */  lw $t6, 12($s4)
    /* 800F86AC 8E990010 */  lw $t9, 16($s4)
    /* 800F86B0 2405FFFF */  addiu $a1, $zero, -1
    /* 800F86B4 95CF005A */  lhu $t7, 90($t6)
    /* 800F86B8 000FC080 */  sll $t8, $t7, 2
    /* 800F86BC 00107880 */  sll $t7, $s0, 2
    /* 800F86C0 03387021 */  addu $t6, $t9, $t8
    /* 800F86C4 01CFC821 */  addu $t9, $t6, $t7
    /* 800F86C8 0C02CFE9 */  jal 0x800B3FA4
    /* 800F86CC 8F240000 */  lw $a0, 0($t9)
    /* 800F86D0 0222082A */  slt $at, $s1, $v0
    /* 800F86D4 10200002 */  beq $at, $zero, 0x800F86E0
    /* 800F86D8 26520001 */  addiu $s2, $s2, 1
    /* 800F86DC 00408825 */  or $s1, $v0, $zero
    /* 800F86E0 26100001 */  addiu $s0, $s0, 1
    /* 800F86E4 1613FFEC */  bne $s0, $s3, 0x800F8698
    /* 800F86E8 00000000 */  nop
    /* 800F86EC 0C02CFD4 */  jal 0x800B3F50
    /* 800F86F0 00000000 */  nop
    /* 800F86F4 00520019 */  multu $v0, $s2
    /* 800F86F8 241800B0 */  addiu $t8, $zero, 176
    /* 800F86FC 06210003 */  bgez $s1, 0x800F870C
    /* 800F8700 00111843 */  sra $v1, $s1, 1
    /* 800F8704 26210001 */  addiu $at, $s1, 1
    /* 800F8708 00011843 */  sra $v1, $at, 1
    /* 800F870C 02C32023 */  subu $a0, $s6, $v1
    /* 800F8710 00763021 */  addu $a2, $v1, $s6
    /* 800F8714 00003812 */  mflo $a3
    /* 800F8718 24E7007A */  addiu $a3, $a3, 122
    /* 800F871C 24C60008 */  addiu $a2, $a2, 8
    /* 800F8720 2484FFF8 */  addiu $a0, $a0, -8
    /* 800F8724 AFB80010 */  sw $t8, 16($sp)
    /* 800F8728 24050072 */  addiu $a1, $zero, 114
    /* 800F872C AFA00014 */  sw $zero, 20($sp)
    /* 800F8730 AFA00018 */  sw $zero, 24($sp)
    /* 800F8734 0C02CED3 */  jal 0x800B3B4C
    /* 800F8738 AFA0001C */  sw $zero, 28($sp)
    /* 800F873C 8FBF0024 */  lw $ra, 36($sp)
    /* 800F8740 27BD0028 */  addiu $sp, $sp, 40
    /* 800F8744 03E00008 */  jr $ra
    /* 800F8748 00000000 */  nop
