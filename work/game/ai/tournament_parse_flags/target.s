# Source: game_code.bin (decompressed)
# Address: 0x800DC220

glabel tournament_parse_flags
    /* 800DC220 0007C042 */  srl $t8, $a3, 1
    /* 800DC224 244E0001 */  addiu $t6, $v0, 1
    /* 800DC228 01AC7825 */  or $t7, $t5, $t4
    /* 800DC22C A50E0000 */  sh $t6, 0($t0)
    /* 800DC230 03003825 */  or $a3, $t8, $zero
    /* 800DC234 1420FFF1 */  bne $at, $zero, 0x800DC1FC
    /* 800DC238 A0CF0000 */  sb $t7, 0($a2)
    /* 800DC23C 24020001 */  addiu $v0, $zero, 1
    /* 800DC240 03E00008 */  jr $ra
    /* 800DC244 00000000 */  nop
