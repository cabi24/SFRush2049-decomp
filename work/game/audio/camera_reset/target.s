# Source: game_code.bin (decompressed)
# Address: 0x800BB69C

glabel camera_reset
    /* 800BB69C 27BDFFD0 */  addiu $sp, $sp, -48
    /* 800BB6A0 3C0E8014 */  lui $t6, 0x8014
    /* 800BB6A4 81CEF1D8 */  lb $t6, -3624($t6)
    /* 800BB6A8 AFBF002C */  sw $ra, 44($sp)
    /* 800BB6AC AFB50028 */  sw $s5, 40($sp)
    /* 800BB6B0 AFB40024 */  sw $s4, 36($sp)
    /* 800BB6B4 AFB30020 */  sw $s3, 32($sp)
    /* 800BB6B8 AFB2001C */  sw $s2, 28($sp)
    /* 800BB6BC AFB10018 */  sw $s1, 24($sp)
    /* 800BB6C0 19C00005 */  blez $t6, 0x800BB6D8
    /* 800BB6C4 AFB00014 */  sw $s0, 20($sp)
    /* 800BB6C8 0C02E43E */  jal 0x800B90F8
    /* 800BB6CC 00000000 */  nop
    /* 800BB6D0 10000040 */  beq $zero, $zero, 0x800BB7D4
    /* 800BB6D4 8FBF002C */  lw $ra, 44($sp)
    /* 800BB6D8 3C028015 */  lui $v0, 0x8015
    /* 800BB6DC 8042978C */  lb $v0, -26740($v0)
    /* 800BB6E0 24010007 */  addiu $at, $zero, 7
    /* 800BB6E4 10410011 */  beq $v0, $at, 0x800BB72C
    /* 800BB6E8 24010008 */  addiu $at, $zero, 8
    /* 800BB6EC 1041000F */  beq $v0, $at, 0x800BB72C
    /* 800BB6F0 24010009 */  addiu $at, $zero, 9
    /* 800BB6F4 1041000D */  beq $v0, $at, 0x800BB72C
    /* 800BB6F8 2401000A */  addiu $at, $zero, 10
    /* 800BB6FC 1041000B */  beq $v0, $at, 0x800BB72C
    /* 800BB700 2401000B */  addiu $at, $zero, 11
    /* 800BB704 10410009 */  beq $v0, $at, 0x800BB72C
    /* 800BB708 2401000C */  addiu $at, $zero, 12
    /* 800BB70C 10410007 */  beq $v0, $at, 0x800BB72C
    /* 800BB710 2401000D */  addiu $at, $zero, 13
    /* 800BB714 10410005 */  beq $v0, $at, 0x800BB72C
    /* 800BB718 24010011 */  addiu $at, $zero, 17
    /* 800BB71C 10410003 */  beq $v0, $at, 0x800BB72C
    /* 800BB720 24010012 */  addiu $at, $zero, 18
    /* 800BB724 14410005 */  bne $v0, $at, 0x800BB73C
    /* 800BB728 3C158015 */  lui $s5, 0x8015
    /* 800BB72C 0C02E43E */  jal 0x800B90F8
    /* 800BB730 00000000 */  nop
    /* 800BB734 10000027 */  beq $zero, $zero, 0x800BB7D4
    /* 800BB738 8FBF002C */  lw $ra, 44($sp)
    /* 800BB73C 26B51AD0 */  addiu $s5, $s5, 6864
    /* 800BB740 86AF0000 */  lh $t7, 0($s5)
    /* 800BB744 3C118012 */  lui $s1, 0x8012
    /* 800BB748 26314FD0 */  addiu $s1, $s1, 20432
    /* 800BB74C 19E00020 */  blez $t7, 0x800BB7D0
    /* 800BB750 00008025 */  or $s0, $zero, $zero
    /* 800BB754 3C148014 */  lui $s4, 0x8014
    /* 800BB758 3C138012 */  lui $s3, 0x8012
    /* 800BB75C 2673120C */  addiu $s3, $s3, 4620
    /* 800BB760 26940BDC */  addiu $s4, $s4, 3036
    /* 800BB764 92860000 */  lbu $a2, 0($s4)
    /* 800BB768 02602025 */  or $a0, $s3, $zero
    /* 800BB76C 00002825 */  or $a1, $zero, $zero
    /* 800BB770 24C6FFFF */  addiu $a2, $a2, -1
    /* 800BB774 0006C600 */  sll $t8, $a2, 24
    /* 800BB778 00183603 */  sra $a2, $t8, 24
    /* 800BB77C 0C024B8B */  jal 0x80092E2C
    /* 800BB780 24070001 */  addiu $a3, $zero, 1
    /* 800BB784 0C029F5B */  jal 0x800A7D6C
    /* 800BB788 00409025 */  or $s2, $v0, $zero
    /* 800BB78C 24080001 */  addiu $t0, $zero, 1
    /* 800BB790 02083804 */  .word 0x02083804
    /* 800BB794 00074C00 */  sll $t1, $a3, 16
    /* 800BB798 00095403 */  sra $t2, $t1, 16
    /* 800BB79C 000A5A00 */  sll $t3, $t2, 8
    /* 800BB7A0 AE220000 */  sw $v0, 0($s1)
    /* 800BB7A4 39670F00 */  xori $a3, $t3, 0x0F00
    /* 800BB7A8 02402025 */  or $a0, $s2, $zero
    /* 800BB7AC 00402825 */  or $a1, $v0, $zero
    /* 800BB7B0 0C02389B */  jal 0x8008E26C
    /* 800BB7B4 2406FFFF */  addiu $a2, $zero, -1
    /* 800BB7B8 86AD0000 */  lh $t5, 0($s5)
    /* 800BB7BC 26100001 */  addiu $s0, $s0, 1
    /* 800BB7C0 26310004 */  addiu $s1, $s1, 4
    /* 800BB7C4 020D082A */  slt $at, $s0, $t5
    /* 800BB7C8 5420FFE7 */  .word 0x5420FFE7
    /* 800BB7CC 92860000 */  lbu $a2, 0($s4)
    /* 800BB7D0 8FBF002C */  lw $ra, 44($sp)
    /* 800BB7D4 8FB00014 */  lw $s0, 20($sp)
    /* 800BB7D8 8FB10018 */  lw $s1, 24($sp)
    /* 800BB7DC 8FB2001C */  lw $s2, 28($sp)
    /* 800BB7E0 8FB30020 */  lw $s3, 32($sp)
    /* 800BB7E4 8FB40024 */  lw $s4, 36($sp)
    /* 800BB7E8 8FB50028 */  lw $s5, 40($sp)
    /* 800BB7EC 03E00008 */  jr $ra
    /* 800BB7F0 27BD0030 */  addiu $sp, $sp, 48
