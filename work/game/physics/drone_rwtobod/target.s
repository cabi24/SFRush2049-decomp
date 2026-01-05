# Source: game_code.bin (decompressed)
# Address: 0x800A373C

glabel drone_rwtobod
    /* 800A373C 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800A3740 27184030 */  addiu $t8, $t8, 16432
    /* 800A3744 000F7880 */  sll $t7, $t7, 2
    /* 800A3748 AFBF0014 */  sw $ra, 20($sp)
    /* 800A374C 01F88021 */  addu $s0, $t7, $t8
    /* 800A3750 AFA70018 */  sw $a3, 24($sp)
    /* 800A3754 82110005 */  lb $s1, 5($s0)
    /* 800A3758 02002025 */  or $a0, $s0, $zero
    /* 800A375C 00002825 */  or $a1, $zero, $zero
    /* 800A3760 0C0009E4 */  jal 0x80002790
    /* 800A3764 24060304 */  addiu $a2, $zero, 772
    /* 800A3768 8FBF0014 */  lw $ra, 20($sp)
    /* 800A376C A2110005 */  sb $s1, 5($s0)
    /* 800A3770 27BD0018 */  addiu $sp, $sp, 24
    /* 800A3774 03E00008 */  jr $ra
    /* 800A3778 00000000 */  nop
