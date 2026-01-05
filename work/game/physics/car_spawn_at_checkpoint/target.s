# Source: game_code.bin (decompressed)
# Address: 0x800AA708

glabel car_spawn_at_checkpoint
    /* 800AA708 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800AA70C AFBE0038 */  sw $fp, 56($sp)
    /* 800AA710 AFB70034 */  sw $s7, 52($sp)
    /* 800AA714 AFB40028 */  sw $s4, 40($sp)
    /* 800AA718 0080B825 */  or $s7, $a0, $zero
    /* 800AA71C AFBF003C */  sw $ra, 60($sp)
    /* 800AA720 AFB60030 */  sw $s6, 48($sp)
    /* 800AA724 AFB5002C */  sw $s5, 44($sp)
    /* 800AA728 AFB30024 */  sw $s3, 36($sp)
    /* 800AA72C AFB20020 */  sw $s2, 32($sp)
    /* 800AA730 AFB1001C */  sw $s1, 28($sp)
    /* 800AA734 AFB00018 */  sw $s0, 24($sp)
    /* 800AA738 0000F025 */  or $fp, $zero, $zero
    /* 800AA73C 24140002 */  addiu $s4, $zero, 2
    /* 800AA740 8EE2005C */  lw $v0, 92($s7)
    /* 800AA744 2C410106 */  sltiu $at, $v0, 262
    /* 800AA748 50200007 */  .word 0x50200007
    /* 800AA74C 2C410003 */  sltiu $at, $v0, 3
    /* 800AA750 0C02A915 */  jal 0x800AA454
    /* 800AA754 02E09825 */  or $s3, $s7, $zero
    /* 800AA758 8EE2005C */  lw $v0, 92($s7)
    /* 800AA75C 24140002 */  addiu $s4, $zero, 2
    /* 800AA760 104000C6 */  beq $v0, $zero, 0x800AAA7C
    /* 800AA764 2C410003 */  sltiu $at, $v0, 3
    /* 800AA768 5420001D */  .word 0x5420001D
    /* 800AA76C 8EF90048 */  lw $t9, 72($s7)
    /* 800AA770 8EEE0030 */  lw $t6, 48($s7)
    /* 800AA774 8EEF0040 */  lw $t7, 64($s7)
    /* 800AA778 8EE30054 */  lw $v1, 84($s7)
    /* 800AA77C 8EF90020 */  lw $t9, 32($s7)
    /* 800AA780 01EEC004 */  .word 0x01EEC004
    /* 800AA784 03237021 */  addu $t6, $t9, $v1
    /* 800AA788 91CF0002 */  lbu $t7, 2($t6)
    /* 800AA78C 8EEE003C */  lw $t6, 60($s7)
    /* 800AA790 01F8C826 */  xor $t9, $t7, $t8
    /* 800AA794 8EF8002C */  lw $t8, 44($s7)
    /* 800AA798 032E7824 */  and $t7, $t9, $t6
    /* 800AA79C AEEF0030 */  sw $t7, 48($s7)
    /* 800AA7A0 000F7040 */  sll $t6, $t7, 1
    /* 800AA7A4 030E7821 */  addu $t7, $t8, $t6
    /* 800AA7A8 8EF8001C */  lw $t8, 28($s7)
    /* 800AA7AC 95FE0000 */  lhu $fp, 0($t7)
    /* 800AA7B0 8EF90028 */  lw $t9, 40($s7)
    /* 800AA7B4 00787024 */  and $t6, $v1, $t8
    /* 800AA7B8 000E7840 */  sll $t7, $t6, 1
    /* 800AA7BC 032FC021 */  addu $t8, $t9, $t7
    /* 800AA7C0 A71E0000 */  sh $fp, 0($t8)
    /* 800AA7C4 8EEF0030 */  lw $t7, 48($s7)
    /* 800AA7C8 8EF9002C */  lw $t9, 44($s7)
    /* 800AA7CC 8EEE0054 */  lw $t6, 84($s7)
    /* 800AA7D0 000FC040 */  sll $t8, $t7, 1
    /* 800AA7D4 03387821 */  addu $t7, $t9, $t8
    /* 800AA7D8 A5EE0000 */  sh $t6, 0($t7)
    /* 800AA7DC 8EF90048 */  lw $t9, 72($s7)
    /* 800AA7E0 8EF80058 */  lw $t8, 88($s7)
    /* 800AA7E4 AEF40048 */  sw $s4, 72($s7)
    /* 800AA7E8 03C01025 */  or $v0, $fp, $zero
    /* 800AA7EC AEF90060 */  sw $t9, 96($s7)
    /* 800AA7F0 13C0001C */  beq $fp, $zero, 0x800AA864
    /* 800AA7F4 A6F8004C */  sh $t8, 76($s7)
    /* 800AA7F8 8EEF0068 */  lw $t7, 104($s7)
    /* 800AA7FC 032F082B */  sltu $at, $t9, $t7
    /* 800AA800 50200019 */  .word 0x50200019
    /* 800AA804 8EF10060 */  lw $s1, 96($s7)
    /* 800AA808 8EF90054 */  lw $t9, 84($s7)
    /* 800AA80C 8EEE0014 */  lw $t6, 20($s7)
    /* 800AA810 02E02825 */  or $a1, $s7, $zero
    /* 800AA814 0322C023 */  subu $t8, $t9, $v0
    /* 800AA818 25CFFEFA */  addiu $t7, $t6, -262
    /* 800AA81C 01F8082B */  sltu $at, $t7, $t8
    /* 800AA820 54200011 */  .word 0x54200011
    /* 800AA824 8EF10060 */  lw $s1, 96($s7)
    /* 800AA828 0C02A889 */  jal 0x800AA224
    /* 800AA82C 33C4FFFF */  andi $a0, $fp, 0xFFFF
    /* 800AA830 2C410006 */  sltiu $at, $v0, 6
    /* 800AA834 1020000B */  beq $at, $zero, 0x800AA864
    /* 800AA838 AEE20048 */  sw $v0, 72($s7)
    /* 800AA83C 24010003 */  addiu $at, $zero, 3
    /* 800AA840 54410009 */  .word 0x54410009
    /* 800AA844 8EF10060 */  lw $s1, 96($s7)
    /* 800AA848 8EEE0054 */  lw $t6, 84($s7)
    /* 800AA84C 8EF80058 */  lw $t8, 88($s7)
    /* 800AA850 01D87823 */  subu $t7, $t6, $t8
    /* 800AA854 2DE11001 */  sltiu $at, $t7, 4097
    /* 800AA858 54200003 */  .word 0x54200003
    /* 800AA85C 8EF10060 */  lw $s1, 96($s7)
    /* 800AA860 AEF40048 */  sw $s4, 72($s7)
    /* 800AA864 8EF10060 */  lw $s1, 96($s7)
    /* 800AA868 8EE30054 */  lw $v1, 84($s7)
    /* 800AA86C 2E210003 */  sltiu $at, $s1, 3
    /* 800AA870 54200055 */  .word 0x54200055
    /* 800AA874 8EEE0050 */  lw $t6, 80($s7)
    /* 800AA878 8EF90048 */  lw $t9, 72($s7)
    /* 800AA87C 02E02025 */  or $a0, $s7, $zero
    /* 800AA880 0239082B */  sltu $at, $s1, $t9
    /* 800AA884 54200050 */  .word 0x54200050
    /* 800AA888 8EEE0050 */  lw $t6, 80($s7)
    /* 800AA88C 8EEE005C */  lw $t6, 92($s7)
    /* 800AA890 96F8004C */  lhu $t8, 76($s7)
    /* 800AA894 2626FFFD */  addiu $a2, $s1, -3
    /* 800AA898 006E8021 */  addu $s0, $v1, $t6
    /* 800AA89C 00782823 */  subu $a1, $v1, $t8
    /* 800AA8A0 2610FFFD */  addiu $s0, $s0, -3
    /* 800AA8A4 0C02A80A */  jal 0x800AA028
    /* 800AA8A8 24A5FFFF */  addiu $a1, $a1, -1
    /* 800AA8AC 8EF10060 */  lw $s1, 96($s7)
    /* 800AA8B0 8EEF005C */  lw $t7, 92($s7)
    /* 800AA8B4 02E02025 */  or $a0, $s7, $zero
    /* 800AA8B8 2638FFFE */  addiu $t8, $s1, -2
    /* 800AA8BC 01F1C823 */  subu $t9, $t7, $s1
    /* 800AA8C0 272E0001 */  addiu $t6, $t9, 1
    /* 800AA8C4 AEEE005C */  sw $t6, 92($s7)
    /* 800AA8C8 AEF80060 */  sw $t8, 96($s7)
    /* 800AA8CC 8EEF0054 */  lw $t7, 84($s7)
    /* 800AA8D0 25E30001 */  addiu $v1, $t7, 1
    /* 800AA8D4 0203082B */  sltu $at, $s0, $v1
    /* 800AA8D8 1420001B */  bne $at, $zero, 0x800AA948
    /* 800AA8DC AEE30054 */  sw $v1, 84($s7)
    /* 800AA8E0 8EEE0030 */  lw $t6, 48($s7)
    /* 800AA8E4 8EF80040 */  lw $t8, 64($s7)
    /* 800AA8E8 8EF90020 */  lw $t9, 32($s7)
    /* 800AA8EC 030E7804 */  .word 0x030E7804
    /* 800AA8F0 03237021 */  addu $t6, $t9, $v1
    /* 800AA8F4 91D80002 */  lbu $t8, 2($t6)
    /* 800AA8F8 8EEE003C */  lw $t6, 60($s7)
    /* 800AA8FC 030FC826 */  xor $t9, $t8, $t7
    /* 800AA900 8EEF002C */  lw $t7, 44($s7)
    /* 800AA904 032EC024 */  and $t8, $t9, $t6
    /* 800AA908 AEF80030 */  sw $t8, 48($s7)
    /* 800AA90C 00187040 */  sll $t6, $t8, 1
    /* 800AA910 01EEC021 */  addu $t8, $t7, $t6
    /* 800AA914 8EEF001C */  lw $t7, 28($s7)
    /* 800AA918 971E0000 */  lhu $fp, 0($t8)
    /* 800AA91C 8EF90028 */  lw $t9, 40($s7)
    /* 800AA920 006F7024 */  and $t6, $v1, $t7
    /* 800AA924 000EC040 */  sll $t8, $t6, 1
    /* 800AA928 03387821 */  addu $t7, $t9, $t8
    /* 800AA92C A5FE0000 */  sh $fp, 0($t7)
    /* 800AA930 8EF80030 */  lw $t8, 48($s7)
    /* 800AA934 8EF9002C */  lw $t9, 44($s7)
    /* 800AA938 8EEE0054 */  lw $t6, 84($s7)
    /* 800AA93C 00187840 */  sll $t7, $t8, 1
    /* 800AA940 032FC021 */  addu $t8, $t9, $t7
    /* 800AA944 A70E0000 */  sh $t6, 0($t8)
    /* 800AA948 8EF90060 */  lw $t9, 96($s7)
    /* 800AA94C 272FFFFF */  addiu $t7, $t9, -1
    /* 800AA950 15E0FFDE */  bne $t7, $zero, 0x800AA8CC
    /* 800AA954 AEEF0060 */  sw $t7, 96($s7)
    /* 800AA958 8EF80054 */  lw $t8, 84($s7)
    /* 800AA95C AEE00050 */  sw $zero, 80($s7)
    /* 800AA960 AEF40048 */  sw $s4, 72($s7)
    /* 800AA964 27190001 */  addiu $t9, $t8, 1
    /* 800AA968 1040FF75 */  beq $v0, $zero, 0x800AA740
    /* 800AA96C AEF90054 */  sw $t9, 84($s7)
    /* 800AA970 8EE20044 */  lw $v0, 68($s7)
    /* 800AA974 00002825 */  or $a1, $zero, $zero
    /* 800AA978 04400004 */  bltz $v0, 0x800AA98C
    /* 800AA97C 00000000 */  nop
    /* 800AA980 8EEF0020 */  lw $t7, 32($s7)
    /* 800AA984 10000001 */  beq $zero, $zero, 0x800AA98C
    /* 800AA988 01E22821 */  addu $a1, $t7, $v0
    /* 800AA98C 8EEE0054 */  lw $t6, 84($s7)
    /* 800AA990 00003825 */  or $a3, $zero, $zero
    /* 800AA994 0C02A672 */  jal 0x800A99C8
    /* 800AA998 01C23023 */  subu $a2, $t6, $v0
    /* 800AA99C 8EF80054 */  lw $t8, 84($s7)
    /* 800AA9A0 8EF00000 */  lw $s0, 0($s7)
    /* 800AA9A4 0C02A034 */  jal 0x800A80D0
    /* 800AA9A8 AEF80044 */  sw $t8, 68($s7)
    /* 800AA9AC 8EF90000 */  lw $t9, 0($s7)
    /* 800AA9B0 8F2F0010 */  lw $t7, 16($t9)
    /* 800AA9B4 55E0FF63 */  .word 0x55E0FF63
    /* 800AA9B8 8EE2005C */  lw $v0, 92($s7)
    /* 800AA9BC 10000051 */  beq $zero, $zero, 0x800AAB04
    /* 800AA9C0 00001025 */  or $v0, $zero, $zero
    /* 800AA9C4 8EEE0050 */  lw $t6, 80($s7)
    /* 800AA9C8 24780001 */  addiu $t8, $v1, 1
    /* 800AA9CC 51C00025 */  .word 0x51C00025
    /* 800AA9D0 8EF9005C */  lw $t9, 92($s7)
    /* 800AA9D4 8EF80020 */  lw $t8, 32($s7)
    /* 800AA9D8 02E02025 */  or $a0, $s7, $zero
    /* 800AA9DC 00002825 */  or $a1, $zero, $zero
    /* 800AA9E0 0303C821 */  addu $t9, $t8, $v1
    /* 800AA9E4 0C02A80A */  jal 0x800AA028
    /* 800AA9E8 9326FFFF */  lbu $a2, -1($t9)
    /* 800AA9EC 10400010 */  beq $v0, $zero, 0x800AAA30
    /* 800AA9F0 02E02025 */  or $a0, $s7, $zero
    /* 800AA9F4 8EE20044 */  lw $v0, 68($s7)
    /* 800AA9F8 00002825 */  or $a1, $zero, $zero
    /* 800AA9FC 04400004 */  bltz $v0, 0x800AAA10
    /* 800AAA00 00000000 */  nop
    /* 800AAA04 8EEF0020 */  lw $t7, 32($s7)
    /* 800AAA08 10000001 */  beq $zero, $zero, 0x800AAA10
    /* 800AAA0C 01E22821 */  addu $a1, $t7, $v0
    /* 800AAA10 8EEE0054 */  lw $t6, 84($s7)
    /* 800AAA14 00003825 */  or $a3, $zero, $zero
    /* 800AAA18 0C02A672 */  jal 0x800A99C8
    /* 800AAA1C 01C23023 */  subu $a2, $t6, $v0
    /* 800AAA20 8EF80054 */  lw $t8, 84($s7)
    /* 800AAA24 8EF00000 */  lw $s0, 0($s7)
    /* 800AAA28 0C02A034 */  jal 0x800A80D0
    /* 800AAA2C AEF80044 */  sw $t8, 68($s7)
    /* 800AAA30 8EF90054 */  lw $t9, 84($s7)
    /* 800AAA34 8EEE005C */  lw $t6, 92($s7)
    /* 800AAA38 272F0001 */  addiu $t7, $t9, 1
    /* 800AAA3C 8EF90000 */  lw $t9, 0($s7)
    /* 800AAA40 25D8FFFF */  addiu $t8, $t6, -1
    /* 800AAA44 AEEF0054 */  sw $t7, 84($s7)
    /* 800AAA48 AEF8005C */  sw $t8, 92($s7)
    /* 800AAA4C 8F2F0010 */  lw $t7, 16($t9)
    /* 800AAA50 55E0FF3C */  .word 0x55E0FF3C
    /* 800AAA54 8EE2005C */  lw $v0, 92($s7)
    /* 800AAA58 1000002A */  beq $zero, $zero, 0x800AAB04
    /* 800AAA5C 00001025 */  or $v0, $zero, $zero
    /* 800AAA60 8EF9005C */  lw $t9, 92($s7)
    /* 800AAA64 240E0001 */  addiu $t6, $zero, 1
    /* 800AAA68 AEEE0050 */  sw $t6, 80($s7)
    /* 800AAA6C 272FFFFF */  addiu $t7, $t9, -1
    /* 800AAA70 AEF80054 */  sw $t8, 84($s7)
    /* 800AAA74 1000FF32 */  beq $zero, $zero, 0x800AA740
    /* 800AAA78 AEEF005C */  sw $t7, 92($s7)
    /* 800AAA7C 8EEE0050 */  lw $t6, 80($s7)
    /* 800AAA80 51C0000A */  .word 0x51C0000A
    /* 800AAA84 8EE20044 */  lw $v0, 68($s7)
    /* 800AAA88 8EF80020 */  lw $t8, 32($s7)
    /* 800AAA8C 8EF90054 */  lw $t9, 84($s7)
    /* 800AAA90 02E02025 */  or $a0, $s7, $zero
    /* 800AAA94 00002825 */  or $a1, $zero, $zero
    /* 800AAA98 03197821 */  addu $t7, $t8, $t9
    /* 800AAA9C 0C02A80A */  jal 0x800AA028
    /* 800AAAA0 91E6FFFF */  lbu $a2, -1($t7)
    /* 800AAAA4 AEE00050 */  sw $zero, 80($s7)
    /* 800AAAA8 8EE20044 */  lw $v0, 68($s7)
    /* 800AAAAC 02E02025 */  or $a0, $s7, $zero
    /* 800AAAB0 00002825 */  or $a1, $zero, $zero
    /* 800AAAB4 04400004 */  bltz $v0, 0x800AAAC8
    /* 800AAAB8 00000000 */  nop
    /* 800AAABC 8EEE0020 */  lw $t6, 32($s7)
    /* 800AAAC0 10000001 */  beq $zero, $zero, 0x800AAAC8
    /* 800AAAC4 01C22821 */  addu $a1, $t6, $v0
    /* 800AAAC8 8EF80054 */  lw $t8, 84($s7)
    /* 800AAACC 24070001 */  addiu $a3, $zero, 1
    /* 800AAAD0 0C02A672 */  jal 0x800A99C8
    /* 800AAAD4 03023023 */  subu $a2, $t8, $v0
    /* 800AAAD8 8EF90054 */  lw $t9, 84($s7)
    /* 800AAADC 8EF00000 */  lw $s0, 0($s7)
    /* 800AAAE0 0C02A034 */  jal 0x800A80D0
    /* 800AAAE4 AEF90044 */  sw $t9, 68($s7)
    /* 800AAAE8 8EEF0000 */  lw $t7, 0($s7)
    /* 800AAAEC 24020001 */  addiu $v0, $zero, 1
    /* 800AAAF0 8DEE0010 */  lw $t6, 16($t7)
    /* 800AAAF4 15C00003 */  bne $t6, $zero, 0x800AAB04
    /* 800AAAF8 00000000 */  nop
    /* 800AAAFC 10000001 */  beq $zero, $zero, 0x800AAB04
    /* 800AAB00 24020002 */  addiu $v0, $zero, 2
    /* 800AAB04 8FBF003C */  lw $ra, 60($sp)
    /* 800AAB08 8FB00018 */  lw $s0, 24($sp)
    /* 800AAB0C 8FB1001C */  lw $s1, 28($sp)
    /* 800AAB10 8FB20020 */  lw $s2, 32($sp)
    /* 800AAB14 8FB30024 */  lw $s3, 36($sp)
    /* 800AAB18 8FB40028 */  lw $s4, 40($sp)
    /* 800AAB1C 8FB5002C */  lw $s5, 44($sp)
    /* 800AAB20 8FB60030 */  lw $s6, 48($sp)
    /* 800AAB24 8FB70034 */  lw $s7, 52($sp)
    /* 800AAB28 8FBE0038 */  lw $fp, 56($sp)
    /* 800AAB2C 03E00008 */  jr $ra
    /* 800AAB30 27BD0040 */  addiu $sp, $sp, 64
