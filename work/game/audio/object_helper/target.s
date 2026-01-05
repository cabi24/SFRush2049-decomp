# Source: game_code.bin (decompressed)
# Address: 0x800B3F50

glabel object_helper
    /* 800B3F50 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800B3F54 AFBF0014 */  sw $ra, 20($sp)
    /* 800B3F58 0C02CF46 */  jal 0x800B3D18
    /* 800B3F5C 00004025 */  or $t0, $zero, $zero
    /* 800B3F60 3C0C8015 */  lui $t4, 0x8015
    /* 800B3F64 258C97F0 */  addiu $t4, $t4, -26640
    /* 800B3F68 8D8E0000 */  lw $t6, 0($t4)
    /* 800B3F6C 00004025 */  or $t0, $zero, $zero
    /* 800B3F70 0C02CF46 */  jal 0x800B3D18
    /* 800B3F74 91C90003 */  lbu $t1, 3($t6)
    /* 800B3F78 8D8F0000 */  lw $t7, 0($t4)
    /* 800B3F7C 3C028015 */  lui $v0, 0x8015
    /* 800B3F80 80429B60 */  lb $v0, -25760($v0)
    /* 800B3F84 91E30002 */  lbu $v1, 2($t7)
    /* 800B3F88 8FBF0014 */  lw $ra, 20($sp)
    /* 800B3F8C 27BD0018 */  addiu $sp, $sp, 24
    /* 800B3F90 0043C021 */  addu $t8, $v0, $v1
    /* 800B3F94 03091021 */  addu $v0, $t8, $t1
    /* 800B3F98 0002CC00 */  sll $t9, $v0, 16
    /* 800B3F9C 03E00008 */  jr $ra
    /* 800B3FA0 00191403 */  sra $v0, $t9, 16
