# Source: A810.s
# Address: 0x80009E18

glabel func_80009E18
    /* AA18 80009E18 27BDFF68 */  addiu      $sp, $sp, -0x98
    /* AA1C 80009E1C AFBF0024 */  sw         $ra, 0x24($sp)
    /* AA20 80009E20 AFB00020 */  sw         $s0, 0x20($sp)
    /* AA24 80009E24 00808025 */  or         $s0, $a0, $zero
    /* AA28 80009E28 0C003A14 */  jal        func_8000E850
    /* AA2C 80009E2C 00002825 */   or        $a1, $zero, $zero
    /* AA30 80009E30 10400003 */  beqz       $v0, .L80009E40
    /* AA34 80009E34 00003025 */   or        $a2, $zero, $zero
    /* AA38 80009E38 10000034 */  b          .L80009F0C
    /* AA3C 80009E3C 8FBF0024 */   lw        $ra, 0x24($sp)
  .L80009E40:
    /* AA40 80009E40 8E040004 */  lw         $a0, 0x4($s0)
    /* AA44 80009E44 8E050008 */  lw         $a1, 0x8($s0)
    /* AA48 80009E48 0C003A34 */  jal        func_8000E8D0
    /* AA4C 80009E4C 27A70030 */   addiu     $a3, $sp, 0x30
    /* AA50 80009E50 10400003 */  beqz       $v0, .L80009E60
    /* AA54 80009E54 27A30070 */   addiu     $v1, $sp, 0x70
    /* AA58 80009E58 1000002C */  b          .L80009F0C
    /* AA5C 80009E5C 8FBF0024 */   lw        $ra, 0x24($sp)
  .L80009E60:
    /* AA60 80009E60 00001025 */  or         $v0, $zero, $zero
    /* AA64 80009E64 24040020 */  addiu      $a0, $zero, 0x20
  .L80009E68:
    /* AA68 80009E68 244E0001 */  addiu      $t6, $v0, 0x1
    /* AA6C 80009E6C 244F0002 */  addiu      $t7, $v0, 0x2
    /* AA70 80009E70 24580003 */  addiu      $t8, $v0, 0x3
    /* AA74 80009E74 A0620000 */  sb         $v0, 0x0($v1)
    /* AA78 80009E78 24420004 */  addiu      $v0, $v0, 0x4
    /* AA7C 80009E7C A0780003 */  sb         $t8, 0x3($v1)
    /* AA80 80009E80 A06F0002 */  sb         $t7, 0x2($v1)
    /* AA84 80009E84 A06E0001 */  sb         $t6, 0x1($v1)
    /* AA88 80009E88 1444FFF7 */  bne        $v0, $a0, .L80009E68
    /* AA8C 80009E8C 24630004 */   addiu     $v1, $v1, 0x4
    /* AA90 80009E90 8E040004 */  lw         $a0, 0x4($s0)
    /* AA94 80009E94 8E050008 */  lw         $a1, 0x8($s0)
    /* AA98 80009E98 AFA00010 */  sw         $zero, 0x10($sp)
    /* AA9C 80009E9C 00003025 */  or         $a2, $zero, $zero
    /* AAA0 80009EA0 0C003DA0 */  jal        func_8000F680
    /* AAA4 80009EA4 27A70070 */   addiu     $a3, $sp, 0x70
    /* AAA8 80009EA8 10400003 */  beqz       $v0, .L80009EB8
    /* AAAC 80009EAC 00003025 */   or        $a2, $zero, $zero
    /* AAB0 80009EB0 10000016 */  b          .L80009F0C
    /* AAB4 80009EB4 8FBF0024 */   lw        $ra, 0x24($sp)
  .L80009EB8:
    /* AAB8 80009EB8 8E040004 */  lw         $a0, 0x4($s0)
    /* AABC 80009EBC 8E050008 */  lw         $a1, 0x8($s0)
    /* AAC0 80009EC0 0C003A34 */  jal        func_8000E8D0
    /* AAC4 80009EC4 27A70050 */   addiu     $a3, $sp, 0x50
    /* AAC8 80009EC8 10400003 */  beqz       $v0, .L80009ED8
    /* AACC 80009ECC 27A40070 */   addiu     $a0, $sp, 0x70
    /* AAD0 80009ED0 1000000E */  b          .L80009F0C
    /* AAD4 80009ED4 8FBF0024 */   lw        $ra, 0x24($sp)
  .L80009ED8:
    /* AAD8 80009ED8 27A50050 */  addiu      $a1, $sp, 0x50
    /* AADC 80009EDC 0C003E34 */  jal        func_8000F8D0
    /* AAE0 80009EE0 24060020 */   addiu     $a2, $zero, 0x20
    /* AAE4 80009EE4 10400003 */  beqz       $v0, .L80009EF4
    /* AAE8 80009EE8 00003025 */   or        $a2, $zero, $zero
    /* AAEC 80009EEC 10000006 */  b          .L80009F08
    /* AAF0 80009EF0 2402000B */   addiu     $v0, $zero, 0xB
  .L80009EF4:
    /* AAF4 80009EF4 8E040004 */  lw         $a0, 0x4($s0)
    /* AAF8 80009EF8 8E050008 */  lw         $a1, 0x8($s0)
    /* AAFC 80009EFC AFA00010 */  sw         $zero, 0x10($sp)
    /* AB00 80009F00 0C003DA0 */  jal        func_8000F680
    /* AB04 80009F04 27A70030 */   addiu     $a3, $sp, 0x30
  .L80009F08:
    /* AB08 80009F08 8FBF0024 */  lw         $ra, 0x24($sp)
  .L80009F0C:
    /* AB0C 80009F0C 8FB00020 */  lw         $s0, 0x20($sp)
    /* AB10 80009F10 27BD0098 */  addiu      $sp, $sp, 0x98
    /* AB14 80009F14 03E00008 */  jr         $ra
    /* AB18 80009F18 00000000 */   nop