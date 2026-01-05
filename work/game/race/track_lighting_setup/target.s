# Source: game_code.bin (decompressed)
# Address: 0x800F5EF8

glabel track_lighting_setup
    /* 800F5EF8 3C0E8015 */  lui $t6, 0x8015
    /* 800F5EFC 8DCEA110 */  lw $t6, -24304($t6)
    /* 800F5F00 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800F5F04 AFBF0014 */  sw $ra, 20($sp)
    /* 800F5F08 2DC10007 */  sltiu $at, $t6, 7
    /* 800F5F0C 1020001C */  beq $at, $zero, 0x800F5F80
    /* 800F5F10 000E7080 */  sll $t6, $t6, 2
    /* 800F5F14 3C018012 */  lui $at, 0x8012
    /* 800F5F18 002E0821 */  addu $at, $at, $t6
    /* 800F5F1C 8C2E45E4 */  lw $t6, 17892($at)
    /* 800F5F20 01C00008 */  jr $t6
    /* 800F5F24 00000000 */  nop
    /* 800F5F28 0C03D5B8 */  jal 0x800F56E0
    /* 800F5F2C 00000000 */  nop
    /* 800F5F30 10000014 */  beq $zero, $zero, 0x800F5F84
    /* 800F5F34 8FBF0014 */  lw $ra, 20($sp)
    /* 800F5F38 0C03D530 */  jal 0x800F54C0
    /* 800F5F3C 00000000 */  nop
    /* 800F5F40 10000010 */  beq $zero, $zero, 0x800F5F84
    /* 800F5F44 8FBF0014 */  lw $ra, 20($sp)
    /* 800F5F48 0C03D3FB */  jal 0x800F4FEC
    /* 800F5F4C 00000000 */  nop
    /* 800F5F50 1000000C */  beq $zero, $zero, 0x800F5F84
    /* 800F5F54 8FBF0014 */  lw $ra, 20($sp)
    /* 800F5F58 0C03D365 */  jal 0x800F4D94
    /* 800F5F5C 00000000 */  nop
    /* 800F5F60 10000008 */  beq $zero, $zero, 0x800F5F84
    /* 800F5F64 8FBF0014 */  lw $ra, 20($sp)
    /* 800F5F68 0C03D2E3 */  jal 0x800F4B8C
    /* 800F5F6C 00000000 */  nop
    /* 800F5F70 10000004 */  beq $zero, $zero, 0x800F5F84
    /* 800F5F74 8FBF0014 */  lw $ra, 20($sp)
    /* 800F5F78 0C03D25F */  jal 0x800F497C
    /* 800F5F7C 00000000 */  nop
    /* 800F5F80 8FBF0014 */  lw $ra, 20($sp)
    /* 800F5F84 27BD0018 */  addiu $sp, $sp, 24
    /* 800F5F88 03E00008 */  jr $ra
    /* 800F5F8C 00000000 */  nop
