nonmatching func_8001ECE0, 0x114

glabel func_8001ECE0
    /* 1F8E0 8001ECE0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 1F8E4 8001ECE4 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 1F8E8 8001ECE8 AFB00018 */  sw         $s0, 0x18($sp)
    /* 1F8EC 8001ECEC 0C007ABB */  jal        func_8001EAEC
    /* 1F8F0 8001ECF0 AFA40020 */   sw        $a0, 0x20($sp)
    /* 1F8F4 8001ECF4 3C108005 */  lui        $s0, %hi(D_80050C50)
    /* 1F8F8 8001ECF8 8E100C50 */  lw         $s0, %lo(D_80050C50)($s0)
    /* 1F8FC 8001ECFC 00402825 */  or         $a1, $v0, $zero
    /* 1F900 8001ED00 00002025 */  or         $a0, $zero, $zero
    /* 1F904 8001ED04 12000011 */  beqz       $s0, .L8001ED4C
    /* 1F908 8001ED08 00000000 */   nop
    /* 1F90C 8001ED0C 8A030008 */  lwl        $v1, 0x8($s0)
  .L8001ED10:
    /* 1F910 8001ED10 9A03000B */  lwr        $v1, 0xB($s0)
    /* 1F914 8001ED14 0043082B */  sltu       $at, $v0, $v1
    /* 1F918 8001ED18 1420000C */  bnez       $at, .L8001ED4C
    /* 1F91C 8001ED1C 00000000 */   nop
    /* 1F920 8001ED20 54430005 */  bnel       $v0, $v1, .L8001ED38
    /* 1F924 8001ED24 02000821 */   addu      $at, $s0, $zero
    /* 1F928 8001ED28 0C007ABB */  jal        func_8001EAEC
    /* 1F92C 8001ED2C 00000000 */   nop
    /* 1F930 8001ED30 00402825 */  or         $a1, $v0, $zero
    /* 1F934 8001ED34 02000821 */  addu       $at, $s0, $zero
  .L8001ED38:
    /* 1F938 8001ED38 02002025 */  or         $a0, $s0, $zero
    /* 1F93C 8001ED3C 8A100000 */  lwl        $s0, 0x0($s0)
    /* 1F940 8001ED40 98300003 */  lwr        $s0, 0x3($at)
    /* 1F944 8001ED44 5600FFF2 */  bnel       $s0, $zero, .L8001ED10
    /* 1F948 8001ED48 8A030008 */   lwl       $v1, 0x8($s0)
  .L8001ED4C:
    /* 1F94C 8001ED4C 3C068005 */  lui        $a2, %hi(D_80050C54)
    /* 1F950 8001ED50 24C60C54 */  addiu      $a2, $a2, %lo(D_80050C54)
    /* 1F954 8001ED54 8CC20000 */  lw         $v0, 0x0($a2)
    /* 1F958 8001ED58 14400003 */  bnez       $v0, .L8001ED68
    /* 1F95C 8001ED5C 00401825 */   or        $v1, $v0, $zero
    /* 1F960 8001ED60 1000001F */  b          .L8001EDE0
    /* 1F964 8001ED64 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8001ED68:
    /* 1F968 8001ED68 884E0000 */  lwl        $t6, 0x0($v0)
    /* 1F96C 8001ED6C 984E0003 */  lwr        $t6, 0x3($v0)
    /* 1F970 8001ED70 3C018005 */  lui        $at, %hi(D_80050C50)
    /* 1F974 8001ED74 11C00003 */  beqz       $t6, .L8001ED84
    /* 1F978 8001ED78 ACCE0000 */   sw        $t6, 0x0($a2)
    /* 1F97C 8001ED7C A9C00004 */  swl        $zero, 0x4($t6)
    /* 1F980 8001ED80 B9C00007 */  swr        $zero, 0x7($t6)
  .L8001ED84:
    /* 1F984 8001ED84 54800004 */  bnel       $a0, $zero, .L8001ED98
    /* 1F988 8001ED88 A8830000 */   swl       $v1, 0x0($a0)
    /* 1F98C 8001ED8C 10000003 */  b          .L8001ED9C
    /* 1F990 8001ED90 AC230C50 */   sw        $v1, %lo(D_80050C50)($at)
    /* 1F994 8001ED94 A8830000 */  swl        $v1, 0x0($a0)
  .L8001ED98:
    /* 1F998 8001ED98 B8830003 */  swr        $v1, 0x3($a0)
  .L8001ED9C:
    /* 1F99C 8001ED9C A8640004 */  swl        $a0, 0x4($v1)
    /* 1F9A0 8001EDA0 A8700000 */  swl        $s0, 0x0($v1)
    /* 1F9A4 8001EDA4 B8640007 */  swr        $a0, 0x7($v1)
    /* 1F9A8 8001EDA8 12000003 */  beqz       $s0, .L8001EDB8
    /* 1F9AC 8001EDAC B8700003 */   swr       $s0, 0x3($v1)
    /* 1F9B0 8001EDB0 AA030004 */  swl        $v1, 0x4($s0)
    /* 1F9B4 8001EDB4 BA030007 */  swr        $v1, 0x7($s0)
  .L8001EDB8:
    /* 1F9B8 8001EDB8 8FA40020 */  lw         $a0, 0x20($sp)
    /* 1F9BC 8001EDBC A8650008 */  swl        $a1, 0x8($v1)
    /* 1F9C0 8001EDC0 B865000B */  swr        $a1, 0xB($v1)
    /* 1F9C4 8001EDC4 888F0060 */  lwl        $t7, 0x60($a0)
    /* 1F9C8 8001EDC8 988F0063 */  lwr        $t7, 0x63($a0)
    /* 1F9CC 8001EDCC 00A01025 */  or         $v0, $a1, $zero
    /* 1F9D0 8001EDD0 A86F000C */  swl        $t7, 0xC($v1)
    /* 1F9D4 8001EDD4 B86F000F */  swr        $t7, 0xF($v1)
    /* 1F9D8 8001EDD8 A8830018 */  swl        $v1, 0x18($a0)
    /* 1F9DC 8001EDDC B883001B */  swr        $v1, 0x1B($a0)
  .L8001EDE0:
    /* 1F9E0 8001EDE0 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 1F9E4 8001EDE4 8FB00018 */  lw         $s0, 0x18($sp)
    /* 1F9E8 8001EDE8 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 1F9EC 8001EDEC 03E00008 */  jr         $ra
    /* 1F9F0 8001EDF0 00000000 */   nop
endlabel func_8001ECE0
