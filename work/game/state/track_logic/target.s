# Source: game_code.bin (decompressed)
# Address: 0x800C8A00

glabel track_logic
    /* 800C8A00 0C001D78 */  jal 0x800075E0
    /* 800C8A04 00003025 */  or $a2, $zero, $zero
    /* 800C8A08 3C048014 */  lui $a0, 0x8014
    /* 800C8A0C 248427A8 */  addiu $a0, $a0, 10152
    /* 800C8A10 8FA5008C */  lw $a1, 140($sp)
    /* 800C8A14 0C001D78 */  jal 0x800075E0
    /* 800C8A18 00003025 */  or $a2, $zero, $zero
    /* 800C8A1C 3C048014 */  lui $a0, 0x8014
    /* 800C8A20 24842728 */  addiu $a0, $a0, 10024
    /* 800C8A24 00002825 */  or $a1, $zero, $zero
    /* 800C8A28 0C001C9C */  jal 0x80007270
    /* 800C8A2C 24060001 */  addiu $a2, $zero, 1
    /* 800C8A30 0C0246C0 */  jal 0x80091B00
    /* 800C8A34 00000000 */  nop
    /* 800C8A38 AFA2006C */  sw $v0, 108($sp)
    /* 800C8A3C 24180007 */  addiu $t8, $zero, 7
    /* 800C8A40 3C048014 */  lui $a0, 0x8014
    /* 800C8A44 24842728 */  addiu $a0, $a0, 10024
    /* 800C8A48 A0580002 */  sb $t8, 2($v0)
    /* 800C8A4C 00002825 */  or $a1, $zero, $zero
    /* 800C8A50 0C001D78 */  jal 0x800075E0
    /* 800C8A54 00003025 */  or $a2, $zero, $zero
    /* 800C8A58 3C048014 */  lui $a0, 0x8014
    /* 800C8A5C 248427A8 */  addiu $a0, $a0, 10152
    /* 800C8A60 8FA5006C */  lw $a1, 108($sp)
    /* 800C8A64 0C001D78 */  jal 0x800075E0
    /* 800C8A68 00003025 */  or $a2, $zero, $zero
    /* 800C8A6C 3C198014 */  lui $t9, 0x8014
    /* 800C8A70 272339D8 */  addiu $v1, $t9, 14808
    /* 800C8A74 00602825 */  or $a1, $v1, $zero
    /* 800C8A78 3C028014 */  lui $v0, 0x8014
    /* 800C8A7C 24422DD8 */  addiu $v0, $v0, 11736
    /* 800C8A80 804E0003 */  lb $t6, 3($v0)
    /* 800C8A84 51C00004 */  .word 0x51C00004
    /* 800C8A88 24420018 */  addiu $v0, $v0, 24
    /* 800C8A8C 10000005 */  beq $zero, $zero, 0x800C8AA4
    /* 800C8A90 0045082B */  sltu $at, $v0, $a1
    /* 800C8A94 24420018 */  addiu $v0, $v0, 24
    /* 800C8A98 5443FFFA */  .word 0x5443FFFA
    /* 800C8A9C 804E0003 */  lb $t6, 3($v0)
    /* 800C8AA0 0045082B */  sltu $at, $v0, $a1
    /* 800C8AA4 1420FFF4 */  bne $at, $zero, 0x800C8A78
    /* 800C8AA8 00000000 */  nop
    /* 800C8AAC 3C018011 */  lui $at, 0x8011
    /* 800C8AB0 0C029315 */  jal 0x800A4C54
    /* 800C8AB4 A020023C */  sb $zero, 572($at)
    /* 800C8AB8 3C0F8011 */  lui $t7, 0x8011
    /* 800C8ABC 8DEF0244 */  lw $t7, 580($t7)
    /* 800C8AC0 3C048015 */  lui $a0, 0x8015
    /* 800C8AC4 24842770 */  addiu $a0, $a0, 10096
    /* 800C8AC8 00002825 */  or $a1, $zero, $zero
    /* 800C8ACC 24060001 */  addiu $a2, $zero, 1
    /* 800C8AD0 0C001C9C */  jal 0x80007270
    /* 800C8AD4 AFAF0050 */  sw $t7, 80($sp)
    /* 800C8AD8 8FA50050 */  lw $a1, 80($sp)
    /* 800C8ADC 0C0257F6 */  jal 0x80095FD8
    /* 800C8AE0 00003025 */  or $a2, $zero, $zero
    /* 800C8AE4 3C048015 */  lui $a0, 0x8015
    /* 800C8AE8 24842770 */  addiu $a0, $a0, 10096
    /* 800C8AEC 00002825 */  or $a1, $zero, $zero
    /* 800C8AF0 0C001D78 */  jal 0x800075E0
    /* 800C8AF4 00003025 */  or $a2, $zero, $zero
    /* 800C8AF8 3C188011 */  lui $t8, 0x8011
    /* 800C8AFC 8F180248 */  lw $t8, 584($t8)
    /* 800C8B00 3C048015 */  lui $a0, 0x8015
    /* 800C8B04 24842770 */  addiu $a0, $a0, 10096
    /* 800C8B08 00002825 */  or $a1, $zero, $zero
    /* 800C8B0C 24060001 */  addiu $a2, $zero, 1
    /* 800C8B10 0C001C9C */  jal 0x80007270
    /* 800C8B14 AFB80038 */  sw $t8, 56($sp)
    /* 800C8B18 8FA50038 */  lw $a1, 56($sp)
    /* 800C8B1C 0C0257F6 */  jal 0x80095FD8
    /* 800C8B20 00003025 */  or $a2, $zero, $zero
    /* 800C8B24 3C048015 */  lui $a0, 0x8015
    /* 800C8B28 24842770 */  addiu $a0, $a0, 10096
    /* 800C8B2C 00002825 */  or $a1, $zero, $zero
    /* 800C8B30 0C001D78 */  jal 0x800075E0
    /* 800C8B34 00003025 */  or $a2, $zero, $zero
    /* 800C8B38 3C198011 */  lui $t9, 0x8011
    /* 800C8B3C 8F390270 */  lw $t9, 624($t9)
    /* 800C8B40 3C048015 */  lui $a0, 0x8015
    /* 800C8B44 24842770 */  addiu $a0, $a0, 10096
    /* 800C8B48 00002825 */  or $a1, $zero, $zero
    /* 800C8B4C 24060001 */  addiu $a2, $zero, 1
    /* 800C8B50 0C001C9C */  jal 0x80007270
    /* 800C8B54 AFB90020 */  sw $t9, 32($sp)
    /* 800C8B58 8FA50020 */  lw $a1, 32($sp)
    /* 800C8B5C 0C0257F6 */  jal 0x80095FD8
    /* 800C8B60 00003025 */  or $a2, $zero, $zero
    /* 800C8B64 3C048015 */  lui $a0, 0x8015
    /* 800C8B68 24842770 */  addiu $a0, $a0, 10096
    /* 800C8B6C 00002825 */  or $a1, $zero, $zero
    /* 800C8B70 0C001D78 */  jal 0x800075E0
    /* 800C8B74 00003025 */  or $a2, $zero, $zero
    /* 800C8B78 8FBF001C */  lw $ra, 28($sp)
    /* 800C8B7C 8FB00014 */  lw $s0, 20($sp)
    /* 800C8B80 8FB10018 */  lw $s1, 24($sp)
    /* 800C8B84 03E00008 */  jr $ra
    /* 800C8B88 27BD00D0 */  addiu $sp, $sp, 208
