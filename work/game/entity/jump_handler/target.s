# Source: game_code.bin (decompressed)
# Address: 0x80095BA0

glabel jump_handler
    /* 80095BA0 3C0E8014 */  lui $t6, 0x8014
    /* 80095BA4 A2990008 */  sb $t9, 8($s4)
    /* 80095BA8 44810000 */  .word 0x44810000
    /* 80095BAC 8DCE4C48 */  lw $t6, 19528($t6)
    /* 80095BB0 E6800010 */  swc1 $f0, 16($s4)
    /* 80095BB4 00CE7821 */  addu $t7, $a2, $t6
    /* 80095BB8 AE8F000C */  sw $t7, 12($s4)
    /* 80095BBC E6800018 */  swc1 $f0, 24($s4)
    /* 80095BC0 E680001C */  swc1 $f0, 28($s4)
    /* 80095BC4 C7A4004C */  lwc1 $f4, 76($sp)
    /* 80095BC8 0C0254F3 */  jal 0x800953CC
    /* 80095BCC E6840014 */  swc1 $f4, 20($s4)
    /* 80095BD0 10000003 */  beq $zero, $zero, 0x80095BE0
    /* 80095BD4 8FBF002C */  lw $ra, 44($sp)
    /* 80095BD8 E4460014 */  swc1 $f6, 20($v0)
    /* 80095BDC 8FBF002C */  lw $ra, 44($sp)
    /* 80095BE0 8FB00018 */  lw $s0, 24($sp)
    /* 80095BE4 8FB1001C */  lw $s1, 28($sp)
    /* 80095BE8 8FB20020 */  lw $s2, 32($sp)
    /* 80095BEC 8FB30024 */  lw $s3, 36($sp)
    /* 80095BF0 8FB40028 */  lw $s4, 40($sp)
    /* 80095BF4 03E00008 */  jr $ra
    /* 80095BF8 27BD0048 */  addiu $sp, $sp, 72
