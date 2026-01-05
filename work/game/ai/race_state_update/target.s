# Source: game_code.bin (decompressed)
# Address: 0x800DB81C

glabel race_state_update
    /* 800DB81C 27BDFEF0 */  addiu $sp, $sp, -272
    /* 800DB820 3C038011 */  lui $v1, 0x8011
    /* 800DB824 8C6370FC */  lw $v1, 28924($v1)
    /* 800DB828 AFBF003C */  sw $ra, 60($sp)
    /* 800DB82C AFB50038 */  sw $s5, 56($sp)
    /* 800DB830 AFB40034 */  sw $s4, 52($sp)
    /* 800DB834 AFB30030 */  sw $s3, 48($sp)
    /* 800DB838 AFB2002C */  sw $s2, 44($sp)
    /* 800DB83C AFB10028 */  sw $s1, 40($sp)
    /* 800DB840 AFB00024 */  sw $s0, 36($sp)
    /* 800DB844 F7B60018 */  .word 0xF7B60018
    /* 800DB848 10600006 */  beq $v1, $zero, 0x800DB864
    /* 800DB84C F7B40010 */  .word 0xF7B40010
    /* 800DB850 00002025 */  or $a0, $zero, $zero
    /* 800DB854 0C03204F */  jal 0x800C813C
    /* 800DB858 00002825 */  or $a1, $zero, $zero
    /* 800DB85C 3C038011 */  lui $v1, 0x8011
    /* 800DB860 8C6370FC */  lw $v1, 28924($v1)
    /* 800DB864 2C610009 */  sltiu $at, $v1, 9
    /* 800DB868 102001BF */  beq $at, $zero, 0x800DBF68
    /* 800DB86C 24080002 */  addiu $t0, $zero, 2
    /* 800DB870 00037080 */  sll $t6, $v1, 2
    /* 800DB874 3C018012 */  lui $at, 0x8012
    /* 800DB878 002E0821 */  addu $at, $at, $t6
    /* 800DB87C 8C2E4284 */  lw $t6, 17028($at)
    /* 800DB880 01C00008 */  jr $t6
    /* 800DB884 00000000 */  nop
    /* 800DB888 3C058015 */  lui $a1, 0x8015
    /* 800DB88C 84A51AD0 */  lh $a1, 6864($a1)
    /* 800DB890 3C048015 */  lui $a0, 0x8015
    /* 800DB894 2484A118 */  addiu $a0, $a0, -24296
    /* 800DB898 18A001EE */  blez $a1, 0x800DC054
    /* 800DB89C 00001825 */  or $v1, $zero, $zero
    /* 800DB8A0 3C068015 */  lui $a2, 0x8015
    /* 800DB8A4 24C66CF0 */  addiu $a2, $a2, 27888
    /* 800DB8A8 8C820004 */  lw $v0, 4($a0)
    /* 800DB8AC 0003C900 */  sll $t9, $v1, 4
    /* 800DB8B0 00D97021 */  addu $t6, $a2, $t9
    /* 800DB8B4 304F0001 */  andi $t7, $v0, 0x0001
    /* 800DB8B8 000F102B */  sltu $v0, $zero, $t7
    /* 800DB8BC 14400006 */  bne $v0, $zero, 0x800DB8D8
    /* 800DB8C0 0003C900 */  sll $t9, $v1, 4
    /* 800DB8C4 81C20000 */  lb $v0, 0($t6)
    /* 800DB8C8 2C4F0001 */  sltiu $t7, $v0, 1
    /* 800DB8CC 15E00002 */  bne $t7, $zero, 0x800DB8D8
    /* 800DB8D0 01E01025 */  or $v0, $t7, $zero
    /* 800DB8D4 00001025 */  or $v0, $zero, $zero
    /* 800DB8D8 10400009 */  beq $v0, $zero, 0x800DB900
    /* 800DB8DC 00D97021 */  addu $t6, $a2, $t9
    /* 800DB8E0 81C20000 */  lb $v0, 0($t6)
    /* 800DB8E4 3C058015 */  lui $a1, 0x8015
    /* 800DB8E8 24A5698C */  addiu $a1, $a1, 27020
    /* 800DB8EC ACA30000 */  sw $v1, 0($a1)
    /* 800DB8F0 3C018011 */  lui $at, 0x8011
    /* 800DB8F4 2C4F0001 */  sltiu $t7, $v0, 1
    /* 800DB8F8 10000007 */  beq $zero, $zero, 0x800DB918
    /* 800DB8FC A02F7350 */  sb $t7, 29520($at)
    /* 800DB900 24630001 */  addiu $v1, $v1, 1
    /* 800DB904 0065082A */  slt $at, $v1, $a1
    /* 800DB908 1420FFE7 */  bne $at, $zero, 0x800DB8A8
    /* 800DB90C 2484004C */  addiu $a0, $a0, 76
    /* 800DB910 100001D1 */  beq $zero, $zero, 0x800DC058
    /* 800DB914 8FBF003C */  lw $ra, 60($sp)
    /* 800DB918 0C02D3EC */  jal 0x800B4FB0
    /* 800DB91C 24040001 */  addiu $a0, $zero, 1
    /* 800DB920 3C038015 */  lui $v1, 0x8015
    /* 800DB924 24639D98 */  addiu $v1, $v1, -25192
    /* 800DB928 0C0004F0 */  jal 0x800013C0
    /* 800DB92C AC600000 */  sw $zero, 0($v1)
    /* 800DB930 3C048014 */  lui $a0, 0x8014
    /* 800DB934 24842728 */  addiu $a0, $a0, 10024
    /* 800DB938 00002825 */  or $a1, $zero, $zero
    /* 800DB93C 0C001C9C */  jal 0x80007270
    /* 800DB940 24060001 */  addiu $a2, $zero, 1
    /* 800DB944 0C0246C0 */  jal 0x80091B00
    /* 800DB948 00000000 */  nop
    /* 800DB94C AFA200F4 */  sw $v0, 244($sp)
    /* 800DB950 24180007 */  addiu $t8, $zero, 7
    /* 800DB954 3C048014 */  lui $a0, 0x8014
    /* 800DB958 24842728 */  addiu $a0, $a0, 10024
    /* 800DB95C A0580002 */  sb $t8, 2($v0)
    /* 800DB960 00002825 */  or $a1, $zero, $zero
    /* 800DB964 0C001D78 */  jal 0x800075E0
    /* 800DB968 00003025 */  or $a2, $zero, $zero
    /* 800DB96C 3C048014 */  lui $a0, 0x8014
    /* 800DB970 248427A8 */  addiu $a0, $a0, 10152
    /* 800DB974 8FA500F4 */  lw $a1, 244($sp)
    /* 800DB978 0C001D78 */  jal 0x800075E0
    /* 800DB97C 00003025 */  or $a2, $zero, $zero
    /* 800DB980 4480A000 */  .word 0x4480A000
    /* 800DB984 3C018012 */  lui $at, 0x8012
    /* 800DB988 C43642A8 */  lwc1 $f22, 17064($at)
    /* 800DB98C 24110001 */  addiu $s1, $zero, 1
    /* 800DB990 0C032484 */  jal 0x800C9210
    /* 800DB994 00009025 */  or $s2, $zero, $zero
    /* 800DB998 0C0354DD */  jal 0x800D5374
    /* 800DB99C 00000000 */  nop
    /* 800DB9A0 0C0355E6 */  jal 0x800D5798
    /* 800DB9A4 00000000 */  nop
    /* 800DB9A8 3C028014 */  lui $v0, 0x8014
    /* 800DB9AC 8C423FD8 */  lw $v0, 16344($v0)
    /* 800DB9B0 24080002 */  addiu $t0, $zero, 2
    /* 800DB9B4 2403FFFF */  addiu $v1, $zero, -1
    /* 800DB9B8 10400006 */  beq $v0, $zero, 0x800DB9D4
    /* 800DB9BC 2404FFFF */  addiu $a0, $zero, -1
    /* 800DB9C0 A0480064 */  sb $t0, 100($v0)
    /* 800DB9C4 AC430060 */  sw $v1, 96($v0)
    /* 800DB9C8 8C420000 */  lw $v0, 0($v0)
    /* 800DB9CC 5440FFFD */  .word 0x5440FFFD
    /* 800DB9D0 A0480064 */  sb $t0, 100($v0)
    /* 800DB9D4 0C032130 */  jal 0x800C84C0
    /* 800DB9D8 24050001 */  addiu $a1, $zero, 1
    /* 800DB9DC 0C02D57F */  jal 0x800B55FC
    /* 800DB9E0 24040001 */  addiu $a0, $zero, 1
    /* 800DB9E4 2404002E */  addiu $a0, $zero, 46
    /* 800DB9E8 00002825 */  or $a1, $zero, $zero
    /* 800DB9EC 24060001 */  addiu $a2, $zero, 1
    /* 800DB9F0 0C0248D8 */  jal 0x80092360
    /* 800DB9F4 00003825 */  or $a3, $zero, $zero
    /* 800DB9F8 3C038011 */  lui $v1, 0x8011
    /* 800DB9FC 24080002 */  addiu $t0, $zero, 2
    /* 800DBA00 10000159 */  beq $zero, $zero, 0x800DBF68
    /* 800DBA04 8C6370FC */  lw $v1, 28924($v1)
    /* 800DBA08 0C036C78 */  jal 0x800DB1E0
    /* 800DBA0C 00000000 */  nop
    /* 800DBA10 3C038011 */  lui $v1, 0x8011
    /* 800DBA14 8C6370FC */  lw $v1, 28924($v1)
    /* 800DBA18 10000153 */  beq $zero, $zero, 0x800DBF68
    /* 800DBA1C 24080002 */  addiu $t0, $zero, 2
    /* 800DBA20 0C036468 */  jal 0x800D91A0
    /* 800DBA24 00000000 */  nop
    /* 800DBA28 3C038011 */  lui $v1, 0x8011
    /* 800DBA2C 8C6370FC */  lw $v1, 28924($v1)
    /* 800DBA30 1000014D */  beq $zero, $zero, 0x800DBF68
    /* 800DBA34 24080002 */  addiu $t0, $zero, 2
    /* 800DBA38 0C035D8D */  jal 0x800D7634
    /* 800DBA3C 00000000 */  nop
    /* 800DBA40 3C038011 */  lui $v1, 0x8011
    /* 800DBA44 8C6370FC */  lw $v1, 28924($v1)
    /* 800DBA48 10000147 */  beq $zero, $zero, 0x800DBF68
    /* 800DBA4C 24080002 */  addiu $t0, $zero, 2
    /* 800DBA50 0C02E305 */  jal 0x800B8C14
    /* 800DBA54 00000000 */  nop
    /* 800DBA58 3C038011 */  lui $v1, 0x8011
    /* 800DBA5C 8C6370FC */  lw $v1, 28924($v1)
    /* 800DBA60 10000141 */  beq $zero, $zero, 0x800DBF68
    /* 800DBA64 24080002 */  addiu $t0, $zero, 2
    /* 800DBA68 3C198015 */  lui $t9, 0x8015
    /* 800DBA6C 8F39698C */  lw $t9, 27020($t9)
    /* 800DBA70 3C048015 */  lui $a0, 0x8015
    /* 800DBA74 00002825 */  or $a1, $zero, $zero
    /* 800DBA78 00197080 */  sll $t6, $t9, 2
    /* 800DBA7C 01D97021 */  addu $t6, $t6, $t9
    /* 800DBA80 000E7080 */  sll $t6, $t6, 2
    /* 800DBA84 01D97023 */  subu $t6, $t6, $t9
    /* 800DBA88 000E7080 */  sll $t6, $t6, 2
    /* 800DBA8C 008E2021 */  addu $a0, $a0, $t6
    /* 800DBA90 8C84A11C */  lw $a0, -24292($a0)
    /* 800DBA94 24060001 */  addiu $a2, $zero, 1
    /* 800DBA98 00003825 */  or $a3, $zero, $zero
    /* 800DBA9C 308F0002 */  andi $t7, $a0, 0x0002
    /* 800DBAA0 51E0007A */  .word 0x51E0007A
    /* 800DBAA4 30990400 */  andi $t9, $a0, 0x0400
    /* 800DBAA8 0C0248D8 */  jal 0x80092360
    /* 800DBAAC 24040025 */  addiu $a0, $zero, 37
    /* 800DBAB0 3C038015 */  lui $v1, 0x8015
    /* 800DBAB4 24639D98 */  addiu $v1, $v1, -25192
    /* 800DBAB8 8C780000 */  lw $t8, 0($v1)
    /* 800DBABC 2F010008 */  sltiu $at, $t8, 8
    /* 800DBAC0 1020006D */  beq $at, $zero, 0x800DBC78
    /* 800DBAC4 0018C080 */  sll $t8, $t8, 2
    /* 800DBAC8 3C018012 */  lui $at, 0x8012
    /* 800DBACC 00380821 */  addu $at, $at, $t8
    /* 800DBAD0 8C3842AC */  lw $t8, 17068($at)
    /* 800DBAD4 03000008 */  jr $t8
    /* 800DBAD8 00000000 */  nop
    /* 800DBADC 0C02D3EC */  jal 0x800B4FB0
    /* 800DBAE0 00002025 */  or $a0, $zero, $zero
    /* 800DBAE4 0C0004F7 */  jal 0x800013DC
    /* 800DBAE8 00000000 */  nop
    /* 800DBAEC 0C03594C */  jal 0x800D6530
    /* 800DBAF0 00000000 */  nop
    /* 800DBAF4 3C048014 */  lui $a0, 0x8014
    /* 800DBAF8 24842728 */  addiu $a0, $a0, 10024
    /* 800DBAFC 00002825 */  or $a1, $zero, $zero
    /* 800DBB00 0C001C9C */  jal 0x80007270
    /* 800DBB04 24060001 */  addiu $a2, $zero, 1
    /* 800DBB08 0C0246C0 */  jal 0x80091B00
    /* 800DBB0C 00000000 */  nop
    /* 800DBB10 AFA200B4 */  sw $v0, 180($sp)
    /* 800DBB14 24190009 */  addiu $t9, $zero, 9
    /* 800DBB18 3C048014 */  lui $a0, 0x8014
    /* 800DBB1C 24842728 */  addiu $a0, $a0, 10024
    /* 800DBB20 A0590002 */  sb $t9, 2($v0)
    /* 800DBB24 00002825 */  or $a1, $zero, $zero
    /* 800DBB28 0C001D78 */  jal 0x800075E0
    /* 800DBB2C 00003025 */  or $a2, $zero, $zero
    /* 800DBB30 3C048014 */  lui $a0, 0x8014
    /* 800DBB34 248427A8 */  addiu $a0, $a0, 10152
    /* 800DBB38 8FA500B4 */  lw $a1, 180($sp)
    /* 800DBB3C 0C001D78 */  jal 0x800075E0
    /* 800DBB40 00003025 */  or $a2, $zero, $zero
    /* 800DBB44 0C035858 */  jal 0x800D6160
    /* 800DBB48 24080001 */  addiu $t0, $zero, 1
    /* 800DBB4C 3C0E8011 */  lui $t6, 0x8011
    /* 800DBB50 8DCE74B4 */  lw $t6, 29876($t6)
    /* 800DBB54 000E7A80 */  sll $t7, $t6, 10
    /* 800DBB58 05E00003 */  bltz $t7, 0x800DBB68
    /* 800DBB5C 00000000 */  nop
    /* 800DBB60 0C035858 */  jal 0x800D6160
    /* 800DBB64 00004025 */  or $t0, $zero, $zero
    /* 800DBB68 0C03582B */  jal 0x800D60AC
    /* 800DBB6C 00000000 */  nop
    /* 800DBB70 2404FFFF */  addiu $a0, $zero, -1
    /* 800DBB74 0C032130 */  jal 0x800C84C0
    /* 800DBB78 00002825 */  or $a1, $zero, $zero
    /* 800DBB7C 10000136 */  beq $zero, $zero, 0x800DC058
    /* 800DBB80 8FBF003C */  lw $ra, 60($sp)
    /* 800DBB84 3C018015 */  lui $at, 0x8015
    /* 800DBB88 C42443CC */  lwc1 $f4, 17356($at)
    /* 800DBB8C 3C014170 */  lui $at, 0x4170
    /* 800DBB90 44813000 */  .word 0x44813000
    /* 800DBB94 3C018011 */  lui $at, 0x8011
    /* 800DBB98 24180001 */  addiu $t8, $zero, 1
    /* 800DBB9C 4606203C */  .word 0x4606203C
    /* 800DBBA0 00000000 */  nop
    /* 800DBBA4 45000003 */  .word 0x45000003
    /* 800DBBA8 00000000 */  nop
    /* 800DBBAC 100000AE */  beq $zero, $zero, 0x800DBE68
    /* 800DBBB0 A0384650 */  sb $t8, 18000($at)
    /* 800DBBB4 0C02D3EC */  jal 0x800B4FB0
    /* 800DBBB8 24040007 */  addiu $a0, $zero, 7
    /* 800DBBBC 1000002E */  beq $zero, $zero, 0x800DBC78
    /* 800DBBC0 00000000 */  nop
    /* 800DBBC4 3C018015 */  lui $at, 0x8015
    /* 800DBBC8 C42843CC */  lwc1 $f8, 17356($at)
    /* 800DBBCC 3C014170 */  lui $at, 0x4170
    /* 800DBBD0 44815000 */  .word 0x44815000
    /* 800DBBD4 3C198011 */  lui $t9, 0x8011
    /* 800DBBD8 460A403C */  .word 0x460A403C
    /* 800DBBDC 00000000 */  nop
    /* 800DBBE0 45000009 */  .word 0x45000009
    /* 800DBBE4 00000000 */  nop
    /* 800DBBE8 8F3974B4 */  lw $t9, 29876($t9)
    /* 800DBBEC 3C018011 */  lui $at, 0x8011
    /* 800DBBF0 A0204650 */  sb $zero, 18000($at)
    /* 800DBBF4 00197240 */  sll $t6, $t9, 9
    /* 800DBBF8 05C1009B */  bgez $t6, 0x800DBE68
    /* 800DBBFC 00000000 */  nop
    /* 800DBC00 10000077 */  beq $zero, $zero, 0x800DBDE0
    /* 800DBC04 24190001 */  addiu $t9, $zero, 1
    /* 800DBC08 0C02D3EC */  jal 0x800B4FB0
    /* 800DBC0C 24040008 */  addiu $a0, $zero, 8
    /* 800DBC10 10000019 */  beq $zero, $zero, 0x800DBC78
    /* 800DBC14 00000000 */  nop
    /* 800DBC18 0C02D3EC */  jal 0x800B4FB0
    /* 800DBC1C 24040002 */  addiu $a0, $zero, 2
    /* 800DBC20 10000015 */  beq $zero, $zero, 0x800DBC78
    /* 800DBC24 00000000 */  nop
    /* 800DBC28 0C02D3EC */  jal 0x800B4FB0
    /* 800DBC2C 24040003 */  addiu $a0, $zero, 3
    /* 800DBC30 10000011 */  beq $zero, $zero, 0x800DBC78
    /* 800DBC34 00000000 */  nop
    /* 800DBC38 0C02D3EC */  jal 0x800B4FB0
    /* 800DBC3C 24040005 */  addiu $a0, $zero, 5
    /* 800DBC40 1000000D */  beq $zero, $zero, 0x800DBC78
    /* 800DBC44 00000000 */  nop
    /* 800DBC48 3C188015 */  lui $t8, 0x8015
    /* 800DBC4C 8F18698C */  lw $t8, 27020($t8)
    /* 800DBC50 3C018015 */  lui $at, 0x8015
    /* 800DBC54 240F0001 */  addiu $t7, $zero, 1
    /* 800DBC58 00380821 */  addu $at, $at, $t8
    /* 800DBC5C A02F418C */  sb $t7, 16780($at)
    /* 800DBC60 0C02D3EC */  jal 0x800B4FB0
    /* 800DBC64 24040004 */  addiu $a0, $zero, 4
    /* 800DBC68 10000003 */  beq $zero, $zero, 0x800DBC78
    /* 800DBC6C 00000000 */  nop
    /* 800DBC70 0C02D3EC */  jal 0x800B4FB0
    /* 800DBC74 24040006 */  addiu $a0, $zero, 6
    /* 800DBC78 3C038011 */  lui $v1, 0x8011
    /* 800DBC7C 8C6370FC */  lw $v1, 28924($v1)
    /* 800DBC80 100000B9 */  beq $zero, $zero, 0x800DBF68
    /* 800DBC84 24080002 */  addiu $t0, $zero, 2
    /* 800DBC88 30990400 */  andi $t9, $a0, 0x0400
    /* 800DBC8C 1320000F */  beq $t9, $zero, 0x800DBCCC
    /* 800DBC90 308E0800 */  andi $t6, $a0, 0x0800
    /* 800DBC94 0C02D7D3 */  jal 0x800B5F4C
    /* 800DBC98 00000000 */  nop
    /* 800DBC9C 3C038015 */  lui $v1, 0x8015
    /* 800DBCA0 24639D98 */  addiu $v1, $v1, -25192
    /* 800DBCA4 8C6E0000 */  lw $t6, 0($v1)
    /* 800DBCA8 24190007 */  addiu $t9, $zero, 7
    /* 800DBCAC 24080002 */  addiu $t0, $zero, 2
    /* 800DBCB0 25CFFFFF */  addiu $t7, $t6, -1
    /* 800DBCB4 05E10002 */  bgez $t7, 0x800DBCC0
    /* 800DBCB8 AC6F0000 */  sw $t7, 0($v1)
    /* 800DBCBC AC790000 */  sw $t9, 0($v1)
    /* 800DBCC0 3C038011 */  lui $v1, 0x8011
    /* 800DBCC4 100000A8 */  beq $zero, $zero, 0x800DBF68
    /* 800DBCC8 8C6370FC */  lw $v1, 28924($v1)
    /* 800DBCCC 51C00010 */  .word 0x51C00010
    /* 800DBCD0 308E0005 */  andi $t6, $a0, 0x0005
    /* 800DBCD4 0C02D7D3 */  jal 0x800B5F4C
    /* 800DBCD8 00000000 */  nop
    /* 800DBCDC 3C038015 */  lui $v1, 0x8015
    /* 800DBCE0 24639D98 */  addiu $v1, $v1, -25192
    /* 800DBCE4 8C6F0000 */  lw $t7, 0($v1)
    /* 800DBCE8 24080002 */  addiu $t0, $zero, 2
    /* 800DBCEC 25F80001 */  addiu $t8, $t7, 1
    /* 800DBCF0 2B010008 */  slti $at, $t8, 8
    /* 800DBCF4 14200002 */  bne $at, $zero, 0x800DBD00
    /* 800DBCF8 AC780000 */  sw $t8, 0($v1)
    /* 800DBCFC AC600000 */  sw $zero, 0($v1)
    /* 800DBD00 3C038011 */  lui $v1, 0x8011
    /* 800DBD04 10000098 */  beq $zero, $zero, 0x800DBF68
    /* 800DBD08 8C6370FC */  lw $v1, 28924($v1)
    /* 800DBD0C 308E0005 */  andi $t6, $a0, 0x0005
    /* 800DBD10 11C00005 */  beq $t6, $zero, 0x800DBD28
    /* 800DBD14 00000000 */  nop
    /* 800DBD18 0C02D7F1 */  jal 0x800B5FC4
    /* 800DBD1C 00000000 */  nop
    /* 800DBD20 1000FF6E */  beq $zero, $zero, 0x800DBADC
    /* 800DBD24 00000000 */  nop
    /* 800DBD28 1000008F */  beq $zero, $zero, 0x800DBF68
    /* 800DBD2C 24080002 */  addiu $t0, $zero, 2
    /* 800DBD30 3C0F8015 */  lui $t7, 0x8015
    /* 800DBD34 8DEF698C */  lw $t7, 27020($t7)
    /* 800DBD38 3C048015 */  lui $a0, 0x8015
    /* 800DBD3C 3C038011 */  lui $v1, 0x8011
    /* 800DBD40 000FC080 */  sll $t8, $t7, 2
    /* 800DBD44 030FC021 */  addu $t8, $t8, $t7
    /* 800DBD48 0018C080 */  sll $t8, $t8, 2
    /* 800DBD4C 030FC023 */  subu $t8, $t8, $t7
    /* 800DBD50 0018C080 */  sll $t8, $t8, 2
    /* 800DBD54 00982021 */  addu $a0, $a0, $t8
    /* 800DBD58 8C84A11C */  lw $a0, -24292($a0)
    /* 800DBD5C 24637354 */  addiu $v1, $v1, 29524
    /* 800DBD60 30993000 */  andi $t9, $a0, 0x3000
    /* 800DBD64 53200010 */  .word 0x53200010
    /* 800DBD68 30990002 */  andi $t9, $a0, 0x0002
    /* 800DBD6C 80620000 */  lb $v0, 0($v1)
    /* 800DBD70 2C4E0001 */  sltiu $t6, $v0, 1
    /* 800DBD74 0C02D7E2 */  jal 0x800B5F88
    /* 800DBD78 A06E0000 */  sb $t6, 0($v1)
    /* 800DBD7C 3C0F8015 */  lui $t7, 0x8015
    /* 800DBD80 8DEF698C */  lw $t7, 27020($t7)
    /* 800DBD84 3C048015 */  lui $a0, 0x8015
    /* 800DBD88 000FC080 */  sll $t8, $t7, 2
    /* 800DBD8C 030FC021 */  addu $t8, $t8, $t7
    /* 800DBD90 0018C080 */  sll $t8, $t8, 2
    /* 800DBD94 030FC023 */  subu $t8, $t8, $t7
    /* 800DBD98 0018C080 */  sll $t8, $t8, 2
    /* 800DBD9C 00982021 */  addu $a0, $a0, $t8
    /* 800DBDA0 8C84A11C */  lw $a0, -24292($a0)
    /* 800DBDA4 30990002 */  andi $t9, $a0, 0x0002
    /* 800DBDA8 1320004F */  beq $t9, $zero, 0x800DBEE8
    /* 800DBDAC 3C0E8011 */  lui $t6, 0x8011
    /* 800DBDB0 81CE7354 */  lb $t6, 29524($t6)
    /* 800DBDB4 3C0F8011 */  lui $t7, 0x8011
    /* 800DBDB8 11C00047 */  beq $t6, $zero, 0x800DBED8
    /* 800DBDBC 00000000 */  nop
    /* 800DBDC0 8DEF74B4 */  lw $t7, 29876($t7)
    /* 800DBDC4 3C038011 */  lui $v1, 0x8011
    /* 800DBDC8 8C6370FC */  lw $v1, 28924($v1)
    /* 800DBDCC 000FC240 */  sll $t8, $t7, 9
    /* 800DBDD0 07010021 */  bgez $t8, 0x800DBE58
    /* 800DBDD4 24010008 */  addiu $at, $zero, 8
    /* 800DBDD8 1461001F */  bne $v1, $at, 0x800DBE58
    /* 800DBDDC 24190001 */  addiu $t9, $zero, 1
    /* 800DBDE0 3C018014 */  lui $at, 0x8014
    /* 800DBDE4 A0392699 */  sb $t9, 9881($at)
    /* 800DBDE8 0C02D3EC */  jal 0x800B4FB0
    /* 800DBDEC 00002025 */  or $a0, $zero, $zero
    /* 800DBDF0 0C0004F7 */  jal 0x800013DC
    /* 800DBDF4 00000000 */  nop
    /* 800DBDF8 3C048014 */  lui $a0, 0x8014
    /* 800DBDFC 24842728 */  addiu $a0, $a0, 10024
    /* 800DBE00 00002825 */  or $a1, $zero, $zero
    /* 800DBE04 0C001C9C */  jal 0x80007270
    /* 800DBE08 24060001 */  addiu $a2, $zero, 1
    /* 800DBE0C 0C0246C0 */  jal 0x80091B00
    /* 800DBE10 00000000 */  nop
    /* 800DBE14 AFA20084 */  sw $v0, 132($sp)
    /* 800DBE18 240E0007 */  addiu $t6, $zero, 7
    /* 800DBE1C 3C048014 */  lui $a0, 0x8014
    /* 800DBE20 24842728 */  addiu $a0, $a0, 10024
    /* 800DBE24 A04E0002 */  sb $t6, 2($v0)
    /* 800DBE28 00002825 */  or $a1, $zero, $zero
    /* 800DBE2C 0C001D78 */  jal 0x800075E0
    /* 800DBE30 00003025 */  or $a2, $zero, $zero
    /* 800DBE34 3C048014 */  lui $a0, 0x8014
    /* 800DBE38 248427A8 */  addiu $a0, $a0, 10152
    /* 800DBE3C 8FA50084 */  lw $a1, 132($sp)
    /* 800DBE40 0C001D78 */  jal 0x800075E0
    /* 800DBE44 00003025 */  or $a2, $zero, $zero
    /* 800DBE48 1040002E */  beq $v0, $zero, 0x800DBF04
    /* 800DBE4C 00000000 */  nop
    /* 800DBE50 1000002C */  beq $zero, $zero, 0x800DBF04
    /* 800DBE54 00000000 */  nop
    /* 800DBE58 386F0007 */  xori $t7, $v1, 0x0007
    /* 800DBE5C 2DEF0001 */  sltiu $t7, $t7, 1
    /* 800DBE60 3C018011 */  lui $at, 0x8011
    /* 800DBE64 A02F4650 */  sb $t7, 18000($at)
    /* 800DBE68 0C02D3EC */  jal 0x800B4FB0
    /* 800DBE6C 00002025 */  or $a0, $zero, $zero
    /* 800DBE70 0C0004F7 */  jal 0x800013DC
    /* 800DBE74 00000000 */  nop
    /* 800DBE78 3C048014 */  lui $a0, 0x8014
    /* 800DBE7C 24842728 */  addiu $a0, $a0, 10024
    /* 800DBE80 00002825 */  or $a1, $zero, $zero
    /* 800DBE84 0C001C9C */  jal 0x80007270
    /* 800DBE88 24060001 */  addiu $a2, $zero, 1
    /* 800DBE8C 0C0246C0 */  jal 0x80091B00
    /* 800DBE90 00000000 */  nop
    /* 800DBE94 AFA20064 */  sw $v0, 100($sp)
    /* 800DBE98 24180007 */  addiu $t8, $zero, 7
    /* 800DBE9C 3C048014 */  lui $a0, 0x8014
    /* 800DBEA0 24842728 */  addiu $a0, $a0, 10024
    /* 800DBEA4 A0580002 */  sb $t8, 2($v0)
    /* 800DBEA8 00002825 */  or $a1, $zero, $zero
    /* 800DBEAC 0C001D78 */  jal 0x800075E0
    /* 800DBEB0 00003025 */  or $a2, $zero, $zero
    /* 800DBEB4 3C048014 */  lui $a0, 0x8014
    /* 800DBEB8 248427A8 */  addiu $a0, $a0, 10152
    /* 800DBEBC 8FA50064 */  lw $a1, 100($sp)
    /* 800DBEC0 0C001D78 */  jal 0x800075E0
    /* 800DBEC4 00003025 */  or $a2, $zero, $zero
    /* 800DBEC8 0C035681 */  jal 0x800D5A04
    /* 800DBECC 00000000 */  nop
    /* 800DBED0 1000000C */  beq $zero, $zero, 0x800DBF04
    /* 800DBED4 00000000 */  nop
    /* 800DBED8 0C02D3EC */  jal 0x800B4FB0
    /* 800DBEDC 24040001 */  addiu $a0, $zero, 1
    /* 800DBEE0 10000008 */  beq $zero, $zero, 0x800DBF04
    /* 800DBEE4 00000000 */  nop
    /* 800DBEE8 30990005 */  andi $t9, $a0, 0x0005
    /* 800DBEEC 13200005 */  beq $t9, $zero, 0x800DBF04
    /* 800DBEF0 00000000 */  nop
    /* 800DBEF4 0C02D7F1 */  jal 0x800B5FC4
    /* 800DBEF8 00000000 */  nop
    /* 800DBEFC 0C02D3EC */  jal 0x800B4FB0
    /* 800DBF00 24040001 */  addiu $a0, $zero, 1
    /* 800DBF04 3C038011 */  lui $v1, 0x8011
    /* 800DBF08 8C6370FC */  lw $v1, 28924($v1)
    /* 800DBF0C 10000016 */  beq $zero, $zero, 0x800DBF68
    /* 800DBF10 24080002 */  addiu $t0, $zero, 2
    /* 800DBF14 3C0E8015 */  lui $t6, 0x8015
    /* 800DBF18 8DCE698C */  lw $t6, 27020($t6)
    /* 800DBF1C 3C048015 */  lui $a0, 0x8015
    /* 800DBF20 000E7880 */  sll $t7, $t6, 2
    /* 800DBF24 01EE7821 */  addu $t7, $t7, $t6
    /* 800DBF28 000F7880 */  sll $t7, $t7, 2
    /* 800DBF2C 01EE7823 */  subu $t7, $t7, $t6
    /* 800DBF30 000F7880 */  sll $t7, $t7, 2
    /* 800DBF34 008F2021 */  addu $a0, $a0, $t7
    /* 800DBF38 8C84A11C */  lw $a0, -24292($a0)
    /* 800DBF3C 30980007 */  andi $t8, $a0, 0x0007
    /* 800DBF40 13000007 */  beq $t8, $zero, 0x800DBF60
    /* 800DBF44 00000000 */  nop
    /* 800DBF48 0C02D7F1 */  jal 0x800B5FC4
    /* 800DBF4C 00000000 */  nop
    /* 800DBF50 0C02D3EC */  jal 0x800B4FB0
    /* 800DBF54 24040001 */  addiu $a0, $zero, 1
    /* 800DBF58 3C038011 */  lui $v1, 0x8011
    /* 800DBF5C 8C6370FC */  lw $v1, 28924($v1)
    /* 800DBF60 10000001 */  beq $zero, $zero, 0x800DBF68
    /* 800DBF64 24080002 */  addiu $t0, $zero, 2
    /* 800DBF68 1060001B */  beq $v1, $zero, 0x800DBFD8
    /* 800DBF6C 3C198015 */  lui $t9, 0x8015
    /* 800DBF70 8F39698C */  lw $t9, 27020($t9)
    /* 800DBF74 3C0F8015 */  lui $t7, 0x8015
    /* 800DBF78 25EF6CF0 */  addiu $t7, $t7, 27888
    /* 800DBF7C 00197100 */  sll $t6, $t9, 4
    /* 800DBF80 01CF2021 */  addu $a0, $t6, $t7
    /* 800DBF84 80820000 */  lb $v0, 0($a0)
    /* 800DBF88 3C198011 */  lui $t9, 0x8011
    /* 800DBF8C 83397350 */  lb $t9, 29520($t9)
    /* 800DBF90 2C580001 */  sltiu $t8, $v0, 1
    /* 800DBF94 13190010 */  beq $t8, $t9, 0x800DBFD8
    /* 800DBF98 00000000 */  nop
    /* 800DBF9C 80820000 */  lb $v0, 0($a0)
    /* 800DBFA0 3C018011 */  lui $at, 0x8011
    /* 800DBFA4 2C4E0001 */  sltiu $t6, $v0, 1
    /* 800DBFA8 A02E7350 */  sb $t6, 29520($at)
    /* 800DBFAC 24010005 */  addiu $at, $zero, 5
    /* 800DBFB0 10610009 */  beq $v1, $at, 0x800DBFD8
    /* 800DBFB4 24010004 */  addiu $at, $zero, 4
    /* 800DBFB8 10610007 */  beq $v1, $at, 0x800DBFD8
    /* 800DBFBC 00000000 */  nop
    /* 800DBFC0 11030005 */  beq $t0, $v1, 0x800DBFD8
    /* 800DBFC4 24010003 */  addiu $at, $zero, 3
    /* 800DBFC8 10610003 */  beq $v1, $at, 0x800DBFD8
    /* 800DBFCC 00000000 */  nop
    /* 800DBFD0 0C02D0E3 */  jal 0x800B438C
    /* 800DBFD4 00000000 */  nop
    /* 800DBFD8 3C0F8015 */  lui $t7, 0x8015
    /* 800DBFDC 81EF7244 */  lb $t7, 29252($t7)
    /* 800DBFE0 11E0001C */  beq $t7, $zero, 0x800DC054
    /* 800DBFE4 3C018011 */  lui $at, 0x8011
    /* 800DBFE8 A0204650 */  sb $zero, 18000($at)
