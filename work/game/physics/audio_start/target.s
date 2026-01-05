# Source: game_code.bin (decompressed)
# Address: 0x800A48C8

glabel audio_start
    /* 800A48C8 3C0E8013 */  lui $t6, 0x8013
    /* 800A48CC 8DCEE6C0 */  lw $t6, -6464($t6)
    /* 800A48D0 3C058015 */  lui $a1, 0x8015
    /* 800A48D4 24A59438 */  addiu $a1, $a1, -27592
    /* 800A48D8 000E7880 */  sll $t7, $t6, 2
    /* 800A48DC 01EE7821 */  addu $t7, $t7, $t6
    /* 800A48E0 000F7880 */  sll $t7, $t7, 2
    /* 800A48E4 01EE7823 */  subu $t7, $t7, $t6
    /* 800A48E8 8CA20000 */  lw $v0, 0($a1)
    /* 800A48EC 000F7880 */  sll $t7, $t7, 2
    /* 800A48F0 3C188012 */  lui $t8, 0x8012
    /* 800A48F4 01EE7823 */  subu $t7, $t7, $t6
    /* 800A48F8 000F7A00 */  sll $t7, $t7, 8
    /* 800A48FC 27184FE8 */  addiu $t8, $t8, 20456
    /* 800A4900 14400003 */  bne $v0, $zero, 0x800A4910
    /* 800A4904 01F81821 */  addu $v1, $t7, $t8
    /* 800A4908 03E00008 */  jr $ra
    /* 800A490C 00001025 */  or $v0, $zero, $zero
