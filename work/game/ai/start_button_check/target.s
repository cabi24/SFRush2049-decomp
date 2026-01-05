# Source: game_code.bin (decompressed)
# Address: 0x800DCD94

glabel start_button_check
    /* 800DCD94 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800DCD98 AFBF0014 */  sw $ra, 20($sp)
    /* 800DCD9C 320E0C00 */  andi $t6, $s0, 0x0C00
    /* 800DCDA0 51C00008 */  .word 0x51C00008
    /* 800DCDA4 32180002 */  andi $t8, $s0, 0x0002
    /* 800DCDA8 0C02D7E2 */  jal 0x800B5F88
    /* 800DCDAC 02002025 */  or $a0, $s0, $zero
    /* 800DCDB0 82220001 */  lb $v0, 1($s1)
    /* 800DCDB4 8E300020 */  lw $s0, 32($s1)
    /* 800DCDB8 2C4F0001 */  sltiu $t7, $v0, 1
    /* 800DCDBC A22F0001 */  sb $t7, 1($s1)
    /* 800DCDC0 32180002 */  andi $t8, $s0, 0x0002
    /* 800DCDC4 13000007 */  beq $t8, $zero, 0x800DCDE4
    /* 800DCDC8 24040025 */  addiu $a0, $zero, 37
    /* 800DCDCC 00002825 */  or $a1, $zero, $zero
    /* 800DCDD0 24060001 */  addiu $a2, $zero, 1
    /* 800DCDD4 0C0248D8 */  jal 0x80092360
    /* 800DCDD8 00003825 */  or $a3, $zero, $zero
    /* 800DCDDC 24190001 */  addiu $t9, $zero, 1
    /* 800DCDE0 A2390000 */  sb $t9, 0($s1)
    /* 800DCDE4 8FBF0014 */  lw $ra, 20($sp)
    /* 800DCDE8 27BD0018 */  addiu $sp, $sp, 24
    /* 800DCDEC 03E00008 */  jr $ra
    /* 800DCDF0 00000000 */  nop
