# Source: game_code.bin (decompressed)
# Address: 0x800FEC78

glabel game_results_init
    /* 800FEC78 24040026 */  addiu $a0, $zero, 38
    /* 800FEC7C 00002825 */  or $a1, $zero, $zero
    /* 800FEC80 24060001 */  addiu $a2, $zero, 1
    /* 800FEC84 0C0248D8 */  jal 0x80092360
    /* 800FEC88 00003825 */  or $a3, $zero, $zero
    /* 800FEC8C 8FBF0014 */  lw $ra, 20($sp)
    /* 800FEC90 27BD0018 */  addiu $sp, $sp, 24
    /* 800FEC94 03E00008 */  jr $ra
    /* 800FEC98 00000000 */  nop
