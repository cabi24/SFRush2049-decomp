# Source: game_code.bin (decompressed)
# Address: 0x800DD4AC

glabel title_screen_handler
    /* 800DD4AC 27BDF898 */  addiu $sp, $sp, -1896
    /* 800DD4B0 8FA80778 */  lw $t0, 1912($sp)
    /* 800DD4B4 AFB30034 */  sw $s3, 52($sp)
    /* 800DD4B8 00809825 */  or $s3, $a0, $zero
    /* 800DD4BC AFBF004C */  sw $ra, 76($sp)
    /* 800DD4C0 AFBE0048 */  sw $fp, 72($sp)
    /* 800DD4C4 AFB70044 */  sw $s7, 68($sp)
    /* 800DD4C8 AFB60040 */  sw $s6, 64($sp)
    /* 800DD4CC AFB5003C */  sw $s5, 60($sp)
    /* 800DD4D0 AFB40038 */  sw $s4, 56($sp)
    /* 800DD4D4 AFB20030 */  sw $s2, 48($sp)
    /* 800DD4D8 AFB1002C */  sw $s1, 44($sp)
    /* 800DD4DC AFB00028 */  sw $s0, 40($sp)
    /* 800DD4E0 AFA5076C */  sw $a1, 1900($sp)
    /* 800DD4E4 AFA60770 */  sw $a2, 1904($sp)
    /* 800DD4E8 11000007 */  beq $t0, $zero, 0x800DD508
    /* 800DD4EC AFA70774 */  sw $a3, 1908($sp)
    /* 800DD4F0 0100F809 */  jalr $ra, $t0
    /* 800DD4F4 00000000 */  nop
    /* 800DD4F8 50400004 */  .word 0x50400004
    /* 800DD4FC 00137080 */  sll $t6, $s3, 2
    /* 800DD500 0C037517 */  jal 0x800DD45C
    /* 800DD504 02602025 */  or $a0, $s3, $zero
    /* 800DD508 00137080 */  sll $t6, $s3, 2
    /* 800DD50C 01D37023 */  subu $t6, $t6, $s3
    /* 800DD510 000E7080 */  sll $t6, $t6, 2
    /* 800DD514 01D37023 */  subu $t6, $t6, $s3
    /* 800DD518 3C0F8015 */  lui $t7, 0x8015
    /* 800DD51C 25EF3FD8 */  addiu $t7, $t7, 16344
    /* 800DD520 000E70C0 */  sll $t6, $t6, 3
    /* 800DD524 01CFC021 */  addu $t8, $t6, $t7
    /* 800DD528 AFB8005C */  sw $t8, 92($sp)
    /* 800DD52C 2711002C */  addiu $s1, $t8, 44
    /* 800DD530 24140001 */  addiu $s4, $zero, 1
    /* 800DD534 8E390014 */  lw $t9, 20($s1)
    /* 800DD538 17200010 */  bne $t9, $zero, 0x800DD57C
    /* 800DD53C 00000000 */  nop
    /* 800DD540 822E0000 */  lb $t6, 0($s1)
    /* 800DD544 55C0000B */  .word 0x55C0000B
    /* 800DD548 2694FFFF */  addiu $s4, $s4, -1
    /* 800DD54C 8FAF0774 */  lw $t7, 1908($sp)
    /* 800DD550 24080001 */  addiu $t0, $zero, 1
    /* 800DD554 A2280000 */  sb $t0, 0($s1)
    /* 800DD558 A1E80000 */  sb $t0, 0($t7)
    /* 800DD55C 8FB80770 */  lw $t8, 1904($sp)
    /* 800DD560 A3000000 */  sb $zero, 0($t8)
    /* 800DD564 8FB9076C */  lw $t9, 1900($sp)
    /* 800DD568 1000022E */  beq $zero, $zero, 0x800DDE24
    /* 800DD56C A3200000 */  sb $zero, 0($t9)
    /* 800DD570 2694FFFF */  addiu $s4, $s4, -1
    /* 800DD574 0681FFEF */  bgez $s4, 0x800DD534
    /* 800DD578 2631FFD4 */  addiu $s1, $s1, -44
    /* 800DD57C 0680023D */  bltz $s4, 0x800DDE74
    /* 800DD580 3C0E8015 */  lui $t6, 0x8015
    /* 800DD584 8DCE694C */  lw $t6, 26956($t6)
    /* 800DD588 3C0F8015 */  lui $t7, 0x8015
    /* 800DD58C 3C048014 */  lui $a0, 0x8014
    /* 800DD590 AE2E0020 */  sw $t6, 32($s1)
    /* 800DD594 8DEF6944 */  lw $t7, 26948($t7)
    /* 800DD598 248461D0 */  addiu $a0, $a0, 25040
    /* 800DD59C 00002825 */  or $a1, $zero, $zero
    /* 800DD5A0 24060001 */  addiu $a2, $zero, 1
    /* 800DD5A4 0C001C9C */  jal 0x80007270
    /* 800DD5A8 AE2F0024 */  sw $t7, 36($s1)
    /* 800DD5AC 2412000B */  addiu $s2, $zero, 11
    /* 800DD5B0 AFB10058 */  sw $s1, 88($sp)
    /* 800DD5B4 0C02D080 */  jal 0x800B4200
    /* 800DD5B8 AFB30768 */  sw $s3, 1896($sp)
    /* 800DD5BC 3C048014 */  lui $a0, 0x8014
    /* 800DD5C0 248461D0 */  addiu $a0, $a0, 25040
    /* 800DD5C4 8FB10058 */  lw $s1, 88($sp)
    /* 800DD5C8 8FB30768 */  lw $s3, 1896($sp)
    /* 800DD5CC 00408025 */  or $s0, $v0, $zero
    /* 800DD5D0 00002825 */  or $a1, $zero, $zero
    /* 800DD5D4 0C001D78 */  jal 0x800075E0
    /* 800DD5D8 00003025 */  or $a2, $zero, $zero
    /* 800DD5DC 0C02DD28 */  jal 0x800B74A0
    /* 800DD5E0 24040001 */  addiu $a0, $zero, 1
    /* 800DD5E4 3C048014 */  lui $a0, 0x8014
    /* 800DD5E8 248461D0 */  addiu $a0, $a0, 25040
    /* 800DD5EC 00002825 */  or $a1, $zero, $zero
    /* 800DD5F0 0C001C9C */  jal 0x80007270
    /* 800DD5F4 24060001 */  addiu $a2, $zero, 1
    /* 800DD5F8 2412000B */  addiu $s2, $zero, 11
    /* 800DD5FC AFB10058 */  sw $s1, 88($sp)
    /* 800DD600 0C02D080 */  jal 0x800B4200
    /* 800DD604 AFB30768 */  sw $s3, 1896($sp)
    /* 800DD608 3C048014 */  lui $a0, 0x8014
    /* 800DD60C 248461D0 */  addiu $a0, $a0, 25040
    /* 800DD610 8FB10058 */  lw $s1, 88($sp)
    /* 800DD614 8FB30768 */  lw $s3, 1896($sp)
    /* 800DD618 00408025 */  or $s0, $v0, $zero
    /* 800DD61C 00002825 */  or $a1, $zero, $zero
    /* 800DD620 0C001D78 */  jal 0x800075E0
    /* 800DD624 00003025 */  or $a2, $zero, $zero
    /* 800DD628 0C02DD28 */  jal 0x800B74A0
    /* 800DD62C 24040001 */  addiu $a0, $zero, 1
    /* 800DD630 3C058012 */  lui $a1, 0x8012
    /* 800DD634 24A51060 */  addiu $a1, $a1, 4192
    /* 800DD638 27A40718 */  addiu $a0, $sp, 1816
    /* 800DD63C 0C001264 */  jal 0x80004990
    /* 800DD640 26660001 */  addiu $a2, $s3, 1
    /* 800DD644 8638001C */  lh $t8, 28($s1)
    /* 800DD648 862E0018 */  lh $t6, 24($s1)
    /* 800DD64C 27A40718 */  addiu $a0, $sp, 1816
    /* 800DD650 07010003 */  bgez $t8, 0x800DD660
    /* 800DD654 0018C843 */  sra $t9, $t8, 1
    /* 800DD658 27010001 */  addiu $at, $t8, 1
    /* 800DD65C 0001C843 */  sra $t9, $at, 1
    /* 800DD660 032E8021 */  addu $s0, $t9, $t6
    /* 800DD664 00107C00 */  sll $t7, $s0, 16
    /* 800DD668 000F8403 */  sra $s0, $t7, 16
    /* 800DD66C 0C02CFE9 */  jal 0x800B3FA4
    /* 800DD670 2405FFFF */  addiu $a1, $zero, -1
    /* 800DD674 0002C842 */  srl $t9, $v0, 1
    /* 800DD678 02197823 */  subu $t7, $s0, $t9
    /* 800DD67C 000F2400 */  sll $a0, $t7, 16
    /* 800DD680 0004C403 */  sra $t8, $a0, 16
    /* 800DD684 03002025 */  or $a0, $t8, $zero
    /* 800DD688 8625001A */  lh $a1, 26($s1)
    /* 800DD68C 0C02DC75 */  jal 0x800B71D4
    /* 800DD690 27A60718 */  addiu $a2, $sp, 1816
    /* 800DD694 8E390014 */  lw $t9, 20($s1)
    /* 800DD698 272EFFFF */  addiu $t6, $t9, -1
    /* 800DD69C 2DC1000F */  sltiu $at, $t6, 15
    /* 800DD6A0 102001D3 */  beq $at, $zero, 0x800DDDF0
    /* 800DD6A4 000E7080 */  sll $t6, $t6, 2
    /* 800DD6A8 3C018012 */  lui $at, 0x8012
    /* 800DD6AC 002E0821 */  addu $at, $at, $t6
    /* 800DD6B0 8C2E42CC */  lw $t6, 17100($at)
    /* 800DD6B4 01C00008 */  jr $t6
    /* 800DD6B8 00000000 */  nop
    /* 800DD6BC 02602025 */  or $a0, $s3, $zero
    /* 800DD6C0 02802825 */  or $a1, $s4, $zero
    /* 800DD6C4 0C037356 */  jal 0x800DCD58
    /* 800DD6C8 AFB10058 */  sw $s1, 88($sp)
    /* 800DD6CC 3C048014 */  lui $a0, 0x8014
    /* 800DD6D0 248461D0 */  addiu $a0, $a0, 25040
    /* 800DD6D4 00002825 */  or $a1, $zero, $zero
    /* 800DD6D8 24060001 */  addiu $a2, $zero, 1
    /* 800DD6DC 0C001C9C */  jal 0x80007270
    /* 800DD6E0 8FB10058 */  lw $s1, 88($sp)
    /* 800DD6E4 2412000B */  addiu $s2, $zero, 11
    /* 800DD6E8 AFB10058 */  sw $s1, 88($sp)
    /* 800DD6EC 0C02D080 */  jal 0x800B4200
    /* 800DD6F0 AFB30768 */  sw $s3, 1896($sp)
    /* 800DD6F4 3C048014 */  lui $a0, 0x8014
    /* 800DD6F8 248461D0 */  addiu $a0, $a0, 25040
    /* 800DD6FC 8FB10058 */  lw $s1, 88($sp)
    /* 800DD700 00408025 */  or $s0, $v0, $zero
    /* 800DD704 00002825 */  or $a1, $zero, $zero
    /* 800DD708 0C001D78 */  jal 0x800075E0
    /* 800DD70C 00003025 */  or $a2, $zero, $zero
    /* 800DD710 0C02DD28 */  jal 0x800B74A0
    /* 800DD714 24040001 */  addiu $a0, $zero, 1
    /* 800DD718 3C108018 */  lui $s0, 0x8018
    /* 800DD71C 2610A4E0 */  addiu $s0, $s0, -23328
    /* 800DD720 8E0F0004 */  lw $t7, 4($s0)
    /* 800DD724 27A405F0 */  addiu $a0, $sp, 1520
    /* 800DD728 0C02F9A9 */  jal 0x800BE6A4
    /* 800DD72C 8DE50194 */  lw $a1, 404($t7)
    /* 800DD730 3C058012 */  lui $a1, 0x8012
    /* 800DD734 24A51070 */  addiu $a1, $a1, 4208
    /* 800DD738 0C02F93C */  jal 0x800BE4F0
    /* 800DD73C 27A405F0 */  addiu $a0, $sp, 1520
    /* 800DD740 8E180004 */  lw $t8, 4($s0)
    /* 800DD744 27A405F0 */  addiu $a0, $sp, 1520
    /* 800DD748 0C02F93C */  jal 0x800BE4F0
    /* 800DD74C 8F050198 */  lw $a1, 408($t8)
    /* 800DD750 8626001C */  lh $a2, 28($s1)
    /* 800DD754 862E0018 */  lh $t6, 24($s1)
    /* 800DD758 8623001E */  lh $v1, 30($s1)
    /* 800DD75C 3C108012 */  lui $s0, 0x8012
    /* 800DD760 26108E20 */  addiu $s0, $s0, -29152
    /* 800DD764 2463FFEC */  addiu $v1, $v1, -20
    /* 800DD768 00033C00 */  sll $a3, $v1, 16
    /* 800DD76C 24080001 */  addiu $t0, $zero, 1
    /* 800DD770 AE080000 */  sw $t0, 0($s0)
    /* 800DD774 AE080004 */  sw $t0, 4($s0)
    /* 800DD778 04C10003 */  bgez $a2, 0x800DD788
    /* 800DD77C 0006C843 */  sra $t9, $a2, 1
    /* 800DD780 24C10001 */  addiu $at, $a2, 1
    /* 800DD784 0001C843 */  sra $t9, $at, 1
    /* 800DD788 032E2021 */  addu $a0, $t9, $t6
    /* 800DD78C 862E001A */  lh $t6, 26($s1)
    /* 800DD790 00047C00 */  sll $t7, $a0, 16
    /* 800DD794 000FC403 */  sra $t8, $t7, 16
    /* 800DD798 03002025 */  or $a0, $t8, $zero
    /* 800DD79C 04610003 */  bgez $v1, 0x800DD7AC
    /* 800DD7A0 0003C843 */  sra $t9, $v1, 1
    /* 800DD7A4 24610001 */  addiu $at, $v1, 1
    /* 800DD7A8 0001C843 */  sra $t9, $at, 1
    /* 800DD7AC 032E2821 */  addu $a1, $t9, $t6
    /* 800DD7B0 24A50014 */  addiu $a1, $a1, 20
    /* 800DD7B4 00057C00 */  sll $t7, $a1, 16
    /* 800DD7B8 000F2C03 */  sra $a1, $t7, 16
    /* 800DD7BC 27AF05F0 */  addiu $t7, $sp, 1520
    /* 800DD7C0 240EFFFF */  addiu $t6, $zero, -1
    /* 800DD7C4 0007CC03 */  sra $t9, $a3, 16
    /* 800DD7C8 03203825 */  or $a3, $t9, $zero
    /* 800DD7CC AFAE0010 */  sw $t6, 16($sp)
    /* 800DD7D0 AFAF0018 */  sw $t7, 24($sp)
    /* 800DD7D4 0C02F81E */  jal 0x800BE078
    /* 800DD7D8 AFA00014 */  sw $zero, 20($sp)
    /* 800DD7DC 24180003 */  addiu $t8, $zero, 3
    /* 800DD7E0 3C048014 */  lui $a0, 0x8014
    /* 800DD7E4 AE180004 */  sw $t8, 4($s0)
    /* 800DD7E8 AE000000 */  sw $zero, 0($s0)
    /* 800DD7EC 248461D0 */  addiu $a0, $a0, 25040
    /* 800DD7F0 00002825 */  or $a1, $zero, $zero
    /* 800DD7F4 0C001C9C */  jal 0x80007270
    /* 800DD7F8 24060001 */  addiu $a2, $zero, 1
    /* 800DD7FC 2412000B */  addiu $s2, $zero, 11
    /* 800DD800 0C02D080 */  jal 0x800B4200
    /* 800DD804 AFB10058 */  sw $s1, 88($sp)
    /* 800DD808 3C048014 */  lui $a0, 0x8014
    /* 800DD80C 248461D0 */  addiu $a0, $a0, 25040
    /* 800DD810 8FB10058 */  lw $s1, 88($sp)
    /* 800DD814 00408025 */  or $s0, $v0, $zero
    /* 800DD818 00002825 */  or $a1, $zero, $zero
    /* 800DD81C 0C001D78 */  jal 0x800075E0
    /* 800DD820 00003025 */  or $a2, $zero, $zero
    /* 800DD824 10000173 */  beq $zero, $zero, 0x800DDDF4
    /* 800DD828 82390000 */  lb $t9, 0($s1)
    /* 800DD82C 3C198011 */  lui $t9, 0x8011
    /* 800DD830 8F3974B4 */  lw $t9, 29876($t9)
    /* 800DD834 3C017C03 */  lui $at, 0x7C03
    /* 800DD838 3421FFFE */  ori $at, $at, 0xFFFE
    /* 800DD83C 03217024 */  and $t6, $t9, $at
    /* 800DD840 15C00007 */  bne $t6, $zero, 0x800DD860
    /* 800DD844 02602825 */  or $a1, $s3, $zero
    /* 800DD848 02602025 */  or $a0, $s3, $zero
    /* 800DD84C 02802825 */  or $a1, $s4, $zero
    /* 800DD850 0C037356 */  jal 0x800DCD58
    /* 800DD854 AFB10058 */  sw $s1, 88($sp)
    /* 800DD858 10000003 */  beq $zero, $zero, 0x800DD868
    /* 800DD85C 8FB10058 */  lw $s1, 88($sp)
    /* 800DD860 0C037338 */  jal 0x800DCCE0
    /* 800DD864 02803025 */  or $a2, $s4, $zero
    /* 800DD868 AFB30000 */  sw $s3, 0($sp)
    /* 800DD86C AFB40004 */  sw $s4, 4($sp)
    /* 800DD870 AFB10058 */  sw $s1, 88($sp)
    /* 800DD874 0C037430 */  jal 0x800DD0C0
    /* 800DD878 AFB40764 */  sw $s4, 1892($sp)
    /* 800DD87C 8FB10058 */  lw $s1, 88($sp)
    /* 800DD880 1000015B */  beq $zero, $zero, 0x800DDDF0
    /* 800DD884 8FB40764 */  lw $s4, 1892($sp)
    /* 800DD888 02602025 */  or $a0, $s3, $zero
    /* 800DD88C 02802825 */  or $a1, $s4, $zero
    /* 800DD890 0C037356 */  jal 0x800DCD58
    /* 800DD894 AFB10058 */  sw $s1, 88($sp)
    /* 800DD898 8FB10058 */  lw $s1, 88($sp)
    /* 800DD89C 3C108018 */  lui $s0, 0x8018
    /* 800DD8A0 2610A4E0 */  addiu $s0, $s0, -23328
    /* 800DD8A4 822F0010 */  lb $t7, 16($s1)
    /* 800DD8A8 51E00007 */  .word 0x51E00007
    /* 800DD8AC 8E190004 */  lw $t9, 4($s0)
    /* 800DD8B0 3C108018 */  lui $s0, 0x8018
    /* 800DD8B4 2610A4E0 */  addiu $s0, $s0, -23328
    /* 800DD8B8 8E180004 */  lw $t8, 4($s0)
    /* 800DD8BC 10000003 */  beq $zero, $zero, 0x800DD8CC
    /* 800DD8C0 8F0501A8 */  lw $a1, 424($t8)
    /* 800DD8C4 8E190004 */  lw $t9, 4($s0)
    /* 800DD8C8 8F2501AC */  lw $a1, 428($t9)
    /* 800DD8CC 0C02F9A9 */  jal 0x800BE6A4
    /* 800DD8D0 27A404A8 */  addiu $a0, $sp, 1192
    /* 800DD8D4 3C058012 */  lui $a1, 0x8012
    /* 800DD8D8 24A51074 */  addiu $a1, $a1, 4212
    /* 800DD8DC 0C02F93C */  jal 0x800BE4F0
    /* 800DD8E0 27A404A8 */  addiu $a0, $sp, 1192
    /* 800DD8E4 8E0E0004 */  lw $t6, 4($s0)
    /* 800DD8E8 27A404A8 */  addiu $a0, $sp, 1192
    /* 800DD8EC 0C02F93C */  jal 0x800BE4F0
    /* 800DD8F0 8DC501B0 */  lw $a1, 432($t6)
    /* 800DD8F4 8E18000C */  lw $t8, 12($s0)
    /* 800DD8F8 8E0F0010 */  lw $t7, 16($s0)
    /* 800DD8FC 0260B025 */  or $s6, $s3, $zero
    /* 800DD900 97190032 */  lhu $t9, 50($t8)
    /* 800DD904 0280B825 */  or $s7, $s4, $zero
    /* 800DD908 27BE04A8 */  addiu $fp, $sp, 1192
    /* 800DD90C 00197080 */  sll $t6, $t9, 2
    /* 800DD910 01EEC021 */  addu $t8, $t7, $t6
    /* 800DD914 8F190000 */  lw $t9, 0($t8)
    /* 800DD918 8E0F0004 */  lw $t7, 4($s0)
    /* 800DD91C 82350001 */  lb $s5, 1($s1)
    /* 800DD920 AFB9000C */  sw $t9, 12($sp)
    /* 800DD924 8DEE03B0 */  lw $t6, 944($t7)
    /* 800DD928 AFB40764 */  sw $s4, 1892($sp)
    /* 800DD92C AFB10058 */  sw $s1, 88($sp)
    /* 800DD930 0C03737D */  jal 0x800DCDF4
    /* 800DD934 AFAE0010 */  sw $t6, 16($sp)
    /* 800DD938 8FB10058 */  lw $s1, 88($sp)
    /* 800DD93C 1000012C */  beq $zero, $zero, 0x800DDDF0
    /* 800DD940 8FB40764 */  lw $s4, 1892($sp)
    /* 800DD944 02602825 */  or $a1, $s3, $zero
    /* 800DD948 0C037338 */  jal 0x800DCCE0
    /* 800DD94C 02803025 */  or $a2, $s4, $zero
    /* 800DD950 3C108018 */  lui $s0, 0x8018
    /* 800DD954 2610A4E0 */  addiu $s0, $s0, -23328
    /* 800DD958 8E180004 */  lw $t8, 4($s0)
    /* 800DD95C 27A40398 */  addiu $a0, $sp, 920
    /* 800DD960 0C02F9A9 */  jal 0x800BE6A4
    /* 800DD964 8F0501B4 */  lw $a1, 436($t8)
    /* 800DD968 3C058012 */  lui $a1, 0x8012
    /* 800DD96C 24A51078 */  addiu $a1, $a1, 4216
    /* 800DD970 0C02F93C */  jal 0x800BE4F0
    /* 800DD974 27A40398 */  addiu $a0, $sp, 920
    /* 800DD978 8E190004 */  lw $t9, 4($s0)
    /* 800DD97C 27A40398 */  addiu $a0, $sp, 920
    /* 800DD980 0C02F93C */  jal 0x800BE4F0
    /* 800DD984 8F2501B8 */  lw $a1, 440($t9)
    /* 800DD988 0260A825 */  or $s5, $s3, $zero
    /* 800DD98C 0280B025 */  or $s6, $s4, $zero
    /* 800DD990 27B70398 */  addiu $s7, $sp, 920
    /* 800DD994 AFB10058 */  sw $s1, 88($sp)
    /* 800DD998 0C037223 */  jal 0x800DC88C
    /* 800DD99C AFB40764 */  sw $s4, 1892($sp)
    /* 800DD9A0 8FB10058 */  lw $s1, 88($sp)
    /* 800DD9A4 10000112 */  beq $zero, $zero, 0x800DDDF0
    /* 800DD9A8 8FB40764 */  lw $s4, 1892($sp)
    /* 800DD9AC 02602825 */  or $a1, $s3, $zero
    /* 800DD9B0 0C037338 */  jal 0x800DCCE0
    /* 800DD9B4 02803025 */  or $a2, $s4, $zero
    /* 800DD9B8 3C108018 */  lui $s0, 0x8018
    /* 800DD9BC 2610A4E0 */  addiu $s0, $s0, -23328
    /* 800DD9C0 8E0F0004 */  lw $t7, 4($s0)
    /* 800DD9C4 27A40290 */  addiu $a0, $sp, 656
    /* 800DD9C8 0C02F9A9 */  jal 0x800BE6A4
    /* 800DD9CC 8DE501BC */  lw $a1, 444($t7)
    /* 800DD9D0 3C058012 */  lui $a1, 0x8012
    /* 800DD9D4 24A51088 */  addiu $a1, $a1, 4232
    /* 800DD9D8 0C02F93C */  jal 0x800BE4F0
    /* 800DD9DC 27A40290 */  addiu $a0, $sp, 656
    /* 800DD9E0 8E0E0004 */  lw $t6, 4($s0)
    /* 800DD9E4 27A40290 */  addiu $a0, $sp, 656
    /* 800DD9E8 0C02F93C */  jal 0x800BE4F0
    /* 800DD9EC 8DC50198 */  lw $a1, 408($t6)
    /* 800DD9F0 0260A825 */  or $s5, $s3, $zero
    /* 800DD9F4 0280B025 */  or $s6, $s4, $zero
    /* 800DD9F8 27B70290 */  addiu $s7, $sp, 656
    /* 800DD9FC AFB10058 */  sw $s1, 88($sp)
    /* 800DDA00 0C037223 */  jal 0x800DC88C
    /* 800DDA04 AFB40764 */  sw $s4, 1892($sp)
    /* 800DDA08 8FB10058 */  lw $s1, 88($sp)
    /* 800DDA0C 100000F8 */  beq $zero, $zero, 0x800DDDF0
    /* 800DDA10 8FB40764 */  lw $s4, 1892($sp)
    /* 800DDA14 02602025 */  or $a0, $s3, $zero
    /* 800DDA18 02802825 */  or $a1, $s4, $zero
    /* 800DDA1C 0C037356 */  jal 0x800DCD58
    /* 800DDA20 AFB10058 */  sw $s1, 88($sp)
    /* 800DDA24 3C108018 */  lui $s0, 0x8018
    /* 800DDA28 2610A4E0 */  addiu $s0, $s0, -23328
    /* 800DDA2C 8E19000C */  lw $t9, 12($s0)
    /* 800DDA30 8E180010 */  lw $t8, 16($s0)
    /* 800DDA34 8E020004 */  lw $v0, 4($s0)
    /* 800DDA38 972F0034 */  lhu $t7, 52($t9)
    /* 800DDA3C 8FB10058 */  lw $s1, 88($sp)
    /* 800DDA40 8C5E01BC */  lw $fp, 444($v0)
    /* 800DDA44 000F7080 */  sll $t6, $t7, 2
    /* 800DDA48 030EC821 */  addu $t9, $t8, $t6
    /* 800DDA4C 8F2F0004 */  lw $t7, 4($t9)
    /* 800DDA50 0260B025 */  or $s6, $s3, $zero
    /* 800DDA54 0280B825 */  or $s7, $s4, $zero
    /* 800DDA58 AFAF000C */  sw $t7, 12($sp)
    /* 800DDA5C 8C5803B0 */  lw $t8, 944($v0)
    /* 800DDA60 AFB40764 */  sw $s4, 1892($sp)
    /* 800DDA64 82350001 */  lb $s5, 1($s1)
    /* 800DDA68 0C03737D */  jal 0x800DCDF4
    /* 800DDA6C AFB80010 */  sw $t8, 16($sp)
    /* 800DDA70 8FB10058 */  lw $s1, 88($sp)
    /* 800DDA74 100000DE */  beq $zero, $zero, 0x800DDDF0
    /* 800DDA78 8FB40764 */  lw $s4, 1892($sp)
    /* 800DDA7C 02602025 */  or $a0, $s3, $zero
    /* 800DDA80 02802825 */  or $a1, $s4, $zero
    /* 800DDA84 0C037356 */  jal 0x800DCD58
    /* 800DDA88 AFB10058 */  sw $s1, 88($sp)
    /* 800DDA8C 3C108018 */  lui $s0, 0x8018
    /* 800DDA90 2610A4E0 */  addiu $s0, $s0, -23328
    /* 800DDA94 8E0F000C */  lw $t7, 12($s0)
    /* 800DDA98 8E0E0004 */  lw $t6, 4($s0)
    /* 800DDA9C 8E190010 */  lw $t9, 16($s0)
    /* 800DDAA0 95F80030 */  lhu $t8, 48($t7)
    /* 800DDAA4 8DDE01C0 */  lw $fp, 448($t6)
    /* 800DDAA8 8FB10058 */  lw $s1, 88($sp)
    /* 800DDAAC 00187080 */  sll $t6, $t8, 2
    /* 800DDAB0 032E1021 */  addu $v0, $t9, $t6
    /* 800DDAB4 8C4F0000 */  lw $t7, 0($v0)
    /* 800DDAB8 0260B025 */  or $s6, $s3, $zero
    /* 800DDABC 0280B825 */  or $s7, $s4, $zero
    /* 800DDAC0 AFAF000C */  sw $t7, 12($sp)
    /* 800DDAC4 8C580004 */  lw $t8, 4($v0)
    /* 800DDAC8 AFB40764 */  sw $s4, 1892($sp)
    /* 800DDACC 82350001 */  lb $s5, 1($s1)
    /* 800DDAD0 0C03737D */  jal 0x800DCDF4
    /* 800DDAD4 AFB80010 */  sw $t8, 16($sp)
    /* 800DDAD8 8FB10058 */  lw $s1, 88($sp)
    /* 800DDADC 100000C4 */  beq $zero, $zero, 0x800DDDF0
    /* 800DDAE0 8FB40764 */  lw $s4, 1892($sp)
    /* 800DDAE4 02602025 */  or $a0, $s3, $zero
    /* 800DDAE8 02802825 */  or $a1, $s4, $zero
    /* 800DDAEC 0C037356 */  jal 0x800DCD58
    /* 800DDAF0 AFB10058 */  sw $s1, 88($sp)
    /* 800DDAF4 3C108018 */  lui $s0, 0x8018
    /* 800DDAF8 2610A4E0 */  addiu $s0, $s0, -23328
    /* 800DDAFC 8E190004 */  lw $t9, 4($s0)
    /* 800DDB00 27A40188 */  addiu $a0, $sp, 392
    /* 800DDB04 8FB10058 */  lw $s1, 88($sp)
    /* 800DDB08 0C02F9A9 */  jal 0x800BE6A4
    /* 800DDB0C 8F2501C4 */  lw $a1, 452($t9)
    /* 800DDB10 3C058012 */  lui $a1, 0x8012
    /* 800DDB14 24A5107C */  addiu $a1, $a1, 4220
    /* 800DDB18 0C02F93C */  jal 0x800BE4F0
    /* 800DDB1C 27A40188 */  addiu $a0, $sp, 392
    /* 800DDB20 8E0E0004 */  lw $t6, 4($s0)
    /* 800DDB24 27A40188 */  addiu $a0, $sp, 392
    /* 800DDB28 0C02F93C */  jal 0x800BE4F0
    /* 800DDB2C 8DC501C8 */  lw $a1, 456($t6)
    /* 800DDB30 0260A825 */  or $s5, $s3, $zero
    /* 800DDB34 0280B025 */  or $s6, $s4, $zero
    /* 800DDB38 27B70188 */  addiu $s7, $sp, 392
    /* 800DDB3C AFB10058 */  sw $s1, 88($sp)
    /* 800DDB40 0C037223 */  jal 0x800DC88C
    /* 800DDB44 AFB40764 */  sw $s4, 1892($sp)
    /* 800DDB48 8FB10058 */  lw $s1, 88($sp)
    /* 800DDB4C 100000A8 */  beq $zero, $zero, 0x800DDDF0
    /* 800DDB50 8FB40764 */  lw $s4, 1892($sp)
    /* 800DDB54 02602825 */  or $a1, $s3, $zero
    /* 800DDB58 0C037338 */  jal 0x800DCCE0
    /* 800DDB5C 02803025 */  or $a2, $s4, $zero
    /* 800DDB60 3C108018 */  lui $s0, 0x8018
    /* 800DDB64 2610A4E0 */  addiu $s0, $s0, -23328
    /* 800DDB68 8E0F0004 */  lw $t7, 4($s0)
    /* 800DDB6C 27A40080 */  addiu $a0, $sp, 128
    /* 800DDB70 0C02F9A9 */  jal 0x800BE6A4
    /* 800DDB74 8DE501CC */  lw $a1, 460($t7)
    /* 800DDB78 3C058012 */  lui $a1, 0x8012
    /* 800DDB7C 24A51080 */  addiu $a1, $a1, 4224
    /* 800DDB80 0C02F93C */  jal 0x800BE4F0
    /* 800DDB84 27A40080 */  addiu $a0, $sp, 128
    /* 800DDB88 8E180004 */  lw $t8, 4($s0)
    /* 800DDB8C 27A40080 */  addiu $a0, $sp, 128
    /* 800DDB90 0C02F93C */  jal 0x800BE4F0
    /* 800DDB94 8F0501B8 */  lw $a1, 440($t8)
    /* 800DDB98 0260A825 */  or $s5, $s3, $zero
    /* 800DDB9C 0280B025 */  or $s6, $s4, $zero
    /* 800DDBA0 27B70080 */  addiu $s7, $sp, 128
    /* 800DDBA4 AFB10058 */  sw $s1, 88($sp)
    /* 800DDBA8 0C037223 */  jal 0x800DC88C
    /* 800DDBAC AFB40764 */  sw $s4, 1892($sp)
    /* 800DDBB0 8FB10058 */  lw $s1, 88($sp)
    /* 800DDBB4 1000008E */  beq $zero, $zero, 0x800DDDF0
    /* 800DDBB8 8FB40764 */  lw $s4, 1892($sp)
    /* 800DDBBC 8E240020 */  lw $a0, 32($s1)
    /* 800DDBC0 30990800 */  andi $t9, $a0, 0x0800
    /* 800DDBC4 53200015 */  .word 0x53200015
    /* 800DDBC8 30980400 */  andi $t8, $a0, 0x0400
    /* 800DDBCC 0C02D7D3 */  jal 0x800B5F4C
    /* 800DDBD0 00000000 */  nop
    /* 800DDBD4 822E0001 */  lb $t6, 1($s1)
    /* 800DDBD8 24080001 */  addiu $t0, $zero, 1
    /* 800DDBDC 51C00006 */  .word 0x51C00006
    /* 800DDBE0 822F0002 */  lb $t7, 2($s1)
    /* 800DDBE4 A2200001 */  sb $zero, 1($s1)
    /* 800DDBE8 A2280002 */  sb $t0, 2($s1)
    /* 800DDBEC 1000000A */  beq $zero, $zero, 0x800DDC18
    /* 800DDBF0 8E240020 */  lw $a0, 32($s1)
    /* 800DDBF4 822F0002 */  lb $t7, 2($s1)
    /* 800DDBF8 24080001 */  addiu $t0, $zero, 1
    /* 800DDBFC 51E00005 */  .word 0x51E00005
    /* 800DDC00 A2280001 */  sb $t0, 1($s1)
    /* 800DDC04 A2200002 */  sb $zero, 2($s1)
    /* 800DDC08 10000003 */  beq $zero, $zero, 0x800DDC18
    /* 800DDC0C 8E240020 */  lw $a0, 32($s1)
    /* 800DDC10 A2280001 */  sb $t0, 1($s1)
    /* 800DDC14 8E240020 */  lw $a0, 32($s1)
    /* 800DDC18 30980400 */  andi $t8, $a0, 0x0400
    /* 800DDC1C 13000013 */  beq $t8, $zero, 0x800DDC6C
    /* 800DDC20 24080001 */  addiu $t0, $zero, 1
    /* 800DDC24 0C02D7D3 */  jal 0x800B5F4C
    /* 800DDC28 00000000 */  nop
    /* 800DDC2C 82390001 */  lb $t9, 1($s1)
    /* 800DDC30 24080001 */  addiu $t0, $zero, 1
    /* 800DDC34 53200005 */  .word 0x53200005
    /* 800DDC38 822E0002 */  lb $t6, 2($s1)
    /* 800DDC3C A2200001 */  sb $zero, 1($s1)
    /* 800DDC40 1000000A */  beq $zero, $zero, 0x800DDC6C
    /* 800DDC44 8E240020 */  lw $a0, 32($s1)
    /* 800DDC48 822E0002 */  lb $t6, 2($s1)
    /* 800DDC4C 51C00006 */  .word 0x51C00006
    /* 800DDC50 A2280002 */  sb $t0, 2($s1)
    /* 800DDC54 A2200002 */  sb $zero, 2($s1)
    /* 800DDC58 A2280001 */  sb $t0, 1($s1)
    /* 800DDC5C 10000003 */  beq $zero, $zero, 0x800DDC6C
    /* 800DDC60 8E240020 */  lw $a0, 32($s1)
    /* 800DDC64 A2280002 */  sb $t0, 2($s1)
    /* 800DDC68 8E240020 */  lw $a0, 32($s1)
    /* 800DDC6C 308F0002 */  andi $t7, $a0, 0x0002
    /* 800DDC70 11E00007 */  beq $t7, $zero, 0x800DDC90
    /* 800DDC74 24040025 */  addiu $a0, $zero, 37
    /* 800DDC78 00002825 */  or $a1, $zero, $zero
