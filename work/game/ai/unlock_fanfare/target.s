# Source: game_code.bin (decompressed)
# Address: 0x800DC88C

glabel unlock_fanfare
    /* 800DC88C 27BDFFD8 */  addiu $sp, $sp, -40
    /* 800DC890 3C148014 */  lui $s4, 0x8014
    /* 800DC894 269461D0 */  addiu $s4, $s4, 25040
    /* 800DC898 AFBF0024 */  sw $ra, 36($sp)
    /* 800DC89C 02802025 */  or $a0, $s4, $zero
    /* 800DC8A0 00002825 */  or $a1, $zero, $zero
    /* 800DC8A4 0C001C9C */  jal 0x80007270
    /* 800DC8A8 24060001 */  addiu $a2, $zero, 1
    /* 800DC8AC 0C02D080 */  jal 0x800B4200
    /* 800DC8B0 2412000B */  addiu $s2, $zero, 11
    /* 800DC8B4 00408025 */  or $s0, $v0, $zero
    /* 800DC8B8 02802025 */  or $a0, $s4, $zero
    /* 800DC8BC 00002825 */  or $a1, $zero, $zero
    /* 800DC8C0 0C001D78 */  jal 0x800075E0
    /* 800DC8C4 00003025 */  or $a2, $zero, $zero
    /* 800DC8C8 0C02DD28 */  jal 0x800B74A0
    /* 800DC8CC 24040001 */  addiu $a0, $zero, 1
    /* 800DC8D0 00157080 */  sll $t6, $s5, 2
    /* 800DC8D4 00167880 */  sll $t7, $s6, 2
    /* 800DC8D8 01F67823 */  subu $t7, $t7, $s6
    /* 800DC8DC 01D57023 */  subu $t6, $t6, $s5
    /* 800DC8E0 000E7080 */  sll $t6, $t6, 2
    /* 800DC8E4 000F7880 */  sll $t7, $t7, 2
    /* 800DC8E8 01F67823 */  subu $t7, $t7, $s6
    /* 800DC8EC 01D57023 */  subu $t6, $t6, $s5
    /* 800DC8F0 000E70C0 */  sll $t6, $t6, 3
    /* 800DC8F4 000F7880 */  sll $t7, $t7, 2
    /* 800DC8F8 3C198015 */  lui $t9, 0x8015
    /* 800DC8FC 27393FD8 */  addiu $t9, $t9, 16344
    /* 800DC900 01CFC021 */  addu $t8, $t6, $t7
    /* 800DC904 03191021 */  addu $v0, $t8, $t9
    /* 800DC908 8446001C */  lh $a2, 28($v0)
    /* 800DC90C 844F0018 */  lh $t7, 24($v0)
    /* 800DC910 8447001E */  lh $a3, 30($v0)
    /* 800DC914 3C108012 */  lui $s0, 0x8012
    /* 800DC918 26108E20 */  addiu $s0, $s0, -29152
    /* 800DC91C 24030001 */  addiu $v1, $zero, 1
    /* 800DC920 AE030000 */  sw $v1, 0($s0)
    /* 800DC924 AE030004 */  sw $v1, 4($s0)
    /* 800DC928 04C10003 */  bgez $a2, 0x800DC938
    /* 800DC92C 00067043 */  sra $t6, $a2, 1
    /* 800DC930 24C10001 */  addiu $at, $a2, 1
    /* 800DC934 00017043 */  sra $t6, $at, 1
    /* 800DC938 01CF2021 */  addu $a0, $t6, $t7
    /* 800DC93C 844F001A */  lh $t7, 26($v0)
    /* 800DC940 0004C400 */  sll $t8, $a0, 16
    /* 800DC944 0018CC03 */  sra $t9, $t8, 16
    /* 800DC948 03202025 */  or $a0, $t9, $zero
    /* 800DC94C 04E10003 */  bgez $a3, 0x800DC95C
    /* 800DC950 00077043 */  sra $t6, $a3, 1
    /* 800DC954 24E10001 */  addiu $at, $a3, 1
    /* 800DC958 00017043 */  sra $t6, $at, 1
    /* 800DC95C 01CF2821 */  addu $a1, $t6, $t7
    /* 800DC960 0005C400 */  sll $t8, $a1, 16
    /* 800DC964 240EFFFF */  addiu $t6, $zero, -1
    /* 800DC968 AFAE0010 */  sw $t6, 16($sp)
    /* 800DC96C 00182C03 */  sra $a1, $t8, 16
    /* 800DC970 AFA00014 */  sw $zero, 20($sp)
    /* 800DC974 0C02F81E */  jal 0x800BE078
    /* 800DC978 AFB70018 */  sw $s7, 24($sp)
    /* 800DC97C 8FBF0024 */  lw $ra, 36($sp)
    /* 800DC980 240F0003 */  addiu $t7, $zero, 3
    /* 800DC984 AE0F0004 */  sw $t7, 4($s0)
    /* 800DC988 AE000000 */  sw $zero, 0($s0)
    /* 800DC98C 03E00008 */  jr $ra
    /* 800DC990 27BD0028 */  addiu $sp, $sp, 40
