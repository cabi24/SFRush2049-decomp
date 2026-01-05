# Source: game_code.bin (decompressed)
# Address: 0x8008A710

glabel audio_task_start
    /* 8008A710 27BDFFE0 */  addiu $sp, $sp, -32
    /* 8008A714 240F0001 */  addiu $t7, $zero, 1
    /* 8008A718 15C0000D */  bne $t6, $zero, 0x8008A750
    /* 8008A71C AFBF0014 */  sw $ra, 20($sp)
    /* 8008A720 3C048015 */  lui $a0, 0x8015
    /* 8008A724 3C058015 */  lui $a1, 0x8015
    /* 8008A728 A04F0000 */  sb $t7, 0($v0)
    /* 8008A72C 24A527E4 */  addiu $a1, $a1, 10212
    /* 8008A730 248497D0 */  addiu $a0, $a0, -26672
    /* 8008A734 0C001A80 */  jal 0x80006A00
    /* 8008A738 24060001 */  addiu $a2, $zero, 1
    /* 8008A73C 3C048015 */  lui $a0, 0x8015
    /* 8008A740 248497D0 */  addiu $a0, $a0, -26672
    /* 8008A744 00002825 */  or $a1, $zero, $zero
    /* 8008A748 0C001D78 */  jal 0x800075E0
    /* 8008A74C 00003025 */  or $a2, $zero, $zero
    /* 8008A750 3C048015 */  lui $a0, 0x8015
    /* 8008A754 248497D0 */  addiu $a0, $a0, -26672
    /* 8008A758 27A5001C */  addiu $a1, $sp, 28
    /* 8008A75C 0C001C9C */  jal 0x80007270
    /* 8008A760 24060001 */  addiu $a2, $zero, 1
    /* 8008A764 8FBF0014 */  lw $ra, 20($sp)
    /* 8008A768 27BD0020 */  addiu $sp, $sp, 32
    /* 8008A76C 03E00008 */  jr $ra
    /* 8008A770 00000000 */  nop
