# Source: game_code.bin (decompressed)
# Address: 0x800B466C

glabel format_string_parse
    /* 800B466C 00801025 */  or $v0, $a0, $zero
    /* 800B4670 10A00029 */  beq $a1, $zero, 0x800B4718
    /* 800B4674 00001825 */  or $v1, $zero, $zero
    /* 800B4678 90440000 */  lbu $a0, 0($v0)
    /* 800B467C 308E0007 */  andi $t6, $a0, 0x0007
    /* 800B4680 2DC10006 */  sltiu $at, $t6, 6
    /* 800B4684 1020001F */  beq $at, $zero, 0x800B4704
    /* 800B4688 000E7080 */  sll $t6, $t6, 2
    /* 800B468C 3C018012 */  lui $at, 0x8012
    /* 800B4690 002E0821 */  addu $at, $at, $t6
    /* 800B4694 8C2E3C20 */  lw $t6, 15392($at)
    /* 800B4698 01C00008 */  jr $t6
    /* 800B469C 00000000 */  nop
    /* 800B46A0 00641823 */  subu $v1, $v1, $a0
    /* 800B46A4 10000019 */  beq $zero, $zero, 0x800B470C
    /* 800B46A8 24420001 */  addiu $v0, $v0, 1
    /* 800B46AC 00641825 */  or $v1, $v1, $a0
    /* 800B46B0 10000016 */  beq $zero, $zero, 0x800B470C
    /* 800B46B4 24420001 */  addiu $v0, $v0, 1
    /* 800B46B8 00641824 */  and $v1, $v1, $a0
    /* 800B46BC 10000013 */  beq $zero, $zero, 0x800B470C
    /* 800B46C0 24420001 */  addiu $v0, $v0, 1
    /* 800B46C4 00641826 */  xor $v1, $v1, $a0
    /* 800B46C8 10000010 */  beq $zero, $zero, 0x800B470C
    /* 800B46CC 24420001 */  addiu $v0, $v0, 1
    /* 800B46D0 00640019 */  multu $v1, $a0
    /* 800B46D4 24420001 */  addiu $v0, $v0, 1
    /* 800B46D8 00001812 */  mflo $v1
    /* 800B46DC 1000000C */  beq $zero, $zero, 0x800B4710
    /* 800B46E0 24A5FFFF */  addiu $a1, $a1, -1
    /* 800B46E4 0064001B */  divu $v1, $a0
    /* 800B46E8 00001812 */  mflo $v1
    /* 800B46EC 24420001 */  addiu $v0, $v0, 1
    /* 800B46F0 14800002 */  bne $a0, $zero, 0x800B46FC
    /* 800B46F4 00000000 */  nop
    /* 800B46F8 0007000D */  .word 0x0007000D
    /* 800B46FC 10000004 */  beq $zero, $zero, 0x800B4710
    /* 800B4700 24A5FFFF */  addiu $a1, $a1, -1
    /* 800B4704 00641821 */  addu $v1, $v1, $a0
    /* 800B4708 24420001 */  addiu $v0, $v0, 1
    /* 800B470C 24A5FFFF */  addiu $a1, $a1, -1
    /* 800B4710 54A0FFDA */  .word 0x54A0FFDA
    /* 800B4714 90440000 */  lbu $a0, 0($v0)
    /* 800B4718 03E00008 */  jr $ra
    /* 800B471C 00601025 */  or $v0, $v1, $zero
