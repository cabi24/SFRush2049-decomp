# Source: game_code.bin (decompressed)
# Address: 0x800F73FC

glabel audio_update_b
    /* 800F73FC 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800F7400 AFBF0014 */  sw $ra, 20($sp)
    /* 800F7404 10800003 */  beq $a0, $zero, 0x800F7414
    /* 800F7408 AFA5001C */  sw $a1, 28($sp)
    /* 800F740C 0C03DCCF */  jal 0x800F733C
    /* 800F7410 00000000 */  nop
    /* 800F7414 8FAE001C */  lw $t6, 28($sp)
    /* 800F7418 11C00003 */  beq $t6, $zero, 0x800F7428
    /* 800F741C 00000000 */  nop
    /* 800F7420 0C02C21A */  jal 0x800B0868
    /* 800F7424 00000000 */  nop
    /* 800F7428 0C02E047 */  jal 0x800B811C
    /* 800F742C 00000000 */  nop
    /* 800F7430 0C028131 */  jal 0x800A04C4
    /* 800F7434 00002025 */  or $a0, $zero, $zero
    /* 800F7438 8FBF0014 */  lw $ra, 20($sp)
    /* 800F743C 27BD0018 */  addiu $sp, $sp, 24
    /* 800F7440 03E00008 */  jr $ra
    /* 800F7444 00000000 */  nop
