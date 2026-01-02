# Source: F700.s
# Address: 0x8000EC70

glabel func_8000EC70
    /* F870 8000EC70 27BDFF60 */  addiu      $sp, $sp, -0xA0
    /* F874 8000EC74 AFBF003C */  sw         $ra, 0x3C($sp)
    /* F878 8000EC78 AFB60038 */  sw         $s6, 0x38($sp)
    /* F87C 8000EC7C AFB50034 */  sw         $s5, 0x34($sp)
    /* F880 8000EC80 AFB40030 */  sw         $s4, 0x30($sp)
    /* F884 8000EC84 AFB3002C */  sw         $s3, 0x2C($sp)
    /* F888 8000EC88 AFB20028 */  sw         $s2, 0x28($sp)
    /* F88C 8000EC8C AFB10024 */  sw         $s1, 0x24($sp)
    /* F890 8000EC90 AFB00020 */  sw         $s0, 0x20($sp)
    /* F894 8000EC94 240EFFFF */  addiu      $t6, $zero, -0x1
    /* F898 8000EC98 00809025 */  or         $s2, $a0, $zero
    /* F89C 8000EC9C 00C0A025 */  or         $s4, $a2, $zero
    /* F8A0 8000ECA0 00A0B025 */  or         $s6, $a1, $zero
    /* F8A4 8000ECA4 00009825 */  or         $s3, $zero, $zero
    /* F8A8 8000ECA8 0C00325C */  jal        func_8000C970
    /* F8AC 8000ECAC ACCE0000 */   sw        $t6, 0x0($a2)
    /* F8B0 8000ECB0 AE820004 */  sw         $v0, 0x4($s4)
    /* F8B4 8000ECB4 8ED9000C */  lw         $t9, 0xC($s6)
    /* F8B8 8000ECB8 8ED80008 */  lw         $t8, 0x8($s6)
    /* F8BC 8000ECBC 02402025 */  or         $a0, $s2, $zero
    /* F8C0 8000ECC0 AE99000C */  sw         $t9, 0xC($s4)
    /* F8C4 8000ECC4 AE980008 */  sw         $t8, 0x8($s4)
    /* F8C8 8000ECC8 8EC90014 */  lw         $t1, 0x14($s6)
    /* F8CC 8000ECCC 8EC80010 */  lw         $t0, 0x10($s6)
    /* F8D0 8000ECD0 AE890014 */  sw         $t1, 0x14($s4)
    /* F8D4 8000ECD4 AE880010 */  sw         $t0, 0x10($s4)
    /* F8D8 8000ECD8 924F0065 */  lbu        $t7, 0x65($s2)
    /* F8DC 8000ECDC 51E00008 */  beql       $t7, $zero, .L8000ED00
    /* F8E0 8000ECE0 27B1007C */   addiu     $s1, $sp, 0x7C
    /* F8E4 8000ECE4 0C003A14 */  jal        func_8000E850
    /* F8E8 8000ECE8 00002825 */   or        $a1, $zero, $zero
    /* F8EC 8000ECEC 50400004 */  beql       $v0, $zero, .L8000ED00
    /* F8F0 8000ECF0 27B1007C */   addiu     $s1, $sp, 0x7C
    /* F8F4 8000ECF4 100000A9 */  b          .L8000EF9C
    /* F8F8 8000ECF8 8FBF003C */   lw        $ra, 0x3C($sp)
    /* F8FC 8000ECFC 27B1007C */  addiu      $s1, $sp, 0x7C
  .L8000ED00:
    /* F900 8000ED00 0220A825 */  or         $s5, $s1, $zero
    /* F904 8000ED04 27B0009C */  addiu      $s0, $sp, 0x9C
    /* F908 8000ED08 02402025 */  or         $a0, $s2, $zero
  .L8000ED0C:
    /* F90C 8000ED0C 0C003A14 */  jal        func_8000E850
    /* F910 8000ED10 326500FF */   andi      $a1, $s3, 0xFF
    /* F914 8000ED14 10400003 */  beqz       $v0, .L8000ED24
    /* F918 8000ED18 00003025 */   or        $a2, $zero, $zero
    /* F91C 8000ED1C 1000009F */  b          .L8000EF9C
    /* F920 8000ED20 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8000ED24:
    /* F924 8000ED24 8E440004 */  lw         $a0, 0x4($s2)
    /* F928 8000ED28 8E450008 */  lw         $a1, 0x8($s2)
    /* F92C 8000ED2C 0C003A34 */  jal        func_8000E8D0
    /* F930 8000ED30 02A03825 */   or        $a3, $s5, $zero
    /* F934 8000ED34 10400003 */  beqz       $v0, .L8000ED44
    /* F938 8000ED38 366B0080 */   ori       $t3, $s3, 0x80
    /* F93C 8000ED3C 10000097 */  b          .L8000EF9C
    /* F940 8000ED40 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8000ED44:
    /* F944 8000ED44 A3AB007C */  sb         $t3, 0x7C($sp)
    /* F948 8000ED48 27A2007D */  addiu      $v0, $sp, 0x7D
  .L8000ED4C:
    /* F94C 8000ED4C 904C0000 */  lbu        $t4, 0x0($v0)
    /* F950 8000ED50 24420001 */  addiu      $v0, $v0, 0x1
    /* F954 8000ED54 0050082B */  sltu       $at, $v0, $s0
    /* F958 8000ED58 01806827 */  not        $t5, $t4
    /* F95C 8000ED5C 1420FFFB */  bnez       $at, .L8000ED4C
    /* F960 8000ED60 A04DFFFF */   sb        $t5, -0x1($v0)
    /* F964 8000ED64 8E440004 */  lw         $a0, 0x4($s2)
    /* F968 8000ED68 8E450008 */  lw         $a1, 0x8($s2)
    /* F96C 8000ED6C AFA00010 */  sw         $zero, 0x10($sp)
    /* F970 8000ED70 00003025 */  or         $a2, $zero, $zero
    /* F974 8000ED74 0C003DA0 */  jal        func_8000F680
    /* F978 8000ED78 02A03825 */   or        $a3, $s5, $zero
    /* F97C 8000ED7C 10400003 */  beqz       $v0, .L8000ED8C
    /* F980 8000ED80 00003025 */   or        $a2, $zero, $zero
    /* F984 8000ED84 10000085 */  b          .L8000EF9C
    /* F988 8000ED88 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8000ED8C:
    /* F98C 8000ED8C 8E440004 */  lw         $a0, 0x4($s2)
    /* F990 8000ED90 8E450008 */  lw         $a1, 0x8($s2)
    /* F994 8000ED94 0C003A34 */  jal        func_8000E8D0
    /* F998 8000ED98 27A7005C */   addiu     $a3, $sp, 0x5C
    /* F99C 8000ED9C 10400003 */  beqz       $v0, .L8000EDAC
    /* F9A0 8000EDA0 27A3005C */   addiu     $v1, $sp, 0x5C
    /* F9A4 8000EDA4 1000007D */  b          .L8000EF9C
    /* F9A8 8000EDA8 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8000EDAC:
    /* F9AC 8000EDAC 27A2007C */  addiu      $v0, $sp, 0x7C
  .L8000EDB0:
    /* F9B0 8000EDB0 906E0000 */  lbu        $t6, 0x0($v1)
    /* F9B4 8000EDB4 90580000 */  lbu        $t8, 0x0($v0)
    /* F9B8 8000EDB8 55D80006 */  bnel       $t6, $t8, .L8000EDD4
    /* F9BC 8000EDBC 27B9007C */   addiu     $t9, $sp, 0x7C
    /* F9C0 8000EDC0 24630001 */  addiu      $v1, $v1, 0x1
    /* F9C4 8000EDC4 0071082B */  sltu       $at, $v1, $s1
    /* F9C8 8000EDC8 1420FFF9 */  bnez       $at, .L8000EDB0
    /* F9CC 8000EDCC 24420001 */   addiu     $v0, $v0, 0x1
    /* F9D0 8000EDD0 27B9007C */  addiu      $t9, $sp, 0x7C
  .L8000EDD4:
    /* F9D4 8000EDD4 14790018 */  bne        $v1, $t9, .L8000EE38
    /* F9D8 8000EDD8 02402025 */   or        $a0, $s2, $zero
    /* F9DC 8000EDDC 5A600013 */  blezl      $s3, .L8000EE2C
    /* F9E0 8000EDE0 26730001 */   addiu     $s3, $s3, 0x1
    /* F9E4 8000EDE4 0C003A14 */  jal        func_8000E850
    /* F9E8 8000EDE8 00002825 */   or        $a1, $zero, $zero
    /* F9EC 8000EDEC 10400003 */  beqz       $v0, .L8000EDFC
    /* F9F0 8000EDF0 00003025 */   or        $a2, $zero, $zero
    /* F9F4 8000EDF4 10000069 */  b          .L8000EF9C
    /* F9F8 8000EDF8 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8000EDFC:
    /* F9FC 8000EDFC 8E440004 */  lw         $a0, 0x4($s2)
    /* FA00 8000EE00 8E450008 */  lw         $a1, 0x8($s2)
    /* FA04 8000EE04 0C003A34 */  jal        func_8000E8D0
    /* FA08 8000EE08 02A03825 */   or        $a3, $s5, $zero
    /* FA0C 8000EE0C 10400003 */  beqz       $v0, .L8000EE1C
    /* FA10 8000EE10 93A8007C */   lbu       $t0, 0x7C($sp)
    /* FA14 8000EE14 10000061 */  b          .L8000EF9C
    /* FA18 8000EE18 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8000EE1C:
    /* FA1C 8000EE1C 24010080 */  addiu      $at, $zero, 0x80
    /* FA20 8000EE20 55010006 */  bnel       $t0, $at, .L8000EE3C
    /* FA24 8000EE24 92490065 */   lbu       $t1, 0x65($s2)
    /* FA28 8000EE28 26730001 */  addiu      $s3, $s3, 0x1
  .L8000EE2C:
    /* FA2C 8000EE2C 2A61003E */  slti       $at, $s3, 0x3E
    /* FA30 8000EE30 5420FFB6 */  bnel       $at, $zero, .L8000ED0C
    /* FA34 8000EE34 02402025 */   or        $a0, $s2, $zero
  .L8000EE38:
    /* FA38 8000EE38 92490065 */  lbu        $t1, 0x65($s2)
  .L8000EE3C:
    /* FA3C 8000EE3C 02402025 */  or         $a0, $s2, $zero
    /* FA40 8000EE40 11200007 */  beqz       $t1, .L8000EE60
    /* FA44 8000EE44 00000000 */   nop
    /* FA48 8000EE48 0C003A14 */  jal        func_8000E850
    /* FA4C 8000EE4C 00002825 */   or        $a1, $zero, $zero
    /* FA50 8000EE50 10400003 */  beqz       $v0, .L8000EE60
    /* FA54 8000EE54 00000000 */   nop
    /* FA58 8000EE58 10000050 */  b          .L8000EF9C
    /* FA5C 8000EE5C 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8000EE60:
    /* FA60 8000EE60 1A600003 */  blez       $s3, .L8000EE70
    /* FA64 8000EE64 02802025 */   or        $a0, $s4, $zero
    /* FA68 8000EE68 10000002 */  b          .L8000EE74
    /* FA6C 8000EE6C 24020001 */   addiu     $v0, $zero, 0x1
  .L8000EE70:
    /* FA70 8000EE70 00001025 */  or         $v0, $zero, $zero
  .L8000EE74:
    /* FA74 8000EE74 96CF0018 */  lhu        $t7, 0x18($s6)
    /* FA78 8000EE78 A293001A */  sb         $s3, 0x1A($s4)
    /* FA7C 8000EE7C 2685001C */  addiu      $a1, $s4, 0x1C
    /* FA80 8000EE80 31EAFFFE */  andi       $t2, $t7, 0xFFFE
    /* FA84 8000EE84 01425825 */  or         $t3, $t2, $v0
    /* FA88 8000EE88 A68B0018 */  sh         $t3, 0x18($s4)
    /* FA8C 8000EE8C 92CC001B */  lbu        $t4, 0x1B($s6)
    /* FA90 8000EE90 2686001E */  addiu      $a2, $s4, 0x1E
    /* FA94 8000EE94 0C003ADD */  jal        func_8000EB74
    /* FA98 8000EE98 A28C001B */   sb        $t4, 0x1B($s4)
    /* FA9C 8000EE9C 240D0001 */  addiu      $t5, $zero, 0x1
    /* FAA0 8000EEA0 240E0003 */  addiu      $t6, $zero, 0x3
    /* FAA4 8000EEA4 24180004 */  addiu      $t8, $zero, 0x4
    /* FAA8 8000EEA8 24190006 */  addiu      $t9, $zero, 0x6
    /* FAAC 8000EEAC A7AD0048 */  sh         $t5, 0x48($sp)
    /* FAB0 8000EEB0 A7AE004A */  sh         $t6, 0x4A($sp)
    /* FAB4 8000EEB4 A7B8004C */  sh         $t8, 0x4C($sp)
    /* FAB8 8000EEB8 A7B9004E */  sh         $t9, 0x4E($sp)
    /* FABC 8000EEBC 27B00048 */  addiu      $s0, $sp, 0x48
    /* FAC0 8000EEC0 27B10050 */  addiu      $s1, $sp, 0x50
    /* FAC4 8000EEC4 8E440004 */  lw         $a0, 0x4($s2)
  .L8000EEC8:
    /* FAC8 8000EEC8 8E450008 */  lw         $a1, 0x8($s2)
    /* FACC 8000EECC 96060000 */  lhu        $a2, 0x0($s0)
    /* FAD0 8000EED0 24080001 */  addiu      $t0, $zero, 0x1
    /* FAD4 8000EED4 AFA80010 */  sw         $t0, 0x10($sp)
    /* FAD8 8000EED8 0C003DA0 */  jal        func_8000F680
    /* FADC 8000EEDC 02803825 */   or        $a3, $s4, $zero
    /* FAE0 8000EEE0 10400003 */  beqz       $v0, .L8000EEF0
    /* FAE4 8000EEE4 26100002 */   addiu     $s0, $s0, 0x2
    /* FAE8 8000EEE8 1000002C */  b          .L8000EF9C
    /* FAEC 8000EEEC 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8000EEF0:
    /* FAF0 8000EEF0 0211082B */  sltu       $at, $s0, $s1
    /* FAF4 8000EEF4 5420FFF4 */  bnel       $at, $zero, .L8000EEC8
    /* FAF8 8000EEF8 8E440004 */   lw        $a0, 0x4($s2)
    /* FAFC 8000EEFC 8E440004 */  lw         $a0, 0x4($s2)
    /* FB00 8000EF00 8E450008 */  lw         $a1, 0x8($s2)
    /* FB04 8000EF04 24060001 */  addiu      $a2, $zero, 0x1
    /* FB08 8000EF08 0C003A34 */  jal        func_8000E8D0
    /* FB0C 8000EF0C 02A03825 */   or        $a3, $s5, $zero
    /* FB10 8000EF10 10400003 */  beqz       $v0, .L8000EF20
    /* FB14 8000EF14 02801825 */   or        $v1, $s4, $zero
    /* FB18 8000EF18 10000020 */  b          .L8000EF9C
    /* FB1C 8000EF1C 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8000EF20:
    /* FB20 8000EF20 27A2007C */  addiu      $v0, $sp, 0x7C
    /* FB24 8000EF24 27A4009C */  addiu      $a0, $sp, 0x9C
  .L8000EF28:
    /* FB28 8000EF28 90490000 */  lbu        $t1, 0x0($v0)
    /* FB2C 8000EF2C 906F0000 */  lbu        $t7, 0x0($v1)
    /* FB30 8000EF30 512F0004 */  beql       $t1, $t7, .L8000EF44
    /* FB34 8000EF34 904A0001 */   lbu       $t2, 0x1($v0)
    /* FB38 8000EF38 10000017 */  b          .L8000EF98
    /* FB3C 8000EF3C 2402000B */   addiu     $v0, $zero, 0xB
    /* FB40 8000EF40 904A0001 */  lbu        $t2, 0x1($v0)
  .L8000EF44:
    /* FB44 8000EF44 906B0001 */  lbu        $t3, 0x1($v1)
    /* FB48 8000EF48 514B0004 */  beql       $t2, $t3, .L8000EF5C
    /* FB4C 8000EF4C 904C0002 */   lbu       $t4, 0x2($v0)
    /* FB50 8000EF50 10000011 */  b          .L8000EF98
    /* FB54 8000EF54 2402000B */   addiu     $v0, $zero, 0xB
    /* FB58 8000EF58 904C0002 */  lbu        $t4, 0x2($v0)
  .L8000EF5C:
    /* FB5C 8000EF5C 906D0002 */  lbu        $t5, 0x2($v1)
    /* FB60 8000EF60 518D0004 */  beql       $t4, $t5, .L8000EF74
    /* FB64 8000EF64 904E0003 */   lbu       $t6, 0x3($v0)
    /* FB68 8000EF68 1000000B */  b          .L8000EF98
    /* FB6C 8000EF6C 2402000B */   addiu     $v0, $zero, 0xB
    /* FB70 8000EF70 904E0003 */  lbu        $t6, 0x3($v0)
  .L8000EF74:
    /* FB74 8000EF74 90780003 */  lbu        $t8, 0x3($v1)
    /* FB78 8000EF78 24420004 */  addiu      $v0, $v0, 0x4
    /* FB7C 8000EF7C 11D80003 */  beq        $t6, $t8, .L8000EF8C
    /* FB80 8000EF80 00000000 */   nop
    /* FB84 8000EF84 10000004 */  b          .L8000EF98
    /* FB88 8000EF88 2402000B */   addiu     $v0, $zero, 0xB
  .L8000EF8C:
    /* FB8C 8000EF8C 1444FFE6 */  bne        $v0, $a0, .L8000EF28
    /* FB90 8000EF90 24630004 */   addiu     $v1, $v1, 0x4
    /* FB94 8000EF94 00001025 */  or         $v0, $zero, $zero
  .L8000EF98:
    /* FB98 8000EF98 8FBF003C */  lw         $ra, 0x3C($sp)
  .L8000EF9C:
    /* FB9C 8000EF9C 8FB00020 */  lw         $s0, 0x20($sp)
    /* FBA0 8000EFA0 8FB10024 */  lw         $s1, 0x24($sp)
    /* FBA4 8000EFA4 8FB20028 */  lw         $s2, 0x28($sp)
    /* FBA8 8000EFA8 8FB3002C */  lw         $s3, 0x2C($sp)
    /* FBAC 8000EFAC 8FB40030 */  lw         $s4, 0x30($sp)
    /* FBB0 8000EFB0 8FB50034 */  lw         $s5, 0x34($sp)
    /* FBB4 8000EFB4 8FB60038 */  lw         $s6, 0x38($sp)
    /* FBB8 8000EFB8 03E00008 */  jr         $ra
    /* FBBC 8000EFBC 27BD00A0 */   addiu     $sp, $sp, 0xA0