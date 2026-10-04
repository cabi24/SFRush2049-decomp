nonmatching func_80014E1C, 0x48

glabel func_80014E1C
    /* 15A1C 80014E1C AFA40000 */  sw         $a0, 0x0($sp)
    /* 15A20 80014E20 8CA20000 */  lw         $v0, 0x0($a1)
    /* 15A24 80014E24 3084FFFF */  andi       $a0, $a0, 0xFFFF
    /* 15A28 80014E28 2406FFFF */  addiu      $a2, $zero, -0x1
    /* 15A2C 80014E2C 10C2000A */  beq        $a2, $v0, .L80014E58
    /* 15A30 80014E30 00801825 */   or        $v1, $a0, $zero
    /* 15A34 80014E34 94AE0004 */  lhu        $t6, 0x4($a1)
  .L80014E38:
    /* 15A38 80014E38 546E0004 */  bnel       $v1, $t6, .L80014E4C
    /* 15A3C 80014E3C 00A22821 */   addu      $a1, $a1, $v0
    /* 15A40 80014E40 03E00008 */  jr         $ra
    /* 15A44 80014E44 00A01025 */   or        $v0, $a1, $zero
    /* 15A48 80014E48 00A22821 */  addu       $a1, $a1, $v0
  .L80014E4C:
    /* 15A4C 80014E4C 8CA20000 */  lw         $v0, 0x0($a1)
    /* 15A50 80014E50 54C2FFF9 */  bnel       $a2, $v0, .L80014E38
    /* 15A54 80014E54 94AE0004 */   lhu       $t6, 0x4($a1)
  .L80014E58:
    /* 15A58 80014E58 00001025 */  or         $v0, $zero, $zero
    /* 15A5C 80014E5C 03E00008 */  jr         $ra
    /* 15A60 80014E60 00000000 */   nop
endlabel func_80014E1C
