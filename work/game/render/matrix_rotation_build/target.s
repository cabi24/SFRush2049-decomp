# Source: game_code.bin (decompressed)
# Address: 0x8008D714

glabel matrix_rotation_build
    /* 8008D714 27BDFFE8 */  addiu $sp, $sp, -24
    /* 8008D718 00F83821 */  addu $a3, $a3, $t8
    /* 8008D71C AFBF0014 */  sw $ra, 20($sp)
    /* 8008D720 AFA40018 */  sw $a0, 24($sp)
    /* 8008D724 10A00007 */  beq $a1, $zero, 0x8008D744
    /* 8008D728 8CE7E708 */  lw $a3, -6392($a3)
    /* 8008D72C C4A40000 */  lwc1 $f4, 0($a1)
    /* 8008D730 E4E40024 */  swc1 $f4, 36($a3)
    /* 8008D734 C4A60004 */  lwc1 $f6, 4($a1)
    /* 8008D738 E4E60028 */  swc1 $f6, 40($a3)
    /* 8008D73C C4A80008 */  lwc1 $f8, 8($a1)
    /* 8008D740 E4E8002C */  swc1 $f8, 44($a3)
    /* 8008D744 10C00003 */  beq $a2, $zero, 0x8008D754
    /* 8008D748 00C02025 */  or $a0, $a2, $zero
    /* 8008D74C 0C0235AC */  jal 0x8008D6B0
    /* 8008D750 00E02825 */  or $a1, $a3, $zero
    /* 8008D754 8FBF0014 */  lw $ra, 20($sp)
    /* 8008D758 27BD0018 */  addiu $sp, $sp, 24
    /* 8008D75C 03E00008 */  jr $ra
    /* 8008D760 00000000 */  nop
