# Source: game_code.bin (decompressed)
# Address: 0x800F27DC

glabel net_connection_update
    /* 800F27DC 26100001 */  addiu $s0, $s0, 1
    /* 800F27E0 00009025 */  or $s2, $zero, $zero
    /* 800F27E4 1840000C */  blez $v0, 0x800F2818
    /* 800F27E8 24130001 */  addiu $s3, $zero, 1
    /* 800F27EC 3C0E8014 */  lui $t6, 0x8014
    /* 800F27F0 25D02694 */  addiu $s0, $t6, 9876
    /* 800F27F4 00501821 */  addu $v1, $v0, $s0
    /* 800F27F8 820F0000 */  lb $t7, 0($s0)
    /* 800F27FC 26100001 */  addiu $s0, $s0, 1
    /* 800F2800 0203082B */  sltu $at, $s0, $v1
    /* 800F2804 166F0002 */  bne $s3, $t7, 0x800F2810
    /* 800F2808 00000000 */  nop
    /* 800F280C 0000B825 */  or $s7, $zero, $zero
    /* 800F2810 5420FFFA */  .word 0x5420FFFA
    /* 800F2814 820F0000 */  lb $t7, 0($s0)
    /* 800F2818 16F30017 */  bne $s7, $s3, 0x800F2878
    /* 800F281C 3C188014 */  lui $t8, 0x8014
    /* 800F2820 83183CE8 */  lb $t8, 15592($t8)
    /* 800F2824 3C028015 */  lui $v0, 0x8015
    /* 800F2828 13000005 */  beq $t8, $zero, 0x800F2840
    /* 800F282C 00000000 */  nop
    /* 800F2830 0C03C484 */  jal 0x800F1210
    /* 800F2834 24120002 */  addiu $s2, $zero, 2
    /* 800F2838 10000010 */  beq $zero, $zero, 0x800F287C
    /* 800F283C 8FBF0014 */  lw $ra, 20($sp)
    /* 800F2840 8C42A110 */  lw $v0, -24304($v0)
    /* 800F2844 24010003 */  addiu $at, $zero, 3
    /* 800F2848 10400005 */  beq $v0, $zero, 0x800F2860
    /* 800F284C 00000000 */  nop
    /* 800F2850 10410003 */  beq $v0, $at, 0x800F2860
    /* 800F2854 24010002 */  addiu $at, $zero, 2
    /* 800F2858 14410005 */  bne $v0, $at, 0x800F2870
    /* 800F285C 00000000 */  nop
    /* 800F2860 0C03C484 */  jal 0x800F1210
    /* 800F2864 24120003 */  addiu $s2, $zero, 3
    /* 800F2868 10000004 */  beq $zero, $zero, 0x800F287C
    /* 800F286C 8FBF0014 */  lw $ra, 20($sp)
    /* 800F2870 0C03C484 */  jal 0x800F1210
    /* 800F2874 00009025 */  or $s2, $zero, $zero
    /* 800F2878 8FBF0014 */  lw $ra, 20($sp)
    /* 800F287C 27BD0018 */  addiu $sp, $sp, 24
    /* 800F2880 03E00008 */  jr $ra
    /* 800F2884 00000000 */  nop
