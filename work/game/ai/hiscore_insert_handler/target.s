# Source: game_code.bin (decompressed)
# Address: 0x800DC100

glabel hiscore_insert_handler
    /* 800DC100 004F1021 */  addu $v0, $v0, $t7
    /* 800DC104 9442E5E8 */  lhu $v0, -6680($v0)
    /* 800DC108 2442FFFF */  addiu $v0, $v0, -1
    /* 800DC10C 0002C400 */  sll $t8, $v0, 16
    /* 800DC110 03E00008 */  jr $ra
    /* 800DC114 00181403 */  sra $v0, $t8, 16
