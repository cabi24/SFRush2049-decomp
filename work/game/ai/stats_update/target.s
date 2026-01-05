# Source: game_code.bin (decompressed)
# Address: 0x800DC99C

glabel stats_update
    /* 800DC99C 27BDFFC8 */  addiu $sp, $sp, -56
    /* 800DC9A0 3C038012 */  lui $v1, 0x8012
    /* 800DC9A4 3C148014 */  lui $s4, 0x8014
    /* 800DC9A8 269461D0 */  addiu $s4, $s4, 25040
    /* 800DC9AC 24638E20 */  addiu $v1, $v1, -29152
    /* 800DC9B0 AFBF0024 */  sw $ra, 36($sp)
    /* 800DC9B4 24060001 */  addiu $a2, $zero, 1
    /* 800DC9B8 AC660000 */  sw $a2, 0($v1)
    /* 800DC9BC AC660004 */  sw $a2, 4($v1)
    /* 800DC9C0 02802025 */  or $a0, $s4, $zero
    /* 800DC9C4 0C001C9C */  jal 0x80007270
    /* 800DC9C8 00002825 */  or $a1, $zero, $zero
    /* 800DC9CC 0C02D080 */  jal 0x800B4200
    /* 800DC9D0 2412000B */  addiu $s2, $zero, 11
    /* 800DC9D4 00408025 */  or $s0, $v0, $zero
    /* 800DC9D8 02802025 */  or $a0, $s4, $zero
    /* 800DC9DC 00002825 */  or $a1, $zero, $zero
    /* 800DC9E0 0C001D78 */  jal 0x800075E0
    /* 800DC9E4 00003025 */  or $a2, $zero, $zero
    /* 800DC9E8 0C02DD28 */  jal 0x800B74A0
    /* 800DC9EC 24040001 */  addiu $a0, $zero, 1
    /* 800DC9F0 00167080 */  sll $t6, $s6, 2
    /* 800DC9F4 00177880 */  sll $t7, $s7, 2
    /* 800DC9F8 01F77823 */  subu $t7, $t7, $s7
    /* 800DC9FC 01D67023 */  subu $t6, $t6, $s6
    /* 800DCA00 000E7080 */  sll $t6, $t6, 2
    /* 800DCA04 000F7880 */  sll $t7, $t7, 2
    /* 800DCA08 01F77823 */  subu $t7, $t7, $s7
    /* 800DCA0C 01D67023 */  subu $t6, $t6, $s6
    /* 800DCA10 000E70C0 */  sll $t6, $t6, 3
    /* 800DCA14 000F7880 */  sll $t7, $t7, 2
    /* 800DCA18 3C198015 */  lui $t9, 0x8015
    /* 800DCA1C 27393FD8 */  addiu $t9, $t9, 16344
    /* 800DCA20 01CFC021 */  addu $t8, $t6, $t7
    /* 800DCA24 03198021 */  addu $s0, $t8, $t9
    /* 800DCA28 8606001C */  lh $a2, 28($s0)
    /* 800DCA2C 860F0018 */  lh $t7, 24($s0)
    /* 800DCA30 8602001E */  lh $v0, 30($s0)
    /* 800DCA34 04C10003 */  bgez $a2, 0x800DCA44
    /* 800DCA38 00067043 */  sra $t6, $a2, 1
    /* 800DCA3C 24C10001 */  addiu $at, $a2, 1
    /* 800DCA40 00017043 */  sra $t6, $at, 1
    /* 800DCA44 01CF2021 */  addu $a0, $t6, $t7
    /* 800DCA48 860F001A */  lh $t7, 26($s0)
    /* 800DCA4C 2442FFD8 */  addiu $v0, $v0, -40
    /* 800DCA50 0004C400 */  sll $t8, $a0, 16
    /* 800DCA54 0018CC03 */  sra $t9, $t8, 16
    /* 800DCA58 00023C00 */  sll $a3, $v0, 16
    /* 800DCA5C 03202025 */  or $a0, $t9, $zero
    /* 800DCA60 04410003 */  bgez $v0, 0x800DCA70
    /* 800DCA64 00027043 */  sra $t6, $v0, 1
    /* 800DCA68 24410001 */  addiu $at, $v0, 1
    /* 800DCA6C 00017043 */  sra $t6, $at, 1
    /* 800DCA70 01CF2821 */  addu $a1, $t6, $t7
    /* 800DCA74 24A50014 */  addiu $a1, $a1, 20
    /* 800DCA78 0005C400 */  sll $t8, $a1, 16
    /* 800DCA7C 00182C03 */  sra $a1, $t8, 16
    /* 800DCA80 8FB80040 */  lw $t8, 64($sp)
    /* 800DCA84 240FFFFF */  addiu $t7, $zero, -1
    /* 800DCA88 00077403 */  sra $t6, $a3, 16
    /* 800DCA8C 01C03825 */  or $a3, $t6, $zero
    /* 800DCA90 AFAF0010 */  sw $t7, 16($sp)
    /* 800DCA94 AFA00014 */  sw $zero, 20($sp)
    /* 800DCA98 0C02F81E */  jal 0x800BE078
    /* 800DCA9C AFB80018 */  sw $t8, 24($sp)
    /* 800DCAA0 0C02CFD4 */  jal 0x800B3F50
    /* 800DCAA4 00000000 */  nop
    /* 800DCAA8 8619001C */  lh $t9, 28($s0)
    /* 800DCAAC 860F0018 */  lh $t7, 24($s0)
    /* 800DCAB0 8618001E */  lh $t8, 30($s0)
    /* 800DCAB4 AFA2002C */  sw $v0, 44($sp)
    /* 800DCAB8 07210003 */  bgez $t9, 0x800DCAC8
    /* 800DCABC 00197043 */  sra $t6, $t9, 1
    /* 800DCAC0 27210001 */  addiu $at, $t9, 1
    /* 800DCAC4 00017043 */  sra $t6, $at, 1
    /* 800DCAC8 8619001A */  lh $t9, 26($s0)
    /* 800DCACC 01CFB021 */  addu $s6, $t6, $t7
    /* 800DCAD0 00027880 */  sll $t7, $v0, 2
    /* 800DCAD4 01E27823 */  subu $t7, $t7, $v0
    /* 800DCAD8 03197021 */  addu $t6, $t8, $t9
    /* 800DCADC 12A00003 */  beq $s5, $zero, 0x800DCAEC
    /* 800DCAE0 01CF8823 */  subu $s1, $t6, $t7
    /* 800DCAE4 10000002 */  beq $zero, $zero, 0x800DCAF0
    /* 800DCAE8 2412000A */  addiu $s2, $zero, 10
    /* 800DCAEC 2412000B */  addiu $s2, $zero, 11
    /* 800DCAF0 02802025 */  or $a0, $s4, $zero
    /* 800DCAF4 00002825 */  or $a1, $zero, $zero
    /* 800DCAF8 0C001C9C */  jal 0x80007270
    /* 800DCAFC 24060001 */  addiu $a2, $zero, 1
    /* 800DCB00 0C02D080 */  jal 0x800B4200
    /* 800DCB04 AFB10030 */  sw $s1, 48($sp)
    /* 800DCB08 8FB10030 */  lw $s1, 48($sp)
    /* 800DCB0C 00408025 */  or $s0, $v0, $zero
    /* 800DCB10 02802025 */  or $a0, $s4, $zero
    /* 800DCB14 00002825 */  or $a1, $zero, $zero
    /* 800DCB18 0C001D78 */  jal 0x800075E0
    /* 800DCB1C 00003025 */  or $a2, $zero, $zero
    /* 800DCB20 52A00004 */  .word 0x52A00004
    /* 800DCB24 24120001 */  addiu $s2, $zero, 1
    /* 800DCB28 10000002 */  beq $zero, $zero, 0x800DCB34
    /* 800DCB2C 24120016 */  addiu $s2, $zero, 22
    /* 800DCB30 24120001 */  addiu $s2, $zero, 1
    /* 800DCB34 0C02DD28 */  jal 0x800B74A0
    /* 800DCB38 02402025 */  or $a0, $s2, $zero
    /* 800DCB3C 12A00003 */  beq $s5, $zero, 0x800DCB4C
    /* 800DCB40 0016BC00 */  sll $s7, $s6, 16
    /* 800DCB44 10000002 */  beq $zero, $zero, 0x800DCB50
    /* 800DCB48 24120001 */  addiu $s2, $zero, 1
    /* 800DCB4C 00009025 */  or $s2, $zero, $zero
    /* 800DCB50 0017C403 */  sra $t8, $s7, 16
    /* 800DCB54 00182400 */  sll $a0, $t8, 16
    /* 800DCB58 02322823 */  subu $a1, $s1, $s2
    /* 800DCB5C 00057400 */  sll $t6, $a1, 16
    /* 800DCB60 0004CC03 */  sra $t9, $a0, 16
    /* 800DCB64 03202025 */  or $a0, $t9, $zero
    /* 800DCB68 000E2C03 */  sra $a1, $t6, 16
    /* 800DCB6C 0300B825 */  or $s7, $t8, $zero
    /* 800DCB70 0C02DC75 */  jal 0x800B71D4
    /* 800DCB74 8FA60044 */  lw $a2, 68($sp)
    /* 800DCB78 13C00003 */  beq $fp, $zero, 0x800DCB88
    /* 800DCB7C 02802025 */  or $a0, $s4, $zero
    /* 800DCB80 10000002 */  beq $zero, $zero, 0x800DCB8C
    /* 800DCB84 2412000A */  addiu $s2, $zero, 10
    /* 800DCB88 2412000B */  addiu $s2, $zero, 11
    /* 800DCB8C 00002825 */  or $a1, $zero, $zero
    /* 800DCB90 0C001C9C */  jal 0x80007270
    /* 800DCB94 24060001 */  addiu $a2, $zero, 1
    /* 800DCB98 0C02D080 */  jal 0x800B4200
    /* 800DCB9C AFB10030 */  sw $s1, 48($sp)
    /* 800DCBA0 8FB10030 */  lw $s1, 48($sp)
    /* 800DCBA4 00408025 */  or $s0, $v0, $zero
    /* 800DCBA8 02802025 */  or $a0, $s4, $zero
    /* 800DCBAC 00002825 */  or $a1, $zero, $zero
    /* 800DCBB0 0C001D78 */  jal 0x800075E0
    /* 800DCBB4 00003025 */  or $a2, $zero, $zero
    /* 800DCBB8 53C00004 */  .word 0x53C00004
    /* 800DCBBC 24120001 */  addiu $s2, $zero, 1
    /* 800DCBC0 10000002 */  beq $zero, $zero, 0x800DCBCC
    /* 800DCBC4 24120016 */  addiu $s2, $zero, 22
    /* 800DCBC8 24120001 */  addiu $s2, $zero, 1
    /* 800DCBCC 0C02DD28 */  jal 0x800B74A0
    /* 800DCBD0 02402025 */  or $a0, $s2, $zero
    /* 800DCBD4 13C00003 */  beq $fp, $zero, 0x800DCBE4
    /* 800DCBD8 00172400 */  sll $a0, $s7, 16
    /* 800DCBDC 10000002 */  beq $zero, $zero, 0x800DCBE8
    /* 800DCBE0 24120001 */  addiu $s2, $zero, 1
    /* 800DCBE4 00009025 */  or $s2, $zero, $zero
    /* 800DCBE8 8FAE002C */  lw $t6, 44($sp)
    /* 800DCBEC 0232C823 */  subu $t9, $s1, $s2
    /* 800DCBF0 0004C403 */  sra $t8, $a0, 16
    /* 800DCBF4 032E2821 */  addu $a1, $t9, $t6
    /* 800DCBF8 00057C00 */  sll $t7, $a1, 16
    /* 800DCBFC 000F2C03 */  sra $a1, $t7, 16
    /* 800DCC00 03002025 */  or $a0, $t8, $zero
    /* 800DCC04 0C02DC75 */  jal 0x800B71D4
    /* 800DCC08 8FA60048 */  lw $a2, 72($sp)
    /* 800DCC0C 16A00003 */  bne $s5, $zero, 0x800DCC1C
    /* 800DCC10 02802025 */  or $a0, $s4, $zero
    /* 800DCC14 13C00003 */  beq $fp, $zero, 0x800DCC24
    /* 800DCC18 2412000A */  addiu $s2, $zero, 10
    /* 800DCC1C 10000001 */  beq $zero, $zero, 0x800DCC24
    /* 800DCC20 2412000B */  addiu $s2, $zero, 11
    /* 800DCC24 00002825 */  or $a1, $zero, $zero
    /* 800DCC28 0C001C9C */  jal 0x80007270
    /* 800DCC2C 24060001 */  addiu $a2, $zero, 1
    /* 800DCC30 0C02D080 */  jal 0x800B4200
    /* 800DCC34 AFB10030 */  sw $s1, 48($sp)
    /* 800DCC38 8FB10030 */  lw $s1, 48($sp)
    /* 800DCC3C 00408025 */  or $s0, $v0, $zero
    /* 800DCC40 02802025 */  or $a0, $s4, $zero
    /* 800DCC44 00002825 */  or $a1, $zero, $zero
    /* 800DCC48 0C001D78 */  jal 0x800075E0
    /* 800DCC4C 00003025 */  or $a2, $zero, $zero
    /* 800DCC50 16A00003 */  bne $s5, $zero, 0x800DCC60
    /* 800DCC54 00000000 */  nop
    /* 800DCC58 13C00003 */  beq $fp, $zero, 0x800DCC68
    /* 800DCC5C 24120016 */  addiu $s2, $zero, 22
    /* 800DCC60 10000001 */  beq $zero, $zero, 0x800DCC68
    /* 800DCC64 24120001 */  addiu $s2, $zero, 1
    /* 800DCC68 0C02DD28 */  jal 0x800B74A0
    /* 800DCC6C 02402025 */  or $a0, $s2, $zero
    /* 800DCC70 16A00003 */  bne $s5, $zero, 0x800DCC80
    /* 800DCC74 00172400 */  sll $a0, $s7, 16
    /* 800DCC78 13C00003 */  beq $fp, $zero, 0x800DCC88
    /* 800DCC7C 24120001 */  addiu $s2, $zero, 1
    /* 800DCC80 10000001 */  beq $zero, $zero, 0x800DCC88
    /* 800DCC84 00009025 */  or $s2, $zero, $zero
    /* 800DCC88 8FAF002C */  lw $t7, 44($sp)
    /* 800DCC8C 0004CC03 */  sra $t9, $a0, 16
    /* 800DCC90 02327023 */  subu $t6, $s1, $s2
    /* 800DCC94 000FC040 */  sll $t8, $t7, 1
    /* 800DCC98 01D82821 */  addu $a1, $t6, $t8
    /* 800DCC9C 03202025 */  or $a0, $t9, $zero
    /* 800DCCA0 0005CC00 */  sll $t9, $a1, 16
    /* 800DCCA4 00192C03 */  sra $a1, $t9, 16
    /* 800DCCA8 0C02DC75 */  jal 0x800B71D4
    /* 800DCCAC 8FA6004C */  lw $a2, 76($sp)
    /* 800DCCB0 8FBF0024 */  lw $ra, 36($sp)
    /* 800DCCB4 3C028012 */  lui $v0, 0x8012
    /* 800DCCB8 24428E20 */  addiu $v0, $v0, -29152
    /* 800DCCBC 240E0003 */  addiu $t6, $zero, 3
    /* 800DCCC0 AC4E0004 */  sw $t6, 4($v0)
    /* 800DCCC4 AC400000 */  sw $zero, 0($v0)
    /* 800DCCC8 03E00008 */  jr $ra
    /* 800DCCCC 27BD0038 */  addiu $sp, $sp, 56
