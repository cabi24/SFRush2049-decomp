# Source: game_code.bin (decompressed)
# Address: 0x800A501C

glabel suspension_setup
    /* 800A501C 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800A5020 00803825 */  or $a3, $a0, $zero
    /* 800A5024 AFBF003C */  sw $ra, 60($sp)
    /* 800A5028 00A04025 */  or $t0, $a1, $zero
    /* 800A502C 00A02025 */  or $a0, $a1, $zero
    /* 800A5030 AFBE0038 */  sw $fp, 56($sp)
    /* 800A5034 AFB70034 */  sw $s7, 52($sp)
    /* 800A5038 AFB60030 */  sw $s6, 48($sp)
    /* 800A503C AFB5002C */  sw $s5, 44($sp)
    /* 800A5040 AFB40028 */  sw $s4, 40($sp)
    /* 800A5044 AFB30024 */  sw $s3, 36($sp)
    /* 800A5048 AFB20020 */  sw $s2, 32($sp)
    /* 800A504C AFB1001C */  sw $s1, 28($sp)
    /* 800A5050 AFB00018 */  sw $s0, 24($sp)
    /* 800A5054 0C025DA5 */  jal 0x80097694
    /* 800A5058 2405FFFF */  addiu $a1, $zero, -1
    /* 800A505C 0441001F */  bgez $v0, 0x800A50DC
    /* 800A5060 00E02025 */  or $a0, $a3, $zero
    /* 800A5064 0C025DA5 */  jal 0x80097694
    /* 800A5068 2405FFFF */  addiu $a1, $zero, -1
    /* 800A506C 0440001B */  bltz $v0, 0x800A50DC
    /* 800A5070 00403825 */  or $a3, $v0, $zero
    /* 800A5074 3C028003 */  lui $v0, 0x8003
    /* 800A5078 2442EB70 */  addiu $v0, $v0, -5264
    /* 800A507C 844E0000 */  lh $t6, 0($v0)
    /* 800A5080 00E02025 */  or $a0, $a3, $zero
    /* 800A5084 24050001 */  addiu $a1, $zero, 1
    /* 800A5088 11C00004 */  beq $t6, $zero, 0x800A509C
    /* 800A508C 00000000 */  nop
    /* 800A5090 844F0000 */  lh $t7, 0($v0)
    /* 800A5094 55E0FFFF */  .word 0x55E0FFFF
    /* 800A5098 844F0000 */  lh $t7, 0($v0)
    /* 800A509C 0C0258A2 */  jal 0x80096288
    /* 800A50A0 24060001 */  addiu $a2, $zero, 1
    /* 800A50A4 0007C080 */  sll $t8, $a3, 2
    /* 800A50A8 0307C021 */  addu $t8, $t8, $a3
    /* 800A50AC 3C198015 */  lui $t9, 0x8015
    /* 800A50B0 27396D38 */  addiu $t9, $t9, 27960
    /* 800A50B4 0018C080 */  sll $t8, $t8, 2
    /* 800A50B8 03191021 */  addu $v0, $t8, $t9
    /* 800A50BC A0400000 */  sb $zero, 0($v0)
    /* 800A50C0 A0400001 */  sb $zero, 1($v0)
    /* 800A50C4 A0480006 */  sb $t0, 6($v0)
    /* 800A50C8 A0400004 */  sb $zero, 4($v0)
    /* 800A50CC 0C025CB1 */  jal 0x800972C4
    /* 800A50D0 00E02025 */  or $a0, $a3, $zero
    /* 800A50D4 0C025C59 */  jal 0x80097164
    /* 800A50D8 00E0F025 */  or $fp, $a3, $zero
    /* 800A50DC 8FBF003C */  lw $ra, 60($sp)
    /* 800A50E0 8FB00018 */  lw $s0, 24($sp)
    /* 800A50E4 8FB1001C */  lw $s1, 28($sp)
    /* 800A50E8 8FB20020 */  lw $s2, 32($sp)
    /* 800A50EC 8FB30024 */  lw $s3, 36($sp)
    /* 800A50F0 8FB40028 */  lw $s4, 40($sp)
    /* 800A50F4 8FB5002C */  lw $s5, 44($sp)
    /* 800A50F8 8FB60030 */  lw $s6, 48($sp)
    /* 800A50FC 8FB70034 */  lw $s7, 52($sp)
    /* 800A5100 8FBE0038 */  lw $fp, 56($sp)
    /* 800A5104 03E00008 */  jr $ra
    /* 800A5108 27BD0040 */  addiu $sp, $sp, 64
