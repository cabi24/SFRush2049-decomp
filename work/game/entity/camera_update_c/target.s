# Source: game_code.bin (decompressed)
# Address: 0x8009C5BC

glabel camera_update_c
    /* 8009C5BC 27BDFFE8 */  addiu $sp, $sp, -24
    /* 8009C5C0 AFBF0014 */  sw $ra, 20($sp)
    /* 8009C5C4 46006406 */  .word 0x46006406
    /* 8009C5C8 0C0270FE */  jal 0x8009C3F8
    /* 8009C5CC 00002025 */  or $a0, $zero, $zero
    /* 8009C5D0 8FBF0014 */  lw $ra, 20($sp)
    /* 8009C5D4 27BD0018 */  addiu $sp, $sp, 24
    /* 8009C5D8 03E00008 */  jr $ra
    /* 8009C5DC 00000000 */  nop
