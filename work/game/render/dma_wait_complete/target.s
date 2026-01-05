# Source: game_code.bin (decompressed)
# Address: 0x8008AA40

glabel dma_wait_complete
    /* 8008AA40 27BDFF98 */  addiu $sp, $sp, -104
    /* 8008AA44 AFBE0040 */  sw $fp, 64($sp)
    /* 8008AA48 3C1E8015 */  lui $fp, 0x8015
    /* 8008AA4C 27DE2788 */  addiu $fp, $fp, 10120
    /* 8008AA50 AFBF0044 */  sw $ra, 68($sp)
    /* 8008AA54 AFA40068 */  sw $a0, 104($sp)
    /* 8008AA58 3C058015 */  lui $a1, 0x8015
    /* 8008AA5C AFB7003C */  sw $s7, 60($sp)
    /* 8008AA60 AFB60038 */  sw $s6, 56($sp)
    /* 8008AA64 AFB50034 */  sw $s5, 52($sp)
    /* 8008AA68 AFB40030 */  sw $s4, 48($sp)
    /* 8008AA6C AFB3002C */  sw $s3, 44($sp)
    /* 8008AA70 AFB20028 */  sw $s2, 40($sp)
    /* 8008AA74 AFB10024 */  sw $s1, 36($sp)
    /* 8008AA78 AFB00020 */  sw $s0, 32($sp)
    /* 8008AA7C F7B40018 */  .word 0xF7B40018
    /* 8008AA80 AFA0005C */  sw $zero, 92($sp)
    /* 8008AA84 24A527A8 */  addiu $a1, $a1, 10152
    /* 8008AA88 03C02025 */  or $a0, $fp, $zero
    /* 8008AA8C 0C001A80 */  jal 0x80006A00
    /* 8008AA90 24060008 */  addiu $a2, $zero, 8
    /* 8008AA94 3C108003 */  lui $s0, 0x8003
    /* 8008AA98 2610E8E8 */  addiu $s0, $s0, -5912
    /* 8008AA9C 02002025 */  or $a0, $s0, $zero
    /* 8008AAA0 27A50060 */  addiu $a1, $sp, 96
    /* 8008AAA4 0C000175 */  jal 0x800005D4
    /* 8008AAA8 03C03025 */  or $a2, $fp, $zero
    /* 8008AAAC 44802000 */  .word 0x44802000
    /* 8008AAB0 3C118015 */  lui $s1, 0x8015
    /* 8008AAB4 3C014661 */  lui $at, 0x4661
    /* 8008AAB8 26312748 */  addiu $s1, $s1, 10056
    /* 8008AABC 3C178003 */  lui $s7, 0x8003
    /* 8008AAC0 3C168003 */  lui $s6, 0x8003
    /* 8008AAC4 3C158012 */  lui $s5, 0x8012
    /* 8008AAC8 4481A000 */  .word 0x4481A000
    /* 8008AACC 26B5EAA8 */  addiu $s5, $s5, -5464
    /* 8008AAD0 26D6AFA0 */  addiu $s6, $s6, -20576
    /* 8008AAD4 26F7AFB8 */  addiu $s7, $s7, -20552
    /* 8008AAD8 24140004 */  addiu $s4, $zero, 4
    /* 8008AADC 24130001 */  addiu $s3, $zero, 1
    /* 8008AAE0 27B2005C */  addiu $s2, $sp, 92
    /* 8008AAE4 E6240000 */  swc1 $f4, 0($s1)
    /* 8008AAE8 03C02025 */  or $a0, $fp, $zero
    /* 8008AAEC 02402825 */  or $a1, $s2, $zero
    /* 8008AAF0 0C001C9C */  jal 0x80007270
    /* 8008AAF4 02603025 */  or $a2, $s3, $zero
    /* 8008AAF8 8FAE005C */  lw $t6, 92($sp)
    /* 8008AAFC 85C20000 */  lh $v0, 0($t6)
    /* 8008AB00 50530006 */  .word 0x50530006
    /* 8008AB04 8EA40000 */  lw $a0, 0($s5)
    /* 8008AB08 1054001F */  beq $v0, $s4, 0x8008AB88
    /* 8008AB0C 00000000 */  nop
    /* 8008AB10 1000FFF6 */  beq $zero, $zero, 0x8008AAEC
    /* 8008AB14 03C02025 */  or $a0, $fp, $zero
    /* 8008AB18 8EA40000 */  lw $a0, 0($s5)
    /* 8008AB1C 5080000F */  .word 0x5080000F
    /* 8008AB20 C6260000 */  lwc1 $f6, 0($s1)
    /* 8008AB24 8EC20000 */  lw $v0, 0($s6)
    /* 8008AB28 10400009 */  beq $v0, $zero, 0x8008AB50
    /* 8008AB2C 00000000 */  nop
    /* 8008AB30 8E0F0274 */  lw $t7, 628($s0)
    /* 8008AB34 15E20006 */  bne $t7, $v0, 0x8008AB50
    /* 8008AB38 00000000 */  nop
    /* 8008AB3C 10400004 */  beq $v0, $zero, 0x8008AB50
    /* 8008AB40 00000000 */  nop
    /* 8008AB44 8E180274 */  lw $t8, 628($s0)
    /* 8008AB48 1302FFFC */  beq $t8, $v0, 0x8008AB3C
    /* 8008AB4C 00000000 */  nop
    /* 8008AB50 0080F809 */  jalr $ra, $a0
    /* 8008AB54 00000000 */  nop
    /* 8008AB58 C6260000 */  lwc1 $f6, 0($s1)
    /* 8008AB5C C6E80000 */  lwc1 $f8, 0($s7)
    /* 8008AB60 46083280 */  .word 0x46083280
    /* 8008AB64 E62A0000 */  swc1 $f10, 0($s1)
    /* 8008AB68 C6200000 */  lwc1 $f0, 0($s1)
    /* 8008AB6C 4600A03C */  .word 0x4600A03C
    /* 8008AB70 00000000 */  nop
    /* 8008AB74 4502FFDD */  .word 0x4502FFDD
    /* 8008AB78 03C02025 */  or $a0, $fp, $zero
    /* 8008AB7C 46140401 */  .word 0x46140401
    /* 8008AB80 1000FFD9 */  beq $zero, $zero, 0x8008AAE8
    /* 8008AB84 E6300000 */  swc1 $f16, 0($s1)
    /* 8008AB88 0C008179 */  jal 0x800205E4
    /* 8008AB8C 00000000 */  nop
    /* 8008AB90 1000FFD6 */  beq $zero, $zero, 0x8008AAEC
    /* 8008AB94 03C02025 */  or $a0, $fp, $zero
    /* 8008AB98 00000000 */  nop
    /* 8008AB9C 00000000 */  nop
    /* 8008ABA0 00000000 */  nop
    /* 8008ABA4 00000000 */  nop
    /* 8008ABA8 00000000 */  nop
    /* 8008ABAC 00000000 */  nop
    /* 8008ABB0 8FBF0044 */  lw $ra, 68($sp)
    /* 8008ABB4 D7B40018 */  .word 0xD7B40018
    /* 8008ABB8 8FB00020 */  lw $s0, 32($sp)
    /* 8008ABBC 8FB10024 */  lw $s1, 36($sp)
    /* 8008ABC0 8FB20028 */  lw $s2, 40($sp)
    /* 8008ABC4 8FB3002C */  lw $s3, 44($sp)
    /* 8008ABC8 8FB40030 */  lw $s4, 48($sp)
    /* 8008ABCC 8FB50034 */  lw $s5, 52($sp)
    /* 8008ABD0 8FB60038 */  lw $s6, 56($sp)
    /* 8008ABD4 8FB7003C */  lw $s7, 60($sp)
    /* 8008ABD8 8FBE0040 */  lw $fp, 64($sp)
    /* 8008ABDC 03E00008 */  jr $ra
    /* 8008ABE0 27BD0068 */  addiu $sp, $sp, 104
