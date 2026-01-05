# Source: game_code.bin (decompressed)
# Address: 0x800E2A00

glabel hud_minimap_render
    /* 800E2A00 10000002 */  beq $zero, $zero, 0x800E2A0C
    /* 800E2A04 00000000 */  nop
    /* 800E2A08 C60005B4 */  lwc1 $f0, 1460($s0)
    /* 800E2A0C C4324168 */  lwc1 $f18, 16744($at)
    /* 800E2A10 C6040060 */  lwc1 $f4, 96($s0)
    /* 800E2A14 46009282 */  .word 0x46009282
    /* 800E2A18 00000000 */  nop
    /* 800E2A1C 460C5202 */  .word 0x460C5202
    /* 800E2A20 46082400 */  .word 0x46082400
    /* 800E2A24 E6100060 */  swc1 $f16, 96($s0)
    /* 800E2A28 8FBF0044 */  lw $ra, 68($sp)
    /* 800E2A2C 8FB00040 */  lw $s0, 64($sp)
    /* 800E2A30 27BD0098 */  addiu $sp, $sp, 152
    /* 800E2A34 03E00008 */  jr $ra
    /* 800E2A38 00000000 */  nop
