nonmatching func_8001EE9C, 0xF0

glabel func_8001EE9C
    /* 1FA9C 8001EE9C 888E0060 */  lwl        $t6, 0x60($a0)
    /* 1FAA0 8001EEA0 988E0063 */  lwr        $t6, 0x63($a0)
    /* 1FAA4 8001EEA4 3C058005 */  lui        $a1, %hi(D_800504C8)
    /* 1FAA8 8001EEA8 24A504C8 */  addiu      $a1, $a1, %lo(D_800504C8)
    /* 1FAAC 8001EEAC 31CF00FF */  andi       $t7, $t6, 0xFF
    /* 1FAB0 8001EEB0 000FC080 */  sll        $t8, $t7, 2
    /* 1FAB4 8001EEB4 00B81021 */  addu       $v0, $a1, $t8
    /* 1FAB8 8001EEB8 94590002 */  lhu        $t9, 0x2($v0)
    /* 1FABC 8001EEBC 24010001 */  addiu      $at, $zero, 0x1
    /* 1FAC0 8001EEC0 17210030 */  bne        $t9, $at, .L8001EF84
    /* 1FAC4 8001EEC4 00000000 */   nop
    /* 1FAC8 8001EEC8 90430000 */  lbu        $v1, 0x0($v0)
    /* 1FACC 8001EECC 240600FF */  addiu      $a2, $zero, 0xFF
    /* 1FAD0 8001EED0 10C30005 */  beq        $a2, $v1, .L8001EEE8
    /* 1FAD4 8001EED4 00034880 */   sll       $t1, $v1, 2
    /* 1FAD8 8001EED8 90480001 */  lbu        $t0, 0x1($v0)
    /* 1FADC 8001EEDC 00A95021 */  addu       $t2, $a1, $t1
    /* 1FAE0 8001EEE0 10000006 */  b          .L8001EEFC
    /* 1FAE4 8001EEE4 A1480001 */   sb        $t0, 0x1($t2)
  .L8001EEE8:
    /* 1FAE8 8001EEE8 908C002E */  lbu        $t4, 0x2E($a0)
    /* 1FAEC 8001EEEC 904B0001 */  lbu        $t3, 0x1($v0)
    /* 1FAF0 8001EEF0 3C018005 */  lui        $at, %hi(D_80050548)
    /* 1FAF4 8001EEF4 002C0821 */  addu       $at, $at, $t4
    /* 1FAF8 8001EEF8 A02B0548 */  sb         $t3, %lo(D_80050548)($at)
  .L8001EEFC:
    /* 1FAFC 8001EEFC 90430001 */  lbu        $v1, 0x1($v0)
    /* 1FB00 8001EF00 10C30005 */  beq        $a2, $v1, .L8001EF18
    /* 1FB04 8001EF04 00037080 */   sll       $t6, $v1, 2
    /* 1FB08 8001EF08 904D0000 */  lbu        $t5, 0x0($v0)
    /* 1FB0C 8001EF0C 00AE7821 */  addu       $t7, $a1, $t6
    /* 1FB10 8001EF10 1000001B */  b          .L8001EF80
    /* 1FB14 8001EF14 A1ED0000 */   sb        $t5, 0x0($t7)
  .L8001EF18:
    /* 1FB18 8001EF18 90580000 */  lbu        $t8, 0x0($v0)
    /* 1FB1C 8001EF1C 54D80019 */  bnel       $a2, $t8, .L8001EF84
    /* 1FB20 8001EF20 A4400002 */   sh        $zero, 0x2($v0)
    /* 1FB24 8001EF24 9099002E */  lbu        $t9, 0x2E($a0)
    /* 1FB28 8001EF28 3C068005 */  lui        $a2, %hi(D_80050648)
    /* 1FB2C 8001EF2C 24C60648 */  addiu      $a2, $a2, %lo(D_80050648)
    /* 1FB30 8001EF30 00194880 */  sll        $t1, $t9, 2
    /* 1FB34 8001EF34 00C91821 */  addu       $v1, $a2, $t1
    /* 1FB38 8001EF38 94650002 */  lhu        $a1, 0x2($v1)
    /* 1FB3C 8001EF3C 3407FFFF */  ori        $a3, $zero, 0xFFFF
    /* 1FB40 8001EF40 10E50005 */  beq        $a3, $a1, .L8001EF58
    /* 1FB44 8001EF44 00055080 */   sll       $t2, $a1, 2
    /* 1FB48 8001EF48 94680000 */  lhu        $t0, 0x0($v1)
    /* 1FB4C 8001EF4C 00CA5821 */  addu       $t3, $a2, $t2
    /* 1FB50 8001EF50 10000004 */  b          .L8001EF64
    /* 1FB54 8001EF54 A5680000 */   sh        $t0, 0x0($t3)
  .L8001EF58:
    /* 1FB58 8001EF58 946C0000 */  lhu        $t4, 0x0($v1)
    /* 1FB5C 8001EF5C 3C018005 */  lui        $at, %hi(D_80050A48)
    /* 1FB60 8001EF60 A42C0A48 */  sh         $t4, %lo(D_80050A48)($at)
  .L8001EF64:
    /* 1FB64 8001EF64 94640000 */  lhu        $a0, 0x0($v1)
    /* 1FB68 8001EF68 50E40006 */  beql       $a3, $a0, .L8001EF84
    /* 1FB6C 8001EF6C A4400002 */   sh        $zero, 0x2($v0)
    /* 1FB70 8001EF70 946E0002 */  lhu        $t6, 0x2($v1)
    /* 1FB74 8001EF74 00046880 */  sll        $t5, $a0, 2
    /* 1FB78 8001EF78 00CD7821 */  addu       $t7, $a2, $t5
    /* 1FB7C 8001EF7C A5EE0002 */  sh         $t6, 0x2($t7)
  .L8001EF80:
    /* 1FB80 8001EF80 A4400002 */  sh         $zero, 0x2($v0)
  .L8001EF84:
    /* 1FB84 8001EF84 03E00008 */  jr         $ra
    /* 1FB88 8001EF88 00000000 */   nop
endlabel func_8001EE9C
