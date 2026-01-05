# Source: game_code.bin (decompressed)
# Address: 0x800A19E4

glabel drone_path_select
    /* 800A19E4 90A9FFFF */  lbu $t1, -1($a1)
    /* 800A19E8 2463FFFF */  addiu $v1, $v1, -1
    /* 800A19EC 24A5FFFF */  addiu $a1, $a1, -1
    /* 800A19F0 03E00008 */  jr $ra
    /* 800A19F4 01091023 */  subu $v0, $t0, $t1
