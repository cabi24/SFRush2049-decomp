# Source: game_code.bin (decompressed)
# Address: 0x800E9200

glabel collision_query_ray
    /* 800E9200 0C03A354 */  jal 0x800E8D50
    /* 800E9204 E7A60084 */  swc1 $f6, 132($sp)
    /* 800E9208 8FBF003C */  lw $ra, 60($sp)
    /* 800E920C D7B40018 */  .word 0xD7B40018
    /* 800E9210 8FB00020 */  lw $s0, 32($sp)
    /* 800E9214 8FB10024 */  lw $s1, 36($sp)
    /* 800E9218 8FB20028 */  lw $s2, 40($sp)
    /* 800E921C 8FB3002C */  lw $s3, 44($sp)
    /* 800E9220 8FB40030 */  lw $s4, 48($sp)
    /* 800E9224 8FB50034 */  lw $s5, 52($sp)
    /* 800E9228 8FB60038 */  lw $s6, 56($sp)
    /* 800E922C 03E00008 */  jr $ra
    /* 800E9230 27BD0090 */  addiu $sp, $sp, 144
