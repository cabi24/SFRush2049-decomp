# Source: game_code.bin (decompressed)
# Address: 0x800D2A74

glabel lap_counter_display
    /* 800D2A74 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800D2A78 AFBF0014 */  sw $ra, 20($sp)
    /* 800D2A7C 00A04825 */  or $t1, $a1, $zero
    /* 800D2A80 14800022 */  bne $a0, $zero, 0x800D2B0C
    /* 800D2A84 00C04025 */  or $t0, $a2, $zero
    /* 800D2A88 00077080 */  sll $t6, $a3, 2
    /* 800D2A8C 3C068015 */  lui $a2, 0x8015
    /* 800D2A90 01C77021 */  addu $t6, $t6, $a3
    /* 800D2A94 24C61CE8 */  addiu $a2, $a2, 7400
    /* 800D2A98 000E7100 */  sll $t6, $t6, 4
    /* 800D2A9C 00CE7821 */  addu $t7, $a2, $t6
    /* 800D2AA0 00052040 */  sll $a0, $a1, 1
    /* 800D2AA4 01E4C021 */  addu $t8, $t7, $a0
    /* 800D2AA8 87050030 */  lh $a1, 48($t8)
    /* 800D2AAC 00A8082A */  slt $at, $a1, $t0
    /* 800D2AB0 10200014 */  beq $at, $zero, 0x800D2B04
    /* 800D2AB4 00000000 */  nop
    /* 800D2AB8 84C20002 */  lh $v0, 2($a2)
    /* 800D2ABC 00E2082A */  slt $at, $a3, $v0
    /* 800D2AC0 14200007 */  bne $at, $zero, 0x800D2AE0
    /* 800D2AC4 0002C880 */  sll $t9, $v0, 2
    /* 800D2AC8 0322C821 */  addu $t9, $t9, $v0
    /* 800D2ACC 0019C900 */  sll $t9, $t9, 4
    /* 800D2AD0 00D95021 */  addu $t2, $a2, $t9
    /* 800D2AD4 01445821 */  addu $t3, $t2, $a0
    /* 800D2AD8 10000002 */  beq $zero, $zero, 0x800D2AE4
    /* 800D2ADC 85630030 */  lh $v1, 48($t3)
    /* 800D2AE0 00001825 */  or $v1, $zero, $zero
    /* 800D2AE4 000960C0 */  sll $t4, $t1, 3
    /* 800D2AE8 3C0D8013 */  lui $t5, 0x8013
    /* 800D2AEC 01AC6821 */  addu $t5, $t5, $t4
    /* 800D2AF0 95ADE5E8 */  lhu $t5, -6680($t5)
    /* 800D2AF4 01A87023 */  subu $t6, $t5, $t0
    /* 800D2AF8 01C57821 */  addu $t7, $t6, $a1
    /* 800D2AFC 10000040 */  beq $zero, $zero, 0x800D2C00
    /* 800D2B00 01E31023 */  subu $v0, $t7, $v1
    /* 800D2B04 1000003E */  beq $zero, $zero, 0x800D2C00
    /* 800D2B08 00A81023 */  subu $v0, $a1, $t0
    /* 800D2B0C 0521001D */  bgez $t1, 0x800D2B84
    /* 800D2B10 3C068015 */  lui $a2, 0x8015
    /* 800D2B14 24050050 */  addiu $a1, $zero, 80
    /* 800D2B18 00E50019 */  multu $a3, $a1
    /* 800D2B1C 3C068015 */  lui $a2, 0x8015
    /* 800D2B20 24C61CE8 */  addiu $a2, $a2, 7400
    /* 800D2B24 0000C012 */  mflo $t8
    /* 800D2B28 00D8C821 */  addu $t9, $a2, $t8
    /* 800D2B2C 8724002E */  lh $a0, 46($t9)
    /* 800D2B30 0088082A */  slt $at, $a0, $t0
    /* 800D2B34 10200011 */  beq $at, $zero, 0x800D2B7C
    /* 800D2B38 00000000 */  nop
    /* 800D2B3C 84C20002 */  lh $v0, 2($a2)
    /* 800D2B40 00E2082A */  slt $at, $a3, $v0
    /* 800D2B44 54200007 */  .word 0x54200007
    /* 800D2B48 00001825 */  or $v1, $zero, $zero
    /* 800D2B4C 00450019 */  multu $v0, $a1
    /* 800D2B50 00005012 */  mflo $t2
    /* 800D2B54 00CA5821 */  addu $t3, $a2, $t2
    /* 800D2B58 10000002 */  beq $zero, $zero, 0x800D2B64
    /* 800D2B5C 8563002E */  lh $v1, 46($t3)
    /* 800D2B60 00001825 */  or $v1, $zero, $zero
    /* 800D2B64 3C0C8014 */  lui $t4, 0x8014
    /* 800D2B68 958C07F0 */  lhu $t4, 2032($t4)
    /* 800D2B6C 01886823 */  subu $t5, $t4, $t0
    /* 800D2B70 01A47021 */  addu $t6, $t5, $a0
    /* 800D2B74 10000022 */  beq $zero, $zero, 0x800D2C00
    /* 800D2B78 01C31023 */  subu $v0, $t6, $v1
    /* 800D2B7C 10000020 */  beq $zero, $zero, 0x800D2C00
    /* 800D2B80 00881023 */  subu $v0, $a0, $t0
    /* 800D2B84 00077880 */  sll $t7, $a3, 2
    /* 800D2B88 01E77821 */  addu $t7, $t7, $a3
    /* 800D2B8C 24C61CE8 */  addiu $a2, $a2, 7400
    /* 800D2B90 000F7900 */  sll $t7, $t7, 4
    /* 800D2B94 00CFC021 */  addu $t8, $a2, $t7
    /* 800D2B98 0009C840 */  sll $t9, $t1, 1
    /* 800D2B9C 03195021 */  addu $t2, $t8, $t9
    /* 800D2BA0 85430038 */  lh $v1, 56($t2)
    /* 800D2BA4 3C0B8014 */  lui $t3, 0x8014
    /* 800D2BA8 04600005 */  bltz $v1, 0x800D2BC0
    /* 800D2BAC 0068082A */  slt $at, $v1, $t0
    /* 800D2BB0 14200003 */  bne $at, $zero, 0x800D2BC0
    /* 800D2BB4 00000000 */  nop
    /* 800D2BB8 10000011 */  beq $zero, $zero, 0x800D2C00
    /* 800D2BBC 00681023 */  subu $v0, $v1, $t0
    /* 800D2BC0 8D6B07FC */  lw $t3, 2044($t3)
    /* 800D2BC4 00091900 */  sll $v1, $t1, 4
    /* 800D2BC8 01631021 */  addu $v0, $t3, $v1
    /* 800D2BCC 80450004 */  lb $a1, 4($v0)
    /* 800D2BD0 94460006 */  lhu $a2, 6($v0)
    /* 800D2BD4 AFA80028 */  sw $t0, 40($sp)
    /* 800D2BD8 0C034A9D */  jal 0x800D2A74
    /* 800D2BDC AFA30018 */  sw $v1, 24($sp)
    /* 800D2BE0 3C0C8014 */  lui $t4, 0x8014
    /* 800D2BE4 8D8C07FC */  lw $t4, 2044($t4)
    /* 800D2BE8 8FA30018 */  lw $v1, 24($sp)
    /* 800D2BEC 8FA80028 */  lw $t0, 40($sp)
    /* 800D2BF0 01836821 */  addu $t5, $t4, $v1
    /* 800D2BF4 95AE000A */  lhu $t6, 10($t5)
    /* 800D2BF8 004E7821 */  addu $t7, $v0, $t6
    /* 800D2BFC 01E81023 */  subu $v0, $t7, $t0
    /* 800D2C00 8FBF0014 */  lw $ra, 20($sp)
    /* 800D2C04 27BD0020 */  addiu $sp, $sp, 32
    /* 800D2C08 03E00008 */  jr $ra
    /* 800D2C0C 00000000 */  nop
