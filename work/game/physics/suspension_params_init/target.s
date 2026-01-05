# Source: game_code.bin (decompressed)
# Address: 0x800A511C

glabel suspension_params_init
    /* 800A511C 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800A5120 AFBF0014 */  sw $ra, 20($sp)
    /* 800A5124 50850009 */  .word 0x50850009
    /* 800A5128 8FBF0014 */  lw $ra, 20($sp)
    /* 800A512C 0C029407 */  jal 0x800A501C
    /* 800A5130 00000000 */  nop
    /* 800A5134 3C0E8014 */  lui $t6, 0x8014
    /* 800A5138 85CEFEC8 */  lh $t6, -312($t6)
    /* 800A513C 3C018014 */  lui $at, 0x8014
    /* 800A5140 0C029396 */  jal 0x800A4E58
    /* 800A5144 A02E0A10 */  sb $t6, 2576($at)
    /* 800A5148 8FBF0014 */  lw $ra, 20($sp)
    /* 800A514C 27BD0018 */  addiu $sp, $sp, 24
    /* 800A5150 03E00008 */  jr $ra
    /* 800A5154 00000000 */  nop
