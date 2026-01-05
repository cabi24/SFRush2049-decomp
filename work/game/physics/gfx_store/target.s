# Source: game_code.bin (decompressed)
# Address: 0x800A2564

glabel gfx_store
    /* 800A2564 00187080 */  sll $t6, $t8, 2
    /* 800A2568 01D87023 */  subu $t6, $t6, $t8
    /* 800A256C 000E7180 */  sll $t6, $t6, 6
    /* 800A2570 01D87021 */  addu $t6, $t6, $t8
    /* 800A2574 0019C080 */  sll $t8, $t9, 2
    /* 800A2578 000E7080 */  sll $t6, $t6, 2
    /* 800A257C 0319C021 */  addu $t8, $t8, $t9
    /* 800A2580 0018C0C0 */  sll $t8, $t8, 3
    /* 800A2584 01AE7821 */  addu $t7, $t5, $t6
    /* 800A2588 01F87021 */  addu $t6, $t7, $t8
    /* 800A258C A1C20085 */  sb $v0, 133($t6)
    /* 800A2590 91990010 */  lbu $t9, 16($t4)
    /* 800A2594 00197880 */  sll $t7, $t9, 2
    /* 800A2598 01F97823 */  subu $t7, $t7, $t9
    /* 800A259C 000F7980 */  sll $t7, $t7, 6
    /* 800A25A0 01F97821 */  addu $t7, $t7, $t9
    /* 800A25A4 000F7880 */  sll $t7, $t7, 2
    /* 800A25A8 01AFC021 */  addu $t8, $t5, $t7
    /* 800A25AC A302000B */  sb $v0, 11($t8)
    /* 800A25B0 8FBF0014 */  lw $ra, 20($sp)
    /* 800A25B4 27BD0018 */  addiu $sp, $sp, 24
    /* 800A25B8 03E00008 */  jr $ra
    /* 800A25BC 00000000 */  nop
