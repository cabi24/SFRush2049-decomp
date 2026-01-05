# Source: game_code.bin (decompressed)
# Address: 0x800B0618

glabel audio_channel_free
    /* 800B0618 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800B061C AFB10018 */  sw $s1, 24($sp)
    /* 800B0620 3C118014 */  lui $s1, 0x8014
    /* 800B0624 263191F0 */  addiu $s1, $s1, -28176
    /* 800B0628 AFB00014 */  sw $s0, 20($sp)
    /* 800B062C 8E300000 */  lw $s0, 0($s1)
    /* 800B0630 AFBF001C */  sw $ra, 28($sp)
    /* 800B0634 12000006 */  beq $s0, $zero, 0x800B0650
    /* 800B0638 02002025 */  or $a0, $s0, $zero
    /* 800B063C 0C0241E7 */  jal 0x8009079C
    /* 800B0640 24050001 */  addiu $a1, $zero, 1
    /* 800B0644 8E300000 */  lw $s0, 0($s1)
    /* 800B0648 5600FFFC */  .word 0x5600FFFC
    /* 800B064C 02002025 */  or $a0, $s0, $zero
    /* 800B0650 0C02C160 */  jal 0x800B0580
    /* 800B0654 00000000 */  nop
    /* 800B0658 8FBF001C */  lw $ra, 28($sp)
    /* 800B065C 8FB00014 */  lw $s0, 20($sp)
    /* 800B0660 8FB10018 */  lw $s1, 24($sp)
    /* 800B0664 03E00008 */  jr $ra
    /* 800B0668 27BD0020 */  addiu $sp, $sp, 32
