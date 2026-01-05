# Source: game_code.bin (decompressed)
# Address: 0x8009638C

glabel display_list_alloc
    /* 8009638C 27BDFFE0 */  addiu $sp, $sp, -32
    /* 80096390 AFBF0014 */  sw $ra, 20($sp)
    /* 80096394 00803825 */  or $a3, $a0, $zero
    /* 80096398 00002825 */  or $a1, $zero, $zero
    /* 8009639C 0C0258A2 */  jal 0x80096288
    /* 800963A0 00003025 */  or $a2, $zero, $zero
    /* 800963A4 00077080 */  sll $t6, $a3, 2
    /* 800963A8 01C77021 */  addu $t6, $t6, $a3
    /* 800963AC 3C0F8015 */  lui $t7, 0x8015
    /* 800963B0 25EF6D38 */  addiu $t7, $t7, 27960
    /* 800963B4 000E7080 */  sll $t6, $t6, 2
    /* 800963B8 01CF1021 */  addu $v0, $t6, $t7
    /* 800963BC 8C44000C */  lw $a0, 12($v0)
    /* 800963C0 AFA20018 */  sw $v0, 24($sp)
    /* 800963C4 0C0258B5 */  jal 0x800962D4
    /* 800963C8 00002825 */  or $a1, $zero, $zero
    /* 800963CC 8FA20018 */  lw $v0, 24($sp)
    /* 800963D0 24180001 */  addiu $t8, $zero, 1
    /* 800963D4 A0580002 */  sb $t8, 2($v0)
    /* 800963D8 8FBF0014 */  lw $ra, 20($sp)
    /* 800963DC 27BD0020 */  addiu $sp, $sp, 32
    /* 800963E0 03E00008 */  jr $ra
    /* 800963E4 00000000 */  nop
