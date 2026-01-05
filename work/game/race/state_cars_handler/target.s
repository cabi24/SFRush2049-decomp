# Source: game_code.bin (decompressed)
# Address: 0x800FC89C

glabel state_cars_handler
    /* 800FC89C 05C10030 */  bgez $t6, 0x800FC960
    /* 800FC8A0 00027980 */  sll $t7, $v0, 6
    /* 800FC8A4 3C138015 */  lui $s3, 0x8015
    /* 800FC8A8 26732744 */  addiu $s3, $s3, 10052
    /* 800FC8AC 82790000 */  lb $t9, 0($s3)
    /* 800FC8B0 00008025 */  or $s0, $zero, $zero
    /* 800FC8B4 1B20001C */  blez $t9, 0x800FC928
    /* 800FC8B8 00000000 */  nop
    /* 800FC8BC 3C128015 */  lui $s2, 0x8015
    /* 800FC8C0 3C118015 */  lui $s1, 0x8015
    /* 800FC8C4 26312818 */  addiu $s1, $s1, 10264
    /* 800FC8C8 2652A250 */  addiu $s2, $s2, -23984
    /* 800FC8CC 2416FFF7 */  addiu $s6, $zero, -9
    /* 800FC8D0 241503B8 */  addiu $s5, $zero, 952
    /* 800FC8D4 24140808 */  addiu $s4, $zero, 2056
    /* 800FC8D8 02140019 */  multu $s0, $s4
    /* 800FC8DC 0000C012 */  mflo $t8
    /* 800FC8E0 02587821 */  addu $t7, $s2, $t8
    /* 800FC8E4 0C03560A */  jal 0x800D5828
    /* 800FC8E8 85E407C6 */  lh $a0, 1990($t7)
    /* 800FC8EC 02150019 */  multu $s0, $s5
    /* 800FC8F0 00007012 */  mflo $t6
    /* 800FC8F4 022E1021 */  addu $v0, $s1, $t6
    /* 800FC8F8 8C5900E8 */  lw $t9, 232($v0)
    /* 800FC8FC 8044035B */  lb $a0, 859($v0)
    /* 800FC900 0336C024 */  and $t8, $t9, $s6
    /* 800FC904 0C02C060 */  jal 0x800B0180
    /* 800FC908 AC5800E8 */  sw $t8, 232($v0)
    /* 800FC90C 82790000 */  lb $t9, 0($s3)
    /* 800FC910 26100001 */  addiu $s0, $s0, 1
    /* 800FC914 00107C00 */  sll $t7, $s0, 16
    /* 800FC918 000F8403 */  sra $s0, $t7, 16
    /* 800FC91C 0219082A */  slt $at, $s0, $t9
    /* 800FC920 1420FFED */  bne $at, $zero, 0x800FC8D8
    /* 800FC924 00000000 */  nop
    /* 800FC928 0C0355E6 */  jal 0x800D5798
    /* 800FC92C 00000000 */  nop
    /* 800FC930 0C0354DD */  jal 0x800D5374
    /* 800FC934 00000000 */  nop
    /* 800FC938 3C014080 */  lui $at, 0x4080
    /* 800FC93C 44816000 */  .word 0x44816000
    /* 800FC940 0C00053C */  jal 0x800014F0
    /* 800FC944 00000000 */  nop
    /* 800FC948 3C180200 */  lui $t8, 0x0200
    /* 800FC94C 3C018011 */  lui $at, 0x8011
    /* 800FC950 3C028011 */  lui $v0, 0x8011
    /* 800FC954 AC3874B8 */  sw $t8, 29880($at)
    /* 800FC958 1000000E */  beq $zero, $zero, 0x800FC994
    /* 800FC95C 8C4274B4 */  lw $v0, 29876($v0)
    /* 800FC960 05E10006 */  bgez $t7, 0x800FC97C
    /* 800FC964 00027200 */  sll $t6, $v0, 8
    /* 800FC968 0C03D935 */  jal 0x800F64D4
    /* 800FC96C 00000000 */  nop
    /* 800FC970 3C028011 */  lui $v0, 0x8011
    /* 800FC974 10000007 */  beq $zero, $zero, 0x800FC994
    /* 800FC978 8C4274B4 */  lw $v0, 29876($v0)
    /* 800FC97C 05C30006 */  .word 0x05C30006
    /* 800FC980 3C010060 */  lui $at, 0x0060
    /* 800FC984 0C03E2DC */  jal 0x800F8B70
    /* 800FC988 00000000 */  nop
    /* 800FC98C 3C028011 */  lui $v0, 0x8011
    /* 800FC990 8C4274B4 */  lw $v0, 29876($v0)
    /* 800FC994 3C010060 */  lui $at, 0x0060
    /* 800FC998 0041C824 */  and $t9, $v0, $at
    /* 800FC99C 53200013 */  .word 0x53200013
    /* 800FC9A0 8FBF0014 */  lw $ra, 20($sp)
    /* 800FC9A4 3C128015 */  lui $s2, 0x8015
    /* 800FC9A8 26521AD0 */  addiu $s2, $s2, 6864
    /* 800FC9AC 86580000 */  lh $t8, 0($s2)
    /* 800FC9B0 00008025 */  or $s0, $zero, $zero
    /* 800FC9B4 5B00000D */  .word 0x5B00000D
    /* 800FC9B8 8FBF0014 */  lw $ra, 20($sp)
    /* 800FC9BC 3C118015 */  lui $s1, 0x8015
    /* 800FC9C0 26310B94 */  addiu $s1, $s1, 2964
    /* 800FC9C4 02002025 */  or $a0, $s0, $zero
    /* 800FC9C8 0C02AC63 */  jal 0x800AB18C
    /* 800FC9CC 02202825 */  or $a1, $s1, $zero
    /* 800FC9D0 864F0000 */  lh $t7, 0($s2)
    /* 800FC9D4 26100001 */  addiu $s0, $s0, 1
    /* 800FC9D8 26310098 */  addiu $s1, $s1, 152
    /* 800FC9DC 020F082A */  slt $at, $s0, $t7
    /* 800FC9E0 5420FFF9 */  .word 0x5420FFF9
    /* 800FC9E4 02002025 */  or $a0, $s0, $zero
    /* 800FC9E8 8FBF0014 */  lw $ra, 20($sp)
    /* 800FC9EC 27BD0018 */  addiu $sp, $sp, 24
    /* 800FC9F0 03E00008 */  jr $ra
    /* 800FC9F4 00000000 */  nop
