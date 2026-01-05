# Source: game_code.bin (decompressed)
# Address: 0x800B58A0

glabel sound_pitch_set
    /* 800B58A0 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800B58A4 AFBF0014 */  sw $ra, 20($sp)
    /* 800B58A8 4604603C */  .word 0x4604603C
    /* 800B58AC AFA50024 */  sw $a1, 36($sp)
    /* 800B58B0 3C018012 */  lui $at, 0x8012
    /* 800B58B4 45010006 */  .word 0x45010006
    /* 800B58B8 00000000 */  nop
    /* 800B58BC C4263DB8 */  lwc1 $f6, 15800($at)
    /* 800B58C0 460C303C */  .word 0x460C303C
    /* 800B58C4 00000000 */  nop
    /* 800B58C8 4502001A */  .word 0x4502001A
    /* 800B58CC 8FBF0014 */  lw $ra, 20($sp)
    /* 800B58D0 0C0021CC */  jal 0x80008730
    /* 800B58D4 E7AC0020 */  swc1 $f12, 32($sp)
    /* 800B58D8 C7AC0020 */  lwc1 $f12, 32($sp)
    /* 800B58DC 0C00223C */  jal 0x800088F0
    /* 800B58E0 E7A00018 */  swc1 $f0, 24($sp)
    /* 800B58E4 C7B00018 */  lwc1 $f16, 24($sp)
    /* 800B58E8 00001825 */  or $v1, $zero, $zero
    /* 800B58EC 8FA20024 */  lw $v0, 36($sp)
    /* 800B58F0 24040003 */  addiu $a0, $zero, 3
    /* 800B58F4 C4420004 */  lwc1 $f2, 4($v0)
    /* 800B58F8 C44C0008 */  lwc1 $f12, 8($v0)
    /* 800B58FC 24630001 */  addiu $v1, $v1, 1
    /* 800B5900 46001202 */  .word 0x46001202
    /* 800B5904 2442000C */  addiu $v0, $v0, 12
    /* 800B5908 46106282 */  .word 0x46106282
    /* 800B590C 00000000 */  nop
    /* 800B5910 46101482 */  .word 0x46101482
    /* 800B5914 00000000 */  nop
    /* 800B5918 46006102 */  .word 0x46006102
    /* 800B591C 460A4381 */  .word 0x460A4381
    /* 800B5920 E44EFFF8 */  swc1 $f14, -8($v0)
    /* 800B5924 46049180 */  .word 0x46049180
    /* 800B5928 1464FFF2 */  bne $v1, $a0, 0x800B58F4
    /* 800B592C E446FFFC */  swc1 $f6, -4($v0)
    /* 800B5930 8FBF0014 */  lw $ra, 20($sp)
    /* 800B5934 27BD0020 */  addiu $sp, $sp, 32
    /* 800B5938 03E00008 */  jr $ra
    /* 800B593C 00000000 */  nop
