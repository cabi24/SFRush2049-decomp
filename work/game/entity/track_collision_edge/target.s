# Source: game_code.bin (decompressed)
# Address: 0x8009DD18

glabel track_collision_edge
    /* 8009DD18 27BDFFE8 */  addiu $sp, $sp, -24
    /* 8009DD1C AFBF0014 */  sw $ra, 20($sp)
    /* 8009DD20 8C8E0000 */  lw $t6, 0($a0)
    /* 8009DD24 240F0100 */  addiu $t7, $zero, 256
    /* 8009DD28 00AFC004 */  .word 0x00AFC004
    /* 8009DD2C 01D8C824 */  and $t9, $t6, $t8
    /* 8009DD30 57200004 */  .word 0x57200004
    /* 8009DD34 84820016 */  lh $v0, 22($a0)
    /* 8009DD38 1000000F */  beq $zero, $zero, 0x8009DD78
    /* 8009DD3C 24020001 */  addiu $v0, $zero, 1
    /* 8009DD40 84820016 */  lh $v0, 22($a0)
    /* 8009DD44 3C098013 */  lui $t1, 0x8013
    /* 8009DD48 2529E700 */  addiu $t1, $t1, -6400
    /* 8009DD4C 04400009 */  bltz $v0, 0x8009DD74
    /* 8009DD50 00024100 */  sll $t0, $v0, 4
    /* 8009DD54 01024021 */  addu $t0, $t0, $v0
    /* 8009DD58 00084080 */  sll $t0, $t0, 2
    /* 8009DD5C 0C027714 */  jal 0x8009DC50
    /* 8009DD60 01092021 */  addu $a0, $t0, $t1
    /* 8009DD64 50400004 */  .word 0x50400004
    /* 8009DD68 00001025 */  or $v0, $zero, $zero
    /* 8009DD6C 10000002 */  beq $zero, $zero, 0x8009DD78
    /* 8009DD70 24020001 */  addiu $v0, $zero, 1
    /* 8009DD74 00001025 */  or $v0, $zero, $zero
    /* 8009DD78 8FBF0014 */  lw $ra, 20($sp)
    /* 8009DD7C 27BD0018 */  addiu $sp, $sp, 24
    /* 8009DD80 03E00008 */  jr $ra
    /* 8009DD84 00000000 */  nop
