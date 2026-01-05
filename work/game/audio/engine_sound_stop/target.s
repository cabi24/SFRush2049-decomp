# Source: game_code.bin (decompressed)
# Address: 0x800B3340

glabel engine_sound_stop
    /* 800B3340 3C188015 */  lui $t8, 0x8015
    /* 800B3344 8F18A110 */  lw $t8, -24304($t8)
    /* 800B3348 56B80004 */  .word 0x56B80004
    /* 800B334C 8FBF0054 */  lw $ra, 84($sp)
    /* 800B3350 0C0E44CF */  jal 0x8039133C
    /* 800B3354 82E40000 */  lb $a0, 0($s7)
    /* 800B3358 8FBF0054 */  lw $ra, 84($sp)
    /* 800B335C D7B40028 */  .word 0xD7B40028
    /* 800B3360 8FB00030 */  lw $s0, 48($sp)
    /* 800B3364 8FB10034 */  lw $s1, 52($sp)
    /* 800B3368 8FB20038 */  lw $s2, 56($sp)
    /* 800B336C 8FB3003C */  lw $s3, 60($sp)
    /* 800B3370 8FB40040 */  lw $s4, 64($sp)
    /* 800B3374 8FB50044 */  lw $s5, 68($sp)
    /* 800B3378 8FB60048 */  lw $s6, 72($sp)
    /* 800B337C 8FB7004C */  lw $s7, 76($sp)
    /* 800B3380 8FBE0050 */  lw $fp, 80($sp)
    /* 800B3384 03E00008 */  jr $ra
    /* 800B3388 27BD00A0 */  addiu $sp, $sp, 160
