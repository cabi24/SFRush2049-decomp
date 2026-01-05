# Source: game_code.bin (decompressed)
# Address: 0x800E6450

glabel collision_rumble_trigger
    /* 800E6450 46000007 */  .word 0x46000007
    /* 800E6454 E4800720 */  swc1 $f0, 1824($a0)
    /* 800E6458 03E00008 */  jr $ra
    /* 800E645C 00000000 */  nop
