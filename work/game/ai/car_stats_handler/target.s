# Source: game_code.bin (decompressed)
# Address: 0x800D18E4

glabel car_stats_handler
    /* 800D18E4 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800D18E8 AFBF0014 */  sw $ra, 20($sp)
    /* 800D18EC 0C024847 */  jal 0x8009211C
    /* 800D18F0 AFA5001C */  sw $a1, 28($sp)
    /* 800D18F4 8FA5001C */  lw $a1, 28($sp)
    /* 800D18F8 3C038011 */  lui $v1, 0x8011
    /* 800D18FC 3C048014 */  lui $a0, 0x8014
    /* 800D1900 A0A0000C */  sb $zero, 12($a1)
    /* 800D1904 3C068014 */  lui $a2, 0x8014
    /* 800D1908 24C66104 */  addiu $a2, $a2, 24836
    /* 800D190C 248460F8 */  addiu $a0, $a0, 24824
    /* 800D1910 24630258 */  addiu $v1, $v1, 600
    /* 800D1914 8C6E0000 */  lw $t6, 0($v1)
    /* 800D1918 8C8F0000 */  lw $t7, 0($a0)
    /* 800D191C 8CC80000 */  lw $t0, 0($a2)
    /* 800D1920 8CB90008 */  lw $t9, 8($a1)
    /* 800D1924 01EEC004 */  .word 0x01EEC004
    /* 800D1928 03284824 */  and $t1, $t9, $t0
    /* 800D192C 03095025 */  or $t2, $t8, $t1
    /* 800D1930 ACAA0008 */  sw $t2, 8($a1)
    /* 800D1934 8C8D0000 */  lw $t5, 0($a0)
    /* 800D1938 8CCB0000 */  lw $t3, 0($a2)
    /* 800D193C 8C620000 */  lw $v0, 0($v1)
    /* 800D1940 25AE0001 */  addiu $t6, $t5, 1
    /* 800D1944 01606027 */  nor $t4, $t3, $zero
    /* 800D1948 01CC7806 */  .word 0x01CC7806
    /* 800D194C 004F082B */  sltu $at, $v0, $t7
    /* 800D1950 14200004 */  bne $at, $zero, 0x800D1964
    /* 800D1954 24480001 */  addiu $t0, $v0, 1
    /* 800D1958 24190001 */  addiu $t9, $zero, 1
    /* 800D195C 10000002 */  beq $zero, $zero, 0x800D1968
    /* 800D1960 AC790000 */  sw $t9, 0($v1)
    /* 800D1964 AC680000 */  sw $t0, 0($v1)
    /* 800D1968 8FBF0014 */  lw $ra, 20($sp)
    /* 800D196C 27BD0020 */  addiu $sp, $sp, 32
    /* 800D1970 00A01025 */  or $v0, $a1, $zero
    /* 800D1974 03E00008 */  jr $ra
    /* 800D1978 00000000 */  nop
