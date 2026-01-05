# Source: game_code.bin (decompressed)
# Address: 0x800CB9A0

glabel menu_render_list
    /* 800CB9A0 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800CB9A4 AFBF0014 */  sw $ra, 20($sp)
    /* 800CB9A8 8C8E0000 */  lw $t6, 0($a0)
    /* 800CB9AC 8DC50004 */  lw $a1, 4($t6)
    /* 800CB9B0 50A00004 */  .word 0x50A00004
    /* 800CB9B4 8FBF0014 */  lw $ra, 20($sp)
    /* 800CB9B8 0C0289A0 */  jal 0x800A2680
    /* 800CB9BC 00A02025 */  or $a0, $a1, $zero
    /* 800CB9C0 8FBF0014 */  lw $ra, 20($sp)
    /* 800CB9C4 27BD0018 */  addiu $sp, $sp, 24
    /* 800CB9C8 03E00008 */  jr $ra
    /* 800CB9CC 00000000 */  nop
