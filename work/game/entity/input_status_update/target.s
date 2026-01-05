# Source: game_code.bin (decompressed)
# Address: 0x80094DB0

glabel input_status_update
    /* 80094DB0 3C188014 */  lui $t8, 0x8014
    /* 80094DB4 27180BF0 */  addiu $t8, $t8, 3056
    /* 80094DB8 00047940 */  sll $t7, $a0, 5
    /* 80094DBC 01F81021 */  addu $v0, $t7, $t8
    /* 80094DC0 80590016 */  lb $t9, 22($v0)
    /* 80094DC4 AFA7000C */  sw $a3, 12($sp)
    /* 80094DC8 30EEFFFF */  andi $t6, $a3, 0xFFFF
    /* 80094DCC 24010002 */  addiu $at, $zero, 2
    /* 80094DD0 13210009 */  beq $t9, $at, 0x80094DF8
    /* 80094DD4 01C03825 */  or $a3, $t6, $zero
    /* 80094DD8 0005402B */  sltu $t0, $zero, $a1
    /* 80094DDC 04C00002 */  bltz $a2, 0x80094DE8
    /* 80094DE0 A0480016 */  sb $t0, 22($v0)
    /* 80094DE4 A0460014 */  sb $a2, 20($v0)
    /* 80094DE8 3401FFFF */  ori $at, $zero, 0xFFFF
    /* 80094DEC 10E10002 */  beq $a3, $at, 0x80094DF8
    /* 80094DF0 00000000 */  nop
    /* 80094DF4 A447000E */  sh $a3, 14($v0)
    /* 80094DF8 03E00008 */  jr $ra
    /* 80094DFC 00000000 */  nop
