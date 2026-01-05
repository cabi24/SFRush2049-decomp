# Source: game_code.bin (decompressed)
# Address: 0x800B2D80

glabel engine_sound_start
    /* 800B2D80 8E4F0000 */  lw $t7, 0($s2)
    /* 800B2D84 26050004 */  addiu $a1, $s0, 4
    /* 800B2D88 03130019 */  multu $t8, $s3
    /* 800B2D8C 24060012 */  addiu $a2, $zero, 18
    /* 800B2D90 0000C812 */  mflo $t9
    /* 800B2D94 01F92021 */  addu $a0, $t7, $t9
    /* 800B2D98 0C001F1A */  jal 0x80007C68
    /* 800B2D9C 24840004 */  addiu $a0, $a0, 4
    /* 800B2DA0 96080016 */  lhu $t0, 22($s0)
    /* 800B2DA4 8E8A0000 */  lw $t2, 0($s4)
    /* 800B2DA8 26050018 */  addiu $a1, $s0, 24
    /* 800B2DAC 000848C0 */  sll $t1, $t0, 3
    /* 800B2DB0 24060008 */  addiu $a2, $zero, 8
    /* 800B2DB4 0C001F1A */  jal 0x80007C68
    /* 800B2DB8 012A2021 */  addu $a0, $t1, $t2
    /* 800B2DBC 3C028015 */  lui $v0, 0x8015
    /* 800B2DC0 9442267C */  lhu $v0, 9852($v0)
    /* 800B2DC4 26310001 */  addiu $s1, $s1, 1
    /* 800B2DC8 0222082A */  slt $at, $s1, $v0
    /* 800B2DCC 1420FFE8 */  bne $at, $zero, 0x800B2D70
    /* 800B2DD0 26100020 */  addiu $s0, $s0, 32
    /* 800B2DD4 8FBF002C */  lw $ra, 44($sp)
    /* 800B2DD8 8FB00014 */  lw $s0, 20($sp)
    /* 800B2DDC 8FB10018 */  lw $s1, 24($sp)
    /* 800B2DE0 8FB2001C */  lw $s2, 28($sp)
    /* 800B2DE4 8FB30020 */  lw $s3, 32($sp)
    /* 800B2DE8 8FB40024 */  lw $s4, 36($sp)
    /* 800B2DEC 8FB50028 */  lw $s5, 40($sp)
    /* 800B2DF0 03E00008 */  jr $ra
    /* 800B2DF4 27BD0030 */  addiu $sp, $sp, 48
