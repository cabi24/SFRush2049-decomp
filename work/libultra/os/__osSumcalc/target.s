# Source: F700.s
# Address: 0x8000EB00

glabel func_8000EB00
    /* F700 8000EB00 00001825 */  or         $v1, $zero, $zero
    /* F704 8000EB04 00801025 */  or         $v0, $a0, $zero
    /* F708 8000EB08 18A00016 */  blez       $a1, .L8000EB64
    /* F70C 8000EB0C 00003025 */   or        $a2, $zero, $zero
    /* F710 8000EB10 30A70003 */  andi       $a3, $a1, 0x3
    /* F714 8000EB14 10E00008 */  beqz       $a3, .L8000EB38
    /* F718 8000EB18 00E02025 */   or        $a0, $a3, $zero
  .L8000EB1C:
    /* F71C 8000EB1C 904E0000 */  lbu        $t6, 0x0($v0)
    /* F720 8000EB20 24C60001 */  addiu      $a2, $a2, 0x1
    /* F724 8000EB24 24420001 */  addiu      $v0, $v0, 0x1
    /* F728 8000EB28 1486FFFC */  bne        $a0, $a2, .L8000EB1C
    /* F72C 8000EB2C 006E1821 */   addu      $v1, $v1, $t6
    /* F730 8000EB30 50C5000D */  beql       $a2, $a1, .L8000EB68
    /* F734 8000EB34 00601025 */   or        $v0, $v1, $zero
  .L8000EB38:
    /* F738 8000EB38 904F0000 */  lbu        $t7, 0x0($v0)
    /* F73C 8000EB3C 90580001 */  lbu        $t8, 0x1($v0)
    /* F740 8000EB40 90590002 */  lbu        $t9, 0x2($v0)
    /* F744 8000EB44 006F1821 */  addu       $v1, $v1, $t7
    /* F748 8000EB48 90480003 */  lbu        $t0, 0x3($v0)
    /* F74C 8000EB4C 00781821 */  addu       $v1, $v1, $t8
    /* F750 8000EB50 24C60004 */  addiu      $a2, $a2, 0x4
    /* F754 8000EB54 00791821 */  addu       $v1, $v1, $t9
    /* F758 8000EB58 24420004 */  addiu      $v0, $v0, 0x4
    /* F75C 8000EB5C 14C5FFF6 */  bne        $a2, $a1, .L8000EB38
    /* F760 8000EB60 00681821 */   addu      $v1, $v1, $t0
  .L8000EB64:
    /* F764 8000EB64 00601025 */  or         $v0, $v1, $zero
  .L8000EB68:
    /* F768 8000EB68 3049FFFF */  andi       $t1, $v0, 0xFFFF
    /* F76C 8000EB6C 03E00008 */  jr         $ra
    /* F770 8000EB70 01201025 */   or        $v0, $t1, $zero