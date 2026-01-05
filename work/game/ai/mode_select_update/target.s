# Source: game_code.bin (decompressed)
# Address: 0x800DFB08

glabel mode_select_update
    /* 800DFB08 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800DFB0C AFBF001C */  sw $ra, 28($sp)
    /* 800DFB10 C6040004 */  lwc1 $f4, 4($s0)
    /* 800DFB14 46046032 */  .word 0x46046032
    /* 800DFB18 00000000 */  nop
    /* 800DFB1C 45030006 */  .word 0x45030006
    /* 800DFB20 C6060008 */  lwc1 $f6, 8($s0)
    /* 800DFB24 44056000 */  .word 0x44056000
    /* 800DFB28 E60C0004 */  swc1 $f12, 4($s0)
    /* 800DFB2C 0C024797 */  jal 0x80091E5C
    /* 800DFB30 8E040000 */  lw $a0, 0($s0)
    /* 800DFB34 C6060008 */  lwc1 $f6, 8($s0)
    /* 800DFB38 00002825 */  or $a1, $zero, $zero
    /* 800DFB3C 24060001 */  addiu $a2, $zero, 1
    /* 800DFB40 4606E032 */  .word 0x4606E032
    /* 800DFB44 00000000 */  nop
    /* 800DFB48 45030012 */  .word 0x45030012
    /* 800DFB4C 8FBF001C */  lw $ra, 28($sp)
    /* 800DFB50 3C128014 */  lui $s2, 0x8014
    /* 800DFB54 26522728 */  addiu $s2, $s2, 10024
    /* 800DFB58 E61C0008 */  swc1 $f28, 8($s0)
    /* 800DFB5C 02402025 */  or $a0, $s2, $zero
    /* 800DFB60 0C001C9C */  jal 0x80007270
    /* 800DFB64 8E110000 */  lw $s1, 0($s0)
    /* 800DFB68 3C01C000 */  lui $at, 0xC000
    /* 800DFB6C 4481B000 */  .word 0x4481B000
    /* 800DFB70 02202025 */  or $a0, $s1, $zero
    /* 800DFB74 4600B606 */  .word 0x4600B606
    /* 800DFB78 0C024729 */  jal 0x80091CA4
    /* 800DFB7C 4600B686 */  .word 0x4600B686
    /* 800DFB80 02402025 */  or $a0, $s2, $zero
    /* 800DFB84 00002825 */  or $a1, $zero, $zero
    /* 800DFB88 0C001D78 */  jal 0x800075E0
    /* 800DFB8C 00003025 */  or $a2, $zero, $zero
    /* 800DFB90 8FBF001C */  lw $ra, 28($sp)
    /* 800DFB94 27BD0020 */  addiu $sp, $sp, 32
    /* 800DFB98 03E00008 */  jr $ra
    /* 800DFB9C 00000000 */  nop
