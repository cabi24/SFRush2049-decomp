# Source: game_code.bin (decompressed)
# Address: 0x800F87AC

glabel find_maxpath_intervals
    /* 800F87AC 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800F87B0 AFBF0014 */  sw $ra, 20($sp)
    /* 800F87B4 308E0003 */  andi $t6, $a0, 0x0003
    /* 800F87B8 11C000C0 */  beq $t6, $zero, 0x800F8ABC
    /* 800F87BC 00000000 */  nop
    /* 800F87C0 0C02D7F1 */  jal 0x800B5FC4
    /* 800F87C4 00000000 */  nop
    /* 800F87C8 3C118014 */  lui $s1, 0x8014
    /* 800F87CC 263191E0 */  addiu $s1, $s1, -28192
    /* 800F87D0 8E2F0000 */  lw $t7, 0($s1)
    /* 800F87D4 2DE10007 */  sltiu $at, $t7, 7
    /* 800F87D8 102000DF */  beq $at, $zero, 0x800F8B58
    /* 800F87DC 000F7880 */  sll $t7, $t7, 2
    /* 800F87E0 3C018012 */  lui $at, 0x8012
    /* 800F87E4 002F0821 */  addu $at, $at, $t7
    /* 800F87E8 8C2F461C */  lw $t7, 17948($at)
    /* 800F87EC 01E00008 */  jr $t7
    /* 800F87F0 00000000 */  nop
    /* 800F87F4 0C03E15F */  jal 0x800F857C
    /* 800F87F8 24100002 */  addiu $s0, $zero, 2
    /* 800F87FC 100000D7 */  beq $zero, $zero, 0x800F8B5C
    /* 800F8800 8FBF0014 */  lw $ra, 20($sp)
    /* 800F8804 3C188015 */  lui $t8, 0x8015
    /* 800F8808 8F18A110 */  lw $t8, -24304($t8)
    /* 800F880C 24010002 */  addiu $at, $zero, 2
    /* 800F8810 3C128014 */  lui $s2, 0x8014
    /* 800F8814 17010038 */  bne $t8, $at, 0x800F88F8
    /* 800F8818 26522728 */  addiu $s2, $s2, 10024
    /* 800F881C 02402025 */  or $a0, $s2, $zero
    /* 800F8820 00002825 */  or $a1, $zero, $zero
    /* 800F8824 0C001C9C */  jal 0x80007270
    /* 800F8828 24060001 */  addiu $a2, $zero, 1
    /* 800F882C 0C0246C0 */  jal 0x80091B00
    /* 800F8830 00000000 */  nop
    /* 800F8834 24190001 */  addiu $t9, $zero, 1
    /* 800F8838 A0590002 */  sb $t9, 2($v0)
    /* 800F883C 00408025 */  or $s0, $v0, $zero
    /* 800F8840 02402025 */  or $a0, $s2, $zero
    /* 800F8844 00002825 */  or $a1, $zero, $zero
    /* 800F8848 0C001D78 */  jal 0x800075E0
    /* 800F884C 00003025 */  or $a2, $zero, $zero
    /* 800F8850 3C048014 */  lui $a0, 0x8014
    /* 800F8854 248427A8 */  addiu $a0, $a0, 10152
    /* 800F8858 02002825 */  or $a1, $s0, $zero
    /* 800F885C 0C001D78 */  jal 0x800075E0
    /* 800F8860 00003025 */  or $a2, $zero, $zero
    /* 800F8864 3C048015 */  lui $a0, 0x8015
    /* 800F8868 0C02E44B */  jal 0x800B912C
    /* 800F886C 8084978C */  lb $a0, -26740($a0)
    /* 800F8870 3C128014 */  lui $s2, 0x8014
    /* 800F8874 265291F0 */  addiu $s2, $s2, -28176
    /* 800F8878 8E500000 */  lw $s0, 0($s2)
    /* 800F887C 12000006 */  beq $s0, $zero, 0x800F8898
    /* 800F8880 02002025 */  or $a0, $s0, $zero
    /* 800F8884 0C0241E7 */  jal 0x8009079C
    /* 800F8888 24050001 */  addiu $a1, $zero, 1
    /* 800F888C 8E500000 */  lw $s0, 0($s2)
    /* 800F8890 5600FFFC */  .word 0x5600FFFC
    /* 800F8894 02002025 */  or $a0, $s0, $zero
    /* 800F8898 0C02C160 */  jal 0x800B0580
    /* 800F889C 00000000 */  nop
    /* 800F88A0 240E0001 */  addiu $t6, $zero, 1
    /* 800F88A4 3C018015 */  lui $at, 0x8015
    /* 800F88A8 A42E1AD0 */  sh $t6, 6864($at)
    /* 800F88AC 0C029AF9 */  jal 0x800A6BE4
    /* 800F88B0 24040001 */  addiu $a0, $zero, 1
    /* 800F88B4 3C108015 */  lui $s0, 0x8015
    /* 800F88B8 261041A4 */  addiu $s0, $s0, 16804
    /* 800F88BC 8E040000 */  lw $a0, 0($s0)
    /* 800F88C0 10800004 */  beq $a0, $zero, 0x800F88D4
    /* 800F88C4 00000000 */  nop
    /* 800F88C8 0C02CD63 */  jal 0x800B358C
    /* 800F88CC 00000000 */  nop
    /* 800F88D0 AE000000 */  sw $zero, 0($s0)
    /* 800F88D4 0C03E1D5 */  jal 0x800F8754
    /* 800F88D8 00000000 */  nop
    /* 800F88DC 3C0F0004 */  lui $t7, 0x0004
    /* 800F88E0 3C018011 */  lui $at, 0x8011
    /* 800F88E4 AC2F74BC */  sw $t7, 29884($at)
    /* 800F88E8 3C018011 */  lui $at, 0x8011
    /* 800F88EC 24180004 */  addiu $t8, $zero, 4
    /* 800F88F0 10000099 */  beq $zero, $zero, 0x800F8B58
    /* 800F88F4 AC3874B8 */  sw $t8, 29880($at)
    /* 800F88F8 0C03E1D5 */  jal 0x800F8754
    /* 800F88FC 00000000 */  nop
    /* 800F8900 3C128014 */  lui $s2, 0x8014
    /* 800F8904 26522728 */  addiu $s2, $s2, 10024
    /* 800F8908 24190001 */  addiu $t9, $zero, 1
    /* 800F890C 3C018011 */  lui $at, 0x8011
    /* 800F8910 A0394650 */  sb $t9, 18000($at)
    /* 800F8914 02402025 */  or $a0, $s2, $zero
    /* 800F8918 00002825 */  or $a1, $zero, $zero
    /* 800F891C 0C001C9C */  jal 0x80007270
    /* 800F8920 24060001 */  addiu $a2, $zero, 1
    /* 800F8924 0C0246C0 */  jal 0x80091B00
    /* 800F8928 00000000 */  nop
    /* 800F892C 240E0007 */  addiu $t6, $zero, 7
    /* 800F8930 A04E0002 */  sb $t6, 2($v0)
    /* 800F8934 00408025 */  or $s0, $v0, $zero
    /* 800F8938 02402025 */  or $a0, $s2, $zero
    /* 800F893C 00002825 */  or $a1, $zero, $zero
    /* 800F8940 0C001D78 */  jal 0x800075E0
    /* 800F8944 00003025 */  or $a2, $zero, $zero
    /* 800F8948 3C048014 */  lui $a0, 0x8014
    /* 800F894C 248427A8 */  addiu $a0, $a0, 10152
    /* 800F8950 02002825 */  or $a1, $s0, $zero
    /* 800F8954 0C001D78 */  jal 0x800075E0
    /* 800F8958 00003025 */  or $a2, $zero, $zero
    /* 800F895C 0C035681 */  jal 0x800D5A04
    /* 800F8960 00000000 */  nop
    /* 800F8964 1000007D */  beq $zero, $zero, 0x800F8B5C
    /* 800F8968 8FBF0014 */  lw $ra, 20($sp)
    /* 800F896C 3C128014 */  lui $s2, 0x8014
    /* 800F8970 26522728 */  addiu $s2, $s2, 10024
    /* 800F8974 02402025 */  or $a0, $s2, $zero
    /* 800F8978 00002825 */  or $a1, $zero, $zero
    /* 800F897C 0C001C9C */  jal 0x80007270
    /* 800F8980 24060001 */  addiu $a2, $zero, 1
    /* 800F8984 0C0246C0 */  jal 0x80091B00
    /* 800F8988 00000000 */  nop
    /* 800F898C 240F0001 */  addiu $t7, $zero, 1
    /* 800F8990 A04F0002 */  sb $t7, 2($v0)
    /* 800F8994 00408025 */  or $s0, $v0, $zero
    /* 800F8998 02402025 */  or $a0, $s2, $zero
    /* 800F899C 00002825 */  or $a1, $zero, $zero
    /* 800F89A0 0C001D78 */  jal 0x800075E0
    /* 800F89A4 00003025 */  or $a2, $zero, $zero
    /* 800F89A8 3C048014 */  lui $a0, 0x8014
    /* 800F89AC 248427A8 */  addiu $a0, $a0, 10152
    /* 800F89B0 02002825 */  or $a1, $s0, $zero
    /* 800F89B4 0C001D78 */  jal 0x800075E0
    /* 800F89B8 00003025 */  or $a2, $zero, $zero
    /* 800F89BC 3C048015 */  lui $a0, 0x8015
    /* 800F89C0 0C02E44B */  jal 0x800B912C
    /* 800F89C4 8084978C */  lb $a0, -26740($a0)
    /* 800F89C8 3C128014 */  lui $s2, 0x8014
    /* 800F89CC 265291F0 */  addiu $s2, $s2, -28176
    /* 800F89D0 8E500000 */  lw $s0, 0($s2)
    /* 800F89D4 12000006 */  beq $s0, $zero, 0x800F89F0
    /* 800F89D8 02002025 */  or $a0, $s0, $zero
    /* 800F89DC 0C0241E7 */  jal 0x8009079C
    /* 800F89E0 24050001 */  addiu $a1, $zero, 1
    /* 800F89E4 8E500000 */  lw $s0, 0($s2)
    /* 800F89E8 5600FFFC */  .word 0x5600FFFC
    /* 800F89EC 02002025 */  or $a0, $s0, $zero
    /* 800F89F0 0C02C160 */  jal 0x800B0580
    /* 800F89F4 00000000 */  nop
    /* 800F89F8 24180001 */  addiu $t8, $zero, 1
    /* 800F89FC 3C018015 */  lui $at, 0x8015
    /* 800F8A00 A4381AD0 */  sh $t8, 6864($at)
    /* 800F8A04 0C029AF9 */  jal 0x800A6BE4
    /* 800F8A08 24040001 */  addiu $a0, $zero, 1
    /* 800F8A0C 3C108015 */  lui $s0, 0x8015
    /* 800F8A10 261041A4 */  addiu $s0, $s0, 16804
    /* 800F8A14 8E040000 */  lw $a0, 0($s0)
    /* 800F8A18 50800005 */  .word 0x50800005
    /* 800F8A1C 8E240000 */  lw $a0, 0($s1)
    /* 800F8A20 0C02CD63 */  jal 0x800B358C
    /* 800F8A24 00000000 */  nop
    /* 800F8A28 AE000000 */  sw $zero, 0($s0)
    /* 800F8A2C 8E240000 */  lw $a0, 0($s1)
    /* 800F8A30 24010002 */  addiu $at, $zero, 2
    /* 800F8A34 24190040 */  addiu $t9, $zero, 64
    /* 800F8A38 14810003 */  bne $a0, $at, 0x800F8A48
    /* 800F8A3C 3C018011 */  lui $at, 0x8011
    /* 800F8A40 10000018 */  beq $zero, $zero, 0x800F8AA4
    /* 800F8A44 AC3974BC */  sw $t9, 29884($at)
    /* 800F8A48 24010003 */  addiu $at, $zero, 3
    /* 800F8A4C 14810004 */  bne $a0, $at, 0x800F8A60
    /* 800F8A50 240E0080 */  addiu $t6, $zero, 128
    /* 800F8A54 3C018011 */  lui $at, 0x8011
    /* 800F8A58 10000012 */  beq $zero, $zero, 0x800F8AA4
    /* 800F8A5C AC2E74BC */  sw $t6, 29884($at)
    /* 800F8A60 24010004 */  addiu $at, $zero, 4
    /* 800F8A64 14810004 */  bne $a0, $at, 0x800F8A78
    /* 800F8A68 240F0100 */  addiu $t7, $zero, 256
    /* 800F8A6C 3C018011 */  lui $at, 0x8011
    /* 800F8A70 1000000C */  beq $zero, $zero, 0x800F8AA4
    /* 800F8A74 AC2F74BC */  sw $t7, 29884($at)
    /* 800F8A78 24010005 */  addiu $at, $zero, 5
    /* 800F8A7C 14810005 */  bne $a0, $at, 0x800F8A94
    /* 800F8A80 24070006 */  addiu $a3, $zero, 6
    /* 800F8A84 24184000 */  addiu $t8, $zero, 16384
    /* 800F8A88 3C018011 */  lui $at, 0x8011
    /* 800F8A8C 10000005 */  beq $zero, $zero, 0x800F8AA4
    /* 800F8A90 AC3874BC */  sw $t8, 29884($at)
    /* 800F8A94 14E40003 */  bne $a3, $a0, 0x800F8AA4
    /* 800F8A98 24190010 */  addiu $t9, $zero, 16
    /* 800F8A9C 3C018011 */  lui $at, 0x8011
    /* 800F8AA0 AC3974BC */  sw $t9, 29884($at)
    /* 800F8AA4 0C03E1D5 */  jal 0x800F8754
    /* 800F8AA8 00000000 */  nop
    /* 800F8AAC 240E0004 */  addiu $t6, $zero, 4
    /* 800F8AB0 3C018011 */  lui $at, 0x8011
    /* 800F8AB4 10000028 */  beq $zero, $zero, 0x800F8B58
    /* 800F8AB8 AC2E74B8 */  sw $t6, 29880($at)
    /* 800F8ABC 3C048015 */  lui $a0, 0x8015
    /* 800F8AC0 8C849784 */  lw $a0, -26748($a0)
    /* 800F8AC4 308F0400 */  andi $t7, $a0, 0x0400
    /* 800F8AC8 11E00012 */  beq $t7, $zero, 0x800F8B14
    /* 800F8ACC 308E0800 */  andi $t6, $a0, 0x0800
    /* 800F8AD0 0C02D7D3 */  jal 0x800B5F4C
    /* 800F8AD4 00000000 */  nop
    /* 800F8AD8 3C118014 */  lui $s1, 0x8014
    /* 800F8ADC 263191E0 */  addiu $s1, $s1, -28192
    /* 800F8AE0 24070006 */  addiu $a3, $zero, 6
    /* 800F8AE4 8E380000 */  lw $t8, 0($s1)
    /* 800F8AE8 2704FFFF */  addiu $a0, $t8, -1
    /* 800F8AEC 04810003 */  bgez $a0, 0x800F8AFC
    /* 800F8AF0 AE240000 */  sw $a0, 0($s1)
    /* 800F8AF4 AE270000 */  sw $a3, 0($s1)
    /* 800F8AF8 00E02025 */  or $a0, $a3, $zero
    /* 800F8AFC 0C03E12C */  jal 0x800F84B0
    /* 800F8B00 00000000 */  nop
    /* 800F8B04 5040FFF8 */  .word 0x5040FFF8
    /* 800F8B08 8E380000 */  lw $t8, 0($s1)
    /* 800F8B0C 10000013 */  beq $zero, $zero, 0x800F8B5C
    /* 800F8B10 8FBF0014 */  lw $ra, 20($sp)
    /* 800F8B14 51C00011 */  .word 0x51C00011
    /* 800F8B18 8FBF0014 */  lw $ra, 20($sp)
    /* 800F8B1C 0C02D7D3 */  jal 0x800B5F4C
    /* 800F8B20 00000000 */  nop
    /* 800F8B24 3C118014 */  lui $s1, 0x8014
    /* 800F8B28 263191E0 */  addiu $s1, $s1, -28192
    /* 800F8B2C 8E2F0000 */  lw $t7, 0($s1)
    /* 800F8B30 25E40001 */  addiu $a0, $t7, 1
    /* 800F8B34 28810007 */  slti $at, $a0, 7
    /* 800F8B38 14200003 */  bne $at, $zero, 0x800F8B48
    /* 800F8B3C AE240000 */  sw $a0, 0($s1)
    /* 800F8B40 AE200000 */  sw $zero, 0($s1)
    /* 800F8B44 00002025 */  or $a0, $zero, $zero
    /* 800F8B48 0C03E12C */  jal 0x800F84B0
    /* 800F8B4C 00000000 */  nop
    /* 800F8B50 5040FFF7 */  .word 0x5040FFF7
    /* 800F8B54 8E2F0000 */  lw $t7, 0($s1)
    /* 800F8B58 8FBF0014 */  lw $ra, 20($sp)
    /* 800F8B5C 27BD0018 */  addiu $sp, $sp, 24
    /* 800F8B60 03E00008 */  jr $ra
    /* 800F8B64 00000000 */  nop
