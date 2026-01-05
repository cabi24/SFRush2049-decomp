# Source: game_code.bin (decompressed)
# Address: 0x800EA600

glabel anim_frame_update
    /* 800EA600 44814000 */  .word 0x44814000
    /* 800EA604 C48A0000 */  lwc1 $f10, 0($a0)
    /* 800EA608 3C0D8015 */  lui $t5, 0x8015
    /* 800EA60C 25AD51D8 */  addiu $t5, $t5, 20952
    /* 800EA610 46085182 */  .word 0x46085182
    /* 800EA614 006D1021 */  addu $v0, $v1, $t5
    /* 800EA618 C4440000 */  lwc1 $f4, 0($v0)
    /* 800EA61C 46062280 */  .word 0x46062280
    /* 800EA620 1000000A */  beq $zero, $zero, 0x800EA64C
    /* 800EA624 E44A0000 */  swc1 $f10, 0($v0)
    /* 800EA628 3C0E8015 */  lui $t6, 0x8015
    /* 800EA62C 3C048003 */  lui $a0, 0x8003
    /* 800EA630 25CE51D8 */  addiu $t6, $t6, 20952
    /* 800EA634 2484EB94 */  addiu $a0, $a0, -5228
    /* 800EA638 006E1021 */  addu $v0, $v1, $t6
    /* 800EA63C C4480000 */  lwc1 $f8, 0($v0)
    /* 800EA640 C4840000 */  lwc1 $f4, 0($a0)
    /* 800EA644 46044181 */  .word 0x46044181
    /* 800EA648 E4460000 */  swc1 $f6, 0($v0)
    /* 800EA64C 44808000 */  .word 0x44808000
    /* 800EA650 C4420000 */  lwc1 $f2, 0($v0)
    /* 800EA654 28A10003 */  slti $at, $a1, 3
    /* 800EA658 4610103C */  .word 0x4610103C
    /* 800EA65C 00000000 */  nop
    /* 800EA660 45020005 */  .word 0x45020005
    /* 800EA664 4602903C */  .word 0x4602903C
    /* 800EA668 E4500000 */  swc1 $f16, 0($v0)
    /* 800EA66C 10000007 */  beq $zero, $zero, 0x800EA68C
    /* 800EA670 C4420000 */  lwc1 $f2, 0($v0)
    /* 800EA674 4602903C */  .word 0x4602903C
    /* 800EA678 00000000 */  nop
    /* 800EA67C 45000003 */  .word 0x45000003
    /* 800EA680 00000000 */  nop
    /* 800EA684 E4520000 */  swc1 $f18, 0($v0)
    /* 800EA688 C4420000 */  lwc1 $f2, 0($v0)
    /* 800EA68C 10200008 */  beq $at, $zero, 0x800EA6B0
    /* 800EA690 3C0F8015 */  lui $t7, 0x8015
    /* 800EA694 25EF5210 */  addiu $t7, $t7, 21008
    /* 800EA698 006F4021 */  addu $t0, $v1, $t7
    /* 800EA69C C50A0000 */  lwc1 $f10, 0($t0)
    /* 800EA6A0 C4880000 */  lwc1 $f8, 0($a0)
    /* 800EA6A4 46085101 */  .word 0x46085101
    /* 800EA6A8 10000034 */  beq $zero, $zero, 0x800EA77C
    /* 800EA6AC E5040000 */  swc1 $f4, 0($t0)
    /* 800EA6B0 3C018015 */  lui $at, 0x8015
    /* 800EA6B4 00230821 */  addu $at, $at, $v1
    /* 800EA6B8 C4202680 */  lwc1 $f0, 9856($at)
    /* 800EA6BC 3C013FC0 */  lui $at, 0x3FC0
    /* 800EA6C0 44813000 */  .word 0x44813000
    /* 800EA6C4 00000000 */  nop
    /* 800EA6C8 4600303C */  .word 0x4600303C
    /* 800EA6CC 00000000 */  nop
    /* 800EA6D0 4500000B */  .word 0x4500000B
    /* 800EA6D4 3C014120 */  lui $at, 0x4120
    /* 800EA6D8 44814000 */  .word 0x44814000
    /* 800EA6DC C48A0000 */  lwc1 $f10, 0($a0)
    /* 800EA6E0 3C188015 */  lui $t8, 0x8015
    /* 800EA6E4 27185210 */  addiu $t8, $t8, 21008
    /* 800EA6E8 46085102 */  .word 0x46085102
    /* 800EA6EC 00784021 */  addu $t0, $v1, $t8
    /* 800EA6F0 C5060000 */  lwc1 $f6, 0($t0)
    /* 800EA6F4 46043280 */  .word 0x46043280
    /* 800EA6F8 10000020 */  beq $zero, $zero, 0x800EA77C
    /* 800EA6FC E50A0000 */  swc1 $f10, 0($t0)
    /* 800EA700 3C013F00 */  lui $at, 0x3F00
    /* 800EA704 44814000 */  .word 0x44814000
    /* 800EA708 00000000 */  nop
    /* 800EA70C 4600403C */  .word 0x4600403C
    /* 800EA710 00000000 */  nop
    /* 800EA714 45000012 */  .word 0x45000012
    /* 800EA718 3C013FC0 */  lui $at, 0x3FC0
    /* 800EA71C 44813000 */  .word 0x44813000
    /* 800EA720 3C018012 */  lui $at, 0x8012
    /* 800EA724 C42A44FC */  lwc1 $f10, 17660($at)
    /* 800EA728 46003101 */  .word 0x46003101
    /* 800EA72C 3C018012 */  lui $at, 0x8012
    /* 800EA730 C4264500 */  lwc1 $f6, 17664($at)
    /* 800EA734 3C198015 */  lui $t9, 0x8015
    /* 800EA738 460A2202 */  .word 0x460A2202
    /* 800EA73C C48A0000 */  lwc1 $f10, 0($a0)
    /* 800EA740 27395210 */  addiu $t9, $t9, 21008
    /* 800EA744 00794021 */  addu $t0, $v1, $t9
    /* 800EA748 46083100 */  .word 0x46083100
    /* 800EA74C C5080000 */  lwc1 $f8, 0($t0)
    /* 800EA750 46045183 */  .word 0x46045183
    /* 800EA754 46064280 */  .word 0x46064280
    /* 800EA758 10000008 */  beq $zero, $zero, 0x800EA77C
    /* 800EA75C E50A0000 */  swc1 $f10, 0($t0)
    /* 800EA760 3C0A8015 */  lui $t2, 0x8015
    /* 800EA764 254A5210 */  addiu $t2, $t2, 21008
    /* 800EA768 006A4021 */  addu $t0, $v1, $t2
    /* 800EA76C C5040000 */  lwc1 $f4, 0($t0)
    /* 800EA770 C4880000 */  lwc1 $f8, 0($a0)
    /* 800EA774 46082180 */  .word 0x46082180
    /* 800EA778 E5060000 */  swc1 $f6, 0($t0)
    /* 800EA77C C5000000 */  lwc1 $f0, 0($t0)
    /* 800EA780 27A20074 */  addiu $v0, $sp, 116
    /* 800EA784 01201825 */  or $v1, $t1, $zero
    /* 800EA788 4610003C */  .word 0x4610003C
    /* 800EA78C 27A40080 */  addiu $a0, $sp, 128
    /* 800EA790 45020004 */  .word 0x45020004
    /* 800EA794 4600903C */  .word 0x4600903C
    /* 800EA798 10000006 */  beq $zero, $zero, 0x800EA7B4
    /* 800EA79C E5100000 */  swc1 $f16, 0($t0)
    /* 800EA7A0 4600903C */  .word 0x4600903C
    /* 800EA7A4 00000000 */  nop
    /* 800EA7A8 45000002 */  .word 0x45000002
    /* 800EA7AC 00000000 */  nop
    /* 800EA7B0 E5120000 */  swc1 $f18, 0($t0)
    /* 800EA7B4 C46A0044 */  lwc1 $f10, 68($v1)
    /* 800EA7B8 24420004 */  addiu $v0, $v0, 4
    /* 800EA7BC 0044082B */  sltu $at, $v0, $a0
    /* 800EA7C0 46005107 */  .word 0x46005107
    /* 800EA7C4 24630004 */  addiu $v1, $v1, 4
    /* 800EA7C8 1420FFFA */  bne $at, $zero, 0x800EA7B4
    /* 800EA7CC E444FFFC */  swc1 $f4, -4($v0)
    /* 800EA7D0 4612103C */  .word 0x4612103C
    /* 800EA7D4 00000000 */  nop
    /* 800EA7D8 45020030 */  .word 0x45020030
    /* 800EA7DC 46028032 */  .word 0x46028032
    /* 800EA7E0 4612703C */  .word 0x4612703C
    /* 800EA7E4 C7A80074 */  lwc1 $f8, 116($sp)
    /* 800EA7E8 45020008 */  .word 0x45020008
    /* 800EA7EC C52400AC */  lwc1 $f4, 172($t1)
    /* 800EA7F0 C7A60078 */  lwc1 $f6, 120($sp)
    /* 800EA7F4 C7AA007C */  lwc1 $f10, 124($sp)
    /* 800EA7F8 E7A80068 */  swc1 $f8, 104($sp)
    /* 800EA7FC E7A6006C */  swc1 $f6, 108($sp)
    /* 800EA800 10000025 */  beq $zero, $zero, 0x800EA898
    /* 800EA804 E7AA0070 */  swc1 $f10, 112($sp)
    /* 800EA808 C52400AC */  lwc1 $f4, 172($t1)
    /* 800EA80C 44804000 */  .word 0x44804000
    /* 800EA810 28A10003 */  slti $at, $a1, 3
    /* 800EA814 4608203C */  .word 0x4608203C
    /* 800EA818 00000000 */  nop
    /* 800EA81C 45020011 */  .word 0x45020011
    /* 800EA820 3C01BF80 */  lui $at, 0xBF80
    /* 800EA824 1420000E */  bne $at, $zero, 0x800EA860
    /* 800EA828 3C013F80 */  lui $at, 0x3F80
    /* 800EA82C 44813000 */  .word 0x44813000
    /* 800EA830 C52A0014 */  lwc1 $f10, 20($t1)
    /* 800EA834 44819000 */  .word 0x44819000
    /* 800EA838 460E3003 */  .word 0x460E3003
    /* 800EA83C 46005102 */  .word 0x46005102
    /* 800EA840 E7A40068 */  swc1 $f4, 104($sp)
    /* 800EA844 C5280018 */  lwc1 $f8, 24($t1)
    /* 800EA848 46004182 */  .word 0x46004182
    /* 800EA84C E7A6006C */  swc1 $f6, 108($sp)
    /* 800EA850 C52A001C */  lwc1 $f10, 28($t1)
    /* 800EA854 46005102 */  .word 0x46005102
    /* 800EA858 1000000F */  beq $zero, $zero, 0x800EA898
    /* 800EA85C E7A40070 */  swc1 $f4, 112($sp)
    /* 800EA860 3C01BF80 */  lui $at, 0xBF80
    /* 800EA864 44814000 */  .word 0x44814000
    /* 800EA868 C5260014 */  lwc1 $f6, 20($t1)
    /* 800EA86C 3C013F80 */  lui $at, 0x3F80
    /* 800EA870 460E4003 */  .word 0x460E4003
    /* 800EA874 44819000 */  .word 0x44819000
    /* 800EA878 46003282 */  .word 0x46003282
    /* 800EA87C E7AA0068 */  swc1 $f10, 104($sp)
    /* 800EA880 C5240018 */  lwc1 $f4, 24($t1)
    /* 800EA884 46002202 */  .word 0x46002202
    /* 800EA888 E7A8006C */  swc1 $f8, 108($sp)
    /* 800EA88C C526001C */  lwc1 $f6, 28($t1)
    /* 800EA890 46003282 */  .word 0x46003282
    /* 800EA894 E7AA0070 */  swc1 $f10, 112($sp)
    /* 800EA898 46028032 */  .word 0x46028032
    /* 800EA89C 00000000 */  nop
    /* 800EA8A0 45020009 */  .word 0x45020009
    /* 800EA8A4 46029032 */  .word 0x46029032
    /* 800EA8A8 C7A20068 */  lwc1 $f2, 104($sp)
    /* 800EA8AC C7AE006C */  lwc1 $f14, 108($sp)
    /* 800EA8B0 C7B00070 */  lwc1 $f16, 112($sp)
    /* 800EA8B4 E7A2005C */  swc1 $f2, 92($sp)
    /* 800EA8B8 E7AE0060 */  swc1 $f14, 96($sp)
    /* 800EA8BC 10000036 */  beq $zero, $zero, 0x800EA998
    /* 800EA8C0 E7B00064 */  swc1 $f16, 100($sp)
    /* 800EA8C4 46029032 */  .word 0x46029032
    /* 800EA8C8 27A40068 */  addiu $a0, $sp, 104
    /* 800EA8CC 27A50074 */  addiu $a1, $sp, 116
    /* 800EA8D0 24060003 */  addiu $a2, $zero, 3
    /* 800EA8D4 45000008 */  .word 0x45000008
    /* 800EA8D8 27AB005C */  addiu $t3, $sp, 92
    /* 800EA8DC C7A40074 */  lwc1 $f4, 116($sp)
    /* 800EA8E0 C7A80078 */  lwc1 $f8, 120($sp)
    /* 800EA8E4 C7A6007C */  lwc1 $f6, 124($sp)
    /* 800EA8E8 E7A4005C */  swc1 $f4, 92($sp)
    /* 800EA8EC E7A80060 */  swc1 $f8, 96($sp)
    /* 800EA8F0 10000029 */  beq $zero, $zero, 0x800EA998
    /* 800EA8F4 E7A60064 */  swc1 $f6, 100($sp)
    /* 800EA8F8 44078000 */  .word 0x44078000
    /* 800EA8FC E7B20010 */  swc1 $f18, 16($sp)
    /* 800EA900 E7A20014 */  swc1 $f2, 20($sp)
    /* 800EA904 AFAB0018 */  sw $t3, 24($sp)
    /* 800EA908 AFA8002C */  sw $t0, 44($sp)
    /* 800EA90C AFA900A0 */  sw $t1, 160($sp)
    /* 800EA910 0C033F7B */  jal 0x800CFDEC
    /* 800EA914 E7AC0040 */  swc1 $f12, 64($sp)
    /* 800EA918 0C022CF2 */  jal 0x8008B3C8
    /* 800EA91C 27A4005C */  addiu $a0, $sp, 92
    /* 800EA920 3C018012 */  lui $at, 0x8012
    /* 800EA924 C42A4504 */  lwc1 $f10, 17668($at)
    /* 800EA928 8FA8002C */  lw $t0, 44($sp)
    /* 800EA92C 8FA900A0 */  lw $t1, 160($sp)
    /* 800EA930 460A003C */  .word 0x460A003C
    /* 800EA934 C7AC0040 */  lwc1 $f12, 64($sp)
    /* 800EA938 C7A40074 */  lwc1 $f4, 116($sp)
    /* 800EA93C 3C013F80 */  lui $at, 0x3F80
    /* 800EA940 4502000A */  .word 0x4502000A
    /* 800EA944 44815000 */  .word 0x44815000
    /* 800EA948 C7A80078 */  lwc1 $f8, 120($sp)
    /* 800EA94C C7A6007C */  lwc1 $f6, 124($sp)
    /* 800EA950 3C013F80 */  lui $at, 0x3F80
    /* 800EA954 44819000 */  .word 0x44819000
    /* 800EA958 E7A4005C */  swc1 $f4, 92($sp)
    /* 800EA95C E7A80060 */  swc1 $f8, 96($sp)
    /* 800EA960 1000000D */  beq $zero, $zero, 0x800EA998
    /* 800EA964 E7A60064 */  swc1 $f6, 100($sp)
    /* 800EA968 44815000 */  .word 0x44815000
    /* 800EA96C C7A4005C */  lwc1 $f4, 92($sp)
    /* 800EA970 C7A60060 */  lwc1 $f6, 96($sp)
    /* 800EA974 46005083 */  .word 0x46005083
    /* 800EA978 44819000 */  .word 0x44819000
    /* 800EA97C 46022202 */  .word 0x46022202
    /* 800EA980 C7A40064 */  lwc1 $f4, 100($sp)
    /* 800EA984 46023282 */  .word 0x46023282
    /* 800EA988 E7A8005C */  swc1 $f8, 92($sp)
    /* 800EA98C 46022202 */  .word 0x46022202
    /* 800EA990 E7AA0060 */  swc1 $f10, 96($sp)
    /* 800EA994 E7A80064 */  swc1 $f8, 100($sp)
    /* 800EA998 44803000 */  .word 0x44803000
    /* 800EA99C C7A2005C */  lwc1 $f2, 92($sp)
    /* 800EA9A0 C7AE0060 */  lwc1 $f14, 96($sp)
    /* 800EA9A4 460C303C */  .word 0x460C303C
    /* 800EA9A8 C7B00064 */  lwc1 $f16, 100($sp)
    /* 800EA9AC 4500003C */  .word 0x4500003C
    /* 800EA9B0 3C013F80 */  lui $at, 0x3F80
    /* 800EA9B4 44815000 */  .word 0x44815000
    /* 800EA9B8 8FAC009C */  lw $t4, 156($sp)
    /* 800EA9BC 3C0E8015 */  lui $t6, 0x8015
    /* 800EA9C0 460C5001 */  .word 0x460C5001
    /* 800EA9C4 000C6880 */  sll $t5, $t4, 2
    /* 800EA9C8 01AC6823 */  subu $t5, $t5, $t4
    /* 800EA9CC 000D6880 */  sll $t5, $t5, 2
    /* 800EA9D0 46001082 */  .word 0x46001082
    /* 800EA9D4 25CE5178 */  addiu $t6, $t6, 20856
    /* 800EA9D8 01AE2821 */  addu $a1, $t5, $t6
    /* 800EA9DC 46007382 */  .word 0x46007382
    /* 800EA9E0 C4A40000 */  lwc1 $f4, 0($a1)
    /* 800EA9E4 C4A60004 */  lwc1 $f6, 4($a1)
    /* 800EA9E8 46008402 */  .word 0x46008402
    /* 800EA9EC 27A4005C */  addiu $a0, $sp, 92
    /* 800EA9F0 AFA8002C */  sw $t0, 44($sp)
    /* 800EA9F4 460C2202 */  .word 0x460C2202
    /* 800EA9F8 C4A40008 */  lwc1 $f4, 8($a1)
    /* 800EA9FC AFA900A0 */  sw $t1, 160($sp)
    /* 800EAA00 460C3282 */  .word 0x460C3282
    /* 800EAA04 E7AC0040 */  swc1 $f12, 64($sp)
    /* 800EAA08 46081080 */  .word 0x46081080
    /* 800EAA0C 460C2202 */  .word 0x460C2202
    /* 800EAA10 460A7380 */  .word 0x460A7380
    /* 800EAA14 E7A2005C */  swc1 $f2, 92($sp)
    /* 800EAA18 E7AE0060 */  swc1 $f14, 96($sp)
    /* 800EAA1C 46088400 */  .word 0x46088400
    /* 800EAA20 0C022CF2 */  jal 0x8008B3C8
    /* 800EAA24 E7B00064 */  swc1 $f16, 100($sp)
    /* 800EAA28 3C018012 */  lui $at, 0x8012
    /* 800EAA2C C4264508 */  lwc1 $f6, 17672($at)
    /* 800EAA30 8FA8002C */  lw $t0, 44($sp)
    /* 800EAA34 8FA900A0 */  lw $t1, 160($sp)
    /* 800EAA38 4606003C */  .word 0x4606003C
    /* 800EAA3C C7AC0040 */  lwc1 $f12, 64($sp)
    /* 800EAA40 C7AA0074 */  lwc1 $f10, 116($sp)
    /* 800EAA44 3C013F80 */  lui $at, 0x3F80
    /* 800EAA48 4502000A */  .word 0x4502000A
    /* 800EAA4C 44813000 */  .word 0x44813000
    /* 800EAA50 C7A40078 */  lwc1 $f4, 120($sp)
    /* 800EAA54 C7A8007C */  lwc1 $f8, 124($sp)
    /* 800EAA58 3C013F80 */  lui $at, 0x3F80
    /* 800EAA5C 44819000 */  .word 0x44819000
    /* 800EAA60 E7AA005C */  swc1 $f10, 92($sp)
    /* 800EAA64 E7A40060 */  swc1 $f4, 96($sp)
    /* 800EAA68 1000000D */  beq $zero, $zero, 0x800EAAA0
    /* 800EAA6C E7A80064 */  swc1 $f8, 100($sp)
    /* 800EAA70 44813000 */  .word 0x44813000
    /* 800EAA74 C7AA005C */  lwc1 $f10, 92($sp)
    /* 800EAA78 C7A80060 */  lwc1 $f8, 96($sp)
    /* 800EAA7C 46003083 */  .word 0x46003083
    /* 800EAA80 44819000 */  .word 0x44819000
    /* 800EAA84 46025102 */  .word 0x46025102
    /* 800EAA88 C7AA0064 */  lwc1 $f10, 100($sp)
    /* 800EAA8C 46024182 */  .word 0x46024182
    /* 800EAA90 E7A4005C */  swc1 $f4, 92($sp)
    /* 800EAA94 46025102 */  .word 0x46025102
    /* 800EAA98 E7A60060 */  swc1 $f6, 96($sp)
    /* 800EAA9C E7A40064 */  swc1 $f4, 100($sp)
    /* 800EAAA0 8FAF009C */  lw $t7, 156($sp)
    /* 800EAAA4 3C198015 */  lui $t9, 0x8015
    /* 800EAAA8 27395178 */  addiu $t9, $t9, 20856
    /* 800EAAAC 000FC080 */  sll $t8, $t7, 2
    /* 800EAAB0 030FC023 */  subu $t8, $t8, $t7
    /* 800EAAB4 0018C080 */  sll $t8, $t8, 2
    /* 800EAAB8 C7A2005C */  lwc1 $f2, 92($sp)
    /* 800EAABC C7AE0060 */  lwc1 $f14, 96($sp)
    /* 800EAAC0 C7B00064 */  lwc1 $f16, 100($sp)
    /* 800EAAC4 03192821 */  addu $a1, $t8, $t9
    /* 800EAAC8 AFB80034 */  sw $t8, 52($sp)
    /* 800EAACC C5000000 */  lwc1 $f0, 0($t0)
    /* 800EAAD0 27A20074 */  addiu $v0, $sp, 116
    /* 800EAAD4 01201825 */  or $v1, $t1, $zero
    /* 800EAAD8 27A40080 */  addiu $a0, $sp, 128
    /* 800EAADC E4A20000 */  swc1 $f2, 0($a1)
    /* 800EAAE0 E4AE0004 */  swc1 $f14, 4($a1)
    /* 800EAAE4 E4B00008 */  swc1 $f16, 8($a1)
    /* 800EAAE8 C4680038 */  lwc1 $f8, 56($v1)
    /* 800EAAEC 24420004 */  addiu $v0, $v0, 4
    /* 800EAAF0 24630004 */  addiu $v1, $v1, 4
    /* 800EAAF4 1444FFFC */  bne $v0, $a0, 0x800EAAE8
    /* 800EAAF8 E448FFFC */  swc1 $f8, -4($v0)
    /* 800EAAFC 4612003C */  .word 0x4612003C
    /* 800EAB00 44804000 */  .word 0x44804000
    /* 800EAB04 45000008 */  .word 0x45000008
    /* 800EAB08 3C028011 */  lui $v0, 0x8011
    /* 800EAB0C 244241BC */  addiu $v0, $v0, 16828
    /* 800EAB10 C4460000 */  lwc1 $f6, 0($v0)
    /* 800EAB14 C44A0004 */  lwc1 $f10, 4($v0)
    /* 800EAB18 C4440008 */  lwc1 $f4, 8($v0)
    /* 800EAB1C E7A60068 */  swc1 $f6, 104($sp)
    /* 800EAB20 E7AA006C */  swc1 $f10, 108($sp)
    /* 800EAB24 E7A40070 */  swc1 $f4, 112($sp)
    /* 800EAB28 46004032 */  .word 0x46004032
    /* 800EAB2C 00000000 */  nop
    /* 800EAB30 45020006 */  .word 0x45020006
    /* 800EAB34 46009032 */  .word 0x46009032
    /* 800EAB38 C7AE0068 */  lwc1 $f14, 104($sp)
    /* 800EAB3C C7B0006C */  lwc1 $f16, 108($sp)
    /* 800EAB40 1000002B */  beq $zero, $zero, 0x800EABF0
    /* 800EAB44 C7B20070 */  lwc1 $f18, 112($sp)
    /* 800EAB48 46009032 */  .word 0x46009032
    /* 800EAB4C 27A40068 */  addiu $a0, $sp, 104
    /* 800EAB50 27A50074 */  addiu $a1, $sp, 116
    /* 800EAB54 24060003 */  addiu $a2, $zero, 3
    /* 800EAB58 45000005 */  .word 0x45000005
    /* 800EAB5C 24070000 */  addiu $a3, $zero, 0
    /* 800EAB60 C7AE0074 */  lwc1 $f14, 116($sp)
    /* 800EAB64 C7B00078 */  lwc1 $f16, 120($sp)
    /* 800EAB68 10000021 */  beq $zero, $zero, 0x800EABF0
    /* 800EAB6C C7B2007C */  lwc1 $f18, 124($sp)
    /* 800EAB70 27AA0050 */  addiu $t2, $sp, 80
    /* 800EAB74 AFAA0018 */  sw $t2, 24($sp)
    /* 800EAB78 E7B20010 */  swc1 $f18, 16($sp)
    /* 800EAB7C E7A00014 */  swc1 $f0, 20($sp)
    /* 800EAB80 AFA900A0 */  sw $t1, 160($sp)
    /* 800EAB84 0C033F7B */  jal 0x800CFDEC
    /* 800EAB88 E7AC0040 */  swc1 $f12, 64($sp)
    /* 800EAB8C 0C022CF2 */  jal 0x8008B3C8
    /* 800EAB90 27A40050 */  addiu $a0, $sp, 80
    /* 800EAB94 3C018012 */  lui $at, 0x8012
    /* 800EAB98 C426450C */  lwc1 $f6, 17676($at)
    /* 800EAB9C 8FA900A0 */  lw $t1, 160($sp)
    /* 800EABA0 C7AC0040 */  lwc1 $f12, 64($sp)
    /* 800EABA4 4606003C */  .word 0x4606003C
    /* 800EABA8 C7AE0074 */  lwc1 $f14, 116($sp)
    /* 800EABAC C7B00078 */  lwc1 $f16, 120($sp)
    /* 800EABB0 3C013F80 */  lui $at, 0x3F80
    /* 800EABB4 45020004 */  .word 0x45020004
    /* 800EABB8 44815000 */  .word 0x44815000
    /* 800EABBC 1000000C */  beq $zero, $zero, 0x800EABF0
    /* 800EABC0 C7B2007C */  lwc1 $f18, 124($sp)
    /* 800EABC4 44815000 */  .word 0x44815000
    /* 800EABC8 C7AE0050 */  lwc1 $f14, 80($sp)
    /* 800EABCC C7B00054 */  lwc1 $f16, 84($sp)
    /* 800EABD0 46005083 */  .word 0x46005083
    /* 800EABD4 C7B20058 */  lwc1 $f18, 88($sp)
    /* 800EABD8 46027382 */  .word 0x46027382
    /* 800EABDC 00000000 */  nop
    /* 800EABE0 46028402 */  .word 0x46028402
    /* 800EABE4 00000000 */  nop
    /* 800EABE8 46029482 */  .word 0x46029482
    /* 800EABEC 00000000 */  nop
    /* 800EABF0 44802000 */  .word 0x44802000
    /* 800EABF4 00000000 */  nop
    /* 800EABF8 460C203C */  .word 0x460C203C
    /* 800EABFC 00000000 */  nop
    /* 800EAC00 45000030 */  .word 0x45000030
    /* 800EAC04 3C013F80 */  lui $at, 0x3F80
    /* 800EAC08 44814000 */  .word 0x44814000
    /* 800EAC0C 8FAB0034 */  lw $t3, 52($sp)
    /* 800EAC10 3C0C8015 */  lui $t4, 0x8015
    /* 800EAC14 460C4001 */  .word 0x460C4001
    /* 800EAC18 258C51A8 */  addiu $t4, $t4, 20904
    /* 800EAC1C 016C1821 */  addu $v1, $t3, $t4
    /* 800EAC20 C4660000 */  lwc1 $f6, 0($v1)
    /* 800EAC24 46007382 */  .word 0x46007382
    /* 800EAC28 C4640004 */  lwc1 $f4, 4($v1)
    /* 800EAC2C 27A40050 */  addiu $a0, $sp, 80
    /* 800EAC30 46008402 */  .word 0x46008402
    /* 800EAC34 AFA900A0 */  sw $t1, 160($sp)
    /* 800EAC38 46009482 */  .word 0x46009482
    /* 800EAC3C 00000000 */  nop
    /* 800EAC40 460C3282 */  .word 0x460C3282
    /* 800EAC44 C4660008 */  lwc1 $f6, 8($v1)
    /* 800EAC48 460C2202 */  .word 0x460C2202
    /* 800EAC4C 460A7380 */  .word 0x460A7380
    /* 800EAC50 460C3282 */  .word 0x460C3282
    /* 800EAC54 46088400 */  .word 0x46088400
    /* 800EAC58 E7AE0050 */  swc1 $f14, 80($sp)
    /* 800EAC5C E7B00054 */  swc1 $f16, 84($sp)
    /* 800EAC60 460A9480 */  .word 0x460A9480
    /* 800EAC64 0C022CF2 */  jal 0x8008B3C8
    /* 800EAC68 E7B20058 */  swc1 $f18, 88($sp)
    /* 800EAC6C 3C018012 */  lui $at, 0x8012
    /* 800EAC70 C4244510 */  lwc1 $f4, 17680($at)
    /* 800EAC74 8FA900A0 */  lw $t1, 160($sp)
    /* 800EAC78 C7AE0074 */  lwc1 $f14, 116($sp)
    /* 800EAC7C 4604003C */  .word 0x4604003C
    /* 800EAC80 C7B00078 */  lwc1 $f16, 120($sp)
    /* 800EAC84 3C013F80 */  lui $at, 0x3F80
    /* 800EAC88 45020004 */  .word 0x45020004
    /* 800EAC8C 44814000 */  .word 0x44814000
    /* 800EAC90 1000000C */  beq $zero, $zero, 0x800EACC4
    /* 800EAC94 C7B2007C */  lwc1 $f18, 124($sp)
    /* 800EAC98 44814000 */  .word 0x44814000
    /* 800EAC9C C7AE0050 */  lwc1 $f14, 80($sp)
    /* 800EACA0 C7B00054 */  lwc1 $f16, 84($sp)
    /* 800EACA4 46004083 */  .word 0x46004083
    /* 800EACA8 C7B20058 */  lwc1 $f18, 88($sp)
    /* 800EACAC 46027382 */  .word 0x46027382
    /* 800EACB0 00000000 */  nop
    /* 800EACB4 46028402 */  .word 0x46028402
    /* 800EACB8 00000000 */  nop
    /* 800EACBC 46029482 */  .word 0x46029482
    /* 800EACC0 00000000 */  nop
    /* 800EACC4 C7A6005C */  lwc1 $f6, 92($sp)
    /* 800EACC8 C7AA0094 */  lwc1 $f10, 148($sp)
    /* 800EACCC C7A40060 */  lwc1 $f4, 96($sp)
    /* 800EACD0 C7A80064 */  lwc1 $f8, 100($sp)
    /* 800EACD4 460A3002 */  .word 0x460A3002
    /* 800EACD8 C7A60090 */  lwc1 $f6, 144($sp)
    /* 800EACDC 8FAF009C */  lw $t7, 156($sp)
    /* 800EACE0 460A2082 */  .word 0x460A2082
    /* 800EACE4 8FAD0034 */  lw $t5, 52($sp)
    /* 800EACE8 000FC080 */  sll $t8, $t7, 2
    /* 800EACEC 460A4302 */  .word 0x460A4302
    /* 800EACF0 030FC021 */  addu $t8, $t8, $t7
    /* 800EACF4 3C0E8015 */  lui $t6, 0x8015
    /* 800EACF8 46067102 */  .word 0x46067102
    /* 800EACFC 0018C080 */  sll $t8, $t8, 2
    /* 800EAD00 25CE51A8 */  addiu $t6, $t6, 20904
    /* 800EAD04 46068202 */  .word 0x46068202
    /* 800EAD08 3C198015 */  lui $t9, 0x8015
    /* 800EAD0C 030FC023 */  subu $t8, $t8, $t7
    /* 800EAD10 46069282 */  .word 0x46069282
    /* 800EAD14 0018C0C0 */  sll $t8, $t8, 3
    /* 800EAD18 46040000 */  .word 0x46040000
    /* 800EAD1C 27390B70 */  addiu $t9, $t9, 2928
    /* 800EAD20 01AE1821 */  addu $v1, $t5, $t6
    /* 800EAD24 46081080 */  .word 0x46081080
    /* 800EAD28 03191021 */  addu $v0, $t8, $t9
    /* 800EAD2C E46E0000 */  swc1 $f14, 0($v1)
    /* 800EAD30 460A6300 */  .word 0x460A6300
    /* 800EAD34 E4700004 */  swc1 $f16, 4($v1)
    /* 800EAD38 E4720008 */  swc1 $f18, 8($v1)
    /* 800EAD3C 24450078 */  addiu $a1, $v0, 120
    /* 800EAD40 E7AC004C */  swc1 $f12, 76($sp)
    /* 800EAD44 AFA20038 */  sw $v0, 56($sp)
    /* 800EAD48 E7A20048 */  swc1 $f2, 72($sp)
    /* 800EAD4C E7A00044 */  swc1 $f0, 68($sp)
    /* 800EAD50 27A40044 */  addiu $a0, $sp, 68
    /* 800EAD54 AFA900A0 */  sw $t1, 160($sp)
    /* 800EAD58 E7AE0050 */  swc1 $f14, 80($sp)
    /* 800EAD5C E7B00054 */  swc1 $f16, 84($sp)
    /* 800EAD60 0C022D1D */  jal 0x8008B474
    /* 800EAD64 E7B20058 */  swc1 $f18, 88($sp)
    /* 800EAD68 8FA20038 */  lw $v0, 56($sp)
    /* 800EAD6C C7A40084 */  lwc1 $f4, 132($sp)
    /* 800EAD70 8FA900A0 */  lw $t1, 160($sp)
    /* 800EAD74 C4400078 */  lwc1 $f0, 120($v0)
    /* 800EAD78 C442007C */  lwc1 $f2, 124($v0)
    /* 800EAD7C C44C0080 */  lwc1 $f12, 128($v0)
    /* 800EAD80 46040202 */  .word 0x46040202
    /* 800EAD84 24010002 */  addiu $at, $zero, 2
    /* 800EAD88 46041182 */  .word 0x46041182
    /* 800EAD8C 00000000 */  nop
    /* 800EAD90 46046282 */  .word 0x46046282
    /* 800EAD94 E7A80044 */  swc1 $f8, 68($sp)
    /* 800EAD98 E7A60048 */  swc1 $f6, 72($sp)
    /* 800EAD9C E7AA004C */  swc1 $f10, 76($sp)
    /* 800EADA0 8123035D */  lb $v1, 861($t1)
    /* 800EADA4 14610017 */  bne $v1, $at, 0x800EAE04
    /* 800EADA8 00000000 */  nop
    /* 800EADAC 3C014080 */  lui $at, 0x4080
    /* 800EADB0 C7AE0050 */  lwc1 $f14, 80($sp)
    /* 800EADB4 44813000 */  .word 0x44813000
    /* 800EADB8 C7A80044 */  lwc1 $f8, 68($sp)
    /* 800EADBC C7B00054 */  lwc1 $f16, 84($sp)
    /* 800EADC0 46067102 */  .word 0x46067102
    /* 800EADC4 C7A60048 */  lwc1 $f6, 72($sp)
    /* 800EADC8 C7B20058 */  lwc1 $f18, 88($sp)
    /* 800EADCC 46044280 */  .word 0x46044280
