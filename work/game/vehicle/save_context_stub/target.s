# Source: game_code.bin (decompressed)
# Address: 0x8010FBF4

glabel save_context_stub
    /* 8010FBF4 27BDFFE8 */  addiu $sp, $sp, -24
    /* 8010FBF8 00802825 */  or $a1, $a0, $zero
    /* 8010FBFC AC2E5288 */  sw $t6, 21128($at)
    /* 8010FC00 AC20528C */  sw $zero, 21132($at)
    /* 8010FC04 AFBF0014 */  sw $ra, 20($sp)
    /* 8010FC08 3C018015 */  lui $at, 0x8015
    /* 8010FC0C 240F0002 */  addiu $t7, $zero, 2
    /* 8010FC10 3C048015 */  lui $a0, 0x8015
    /* 8010FC14 AC2F5240 */  sw $t7, 21056($at)
    /* 8010FC18 24845248 */  addiu $a0, $a0, 21064
    /* 8010FC1C 0C001F1A */  jal 0x80007C68
    /* 8010FC20 24060040 */  addiu $a2, $zero, 64
    /* 8010FC24 3C048003 */  lui $a0, 0x8003
    /* 8010FC28 3C058015 */  lui $a1, 0x8015
    /* 8010FC2C 24A55238 */  addiu $a1, $a1, 21048
    /* 8010FC30 2484E960 */  addiu $a0, $a0, -5792
    /* 8010FC34 0C001D78 */  jal 0x800075E0
    /* 8010FC38 24060001 */  addiu $a2, $zero, 1
    /* 8010FC3C 3C048003 */  lui $a0, 0x8003
    /* 8010FC40 2484E928 */  addiu $a0, $a0, -5848
    /* 8010FC44 2405029E */  addiu $a1, $zero, 670
    /* 8010FC48 0C001D78 */  jal 0x800075E0
    /* 8010FC4C 24060001 */  addiu $a2, $zero, 1
    /* 8010FC50 8FBF0014 */  lw $ra, 20($sp)
    /* 8010FC54 27BD0018 */  addiu $sp, $sp, 24
    /* 8010FC58 03E00008 */  jr $ra
    /* 8010FC5C 00000000 */  nop
