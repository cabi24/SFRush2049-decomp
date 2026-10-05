nonmatching func_80018F20, 0x84

glabel func_80018F20
    /* 19B20 80018F20 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 19B24 80018F24 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 19B28 80018F28 0C005D91 */  jal        func_80017644
    /* 19B2C 80018F2C AFA5001C */   sw        $a1, 0x1C($sp)
    /* 19B30 80018F30 2401FFFF */  addiu      $at, $zero, -0x1
    /* 19B34 80018F34 10410017 */  beq        $v0, $at, .L80018F94
    /* 19B38 80018F38 97A5001E */   lhu       $a1, 0x1E($sp)
    /* 19B3C 80018F3C 00027000 */  sll        $t6, $v0, 0
    /* 19B40 80018F40 05C00008 */  bltz       $t6, .L80018F64
    /* 19B44 80018F44 3C017FFF */   lui       $at, (0x7FFFFFFF >> 16)
    /* 19B48 80018F48 00027A40 */  sll        $t7, $v0, 9
    /* 19B4C 80018F4C 01E27823 */  subu       $t7, $t7, $v0
    /* 19B50 80018F50 000F78C0 */  sll        $t7, $t7, 3
    /* 19B54 80018F54 3C018004 */  lui        $at, %hi(D_80043EB8 + 0xFC2)
    /* 19B58 80018F58 002F0821 */  addu       $at, $at, $t7
    /* 19B5C 80018F5C 1000000D */  b          .L80018F94
    /* 19B60 80018F60 A4254E7A */   sh        $a1, %lo(D_80043EB8 + 0xFC2)($at)
  .L80018F64:
    /* 19B64 80018F64 3421FFFF */  ori        $at, $at, (0x7FFFFFFF & 0xFFFF)
    /* 19B68 80018F68 00412024 */  and        $a0, $v0, $at
    /* 19B6C 80018F6C 0004C240 */  sll        $t8, $a0, 9
    /* 19B70 80018F70 0304C023 */  subu       $t8, $t8, $a0
    /* 19B74 80018F74 3C198004 */  lui        $t9, %hi(D_80043EB8)
    /* 19B78 80018F78 27393EB8 */  addiu      $t9, $t9, %lo(D_80043EB8)
    /* 19B7C 80018F7C 0018C0C0 */  sll        $t8, $t8, 3
    /* 19B80 80018F80 03191821 */  addu       $v1, $t8, $t9
    /* 19B84 80018F84 90680FEE */  lbu        $t0, 0xFEE($v1)
    /* 19B88 80018F88 A4650FEC */  sh         $a1, 0xFEC($v1)
    /* 19B8C 80018F8C 35090020 */  ori        $t1, $t0, 0x20
    /* 19B90 80018F90 A0690FEE */  sb         $t1, 0xFEE($v1)
  .L80018F94:
    /* 19B94 80018F94 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 19B98 80018F98 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 19B9C 80018F9C 03E00008 */  jr         $ra
    /* 19BA0 80018FA0 00000000 */   nop
endlabel func_80018F20
