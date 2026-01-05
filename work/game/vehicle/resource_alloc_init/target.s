# Source: game_code.bin (decompressed)
# Address: 0x8010FC80

glabel resource_alloc_init
    /* 8010FC80 27BDFFE0 */  addiu $sp, $sp, -32
    /* 8010FC84 AFA50024 */  sw $a1, 36($sp)
    /* 8010FC88 00802825 */  or $a1, $a0, $zero
    /* 8010FC8C AFBF0014 */  sw $ra, 20($sp)
    /* 8010FC90 AFA40020 */  sw $a0, 32($sp)
    /* 8010FC94 0C025D1C */  jal 0x80097470
    /* 8010FC98 00002025 */  or $a0, $zero, $zero
    /* 8010FC9C AFA2001C */  sw $v0, 28($sp)
    /* 8010FCA0 00402025 */  or $a0, $v0, $zero
    /* 8010FCA4 0C0258B5 */  jal 0x800962D4
    /* 8010FCA8 00002825 */  or $a1, $zero, $zero
    /* 8010FCAC 8FBF0014 */  lw $ra, 20($sp)
    /* 8010FCB0 8FA2001C */  lw $v0, 28($sp)
    /* 8010FCB4 27BD0020 */  addiu $sp, $sp, 32
    /* 8010FCB8 03E00008 */  jr $ra
    /* 8010FCBC 00000000 */  nop
