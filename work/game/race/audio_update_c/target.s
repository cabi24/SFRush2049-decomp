# Source: game_code.bin (decompressed)
# Address: 0x800F7E70

glabel audio_update_c
    /* 800F7E70 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800F7E74 AFBF0014 */  sw $ra, 20($sp)
    /* 800F7E78 3C048015 */  lui $a0, 0x8015
    /* 800F7E7C 24849428 */  addiu $a0, $a0, -27608
    /* 800F7E80 00002825 */  or $a1, $zero, $zero
    /* 800F7E84 0C0009E4 */  jal 0x80002790
    /* 800F7E88 24060010 */  addiu $a2, $zero, 16
    /* 800F7E8C 3C048015 */  lui $a0, 0x8015
    /* 800F7E90 2484256C */  addiu $a0, $a0, 9580
    /* 800F7E94 00002825 */  or $a1, $zero, $zero
    /* 800F7E98 0C0009E4 */  jal 0x80002790
    /* 800F7E9C 24060004 */  addiu $a2, $zero, 4
    /* 800F7EA0 8FBF0014 */  lw $ra, 20($sp)
    /* 800F7EA4 27BD0018 */  addiu $sp, $sp, 24
    /* 800F7EA8 03E00008 */  jr $ra
    /* 800F7EAC 00000000 */  nop
