# Source: game_code.bin (decompressed)
# Address: 0x800FCEB0

glabel state_update_loop
    /* 800FCEB0 0C03EA6D */  jal 0x800FA9B4
    /* 800FCEB4 00000000 */  nop
    /* 800FCEB8 10000051 */  beq $zero, $zero, 0x800FD000
    /* 800FCEBC 8FBF0014 */  lw $ra, 20($sp)
    /* 800FCEC0 3C138015 */  lui $s3, 0x8015
    /* 800FCEC4 3C018011 */  lui $at, 0x8011
    /* 800FCEC8 26732744 */  addiu $s3, $s3, 10052
    /* 800FCECC A020415C */  sb $zero, 16732($at)
    /* 800FCED0 826F0000 */  lb $t7, 0($s3)
    /* 800FCED4 3C118011 */  lui $s1, 0x8011
    /* 800FCED8 26314154 */  addiu $s1, $s1, 16724
    /* 800FCEDC A2200000 */  sb $zero, 0($s1)
    /* 800FCEE0 19E00011 */  blez $t7, 0x800FCF28
    /* 800FCEE4 00008025 */  or $s0, $zero, $zero
    /* 800FCEE8 3C118015 */  lui $s1, 0x8015
    /* 800FCEEC 2631A250 */  addiu $s1, $s1, -23984
    /* 800FCEF0 24120808 */  addiu $s2, $zero, 2056
    /* 800FCEF4 02120019 */  multu $s0, $s2
    /* 800FCEF8 0000C012 */  mflo $t8
    /* 800FCEFC 0238C821 */  addu $t9, $s1, $t8
    /* 800FCF00 0C03560A */  jal 0x800D5828
    /* 800FCF04 872407C6 */  lh $a0, 1990($t9)
    /* 800FCF08 82780000 */  lb $t8, 0($s3)
    /* 800FCF0C 26100001 */  addiu $s0, $s0, 1
    /* 800FCF10 00107400 */  sll $t6, $s0, 16
    /* 800FCF14 000E8403 */  sra $s0, $t6, 16
    /* 800FCF18 0218082A */  slt $at, $s0, $t8
    /* 800FCF1C 1420FFF5 */  bne $at, $zero, 0x800FCEF4
    /* 800FCF20 00000000 */  nop
    /* 800FCF24 00008025 */  or $s0, $zero, $zero
    /* 800FCF28 0C0355E6 */  jal 0x800D5798
    /* 800FCF2C 00000000 */  nop
    /* 800FCF30 0C0354DD */  jal 0x800D5374
    /* 800FCF34 00000000 */  nop
    /* 800FCF38 0C035633 */  jal 0x800D58CC
    /* 800FCF3C 00000000 */  nop
    /* 800FCF40 3C128014 */  lui $s2, 0x8014
    /* 800FCF44 265291F0 */  addiu $s2, $s2, -28176
    /* 800FCF48 8E510000 */  lw $s1, 0($s2)
    /* 800FCF4C 12200006 */  beq $s1, $zero, 0x800FCF68
    /* 800FCF50 02202025 */  or $a0, $s1, $zero
    /* 800FCF54 0C0241E7 */  jal 0x8009079C
    /* 800FCF58 24050001 */  addiu $a1, $zero, 1
    /* 800FCF5C 8E510000 */  lw $s1, 0($s2)
    /* 800FCF60 5620FFFC */  .word 0x5620FFFC
    /* 800FCF64 02202025 */  or $a0, $s1, $zero
    /* 800FCF68 0C02C160 */  jal 0x800B0580
    /* 800FCF6C 00000000 */  nop
    /* 800FCF70 82790000 */  lb $t9, 0($s3)
    /* 800FCF74 1B20000C */  blez $t9, 0x800FCFA8
    /* 800FCF78 00102400 */  sll $a0, $s0, 16
    /* 800FCF7C 00047403 */  sra $t6, $a0, 16
    /* 800FCF80 0C02C7CC */  jal 0x800B1F30
    /* 800FCF84 01C02025 */  or $a0, $t6, $zero
    /* 800FCF88 82790000 */  lb $t9, 0($s3)
    /* 800FCF8C 26100001 */  addiu $s0, $s0, 1
    /* 800FCF90 00107C00 */  sll $t7, $s0, 16
    /* 800FCF94 000F8403 */  sra $s0, $t7, 16
    /* 800FCF98 0219082A */  slt $at, $s0, $t9
    /* 800FCF9C 5420FFF7 */  .word 0x5420FFF7
    /* 800FCFA0 00102400 */  sll $a0, $s0, 16
    /* 800FCFA4 00008025 */  or $s0, $zero, $zero
    /* 800FCFA8 3C028015 */  lui $v0, 0x8015
    /* 800FCFAC 2442A118 */  addiu $v0, $v0, -24296
    /* 800FCFB0 2403004C */  addiu $v1, $zero, 76
    /* 800FCFB4 02030019 */  multu $s0, $v1
    /* 800FCFB8 00007012 */  mflo $t6
    /* 800FCFBC 004E7821 */  addu $t7, $v0, $t6
    /* 800FCFC0 A1F00000 */  sb $s0, 0($t7)
    /* 800FCFC4 26100001 */  addiu $s0, $s0, 1
    /* 800FCFC8 0010C400 */  sll $t8, $s0, 16
    /* 800FCFCC 00188403 */  sra $s0, $t8, 16
    /* 800FCFD0 2A010004 */  slti $at, $s0, 4
    /* 800FCFD4 1420FFF7 */  bne $at, $zero, 0x800FCFB4
    /* 800FCFD8 00000000 */  nop
    /* 800FCFDC 3C0E8011 */  lui $t6, 0x8011
    /* 800FCFE0 8DCE74B4 */  lw $t6, 29876($t6)
    /* 800FCFE4 3C048015 */  lui $a0, 0x8015
    /* 800FCFE8 000E7980 */  sll $t7, $t6, 6
    /* 800FCFEC 05E20004 */  .word 0x05E20004
    /* 800FCFF0 8FBF0014 */  lw $ra, 20($sp)
    /* 800FCFF4 0C02E44B */  jal 0x800B912C
    /* 800FCFF8 8084978C */  lb $a0, -26740($a0)
    /* 800FCFFC 8FBF0014 */  lw $ra, 20($sp)
    /* 800FD000 27BD0018 */  addiu $sp, $sp, 24
    /* 800FD004 03E00008 */  jr $ra
    /* 800FD008 00000000 */  nop
