# Source: game_code.bin (decompressed)
# Address: 0x80096298

glabel slot_value_get
    /* 80096298 27BDFFE8 */  addiu $sp, $sp, -24
    /* 8009629C AFBF0014 */  sw $ra, 20($sp)
    /* 800962A0 00803825 */  or $a3, $a0, $zero
    /* 800962A4 00002825 */  or $a1, $zero, $zero
    /* 800962A8 0C0258A2 */  jal 0x80096288
    /* 800962AC 00003025 */  or $a2, $zero, $zero
    /* 800962B0 00077080 */  sll $t6, $a3, 2
    /* 800962B4 01C77021 */  addu $t6, $t6, $a3
    /* 800962B8 8FBF0014 */  lw $ra, 20($sp)
    /* 800962BC 000E7080 */  sll $t6, $t6, 2
    /* 800962C0 3C028015 */  lui $v0, 0x8015
    /* 800962C4 004E1021 */  addu $v0, $v0, $t6
    /* 800962C8 8C426D44 */  lw $v0, 27972($v0)
    /* 800962CC 03E00008 */  jr $ra
    /* 800962D0 27BD0018 */  addiu $sp, $sp, 24
