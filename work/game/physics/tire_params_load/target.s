# Source: game_code.bin (decompressed)
# Address: 0x800A5A74

glabel tire_params_load
    /* 800A5A74 27BDFFD0 */  addiu $sp, $sp, -48
    /* 800A5A78 E7A40010 */  swc1 $f4, 16($sp)
    /* 800A5A7C 46805420 */  .word 0x46805420
    /* 800A5A80 E7A80014 */  swc1 $f8, 20($sp)
    /* 800A5A84 AFBF002C */  sw $ra, 44($sp)
    /* 800A5A88 3C068015 */  lui $a2, 0x8015
    /* 800A5A8C 3C058012 */  lui $a1, 0x8012
    /* 800A5A90 3C078015 */  lui $a3, 0x8015
    /* 800A5A94 24C69870 */  addiu $a2, $a2, -26512
    /* 800A5A98 8CE74188 */  lw $a3, 16776($a3)
    /* 800A5A9C 24A5EA30 */  addiu $a1, $a1, -5584
    /* 800A5AA0 E7B00018 */  swc1 $f16, 24($sp)
    /* 800A5AA4 00002025 */  or $a0, $zero, $zero
    /* 800A5AA8 04410003 */  bgez $v0, 0x800A5AB8
    /* 800A5AAC 00027843 */  sra $t7, $v0, 1
    /* 800A5AB0 24410001 */  addiu $at, $v0, 1
    /* 800A5AB4 00017843 */  sra $t7, $at, 1
    /* 800A5AB8 448F9000 */  .word 0x448F9000
    /* 800A5ABC 00000000 */  nop
    /* 800A5AC0 46809120 */  .word 0x46809120
    /* 800A5AC4 E7A4001C */  swc1 $f4, 28($sp)
    /* 800A5AC8 04610003 */  bgez $v1, 0x800A5AD8
    /* 800A5ACC 0003C043 */  sra $t8, $v1, 1
    /* 800A5AD0 24610001 */  addiu $at, $v1, 1
    /* 800A5AD4 0001C043 */  sra $t8, $at, 1
    /* 800A5AD8 44983000 */  .word 0x44983000
    /* 800A5ADC 00000000 */  nop
    /* 800A5AE0 46803220 */  .word 0x46803220
    /* 800A5AE4 0C029642 */  jal 0x800A5908
    /* 800A5AE8 E7A80020 */  swc1 $f8, 32($sp)
    /* 800A5AEC 3C038003 */  lui $v1, 0x8003
    /* 800A5AF0 3C048003 */  lui $a0, 0x8003
    /* 800A5AF4 9484AFC6 */  lhu $a0, -20538($a0)
    /* 800A5AF8 9463AFC2 */  lhu $v1, -20542($v1)
    /* 800A5AFC 3C068015 */  lui $a2, 0x8015
    /* 800A5B00 24C69870 */  addiu $a2, $a2, -26512
    /* 800A5B04 3C058018 */  lui $a1, 0x8018
    /* 800A5B08 24A5A510 */  addiu $a1, $a1, -23280
    /* 800A5B0C 3C198012 */  lui $t9, 0x8012
    /* 800A5B10 2739EA30 */  addiu $t9, $t9, -5584
    /* 800A5B14 A4C00000 */  sh $zero, 0($a2)
    /* 800A5B18 A4C00002 */  sh $zero, 2($a2)
    /* 800A5B1C ACB90000 */  sw $t9, 0($a1)
    /* 800A5B20 ACA60004 */  sw $a2, 4($a1)
    /* 800A5B24 A4C40006 */  sh $a0, 6($a2)
    /* 800A5B28 A4C30004 */  sh $v1, 4($a2)
    /* 800A5B2C 8FBF002C */  lw $ra, 44($sp)
    /* 800A5B30 27BD0030 */  addiu $sp, $sp, 48
    /* 800A5B34 03E00008 */  jr $ra
    /* 800A5B38 00000000 */  nop
