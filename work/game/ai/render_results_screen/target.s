# Source: game_code.bin (decompressed)
# Address: 0x800D7DC4

glabel render_results_screen
    /* 800D7DC4 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800D7DC8 AFB70034 */  sw $s7, 52($sp)
    /* 800D7DCC AFB60030 */  sw $s6, 48($sp)
    /* 800D7DD0 AFBE0038 */  sw $fp, 56($sp)
    /* 800D7DD4 AFB5002C */  sw $s5, 44($sp)
    /* 800D7DD8 3C168015 */  lui $s6, 0x8015
    /* 800D7DDC 3C178015 */  lui $s7, 0x8015
    /* 800D7DE0 AFBF003C */  sw $ra, 60($sp)
    /* 800D7DE4 AFB40028 */  sw $s4, 40($sp)
    /* 800D7DE8 AFB30024 */  sw $s3, 36($sp)
    /* 800D7DEC AFB20020 */  sw $s2, 32($sp)
    /* 800D7DF0 AFB1001C */  sw $s1, 28($sp)
    /* 800D7DF4 AFB00018 */  sw $s0, 24($sp)
    /* 800D7DF8 26F7A10C */  addiu $s7, $s7, -24308
    /* 800D7DFC 26D6418C */  addiu $s6, $s6, 16780
    /* 800D7E00 0000A825 */  or $s5, $zero, $zero
    /* 800D7E04 241E0004 */  addiu $fp, $zero, 4
    /* 800D7E08 0015A600 */  sll $s4, $s5, 24
    /* 800D7E0C 00147603 */  sra $t6, $s4, 24
    /* 800D7E10 A2C00000 */  sb $zero, 0($s6)
    /* 800D7E14 A2E00000 */  sb $zero, 0($s7)
    /* 800D7E18 0C035F50 */  jal 0x800D7D40
    /* 800D7E1C 01C0A025 */  or $s4, $t6, $zero
    /* 800D7E20 26B50001 */  addiu $s5, $s5, 1
    /* 800D7E24 26D60001 */  addiu $s6, $s6, 1
    /* 800D7E28 16BEFFF7 */  bne $s5, $fp, 0x800D7E08
    /* 800D7E2C 26F70001 */  addiu $s7, $s7, 1
    /* 800D7E30 3C158011 */  lui $s5, 0x8011
    /* 800D7E34 26B53ED8 */  addiu $s5, $s5, 16088
    /* 800D7E38 8EA40000 */  lw $a0, 0($s5)
    /* 800D7E3C 50800005 */  .word 0x50800005
    /* 800D7E40 8FBF003C */  lw $ra, 60($sp)
    /* 800D7E44 0C02CD63 */  jal 0x800B358C
    /* 800D7E48 00000000 */  nop
    /* 800D7E4C AEA00000 */  sw $zero, 0($s5)
    /* 800D7E50 8FBF003C */  lw $ra, 60($sp)
    /* 800D7E54 3C018011 */  lui $at, 0x8011
    /* 800D7E58 8FB00018 */  lw $s0, 24($sp)
    /* 800D7E5C 8FB1001C */  lw $s1, 28($sp)
    /* 800D7E60 8FB20020 */  lw $s2, 32($sp)
    /* 800D7E64 8FB30024 */  lw $s3, 36($sp)
    /* 800D7E68 8FB40028 */  lw $s4, 40($sp)
    /* 800D7E6C 8FB5002C */  lw $s5, 44($sp)
    /* 800D7E70 8FB60030 */  lw $s6, 48($sp)
    /* 800D7E74 8FB70034 */  lw $s7, 52($sp)
    /* 800D7E78 8FBE0038 */  lw $fp, 56($sp)
    /* 800D7E7C A0203ED4 */  sb $zero, 16084($at)
    /* 800D7E80 03E00008 */  jr $ra
    /* 800D7E84 27BD0040 */  addiu $sp, $sp, 64
