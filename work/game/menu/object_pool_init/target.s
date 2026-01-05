# Source: game_code.bin (decompressed)
# Address: 0x800E7000

glabel object_pool_init
    /* 800E7000 D7B40018 */  .word 0xD7B40018
    /* 800E7004 8FB00020 */  lw $s0, 32($sp)
    /* 800E7008 8FB10024 */  lw $s1, 36($sp)
    /* 800E700C 8FB20028 */  lw $s2, 40($sp)
    /* 800E7010 8FB3002C */  lw $s3, 44($sp)
    /* 800E7014 8FB40030 */  lw $s4, 48($sp)
    /* 800E7018 8FB50034 */  lw $s5, 52($sp)
    /* 800E701C 8FB60038 */  lw $s6, 56($sp)
    /* 800E7020 8FB7003C */  lw $s7, 60($sp)
    /* 800E7024 8FBE0040 */  lw $fp, 64($sp)
    /* 800E7028 03E00008 */  jr $ra
    /* 800E702C 27BD00A0 */  addiu $sp, $sp, 160
