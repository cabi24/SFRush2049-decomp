# Source: game_code.bin (decompressed)
# Address: 0x800E7900

glabel input_deadzone_apply
    /* 800E7900 8FB30020 */  lw $s3, 32($sp)
    /* 800E7904 8FB40024 */  lw $s4, 36($sp)
    /* 800E7908 8FB50028 */  lw $s5, 40($sp)
    /* 800E790C 03E00008 */  jr $ra
    /* 800E7910 27BD0040 */  addiu $sp, $sp, 64
