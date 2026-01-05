# Source: game_code.bin (decompressed)
# Address: 0x800CC804

glabel menu_item_value_get
    /* 800CC804 27BDFFD8 */  addiu $sp, $sp, -40
    /* 800CC808 AFBF0024 */  sw $ra, 36($sp)
    /* 800CC80C AFB30020 */  sw $s3, 32($sp)
    /* 800CC810 AFB10018 */  sw $s1, 24($sp)
    /* 800CC814 AFB00014 */  sw $s0, 20($sp)
    /* 800CC818 AFB2001C */  sw $s2, 28($sp)
    /* 800CC81C 24100001 */  addiu $s0, $zero, 1
    /* 800CC820 00808825 */  or $s1, $a0, $zero
    /* 800CC824 0C032FCB */  jal 0x800CBF2C
    /* 800CC828 24130001 */  addiu $s3, $zero, 1
    /* 800CC82C 8FBF0024 */  lw $ra, 36($sp)
    /* 800CC830 8FB00014 */  lw $s0, 20($sp)
    /* 800CC834 8FB10018 */  lw $s1, 24($sp)
    /* 800CC838 8FB2001C */  lw $s2, 28($sp)
    /* 800CC83C 8FB30020 */  lw $s3, 32($sp)
    /* 800CC840 03E00008 */  jr $ra
    /* 800CC844 27BD0028 */  addiu $sp, $sp, 40
