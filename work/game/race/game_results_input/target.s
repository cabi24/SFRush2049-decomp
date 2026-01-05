# Source: game_code.bin (decompressed)
# Address: 0x800FF724

glabel game_results_input
    /* 800FF724 27BDFF88 */  addiu $sp, $sp, -120
    /* 800FF728 AFBF001C */  sw $ra, 28($sp)
    /* 800FF72C 8C82002C */  lw $v0, 44($a0)
    /* 800FF730 3C088015 */  lui $t0, 0x8015
    /* 800FF734 3C018011 */  lui $at, 0x8011
    /* 800FF738 00026902 */  srl $t5, $v0, 4
    /* 800FF73C 00026302 */  srl $t4, $v0, 12
    /* 800FF740 31AE000F */  andi $t6, $t5, 0x000F
    /* 800FF744 3198000F */  andi $t8, $t4, 0x000F
    /* 800FF748 01C06825 */  or $t5, $t6, $zero
    /* 800FF74C 03006025 */  or $t4, $t8, $zero
    /* 800FF750 0002CC02 */  srl $t9, $v0, 16
    /* 800FF754 332E00FF */  andi $t6, $t9, 0x00FF
    /* 800FF758 0018C040 */  sll $t8, $t8, 1
    /* 800FF75C 01184021 */  addu $t0, $t0, $t8
    /* 800FF760 3C198015 */  lui $t9, 0x8015
    /* 800FF764 8508A100 */  lh $t0, -24320($t0)
    /* 800FF768 87391AD0 */  lh $t9, 6864($t9)
    /* 800FF76C 0002FA02 */  srl $ra, $v0, 8
    /* 800FF770 C4242A9C */  lwc1 $f4, 10908($at)
    /* 800FF774 33EF000F */  andi $t7, $ra, 0x000F
    /* 800FF778 01E0F825 */  or $ra, $t7, $zero
    /* 800FF77C 304A000F */  andi $t2, $v0, 0x000F
    /* 800FF780 25CFFFFF */  addiu $t7, $t6, -1
    /* 800FF784 010A5821 */  addu $t3, $t0, $t2
    /* 800FF788 2B210002 */  slti $at, $t9, 2
    /* 800FF78C 00804825 */  or $t1, $a0, $zero
    /* 800FF790 AFAF0064 */  sw $t7, 100($sp)
    /* 800FF794 256BFFFF */  addiu $t3, $t3, -1
    /* 800FF798 14200006 */  bne $at, $zero, 0x800FF7B4
    /* 800FF79C E7A40054 */  swc1 $f4, 84($sp)
    /* 800FF7A0 3C013F00 */  lui $at, 0x3F00
    /* 800FF7A4 44813000 */  .word 0x44813000
    /* 800FF7A8 00000000 */  nop
    /* 800FF7AC 46062202 */  .word 0x46062202
    /* 800FF7B0 E7A80054 */  swc1 $f8, 84($sp)
    /* 800FF7B4 00002025 */  or $a0, $zero, $zero
    /* 800FF7B8 19400011 */  blez $t2, 0x800FF800
    /* 800FF7BC 00002825 */  or $a1, $zero, $zero
    /* 800FF7C0 24060008 */  addiu $a2, $zero, 8
    /* 800FF7C4 01051021 */  addu $v0, $t0, $a1
    /* 800FF7C8 14460002 */  bne $v0, $a2, 0x800FF7D4
    /* 800FF7CC 24030001 */  addiu $v1, $zero, 1
    /* 800FF7D0 00001825 */  or $v1, $zero, $zero
    /* 800FF7D4 14600003 */  bne $v1, $zero, 0x800FF7E4
    /* 800FF7D8 24A50001 */  addiu $a1, $a1, 1
    /* 800FF7DC 10000004 */  beq $zero, $zero, 0x800FF7F0
    /* 800FF7E0 256B0001 */  addiu $t3, $t3, 1
    /* 800FF7E4 24840001 */  addiu $a0, $a0, 1
    /* 800FF7E8 00047400 */  sll $t6, $a0, 16
    /* 800FF7EC 000E2403 */  sra $a0, $t6, 16
    /* 800FF7F0 0005C400 */  sll $t8, $a1, 16
    /* 800FF7F4 008A082A */  slt $at, $a0, $t2
    /* 800FF7F8 1420FFF2 */  bne $at, $zero, 0x800FF7C4
    /* 800FF7FC 00182C03 */  sra $a1, $t8, 16
    /* 800FF800 000C7080 */  sll $t6, $t4, 2
    /* 800FF804 01CC7021 */  addu $t6, $t6, $t4
    /* 800FF808 000E7080 */  sll $t6, $t6, 2
    /* 800FF80C 01CC7023 */  subu $t6, $t6, $t4
    /* 800FF810 3C0F8015 */  lui $t7, 0x8015
    /* 800FF814 25EFA118 */  addiu $t7, $t7, -24296
    /* 800FF818 000E7080 */  sll $t6, $t6, 2
    /* 800FF81C 01CFC021 */  addu $t8, $t6, $t7
    /* 800FF820 AFB8002C */  sw $t8, 44($sp)
    /* 800FF824 93020001 */  lbu $v0, 1($t8)
    /* 800FF828 01202025 */  or $a0, $t1, $zero
    /* 800FF82C 38590005 */  xori $t9, $v0, 0x0005
    /* 800FF830 2F220001 */  sltiu $v0, $t9, 1
    /* 800FF834 5440001A */  .word 0x5440001A
    /* 800FF838 8123001A */  lb $v1, 26($t1)
    /* 800FF83C 11A0000A */  beq $t5, $zero, 0x800FF868
    /* 800FF840 000C7080 */  sll $t6, $t4, 2
    /* 800FF844 01CC7021 */  addu $t6, $t6, $t4
    /* 800FF848 000E7040 */  sll $t6, $t6, 1
    /* 800FF84C 01CB7821 */  addu $t7, $t6, $t3
    /* 800FF850 3C028015 */  lui $v0, 0x8015
    /* 800FF854 004F1021 */  addu $v0, $v0, $t7
    /* 800FF858 80429DA8 */  lb $v0, -25176($v0)
    /* 800FF85C 2C580001 */  sltiu $t8, $v0, 1
    /* 800FF860 1700000E */  bne $t8, $zero, 0x800FF89C
    /* 800FF864 03001025 */  or $v0, $t8, $zero
    /* 800FF868 2963000A */  slti $v1, $t3, 10
    /* 800FF86C 38620001 */  xori $v0, $v1, 0x0001
    /* 800FF870 5440000B */  .word 0x5440000B
    /* 800FF874 8123001A */  lb $v1, 26($t1)
    /* 800FF878 3C028015 */  lui $v0, 0x8015
    /* 800FF87C 004C1021 */  addu $v0, $v0, $t4
    /* 800FF880 8042418C */  lb $v0, 16780($v0)
    /* 800FF884 3C0E8015 */  lui $t6, 0x8015
    /* 800FF888 2C590001 */  sltiu $t9, $v0, 1
    /* 800FF88C 17200003 */  bne $t9, $zero, 0x800FF89C
    /* 800FF890 03201025 */  or $v0, $t9, $zero
    /* 800FF894 85CEA10A */  lh $t6, -24310($t6)
    /* 800FF898 01CA102A */  slt $v0, $t6, $t2
    /* 800FF89C 8123001A */  lb $v1, 26($t1)
    /* 800FF8A0 10430010 */  beq $v0, $v1, 0x800FF8E4
    /* 800FF8A4 00000000 */  nop
    /* 800FF8A8 A122001A */  sb $v0, 26($t1)
    /* 800FF8AC AFBF006C */  sw $ra, 108($sp)
    /* 800FF8B0 AFAD0070 */  sw $t5, 112($sp)
    /* 800FF8B4 AFAC0068 */  sw $t4, 104($sp)
    /* 800FF8B8 AFAB0060 */  sw $t3, 96($sp)
    /* 800FF8BC AFAA0074 */  sw $t2, 116($sp)
    /* 800FF8C0 0C0253B2 */  jal 0x80094EC8
    /* 800FF8C4 AFA90078 */  sw $t1, 120($sp)
    /* 800FF8C8 8FA90078 */  lw $t1, 120($sp)
    /* 800FF8CC 8FAA0074 */  lw $t2, 116($sp)
    /* 800FF8D0 8FAB0060 */  lw $t3, 96($sp)
    /* 800FF8D4 8FAC0068 */  lw $t4, 104($sp)
    /* 800FF8D8 8FAD0070 */  lw $t5, 112($sp)
    /* 800FF8DC 8FBF006C */  lw $ra, 108($sp)
    /* 800FF8E0 8123001A */  lb $v1, 26($t1)
    /* 800FF8E4 10600003 */  beq $v1, $zero, 0x800FF8F4
    /* 800FF8E8 000D782B */  sltu $t7, $zero, $t5
    /* 800FF8EC 1000013A */  beq $zero, $zero, 0x800FFDD8
    /* 800FF8F0 24020001 */  addiu $v0, $zero, 1
    /* 800FF8F4 13E0009A */  beq $ra, $zero, 0x800FFB60
    /* 800FF8F8 AFAF0028 */  sw $t7, 40($sp)
    /* 800FF8FC 3C188015 */  lui $t8, 0x8015
    /* 800FF900 87181AD0 */  lh $t8, 6864($t8)
    /* 800FF904 3C058012 */  lui $a1, 0x8012
    /* 800FF908 24A50234 */  addiu $a1, $a1, 564
    /* 800FF90C 2B010002 */  slti $at, $t8, 2
    /* 800FF910 1420000B */  bne $at, $zero, 0x800FF940
    /* 800FF914 01202025 */  or $a0, $t1, $zero
    /* 800FF918 00003025 */  or $a2, $zero, $zero
    /* 800FF91C AFA90078 */  sw $t1, 120($sp)
    /* 800FF920 AFAA0074 */  sw $t2, 116($sp)
    /* 800FF924 AFAB0060 */  sw $t3, 96($sp)
    /* 800FF928 0C03BD6C */  jal 0x800EF5B0
    /* 800FF92C AFAC0068 */  sw $t4, 104($sp)
    /* 800FF930 8FA90078 */  lw $t1, 120($sp)
    /* 800FF934 8FAA0074 */  lw $t2, 116($sp)
    /* 800FF938 8FAB0060 */  lw $t3, 96($sp)
    /* 800FF93C 8FAC0068 */  lw $t4, 104($sp)
    /* 800FF940 000CC880 */  sll $t9, $t4, 2
    /* 800FF944 3C0E8015 */  lui $t6, 0x8015
    /* 800FF948 01D97021 */  addu $t6, $t6, $t9
    /* 800FF94C 8DCE4618 */  lw $t6, 17944($t6)
    /* 800FF950 000C7900 */  sll $t7, $t4, 4
    /* 800FF954 01EC7821 */  addu $t7, $t7, $t4
    /* 800FF958 11C00039 */  beq $t6, $zero, 0x800FFA40
    /* 800FF95C 01802025 */  or $a0, $t4, $zero
    /* 800FF960 000F7980 */  sll $t7, $t7, 6
    /* 800FF964 000BC180 */  sll $t8, $t3, 6
    /* 800FF968 01F8C821 */  addu $t9, $t7, $t8
    /* 800FF96C 000C7880 */  sll $t7, $t4, 2
    /* 800FF970 01EC7821 */  addu $t7, $t7, $t4
    /* 800FF974 3C0E8011 */  lui $t6, 0x8011
    /* 800FF978 25CE1998 */  addiu $t6, $t6, 6552
    /* 800FF97C 000F7880 */  sll $t7, $t7, 2
    /* 800FF980 01EC7823 */  subu $t7, $t7, $t4
    /* 800FF984 032E1821 */  addu $v1, $t9, $t6
    /* 800FF988 3C188015 */  lui $t8, 0x8015
    /* 800FF98C 27180B70 */  addiu $t8, $t8, 2928
    /* 800FF990 27B90058 */  addiu $t9, $sp, 88
    /* 800FF994 000F78C0 */  sll $t7, $t7, 3
    /* 800FF998 01F83021 */  addu $a2, $t7, $t8
    /* 800FF99C AFB90010 */  sw $t9, 16($sp)
    /* 800FF9A0 24650030 */  addiu $a1, $v1, 48
    /* 800FF9A4 AFA30024 */  sw $v1, 36($sp)
    /* 800FF9A8 00003825 */  or $a3, $zero, $zero
    /* 800FF9AC AFA90078 */  sw $t1, 120($sp)
    /* 800FF9B0 AFAB0060 */  sw $t3, 96($sp)
    /* 800FF9B4 0C029891 */  jal 0x800A6244
    /* 800FF9B8 AFAC0068 */  sw $t4, 104($sp)
    /* 800FF9BC 3C01425C */  lui $at, 0x425C
    /* 800FF9C0 44815000 */  .word 0x44815000
    /* 800FF9C4 C7B00054 */  lwc1 $f16, 84($sp)
    /* 800FF9C8 3C014396 */  lui $at, 0x4396
    /* 800FF9CC 44812000 */  .word 0x44812000
    /* 800FF9D0 46105482 */  .word 0x46105482
    /* 800FF9D4 3C018011 */  lui $at, 0x8011
    /* 800FF9D8 C4282AB0 */  lwc1 $f8, 10928($at)
    /* 800FF9DC 87AE0058 */  lh $t6, 88($sp)
    /* 800FF9E0 8FA90078 */  lw $t1, 120($sp)
    /* 800FF9E4 8FA30024 */  lw $v1, 36($sp)
    /* 800FF9E8 448E8000 */  .word 0x448E8000
    /* 800FF9EC 46049182 */  .word 0x46049182
    /* 800FF9F0 8FAB0060 */  lw $t3, 96($sp)
    /* 800FF9F4 8FAC0068 */  lw $t4, 104($sp)
    /* 800FF9F8 852E0016 */  lh $t6, 22($t1)
    /* 800FF9FC 468084A0 */  .word 0x468084A0
    /* 800FFA00 46083283 */  .word 0x46083283
    /* 800FFA04 460A9100 */  .word 0x460A9100
    /* 800FFA08 4600218D */  .word 0x4600218D
    /* 800FFA0C 44183000 */  .word 0x44183000
    /* 800FFA10 00000000 */  nop
    /* 800FFA14 A538000E */  sh $t8, 14($t1)
    /* 800FFA18 87B9005A */  lh $t9, 90($sp)
    /* 800FFA1C 05C10003 */  bgez $t6, 0x800FFA2C
    /* 800FFA20 000E7843 */  sra $t7, $t6, 1
    /* 800FFA24 25C10001 */  addiu $at, $t6, 1
    /* 800FFA28 00017843 */  sra $t7, $at, 1
    /* 800FFA2C 032FC023 */  subu $t8, $t9, $t7
    /* 800FFA30 A5380010 */  sh $t8, 16($t1)
    /* 800FFA34 906E003C */  lbu $t6, 60($v1)
    /* 800FFA38 1000001C */  beq $zero, $zero, 0x800FFAAC
    /* 800FFA3C A12E0018 */  sb $t6, 24($t1)
    /* 800FFA40 3C198015 */  lui $t9, 0x8015
    /* 800FFA44 87391AD0 */  lh $t9, 6864($t9)
    /* 800FFA48 3C038011 */  lui $v1, 0x8011
    /* 800FFA4C 000C1080 */  sll $v0, $t4, 2
    /* 800FFA50 00197880 */  sll $t7, $t9, 2
    /* 800FFA54 01F97823 */  subu $t7, $t7, $t9
    /* 800FFA58 24633EE0 */  addiu $v1, $v1, 16096
    /* 800FFA5C 000F7940 */  sll $t7, $t7, 5
    /* 800FFA60 004C1023 */  subu $v0, $v0, $t4
    /* 800FFA64 000210C0 */  sll $v0, $v0, 3
    /* 800FFA68 006FC021 */  addu $t8, $v1, $t7
    /* 800FFA6C 03027021 */  addu $t6, $t8, $v0
    /* 800FFA70 8DD9FFB0 */  lw $t9, -80($t6)
    /* 800FFA74 3C188015 */  lui $t8, 0x8015
    /* 800FFA78 272F0014 */  addiu $t7, $t9, 20
    /* 800FFA7C A52F000E */  sh $t7, 14($t1)
    /* 800FFA80 87181AD0 */  lh $t8, 6864($t8)
    /* 800FFA84 00187080 */  sll $t6, $t8, 2
    /* 800FFA88 01D87023 */  subu $t6, $t6, $t8
    /* 800FFA8C 000E7140 */  sll $t6, $t6, 5
    /* 800FFA90 006EC821 */  addu $t9, $v1, $t6
    /* 800FFA94 03227821 */  addu $t7, $t9, $v0
    /* 800FFA98 8DF8FFA4 */  lw $t8, -92($t7)
    /* 800FFA9C 000A7100 */  sll $t6, $t2, 4
    /* 800FFAA0 030EC821 */  addu $t9, $t8, $t6
    /* 800FFAA4 272F0008 */  addiu $t7, $t9, 8
    /* 800FFAA8 A52F0010 */  sh $t7, 16($t1)
    /* 800FFAAC 8FB80028 */  lw $t8, 40($sp)
    /* 800FFAB0 3C0E8015 */  lui $t6, 0x8015
    /* 800FFAB4 01202025 */  or $a0, $t1, $zero
    /* 800FFAB8 5300000C */  .word 0x5300000C
    /* 800FFABC AFA90078 */  sw $t1, 120($sp)
    /* 800FFAC0 85CE1AD0 */  lh $t6, 6864($t6)
    /* 800FFAC4 24010001 */  addiu $at, $zero, 1
    /* 800FFAC8 2402000A */  addiu $v0, $zero, 10
    /* 800FFACC 15C10003 */  bne $t6, $at, 0x800FFADC
    /* 800FFAD0 00000000 */  nop
    /* 800FFAD4 10000001 */  beq $zero, $zero, 0x800FFADC
    /* 800FFAD8 24020014 */  addiu $v0, $zero, 20
    /* 800FFADC 8539000E */  lh $t9, 14($t1)
    /* 800FFAE0 03227821 */  addu $t7, $t9, $v0
    /* 800FFAE4 A52F000E */  sh $t7, 14($t1)
    /* 800FFAE8 AFA90078 */  sw $t1, 120($sp)
    /* 800FFAEC AFAB0060 */  sw $t3, 96($sp)
    /* 800FFAF0 0C0253B2 */  jal 0x80094EC8
    /* 800FFAF4 AFAC0068 */  sw $t4, 104($sp)
    /* 800FFAF8 8FA90078 */  lw $t1, 120($sp)
    /* 800FFAFC 3C0100FF */  lui $at, 0x00FF
    /* 800FFB00 3421F0FF */  ori $at, $at, 0xF0FF
    /* 800FFB04 8D38002C */  lw $t8, 44($t1)
    /* 800FFB08 8FAB0060 */  lw $t3, 96($sp)
    /* 800FFB0C 8FAC0068 */  lw $t4, 104($sp)
    /* 800FFB10 03017024 */  and $t6, $t8, $at
    /* 800FFB14 AD2E002C */  sw $t6, 44($t1)
    /* 800FFB18 8FB90028 */  lw $t9, 40($sp)
    /* 800FFB1C 31D8FFFF */  andi $t8, $t6, 0xFFFF
    /* 800FFB20 3C01001C */  lui $at, 0x001C
    /* 800FFB24 1320000E */  beq $t9, $zero, 0x800FFB60
    /* 800FFB28 03017025 */  or $t6, $t8, $at
    /* 800FFB2C 85260016 */  lh $a2, 22($t1)
    /* 800FFB30 AD2E002C */  sw $t6, 44($t1)
    /* 800FFB34 3C058011 */  lui $a1, 0x8011
    /* 800FFB38 8CA540E8 */  lw $a1, 16616($a1)
    /* 800FFB3C AFAC0068 */  sw $t4, 104($sp)
    /* 800FFB40 AFAB0060 */  sw $t3, 96($sp)
    /* 800FFB44 AFA90078 */  sw $t1, 120($sp)
    /* 800FFB48 01202025 */  or $a0, $t1, $zero
    /* 800FFB4C 0C03F96C */  jal 0x800FE5B0
    /* 800FFB50 00C03825 */  or $a3, $a2, $zero
    /* 800FFB54 8FA90078 */  lw $t1, 120($sp)
    /* 800FFB58 8FAB0060 */  lw $t3, 96($sp)
    /* 800FFB5C 8FAC0068 */  lw $t4, 104($sp)
    /* 800FFB60 000CC880 */  sll $t9, $t4, 2
    /* 800FFB64 3C0F8015 */  lui $t7, 0x8015
    /* 800FFB68 01F97821 */  addu $t7, $t7, $t9
    /* 800FFB6C 8DEF4618 */  lw $t7, 17944($t7)
    /* 800FFB70 000CC100 */  sll $t8, $t4, 4
    /* 800FFB74 240E00FE */  addiu $t6, $zero, 254
    /* 800FFB78 11E00065 */  beq $t7, $zero, 0x800FFD10
    /* 800FFB7C 01202025 */  or $a0, $t1, $zero
    /* 800FFB80 030CC021 */  addu $t8, $t8, $t4
    /* 800FFB84 0018C180 */  sll $t8, $t8, 6
    /* 800FFB88 000B7180 */  sll $t6, $t3, 6
    /* 800FFB8C 030EC821 */  addu $t9, $t8, $t6
    /* 800FFB90 000CC080 */  sll $t8, $t4, 2
    /* 800FFB94 030CC021 */  addu $t8, $t8, $t4
    /* 800FFB98 3C0F8011 */  lui $t7, 0x8011
    /* 800FFB9C 25EF1998 */  addiu $t7, $t7, 6552
    /* 800FFBA0 0018C080 */  sll $t8, $t8, 2
    /* 800FFBA4 030CC023 */  subu $t8, $t8, $t4
    /* 800FFBA8 032F1821 */  addu $v1, $t9, $t7
    /* 800FFBAC 3C0E8015 */  lui $t6, 0x8015
    /* 800FFBB0 25CE0B70 */  addiu $t6, $t6, 2928
    /* 800FFBB4 27B90058 */  addiu $t9, $sp, 88
    /* 800FFBB8 0018C0C0 */  sll $t8, $t8, 3
    /* 800FFBBC 030E3021 */  addu $a2, $t8, $t6
    /* 800FFBC0 AFB90010 */  sw $t9, 16($sp)
    /* 800FFBC4 24650030 */  addiu $a1, $v1, 48
    /* 800FFBC8 AFA30024 */  sw $v1, 36($sp)
    /* 800FFBCC 01802025 */  or $a0, $t4, $zero
    /* 800FFBD0 00003825 */  or $a3, $zero, $zero
    /* 800FFBD4 AFA90078 */  sw $t1, 120($sp)
    /* 800FFBD8 0C029891 */  jal 0x800A6244
    /* 800FFBDC AFAB0060 */  sw $t3, 96($sp)
    /* 800FFBE0 3C014296 */  lui $at, 0x4296
    /* 800FFBE4 44814000 */  .word 0x44814000
    /* 800FFBE8 C7B00054 */  lwc1 $f16, 84($sp)
    /* 800FFBEC 3C014396 */  lui $at, 0x4396
    /* 800FFBF0 44815000 */  .word 0x44815000
    /* 800FFBF4 46104482 */  .word 0x46104482
    /* 800FFBF8 3C018011 */  lui $at, 0x8011
    /* 800FFBFC C4262AB0 */  lwc1 $f6, 10928($at)
    /* 800FFC00 87AF0058 */  lh $t7, 88($sp)
    /* 800FFC04 8FA90078 */  lw $t1, 120($sp)
    /* 800FFC08 8FA30024 */  lw $v1, 36($sp)
    /* 800FFC0C 448F8000 */  .word 0x448F8000
    /* 800FFC10 460A9102 */  .word 0x460A9102
    /* 800FFC14 852F0016 */  lh $t7, 22($t1)
    /* 800FFC18 8FAB0060 */  lw $t3, 96($sp)
    /* 800FFC1C 01202025 */  or $a0, $t1, $zero
    /* 800FFC20 468084A0 */  .word 0x468084A0
    /* 800FFC24 46062203 */  .word 0x46062203
    /* 800FFC28 46089280 */  .word 0x46089280
    /* 800FFC2C 4600510D */  .word 0x4600510D
    /* 800FFC30 440E2000 */  .word 0x440E2000
    /* 800FFC34 00000000 */  nop
    /* 800FFC38 A52E000E */  sh $t6, 14($t1)
    /* 800FFC3C 87B9005A */  lh $t9, 90($sp)
    /* 800FFC40 05E10003 */  bgez $t7, 0x800FFC50
    /* 800FFC44 000FC043 */  sra $t8, $t7, 1
    /* 800FFC48 25E10001 */  addiu $at, $t7, 1
    /* 800FFC4C 0001C043 */  sra $t8, $at, 1
    /* 800FFC50 240F7F00 */  addiu $t7, $zero, 32512
    /* 800FFC54 03387023 */  subu $t6, $t9, $t8
    /* 800FFC58 A52E0010 */  sh $t6, 16($t1)
    /* 800FFC5C A52F0012 */  sh $t7, 18($t1)
    /* 800FFC60 8FB90028 */  lw $t9, 40($sp)
    /* 800FFC64 3C188015 */  lui $t8, 0x8015
    /* 800FFC68 5320000C */  .word 0x5320000C
    /* 800FFC6C 9062003C */  lbu $v0, 60($v1)
    /* 800FFC70 87181AD0 */  lh $t8, 6864($t8)
    /* 800FFC74 24010001 */  addiu $at, $zero, 1
    /* 800FFC78 2402000A */  addiu $v0, $zero, 10
    /* 800FFC7C 17010003 */  bne $t8, $at, 0x800FFC8C
    /* 800FFC80 00000000 */  nop
    /* 800FFC84 10000001 */  beq $zero, $zero, 0x800FFC8C
    /* 800FFC88 24020014 */  addiu $v0, $zero, 20
    /* 800FFC8C 852E000E */  lh $t6, 14($t1)
    /* 800FFC90 01C27821 */  addu $t7, $t6, $v0
    /* 800FFC94 A52F000E */  sh $t7, 14($t1)
    /* 800FFC98 9062003C */  lbu $v0, 60($v1)
    /* 800FFC9C 241900FE */  addiu $t9, $zero, 254
    /* 800FFCA0 284100FF */  slti $at, $v0, 255
    /* 800FFCA4 54200004 */  .word 0x54200004
    /* 800FFCA8 A1220018 */  sb $v0, 24($t1)
    /* 800FFCAC 10000002 */  beq $zero, $zero, 0x800FFCB8
    /* 800FFCB0 A1390018 */  sb $t9, 24($t1)
    /* 800FFCB4 A1220018 */  sb $v0, 24($t1)
    /* 800FFCB8 91220018 */  lbu $v0, 24($t1)
    /* 800FFCBC 8123001A */  lb $v1, 26($t1)
    /* 800FFCC0 2C580001 */  sltiu $t8, $v0, 1
    /* 800FFCC4 13030008 */  beq $t8, $v1, 0x800FFCE8
    /* 800FFCC8 00000000 */  nop
    /* 800FFCCC A138001A */  sb $t8, 26($t1)
    /* 800FFCD0 AFAB0060 */  sw $t3, 96($sp)
    /* 800FFCD4 0C0253B2 */  jal 0x80094EC8
    /* 800FFCD8 AFA90078 */  sw $t1, 120($sp)
    /* 800FFCDC 8FA90078 */  lw $t1, 120($sp)
    /* 800FFCE0 8FAB0060 */  lw $t3, 96($sp)
    /* 800FFCE4 8123001A */  lb $v1, 26($t1)
    /* 800FFCE8 10600003 */  beq $v1, $zero, 0x800FFCF8
    /* 800FFCEC 01202025 */  or $a0, $t1, $zero
    /* 800FFCF0 10000039 */  beq $zero, $zero, 0x800FFDD8
    /* 800FFCF4 24020001 */  addiu $v0, $zero, 1
    /* 800FFCF8 AFA90078 */  sw $t1, 120($sp)
    /* 800FFCFC 0C0253B2 */  jal 0x80094EC8
    /* 800FFD00 AFAB0060 */  sw $t3, 96($sp)
    /* 800FFD04 8FA90078 */  lw $t1, 120($sp)
    /* 800FFD08 10000007 */  beq $zero, $zero, 0x800FFD28
    /* 800FFD0C 8FAB0060 */  lw $t3, 96($sp)
    /* 800FFD10 A12E0018 */  sb $t6, 24($t1)
    /* 800FFD14 AFAB0060 */  sw $t3, 96($sp)
    /* 800FFD18 0C0253B2 */  jal 0x80094EC8
    /* 800FFD1C AFA90078 */  sw $t1, 120($sp)
    /* 800FFD20 8FA90078 */  lw $t1, 120($sp)
    /* 800FFD24 8FAB0060 */  lw $t3, 96($sp)
    /* 800FFD28 8FAF0028 */  lw $t7, 40($sp)
    /* 800FFD2C 8FB9002C */  lw $t9, 44($sp)
    /* 800FFD30 51E00004 */  .word 0x51E00004
    /* 800FFD34 93380001 */  lbu $t8, 1($t9)
    /* 800FFD38 10000027 */  beq $zero, $zero, 0x800FFDD8
    /* 800FFD3C 24020001 */  addiu $v0, $zero, 1
    /* 800FFD40 93380001 */  lbu $t8, 1($t9)
    /* 800FFD44 3C028014 */  lui $v0, 0x8014
    /* 800FFD48 8FB90064 */  lw $t9, 100($sp)
    /* 800FFD4C 00187080 */  sll $t6, $t8, 2
    /* 800FFD50 01D87021 */  addu $t6, $t6, $t8
    /* 800FFD54 000E7040 */  sll $t6, $t6, 1
    /* 800FFD58 01CB7821 */  addu $t7, $t6, $t3
    /* 800FFD5C 004F1021 */  addu $v0, $v0, $t7
    /* 800FFD60 8042C068 */  lb $v0, -16280($v0)
    /* 800FFD64 3C0100FF */  lui $at, 0x00FF
    /* 800FFD68 01202025 */  or $a0, $t1, $zero
    /* 800FFD6C 13220019 */  beq $t9, $v0, 0x800FFDD4
    /* 800FFD70 24430001 */  addiu $v1, $v0, 1
    /* 800FFD74 AFA20064 */  sw $v0, 100($sp)
    /* 800FFD78 8D38002C */  lw $t8, 44($t1)
    /* 800FFD7C 00037C00 */  sll $t7, $v1, 16
    /* 800FFD80 01E1C824 */  and $t9, $t7, $at
    /* 800FFD84 330EFFFF */  andi $t6, $t8, 0xFFFF
    /* 800FFD88 01D9C025 */  or $t8, $t6, $t9
    /* 800FFD8C AD38002C */  sw $t8, 44($t1)
    /* 800FFD90 8FAF0064 */  lw $t7, 100($sp)
    /* 800FFD94 24010019 */  addiu $at, $zero, 25
    /* 800FFD98 55E10007 */  .word 0x55E10007
    /* 800FFD9C 8FAE0064 */  lw $t6, 100($sp)
    /* 800FFDA0 11600003 */  beq $t3, $zero, 0x800FFDB0
    /* 800FFDA4 24010001 */  addiu $at, $zero, 1
    /* 800FFDA8 55610003 */  .word 0x55610003
    /* 800FFDAC 8FAE0064 */  lw $t6, 100($sp)
    /* 800FFDB0 AFA30064 */  sw $v1, 100($sp)
    /* 800FFDB4 8FAE0064 */  lw $t6, 100($sp)
    /* 800FFDB8 85260016 */  lh $a2, 22($t1)
    /* 800FFDBC 3C058011 */  lui $a1, 0x8011
    /* 800FFDC0 000EC880 */  sll $t9, $t6, 2
    /* 800FFDC4 00B92821 */  addu $a1, $a1, $t9
    /* 800FFDC8 8CA5407C */  lw $a1, 16508($a1)
    /* 800FFDCC 0C03F96C */  jal 0x800FE5B0
    /* 800FFDD0 00C03825 */  or $a3, $a2, $zero
    /* 800FFDD4 24020001 */  addiu $v0, $zero, 1
    /* 800FFDD8 8FBF001C */  lw $ra, 28($sp)
    /* 800FFDDC 27BD0078 */  addiu $sp, $sp, 120
    /* 800FFDE0 03E00008 */  jr $ra
    /* 800FFDE4 00000000 */  nop
