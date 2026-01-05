# Source: game_code.bin (decompressed)
# Address: 0x800B5FC4

glabel resource_type_select
    /* 800B5FC4 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800B5FC8 308E0001 */  andi $t6, $a0, 0x0001
    /* 800B5FCC 11C00003 */  beq $t6, $zero, 0x800B5FDC
    /* 800B5FD0 AFBF0014 */  sw $ra, 20($sp)
    /* 800B5FD4 10000007 */  beq $zero, $zero, 0x800B5FF4
    /* 800B5FD8 2404002E */  addiu $a0, $zero, 46
    /* 800B5FDC 308F0002 */  andi $t7, $a0, 0x0002
    /* 800B5FE0 11E00003 */  beq $t7, $zero, 0x800B5FF0
    /* 800B5FE4 24020026 */  addiu $v0, $zero, 38
    /* 800B5FE8 10000001 */  beq $zero, $zero, 0x800B5FF0
    /* 800B5FEC 24020025 */  addiu $v0, $zero, 37
    /* 800B5FF0 00402025 */  or $a0, $v0, $zero
    /* 800B5FF4 00002825 */  or $a1, $zero, $zero
    /* 800B5FF8 24060001 */  addiu $a2, $zero, 1
    /* 800B5FFC 0C0248D8 */  jal 0x80092360
    /* 800B6000 00003825 */  or $a3, $zero, $zero
    /* 800B6004 8FBF0014 */  lw $ra, 20($sp)
    /* 800B6008 27BD0018 */  addiu $sp, $sp, 24
    /* 800B600C 03E00008 */  jr $ra
    /* 800B6010 00000000 */  nop
