# Source: game_code.bin (decompressed)
# Address: 0x800F4D2C

glabel track_surface_setup
    /* 800F4D2C 1495FFC9 */  bne $a0, $s5, 0x800F4C54
    /* 800F4D30 00000000 */  nop
    /* 800F4D34 3C058015 */  lui $a1, 0x8015
    /* 800F4D38 90A5978C */  lbu $a1, -26740($a1)
    /* 800F4D3C 0C03363B */  jal 0x800CD8EC
    /* 800F4D40 8E040048 */  lw $a0, 72($s0)
    /* 800F4D44 3C0C8015 */  lui $t4, 0x8015
    /* 800F4D48 858CA108 */  lh $t4, -24312($t4)
    /* 800F4D4C 26F70001 */  addiu $s7, $s7, 1
    /* 800F4D50 2610004C */  addiu $s0, $s0, 76
    /* 800F4D54 02EC082A */  slt $at, $s7, $t4
    /* 800F4D58 5420FFAC */  .word 0x5420FFAC
    /* 800F4D5C 8E020048 */  lw $v0, 72($s0)
    /* 800F4D60 8FBF0044 */  lw $ra, 68($sp)
    /* 800F4D64 D7B40018 */  .word 0xD7B40018
    /* 800F4D68 8FB00020 */  lw $s0, 32($sp)
    /* 800F4D6C 8FB10024 */  lw $s1, 36($sp)
    /* 800F4D70 8FB20028 */  lw $s2, 40($sp)
    /* 800F4D74 8FB3002C */  lw $s3, 44($sp)
    /* 800F4D78 8FB40030 */  lw $s4, 48($sp)
    /* 800F4D7C 8FB50034 */  lw $s5, 52($sp)
    /* 800F4D80 8FB60038 */  lw $s6, 56($sp)
    /* 800F4D84 8FB7003C */  lw $s7, 60($sp)
    /* 800F4D88 8FBE0040 */  lw $fp, 64($sp)
    /* 800F4D8C 03E00008 */  jr $ra
    /* 800F4D90 27BD0048 */  addiu $sp, $sp, 72
