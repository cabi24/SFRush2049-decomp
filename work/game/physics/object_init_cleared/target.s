# Source: game_code.bin (decompressed)
# Address: 0x800AED2C

glabel object_init_cleared
    /* 800AED2C 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800AED30 AFBF0014 */  sw $ra, 20($sp)
    /* 800AED34 14A00003 */  bne $a1, $zero, 0x800AED44
    /* 800AED38 00000000 */  nop
    /* 800AED3C 10000005 */  beq $zero, $zero, 0x800AED54
    /* 800AED40 00001025 */  or $v0, $zero, $zero
    /* 800AED44 0C024847 */  jal 0x8009211C
    /* 800AED48 AFA5001C */  sw $a1, 28($sp)
    /* 800AED4C 8FA2001C */  lw $v0, 28($sp)
    /* 800AED50 A0400008 */  sb $zero, 8($v0)
    /* 800AED54 8FBF0014 */  lw $ra, 20($sp)
    /* 800AED58 27BD0020 */  addiu $sp, $sp, 32
    /* 800AED5C 03E00008 */  jr $ra
    /* 800AED60 00000000 */  nop
