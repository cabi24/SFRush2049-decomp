# Source: game_code.bin (decompressed)
# Address: 0x800BB000

glabel collision_detect
    /* 800BB000 00EF3821 */  addu $a3, $a3, $t7
    /* 800BB004 8C630004 */  lw $v1, 4($v1)
    /* 800BB008 5460FFFA */  .word 0x5460FFFA
    /* 800BB00C 806E0014 */  lb $t6, 20($v1)
    /* 800BB010 0C001D78 */  jal 0x800075E0
    /* 800BB014 AFA7001C */  sw $a3, 28($sp)
    /* 800BB018 8FBF0014 */  lw $ra, 20($sp)
    /* 800BB01C 8FA2001C */  lw $v0, 28($sp)
    /* 800BB020 27BD0020 */  addiu $sp, $sp, 32
    /* 800BB024 03E00008 */  jr $ra
    /* 800BB028 00000000 */  nop
