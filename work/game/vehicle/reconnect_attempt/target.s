# Source: game_code.bin (decompressed)
# Address: 0x8010BE7C

glabel reconnect_attempt
    /* 8010BE7C 02148023 */  subu $s0, $s0, $s4
    /* 8010BE80 000E7140 */  sll $t6, $t6, 5
    /* 8010BE84 02CE7821 */  addu $t7, $s6, $t6
    /* 8010BE88 001080C0 */  sll $s0, $s0, 3
    /* 8010BE8C 01F0C021 */  addu $t8, $t7, $s0
    /* 8010BE90 0C02CFD4 */  jal 0x800B3F50
    /* 8010BE94 8715FFB2 */  lh $s5, -78($t8)
    /* 8010BE98 87D90000 */  lh $t9, 0($fp)
    /* 8010BE9C 3C048011 */  lui $a0, 0x8011
    /* 8010BEA0 00197080 */  sll $t6, $t9, 2
    /* 8010BEA4 01D97023 */  subu $t6, $t6, $t9
    /* 8010BEA8 000E7140 */  sll $t6, $t6, 5
    /* 8010BEAC 02CE7821 */  addu $t7, $s6, $t6
    /* 8010BEB0 01F0C021 */  addu $t8, $t7, $s0
    /* 8010BEB4 8F19FFB4 */  lw $t9, -76($t8)
    /* 8010BEB8 0014C080 */  sll $t8, $s4, 2
    /* 8010BEBC 00982021 */  addu $a0, $a0, $t8
    /* 8010BEC0 03229023 */  subu $s2, $t9, $v0
    /* 8010BEC4 00127400 */  sll $t6, $s2, 16
    /* 8010BEC8 000E9403 */  sra $s2, $t6, 16
    /* 8010BECC 0C02DD28 */  jal 0x800B74A0
    /* 8010BED0 8C8446AC */  lw $a0, 18092($a0)
    /* 8010BED4 0014C880 */  sll $t9, $s4, 2
    /* 8010BED8 0334C821 */  addu $t9, $t9, $s4
    /* 8010BEDC 0019C880 */  sll $t9, $t9, 2
    /* 8010BEE0 0334C823 */  subu $t9, $t9, $s4
    /* 8010BEE4 3C0E8015 */  lui $t6, 0x8015
    /* 8010BEE8 25CEA118 */  addiu $t6, $t6, -24296
    /* 8010BEEC 0019C880 */  sll $t9, $t9, 2
    /* 8010BEF0 032E8821 */  addu $s1, $t9, $t6
    /* 8010BEF4 8E2F0048 */  lw $t7, 72($s1)
    /* 8010BEF8 2405FFFF */  addiu $a1, $zero, -1
    /* 8010BEFC 8DF00000 */  lw $s0, 0($t7)
    /* 8010BF00 26100014 */  addiu $s0, $s0, 20
    /* 8010BF04 0C02CFE9 */  jal 0x800B3FA4
    /* 8010BF08 02002025 */  or $a0, $s0, $zero
    /* 8010BF0C 0002C042 */  srl $t8, $v0, 1
    /* 8010BF10 02B87023 */  subu $t6, $s5, $t8
    /* 8010BF14 000E2400 */  sll $a0, $t6, 16
    /* 8010BF18 00122C00 */  sll $a1, $s2, 16
    /* 8010BF1C 0005C403 */  sra $t8, $a1, 16
    /* 8010BF20 00047C03 */  sra $t7, $a0, 16
    /* 8010BF24 01E02025 */  or $a0, $t7, $zero
    /* 8010BF28 03002825 */  or $a1, $t8, $zero
    /* 8010BF2C 02A09825 */  or $s3, $s5, $zero
    /* 8010BF30 0C02DC75 */  jal 0x800B71D4
    /* 8010BF34 02003025 */  or $a2, $s0, $zero
    /* 8010BF38 0C02CFD4 */  jal 0x800B3F50
    /* 8010BF3C 00000000 */  nop
    /* 8010BF40 92230001 */  lbu $v1, 1($s1)
    /* 8010BF44 02429021 */  addu $s2, $s2, $v0
    /* 8010BF48 3C028014 */  lui $v0, 0x8014
    /* 8010BF4C 00037880 */  sll $t7, $v1, 2
    /* 8010BF50 01E37823 */  subu $t7, $t7, $v1
    /* 8010BF54 000F7980 */  sll $t7, $t7, 6
    /* 8010BF58 01E37821 */  addu $t7, $t7, $v1
    /* 8010BF5C 000F7880 */  sll $t7, $t7, 2
    /* 8010BF60 004F1021 */  addu $v0, $v0, $t7
    /* 8010BF64 80424036 */  lb $v0, 16438($v0)
    /* 8010BF68 0012CC00 */  sll $t9, $s2, 16
    /* 8010BF6C 00199403 */  sra $s2, $t9, 16
    /* 8010BF70 50400005 */  .word 0x50400005
    /* 8010BF74 8EF90004 */  lw $t9, 4($s7)
    /* 8010BF78 8EF80004 */  lw $t8, 4($s7)
    /* 8010BF7C 10000003 */  beq $zero, $zero, 0x8010BF8C
    /* 8010BF80 8F1002AC */  lw $s0, 684($t8)
    /* 8010BF84 8EF90004 */  lw $t9, 4($s7)
    /* 8010BF88 8F3002A8 */  lw $s0, 680($t9)
    /* 8010BF8C 0C02DD28 */  jal 0x800B74A0
    /* 8010BF90 24040001 */  addiu $a0, $zero, 1
    /* 8010BF94 02002025 */  or $a0, $s0, $zero
    /* 8010BF98 0C02CFE9 */  jal 0x800B3FA4
    /* 8010BF9C 2405FFFF */  addiu $a1, $zero, -1
    /* 8010BFA0 00027042 */  srl $t6, $v0, 1
    /* 8010BFA4 026EC023 */  subu $t8, $s3, $t6
    /* 8010BFA8 00182400 */  sll $a0, $t8, 16
    /* 8010BFAC 00122C00 */  sll $a1, $s2, 16
    /* 8010BFB0 00057403 */  sra $t6, $a1, 16
    /* 8010BFB4 0004CC03 */  sra $t9, $a0, 16
    /* 8010BFB8 03202025 */  or $a0, $t9, $zero
    /* 8010BFBC 01C02825 */  or $a1, $t6, $zero
    /* 8010BFC0 0C02DC75 */  jal 0x800B71D4
    /* 8010BFC4 02003025 */  or $a2, $s0, $zero
    /* 8010BFC8 87C40000 */  lh $a0, 0($fp)
    /* 8010BFCC 26940001 */  addiu $s4, $s4, 1
    /* 8010BFD0 328FFFFF */  andi $t7, $s4, 0xFFFF
    /* 8010BFD4 01E4082A */  slt $at, $t7, $a0
    /* 8010BFD8 1420FFA5 */  bne $at, $zero, 0x8010BE70
    /* 8010BFDC 01E0A025 */  or $s4, $t7, $zero
    /* 8010BFE0 3C01BF80 */  lui $at, 0xBF80
    /* 8010BFE4 44816000 */  .word 0x44816000
    /* 8010BFE8 0C02D96D */  jal 0x800B65B4
    /* 8010BFEC 00000000 */  nop
    /* 8010BFF0 24020001 */  addiu $v0, $zero, 1
    /* 8010BFF4 8FBF003C */  lw $ra, 60($sp)
    /* 8010BFF8 8FB00018 */  lw $s0, 24($sp)
    /* 8010BFFC 8FB1001C */  lw $s1, 28($sp)
    /* 8010C000 8FB20020 */  lw $s2, 32($sp)
    /* 8010C004 8FB30024 */  lw $s3, 36($sp)
    /* 8010C008 8FB40028 */  lw $s4, 40($sp)
    /* 8010C00C 8FB5002C */  lw $s5, 44($sp)
    /* 8010C010 8FB60030 */  lw $s6, 48($sp)
    /* 8010C014 8FB70034 */  lw $s7, 52($sp)
    /* 8010C018 8FBE0038 */  lw $fp, 56($sp)
    /* 8010C01C 03E00008 */  jr $ra
    /* 8010C020 27BD0040 */  addiu $sp, $sp, 64
