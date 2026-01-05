# Source: game_code.bin (decompressed)
# Address: 0x800C9530

glabel input_callback_process
    /* 800C9530 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800C9534 AFBF001C */  sw $ra, 28($sp)
    /* 800C9538 AFB10018 */  sw $s1, 24($sp)
    /* 800C953C 15C00003 */  bne $t6, $zero, 0x800C954C
    /* 800C9540 AFB00014 */  sw $s0, 20($sp)
    /* 800C9544 0C02DCF9 */  jal 0x800B73E4
    /* 800C9548 00000000 */  nop
    /* 800C954C 3C108015 */  lui $s0, 0x8015
    /* 800C9550 3C118015 */  lui $s1, 0x8015
    /* 800C9554 26315210 */  addiu $s1, $s1, 21008
    /* 800C9558 261051E8 */  addiu $s0, $s0, 20968
    /* 800C955C 8E020000 */  lw $v0, 0($s0)
    /* 800C9560 50400004 */  .word 0x50400004
    /* 800C9564 26100004 */  addiu $s0, $s0, 4
    /* 800C9568 0040F809 */  jalr $ra, $v0
    /* 800C956C 00000000 */  nop
    /* 800C9570 26100004 */  addiu $s0, $s0, 4
    /* 800C9574 5611FFFA */  .word 0x5611FFFA
    /* 800C9578 8E020000 */  lw $v0, 0($s0)
    /* 800C957C 8FBF001C */  lw $ra, 28($sp)
    /* 800C9580 8FB00014 */  lw $s0, 20($sp)
    /* 800C9584 8FB10018 */  lw $s1, 24($sp)
    /* 800C9588 03E00008 */  jr $ra
    /* 800C958C 27BD0020 */  addiu $sp, $sp, 32
