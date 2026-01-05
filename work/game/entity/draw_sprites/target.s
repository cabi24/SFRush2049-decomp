# Source: game_code.bin (decompressed)
# Address: 0x800953A0

glabel draw_sprites
    /* 800953A0 8CB80008 */  lw $t8, 8($a1)
    /* 800953A4 8DCF001C */  lw $t7, 28($t6)
    /* 800953A8 8F19001C */  lw $t9, 28($t8)
    /* 800953AC 03E00008 */  jr $ra
    /* 800953B0 01F91023 */  subu $v0, $t7, $t9
