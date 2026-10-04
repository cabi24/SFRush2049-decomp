nonmatching func_80019C8C, 0x244

glabel func_80019C8C
    /* 1A88C 80019C8C 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* 1A890 80019C90 3C0E8005 */  lui        $t6, %hi(D_8004FA18)
    /* 1A894 80019C94 91CEFA18 */  lbu        $t6, %lo(D_8004FA18)($t6)
    /* 1A898 80019C98 AFB60030 */  sw         $s6, 0x30($sp)
    /* 1A89C 80019C9C 3C168005 */  lui        $s6, %hi(D_8004BEB8)
    /* 1A8A0 80019CA0 26D6BEB8 */  addiu      $s6, $s6, %lo(D_8004BEB8)
    /* 1A8A4 80019CA4 AFBE0038 */  sw         $fp, 0x38($sp)
    /* 1A8A8 80019CA8 AFB5002C */  sw         $s5, 0x2C($sp)
    /* 1A8AC 80019CAC AFB20020 */  sw         $s2, 0x20($sp)
    /* 1A8B0 80019CB0 AFB00018 */  sw         $s0, 0x18($sp)
    /* 1A8B4 80019CB4 309500FF */  andi       $s5, $a0, 0xFF
    /* 1A8B8 80019CB8 AFBF003C */  sw         $ra, 0x3C($sp)
    /* 1A8BC 80019CBC AFB70034 */  sw         $s7, 0x34($sp)
    /* 1A8C0 80019CC0 AFB40028 */  sw         $s4, 0x28($sp)
    /* 1A8C4 80019CC4 AFB30024 */  sw         $s3, 0x24($sp)
    /* 1A8C8 80019CC8 AFB1001C */  sw         $s1, 0x1C($sp)
    /* 1A8CC 80019CCC AFA40058 */  sw         $a0, 0x58($sp)
    /* 1A8D0 80019CD0 AFA5005C */  sw         $a1, 0x5C($sp)
    /* 1A8D4 80019CD4 AFA60060 */  sw         $a2, 0x60($sp)
    /* 1A8D8 80019CD8 241EFFFF */  addiu      $fp, $zero, -0x1
    /* 1A8DC 80019CDC 02C08025 */  or         $s0, $s6, $zero
    /* 1A8E0 80019CE0 11C00064 */  beqz       $t6, .L80019E74
    /* 1A8E4 80019CE4 00009025 */   or        $s2, $zero, $zero
    /* 1A8E8 80019CE8 241701A0 */  addiu      $s7, $zero, 0x1A0
    /* 1A8EC 80019CEC 8FB4004C */  lw         $s4, 0x4C($sp)
    /* 1A8F0 80019CF0 2413FFFF */  addiu      $s3, $zero, -0x1
    /* 1A8F4 80019CF4 8A0F0060 */  lwl        $t7, 0x60($s0)
  .L80019CF8:
    /* 1A8F8 80019CF8 9A0F0063 */  lwr        $t7, 0x63($s0)
    /* 1A8FC 80019CFC 93B8005F */  lbu        $t8, 0x5F($sp)
    /* 1A900 80019D00 126F0054 */  beq        $s3, $t7, .L80019E54
    /* 1A904 80019D04 00000000 */   nop
    /* 1A908 80019D08 9219004A */  lbu        $t9, 0x4A($s0)
    /* 1A90C 80019D0C 93A80063 */  lbu        $t0, 0x63($sp)
    /* 1A910 80019D10 17190050 */  bne        $t8, $t9, .L80019E54
    /* 1A914 80019D14 00000000 */   nop
    /* 1A918 80019D18 9209004B */  lbu        $t1, 0x4B($s0)
    /* 1A91C 80019D1C 1509004D */  bne        $t0, $t1, .L80019E54
    /* 1A920 80019D20 00000000 */   nop
    /* 1A924 80019D24 8A020024 */  lwl        $v0, 0x24($s0)
    /* 1A928 80019D28 9A020027 */  lwr        $v0, 0x27($s0)
    /* 1A92C 80019D2C 304A0010 */  andi       $t2, $v0, 0x10
    /* 1A930 80019D30 11400048 */  beqz       $t2, .L80019E54
    /* 1A934 80019D34 304B0008 */   andi      $t3, $v0, 0x8
    /* 1A938 80019D38 11600003 */  beqz       $t3, .L80019D48
    /* 1A93C 80019D3C 00026040 */   sll       $t4, $v0, 1
    /* 1A940 80019D40 05810044 */  bgez       $t4, .L80019E54
    /* 1A944 80019D44 00000000 */   nop
  .L80019D48:
    /* 1A948 80019D48 0C00519F */  jal        func_8001467C
    /* 1A94C 80019D4C 02402025 */   or        $a0, $s2, $zero
    /* 1A950 80019D50 10400040 */  beqz       $v0, .L80019E54
    /* 1A954 80019D54 00000000 */   nop
    /* 1A958 80019D58 AFB00044 */  sw         $s0, 0x44($sp)
    /* 1A95C 80019D5C 92010050 */  lbu        $at, 0x50($s0)
    /* 1A960 80019D60 92020051 */  lbu        $v0, 0x51($s0)
    /* 1A964 80019D64 820E00C0 */  lb         $t6, 0xC0($s0)
    /* 1A968 80019D68 00010A00 */  sll        $at, $at, 8
    /* 1A96C 80019D6C 00411025 */  or         $v0, $v0, $at
    /* 1A970 80019D70 24010064 */  addiu      $at, $zero, 0x64
    /* 1A974 80019D74 000E7C00 */  sll        $t7, $t6, 16
    /* 1A978 80019D78 01E1001A */  div        $zero, $t7, $at
    /* 1A97C 80019D7C 0000C012 */  mflo       $t8
    /* 1A980 80019D80 920A004F */  lbu        $t2, 0x4F($s0)
    /* 1A984 80019D84 304800FF */  andi       $t0, $v0, 0xFF
    /* 1A988 80019D88 02570019 */  multu      $s2, $s7
    /* 1A98C 80019D8C 02A84821 */  addu       $t1, $s5, $t0
    /* 1A990 80019D90 012A5823 */  subu       $t3, $t1, $t2
    /* 1A994 80019D94 000B0A02 */  srl        $at, $t3, 8
    /* 1A998 80019D98 A2010050 */  sb         $at, 0x50($s0)
    /* 1A99C 80019D9C 00150A02 */  srl        $at, $s5, 8
    /* 1A9A0 80019DA0 8A0C0024 */  lwl        $t4, 0x24($s0)
    /* 1A9A4 80019DA4 9A0C0027 */  lwr        $t4, 0x27($s0)
    /* 1A9A8 80019DA8 A201004E */  sb         $at, 0x4E($s0)
    /* 1A9AC 80019DAC 3C010008 */  lui        $at, (0x80800 >> 16)
    /* 1A9B0 80019DB0 00026C00 */  sll        $t5, $v0, 16
    /* 1A9B4 80019DB4 34210800 */  ori        $at, $at, (0x80800 & 0xFFFF)
    /* 1A9B8 80019DB8 01B8C821 */  addu       $t9, $t5, $t8
    /* 1A9BC 80019DBC 00007812 */  mflo       $t7
    /* 1A9C0 80019DC0 01817025 */  or         $t6, $t4, $at
    /* 1A9C4 80019DC4 AA190094 */  swl        $t9, 0x94($s0)
    /* 1A9C8 80019DC8 AA00008C */  swl        $zero, 0x8C($s0)
    /* 1A9CC 80019DCC AA0E0024 */  swl        $t6, 0x24($s0)
    /* 1A9D0 80019DD0 02CF8821 */  addu       $s1, $s6, $t7
    /* 1A9D4 80019DD4 BA190097 */  swr        $t9, 0x97($s0)
    /* 1A9D8 80019DD8 A20200C1 */  sb         $v0, 0xC1($s0)
    /* 1A9DC 80019DDC A20B0051 */  sb         $t3, 0x51($s0)
    /* 1A9E0 80019DE0 A215004F */  sb         $s5, 0x4F($s0)
    /* 1A9E4 80019DE4 A20000C0 */  sb         $zero, 0xC0($s0)
    /* 1A9E8 80019DE8 BA00008F */  swr        $zero, 0x8F($s0)
    /* 1A9EC 80019DEC BA0E0027 */  swr        $t6, 0x27($s0)
    /* 1A9F0 80019DF0 0C007AC4 */  jal        func_8001EB10
    /* 1A9F4 80019DF4 02202025 */   or        $a0, $s1, $zero
    /* 1A9F8 80019DF8 17D3000B */  bne        $fp, $s3, .L80019E28
    /* 1A9FC 80019DFC 329800FF */   andi      $t8, $s4, 0xFF
    /* 1AA00 80019E00 AA130010 */  swl        $s3, 0x10($s0)
    /* 1AA04 80019E04 AA130014 */  swl        $s3, 0x14($s0)
    /* 1AA08 80019E08 BA130013 */  swr        $s3, 0x13($s0)
    /* 1AA0C 80019E0C BA130017 */  swr        $s3, 0x17($s0)
    /* 1AA10 80019E10 0C007B38 */  jal        func_8001ECE0
    /* 1AA14 80019E14 02202025 */   or        $a0, $s1, $zero
    /* 1AA18 80019E18 8A140060 */  lwl        $s4, 0x60($s0)
    /* 1AA1C 80019E1C 0040F025 */  or         $fp, $v0, $zero
    /* 1AA20 80019E20 1000000C */  b          .L80019E54
    /* 1AA24 80019E24 9A140063 */   lwr       $s4, 0x63($s0)
  .L80019E28:
    /* 1AA28 80019E28 03170019 */  multu      $t8, $s7
    /* 1AA2C 80019E2C 8A0D0060 */  lwl        $t5, 0x60($s0)
    /* 1AA30 80019E30 9A0D0063 */  lwr        $t5, 0x63($s0)
    /* 1AA34 80019E34 0000C812 */  mflo       $t9
    /* 1AA38 80019E38 02D94021 */  addu       $t0, $s6, $t9
    /* 1AA3C 80019E3C A90D0010 */  swl        $t5, 0x10($t0)
    /* 1AA40 80019E40 B90D0013 */  swr        $t5, 0x13($t0)
    /* 1AA44 80019E44 AA140014 */  swl        $s4, 0x14($s0)
    /* 1AA48 80019E48 BA140017 */  swr        $s4, 0x17($s0)
    /* 1AA4C 80019E4C 8A140060 */  lwl        $s4, 0x60($s0)
    /* 1AA50 80019E50 9A140063 */  lwr        $s4, 0x63($s0)
  .L80019E54:
    /* 1AA54 80019E54 3C098005 */  lui        $t1, %hi(D_8004FA18)
    /* 1AA58 80019E58 9129FA18 */  lbu        $t1, %lo(D_8004FA18)($t1)
    /* 1AA5C 80019E5C 26520001 */  addiu      $s2, $s2, 0x1
    /* 1AA60 80019E60 261001A0 */  addiu      $s0, $s0, 0x1A0
    /* 1AA64 80019E64 0249082B */  sltu       $at, $s2, $t1
    /* 1AA68 80019E68 5420FFA3 */  bnel       $at, $zero, .L80019CF8
    /* 1AA6C 80019E6C 8A0F0060 */   lwl       $t7, 0x60($s0)
    /* 1AA70 80019E70 AFB4004C */  sw         $s4, 0x4C($sp)
  .L80019E74:
    /* 1AA74 80019E74 2413FFFF */  addiu      $s3, $zero, -0x1
    /* 1AA78 80019E78 53D30009 */  beql       $fp, $s3, .L80019EA0
    /* 1AA7C 80019E7C 8FBF003C */   lw        $ra, 0x3C($sp)
    /* 1AA80 80019E80 0C0085F9 */  jal        func_800217E4
    /* 1AA84 80019E84 8FA40044 */   lw        $a0, 0x44($sp)
    /* 1AA88 80019E88 8FAA0044 */  lw         $t2, 0x44($sp)
    /* 1AA8C 80019E8C 9144004A */  lbu        $a0, 0x4A($t2)
    /* 1AA90 80019E90 9145004B */  lbu        $a1, 0x4B($t2)
    /* 1AA94 80019E94 0C0083D3 */  jal        func_80020F4C
    /* 1AA98 80019E98 91460051 */   lbu       $a2, 0x51($t2)
    /* 1AA9C 80019E9C 8FBF003C */  lw         $ra, 0x3C($sp)
  .L80019EA0:
    /* 1AAA0 80019EA0 03C01025 */  or         $v0, $fp, $zero
    /* 1AAA4 80019EA4 8FBE0038 */  lw         $fp, 0x38($sp)
    /* 1AAA8 80019EA8 8FB00018 */  lw         $s0, 0x18($sp)
    /* 1AAAC 80019EAC 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 1AAB0 80019EB0 8FB20020 */  lw         $s2, 0x20($sp)
    /* 1AAB4 80019EB4 8FB30024 */  lw         $s3, 0x24($sp)
    /* 1AAB8 80019EB8 8FB40028 */  lw         $s4, 0x28($sp)
    /* 1AABC 80019EBC 8FB5002C */  lw         $s5, 0x2C($sp)
    /* 1AAC0 80019EC0 8FB60030 */  lw         $s6, 0x30($sp)
    /* 1AAC4 80019EC4 8FB70034 */  lw         $s7, 0x34($sp)
    /* 1AAC8 80019EC8 03E00008 */  jr         $ra
    /* 1AACC 80019ECC 27BD0058 */   addiu     $sp, $sp, 0x58
endlabel func_80019C8C
