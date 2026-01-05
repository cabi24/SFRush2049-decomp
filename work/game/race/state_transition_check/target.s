# Source: game_code.bin (decompressed)
# Address: 0x800FC9B8

glabel state_transition_check
    /* 800FC9B8 8FBF0014 */  lw $ra, 20($sp)
    /* 800FC9BC 3C118015 */  lui $s1, 0x8015
    /* 800FC9C0 26310B94 */  addiu $s1, $s1, 2964
    /* 800FC9C4 02002025 */  or $a0, $s0, $zero
    /* 800FC9C8 0C02AC63 */  jal 0x800AB18C
    /* 800FC9CC 02202825 */  or $a1, $s1, $zero
    /* 800FC9D0 864F0000 */  lh $t7, 0($s2)
    /* 800FC9D4 26100001 */  addiu $s0, $s0, 1
    /* 800FC9D8 26310098 */  addiu $s1, $s1, 152
    /* 800FC9DC 020F082A */  slt $at, $s0, $t7
    /* 800FC9E0 5420FFF9 */  .word 0x5420FFF9
    /* 800FC9E4 02002025 */  or $a0, $s0, $zero
    /* 800FC9E8 8FBF0014 */  lw $ra, 20($sp)
    /* 800FC9EC 27BD0018 */  addiu $sp, $sp, 24
    /* 800FC9F0 03E00008 */  jr $ra
    /* 800FC9F4 00000000 */  nop
