# Source: game_code.bin (decompressed)
# Address: 0x800BA800

glabel tire_suspension_update
    /* 800BA800 18400067 */  blez $v0, 0x800BA9A0
    /* 800BA804 0000A825 */  or $s5, $zero, $zero
    /* 800BA808 3C0E8015 */  lui $t6, 0x8015
    /* 800BA80C 25D71CE8 */  addiu $s7, $t6, 7400
    /* 800BA810 3C138014 */  lui $s3, 0x8014
    /* 800BA814 267307F0 */  addiu $s3, $s3, 2032
    /* 800BA818 02E0B025 */  or $s6, $s7, $zero
    /* 800BA81C 24140014 */  addiu $s4, $zero, 20
    /* 800BA820 C6C4000C */  lwc1 $f4, 12($s6)
    /* 800BA824 C6C80010 */  lwc1 $f8, 16($s6)
    /* 800BA828 87B90052 */  lh $t9, 82($sp)
    /* 800BA82C 4600218D */  .word 0x4600218D
    /* 800BA830 C6C40014 */  lwc1 $f4, 20($s6)
    /* 800BA834 00008025 */  or $s0, $zero, $zero
    /* 800BA838 4600428D */  .word 0x4600428D
    /* 800BA83C 44183000 */  .word 0x44183000
    /* 800BA840 00159400 */  sll $s2, $s5, 16
    /* 800BA844 4600218D */  .word 0x4600218D
    /* 800BA848 A7B80040 */  sh $t8, 64($sp)
    /* 800BA84C 440E5000 */  .word 0x440E5000
    /* 800BA850 02E08825 */  or $s1, $s7, $zero
    /* 800BA854 44183000 */  .word 0x44183000
    /* 800BA858 A7AE0042 */  sh $t6, 66($sp)
    /* 800BA85C 07210022 */  bgez $t9, 0x800BA8E8
    /* 800BA860 A7B80044 */  sh $t8, 68($sp)
    /* 800BA864 00127403 */  sra $t6, $s2, 16
    /* 800BA868 01C09025 */  or $s2, $t6, $zero
    /* 800BA86C 16000007 */  bne $s0, $zero, 0x800BA88C
    /* 800BA870 2A010005 */  slti $at, $s0, 5
    /* 800BA874 00122400 */  sll $a0, $s2, 16
    /* 800BA878 00047C03 */  sra $t7, $a0, 16
    /* 800BA87C 0C02E987 */  jal 0x800BA61C
    /* 800BA880 01E02025 */  or $a0, $t7, $zero
    /* 800BA884 10000013 */  beq $zero, $zero, 0x800BA8D4
    /* 800BA888 A622002E */  sh $v0, 46($s1)
    /* 800BA88C 1420000A */  bne $at, $zero, 0x800BA8B8
    /* 800BA890 00122400 */  sll $a0, $s2, 16
    /* 800BA894 00122C00 */  sll $a1, $s2, 16
    /* 800BA898 2604FFFB */  addiu $a0, $s0, -5
    /* 800BA89C 0004CC00 */  sll $t9, $a0, 16
    /* 800BA8A0 0005C403 */  sra $t8, $a1, 16
    /* 800BA8A4 03002825 */  or $a1, $t8, $zero
    /* 800BA8A8 0C02E91B */  jal 0x800BA46C
    /* 800BA8AC 00192403 */  sra $a0, $t9, 16
    /* 800BA8B0 10000008 */  beq $zero, $zero, 0x800BA8D4
    /* 800BA8B4 A622002E */  sh $v0, 46($s1)
    /* 800BA8B8 2605FFFF */  addiu $a1, $s0, -1
    /* 800BA8BC 0005C400 */  sll $t8, $a1, 16
    /* 800BA8C0 00047C03 */  sra $t7, $a0, 16
    /* 800BA8C4 01E02025 */  or $a0, $t7, $zero
    /* 800BA8C8 0C02E8AE */  jal 0x800BA2B8
    /* 800BA8CC 00182C03 */  sra $a1, $t8, 16
    /* 800BA8D0 A622002E */  sh $v0, 46($s1)
    /* 800BA8D4 26100001 */  addiu $s0, $s0, 1
    /* 800BA8D8 1614FFE4 */  bne $s0, $s4, 0x800BA86C
    /* 800BA8DC 26310002 */  addiu $s1, $s1, 2
    /* 800BA8E0 1000002A */  beq $zero, $zero, 0x800BA98C
    /* 800BA8E4 87C20008 */  lh $v0, 8($fp)
    /* 800BA8E8 87AE0052 */  lh $t6, 82($sp)
    /* 800BA8EC 00152400 */  sll $a0, $s5, 16
    /* 800BA8F0 87A50052 */  lh $a1, 82($sp)
    /* 800BA8F4 000E7840 */  sll $t7, $t6, 1
    /* 800BA8F8 15C0001C */  bne $t6, $zero, 0x800BA96C
    /* 800BA8FC 02EF8821 */  addu $s1, $s7, $t7
    /* 800BA900 00159400 */  sll $s2, $s5, 16
    /* 800BA904 0012C403 */  sra $t8, $s2, 16
    /* 800BA908 00182400 */  sll $a0, $t8, 16
    /* 800BA90C 0004CC03 */  sra $t9, $a0, 16
    /* 800BA910 03202025 */  or $a0, $t9, $zero
    /* 800BA914 03009025 */  or $s2, $t8, $zero
    /* 800BA918 0C02E987 */  jal 0x800BA61C
    /* 800BA91C 00008025 */  or $s0, $zero, $zero
    /* 800BA920 926F0008 */  lbu $t7, 8($s3)
    /* 800BA924 A622002E */  sh $v0, 46($s1)
    /* 800BA928 02E08825 */  or $s1, $s7, $zero
    /* 800BA92C 19E00016 */  blez $t7, 0x800BA988
    /* 800BA930 00122C00 */  sll $a1, $s2, 16
    /* 800BA934 00102400 */  sll $a0, $s0, 16
    /* 800BA938 0004C403 */  sra $t8, $a0, 16
    /* 800BA93C 00057403 */  sra $t6, $a1, 16
    /* 800BA940 01C02825 */  or $a1, $t6, $zero
    /* 800BA944 0C02E91B */  jal 0x800BA46C
    /* 800BA948 03002025 */  or $a0, $t8, $zero
    /* 800BA94C 92790008 */  lbu $t9, 8($s3)
    /* 800BA950 26100001 */  addiu $s0, $s0, 1
    /* 800BA954 26310002 */  addiu $s1, $s1, 2
    /* 800BA958 0219082A */  slt $at, $s0, $t9
    /* 800BA95C 1420FFF4 */  bne $at, $zero, 0x800BA930
    /* 800BA960 A6220036 */  sh $v0, 54($s1)
    /* 800BA964 10000009 */  beq $zero, $zero, 0x800BA98C
    /* 800BA968 87C20008 */  lh $v0, 8($fp)
    /* 800BA96C 24A5FFFF */  addiu $a1, $a1, -1
    /* 800BA970 00057400 */  sll $t6, $a1, 16
    /* 800BA974 00047C03 */  sra $t7, $a0, 16
    /* 800BA978 01E02025 */  or $a0, $t7, $zero
    /* 800BA97C 0C02E8AE */  jal 0x800BA2B8
    /* 800BA980 000E2C03 */  sra $a1, $t6, 16
    /* 800BA984 A622002E */  sh $v0, 46($s1)
    /* 800BA988 87C20008 */  lh $v0, 8($fp)
    /* 800BA98C 26B50001 */  addiu $s5, $s5, 1
    /* 800BA990 26D60050 */  addiu $s6, $s6, 80
    /* 800BA994 02A2082A */  slt $at, $s5, $v0
    /* 800BA998 1420FFA1 */  bne $at, $zero, 0x800BA820
    /* 800BA99C 26F70050 */  addiu $s7, $s7, 80
    /* 800BA9A0 44804000 */  .word 0x44804000
    /* 800BA9A4 3C138015 */  lui $s3, 0x8015
    /* 800BA9A8 26732800 */  addiu $s3, $s3, 10240
    /* 800BA9AC E6680000 */  swc1 $f8, 0($s3)
    /* 800BA9B0 C66A0000 */  lwc1 $f10, 0($s3)
    /* 800BA9B4 3C148015 */  lui $s4, 0x8015
    /* 800BA9B8 269443AC */  addiu $s4, $s4, 17324
    /* 800BA9BC 00009025 */  or $s2, $zero, $zero
    /* 800BA9C0 00008025 */  or $s0, $zero, $zero
    /* 800BA9C4 1840002A */  blez $v0, 0x800BAA70
    /* 800BA9C8 E68A0000 */  swc1 $f10, 0($s4)
    /* 800BA9CC 3C118015 */  lui $s1, 0x8015
    /* 800BA9D0 26311CE8 */  addiu $s1, $s1, 7400
    /* 800BA9D4 24150050 */  addiu $s5, $zero, 80
    /* 800BA9D8 26190001 */  addiu $t9, $s0, 1
    /* 800BA9DC 57220004 */  .word 0x57220004
    /* 800BA9E0 26020001 */  addiu $v0, $s0, 1
    /* 800BA9E4 10000002 */  beq $zero, $zero, 0x800BA9F0
    /* 800BA9E8 87C20002 */  lh $v0, 2($fp)
    /* 800BA9EC 26020001 */  addiu $v0, $s0, 1
    /* 800BA9F0 00550019 */  multu $v0, $s5
    /* 800BA9F4 8624002E */  lh $a0, 46($s1)
    /* 800BA9F8 00007812 */  mflo $t7
    /* 800BA9FC 03CF7021 */  addu $t6, $fp, $t7
    /* 800BAA00 0C02E803 */  jal 0x800BA00C
    /* 800BAA04 85C5002E */  lh $a1, 46($t6)
    /* 800BAA08 1A000005 */  blez $s0, 0x800BAA20
    /* 800BAA0C E6200058 */  swc1 $f0, 88($s1)
    /* 800BAA10 87D80004 */  lh $t8, 4($fp)
    /* 800BAA14 16180002 */  bne $s0, $t8, 0x800BAA20
    /* 800BAA18 00000000 */  nop
    /* 800BAA1C 24120001 */  addiu $s2, $zero, 1
    /* 800BAA20 56400006 */  .word 0x56400006
    /* 800BAA24 87D90002 */  lh $t9, 2($fp)
    /* 800BAA28 C6640000 */  lwc1 $f4, 0($s3)
    /* 800BAA2C C6260058 */  lwc1 $f6, 88($s1)
    /* 800BAA30 46062200 */  .word 0x46062200
    /* 800BAA34 E6680000 */  swc1 $f8, 0($s3)
    /* 800BAA38 87D90002 */  lh $t9, 2($fp)
    /* 800BAA3C 0219082A */  slt $at, $s0, $t9
    /* 800BAA40 54200006 */  .word 0x54200006
    /* 800BAA44 87C20008 */  lh $v0, 8($fp)
    /* 800BAA48 C68A0000 */  lwc1 $f10, 0($s4)
    /* 800BAA4C C6240058 */  lwc1 $f4, 88($s1)
    /* 800BAA50 46045180 */  .word 0x46045180
    /* 800BAA54 E6860000 */  swc1 $f6, 0($s4)
    /* 800BAA58 87C20008 */  lh $v0, 8($fp)
    /* 800BAA5C 26100001 */  addiu $s0, $s0, 1
    /* 800BAA60 26310050 */  addiu $s1, $s1, 80
    /* 800BAA64 0202082A */  slt $at, $s0, $v0
    /* 800BAA68 5420FFDC */  .word 0x5420FFDC
    /* 800BAA6C 26190001 */  addiu $t9, $s0, 1
    /* 800BAA70 8FBF003C */  lw $ra, 60($sp)
    /* 800BAA74 8FB00018 */  lw $s0, 24($sp)
    /* 800BAA78 8FB1001C */  lw $s1, 28($sp)
    /* 800BAA7C 8FB20020 */  lw $s2, 32($sp)
    /* 800BAA80 8FB30024 */  lw $s3, 36($sp)
    /* 800BAA84 8FB40028 */  lw $s4, 40($sp)
    /* 800BAA88 8FB5002C */  lw $s5, 44($sp)
    /* 800BAA8C 8FB60030 */  lw $s6, 48($sp)
    /* 800BAA90 8FB70034 */  lw $s7, 52($sp)
    /* 800BAA94 8FBE0038 */  lw $fp, 56($sp)
    /* 800BAA98 03E00008 */  jr $ra
    /* 800BAA9C 27BD0050 */  addiu $sp, $sp, 80
