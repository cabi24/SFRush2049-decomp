# Source: game_code.bin (decompressed)
# Address: 0x8008AA20

glabel controller_rumble_thunk
    /* 8008AA20 27BDFFE8 */  addiu $sp, $sp, -24
    /* 8008AA24 AFBF0014 */  sw $ra, 20($sp)
    /* 8008AA28 0C008179 */  jal 0x800205E4
    /* 8008AA2C 00000000 */  nop
    /* 8008AA30 8FBF0014 */  lw $ra, 20($sp)
    /* 8008AA34 27BD0018 */  addiu $sp, $sp, 24
    /* 8008AA38 03E00008 */  jr $ra
    /* 8008AA3C 00000000 */  nop
