# Source: game_code.bin (decompressed)
# Address: 0x800AAE68

glabel car_angular_velocity_clamp
    /* 800AAE68 27BDFF38 */  addiu $sp, $sp, -200
    /* 800AAE6C AFBF0024 */  sw $ra, 36($sp)
    /* 800AAE70 AFA400AC */  sw $a0, 172($sp)
    /* 800AAE74 AFA500B0 */  sw $a1, 176($sp)
    /* 800AAE78 AFB20020 */  sw $s2, 32($sp)
    /* 800AAE7C AFB1001C */  sw $s1, 28($sp)
    /* 800AAE80 AFB00018 */  sw $s0, 24($sp)
    /* 800AAE84 AFA600B8 */  sw $a2, 184($sp)
    /* 800AAE88 AFA700BC */  sw $a3, 188($sp)
    /* 800AAE8C 240516A8 */  addiu $a1, $zero, 5800
    /* 800AAE90 0C025D1C */  jal 0x80097470
    /* 800AAE94 00002025 */  or $a0, $zero, $zero
    /* 800AAE98 8FA800DC */  lw $t0, 220($sp)
    /* 800AAE9C AFA000B4 */  sw $zero, 180($sp)
    /* 800AAEA0 AFA000C0 */  sw $zero, 192($sp)
    /* 800AAEA4 AFA200C4 */  sw $v0, 196($sp)
    /* 800AAEA8 240F0001 */  addiu $t7, $zero, 1
    /* 800AAEAC 010F1804 */  .word 0x010F1804
    /* 800AAEB0 27AE00AC */  addiu $t6, $sp, 172
    /* 800AAEB4 2478FFFF */  addiu $t8, $v1, -1
    /* 800AAEB8 AC4E0000 */  sw $t6, 0($v0)
    /* 800AAEBC AC430014 */  sw $v1, 20($v0)
    /* 800AAEC0 AC58001C */  sw $t8, 28($v0)
    /* 800AAEC4 AC480018 */  sw $t0, 24($v0)
    /* 800AAEC8 8FA600E0 */  lw $a2, 224($sp)
    /* 800AAECC 24010003 */  addiu $at, $zero, 3
    /* 800AAED0 24190001 */  addiu $t9, $zero, 1
    /* 800AAED4 24C60007 */  addiu $a2, $a2, 7
    /* 800AAED8 24CF0002 */  addiu $t7, $a2, 2
    /* 800AAEDC 01E1001B */  divu $t7, $at
    /* 800AAEE0 00D93804 */  .word 0x00D93804
    /* 800AAEE4 24EEFFFF */  addiu $t6, $a3, -1
    /* 800AAEE8 0000C012 */  mflo $t8
    /* 800AAEEC 00409025 */  or $s2, $v0, $zero
    /* 800AAEF0 AC460038 */  sw $a2, 56($v0)
    /* 800AAEF4 AC470034 */  sw $a3, 52($v0)
    /* 800AAEF8 AC4E003C */  sw $t6, 60($v0)
    /* 800AAEFC AC580040 */  sw $t8, 64($v0)
    /* 800AAF00 00032840 */  sll $a1, $v1, 1
    /* 800AAF04 0C025D1C */  jal 0x80097470
    /* 800AAF08 00002025 */  or $a0, $zero, $zero
    /* 800AAF0C 8E450014 */  lw $a1, 20($s2)
    /* 800AAF10 AE420020 */  sw $v0, 32($s2)
    /* 800AAF14 00002025 */  or $a0, $zero, $zero
    /* 800AAF18 0005C840 */  sll $t9, $a1, 1
    /* 800AAF1C 0C025D1C */  jal 0x80097470
    /* 800AAF20 03202825 */  or $a1, $t9, $zero
    /* 800AAF24 8E450034 */  lw $a1, 52($s2)
    /* 800AAF28 AE420028 */  sw $v0, 40($s2)
    /* 800AAF2C 00002025 */  or $a0, $zero, $zero
    /* 800AAF30 00057040 */  sll $t6, $a1, 1
    /* 800AAF34 0C025D1C */  jal 0x80097470
    /* 800AAF38 01C02825 */  or $a1, $t6, $zero
    /* 800AAF3C AE42002C */  sw $v0, 44($s2)
    /* 800AAF40 8FAF00E0 */  lw $t7, 224($sp)
    /* 800AAF44 24190001 */  addiu $t9, $zero, 1
    /* 800AAF48 00002025 */  or $a0, $zero, $zero
    /* 800AAF4C 25F80006 */  addiu $t8, $t7, 6
    /* 800AAF50 03191804 */  .word 0x03191804
    /* 800AAF54 AE431680 */  sw $v1, 5760($s2)
    /* 800AAF58 0C025D1C */  jal 0x80097470
    /* 800AAF5C 00032880 */  sll $a1, $v1, 2
    /* 800AAF60 8E441680 */  lw $a0, 5760($s2)
    /* 800AAF64 8FA500D8 */  lw $a1, 216($sp)
    /* 800AAF68 AE420004 */  sw $v0, 4($s2)
    /* 800AAF6C 00047042 */  srl $t6, $a0, 1
    /* 800AAF70 000E7840 */  sll $t7, $t6, 1
    /* 800AAF74 01E2C821 */  addu $t9, $t7, $v0
    /* 800AAF78 AE591688 */  sw $t9, 5768($s2)
    /* 800AAF7C 0004C080 */  sll $t8, $a0, 2
    /* 800AAF80 3C198012 */  lui $t9, 0x8012
    /* 800AAF84 0304C023 */  subu $t8, $t8, $a0
    /* 800AAF88 00587021 */  addu $t6, $v0, $t8
    /* 800AAF8C 2739E838 */  addiu $t9, $t9, -6088
    /* 800AAF90 000578C0 */  sll $t7, $a1, 3
    /* 800AAF94 AE4E167C */  sw $t6, 5756($s2)
    /* 800AAF98 01F91821 */  addu $v1, $t7, $t9
    /* 800AAF9C AE45006C */  sw $a1, 108($s2)
    /* 800AAFA0 94780000 */  lhu $t8, 0($v1)
    /* 800AAFA4 AE580070 */  sw $t8, 112($s2)
    /* 800AAFA8 946E0002 */  lhu $t6, 2($v1)
    /* 800AAFAC AE4E0068 */  sw $t6, 104($s2)
    /* 800AAFB0 946F0004 */  lhu $t7, 4($v1)
    /* 800AAFB4 AE4F0074 */  sw $t7, 116($s2)
    /* 800AAFB8 94790006 */  lhu $t9, 6($v1)
    /* 800AAFBC AE590064 */  sw $t9, 100($s2)
    /* 800AAFC0 8FA400C4 */  lw $a0, 196($sp)
    /* 800AAFC4 8C8E0004 */  lw $t6, 4($a0)
    /* 800AAFC8 00809025 */  or $s2, $a0, $zero
    /* 800AAFCC AC80000C */  sw $zero, 12($a0)
    /* 800AAFD0 0C02AACF */  jal 0x800AAB3C
    /* 800AAFD4 AC8E0008 */  sw $t6, 8($a0)
    /* 800AAFD8 8E4F0014 */  lw $t7, 20($s2)
    /* 800AAFDC 8E4E0034 */  lw $t6, 52($s2)
    /* 800AAFE0 8E58002C */  lw $t8, 44($s2)
    /* 800AAFE4 000FC840 */  sll $t9, $t7, 1
    /* 800AAFE8 AE590024 */  sw $t9, 36($s2)
    /* 800AAFEC 000E7840 */  sll $t7, $t6, 1
    /* 800AAFF0 030FC821 */  addu $t9, $t8, $t7
    /* 800AAFF4 A720FFFE */  sh $zero, -2($t9)
    /* 800AAFF8 8E460034 */  lw $a2, 52($s2)
    /* 800AAFFC 8E44002C */  lw $a0, 44($s2)
    /* 800AB000 00002825 */  or $a1, $zero, $zero
    /* 800AB004 00067040 */  sll $t6, $a2, 1
    /* 800AB008 0C0009E4 */  jal 0x80002790
    /* 800AB00C 25C6FFFE */  addiu $a2, $t6, -2
    /* 800AB010 24030002 */  addiu $v1, $zero, 2
    /* 800AB014 AE400054 */  sw $zero, 84($s2)
    /* 800AB018 AE400044 */  sw $zero, 68($s2)
    /* 800AB01C AE40005C */  sw $zero, 92($s2)
    /* 800AB020 AE430060 */  sw $v1, 96($s2)
    /* 800AB024 AE430048 */  sw $v1, 72($s2)
    /* 800AB028 AE400050 */  sw $zero, 80($s2)
    /* 800AB02C AE400030 */  sw $zero, 48($s2)
    /* 800AB030 0C02A9C2 */  jal 0x800AA708
    /* 800AB034 8FA400C4 */  lw $a0, 196($sp)
    /* 800AB038 8E430004 */  lw $v1, 4($s2)
    /* 800AB03C 3C048015 */  lui $a0, 0x8015
    /* 800AB040 24842770 */  addiu $a0, $a0, 10096
    /* 800AB044 1060000C */  beq $v1, $zero, 0x800AB078
    /* 800AB048 00002825 */  or $a1, $zero, $zero
    /* 800AB04C AFA300A0 */  sw $v1, 160($sp)
    /* 800AB050 0C001C9C */  jal 0x80007270
    /* 800AB054 24060001 */  addiu $a2, $zero, 1
    /* 800AB058 8FA500A0 */  lw $a1, 160($sp)
    /* 800AB05C 0C0257F6 */  jal 0x80095FD8
    /* 800AB060 00003025 */  or $a2, $zero, $zero
    /* 800AB064 3C048015 */  lui $a0, 0x8015
    /* 800AB068 24842770 */  addiu $a0, $a0, 10096
    /* 800AB06C 00002825 */  or $a1, $zero, $zero
    /* 800AB070 0C001D78 */  jal 0x800075E0
    /* 800AB074 00003025 */  or $a2, $zero, $zero
    /* 800AB078 8E42002C */  lw $v0, 44($s2)
    /* 800AB07C 3C048015 */  lui $a0, 0x8015
    /* 800AB080 24842770 */  addiu $a0, $a0, 10096
    /* 800AB084 1040000C */  beq $v0, $zero, 0x800AB0B8
    /* 800AB088 00002825 */  or $a1, $zero, $zero
    /* 800AB08C AFA20088 */  sw $v0, 136($sp)
    /* 800AB090 0C001C9C */  jal 0x80007270
    /* 800AB094 24060001 */  addiu $a2, $zero, 1
    /* 800AB098 8FA50088 */  lw $a1, 136($sp)
    /* 800AB09C 0C0257F6 */  jal 0x80095FD8
    /* 800AB0A0 00003025 */  or $a2, $zero, $zero
    /* 800AB0A4 3C048015 */  lui $a0, 0x8015
    /* 800AB0A8 24842770 */  addiu $a0, $a0, 10096
    /* 800AB0AC 00002825 */  or $a1, $zero, $zero
    /* 800AB0B0 0C001D78 */  jal 0x800075E0
    /* 800AB0B4 00003025 */  or $a2, $zero, $zero
    /* 800AB0B8 8E420028 */  lw $v0, 40($s2)
    /* 800AB0BC 3C048015 */  lui $a0, 0x8015
    /* 800AB0C0 24842770 */  addiu $a0, $a0, 10096
    /* 800AB0C4 1040000C */  beq $v0, $zero, 0x800AB0F8
    /* 800AB0C8 00002825 */  or $a1, $zero, $zero
    /* 800AB0CC AFA20070 */  sw $v0, 112($sp)
    /* 800AB0D0 0C001C9C */  jal 0x80007270
    /* 800AB0D4 24060001 */  addiu $a2, $zero, 1
    /* 800AB0D8 8FA50070 */  lw $a1, 112($sp)
    /* 800AB0DC 0C0257F6 */  jal 0x80095FD8
    /* 800AB0E0 00003025 */  or $a2, $zero, $zero
    /* 800AB0E4 3C048015 */  lui $a0, 0x8015
    /* 800AB0E8 24842770 */  addiu $a0, $a0, 10096
    /* 800AB0EC 00002825 */  or $a1, $zero, $zero
    /* 800AB0F0 0C001D78 */  jal 0x800075E0
    /* 800AB0F4 00003025 */  or $a2, $zero, $zero
    /* 800AB0F8 8E420020 */  lw $v0, 32($s2)
    /* 800AB0FC 3C048015 */  lui $a0, 0x8015
    /* 800AB100 24842770 */  addiu $a0, $a0, 10096
    /* 800AB104 1040000C */  beq $v0, $zero, 0x800AB138
    /* 800AB108 00002825 */  or $a1, $zero, $zero
    /* 800AB10C AFA20058 */  sw $v0, 88($sp)
    /* 800AB110 0C001C9C */  jal 0x80007270
    /* 800AB114 24060001 */  addiu $a2, $zero, 1
    /* 800AB118 8FA50058 */  lw $a1, 88($sp)
    /* 800AB11C 0C0257F6 */  jal 0x80095FD8
    /* 800AB120 00003025 */  or $a2, $zero, $zero
    /* 800AB124 3C048015 */  lui $a0, 0x8015
    /* 800AB128 24842770 */  addiu $a0, $a0, 10096
    /* 800AB12C 00002825 */  or $a1, $zero, $zero
    /* 800AB130 0C001D78 */  jal 0x800075E0
    /* 800AB134 00003025 */  or $a2, $zero, $zero
    /* 800AB138 1240000D */  beq $s2, $zero, 0x800AB170
    /* 800AB13C 3C048015 */  lui $a0, 0x8015
    /* 800AB140 24842770 */  addiu $a0, $a0, 10096
    /* 800AB144 00002825 */  or $a1, $zero, $zero
    /* 800AB148 0C001C9C */  jal 0x80007270
    /* 800AB14C 24060001 */  addiu $a2, $zero, 1
    /* 800AB150 02402825 */  or $a1, $s2, $zero
    /* 800AB154 0C0257F6 */  jal 0x80095FD8
    /* 800AB158 00003025 */  or $a2, $zero, $zero
    /* 800AB15C 3C048015 */  lui $a0, 0x8015
    /* 800AB160 24842770 */  addiu $a0, $a0, 10096
    /* 800AB164 00002825 */  or $a1, $zero, $zero
    /* 800AB168 0C001D78 */  jal 0x800075E0
    /* 800AB16C 00003025 */  or $a2, $zero, $zero
    /* 800AB170 8FBF0024 */  lw $ra, 36($sp)
    /* 800AB174 8FA200C0 */  lw $v0, 192($sp)
    /* 800AB178 8FB00018 */  lw $s0, 24($sp)
    /* 800AB17C 8FB1001C */  lw $s1, 28($sp)
    /* 800AB180 8FB20020 */  lw $s2, 32($sp)
    /* 800AB184 03E00008 */  jr $ra
    /* 800AB188 27BD00C8 */  addiu $sp, $sp, 200
