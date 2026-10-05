nonmatching func_8001E9B0, 0xF0

glabel func_8001E9B0
    /* 1F5B0 8001E9B0 3C018005 */  lui        $at, %hi(D_80050A4C)
    /* 1F5B4 8001E9B4 AC200A4C */  sw         $zero, %lo(D_80050A4C)($at)
    /* 1F5B8 8001E9B8 3C018005 */  lui        $at, %hi(D_80050C50)
    /* 1F5BC 8001E9BC 3C088005 */  lui        $t0, %hi(D_80050A50)
    /* 1F5C0 8001E9C0 AC200C50 */  sw         $zero, %lo(D_80050C50)($at)
    /* 1F5C4 8001E9C4 25080A50 */  addiu      $t0, $t0, %lo(D_80050A50)
    /* 1F5C8 8001E9C8 3C018005 */  lui        $at, %hi(D_80050C54)
    /* 1F5CC 8001E9CC 3C048005 */  lui        $a0, %hi(D_80050A50)
    /* 1F5D0 8001E9D0 3C058005 */  lui        $a1, %hi(D_80050A50 + 0x10)
    /* 1F5D4 8001E9D4 3C068005 */  lui        $a2, %hi(D_80050A50 + 0x20)
    /* 1F5D8 8001E9D8 3C078005 */  lui        $a3, %hi(D_80050A50 + 0x30)
    /* 1F5DC 8001E9DC AC280C54 */  sw         $t0, %lo(D_80050C54)($at)
    /* 1F5E0 8001E9E0 00001025 */  or         $v0, $zero, $zero
    /* 1F5E4 8001E9E4 24E70A80 */  addiu      $a3, $a3, %lo(D_80050A50 + 0x30)
    /* 1F5E8 8001E9E8 24C60A70 */  addiu      $a2, $a2, %lo(D_80050A50 + 0x20)
    /* 1F5EC 8001E9EC 24A50A60 */  addiu      $a1, $a1, %lo(D_80050A50 + 0x10)
    /* 1F5F0 8001E9F0 24840A50 */  addiu      $a0, $a0, %lo(D_80050A50)
    /* 1F5F4 8001E9F4 00001825 */  or         $v1, $zero, $zero
    /* 1F5F8 8001E9F8 24090020 */  addiu      $t1, $zero, 0x20
  .L8001E9FC:
    /* 1F5FC 8001E9FC A8820004 */  swl        $v0, 0x4($a0)
    /* 1F600 8001EA00 10400005 */  beqz       $v0, .L8001EA18
    /* 1F604 8001EA04 B8820007 */   swr       $v0, 0x7($a0)
    /* 1F608 8001EA08 00037100 */  sll        $t6, $v1, 4
    /* 1F60C 8001EA0C 010E7821 */  addu       $t7, $t0, $t6
    /* 1F610 8001EA10 A84F0000 */  swl        $t7, 0x0($v0)
    /* 1F614 8001EA14 B84F0003 */  swr        $t7, 0x3($v0)
  .L8001EA18:
    /* 1F618 8001EA18 A8840014 */  swl        $a0, 0x14($a0)
    /* 1F61C 8001EA1C 10800006 */  beqz       $a0, .L8001EA38
    /* 1F620 8001EA20 B8840017 */   swr       $a0, 0x17($a0)
    /* 1F624 8001EA24 0003C100 */  sll        $t8, $v1, 4
    /* 1F628 8001EA28 0118C821 */  addu       $t9, $t0, $t8
    /* 1F62C 8001EA2C 272A0010 */  addiu      $t2, $t9, 0x10
    /* 1F630 8001EA30 A88A0000 */  swl        $t2, 0x0($a0)
    /* 1F634 8001EA34 B88A0003 */  swr        $t2, 0x3($a0)
  .L8001EA38:
    /* 1F638 8001EA38 A8850024 */  swl        $a1, 0x24($a0)
    /* 1F63C 8001EA3C 10A00006 */  beqz       $a1, .L8001EA58
    /* 1F640 8001EA40 B8850027 */   swr       $a1, 0x27($a0)
    /* 1F644 8001EA44 00035900 */  sll        $t3, $v1, 4
    /* 1F648 8001EA48 010B6021 */  addu       $t4, $t0, $t3
    /* 1F64C 8001EA4C 258D0020 */  addiu      $t5, $t4, 0x20
    /* 1F650 8001EA50 A8AD0000 */  swl        $t5, 0x0($a1)
    /* 1F654 8001EA54 B8AD0003 */  swr        $t5, 0x3($a1)
  .L8001EA58:
    /* 1F658 8001EA58 A8860034 */  swl        $a2, 0x34($a0)
    /* 1F65C 8001EA5C 10C00006 */  beqz       $a2, .L8001EA78
    /* 1F660 8001EA60 B8860037 */   swr       $a2, 0x37($a0)
    /* 1F664 8001EA64 00037100 */  sll        $t6, $v1, 4
    /* 1F668 8001EA68 010E7821 */  addu       $t7, $t0, $t6
    /* 1F66C 8001EA6C 25F80030 */  addiu      $t8, $t7, 0x30
    /* 1F670 8001EA70 A8D80000 */  swl        $t8, 0x0($a2)
    /* 1F674 8001EA74 B8D80003 */  swr        $t8, 0x3($a2)
  .L8001EA78:
    /* 1F678 8001EA78 00E01025 */  or         $v0, $a3, $zero
    /* 1F67C 8001EA7C 24630004 */  addiu      $v1, $v1, 0x4
    /* 1F680 8001EA80 24E70040 */  addiu      $a3, $a3, 0x40
    /* 1F684 8001EA84 24840040 */  addiu      $a0, $a0, 0x40
    /* 1F688 8001EA88 24A50040 */  addiu      $a1, $a1, 0x40
    /* 1F68C 8001EA8C 1469FFDB */  bne        $v1, $t1, .L8001E9FC
    /* 1F690 8001EA90 24C60040 */   addiu     $a2, $a2, 0x40
    /* 1F694 8001EA94 A8400000 */  swl        $zero, 0x0($v0)
    /* 1F698 8001EA98 03E00008 */  jr         $ra
    /* 1F69C 8001EA9C B8400003 */   swr       $zero, 0x3($v0)
endlabel func_8001E9B0
