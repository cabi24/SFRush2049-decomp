# Source: game_code.bin (decompressed)
# Address: 0x8010FC34

glabel clear_region
    /* 8010FC34 0C001D78 */  jal 0x800075E0
    /* 8010FC38 24060001 */  addiu $a2, $zero, 1
    /* 8010FC3C 3C048003 */  lui $a0, 0x8003
    /* 8010FC40 2484E928 */  addiu $a0, $a0, -5848
    /* 8010FC44 2405029E */  addiu $a1, $zero, 670
    /* 8010FC48 0C001D78 */  jal 0x800075E0
    /* 8010FC4C 24060001 */  addiu $a2, $zero, 1
    /* 8010FC50 8FBF0014 */  lw $ra, 20($sp)
    /* 8010FC54 27BD0018 */  addiu $sp, $sp, 24
    /* 8010FC58 03E00008 */  jr $ra
    /* 8010FC5C 00000000 */  nop
