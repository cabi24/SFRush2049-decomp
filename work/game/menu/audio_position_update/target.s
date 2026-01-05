# Source: game_code.bin (decompressed)
# Address: 0x800E6A00

glabel audio_position_update
    /* 800E6A00 14200003 */  bne $at, $zero, 0x800E6A10
    /* 800E6A04 240F0003 */  addiu $t7, $zero, 3
    /* 800E6A08 10000007 */  beq $zero, $zero, 0x800E6A28
    /* 800E6A0C A12F0730 */  sb $t7, 1840($t1)
    /* 800E6A10 28410033 */  slti $at, $v0, 51
    /* 800E6A14 54200004 */  .word 0x54200004
    /* 800E6A18 A12B0730 */  sb $t3, 1840($t1)
    /* 800E6A1C 10000002 */  beq $zero, $zero, 0x800E6A28
    /* 800E6A20 A1260730 */  sb $a2, 1840($t1)
    /* 800E6A24 A12B0730 */  sb $t3, 1840($t1)
    /* 800E6A28 8CF90000 */  lw $t9, 0($a3)
    /* 800E6A2C 3C028015 */  lui $v0, 0x8015
    /* 800E6A30 50D9000F */  .word 0x50D9000F
    /* 800E6A34 A1200731 */  sb $zero, 1841($t1)
    /* 800E6A38 81AE0000 */  lb $t6, 0($t5)
    /* 800E6A3C 55C0000C */  .word 0x55C0000C
    /* 800E6A40 A1200731 */  sb $zero, 1841($t1)
    /* 800E6A44 914F0001 */  lbu $t7, 1($t2)
    /* 800E6A48 8D58003C */  lw $t8, 60($t2)
    /* 800E6A4C 000FC880 */  sll $t9, $t7, 2
    /* 800E6A50 01197021 */  addu $t6, $t0, $t9
    /* 800E6A54 8DCF0000 */  lw $t7, 0($t6)
    /* 800E6A58 030FC824 */  and $t9, $t8, $t7
    /* 800E6A5C 53200004 */  .word 0x53200004
    /* 800E6A60 A1200731 */  sb $zero, 1841($t1)
    /* 800E6A64 10000002 */  beq $zero, $zero, 0x800E6A70
    /* 800E6A68 A12B0731 */  sb $t3, 1841($t1)
    /* 800E6A6C A1200731 */  sb $zero, 1841($t1)
    /* 800E6A70 8CEE0000 */  lw $t6, 0($a3)
    /* 800E6A74 24010006 */  addiu $at, $zero, 6
    /* 800E6A78 51C10011 */  .word 0x51C10011
    /* 800E6A7C A1200732 */  sb $zero, 1842($t1)
    /* 800E6A80 81B80000 */  lb $t8, 0($t5)
    /* 800E6A84 5700000E */  .word 0x5700000E
    /* 800E6A88 A1200732 */  sb $zero, 1842($t1)
    /* 800E6A8C 91590001 */  lbu $t9, 1($t2)
    /* 800E6A90 8D4F0030 */  lw $t7, 48($t2)
    /* 800E6A94 00197080 */  sll $t6, $t9, 2
    /* 800E6A98 020EC021 */  addu $t8, $s0, $t6
    /* 800E6A9C 8F190000 */  lw $t9, 0($t8)
    /* 800E6AA0 01F97024 */  and $t6, $t7, $t9
    /* 800E6AA4 51C00006 */  .word 0x51C00006
    /* 800E6AA8 A1200732 */  sb $zero, 1842($t1)
    /* 800E6AAC A12B0732 */  sb $t3, 1842($t1)
    /* 800E6AB0 3C028015 */  lui $v0, 0x8015
    /* 800E6AB4 10000003 */  beq $zero, $zero, 0x800E6AC4
    /* 800E6AB8 8442A108 */  lh $v0, -24312($v0)
    /* 800E6ABC A1200732 */  sb $zero, 1842($t1)
    /* 800E6AC0 8442A108 */  lh $v0, -24312($v0)
    /* 800E6AC4 258C0001 */  addiu $t4, $t4, 1
    /* 800E6AC8 0182082A */  slt $at, $t4, $v0
    /* 800E6ACC 1420FF63 */  bne $at, $zero, 0x800E685C
    /* 800E6AD0 254A004C */  addiu $t2, $t2, 76
    /* 800E6AD4 8FBF001C */  lw $ra, 28($sp)
    /* 800E6AD8 8FB00014 */  lw $s0, 20($sp)
    /* 800E6ADC 8FB10018 */  lw $s1, 24($sp)
    /* 800E6AE0 03E00008 */  jr $ra
    /* 800E6AE4 27BD0020 */  addiu $sp, $sp, 32
