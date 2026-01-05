# Source: game_code.bin (decompressed)
# Address: 0x800BBC00

glabel collision_point_in_body
    /* 800BBC00 0C025DE6 */  jal 0x80097798
    /* 800BBC04 AFA00010 */  sw $zero, 16($sp)
    /* 800BBC08 3C098014 */  lui $t1, 0x8014
    /* 800BBC0C 25293A10 */  addiu $t1, $t1, 14864
    /* 800BBC10 24190001 */  addiu $t9, $zero, 1
    /* 800BBC14 AD220018 */  sw $v0, 24($t1)
    /* 800BBC18 A1390014 */  sb $t9, 20($t1)
    /* 800BBC1C 8D270018 */  lw $a3, 24($t1)
    /* 800BBC20 00002825 */  or $a1, $zero, $zero
    /* 800BBC24 00003025 */  or $a2, $zero, $zero
    /* 800BBC28 0C0258A2 */  jal 0x80096288
    /* 800BBC2C 00E02025 */  or $a0, $a3, $zero
    /* 800BBC30 00077080 */  sll $t6, $a3, 2
    /* 800BBC34 01C77021 */  addu $t6, $t6, $a3
    /* 800BBC38 000E7080 */  sll $t6, $t6, 2
    /* 800BBC3C 3C028015 */  lui $v0, 0x8015
    /* 800BBC40 004E1021 */  addu $v0, $v0, $t6
    /* 800BBC44 80426D39 */  lb $v0, 27961($v0)
    /* 800BBC48 240F0001 */  addiu $t7, $zero, 1
    /* 800BBC4C 54400004 */  .word 0x54400004
    /* 800BBC50 81390017 */  lb $t9, 23($t1)
    /* 800BBC54 10000160 */  beq $zero, $zero, 0x800BC1D8
    /* 800BBC58 00001025 */  or $v0, $zero, $zero
    /* 800BBC5C 81390017 */  lb $t9, 23($t1)
    /* 800BBC60 24180001 */  addiu $t8, $zero, 1
    /* 800BBC64 A12F0015 */  sb $t7, 21($t1)
    /* 800BBC68 17200010 */  bne $t9, $zero, 0x800BBCAC
    /* 800BBC6C A1380016 */  sb $t8, 22($t1)
    /* 800BBC70 8D270018 */  lw $a3, 24($t1)
    /* 800BBC74 00002825 */  or $a1, $zero, $zero
    /* 800BBC78 00003025 */  or $a2, $zero, $zero
    /* 800BBC7C 0C0258A2 */  jal 0x80096288
    /* 800BBC80 00E02025 */  or $a0, $a3, $zero
    /* 800BBC84 00077080 */  sll $t6, $a3, 2
    /* 800BBC88 01C77021 */  addu $t6, $t6, $a3
    /* 800BBC8C 000E7080 */  sll $t6, $t6, 2
    /* 800BBC90 3C028015 */  lui $v0, 0x8015
    /* 800BBC94 004E1021 */  addu $v0, $v0, $t6
    /* 800BBC98 8C426D44 */  lw $v0, 27972($v0)
    /* 800BBC9C 3C018015 */  lui $at, 0x8015
    /* 800BBCA0 240F0001 */  addiu $t7, $zero, 1
    /* 800BBCA4 AC2226DC */  sw $v0, 9948($at)
    /* 800BBCA8 A12F0017 */  sb $t7, 23($t1)
    /* 800BBCAC 8138001C */  lb $t8, 28($t1)
    /* 800BBCB0 3C198015 */  lui $t9, 0x8015
    /* 800BBCB4 8FAE00A8 */  lw $t6, 168($sp)
    /* 800BBCB8 17000010 */  bne $t8, $zero, 0x800BBCFC
    /* 800BBCBC 00002825 */  or $a1, $zero, $zero
    /* 800BBCC0 83392570 */  lb $t9, 9584($t9)
    /* 800BBCC4 8FA600AC */  lw $a2, 172($sp)
    /* 800BBCC8 00003825 */  or $a3, $zero, $zero
    /* 800BBCCC 13200003 */  beq $t9, $zero, 0x800BBCDC
    /* 800BBCD0 2408009E */  addiu $t0, $zero, 158
    /* 800BBCD4 10000001 */  beq $zero, $zero, 0x800BBCDC
    /* 800BBCD8 240800B1 */  addiu $t0, $zero, 177
    /* 800BBCDC 010E2021 */  addu $a0, $t0, $t6
    /* 800BBCE0 0C025DE6 */  jal 0x80097798
    /* 800BBCE4 AFA00010 */  sw $zero, 16($sp)
    /* 800BBCE8 3C098014 */  lui $t1, 0x8014
    /* 800BBCEC 25293A10 */  addiu $t1, $t1, 14864
    /* 800BBCF0 240F0001 */  addiu $t7, $zero, 1
    /* 800BBCF4 AD220020 */  sw $v0, 32($t1)
    /* 800BBCF8 A12F001C */  sb $t7, 28($t1)
    /* 800BBCFC 8D270020 */  lw $a3, 32($t1)
    /* 800BBD00 00002825 */  or $a1, $zero, $zero
    /* 800BBD04 00003025 */  or $a2, $zero, $zero
    /* 800BBD08 0C0258A2 */  jal 0x80096288
    /* 800BBD0C 00E02025 */  or $a0, $a3, $zero
    /* 800BBD10 0007C080 */  sll $t8, $a3, 2
    /* 800BBD14 0307C021 */  addu $t8, $t8, $a3
    /* 800BBD18 0018C080 */  sll $t8, $t8, 2
    /* 800BBD1C 3C028015 */  lui $v0, 0x8015
    /* 800BBD20 00581021 */  addu $v0, $v0, $t8
    /* 800BBD24 80426D39 */  lb $v0, 27961($v0)
    /* 800BBD28 54400004 */  .word 0x54400004
    /* 800BBD2C 8139001F */  lb $t9, 31($t1)
    /* 800BBD30 10000129 */  beq $zero, $zero, 0x800BC1D8
    /* 800BBD34 00001025 */  or $v0, $zero, $zero
    /* 800BBD38 8139001F */  lb $t9, 31($t1)
    /* 800BBD3C 24080001 */  addiu $t0, $zero, 1
    /* 800BBD40 A128001D */  sb $t0, 29($t1)
    /* 800BBD44 17200011 */  bne $t9, $zero, 0x800BBD8C
    /* 800BBD48 A128001E */  sb $t0, 30($t1)
    /* 800BBD4C 8D270020 */  lw $a3, 32($t1)
    /* 800BBD50 00002825 */  or $a1, $zero, $zero
    /* 800BBD54 00003025 */  or $a2, $zero, $zero
    /* 800BBD58 0C0258A2 */  jal 0x80096288
    /* 800BBD5C 00E02025 */  or $a0, $a3, $zero
    /* 800BBD60 00077080 */  sll $t6, $a3, 2
    /* 800BBD64 01C77021 */  addu $t6, $t6, $a3
    /* 800BBD68 000E7080 */  sll $t6, $t6, 2
    /* 800BBD6C 3C028015 */  lui $v0, 0x8015
    /* 800BBD70 004E1021 */  addu $v0, $v0, $t6
    /* 800BBD74 8C426D44 */  lw $v0, 27972($v0)
    /* 800BBD78 3C018015 */  lui $at, 0x8015
    /* 800BBD7C 240F0001 */  addiu $t7, $zero, 1
    /* 800BBD80 AC223F20 */  sw $v0, 16160($at)
    /* 800BBD84 A12F001F */  sb $t7, 31($t1)
    /* 800BBD88 24080001 */  addiu $t0, $zero, 1
    /* 800BBD8C 81380024 */  lb $t8, 36($t1)
    /* 800BBD90 3C038011 */  lui $v1, 0x8011
    /* 800BBD94 570000B2 */  .word 0x570000B2
    /* 800BBD98 812E0000 */  lb $t6, 0($t1)
    /* 800BBD9C A1280024 */  sb $t0, 36($t1)
    /* 800BBDA0 8C6374B4 */  lw $v1, 29876($v1)
    /* 800BBDA4 3C0E8015 */  lui $t6, 0x8015
    /* 800BBDA8 30790100 */  andi $t9, $v1, 0x0100
    /* 800BBDAC 572000A8 */  .word 0x572000A8
    /* 800BBDB0 24080001 */  addiu $t0, $zero, 1
    /* 800BBDB4 8DCEA110 */  lw $t6, -24304($t6)
    /* 800BBDB8 24010006 */  addiu $at, $zero, 6
    /* 800BBDBC 8FAF00A8 */  lw $t7, 168($sp)
    /* 800BBDC0 11C10005 */  beq $t6, $at, 0x800BBDD8
    /* 800BBDC4 2404004C */  addiu $a0, $zero, 76
    /* 800BBDC8 29E10006 */  slti $at, $t7, 6
    /* 800BBDCC 14200009 */  bne $at, $zero, 0x800BBDF4
    /* 800BBDD0 29E1000E */  slti $at, $t7, 14
    /* 800BBDD4 10200007 */  beq $at, $zero, 0x800BBDF4
    /* 800BBDD8 00002825 */  or $a1, $zero, $zero
    /* 800BBDDC 00003025 */  or $a2, $zero, $zero
    /* 800BBDE0 00003825 */  or $a3, $zero, $zero
    /* 800BBDE4 0C025DE6 */  jal 0x80097798
    /* 800BBDE8 AFA00010 */  sw $zero, 16($sp)
    /* 800BBDEC 3C038011 */  lui $v1, 0x8011
    /* 800BBDF0 8C6374B4 */  lw $v1, 29876($v1)
    /* 800BBDF4 30620008 */  andi $v0, $v1, 0x0008
    /* 800BBDF8 10400005 */  beq $v0, $zero, 0x800BBE10
    /* 800BBDFC 24040044 */  addiu $a0, $zero, 68
    /* 800BBE00 10400036 */  beq $v0, $zero, 0x800BBEDC
    /* 800BBE04 3C188015 */  lui $t8, 0x8015
    /* 800BBE08 83186994 */  lb $t8, 27028($t8)
    /* 800BBE0C 13000033 */  beq $t8, $zero, 0x800BBEDC
    /* 800BBE10 00002825 */  or $a1, $zero, $zero
    /* 800BBE14 00003025 */  or $a2, $zero, $zero
    /* 800BBE18 00003825 */  or $a3, $zero, $zero
    /* 800BBE1C 0C025DE6 */  jal 0x80097798
    /* 800BBE20 AFA00010 */  sw $zero, 16($sp)
    /* 800BBE24 8FB900A8 */  lw $t9, 168($sp)
    /* 800BBE28 24010012 */  addiu $at, $zero, 18
    /* 800BBE2C 2404004E */  addiu $a0, $zero, 78
    /* 800BBE30 17210008 */  bne $t9, $at, 0x800BBE54
    /* 800BBE34 8FAE00A8 */  lw $t6, 168($sp)
    /* 800BBE38 00002825 */  or $a1, $zero, $zero
    /* 800BBE3C 00003025 */  or $a2, $zero, $zero
    /* 800BBE40 00003825 */  or $a3, $zero, $zero
    /* 800BBE44 0C025DE6 */  jal 0x80097798
    /* 800BBE48 AFA00010 */  sw $zero, 16($sp)
    /* 800BBE4C 1000001E */  beq $zero, $zero, 0x800BBEC8
    /* 800BBE50 2404004D */  addiu $a0, $zero, 77
    /* 800BBE54 05C0001B */  bltz $t6, 0x800BBEC4
    /* 800BBE58 29C10006 */  slti $at, $t6, 6
    /* 800BBE5C 10200019 */  beq $at, $zero, 0x800BBEC4
    /* 800BBE60 3C0F8015 */  lui $t7, 0x8015
    /* 800BBE64 81EF6994 */  lb $t7, 27028($t7)
    /* 800BBE68 2404004F */  addiu $a0, $zero, 79
    /* 800BBE6C 00002825 */  or $a1, $zero, $zero
    /* 800BBE70 11E00011 */  beq $t7, $zero, 0x800BBEB8
    /* 800BBE74 00003025 */  or $a2, $zero, $zero
    /* 800BBE78 2404004E */  addiu $a0, $zero, 78
    /* 800BBE7C 00002825 */  or $a1, $zero, $zero
    /* 800BBE80 00003025 */  or $a2, $zero, $zero
    /* 800BBE84 00003825 */  or $a3, $zero, $zero
    /* 800BBE88 0C025DE6 */  jal 0x80097798
    /* 800BBE8C AFA00010 */  sw $zero, 16($sp)
    /* 800BBE90 3C048015 */  lui $a0, 0x8015
    /* 800BBE94 8084978C */  lb $a0, -26740($a0)
    /* 800BBE98 00002825 */  or $a1, $zero, $zero
    /* 800BBE9C 00003025 */  or $a2, $zero, $zero
    /* 800BBEA0 00003825 */  or $a3, $zero, $zero
    /* 800BBEA4 AFA00010 */  sw $zero, 16($sp)
    /* 800BBEA8 0C025DE6 */  jal 0x80097798
    /* 800BBEAC 24840052 */  addiu $a0, $a0, 82
    /* 800BBEB0 10000005 */  beq $zero, $zero, 0x800BBEC8
    /* 800BBEB4 2404004D */  addiu $a0, $zero, 77
    /* 800BBEB8 00003825 */  or $a3, $zero, $zero
    /* 800BBEBC 0C025DE6 */  jal 0x80097798
    /* 800BBEC0 AFA00010 */  sw $zero, 16($sp)
    /* 800BBEC4 2404004D */  addiu $a0, $zero, 77
    /* 800BBEC8 00002825 */  or $a1, $zero, $zero
    /* 800BBECC 00003025 */  or $a2, $zero, $zero
    /* 800BBED0 00003825 */  or $a3, $zero, $zero
    /* 800BBED4 0C025DE6 */  jal 0x80097798
    /* 800BBED8 AFA00010 */  sw $zero, 16($sp)
    /* 800BBEDC 2404003E */  addiu $a0, $zero, 62
    /* 800BBEE0 00002825 */  or $a1, $zero, $zero
    /* 800BBEE4 00003025 */  or $a2, $zero, $zero
    /* 800BBEE8 00003825 */  or $a3, $zero, $zero
    /* 800BBEEC 0C025DE6 */  jal 0x80097798
    /* 800BBEF0 AFA00010 */  sw $zero, 16($sp)
    /* 800BBEF4 3C188015 */  lui $t8, 0x8015
    /* 800BBEF8 83186994 */  lb $t8, 27028($t8)
    /* 800BBEFC 3C198015 */  lui $t9, 0x8015
    /* 800BBF00 24040050 */  addiu $a0, $zero, 80
    /* 800BBF04 17000007 */  bne $t8, $zero, 0x800BBF24
    /* 800BBF08 00002825 */  or $a1, $zero, $zero
    /* 800BBF0C 8339978C */  lb $t9, -26740($t9)
    /* 800BBF10 00003025 */  or $a2, $zero, $zero
    /* 800BBF14 00003825 */  or $a3, $zero, $zero
    /* 800BBF18 2B210006 */  slti $at, $t9, 6
    /* 800BBF1C 5420000E */  .word 0x5420000E
    /* 800BBF20 24040051 */  addiu $a0, $zero, 81
    /* 800BBF24 00003025 */  or $a2, $zero, $zero
    /* 800BBF28 00003825 */  or $a3, $zero, $zero
    /* 800BBF2C 0C025DE6 */  jal 0x80097798
    /* 800BBF30 AFA00010 */  sw $zero, 16($sp)
    /* 800BBF34 2404003D */  addiu $a0, $zero, 61
    /* 800BBF38 00002825 */  or $a1, $zero, $zero
    /* 800BBF3C 00003025 */  or $a2, $zero, $zero
    /* 800BBF40 00003825 */  or $a3, $zero, $zero
    /* 800BBF44 0C025DE6 */  jal 0x80097798
    /* 800BBF48 AFA00010 */  sw $zero, 16($sp)
    /* 800BBF4C 10000006 */  beq $zero, $zero, 0x800BBF68
    /* 800BBF50 24040040 */  addiu $a0, $zero, 64
    /* 800BBF54 24040051 */  addiu $a0, $zero, 81
    /* 800BBF58 00002825 */  or $a1, $zero, $zero
    /* 800BBF5C 0C025DE6 */  jal 0x80097798
    /* 800BBF60 AFA00010 */  sw $zero, 16($sp)
    /* 800BBF64 24040040 */  addiu $a0, $zero, 64
    /* 800BBF68 00002825 */  or $a1, $zero, $zero
    /* 800BBF6C 00003025 */  or $a2, $zero, $zero
    /* 800BBF70 00003825 */  or $a3, $zero, $zero
    /* 800BBF74 0C025DE6 */  jal 0x80097798
    /* 800BBF78 AFA00010 */  sw $zero, 16($sp)
    /* 800BBF7C 3C0E8015 */  lui $t6, 0x8015
    /* 800BBF80 85CE1AD0 */  lh $t6, 6864($t6)
    /* 800BBF84 3C098014 */  lui $t1, 0x8014
    /* 800BBF88 25293A10 */  addiu $t1, $t1, 14864
    /* 800BBF8C 29C10002 */  slti $at, $t6, 2
    /* 800BBF90 14200017 */  bne $at, $zero, 0x800BBFF0
    /* 800BBF94 24040042 */  addiu $a0, $zero, 66
    /* 800BBF98 00002825 */  or $a1, $zero, $zero
    /* 800BBF9C 00003025 */  or $a2, $zero, $zero
    /* 800BBFA0 00003825 */  or $a3, $zero, $zero
    /* 800BBFA4 0C025DE6 */  jal 0x80097798
    /* 800BBFA8 AFA00010 */  sw $zero, 16($sp)
    /* 800BBFAC 3C0F8015 */  lui $t7, 0x8015
    /* 800BBFB0 81EF6994 */  lb $t7, 27028($t7)
    /* 800BBFB4 3C098014 */  lui $t1, 0x8014
    /* 800BBFB8 25293A10 */  addiu $t1, $t1, 14864
    /* 800BBFBC 15E00004 */  bne $t7, $zero, 0x800BBFD0
    /* 800BBFC0 3C188015 */  lui $t8, 0x8015
    /* 800BBFC4 8318978C */  lb $t8, -26740($t8)
    /* 800BBFC8 2B010006 */  slti $at, $t8, 6
    /* 800BBFCC 14200008 */  bne $at, $zero, 0x800BBFF0
    /* 800BBFD0 24040041 */  addiu $a0, $zero, 65
    /* 800BBFD4 00002825 */  or $a1, $zero, $zero
    /* 800BBFD8 00003025 */  or $a2, $zero, $zero
    /* 800BBFDC 00003825 */  or $a3, $zero, $zero
    /* 800BBFE0 0C025DE6 */  jal 0x80097798
    /* 800BBFE4 AFA00010 */  sw $zero, 16($sp)
    /* 800BBFE8 3C098014 */  lui $t1, 0x8014
    /* 800BBFEC 25293A10 */  addiu $t1, $t1, 14864
    /* 800BBFF0 3C028015 */  lui $v0, 0x8015
    /* 800BBFF4 8C42A110 */  lw $v0, -24304($v0)
    /* 800BBFF8 24010006 */  addiu $at, $zero, 6
    /* 800BBFFC 2404003F */  addiu $a0, $zero, 63
    /* 800BC000 14410008 */  bne $v0, $at, 0x800BC024
    /* 800BC004 00002825 */  or $a1, $zero, $zero
    /* 800BC008 00003025 */  or $a2, $zero, $zero
    /* 800BC00C 00003825 */  or $a3, $zero, $zero
    /* 800BC010 0C025DE6 */  jal 0x80097798
    /* 800BC014 AFA00010 */  sw $zero, 16($sp)
    /* 800BC018 3C098014 */  lui $t1, 0x8014
    /* 800BC01C 1000000B */  beq $zero, $zero, 0x800BC04C
    /* 800BC020 25293A10 */  addiu $t1, $t1, 14864
    /* 800BC024 24010004 */  addiu $at, $zero, 4
    /* 800BC028 14410008 */  bne $v0, $at, 0x800BC04C
    /* 800BC02C 24040043 */  addiu $a0, $zero, 67
    /* 800BC030 00002825 */  or $a1, $zero, $zero
    /* 800BC034 00003025 */  or $a2, $zero, $zero
    /* 800BC038 00003825 */  or $a3, $zero, $zero
    /* 800BC03C 0C025DE6 */  jal 0x80097798
    /* 800BC040 AFA00010 */  sw $zero, 16($sp)
    /* 800BC044 3C098014 */  lui $t1, 0x8014
    /* 800BC048 25293A10 */  addiu $t1, $t1, 14864
    /* 800BC04C 24080001 */  addiu $t0, $zero, 1
    /* 800BC050 A1280025 */  sb $t0, 37($t1)
    /* 800BC054 A1280027 */  sb $t0, 39($t1)
    /* 800BC058 A1280026 */  sb $t0, 38($t1)
    /* 800BC05C 812E0000 */  lb $t6, 0($t1)
    /* 800BC060 51C00004 */  .word 0x51C00004
    /* 800BC064 A1280000 */  sb $t0, 0($t1)
    /* 800BC068 1000005B */  beq $zero, $zero, 0x800BC1D8
    /* 800BC06C 24020001 */  addiu $v0, $zero, 1
    /* 800BC070 A1280000 */  sb $t0, 0($t1)
    /* 800BC074 0C02EE0D */  jal 0x800BB834
    /* 800BC078 87A400AA */  lh $a0, 170($sp)
    /* 800BC07C 0C02EDA7 */  jal 0x800BB69C
    /* 800BC080 00000000 */  nop
    /* 800BC084 8FAF00A8 */  lw $t7, 168($sp)
    /* 800BC088 3C198012 */  lui $t9, 0x8012
    /* 800BC08C 2739A924 */  addiu $t9, $t9, -22236
    /* 800BC090 000FC080 */  sll $t8, $t7, 2
    /* 800BC094 03191021 */  addu $v0, $t8, $t9
    /* 800BC098 8C450000 */  lw $a1, 0($v0)
    /* 800BC09C 240EFFFF */  addiu $t6, $zero, -1
    /* 800BC0A0 3C018015 */  lui $at, 0x8015
    /* 800BC0A4 A42E9D90 */  sh $t6, -25200($at)
    /* 800BC0A8 AFA20020 */  sw $v0, 32($sp)
    /* 800BC0AC 0C0291CF */  jal 0x800A473C
    /* 800BC0B0 27A40034 */  addiu $a0, $sp, 52
    /* 800BC0B4 3C0F8015 */  lui $t7, 0x8015
    /* 800BC0B8 8DEF97FC */  lw $t7, -26628($t7)
    /* 800BC0BC 3C188009 */  lui $t8, 0x8009
    /* 800BC0C0 2718AD48 */  addiu $t8, $t8, -21176
    /* 800BC0C4 3C058015 */  lui $a1, 0x8015
    /* 800BC0C8 8DE60000 */  lw $a2, 0($t7)
    /* 800BC0CC AFB80010 */  sw $t8, 16($sp)
    /* 800BC0D0 8CA59818 */  lw $a1, -26600($a1)
    /* 800BC0D4 27A40030 */  addiu $a0, $sp, 48
    /* 800BC0D8 0C024B16 */  jal 0x80092C58
    /* 800BC0DC 24070014 */  addiu $a3, $zero, 20
    /* 800BC0E0 8C590000 */  lw $t9, 0($v0)
    /* 800BC0E4 3C018015 */  lui $at, 0x8015
    /* 800BC0E8 3C048015 */  lui $a0, 0x8015
    /* 800BC0EC AC399B80 */  sw $t9, -25728($at)
    /* 800BC0F0 8C849B80 */  lw $a0, -25728($a0)
    /* 800BC0F4 0C02B0F6 */  jal 0x800AC3D8
    /* 800BC0F8 2405FFFF */  addiu $a1, $zero, -1
    /* 800BC0FC 0C02AD8E */  jal 0x800AB638
    /* 800BC100 00000000 */  nop
    /* 800BC104 0C02CAF7 */  jal 0x800B2BDC
    /* 800BC108 00000000 */  nop
    /* 800BC10C 0C02CCE3 */  jal 0x800B338C
    /* 800BC110 00000000 */  nop
    /* 800BC114 3C028011 */  lui $v0, 0x8011
    /* 800BC118 8C4274B4 */  lw $v0, 29876($v0)
    /* 800BC11C 8FAF00B0 */  lw $t7, 176($sp)
    /* 800BC120 304E0100 */  andi $t6, $v0, 0x0100
    /* 800BC124 15C00009 */  bne $t6, $zero, 0x800BC14C
    /* 800BC128 01C01025 */  or $v0, $t6, $zero
    /* 800BC12C 15E00007 */  bne $t7, $zero, 0x800BC14C
    /* 800BC130 00000000 */  nop
    /* 800BC134 0C02EC50 */  jal 0x800BB140
    /* 800BC138 00002025 */  or $a0, $zero, $zero
    /* 800BC13C 3C028011 */  lui $v0, 0x8011
    /* 800BC140 8C4274B4 */  lw $v0, 29876($v0)
    /* 800BC144 30580100 */  andi $t8, $v0, 0x0100
    /* 800BC148 03001025 */  or $v0, $t8, $zero
    /* 800BC14C 0C02CA4A */  jal 0x800B2928
    /* 800BC150 2C440001 */  sltiu $a0, $v0, 1
    /* 800BC154 3C028011 */  lui $v0, 0x8011
    /* 800BC158 24420680 */  addiu $v0, $v0, 1664
    /* 800BC15C A0400000 */  sb $zero, 0($v0)
    /* 800BC160 0C02EB78 */  jal 0x800BADE0
    /* 800BC164 A0400001 */  sb $zero, 1($v0)
    /* 800BC168 0C02EB56 */  jal 0x800BAD58
    /* 800BC16C 00000000 */  nop
    /* 800BC170 3C028015 */  lui $v0, 0x8015
    /* 800BC174 8042978C */  lb $v0, -26740($v0)
    /* 800BC178 04400005 */  bltz $v0, 0x800BC190
    /* 800BC17C 28410006 */  slti $at, $v0, 6
    /* 800BC180 10200003 */  beq $at, $zero, 0x800BC190
    /* 800BC184 00000000 */  nop
    /* 800BC188 0C02E4A1 */  jal 0x800B9284
    /* 800BC18C 00000000 */  nop
    /* 800BC190 0C02C160 */  jal 0x800B0580
    /* 800BC194 00000000 */  nop
    /* 800BC198 0C02E465 */  jal 0x800B9194
    /* 800BC19C 00000000 */  nop
    /* 800BC1A0 0C02BD47 */  jal 0x800AF51C
    /* 800BC1A4 00000000 */  nop
    /* 800BC1A8 3C198015 */  lui $t9, 0x8015
    /* 800BC1AC 83396994 */  lb $t9, 27028($t9)
    /* 800BC1B0 3C0F8012 */  lui $t7, 0x8012
    /* 800BC1B4 25EFA93C */  addiu $t7, $t7, -22212
    /* 800BC1B8 17200004 */  bne $t9, $zero, 0x800BC1CC
    /* 800BC1BC 8FAE0020 */  lw $t6, 32($sp)
    /* 800BC1C0 01CF082B */  sltu $at, $t6, $t7
    /* 800BC1C4 54200004 */  .word 0x54200004
    /* 800BC1C8 24020001 */  addiu $v0, $zero, 1
    /* 800BC1CC 0C0E2A33 */  jal 0x8038A8CC
    /* 800BC1D0 00000000 */  nop
    /* 800BC1D4 24020001 */  addiu $v0, $zero, 1
    /* 800BC1D8 8FBF001C */  lw $ra, 28($sp)
    /* 800BC1DC 27BD00A8 */  addiu $sp, $sp, 168
    /* 800BC1E0 03E00008 */  jr $ra
    /* 800BC1E4 00000000 */  nop
