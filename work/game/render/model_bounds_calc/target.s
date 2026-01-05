# Source: game_code.bin (decompressed)
# Address: 0x8008B26C

glabel model_bounds_calc
    /* 8008B26C 27BDFFE8 */  addiu $sp, $sp, -24
    /* 8008B270 12000007 */  beq $s0, $zero, 0x8008B290
    /* 8008B274 AFBF0014 */  sw $ra, 20($sp)
    /* 8008B278 84E40006 */  lh $a0, 6($a3)
    /* 8008B27C 00002825 */  or $a1, $zero, $zero
    /* 8008B280 0C022C36 */  jal 0x8008B0D8
    /* 8008B284 2406000F */  addiu $a2, $zero, 15
    /* 8008B288 10000006 */  beq $zero, $zero, 0x8008B2A4
    /* 8008B28C 8FBF0014 */  lw $ra, 20($sp)
    /* 8008B290 84E40006 */  lh $a0, 6($a3)
    /* 8008B294 24050001 */  addiu $a1, $zero, 1
    /* 8008B298 0C022BA3 */  jal 0x8008AE8C
    /* 8008B29C 2406000F */  addiu $a2, $zero, 15
    /* 8008B2A0 8FBF0014 */  lw $ra, 20($sp)
    /* 8008B2A4 27BD0018 */  addiu $sp, $sp, 24
    /* 8008B2A8 02001025 */  or $v0, $s0, $zero
    /* 8008B2AC 03E00008 */  jr $ra
    /* 8008B2B0 00000000 */  nop
