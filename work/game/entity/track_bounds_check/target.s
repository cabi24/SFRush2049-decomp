# Source: game_code.bin (decompressed)
# Address: 0x8009EA70

glabel track_bounds_check
    /* 8009EA70 27BDFFE0 */  addiu $sp, $sp, -32
    /* 8009EA74 AFBF0014 */  sw $ra, 20($sp)
    /* 8009EA78 4604603C */  .word 0x4604603C
    /* 8009EA7C AFA50024 */  sw $a1, 36($sp)
    /* 8009EA80 3C018012 */  lui $at, 0x8012
    /* 8009EA84 45010006 */  .word 0x45010006
    /* 8009EA88 00000000 */  nop
    /* 8009EA8C C4263B08 */  lwc1 $f6, 15112($at)
    /* 8009EA90 460C303C */  .word 0x460C303C
    /* 8009EA94 00000000 */  nop
    /* 8009EA98 4502001A */  .word 0x4502001A
    /* 8009EA9C 8FBF0014 */  lw $ra, 20($sp)
    /* 8009EAA0 0C0021CC */  jal 0x80008730
    /* 8009EAA4 E7AC0020 */  swc1 $f12, 32($sp)
    /* 8009EAA8 C7AC0020 */  lwc1 $f12, 32($sp)
    /* 8009EAAC 0C00223C */  jal 0x800088F0
    /* 8009EAB0 E7A00018 */  swc1 $f0, 24($sp)
    /* 8009EAB4 C7B00018 */  lwc1 $f16, 24($sp)
    /* 8009EAB8 00001825 */  or $v1, $zero, $zero
    /* 8009EABC 8FA20024 */  lw $v0, 36($sp)
    /* 8009EAC0 24040003 */  addiu $a0, $zero, 3
    /* 8009EAC4 C4420000 */  lwc1 $f2, 0($v0)
    /* 8009EAC8 C44C000C */  lwc1 $f12, 12($v0)
    /* 8009EACC 24630001 */  addiu $v1, $v1, 1
    /* 8009EAD0 46001202 */  .word 0x46001202
    /* 8009EAD4 24420004 */  addiu $v0, $v0, 4
    /* 8009EAD8 46106282 */  .word 0x46106282
    /* 8009EADC 00000000 */  nop
    /* 8009EAE0 46101482 */  .word 0x46101482
    /* 8009EAE4 00000000 */  nop
    /* 8009EAE8 46006102 */  .word 0x46006102
    /* 8009EAEC 460A4381 */  .word 0x460A4381
    /* 8009EAF0 E44EFFFC */  swc1 $f14, -4($v0)
    /* 8009EAF4 46049180 */  .word 0x46049180
    /* 8009EAF8 1464FFF2 */  bne $v1, $a0, 0x8009EAC4
    /* 8009EAFC E4460008 */  swc1 $f6, 8($v0)
    /* 8009EB00 8FBF0014 */  lw $ra, 20($sp)
    /* 8009EB04 27BD0020 */  addiu $sp, $sp, 32
    /* 8009EB08 03E00008 */  jr $ra
    /* 8009EB0C 00000000 */  nop
