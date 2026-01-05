# Source: game_code.bin (decompressed)
# Address: 0x80095A24

glabel sound_priority_set
    /* 80095A24 27BDFFB8 */  addiu $sp, $sp, -72
    /* 80095A28 2401FFFF */  addiu $at, $zero, -1
    /* 80095A2C AFBF002C */  sw $ra, 44($sp)
    /* 80095A30 AFB40028 */  sw $s4, 40($sp)
    /* 80095A34 AFB30024 */  sw $s3, 36($sp)
    /* 80095A38 AFB20020 */  sw $s2, 32($sp)
    /* 80095A3C AFB1001C */  sw $s1, 28($sp)
    /* 80095A40 AFB00018 */  sw $s0, 24($sp)
    /* 80095A44 1081002A */  beq $a0, $at, 0x80095AF0
    /* 80095A48 AFA5004C */  sw $a1, 76($sp)
    /* 80095A4C 3C028014 */  lui $v0, 0x8014
    /* 80095A50 308600FF */  andi $a2, $a0, 0x00FF
    /* 80095A54 8C424C48 */  lw $v0, 19528($v0)
    /* 80095A58 00067080 */  sll $t6, $a2, 2
    /* 80095A5C 01C67023 */  subu $t6, $t6, $a2
    /* 80095A60 000E70C0 */  sll $t6, $t6, 3
    /* 80095A64 004E7821 */  addu $t7, $v0, $t6
    /* 80095A68 8DF80008 */  lw $t8, 8($t7)
    /* 80095A6C 01C03025 */  or $a2, $t6, $zero
    /* 80095A70 54980020 */  .word 0x54980020
    /* 80095A74 8FBF002C */  lw $ra, 44($sp)
    /* 80095A78 0C0255AF */  jal 0x800956BC
    /* 80095A7C 01C22021 */  addu $a0, $t6, $v0
    /* 80095A80 1440001A */  bne $v0, $zero, 0x80095AEC
    /* 80095A84 C7A6004C */  lwc1 $f6, 76($sp)
    /* 80095A88 3C048014 */  lui $a0, 0x8014
    /* 80095A8C 24846138 */  addiu $a0, $a0, 24888
    /* 80095A90 8C85000C */  lw $a1, 12($a0)
    /* 80095A94 A0A00009 */  sb $zero, 9($a1)
    /* 80095A98 AFA60034 */  sw $a2, 52($sp)
    /* 80095A9C 0C024847 */  jal 0x8009211C
    /* 80095AA0 AFA5003C */  sw $a1, 60($sp)
    /* 80095AA4 8FB4003C */  lw $s4, 60($sp)
    /* 80095AA8 8FA60034 */  lw $a2, 52($sp)
    /* 80095AAC 24190003 */  addiu $t9, $zero, 3
    /* 80095AB0 3C01C000 */  lui $at, 0xC000
    /* 80095AB4 3C0E8014 */  lui $t6, 0x8014
    /* 80095AB8 A2990008 */  sb $t9, 8($s4)
    /* 80095ABC 44810000 */  .word 0x44810000
    /* 80095AC0 8DCE4C48 */  lw $t6, 19528($t6)
    /* 80095AC4 E6800010 */  swc1 $f0, 16($s4)
    /* 80095AC8 00CE7821 */  addu $t7, $a2, $t6
    /* 80095ACC AE8F000C */  sw $t7, 12($s4)
    /* 80095AD0 E6800014 */  swc1 $f0, 20($s4)
    /* 80095AD4 E680001C */  swc1 $f0, 28($s4)
    /* 80095AD8 C7A4004C */  lwc1 $f4, 76($sp)
    /* 80095ADC 0C0254F3 */  jal 0x800953CC
    /* 80095AE0 E6840018 */  swc1 $f4, 24($s4)
    /* 80095AE4 10000003 */  beq $zero, $zero, 0x80095AF4
    /* 80095AE8 8FBF002C */  lw $ra, 44($sp)
    /* 80095AEC E4460018 */  swc1 $f6, 24($v0)
    /* 80095AF0 8FBF002C */  lw $ra, 44($sp)
    /* 80095AF4 8FB00018 */  lw $s0, 24($sp)
    /* 80095AF8 8FB1001C */  lw $s1, 28($sp)
    /* 80095AFC 8FB20020 */  lw $s2, 32($sp)
    /* 80095B00 8FB30024 */  lw $s3, 36($sp)
    /* 80095B04 8FB40028 */  lw $s4, 40($sp)
    /* 80095B08 03E00008 */  jr $ra
    /* 80095B0C 27BD0048 */  addiu $sp, $sp, 72
