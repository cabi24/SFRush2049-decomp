# Source: game_code.bin (decompressed)
# Address: 0x800B4C80

glabel reverb_update
    /* 800B4C80 80620068 */  lb $v0, 104($v1)
    /* 800B4C84 10000043 */  beq $zero, $zero, 0x800B4D94
    /* 800B4C88 80620069 */  lb $v0, 105($v1)
    /* 800B4C8C 10000041 */  beq $zero, $zero, 0x800B4D94
    /* 800B4C90 8062006A */  lb $v0, 106($v1)
    /* 800B4C94 1000003F */  beq $zero, $zero, 0x800B4D94
    /* 800B4C98 8062006B */  lb $v0, 107($v1)
    /* 800B4C9C 00001025 */  or $v0, $zero, $zero
    /* 800B4CA0 1000003D */  beq $zero, $zero, 0x800B4D98
    /* 800B4CA4 8FBF0014 */  lw $ra, 20($sp)
    /* 800B4CA8 2841000E */  slti $at, $v0, 14
    /* 800B4CAC 14200024 */  bne $at, $zero, 0x800B4D40
    /* 800B4CB0 28410012 */  slti $at, $v0, 18
    /* 800B4CB4 50200023 */  .word 0x50200023
    /* 800B4CB8 8C990000 */  lw $t9, 0($a0)
    /* 800B4CBC 8C8E0000 */  lw $t6, 0($a0)
    /* 800B4CC0 24D8FFE9 */  addiu $t8, $a2, -23
    /* 800B4CC4 2F01000E */  sltiu $at, $t8, 14
    /* 800B4CC8 8DCF002C */  lw $t7, 44($t6)
    /* 800B4CCC 0018C080 */  sll $t8, $t8, 2
    /* 800B4CD0 10200018 */  beq $at, $zero, 0x800B4D34
    /* 800B4CD4 8DE30000 */  lw $v1, 0($t7)
    /* 800B4CD8 3C018012 */  lui $at, 0x8012
    /* 800B4CDC 00380821 */  addu $at, $at, $t8
    /* 800B4CE0 8C383D58 */  lw $t8, 15704($at)
    /* 800B4CE4 03000008 */  jr $t8
    /* 800B4CE8 00000000 */  nop
    /* 800B4CEC 10000029 */  beq $zero, $zero, 0x800B4D94
    /* 800B4CF0 80620074 */  lb $v0, 116($v1)
    /* 800B4CF4 10000027 */  beq $zero, $zero, 0x800B4D94
    /* 800B4CF8 80620075 */  lb $v0, 117($v1)
    /* 800B4CFC 10000025 */  beq $zero, $zero, 0x800B4D94
    /* 800B4D00 80620076 */  lb $v0, 118($v1)
    /* 800B4D04 10000023 */  beq $zero, $zero, 0x800B4D94
    /* 800B4D08 80620077 */  lb $v0, 119($v1)
    /* 800B4D0C 10000021 */  beq $zero, $zero, 0x800B4D94
    /* 800B4D10 80620078 */  lb $v0, 120($v1)
    /* 800B4D14 1000001F */  beq $zero, $zero, 0x800B4D94
    /* 800B4D18 80620079 */  lb $v0, 121($v1)
    /* 800B4D1C 1000001D */  beq $zero, $zero, 0x800B4D94
    /* 800B4D20 8062007A */  lb $v0, 122($v1)
    /* 800B4D24 1000001B */  beq $zero, $zero, 0x800B4D94
    /* 800B4D28 8062007B */  lb $v0, 123($v1)
    /* 800B4D2C 10000019 */  beq $zero, $zero, 0x800B4D94
    /* 800B4D30 8062007C */  lb $v0, 124($v1)
    /* 800B4D34 00001025 */  or $v0, $zero, $zero
    /* 800B4D38 10000017 */  beq $zero, $zero, 0x800B4D98
    /* 800B4D3C 8FBF0014 */  lw $ra, 20($sp)
    /* 800B4D40 8C990000 */  lw $t9, 0($a0)
    /* 800B4D44 24010017 */  addiu $at, $zero, 23
    /* 800B4D48 8F2E002C */  lw $t6, 44($t9)
    /* 800B4D4C 10C1000A */  beq $a2, $at, 0x800B4D78
    /* 800B4D50 8DC30000 */  lw $v1, 0($t6)
    /* 800B4D54 2401001C */  addiu $at, $zero, 28
    /* 800B4D58 10C10009 */  beq $a2, $at, 0x800B4D80
    /* 800B4D5C 2401001D */  addiu $at, $zero, 29
    /* 800B4D60 10C10009 */  beq $a2, $at, 0x800B4D88
    /* 800B4D64 2401001E */  addiu $at, $zero, 30
    /* 800B4D68 50C1000A */  .word 0x50C1000A
    /* 800B4D6C 8062008B */  lb $v0, 139($v1)
    /* 800B4D70 10000008 */  beq $zero, $zero, 0x800B4D94
    /* 800B4D74 00001025 */  or $v0, $zero, $zero
    /* 800B4D78 10000006 */  beq $zero, $zero, 0x800B4D94
    /* 800B4D7C 80620088 */  lb $v0, 136($v1)
    /* 800B4D80 10000004 */  beq $zero, $zero, 0x800B4D94
    /* 800B4D84 80620089 */  lb $v0, 137($v1)
    /* 800B4D88 10000002 */  beq $zero, $zero, 0x800B4D94
    /* 800B4D8C 8062008A */  lb $v0, 138($v1)
    /* 800B4D90 8062008B */  lb $v0, 139($v1)
    /* 800B4D94 8FBF0014 */  lw $ra, 20($sp)
    /* 800B4D98 27BD0018 */  addiu $sp, $sp, 24
    /* 800B4D9C 03E00008 */  jr $ra
    /* 800B4DA0 00000000 */  nop
