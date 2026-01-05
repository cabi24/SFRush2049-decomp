# Source: game_code.bin (decompressed)
# Address: 0x800959DC

glabel audio_pitch_adjust
    /* 800959DC 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800959E0 AFBF0014 */  sw $ra, 20($sp)
    /* 800959E4 240E0003 */  addiu $t6, $zero, 3
    /* 800959E8 AE0E0010 */  sw $t6, 16($s0)
    /* 800959EC 8E040008 */  lw $a0, 8($s0)
    /* 800959F0 0C024847 */  jal 0x8009211C
    /* 800959F4 02002825 */  or $a1, $s0, $zero
    /* 800959F8 3C118014 */  lui $s1, 0x8014
    /* 800959FC 26314C50 */  addiu $s1, $s1, 19536
    /* 80095A00 02202025 */  or $a0, $s1, $zero
    /* 80095A04 8E260008 */  lw $a2, 8($s1)
    /* 80095A08 0C0247EF */  jal 0x80091FBC
    /* 80095A0C 02002825 */  or $a1, $s0, $zero
    /* 80095A10 AE110008 */  sw $s1, 8($s0)
    /* 80095A14 8FBF0014 */  lw $ra, 20($sp)
    /* 80095A18 27BD0018 */  addiu $sp, $sp, 24
    /* 80095A1C 03E00008 */  jr $ra
    /* 80095A20 00000000 */  nop
