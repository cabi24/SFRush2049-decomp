# Source: game_code.bin (decompressed)
# Address: 0x800AB7D8

glabel engine_torque_calc
    /* 800AB7D8 27BDFFA0 */  addiu $sp, $sp, -96
    /* 800AB7DC AFBF0014 */  sw $ra, 20($sp)
    /* 800AB7E0 862E0010 */  lh $t6, 16($s1)
    /* 800AB7E4 24140030 */  addiu $s4, $zero, 48
    /* 800AB7E8 3C138011 */  lui $s3, 0x8011
    /* 800AB7EC 01D40019 */  multu $t6, $s4
    /* 800AB7F0 3C188011 */  lui $t8, 0x8011
    /* 800AB7F4 8F1874B4 */  lw $t8, 29876($t8)
    /* 800AB7F8 26737530 */  addiu $s3, $s3, 30000
    /* 800AB7FC 0018CA40 */  sll $t9, $t8, 9
    /* 800AB800 00007812 */  mflo $t7
    /* 800AB804 026F9021 */  addu $s2, $s3, $t7
    /* 800AB808 82420016 */  lb $v0, 22($s2)
    /* 800AB80C 07200062 */  bltz $t9, 0x800AB998
    /* 800AB810 00403025 */  or $a2, $v0, $zero
    /* 800AB814 964E0012 */  lhu $t6, 18($s2)
    /* 800AB818 3C198015 */  lui $t9, 0x8015
    /* 800AB81C 27390E98 */  addiu $t9, $t9, 3736
    /* 800AB820 31CF0008 */  andi $t7, $t6, 0x0008
    /* 800AB824 11E00023 */  beq $t7, $zero, 0x800AB8B4
    /* 800AB828 0006C080 */  sll $t8, $a2, 2
    /* 800AB82C 922F0004 */  lbu $t7, 4($s1)
    /* 800AB830 03191821 */  addu $v1, $t8, $t9
    /* 800AB834 8C6E0000 */  lw $t6, 0($v1)
    /* 800AB838 31F80010 */  andi $t8, $t7, 0x0010
    /* 800AB83C 17000016 */  bne $t8, $zero, 0x800AB898
    /* 800AB840 AE2E006C */  sw $t6, 108($s1)
    /* 800AB844 24010004 */  addiu $at, $zero, 4
    /* 800AB848 14C10013 */  bne $a2, $at, 0x800AB898
    /* 800AB84C 3C048014 */  lui $a0, 0x8014
    /* 800AB850 82220065 */  lb $v0, 101($s1)
    /* 800AB854 248492D0 */  addiu $a0, $a0, -27952
    /* 800AB858 8C8E0000 */  lw $t6, 0($a0)
    /* 800AB85C 0002C8C0 */  sll $t9, $v0, 3
    /* 800AB860 8E38006C */  lw $t8, 108($s1)
    /* 800AB864 0322C821 */  addu $t9, $t9, $v0
    /* 800AB868 0019C880 */  sll $t9, $t9, 2
    /* 800AB86C 032E7821 */  addu $t7, $t9, $t6
    /* 800AB870 AF0F0000 */  sw $t7, 0($t8)
    /* 800AB874 8E39006C */  lw $t9, 108($s1)
    /* 800AB878 8F2E0000 */  lw $t6, 0($t9)
    /* 800AB87C 8DCF0010 */  lw $t7, 16($t6)
    /* 800AB880 31F80010 */  andi $t8, $t7, 0x0010
    /* 800AB884 13000004 */  beq $t8, $zero, 0x800AB898
    /* 800AB888 00000000 */  nop
    /* 800AB88C 92390004 */  lbu $t9, 4($s1)
    /* 800AB890 332EFFF7 */  andi $t6, $t9, 0xFFF7
    /* 800AB894 A22E0004 */  sb $t6, 4($s1)
    /* 800AB898 3C0F8011 */  lui $t7, 0x8011
    /* 800AB89C 01E67821 */  addu $t7, $t7, $a2
    /* 800AB8A0 91EF7510 */  lbu $t7, 29968($t7)
    /* 800AB8A4 8C780000 */  lw $t8, 0($v1)
    /* 800AB8A8 01F8C821 */  addu $t9, $t7, $t8
    /* 800AB8AC 1000000F */  beq $zero, $zero, 0x800AB8EC
    /* 800AB8B0 AC790000 */  sw $t9, 0($v1)
    /* 800AB8B4 24010007 */  addiu $at, $zero, 7
    /* 800AB8B8 10C1000C */  beq $a2, $at, 0x800AB8EC
    /* 800AB8BC 3C0E8011 */  lui $t6, 0x8011
    /* 800AB8C0 01C67021 */  addu $t6, $t6, $a2
    /* 800AB8C4 91CE7510 */  lbu $t6, 29968($t6)
    /* 800AB8C8 824F0017 */  lb $t7, 23($s2)
    /* 800AB8CC 0006C880 */  sll $t9, $a2, 2
    /* 800AB8D0 01CF0019 */  multu $t6, $t7
    /* 800AB8D4 3C0E8012 */  lui $t6, 0x8012
    /* 800AB8D8 01D97021 */  addu $t6, $t6, $t9
    /* 800AB8DC 8DCE8DDC */  lw $t6, -29220($t6)
    /* 800AB8E0 0000C012 */  mflo $t8
    /* 800AB8E4 030E7821 */  addu $t7, $t8, $t6
    /* 800AB8E8 AE2F006C */  sw $t7, 108($s1)
    /* 800AB8EC 86590010 */  lh $t9, 16($s2)
    /* 800AB8F0 24010004 */  addiu $at, $zero, 4
    /* 800AB8F4 17210026 */  bne $t9, $at, 0x800AB990
    /* 800AB8F8 3C028015 */  lui $v0, 0x8015
    /* 800AB8FC 24420F80 */  addiu $v0, $v0, 3968
    /* 800AB900 8C580000 */  lw $t8, 0($v0)
    /* 800AB904 3C048011 */  lui $a0, 0x8011
    /* 800AB908 2484418C */  addiu $a0, $a0, 16780
    /* 800AB90C A2380065 */  sb $t8, 101($s1)
    /* 800AB910 8C4E0000 */  lw $t6, 0($v0)
    /* 800AB914 25CF0001 */  addiu $t7, $t6, 1
    /* 800AB918 AC4F0000 */  sw $t7, 0($v0)
    /* 800AB91C 82390065 */  lb $t9, 101($s1)
    /* 800AB920 3C0E8015 */  lui $t6, 0x8015
    /* 800AB924 8DCE0F38 */  lw $t6, 3896($t6)
    /* 800AB928 0019C180 */  sll $t8, $t9, 6
    /* 800AB92C 030E8021 */  addu $s0, $t8, $t6
    /* 800AB930 0C0235AC */  jal 0x8008D6B0
    /* 800AB934 26050004 */  addiu $a1, $s0, 4
    /* 800AB938 4480A000 */  .word 0x4480A000
    /* 800AB93C 00001825 */  or $v1, $zero, $zero
    /* 800AB940 02001025 */  or $v0, $s0, $zero
    /* 800AB944 24630001 */  addiu $v1, $v1, 1
    /* 800AB948 28610003 */  slti $at, $v1, 3
    /* 800AB94C 24420004 */  addiu $v0, $v0, 4
    /* 800AB950 E4540030 */  swc1 $f20, 48($v0)
    /* 800AB954 1420FFFB */  bne $at, $zero, 0x800AB944
    /* 800AB958 E4540024 */  swc1 $f20, 36($v0)
    /* 800AB95C 862F0010 */  lh $t7, 16($s1)
    /* 800AB960 01F40019 */  multu $t7, $s4
    /* 800AB964 0000C812 */  mflo $t9
    /* 800AB968 0279C021 */  addu $t8, $s3, $t9
    /* 800AB96C C7040018 */  lwc1 $f4, 24($t8)
    /* 800AB970 3C198012 */  lui $t9, 0x8012
    /* 800AB974 27398C10 */  addiu $t9, $t9, -29680
    /* 800AB978 4600218D */  .word 0x4600218D
    /* 800AB97C 44033000 */  .word 0x44033000
    /* 800AB980 00000000 */  nop
    /* 800AB984 00037940 */  sll $t7, $v1, 5
    /* 800AB988 01F9C021 */  addu $t8, $t7, $t9
    /* 800AB98C AE180000 */  sw $t8, 0($s0)
    /* 800AB990 10000029 */  beq $zero, $zero, 0x800ABA38
    /* 800AB994 86250058 */  lh $a1, 88($s1)
    /* 800AB998 964E0012 */  lhu $t6, 18($s2)
    /* 800AB99C 24010005 */  addiu $at, $zero, 5
    /* 800AB9A0 31CF0008 */  andi $t7, $t6, 0x0008
    /* 800AB9A4 51E00024 */  .word 0x51E00024
    /* 800AB9A8 86250058 */  lh $a1, 88($s1)
    /* 800AB9AC 14410021 */  bne $v0, $at, 0x800ABA34
    /* 800AB9B0 00001825 */  or $v1, $zero, $zero
    /* 800AB9B4 3C028015 */  lui $v0, 0x8015
    /* 800AB9B8 8C420E7C */  lw $v0, 3708($v0)
    /* 800AB9BC 24050010 */  addiu $a1, $zero, 16
    /* 800AB9C0 2404FFFF */  addiu $a0, $zero, -1
    /* 800AB9C4 84590086 */  lh $t9, 134($v0)
    /* 800AB9C8 5720000B */  .word 0x5720000B
    /* 800AB9CC 24630001 */  addiu $v1, $v1, 1
    /* 800AB9D0 8C580000 */  lw $t8, 0($v0)
    /* 800AB9D4 54980008 */  .word 0x54980008
    /* 800AB9D8 24630001 */  addiu $v1, $v1, 1
    /* 800AB9DC 8C4E0004 */  lw $t6, 4($v0)
    /* 800AB9E0 548E0005 */  .word 0x548E0005
    /* 800AB9E4 24630001 */  addiu $v1, $v1, 1
    /* 800AB9E8 8C4F0008 */  lw $t7, 8($v0)
    /* 800AB9EC 508F0005 */  .word 0x508F0005
    /* 800AB9F0 24190001 */  addiu $t9, $zero, 1
    /* 800AB9F4 24630001 */  addiu $v1, $v1, 1
    /* 800AB9F8 1465FFF2 */  bne $v1, $a1, 0x800AB9C4
    /* 800AB9FC 24420088 */  addiu $v0, $v0, 136
    /* 800ABA00 24190001 */  addiu $t9, $zero, 1
    /* 800ABA04 3C188011 */  lui $t8, 0x8011
    /* 800ABA08 A4590086 */  sh $t9, 134($v0)
    /* 800ABA0C 0306C021 */  addu $t8, $t8, $a2
    /* 800ABA10 93187510 */  lbu $t8, 29968($t8)
    /* 800ABA14 3C198015 */  lui $t9, 0x8015
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
