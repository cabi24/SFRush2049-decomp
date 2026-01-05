# Source: game_code.bin (decompressed)
# Address: 0x800954E0

glabel drone_move_update
    /* 800954E0 8FB40034 */  lw $s4, 52($sp)
    /* 800954E4 240E0004 */  addiu $t6, $zero, 4
    /* 800954E8 A28E0008 */  sb $t6, 8($s4)
    /* 800954EC 8FAF0040 */  lw $t7, 64($sp)
    /* 800954F0 AE80000C */  sw $zero, 12($s4)
    /* 800954F4 448F2000 */  .word 0x448F2000
    /* 800954F8 00000000 */  nop
    /* 800954FC 468021A0 */  .word 0x468021A0
    /* 80095500 0C0254F3 */  jal 0x800953CC
    /* 80095504 E6860010 */  swc1 $f6, 16($s4)
    /* 80095508 8FBF002C */  lw $ra, 44($sp)
    /* 8009550C 8FB00018 */  lw $s0, 24($sp)
    /* 80095510 8FB1001C */  lw $s1, 28($sp)
    /* 80095514 8FB20020 */  lw $s2, 32($sp)
    /* 80095518 8FB30024 */  lw $s3, 36($sp)
    /* 8009551C 8FB40028 */  lw $s4, 40($sp)
    /* 80095520 03E00008 */  jr $ra
    /* 80095524 27BD0040 */  addiu $sp, $sp, 64
