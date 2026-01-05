# Source: game_code.bin (decompressed)
# Address: 0x800EF500

glabel fire_effect
    /* 800EF500 A3AE00B4 */  sb $t6, 180($sp)
    /* 800EF504 0C02DD28 */  jal 0x800B74A0
    /* 800EF508 A3A000B5 */  sb $zero, 181($sp)
    /* 800EF50C 26040001 */  addiu $a0, $s0, 1
    /* 800EF510 26250001 */  addiu $a1, $s1, 1
    /* 800EF514 0005CC00 */  sll $t9, $a1, 16
    /* 800EF518 00047C00 */  sll $t7, $a0, 16
    /* 800EF51C 000F2403 */  sra $a0, $t7, 16
    /* 800EF520 00192C03 */  sra $a1, $t9, 16
    /* 800EF524 0C02DC75 */  jal 0x800B71D4
    /* 800EF528 03C03025 */  or $a2, $fp, $zero
    /* 800EF52C 0C02DD28 */  jal 0x800B74A0
    /* 800EF530 2404000E */  addiu $a0, $zero, 14
    /* 800EF534 00102400 */  sll $a0, $s0, 16
    /* 800EF538 00112C00 */  sll $a1, $s1, 16
    /* 800EF53C 0005C403 */  sra $t8, $a1, 16
    /* 800EF540 00047C03 */  sra $t7, $a0, 16
    /* 800EF544 01E02025 */  or $a0, $t7, $zero
    /* 800EF548 03002825 */  or $a1, $t8, $zero
    /* 800EF54C 0C02DC75 */  jal 0x800B71D4
    /* 800EF550 03C03025 */  or $a2, $fp, $zero
    /* 800EF554 3C038015 */  lui $v1, 0x8015
    /* 800EF558 84631AD0 */  lh $v1, 6864($v1)
    /* 800EF55C 26940001 */  addiu $s4, $s4, 1
    /* 800EF560 0283082A */  slt $at, $s4, $v1
    /* 800EF564 1420FF92 */  bne $at, $zero, 0x800EF3B0
    /* 800EF568 26B50808 */  addiu $s5, $s5, 2056
    /* 800EF56C 3C01BF80 */  lui $at, 0xBF80
    /* 800EF570 44816000 */  .word 0x44816000
    /* 800EF574 0C02D96D */  jal 0x800B65B4
    /* 800EF578 00000000 */  nop
    /* 800EF57C 8FBF003C */  lw $ra, 60($sp)
    /* 800EF580 8FB00018 */  lw $s0, 24($sp)
    /* 800EF584 8FB1001C */  lw $s1, 28($sp)
    /* 800EF588 8FB20020 */  lw $s2, 32($sp)
    /* 800EF58C 8FB30024 */  lw $s3, 36($sp)
    /* 800EF590 8FB40028 */  lw $s4, 40($sp)
    /* 800EF594 8FB5002C */  lw $s5, 44($sp)
    /* 800EF598 8FB60030 */  lw $s6, 48($sp)
    /* 800EF59C 8FB70034 */  lw $s7, 52($sp)
    /* 800EF5A0 8FBE0038 */  lw $fp, 56($sp)
    /* 800EF5A4 27BD00B8 */  addiu $sp, $sp, 184
    /* 800EF5A8 03E00008 */  jr $ra
    /* 800EF5AC 24020001 */  addiu $v0, $zero, 1
