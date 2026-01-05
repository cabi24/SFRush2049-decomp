# Source: game_code.bin (decompressed)
# Address: 0x800A214C

glabel physics_contact_test
    /* 800A214C 8FA40070 */  lw $a0, 112($sp)
    /* 800A2150 8FA5005C */  lw $a1, 92($sp)
    /* 800A2154 8FA60058 */  lw $a2, 88($sp)
    /* 800A2158 0C028775 */  jal 0x800A1DD4
    /* 800A215C 00005825 */  or $t3, $zero, $zero
    /* 800A2160 8FA40070 */  lw $a0, 112($sp)
    /* 800A2164 27AA005C */  addiu $t2, $sp, 92
    /* 800A2168 27A50058 */  addiu $a1, $sp, 88
    /* 800A216C 0C02871B */  jal 0x800A1C6C
    /* 800A2170 27A90054 */  addiu $t1, $sp, 84
    /* 800A2174 3C108014 */  lui $s0, 0x8014
    /* 800A2178 1440FFA2 */  bne $v0, $zero, 0x800A2004
    /* 800A217C 26104008 */  addiu $s0, $s0, 16392
    /* 800A2180 8FB80030 */  lw $t8, 48($sp)
    /* 800A2184 02C02025 */  or $a0, $s6, $zero
    /* 800A2188 0C0286ED */  jal 0x800A1BB4
    /* 800A218C A3000085 */  sb $zero, 133($t8)
    /* 800A2190 24020001 */  addiu $v0, $zero, 1
    /* 800A2194 8FBF0024 */  lw $ra, 36($sp)
    /* 800A2198 27BD0070 */  addiu $sp, $sp, 112
    /* 800A219C 03E00008 */  jr $ra
    /* 800A21A0 00000000 */  nop
