# Source: game_code.bin (decompressed)
# Address: 0x800B8650

glabel particle_render
    /* 800B8650 27BDFF80 */  addiu $sp, $sp, -128
    /* 800B8654 3C148014 */  lui $s4, 0x8014
    /* 800B8658 269461D0 */  addiu $s4, $s4, 25040
    /* 800B865C AFBF0014 */  sw $ra, 20($sp)
    /* 800B8660 02802025 */  or $a0, $s4, $zero
    /* 800B8664 00002825 */  or $a1, $zero, $zero
    /* 800B8668 0C001C9C */  jal 0x80007270
    /* 800B866C 24060001 */  addiu $a2, $zero, 1
    /* 800B8670 0C02D080 */  jal 0x800B4200
    /* 800B8674 2412000B */  addiu $s2, $zero, 11
    /* 800B8678 02802025 */  or $a0, $s4, $zero
    /* 800B867C 00002825 */  or $a1, $zero, $zero
    /* 800B8680 0C001D78 */  jal 0x800075E0
    /* 800B8684 00003025 */  or $a2, $zero, $zero
    /* 800B8688 3C158012 */  lui $s5, 0x8012
    /* 800B868C 3C118018 */  lui $s1, 0x8018
    /* 800B8690 0000B825 */  or $s7, $zero, $zero
    /* 800B8694 0000B025 */  or $s6, $zero, $zero
    /* 800B8698 00009825 */  or $s3, $zero, $zero
    /* 800B869C 2631A4E0 */  addiu $s1, $s1, -23328
    /* 800B86A0 26B5AD40 */  addiu $s5, $s5, -21184
    /* 800B86A4 00009025 */  or $s2, $zero, $zero
    /* 800B86A8 241E0001 */  addiu $fp, $zero, 1
    /* 800B86AC 27B40048 */  addiu $s4, $sp, 72
    /* 800B86B0 82A30000 */  lb $v1, 0($s5)
    /* 800B86B4 547E003C */  .word 0x547E003C
    /* 800B86B8 26520001 */  addiu $s2, $s2, 1
    /* 800B86BC 8E2F000C */  lw $t7, 12($s1)
    /* 800B86C0 8E2E0010 */  lw $t6, 16($s1)
    /* 800B86C4 00128080 */  sll $s0, $s2, 2
    /* 800B86C8 95F80052 */  lhu $t8, 82($t7)
    /* 800B86CC 26F70001 */  addiu $s7, $s7, 1
    /* 800B86D0 2405FFFF */  addiu $a1, $zero, -1
    /* 800B86D4 0018C880 */  sll $t9, $t8, 2
    /* 800B86D8 01D97821 */  addu $t7, $t6, $t9
    /* 800B86DC 01F0C021 */  addu $t8, $t7, $s0
    /* 800B86E0 0C02CFE9 */  jal 0x800B3FA4
    /* 800B86E4 8F040000 */  lw $a0, 0($t8)
    /* 800B86E8 02C2082B */  sltu $at, $s6, $v0
    /* 800B86EC 5020000C */  .word 0x5020000C
    /* 800B86F0 24010002 */  addiu $at, $zero, 2
    /* 800B86F4 8E39000C */  lw $t9, 12($s1)
    /* 800B86F8 8E2E0010 */  lw $t6, 16($s1)
    /* 800B86FC 2405FFFF */  addiu $a1, $zero, -1
    /* 800B8700 972F0052 */  lhu $t7, 82($t9)
    /* 800B8704 000FC080 */  sll $t8, $t7, 2
    /* 800B8708 01D8C821 */  addu $t9, $t6, $t8
    /* 800B870C 03307821 */  addu $t7, $t9, $s0
    /* 800B8710 0C02CFE9 */  jal 0x800B3FA4
    /* 800B8714 8DE40000 */  lw $a0, 0($t7)
    /* 800B8718 0040B025 */  or $s6, $v0, $zero
    /* 800B871C 24010002 */  addiu $at, $zero, 2
    /* 800B8720 16410016 */  bne $s2, $at, 0x800B877C
    /* 800B8724 3C048012 */  lui $a0, 0x8012
    /* 800B8728 8E38000C */  lw $t8, 12($s1)
    /* 800B872C 8E2E0010 */  lw $t6, 16($s1)
    /* 800B8730 3C058012 */  lui $a1, 0x8012
    /* 800B8734 97190054 */  lhu $t9, 84($t8)
    /* 800B8738 24A5267C */  addiu $a1, $a1, 9852
    /* 800B873C 02802025 */  or $a0, $s4, $zero
    /* 800B8740 00197880 */  sll $t7, $t9, 2
    /* 800B8744 01CF1021 */  addu $v0, $t6, $t7
    /* 800B8748 8C460000 */  lw $a2, 0($v0)
    /* 800B874C 0C02D0D8 */  jal 0x800B4360
    /* 800B8750 8C470004 */  lw $a3, 4($v0)
    /* 800B8754 02802025 */  or $a0, $s4, $zero
    /* 800B8758 0C02CFE9 */  jal 0x800B3FA4
    /* 800B875C 2405FFFF */  addiu $a1, $zero, -1
    /* 800B8760 0262082B */  sltu $at, $s3, $v0
    /* 800B8764 1020000F */  beq $at, $zero, 0x800B87A4
    /* 800B8768 02802025 */  or $a0, $s4, $zero
    /* 800B876C 0C02CFE9 */  jal 0x800B3FA4
    /* 800B8770 2405FFFF */  addiu $a1, $zero, -1
    /* 800B8774 1000000B */  beq $zero, $zero, 0x800B87A4
    /* 800B8778 00409825 */  or $s3, $v0, $zero
    /* 800B877C 24842684 */  addiu $a0, $a0, 9860
    /* 800B8780 0C02CFE9 */  jal 0x800B3FA4
    /* 800B8784 2405FFFF */  addiu $a1, $zero, -1
    /* 800B8788 0262082B */  sltu $at, $s3, $v0
    /* 800B878C 10200005 */  beq $at, $zero, 0x800B87A4
    /* 800B8790 3C048012 */  lui $a0, 0x8012
    /* 800B8794 24842688 */  addiu $a0, $a0, 9864
    /* 800B8798 0C02CFE9 */  jal 0x800B3FA4
    /* 800B879C 2405FFFF */  addiu $a1, $zero, -1
    /* 800B87A0 00409825 */  or $s3, $v0, $zero
    /* 800B87A4 26520001 */  addiu $s2, $s2, 1
    /* 800B87A8 24010004 */  addiu $at, $zero, 4
    /* 800B87AC 1641FFC0 */  bne $s2, $at, 0x800B86B0
    /* 800B87B0 26B50001 */  addiu $s5, $s5, 1
    /* 800B87B4 3C108012 */  lui $s0, 0x8012
    /* 800B87B8 02D3C021 */  addu $t8, $s6, $s3
    /* 800B87BC 2610AD70 */  addiu $s0, $s0, -21136
    /* 800B87C0 2719000A */  addiu $t9, $t8, 10
    /* 800B87C4 0C02CFD4 */  jal 0x800B3F50
    /* 800B87C8 AE190004 */  sw $t9, 4($s0)
    /* 800B87CC 00570019 */  multu $v0, $s7
    /* 800B87D0 8E0F0004 */  lw $t7, 4($s0)
    /* 800B87D4 241900A0 */  addiu $t9, $zero, 160
    /* 800B87D8 8FBF0014 */  lw $ra, 20($sp)
    /* 800B87DC 27BD0080 */  addiu $sp, $sp, 128
    /* 800B87E0 00007012 */  mflo $t6
    /* 800B87E4 AE0E0008 */  sw $t6, 8($s0)
    /* 800B87E8 05E10003 */  bgez $t7, 0x800B87F8
    /* 800B87EC 000FC043 */  sra $t8, $t7, 1
    /* 800B87F0 25E10001 */  addiu $at, $t7, 1
    /* 800B87F4 0001C043 */  sra $t8, $at, 1
    /* 800B87F8 03387023 */  subu $t6, $t9, $t8
    /* 800B87FC 01D6C821 */  addu $t9, $t6, $s6
    /* 800B8800 27380005 */  addiu $t8, $t9, 5
    /* 800B8804 AE0E0000 */  sw $t6, 0($s0)
    /* 800B8808 03E00008 */  jr $ra
    /* 800B880C AE18000C */  sw $t8, 12($s0)
