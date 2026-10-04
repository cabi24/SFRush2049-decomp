nonmatching func_8001DC08, 0x1D8

glabel func_8001DC08
    /* 1E808 8001DC08 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* 1E80C 8001DC0C 3C028005 */  lui        $v0, %hi(D_8004FF20)
    /* 1E810 8001DC10 9042FF20 */  lbu        $v0, %lo(D_8004FF20)($v0)
    /* 1E814 8001DC14 AFB70050 */  sw         $s7, 0x50($sp)
    /* 1E818 8001DC18 AFBF0054 */  sw         $ra, 0x54($sp)
    /* 1E81C 8001DC1C AFB6004C */  sw         $s6, 0x4C($sp)
    /* 1E820 8001DC20 AFB50048 */  sw         $s5, 0x48($sp)
    /* 1E824 8001DC24 AFB40044 */  sw         $s4, 0x44($sp)
    /* 1E828 8001DC28 AFB30040 */  sw         $s3, 0x40($sp)
    /* 1E82C 8001DC2C AFB2003C */  sw         $s2, 0x3C($sp)
    /* 1E830 8001DC30 AFB10038 */  sw         $s1, 0x38($sp)
    /* 1E834 8001DC34 AFB00034 */  sw         $s0, 0x34($sp)
    /* 1E838 8001DC38 F7B80028 */  sdc1       $fs2, 0x28($sp)
    /* 1E83C 8001DC3C F7B60020 */  sdc1       $fs1, 0x20($sp)
    /* 1E840 8001DC40 F7B40018 */  sdc1       $fs0, 0x18($sp)
    /* 1E844 8001DC44 18400058 */  blez       $v0, .L8001DDA8
    /* 1E848 8001DC48 0000B825 */   or        $s7, $zero, $zero
    /* 1E84C 8001DC4C 3C018003 */  lui        $at, %hi(D_8002D904)
    /* 1E850 8001DC50 C436D904 */  lwc1       $fs1, %lo(D_8002D904)($at)
    /* 1E854 8001DC54 3C018003 */  lui        $at, %hi(D_8002D908)
    /* 1E858 8001DC58 3C128005 */  lui        $s2, %hi(D_8004FDA0)
    /* 1E85C 8001DC5C 3C13FFFD */  lui        $s3, (0xFFFDFFFF >> 16)
    /* 1E860 8001DC60 4480C000 */  mtc1       $zero, $fs2
    /* 1E864 8001DC64 3673FFFF */  ori        $s3, $s3, (0xFFFDFFFF & 0xFFFF)
    /* 1E868 8001DC68 2652FDA0 */  addiu      $s2, $s2, %lo(D_8004FDA0)
    /* 1E86C 8001DC6C C434D908 */  lwc1       $fs0, %lo(D_8002D908)($at)
    /* 1E870 8001DC70 3C160010 */  lui        $s6, (0x100000 >> 16)
    /* 1E874 8001DC74 3C150004 */  lui        $s5, (0x40000 >> 16)
    /* 1E878 8001DC78 2414FFFF */  addiu      $s4, $zero, -0x1
  .L8001DC7C:
    /* 1E87C 8001DC7C 8E510004 */  lw         $s1, 0x4($s2)
    /* 1E880 8001DC80 52200046 */  beql       $s1, $zero, .L8001DD9C
    /* 1E884 8001DC84 26F70001 */   addiu     $s7, $s7, 0x1
    /* 1E888 8001DC88 8E420008 */  lw         $v0, 0x8($s2)
  .L8001DC8C:
    /* 1E88C 8001DC8C 2405007F */  addiu      $a1, $zero, 0x7F
    /* 1E890 8001DC90 5040001A */  beql       $v0, $zero, .L8001DCFC
    /* 1E894 8001DC94 8E300018 */   lw        $s0, 0x18($s1)
    /* 1E898 8001DC98 C6240004 */  lwc1       $ft0, 0x4($s1)
    /* 1E89C 8001DC9C C4460004 */  lwc1       $ft1, 0x4($v0)
    /* 1E8A0 8001DCA0 46062001 */  sub.s      $fv0, $ft0, $ft1
    /* 1E8A4 8001DCA4 4614003E */  c.le.s     $fv0, $fs0
    /* 1E8A8 8001DCA8 00000000 */  nop
    /* 1E8AC 8001DCAC 45030036 */  bc1tl      .L8001DD88
    /* 1E8B0 8001DCB0 8E310000 */   lw        $s1, 0x0($s1)
    /* 1E8B4 8001DCB4 4616003E */  c.le.s     $fv0, $fs1
    /* 1E8B8 8001DCB8 00000000 */  nop
    /* 1E8BC 8001DCBC 4502000D */  bc1fl      .L8001DCF4
    /* 1E8C0 8001DCC0 8E280018 */   lw        $t0, 0x18($s1)
    /* 1E8C4 8001DCC4 8E220018 */  lw         $v0, 0x18($s1)
    /* 1E8C8 8001DCC8 944E003E */  lhu        $t6, 0x3E($v0)
    /* 1E8CC 8001DCCC 25CF0001 */  addiu      $t7, $t6, 0x1
    /* 1E8D0 8001DCD0 A44F003E */  sh         $t7, 0x3E($v0)
    /* 1E8D4 8001DCD4 8E380018 */  lw         $t8, 0x18($s1)
    /* 1E8D8 8001DCD8 9719003E */  lhu        $t9, 0x3E($t8)
    /* 1E8DC 8001DCDC 2B210014 */  slti       $at, $t9, 0x14
    /* 1E8E0 8001DCE0 50200006 */  beql       $at, $zero, .L8001DCFC
    /* 1E8E4 8001DCE4 8E300018 */   lw        $s0, 0x18($s1)
    /* 1E8E8 8001DCE8 10000027 */  b          .L8001DD88
    /* 1E8EC 8001DCEC 8E310000 */   lw        $s1, 0x0($s1)
    /* 1E8F0 8001DCF0 8E280018 */  lw         $t0, 0x18($s1)
  .L8001DCF4:
    /* 1E8F4 8001DCF4 A500003E */  sh         $zero, 0x3E($t0)
    /* 1E8F8 8001DCF8 8E300018 */  lw         $s0, 0x18($s1)
  .L8001DCFC:
    /* 1E8FC 8001DCFC 24060040 */  addiu      $a2, $zero, 0x40
    /* 1E900 8001DD00 0C006C74 */  jal        func_8001B1D0
    /* 1E904 8001DD04 9604003C */   lhu       $a0, 0x3C($s0)
    /* 1E908 8001DD08 14540009 */  bne        $v0, $s4, .L8001DD30
    /* 1E90C 8001DD0C AE020034 */   sw        $v0, 0x34($s0)
    /* 1E910 8001DD10 8E020008 */  lw         $v0, 0x8($s0)
    /* 1E914 8001DD14 30490002 */  andi       $t1, $v0, 0x2
    /* 1E918 8001DD18 1520001A */  bnez       $t1, .L8001DD84
    /* 1E91C 8001DD1C 00555025 */   or        $t2, $v0, $s5
    /* 1E920 8001DD20 AE0A0008 */  sw         $t2, 0x8($s0)
    /* 1E924 8001DD24 01536024 */  and        $t4, $t2, $s3
    /* 1E928 8001DD28 10000016 */  b          .L8001DD84
    /* 1E92C 8001DD2C AE0C0008 */   sw        $t4, 0x8($s0)
  .L8001DD30:
    /* 1E930 8001DD30 8E0D0008 */  lw         $t5, 0x8($s0)
    /* 1E934 8001DD34 E6180040 */  swc1       $fs2, 0x40($s0)
    /* 1E938 8001DD38 02002025 */  or         $a0, $s0, $zero
    /* 1E93C 8001DD3C 01B67025 */  or         $t6, $t5, $s6
    /* 1E940 8001DD40 AE0E0008 */  sw         $t6, 0x8($s0)
    /* 1E944 8001DD44 C6280010 */  lwc1       $ft2, 0x10($s1)
    /* 1E948 8001DD48 8E27000C */  lw         $a3, 0xC($s1)
    /* 1E94C 8001DD4C 8E260008 */  lw         $a2, 0x8($s1)
    /* 1E950 8001DD50 8E250004 */  lw         $a1, 0x4($s1)
    /* 1E954 8001DD54 E7A80010 */  swc1       $ft2, 0x10($sp)
    /* 1E958 8001DD58 C62A0014 */  lwc1       $ft3, 0x14($s1)
    /* 1E95C 8001DD5C 0C007337 */  jal        func_8001CCDC
    /* 1E960 8001DD60 E7AA0014 */   swc1      $ft3, 0x14($sp)
    /* 1E964 8001DD64 8E0F0008 */  lw         $t7, 0x8($s0)
    /* 1E968 8001DD68 01F3C024 */  and        $t8, $t7, $s3
    /* 1E96C 8001DD6C AE180008 */  sw         $t8, 0x8($s0)
    /* 1E970 8001DD70 8E420008 */  lw         $v0, 0x8($s2)
    /* 1E974 8001DD74 50400004 */  beql       $v0, $zero, .L8001DD88
    /* 1E978 8001DD78 8E310000 */   lw        $s1, 0x0($s1)
    /* 1E97C 8001DD7C 8C590000 */  lw         $t9, 0x0($v0)
    /* 1E980 8001DD80 AE590008 */  sw         $t9, 0x8($s2)
  .L8001DD84:
    /* 1E984 8001DD84 8E310000 */  lw         $s1, 0x0($s1)
  .L8001DD88:
    /* 1E988 8001DD88 5620FFC0 */  bnel       $s1, $zero, .L8001DC8C
    /* 1E98C 8001DD8C 8E420008 */   lw        $v0, 0x8($s2)
    /* 1E990 8001DD90 3C028005 */  lui        $v0, %hi(D_8004FF20)
    /* 1E994 8001DD94 9042FF20 */  lbu        $v0, %lo(D_8004FF20)($v0)
    /* 1E998 8001DD98 26F70001 */  addiu      $s7, $s7, 0x1
  .L8001DD9C:
    /* 1E99C 8001DD9C 02E2082A */  slt        $at, $s7, $v0
    /* 1E9A0 8001DDA0 1420FFB6 */  bnez       $at, .L8001DC7C
    /* 1E9A4 8001DDA4 2652000C */   addiu     $s2, $s2, 0xC
  .L8001DDA8:
    /* 1E9A8 8001DDA8 8FBF0054 */  lw         $ra, 0x54($sp)
    /* 1E9AC 8001DDAC D7B40018 */  ldc1       $fs0, 0x18($sp)
    /* 1E9B0 8001DDB0 D7B60020 */  ldc1       $fs1, 0x20($sp)
    /* 1E9B4 8001DDB4 D7B80028 */  ldc1       $fs2, 0x28($sp)
    /* 1E9B8 8001DDB8 8FB00034 */  lw         $s0, 0x34($sp)
    /* 1E9BC 8001DDBC 8FB10038 */  lw         $s1, 0x38($sp)
    /* 1E9C0 8001DDC0 8FB2003C */  lw         $s2, 0x3C($sp)
    /* 1E9C4 8001DDC4 8FB30040 */  lw         $s3, 0x40($sp)
    /* 1E9C8 8001DDC8 8FB40044 */  lw         $s4, 0x44($sp)
    /* 1E9CC 8001DDCC 8FB50048 */  lw         $s5, 0x48($sp)
    /* 1E9D0 8001DDD0 8FB6004C */  lw         $s6, 0x4C($sp)
    /* 1E9D4 8001DDD4 8FB70050 */  lw         $s7, 0x50($sp)
    /* 1E9D8 8001DDD8 03E00008 */  jr         $ra
    /* 1E9DC 8001DDDC 27BD0058 */   addiu     $sp, $sp, 0x58
endlabel func_8001DC08
