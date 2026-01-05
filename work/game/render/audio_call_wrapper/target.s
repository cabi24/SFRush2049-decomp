# Source: game_code.bin (decompressed)
# Address: 0x8008A750

glabel audio_call_wrapper
    /* 8008A750 3C048015 */  lui $a0, 0x8015
    /* 8008A754 248497D0 */  addiu $a0, $a0, -26672
    /* 8008A758 27A5001C */  addiu $a1, $sp, 28
    /* 8008A75C 0C001C9C */  jal 0x80007270
    /* 8008A760 24060001 */  addiu $a2, $zero, 1
    /* 8008A764 8FBF0014 */  lw $ra, 20($sp)
    /* 8008A768 27BD0020 */  addiu $sp, $sp, 32
    /* 8008A76C 03E00008 */  jr $ra
    /* 8008A770 00000000 */  nop
