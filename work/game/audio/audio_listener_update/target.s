# Source: game_code.bin (decompressed)
# Address: 0x800B7440

glabel audio_listener_update
    /* 800B7440 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800B7444 AFBF0014 */  sw $ra, 20($sp)
    /* 800B7448 15C00003 */  bne $t6, $zero, 0x800B7458
    /* 800B744C 00000000 */  nop
    /* 800B7450 0C02DCF9 */  jal 0x800B73E4
    /* 800B7454 00000000 */  nop
    /* 800B7458 3C028015 */  lui $v0, 0x8015
    /* 800B745C 3C038015 */  lui $v1, 0x8015
    /* 800B7460 24635210 */  addiu $v1, $v1, 21008
    /* 800B7464 244251E8 */  addiu $v0, $v0, 20968
    /* 800B7468 8C4F0000 */  lw $t7, 0($v0)
    /* 800B746C 55E00004 */  .word 0x55E00004
    /* 800B7470 24420004 */  addiu $v0, $v0, 4
    /* 800B7474 10000004 */  beq $zero, $zero, 0x800B7488
    /* 800B7478 AC440000 */  sw $a0, 0($v0)
    /* 800B747C 24420004 */  addiu $v0, $v0, 4
    /* 800B7480 5443FFFA */  .word 0x5443FFFA
    /* 800B7484 8C4F0000 */  lw $t7, 0($v0)
    /* 800B7488 8FBF0014 */  lw $ra, 20($sp)
    /* 800B748C 27BD0018 */  addiu $sp, $sp, 24
    /* 800B7490 03E00008 */  jr $ra
    /* 800B7494 00000000 */  nop
