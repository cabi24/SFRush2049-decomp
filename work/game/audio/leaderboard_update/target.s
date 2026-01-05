# Source: game_code.bin (decompressed)
# Address: 0x800BF148

glabel leaderboard_update
    /* 800BF148 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800BF14C AFA40018 */  sw $a0, 24($sp)
    /* 800BF150 AFBF0014 */  sw $ra, 20($sp)
    /* 800BF154 3C048014 */  lui $a0, 0x8014
    /* 800BF158 24842728 */  addiu $a0, $a0, 10024
    /* 800BF15C 00002825 */  or $a1, $zero, $zero
    /* 800BF160 0C001C9C */  jal 0x80007270
    /* 800BF164 24060001 */  addiu $a2, $zero, 1
    /* 800BF168 3C078014 */  lui $a3, 0x8014
    /* 800BF16C 24E72728 */  addiu $a3, $a3, 10024
    /* 800BF170 0C0246EA */  jal 0x80091BA8
    /* 800BF174 8FA40018 */  lw $a0, 24($sp)
    /* 800BF178 14400007 */  bne $v0, $zero, 0x800BF198
    /* 800BF17C 00401825 */  or $v1, $v0, $zero
    /* 800BF180 00E02025 */  or $a0, $a3, $zero
    /* 800BF184 00002825 */  or $a1, $zero, $zero
    /* 800BF188 0C001D78 */  jal 0x800075E0
    /* 800BF18C 00003025 */  or $a2, $zero, $zero
    /* 800BF190 10000009 */  beq $zero, $zero, 0x800BF1B8
    /* 800BF194 00001025 */  or $v0, $zero, $zero
    /* 800BF198 0C02FC07 */  jal 0x800BF01C
    /* 800BF19C 8C640040 */  lw $a0, 64($v1)
    /* 800BF1A0 00E02025 */  or $a0, $a3, $zero
    /* 800BF1A4 00002825 */  or $a1, $zero, $zero
    /* 800BF1A8 0C001D78 */  jal 0x800075E0
    /* 800BF1AC 00003025 */  or $a2, $zero, $zero
    /* 800BF1B0 0C02FC29 */  jal 0x800BF0A4
    /* 800BF1B4 8FA40018 */  lw $a0, 24($sp)
    /* 800BF1B8 8FBF0014 */  lw $ra, 20($sp)
    /* 800BF1BC 27BD0018 */  addiu $sp, $sp, 24
    /* 800BF1C0 03E00008 */  jr $ra
    /* 800BF1C4 00000000 */  nop
