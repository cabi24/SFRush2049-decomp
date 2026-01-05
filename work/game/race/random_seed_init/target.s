# Source: game_code.bin (decompressed)
# Address: 0x800FD7E8

glabel random_seed_init
    /* 800FD7E8 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800FD7EC AFBF0014 */  sw $ra, 20($sp)
    /* 800FD7F0 10A0000E */  beq $a1, $zero, 0x800FD82C
    /* 800FD7F4 00803825 */  or $a3, $a0, $zero
    /* 800FD7F8 00047080 */  sll $t6, $a0, 2
    /* 800FD7FC 01C47021 */  addu $t6, $t6, $a0
    /* 800FD800 000E7080 */  sll $t6, $t6, 2
    /* 800FD804 01C47023 */  subu $t6, $t6, $a0
    /* 800FD808 3C0F8015 */  lui $t7, 0x8015
    /* 800FD80C 3C188015 */  lui $t8, 0x8015
    /* 800FD810 27186998 */  addiu $t8, $t8, 27032
    /* 800FD814 25EFA118 */  addiu $t7, $t7, -24296
    /* 800FD818 000E7080 */  sll $t6, $t6, 2
    /* 800FD81C 00041880 */  sll $v1, $a0, 2
    /* 800FD820 00784021 */  addu $t0, $v1, $t8
    /* 800FD824 10000013 */  beq $zero, $zero, 0x800FD874
    /* 800FD828 01CF1021 */  addu $v0, $t6, $t7
    /* 800FD82C 0007C880 */  sll $t9, $a3, 2
    /* 800FD830 0327C821 */  addu $t9, $t9, $a3
    /* 800FD834 0019C880 */  sll $t9, $t9, 2
    /* 800FD838 0327C823 */  subu $t9, $t9, $a3
    /* 800FD83C 3C098015 */  lui $t1, 0x8015
    /* 800FD840 2529A118 */  addiu $t1, $t1, -24296
    /* 800FD844 0019C880 */  sll $t9, $t9, 2
    /* 800FD848 03291021 */  addu $v0, $t9, $t1
    /* 800FD84C 904A0001 */  lbu $t2, 1($v0)
    /* 800FD850 24010005 */  addiu $at, $zero, 5
    /* 800FD854 00071880 */  sll $v1, $a3, 2
    /* 800FD858 1541001C */  bne $t2, $at, 0x800FD8CC
    /* 800FD85C 3C0B8015 */  lui $t3, 0x8015
    /* 800FD860 256B6998 */  addiu $t3, $t3, 27032
    /* 800FD864 006B4021 */  addu $t0, $v1, $t3
    /* 800FD868 8D0C0000 */  lw $t4, 0($t0)
    /* 800FD86C 318D0001 */  andi $t5, $t4, 0x0001
    /* 800FD870 11A00016 */  beq $t5, $zero, 0x800FD8CC
    /* 800FD874 3C0E8015 */  lui $t6, 0x8015
    /* 800FD878 A0470001 */  sb $a3, 1($v0)
    /* 800FD87C 01C37021 */  addu $t6, $t6, $v1
    /* 800FD880 8DCE6978 */  lw $t6, 27000($t6)
    /* 800FD884 3C188014 */  lui $t8, 0x8014
    /* 800FD888 0303C021 */  addu $t8, $t8, $v1
    /* 800FD88C AC4E0008 */  sw $t6, 8($v0)
    /* 800FD890 8D0F0000 */  lw $t7, 0($t0)
    /* 800FD894 3C098015 */  lui $t1, 0x8015
    /* 800FD898 25296958 */  addiu $t1, $t1, 26968
    /* 800FD89C AC4F0004 */  sw $t7, 4($v0)
    /* 800FD8A0 8F183A00 */  lw $t8, 14848($t8)
    /* 800FD8A4 0007C8C0 */  sll $t9, $a3, 3
    /* 800FD8A8 03293021 */  addu $a2, $t9, $t1
    /* 800FD8AC AC58000C */  sw $t8, 12($v0)
    /* 800FD8B0 C4C40000 */  lwc1 $f4, 0($a2)
    /* 800FD8B4 30E400FF */  andi $a0, $a3, 0x00FF
    /* 800FD8B8 24050001 */  addiu $a1, $zero, 1
    /* 800FD8BC E4440010 */  swc1 $f4, 16($v0)
    /* 800FD8C0 C4C60004 */  lwc1 $f6, 4($a2)
    /* 800FD8C4 0C032456 */  jal 0x800C9158
    /* 800FD8C8 E4460014 */  swc1 $f6, 20($v0)
    /* 800FD8CC 8FBF0014 */  lw $ra, 20($sp)
    /* 800FD8D0 27BD0018 */  addiu $sp, $sp, 24
    /* 800FD8D4 03E00008 */  jr $ra
    /* 800FD8D8 00000000 */  nop
