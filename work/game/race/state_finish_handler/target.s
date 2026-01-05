# Source: game_code.bin (decompressed)
# Address: 0x800FC960

glabel state_finish_handler
    /* 800FC960 05E10006 */  bgez $t7, 0x800FC97C
    /* 800FC964 00027200 */  sll $t6, $v0, 8
    /* 800FC968 0C03D935 */  jal 0x800F64D4
    /* 800FC96C 00000000 */  nop
    /* 800FC970 3C028011 */  lui $v0, 0x8011
    /* 800FC974 10000007 */  beq $zero, $zero, 0x800FC994
    /* 800FC978 8C4274B4 */  lw $v0, 29876($v0)
    /* 800FC97C 05C30006 */  .word 0x05C30006
    /* 800FC980 3C010060 */  lui $at, 0x0060
    /* 800FC984 0C03E2DC */  jal 0x800F8B70
    /* 800FC988 00000000 */  nop
    /* 800FC98C 3C028011 */  lui $v0, 0x8011
    /* 800FC990 8C4274B4 */  lw $v0, 29876($v0)
    /* 800FC994 3C010060 */  lui $at, 0x0060
    /* 800FC998 0041C824 */  and $t9, $v0, $at
    /* 800FC99C 53200013 */  .word 0x53200013
    /* 800FC9A0 8FBF0014 */  lw $ra, 20($sp)
    /* 800FC9A4 3C128015 */  lui $s2, 0x8015
    /* 800FC9A8 26521AD0 */  addiu $s2, $s2, 6864
    /* 800FC9AC 86580000 */  lh $t8, 0($s2)
    /* 800FC9B0 00008025 */  or $s0, $zero, $zero
    /* 800FC9B4 5B00000D */  .word 0x5B00000D
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
