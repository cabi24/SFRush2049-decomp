# Source: game_code.bin (decompressed)
# Address: 0x800ABA18

glabel clutch_slip_calc
    /* 800ABA18 00067880 */  sll $t7, $a2, 2
    /* 800ABA1C 03030019 */  multu $t8, $v1
    /* 800ABA20 032FC821 */  addu $t9, $t9, $t7
    /* 800ABA24 8F390E98 */  lw $t9, 3736($t9)
    /* 800ABA28 00007012 */  mflo $t6
    /* 800ABA2C 01D9C021 */  addu $t8, $t6, $t9
    /* 800ABA30 AE38006C */  sw $t8, 108($s1)
    /* 800ABA34 86250058 */  lh $a1, 88($s1)
    /* 800ABA38 2404FFFF */  addiu $a0, $zero, -1
    /* 800ABA3C 4480A000 */  .word 0x4480A000
    /* 800ABA40 1085000B */  beq $a0, $a1, 0x800ABA70
    /* 800ABA44 00057840 */  sll $t7, $a1, 1
    /* 800ABA48 8622000E */  lh $v0, 14($s1)
    /* 800ABA4C 3C038014 */  lui $v1, 0x8014
    /* 800ABA50 006F1821 */  addu $v1, $v1, $t7
    /* 800ABA54 00027100 */  sll $t6, $v0, 4
    /* 800ABA58 946327C0 */  lhu $v1, 10176($v1)
    /* 800ABA5C 01C27021 */  addu $t6, $t6, $v0
    /* 800ABA60 000E7080 */  sll $t6, $t6, 2
    /* 800ABA64 3C018013 */  lui $at, 0x8013
    /* 800ABA68 002E0821 */  addu $at, $at, $t6
    /* 800ABA6C A423E714 */  sh $v1, -6380($at)
    /* 800ABA70 96590012 */  lhu $t9, 18($s2)
    /* 800ABA74 33380002 */  andi $t8, $t9, 0x0002
    /* 800ABA78 53000019 */  .word 0x53000019
    /* 800ABA7C 964F0012 */  lhu $t7, 18($s2)
    /* 800ABA80 0C0240A1 */  jal 0x80090284
    /* 800ABA84 00000000 */  nop
    /* 800ABA88 50400015 */  .word 0x50400015
    /* 800ABA8C 964F0012 */  lhu $t7, 18($s2)
    /* 800ABA90 A4400004 */  sh $zero, 4($v0)
    /* 800ABA94 8E4F000C */  lw $t7, 12($s2)
    /* 800ABA98 AC51000C */  sw $s1, 12($v0)
    /* 800ABA9C E4540010 */  swc1 $f20, 16($v0)
    /* 800ABAA0 3C0E8014 */  lui $t6, 0x8014
    /* 800ABAA4 AC4F0014 */  sw $t7, 20($v0)
    /* 800ABAA8 8DCE91F0 */  lw $t6, -28176($t6)
    /* 800ABAAC 3C018014 */  lui $at, 0x8014
    /* 800ABAB0 AC4E0000 */  sw $t6, 0($v0)
    /* 800ABAB4 AC2291F0 */  sw $v0, -28176($at)
    /* 800ABAB8 96590012 */  lhu $t9, 18($s2)
    /* 800ABABC 33380004 */  andi $t8, $t9, 0x0004
    /* 800ABAC0 53000007 */  .word 0x53000007
    /* 800ABAC4 964F0012 */  lhu $t7, 18($s2)
    /* 800ABAC8 8E420008 */  lw $v0, 8($s2)
    /* 800ABACC 50400004 */  .word 0x50400004
    /* 800ABAD0 964F0012 */  lhu $t7, 18($s2)
    /* 800ABAD4 0040F809 */  jalr $ra, $v0
    /* 800ABAD8 02202025 */  or $a0, $s1, $zero
    /* 800ABADC 964F0012 */  lhu $t7, 18($s2)
    /* 800ABAE0 31EE4000 */  andi $t6, $t7, 0x4000
    /* 800ABAE4 51C00019 */  .word 0x51C00019
    /* 800ABAE8 8FBF0014 */  lw $ra, 20($sp)
    /* 800ABAEC 92390004 */  lbu $t9, 4($s1)
    /* 800ABAF0 33380010 */  andi $t8, $t9, 0x0010
    /* 800ABAF4 17000007 */  bne $t8, $zero, 0x800ABB14
    /* 800ABAF8 00000000 */  nop
    /* 800ABAFC 8E590008 */  lw $t9, 8($s2)
    /* 800ABB00 02202025 */  or $a0, $s1, $zero
    /* 800ABB04 0320F809 */  jalr $ra, $t9
    /* 800ABB08 00000000 */  nop
    /* 800ABB0C 1000000F */  beq $zero, $zero, 0x800ABB4C
    /* 800ABB10 8FBF0014 */  lw $ra, 20($sp)
    /* 800ABB14 3C0F8012 */  lui $t7, 0x8012
    /* 800ABB18 25EF8DFC */  addiu $t7, $t7, -29188
    /* 800ABB1C 8DE10000 */  lw $at, 0($t7)
    /* 800ABB20 27A50040 */  addiu $a1, $sp, 64
    /* 800ABB24 26260044 */  addiu $a2, $s1, 68
    /* 800ABB28 ACA10000 */  sw $at, 0($a1)
    /* 800ABB2C 8DF80004 */  lw $t8, 4($t7)
    /* 800ABB30 26270014 */  addiu $a3, $s1, 20
    /* 800ABB34 ACB80004 */  sw $t8, 4($a1)
    /* 800ABB38 8DE10008 */  lw $at, 8($t7)
    /* 800ABB3C ACA10008 */  sw $at, 8($a1)
    /* 800ABB40 0C02ADD4 */  jal 0x800AB750
    /* 800ABB44 82240065 */  lb $a0, 101($s1)
    /* 800ABB48 8FBF0014 */  lw $ra, 20($sp)
    /* 800ABB4C 27BD0060 */  addiu $sp, $sp, 96
    /* 800ABB50 03E00008 */  jr $ra
    /* 800ABB54 00000000 */  nop
