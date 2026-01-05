# Source: game_code.bin (decompressed)
# Address: 0x800BE9E8

glabel music_tempo_adjust
    /* 800BE9E8 27BDFEE0 */  addiu $sp, $sp, -288
    /* 800BE9EC AFA50124 */  sw $a1, 292($sp)
    /* 800BE9F0 AFBF0014 */  sw $ra, 20($sp)
    /* 800BE9F4 AFA40120 */  sw $a0, 288($sp)
    /* 800BE9F8 AFA60128 */  sw $a2, 296($sp)
    /* 800BE9FC 00C02825 */  or $a1, $a2, $zero
    /* 800BEA00 AFA7012C */  sw $a3, 300($sp)
    /* 800BEA04 27A6012C */  addiu $a2, $sp, 300
    /* 800BEA08 0C000B34 */  jal 0x80002CD0
    /* 800BEA0C 27A40020 */  addiu $a0, $sp, 32
    /* 800BEA10 87A40122 */  lh $a0, 290($sp)
    /* 800BEA14 87A50126 */  lh $a1, 294($sp)
    /* 800BEA18 0C02DC75 */  jal 0x800B71D4
    /* 800BEA1C 27A60020 */  addiu $a2, $sp, 32
    /* 800BEA20 8FBF0014 */  lw $ra, 20($sp)
    /* 800BEA24 27BD0120 */  addiu $sp, $sp, 288
    /* 800BEA28 03E00008 */  jr $ra
    /* 800BEA2C 00000000 */  nop
