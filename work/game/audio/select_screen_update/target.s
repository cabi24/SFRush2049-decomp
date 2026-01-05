# Source: game_code.bin (decompressed)
# Address: 0x800BFD68

glabel select_screen_update
    /* 800BFD68 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800BFD6C AFBF0014 */  sw $ra, 20($sp)
    /* 800BFD70 46006406 */  .word 0x46006406
    /* 800BFD74 0C0270FE */  jal 0x8009C3F8
    /* 800BFD78 24040001 */  addiu $a0, $zero, 1
    /* 800BFD7C 8FBF0014 */  lw $ra, 20($sp)
    /* 800BFD80 27BD0018 */  addiu $sp, $sp, 24
    /* 800BFD84 03E00008 */  jr $ra
    /* 800BFD88 00000000 */  nop
