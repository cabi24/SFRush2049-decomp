# Source: game_code.bin (decompressed)
# Address: 0x800DFBC4

glabel profile_select_handler
    /* 800DFBC4 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800DFBC8 3C013F80 */  lui $at, 0x3F80
    /* 800DFBCC AFBF0014 */  sw $ra, 20($sp)
    /* 800DFBD0 44817000 */  .word 0x44817000
    /* 800DFBD4 860C07C6 */  lh $t4, 1990($s0)
    /* 800DFBD8 00001825 */  or $v1, $zero, $zero
    /* 800DFBDC 00002025 */  or $a0, $zero, $zero
    /* 800DFBE0 00002825 */  or $a1, $zero, $zero
    /* 800DFBE4 00003025 */  or $a2, $zero, $zero
    /* 800DFBE8 02004825 */  or $t1, $s0, $zero
    /* 800DFBEC 240B0003 */  addiu $t3, $zero, 3
    /* 800DFBF0 240A0004 */  addiu $t2, $zero, 4
    /* 800DFBF4 24080002 */  addiu $t0, $zero, 2
    /* 800DFBF8 24070001 */  addiu $a3, $zero, 1
    /* 800DFBFC 46008686 */  .word 0x46008686
    /* 800DFC00 46008606 */  .word 0x46008606
    /* 800DFC04 46008786 */  .word 0x46008786
    /* 800DFC08 9522061C */  lhu $v0, 1564($t1)
    /* 800DFC0C 1440002D */  bne $v0, $zero, 0x800DFCC4
    /* 800DFC10 00000000 */  nop
    /* 800DFC14 860E0758 */  lh $t6, 1880($s0)
    /* 800DFC18 3C014320 */  lui $at, 0x4320
    /* 800DFC1C 44814000 */  .word 0x44814000
    /* 800DFC20 000E7883 */  sra $t7, $t6, 2
    /* 800DFC24 448F2000 */  .word 0x448F2000
    /* 800DFC28 00000000 */  nop
    /* 800DFC2C 468021A0 */  .word 0x468021A0
    /* 800DFC30 46083083 */  .word 0x46083083
    /* 800DFC34 4610103C */  .word 0x4610103C
    /* 800DFC38 00000000 */  nop
    /* 800DFC3C 45020003 */  .word 0x45020003
    /* 800DFC40 4610103C */  .word 0x4610103C
    /* 800DFC44 46001087 */  .word 0x46001087
    /* 800DFC48 4610103C */  .word 0x4610103C
    /* 800DFC4C 00000000 */  nop
    /* 800DFC50 45020004 */  .word 0x45020004
    /* 800DFC54 4602703C */  .word 0x4602703C
    /* 800DFC58 10000009 */  beq $zero, $zero, 0x800DFC80
    /* 800DFC5C 46008086 */  .word 0x46008086
    /* 800DFC60 4602703C */  .word 0x4602703C
    /* 800DFC64 00000000 */  nop
    /* 800DFC68 45020004 */  .word 0x45020004
    /* 800DFC6C 46001006 */  .word 0x46001006
    /* 800DFC70 10000002 */  beq $zero, $zero, 0x800DFC7C
    /* 800DFC74 46007006 */  .word 0x46007006
    /* 800DFC78 46001006 */  .word 0x46001006
    /* 800DFC7C 46000086 */  .word 0x46000086
    /* 800DFC80 46027001 */  .word 0x46027001
    /* 800DFC84 24630001 */  addiu $v1, $v1, 1
    /* 800DFC88 46000282 */  .word 0x46000282
    /* 800DFC8C 14670004 */  bne $v1, $a3, 0x800DFCA0
    /* 800DFC90 460A7081 */  .word 0x460A7081
    /* 800DFC94 46121083 */  .word 0x46121083
    /* 800DFC98 10000073 */  beq $zero, $zero, 0x800DFE68
    /* 800DFC9C 4602C600 */  .word 0x4602C600
    /* 800DFCA0 54680004 */  .word 0x54680004
    /* 800DFCA4 4600B306 */  .word 0x4600B306
    /* 800DFCA8 10000002 */  beq $zero, $zero, 0x800DFCB4
    /* 800DFCAC 4600A306 */  .word 0x4600A306
    /* 800DFCB0 4600B306 */  .word 0x4600B306
    /* 800DFCB4 46006006 */  .word 0x46006006
    /* 800DFCB8 46001083 */  .word 0x46001083
    /* 800DFCBC 1000006A */  beq $zero, $zero, 0x800DFE68
    /* 800DFCC0 4602C600 */  .word 0x4602C600
    /* 800DFCC4 14E20037 */  bne $a3, $v0, 0x800DFDA4
    /* 800DFCC8 00000000 */  nop
    /* 800DFCCC 86180758 */  lh $t8, 1880($s0)
    /* 800DFCD0 00067080 */  sll $t6, $a2, 2
    /* 800DFCD4 020E1021 */  addu $v0, $s0, $t6
    /* 800DFCD8 0018C883 */  sra $t9, $t8, 2
    /* 800DFCDC 44992000 */  .word 0x44992000
    /* 800DFCE0 24840001 */  addiu $a0, $a0, 1
    /* 800DFCE4 468021A0 */  .word 0x468021A0
    /* 800DFCE8 461C3083 */  .word 0x461C3083
    /* 800DFCEC 46101032 */  .word 0x46101032
    /* 800DFCF0 00000000 */  nop
    /* 800DFCF4 45030018 */  .word 0x45030018
    /* 800DFCF8 C44007F8 */  lwc1 $f0, 2040($v0)
    /* 800DFCFC 4610103C */  .word 0x4610103C
    /* 800DFD00 00000000 */  nop
    /* 800DFD04 45020003 */  .word 0x45020003
    /* 800DFD08 4610103C */  .word 0x4610103C
    /* 800DFD0C 46001087 */  .word 0x46001087
    /* 800DFD10 4610103C */  .word 0x4610103C
    /* 800DFD14 00000000 */  nop
    /* 800DFD18 45020004 */  .word 0x45020004
    /* 800DFD1C 4602703C */  .word 0x4602703C
    /* 800DFD20 10000009 */  beq $zero, $zero, 0x800DFD48
    /* 800DFD24 46008086 */  .word 0x46008086
    /* 800DFD28 4602703C */  .word 0x4602703C
    /* 800DFD2C 00000000 */  nop
    /* 800DFD30 45020004 */  .word 0x45020004
    /* 800DFD34 46001006 */  .word 0x46001006
    /* 800DFD38 10000002 */  beq $zero, $zero, 0x800DFD44
    /* 800DFD3C 46007006 */  .word 0x46007006
    /* 800DFD40 46001006 */  .word 0x46001006
    /* 800DFD44 46000086 */  .word 0x46000086
    /* 800DFD48 46027001 */  .word 0x46027001
    /* 800DFD4C 46000202 */  .word 0x46000202
    /* 800DFD50 46087081 */  .word 0x46087081
    /* 800DFD54 C44007F8 */  lwc1 $f0, 2040($v0)
    /* 800DFD58 4600103C */  .word 0x4600103C
    /* 800DFD5C 00000000 */  nop
    /* 800DFD60 45000002 */  .word 0x45000002
    /* 800DFD64 00000000 */  nop
    /* 800DFD68 46000086 */  .word 0x46000086
    /* 800DFD6C 14870004 */  bne $a0, $a3, 0x800DFD80
    /* 800DFD70 00000000 */  nop
    /* 800DFD74 46121083 */  .word 0x46121083
    /* 800DFD78 1000003B */  beq $zero, $zero, 0x800DFE68
    /* 800DFD7C 4602D680 */  .word 0x4602D680
    /* 800DFD80 54880004 */  .word 0x54880004
    /* 800DFD84 4600B306 */  .word 0x4600B306
    /* 800DFD88 10000002 */  beq $zero, $zero, 0x800DFD94
    /* 800DFD8C 4600A306 */  .word 0x4600A306
    /* 800DFD90 4600B306 */  .word 0x4600B306
    /* 800DFD94 46006006 */  .word 0x46006006
    /* 800DFD98 46001083 */  .word 0x46001083
    /* 800DFD9C 10000032 */  beq $zero, $zero, 0x800DFE68
    /* 800DFDA0 4602D680 */  .word 0x4602D680
    /* 800DFDA4 51020004 */  .word 0x51020004
    /* 800DFDA8 860F0758 */  lh $t7, 1880($s0)
    /* 800DFDAC 5562002F */  .word 0x5562002F
    /* 800DFDB0 24C60001 */  addiu $a2, $a2, 1
    /* 800DFDB4 860F0758 */  lh $t7, 1880($s0)
    /* 800DFDB8 24A50001 */  addiu $a1, $a1, 1
    /* 800DFDBC 000FC083 */  sra $t8, $t7, 2
    /* 800DFDC0 44985000 */  .word 0x44985000
    /* 800DFDC4 00000000 */  nop
    /* 800DFDC8 46805120 */  .word 0x46805120
    /* 800DFDCC 461C2083 */  .word 0x461C2083
    /* 800DFDD0 46101032 */  .word 0x46101032
    /* 800DFDD4 00000000 */  nop
    /* 800DFDD8 45010017 */  .word 0x45010017
    /* 800DFDDC 00000000 */  nop
    /* 800DFDE0 4610103C */  .word 0x4610103C
    /* 800DFDE4 00000000 */  nop
    /* 800DFDE8 45020003 */  .word 0x45020003
    /* 800DFDEC 4610103C */  .word 0x4610103C
    /* 800DFDF0 46001087 */  .word 0x46001087
    /* 800DFDF4 4610103C */  .word 0x4610103C
    /* 800DFDF8 00000000 */  nop
    /* 800DFDFC 45020004 */  .word 0x45020004
    /* 800DFE00 4602703C */  .word 0x4602703C
    /* 800DFE04 10000009 */  beq $zero, $zero, 0x800DFE2C
    /* 800DFE08 46008086 */  .word 0x46008086
    /* 800DFE0C 4602703C */  .word 0x4602703C
    /* 800DFE10 00000000 */  nop
    /* 800DFE14 45020004 */  .word 0x45020004
    /* 800DFE18 46001006 */  .word 0x46001006
    /* 800DFE1C 10000002 */  beq $zero, $zero, 0x800DFE28
    /* 800DFE20 46007006 */  .word 0x46007006
    /* 800DFE24 46001006 */  .word 0x46001006
    /* 800DFE28 46000086 */  .word 0x46000086
    /* 800DFE2C 46027001 */  .word 0x46027001
    /* 800DFE30 46000182 */  .word 0x46000182
    /* 800DFE34 46067081 */  .word 0x46067081
    /* 800DFE38 14A70003 */  bne $a1, $a3, 0x800DFE48
    /* 800DFE3C 00000000 */  nop
    /* 800DFE40 10000007 */  beq $zero, $zero, 0x800DFE60
    /* 800DFE44 46009006 */  .word 0x46009006
    /* 800DFE48 54A80004 */  .word 0x54A80004
    /* 800DFE4C 4600B306 */  .word 0x4600B306
    /* 800DFE50 10000002 */  beq $zero, $zero, 0x800DFE5C
    /* 800DFE54 4600A306 */  .word 0x4600A306
    /* 800DFE58 4600B306 */  .word 0x4600B306
    /* 800DFE5C 46006006 */  .word 0x46006006
    /* 800DFE60 46001083 */  .word 0x46001083
    /* 800DFE64 4602F780 */  .word 0x4602F780
    /* 800DFE68 24C60001 */  addiu $a2, $a2, 1
    /* 800DFE6C 14CAFF66 */  bne $a2, $t2, 0x800DFC08
    /* 800DFE70 25290002 */  addiu $t1, $t1, 2
    /* 800DFE74 461AC03C */  .word 0x461AC03C
    /* 800DFE78 00000000 */  nop
    /* 800DFE7C 4502002F */  .word 0x4502002F
    /* 800DFE80 461EC03C */  .word 0x461EC03C
    /* 800DFE84 461AF03C */  .word 0x461AF03C
    /* 800DFE88 000CC900 */  sll $t9, $t4, 4
    /* 800DFE8C 032CC823 */  subu $t9, $t9, $t4
    /* 800DFE90 0019C880 */  sll $t9, $t9, 2
    /* 800DFE94 45000028 */  .word 0x45000028
    /* 800DFE98 3C0E8014 */  lui $t6, 0x8014
    /* 800DFE9C 25CE06C0 */  addiu $t6, $t6, 1728
    /* 800DFEA0 032E8821 */  addu $s1, $t9, $t6
    /* 800DFEA4 8E2F0000 */  lw $t7, 0($s1)
    /* 800DFEA8 2401FFFF */  addiu $at, $zero, -1
    /* 800DFEAC 55E10010 */  .word 0x55E10010
    /* 800DFEB0 3C013F00 */  lui $at, 0x3F00
    /* 800DFEB4 0C024701 */  jal 0x80091C04
    /* 800DFEB8 8E240014 */  lw $a0, 20($s1)
    /* 800DFEBC 0C03552F */  jal 0x800D54BC
    /* 800DFEC0 26240014 */  addiu $a0, $s1, 20
    /* 800DFEC4 0C024701 */  jal 0x80091C04
    /* 800DFEC8 8E240028 */  lw $a0, 40($s1)
    /* 800DFECC 0C03552F */  jal 0x800D54BC
    /* 800DFED0 26240028 */  addiu $a0, $s1, 40
    /* 800DFED4 2404000B */  addiu $a0, $zero, 11
    /* 800DFED8 860507C6 */  lh $a1, 1990($s0)
    /* 800DFEDC 24060001 */  addiu $a2, $zero, 1
    /* 800DFEE0 0C0248D8 */  jal 0x80092360
    /* 800DFEE4 24070002 */  addiu $a3, $zero, 2
    /* 800DFEE8 AE220000 */  sw $v0, 0($s1)
    /* 800DFEEC 3C013F00 */  lui $at, 0x3F00
    /* 800DFEF0 44811000 */  .word 0x44811000
    /* 800DFEF4 3C018012 */  lui $at, 0x8012
    /* 800DFEF8 02208025 */  or $s0, $s1, $zero
    /* 800DFEFC 461A103C */  .word 0x461A103C
    /* 800DFF00 00000000 */  nop
    /* 800DFF04 45020004 */  .word 0x45020004
    /* 800DFF08 46001006 */  .word 0x46001006
    /* 800DFF0C 10000002 */  beq $zero, $zero, 0x800DFF18
    /* 800DFF10 4600D006 */  .word 0x4600D006
    /* 800DFF14 46001006 */  .word 0x46001006
    /* 800DFF18 C4284370 */  lwc1 $f8, 17264($at)
    /* 800DFF1C 3C013E80 */  lui $at, 0x3E80
    /* 800DFF20 44815000 */  .word 0x44815000
    /* 800DFF24 4608D302 */  .word 0x4608D302
    /* 800DFF28 0C037EC2 */  jal 0x800DFB08
    /* 800DFF2C 460A0701 */  .word 0x460A0701
    /* 800DFF30 10000042 */  beq $zero, $zero, 0x800E003C
    /* 800DFF34 8FBF0014 */  lw $ra, 20($sp)
    /* 800DFF38 461EC03C */  .word 0x461EC03C
    /* 800DFF3C 000C7900 */  sll $t7, $t4, 4
    /* 800DFF40 01EC7823 */  subu $t7, $t7, $t4
    /* 800DFF44 000F7880 */  sll $t7, $t7, 2
    /* 800DFF48 4500002D */  .word 0x4500002D
    /* 800DFF4C 3C188014 */  lui $t8, 0x8014
    /* 800DFF50 461ED03C */  .word 0x461ED03C
    /* 800DFF54 3C198014 */  lui $t9, 0x8014
    /* 800DFF58 273906C0 */  addiu $t9, $t9, 1728
    /* 800DFF5C 45000028 */  .word 0x45000028
    /* 800DFF60 00000000 */  nop
    /* 800DFF64 000CC100 */  sll $t8, $t4, 4
    /* 800DFF68 030CC023 */  subu $t8, $t8, $t4
    /* 800DFF6C 0018C080 */  sll $t8, $t8, 2
    /* 800DFF70 03198821 */  addu $s1, $t8, $t9
    /* 800DFF74 8E2E0014 */  lw $t6, 20($s1)
    /* 800DFF78 2401FFFF */  addiu $at, $zero, -1
    /* 800DFF7C 55C1001A */  .word 0x55C1001A
    /* 800DFF80 3C013F80 */  lui $at, 0x3F80
    /* 800DFF84 0C024701 */  jal 0x80091C04
    /* 800DFF88 8E240000 */  lw $a0, 0($s1)
    /* 800DFF8C 0C03552F */  jal 0x800D54BC
    /* 800DFF90 02202025 */  or $a0, $s1, $zero
    /* 800DFF94 0C024701 */  jal 0x80091C04
    /* 800DFF98 8E240028 */  lw $a0, 40($s1)
    /* 800DFF9C 0C03552F */  jal 0x800D54BC
    /* 800DFFA0 26240028 */  addiu $a0, $s1, 40
    /* 800DFFA4 3C018012 */  lui $at, 0x8012
    /* 800DFFA8 C4244374 */  lwc1 $f4, 17268($at)
    /* 800DFFAC 24040015 */  addiu $a0, $zero, 21
    /* 800DFFB0 24060002 */  addiu $a2, $zero, 2
    /* 800DFFB4 461E203C */  .word 0x461E203C
    /* 800DFFB8 24070001 */  addiu $a3, $zero, 1
    /* 800DFFBC 45020004 */  .word 0x45020004
    /* 800DFFC0 24040014 */  addiu $a0, $zero, 20
    /* 800DFFC4 0C0248D8 */  jal 0x80092360
    /* 800DFFC8 860507C6 */  lh $a1, 1990($s0)
    /* 800DFFCC 24040014 */  addiu $a0, $zero, 20
    /* 800DFFD0 860507C6 */  lh $a1, 1990($s0)
    /* 800DFFD4 24060001 */  addiu $a2, $zero, 1
    /* 800DFFD8 0C0248D8 */  jal 0x80092360
    /* 800DFFDC 24070002 */  addiu $a3, $zero, 2
    /* 800DFFE0 AE220014 */  sw $v0, 20($s1)
    /* 800DFFE4 3C013F80 */  lui $at, 0x3F80
    /* 800DFFE8 4481E000 */  .word 0x4481E000
    /* 800DFFEC 26300014 */  addiu $s0, $s1, 20
    /* 800DFFF0 0C037EC2 */  jal 0x800DFB08
    /* 800DFFF4 4600F306 */  .word 0x4600F306
    /* 800DFFF8 10000010 */  beq $zero, $zero, 0x800E003C
    /* 800DFFFC 8FBF0014 */  lw $ra, 20($sp)
    /* 800E0000 271806C0 */  addiu $t8, $t8, 1728
    /* 800E0004 01F88821 */  addu $s1, $t7, $t8
    /* 800E0008 8E390028 */  lw $t9, 40($s1)
    /* 800E000C 2401FFFF */  addiu $at, $zero, -1
    /* 800E0010 5721000A */  .word 0x5721000A
    /* 800E0014 8FBF0014 */  lw $ra, 20($sp)
    /* 800E0018 0C024701 */  jal 0x80091C04
    /* 800E001C 8E240000 */  lw $a0, 0($s1)
    /* 800E0020 0C03552F */  jal 0x800D54BC
    /* 800E0024 02202025 */  or $a0, $s1, $zero
    /* 800E0028 0C024701 */  jal 0x80091C04
    /* 800E002C 8E240014 */  lw $a0, 20($s1)
    /* 800E0030 0C03552F */  jal 0x800D54BC
    /* 800E0034 26240014 */  addiu $a0, $s1, 20
    /* 800E0038 8FBF0014 */  lw $ra, 20($sp)
    /* 800E003C 27BD0018 */  addiu $sp, $sp, 24
    /* 800E0040 03E00008 */  jr $ra
    /* 800E0044 00000000 */  nop
