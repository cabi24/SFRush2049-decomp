# Source: game_code.bin (decompressed)
# Address: 0x800BF024

glabel results_screen_update
    /* 800BF024 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800BF028 AFA40018 */  sw $a0, 24($sp)
    /* 800BF02C AFBF0014 */  sw $ra, 20($sp)
    /* 800BF030 3C048014 */  lui $a0, 0x8014
    /* 800BF034 24842728 */  addiu $a0, $a0, 10024
    /* 800BF038 00002825 */  or $a1, $zero, $zero
    /* 800BF03C 0C001C9C */  jal 0x80007270
    /* 800BF040 24060001 */  addiu $a2, $zero, 1
    /* 800BF044 3C078014 */  lui $a3, 0x8014
    /* 800BF048 24E72728 */  addiu $a3, $a3, 10024
    /* 800BF04C 0C0246EA */  jal 0x80091BA8
    /* 800BF050 8FA40018 */  lw $a0, 24($sp)
    /* 800BF054 14400007 */  bne $v0, $zero, 0x800BF074
    /* 800BF058 00401825 */  or $v1, $v0, $zero
    /* 800BF05C 00E02025 */  or $a0, $a3, $zero
    /* 800BF060 00002825 */  or $a1, $zero, $zero
    /* 800BF064 0C001D78 */  jal 0x800075E0
    /* 800BF068 00003025 */  or $a2, $zero, $zero
    /* 800BF06C 1000000A */  beq $zero, $zero, 0x800BF098
    /* 800BF070 8FBF0014 */  lw $ra, 20($sp)
    /* 800BF074 0C02FC07 */  jal 0x800BF01C
    /* 800BF078 8C640040 */  lw $a0, 64($v1)
    /* 800BF07C 00E02025 */  or $a0, $a3, $zero
    /* 800BF080 00002825 */  or $a1, $zero, $zero
    /* 800BF084 0C001D78 */  jal 0x800075E0
    /* 800BF088 00003025 */  or $a2, $zero, $zero
    /* 800BF08C 0C024701 */  jal 0x80091C04
    /* 800BF090 8FA40018 */  lw $a0, 24($sp)
    /* 800BF094 8FBF0014 */  lw $ra, 20($sp)
    /* 800BF098 27BD0018 */  addiu $sp, $sp, 24
    /* 800BF09C 03E00008 */  jr $ra
    /* 800BF0A0 00000000 */  nop
