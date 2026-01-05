# Source: game_code.bin (decompressed)
# Address: 0x800A2D1C

glabel gfx_store_b
    /* 800A2D1C 27394D60 */  addiu $t9, $t9, 19808
    /* 800A2D20 91CF0010 */  lbu $t7, 16($t6)
    /* 800A2D24 000FC100 */  sll $t8, $t7, 4
    /* 800A2D28 03192021 */  addu $a0, $t8, $t9
    /* 800A2D2C 0C0247EF */  jal 0x80091FBC
    /* 800A2D30 8C860008 */  lw $a2, 8($a0)
    /* 800A2D34 8FBF0014 */  lw $ra, 20($sp)
    /* 800A2D38 27BD0018 */  addiu $sp, $sp, 24
    /* 800A2D3C 03E00008 */  jr $ra
    /* 800A2D40 00000000 */  nop
