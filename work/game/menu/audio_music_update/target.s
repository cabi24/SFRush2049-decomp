# Source: game_code.bin (decompressed)
# Address: 0x800E6800

glabel audio_music_update
    /* 800E6800 44985000 */  .word 0x44985000
    /* 800E6804 00000000 */  nop
    /* 800E6808 46805120 */  .word 0x46805120
    /* 800E680C 46102183 */  .word 0x46102183
    /* 800E6810 E4860728 */  swc1 $f6, 1832($a0)
    /* 800E6814 03E00008 */  jr $ra
    /* 800E6818 00000000 */  nop
