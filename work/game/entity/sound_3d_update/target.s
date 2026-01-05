# Source: game_code.bin (decompressed)
# Address: 0x80096240

glabel sound_3d_update
    /* 80096240 27BDFFE8 */  addiu $sp, $sp, -24
    /* 80096244 2401FFFF */  addiu $at, $zero, -1
    /* 80096248 10A1000B */  beq $a1, $at, 0x80096278
    /* 8009624C AFBF0014 */  sw $ra, 20($sp)
    /* 80096250 0C00638B */  jal 0x80018E2C
    /* 80096254 00A02025 */  or $a0, $a1, $zero
    /* 80096258 0C005529 */  jal 0x800154A4
    /* 8009625C 00000000 */  nop
    /* 80096260 3C048015 */  lui $a0, 0x8015
    /* 80096264 0C02584C */  jal 0x80096130
    /* 80096268 8C841AD4 */  lw $a0, 6868($a0)
    /* 8009626C 240EFFFF */  addiu $t6, $zero, -1
    /* 80096270 3C018012 */  lui $at, 0x8012
    /* 80096274 AC2EEAA0 */  sw $t6, -5472($at)
    /* 80096278 8FBF0014 */  lw $ra, 20($sp)
    /* 8009627C 27BD0018 */  addiu $sp, $sp, 24
    /* 80096280 03E00008 */  jr $ra
    /* 80096284 00000000 */  nop
