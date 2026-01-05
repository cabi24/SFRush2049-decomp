# Source: game_code.bin (decompressed)
# Address: 0x800D54BC

glabel player_conditional_call
    /* 800D54BC 3C01C000 */  lui $at, 0xC000
    /* 800D54C0 44810000 */  .word 0x44810000
    /* 800D54C4 240EFFFF */  addiu $t6, $zero, -1
    /* 800D54C8 AC8E0000 */  sw $t6, 0($a0)
    /* 800D54CC E4800004 */  swc1 $f0, 4($a0)
    /* 800D54D0 E4800008 */  swc1 $f0, 8($a0)
    /* 800D54D4 E480000C */  swc1 $f0, 12($a0)
    /* 800D54D8 03E00008 */  jr $ra
    /* 800D54DC E4800010 */  swc1 $f0, 16($a0)
