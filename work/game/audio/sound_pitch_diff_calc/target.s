# Source: game_code.bin (decompressed)
# Address: 0x800B7170

glabel sound_pitch_diff_calc
    /* 800B7170 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800B7174 AFBF0014 */  sw $ra, 20($sp)
    /* 800B7178 AFA5001C */  sw $a1, 28($sp)
    /* 800B717C 0C02CFE9 */  jal 0x800B3FA4
    /* 800B7180 2405FFFF */  addiu $a1, $zero, -1
    /* 800B7184 87AE001E */  lh $t6, 30($sp)
    /* 800B7188 8FBF0014 */  lw $ra, 20($sp)
    /* 800B718C 27BD0018 */  addiu $sp, $sp, 24
    /* 800B7190 01C21023 */  subu $v0, $t6, $v0
    /* 800B7194 00027C00 */  sll $t7, $v0, 16
    /* 800B7198 03E00008 */  jr $ra
    /* 800B719C 000F1403 */  sra $v0, $t7, 16
