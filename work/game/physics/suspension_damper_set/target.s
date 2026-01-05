# Source: game_code.bin (decompressed)
# Address: 0x800A5160

glabel suspension_damper_set
    /* 800A5160 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800A5164 AFBF001C */  sw $ra, 28($sp)
    /* 800A5168 15C00017 */  bne $t6, $zero, 0x800A51C8
    /* 800A516C 240F0001 */  addiu $t7, $zero, 1
    /* 800A5170 3C018012 */  lui $at, 0x8012
    /* 800A5174 A02F8EE4 */  sb $t7, -28956($at)
    /* 800A5178 3C018014 */  lui $at, 0x8014
    /* 800A517C 3C028003 */  lui $v0, 0x8003
    /* 800A5180 9044E860 */  lbu $a0, -6048($v0)
    /* 800A5184 A420FEC8 */  sh $zero, -312($at)
    /* 800A5188 3C018014 */  lui $at, 0x8014
    /* 800A518C 00002825 */  or $a1, $zero, $zero
    /* 800A5190 00003025 */  or $a2, $zero, $zero
    /* 800A5194 00003825 */  or $a3, $zero, $zero
    /* 800A5198 AFA00010 */  sw $zero, 16($sp)
    /* 800A519C 0C025DE6 */  jal 0x80097798
    /* 800A51A0 A0240A10 */  sb $a0, 2576($at)
    /* 800A51A4 3C038014 */  lui $v1, 0x8014
    /* 800A51A8 24630AF0 */  addiu $v1, $v1, 2800
    /* 800A51AC AC620000 */  sw $v0, 0($v1)
    /* 800A51B0 0C0258E3 */  jal 0x8009638C
    /* 800A51B4 00402025 */  or $a0, $v0, $zero
    /* 800A51B8 0C029396 */  jal 0x800A4E58
    /* 800A51BC 00000000 */  nop
    /* 800A51C0 0C029443 */  jal 0x800A510C
    /* 800A51C4 00000000 */  nop
    /* 800A51C8 8FBF001C */  lw $ra, 28($sp)
    /* 800A51CC 27BD0020 */  addiu $sp, $sp, 32
    /* 800A51D0 03E00008 */  jr $ra
    /* 800A51D4 00000000 */  nop
