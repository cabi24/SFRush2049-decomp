# Source: game_code.bin (decompressed)
# Address: 0x800A7A10

glabel Input_AllocPadSlot
    /* 800A7A10 24840BF0 */  addiu $a0, $a0, 3056
    /* 800A7A14 240A0002 */  addiu $t2, $zero, 2
    /* 800A7A18 808E0016 */  lb $t6, 22($a0)
    /* 800A7A1C 514E0006 */  .word 0x514E0006
    /* 800A7A20 286100C8 */  slti $at, $v1, 200
    /* 800A7A24 24630001 */  addiu $v1, $v1, 1
    /* 800A7A28 0062082A */  slt $at, $v1, $v0
    /* 800A7A2C 1420FFFA */  bne $at, $zero, 0x800A7A18
    /* 800A7A30 24840020 */  addiu $a0, $a0, 32
    /* 800A7A34 286100C8 */  slti $at, $v1, 200
    /* 800A7A38 14200003 */  bne $at, $zero, 0x800A7A48
    /* 800A7A3C 00037940 */  sll $t7, $v1, 5
    /* 800A7A40 03E00008 */  jr $ra
    /* 800A7A44 2402FFFF */  addiu $v0, $zero, -1
