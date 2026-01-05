# Source: game_code.bin (decompressed)
# Address: 0x800D54E0

glabel player_conditional_check
    /* 800D54E0 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800D54E4 AFBF0014 */  sw $ra, 20($sp)
    /* 800D54E8 10A00005 */  beq $a1, $zero, 0x800D5500
    /* 800D54EC AFA40018 */  sw $a0, 24($sp)
    /* 800D54F0 0C02FC09 */  jal 0x800BF024
    /* 800D54F4 8C840000 */  lw $a0, 0($a0)
    /* 800D54F8 10000004 */  beq $zero, $zero, 0x800D550C
    /* 800D54FC 00000000 */  nop
    /* 800D5500 8FAF0018 */  lw $t7, 24($sp)
    /* 800D5504 0C024701 */  jal 0x80091C04
    /* 800D5508 8DE40000 */  lw $a0, 0($t7)
    /* 800D550C 0C03552F */  jal 0x800D54BC
    /* 800D5510 8FA40018 */  lw $a0, 24($sp)
    /* 800D5514 8FBF0014 */  lw $ra, 20($sp)
    /* 800D5518 27BD0018 */  addiu $sp, $sp, 24
    /* 800D551C 03E00008 */  jr $ra
    /* 800D5520 00000000 */  nop
