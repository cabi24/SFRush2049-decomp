# Source: game_code.bin (decompressed)
# Address: 0x8009EB18

glabel gfx_setup_fc
    /* 8009EB18 27BDFFE0 */  addiu $sp, $sp, -32
    /* 8009EB1C AFBF0014 */  sw $ra, 20($sp)
    /* 8009EB20 4604603C */  .word 0x4604603C
    /* 8009EB24 AFA50024 */  sw $a1, 36($sp)
    /* 8009EB28 3C018012 */  lui $at, 0x8012
    /* 8009EB2C 45010006 */  .word 0x45010006
    /* 8009EB30 00000000 */  nop
    /* 8009EB34 C4263B10 */  lwc1 $f6, 15120($at)
    /* 8009EB38 460C303C */  .word 0x460C303C
    /* 8009EB3C 00000000 */  nop
    /* 8009EB40 4502001A */  .word 0x4502001A
    /* 8009EB44 8FBF0014 */  lw $ra, 20($sp)
    /* 8009EB48 0C0021CC */  jal 0x80008730
    /* 8009EB4C E7AC0020 */  swc1 $f12, 32($sp)
    /* 8009EB50 C7AC0020 */  lwc1 $f12, 32($sp)
    /* 8009EB54 0C00223C */  jal 0x800088F0
    /* 8009EB58 E7A00018 */  swc1 $f0, 24($sp)
    /* 8009EB5C C7B00018 */  lwc1 $f16, 24($sp)
    /* 8009EB60 00001825 */  or $v1, $zero, $zero
    /* 8009EB64 8FA20024 */  lw $v0, 36($sp)
    /* 8009EB68 24040003 */  addiu $a0, $zero, 3
    /* 8009EB6C C4420008 */  lwc1 $f2, 8($v0)
    /* 8009EB70 C44C0000 */  lwc1 $f12, 0($v0)
    /* 8009EB74 24630001 */  addiu $v1, $v1, 1
    /* 8009EB78 46101202 */  .word 0x46101202
    /* 8009EB7C 2442000C */  addiu $v0, $v0, 12
    /* 8009EB80 46006282 */  .word 0x46006282
    /* 8009EB84 00000000 */  nop
    /* 8009EB88 46001482 */  .word 0x46001482
    /* 8009EB8C 00000000 */  nop
    /* 8009EB90 46106102 */  .word 0x46106102
    /* 8009EB94 460A4380 */  .word 0x460A4380
    /* 8009EB98 E44EFFF4 */  swc1 $f14, -12($v0)
    /* 8009EB9C 46049181 */  .word 0x46049181
    /* 8009EBA0 1464FFF2 */  bne $v1, $a0, 0x8009EB6C
    /* 8009EBA4 E446FFFC */  swc1 $f6, -4($v0)
    /* 8009EBA8 8FBF0014 */  lw $ra, 20($sp)
    /* 8009EBAC 27BD0020 */  addiu $sp, $sp, 32
    /* 8009EBB0 03E00008 */  jr $ra
    /* 8009EBB4 00000000 */  nop
