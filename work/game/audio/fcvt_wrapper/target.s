# Source: game_code.bin (decompressed)
# Address: 0x800B4360

glabel fcvt_wrapper
    /* 800B4360 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800B4364 AFBF0014 */  sw $ra, 20($sp)
    /* 800B4368 AFA60028 */  sw $a2, 40($sp)
    /* 800B436C AFA50024 */  sw $a1, 36($sp)
    /* 800B4370 AFA7002C */  sw $a3, 44($sp)
    /* 800B4374 0C000B34 */  jal 0x80002CD0
    /* 800B4378 27A60028 */  addiu $a2, $sp, 40
    /* 800B437C 8FBF0014 */  lw $ra, 20($sp)
    /* 800B4380 27BD0020 */  addiu $sp, $sp, 32
    /* 800B4384 03E00008 */  jr $ra
    /* 800B4388 00000000 */  nop
