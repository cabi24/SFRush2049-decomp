# Source: game_code.bin (decompressed)
# Address: 0x8008A650

glabel texture_load_start
    /* 8008A650 27BDFFE8 */  addiu $sp, $sp, -24
    /* 8008A654 308EFFFF */  andi $t6, $a0, 0xFFFF
    /* 8008A658 AFBF0014 */  sw $ra, 20($sp)
    /* 8008A65C 11CF000B */  beq $t6, $t7, 0x8008A68C
    /* 8008A660 AFA40018 */  sw $a0, 24($sp)
    /* 8008A664 3C058015 */  lui $a1, 0x8015
    /* 8008A668 24A59438 */  addiu $a1, $a1, -27592
    /* 8008A66C 8CA30000 */  lw $v1, 0($a1)
    /* 8008A670 3C19EE00 */  lui $t9, 0xEE00
    /* 8008A674 000E4C00 */  sll $t1, $t6, 16
    /* 8008A678 24780008 */  addiu $t8, $v1, 8
    /* 8008A67C ACB80000 */  sw $t8, 0($a1)
    /* 8008A680 AC690004 */  sw $t1, 4($v1)
    /* 8008A684 AC790000 */  sw $t9, 0($v1)
    /* 8008A688 A4CE0000 */  sh $t6, 0($a2)
    /* 8008A68C 0C021E38 */  jal 0x800878E0
    /* 8008A690 24040010 */  addiu $a0, $zero, 16
    /* 8008A694 8FBF0014 */  lw $ra, 20($sp)
    /* 8008A698 27BD0018 */  addiu $sp, $sp, 24
    /* 8008A69C 03E00008 */  jr $ra
    /* 8008A6A0 00000000 */  nop
