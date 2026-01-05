# Source: game_code.bin (decompressed)
# Address: 0x800B71A0

glabel sound_pitch_diff_halved
    /* 800B71A0 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800B71A4 AFBF0014 */  sw $ra, 20($sp)
    /* 800B71A8 AFA5001C */  sw $a1, 28($sp)
    /* 800B71AC 0C02CFE9 */  jal 0x800B3FA4
    /* 800B71B0 2405FFFF */  addiu $a1, $zero, -1
    /* 800B71B4 87AE001E */  lh $t6, 30($sp)
    /* 800B71B8 8FBF0014 */  lw $ra, 20($sp)
    /* 800B71BC 00027842 */  srl $t7, $v0, 1
    /* 800B71C0 01CF1023 */  subu $v0, $t6, $t7
    /* 800B71C4 0002C400 */  sll $t8, $v0, 16
    /* 800B71C8 00181403 */  sra $v0, $t8, 16
    /* 800B71CC 03E00008 */  jr $ra
    /* 800B71D0 27BD0018 */  addiu $sp, $sp, 24
