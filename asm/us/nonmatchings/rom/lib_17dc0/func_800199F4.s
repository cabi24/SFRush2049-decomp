nonmatching func_800199F4, 0x64

glabel func_800199F4
    /* 1A5F4 800199F4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1A5F8 800199F8 3C028004 */  lui        $v0, %hi(D_80043EB8)
    /* 1A5FC 800199FC 3C048005 */  lui        $a0, %hi(D_8004BE78)
    /* 1A600 80019A00 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1A604 80019A04 2484BE78 */  addiu      $a0, $a0, %lo(D_8004BE78)
    /* 1A608 80019A08 24423EB8 */  addiu      $v0, $v0, %lo(D_80043EB8)
    /* 1A60C 80019A0C 24030001 */  addiu      $v1, $zero, 0x1
  .L80019A10:
    /* 1A610 80019A10 24423FE0 */  addiu      $v0, $v0, 0x3FE0
    /* 1A614 80019A14 A040DFD8 */  sb         $zero, -0x2028($v0)
    /* 1A618 80019A18 A043DFD9 */  sb         $v1, -0x2027($v0)
    /* 1A61C 80019A1C A040EFD0 */  sb         $zero, -0x1030($v0)
    /* 1A620 80019A20 A043EFD1 */  sb         $v1, -0x102F($v0)
    /* 1A624 80019A24 A040FFC8 */  sb         $zero, -0x38($v0)
    /* 1A628 80019A28 A043FFC9 */  sb         $v1, -0x37($v0)
    /* 1A62C 80019A2C A040CFE0 */  sb         $zero, -0x3020($v0)
    /* 1A630 80019A30 1444FFF7 */  bne        $v0, $a0, .L80019A10
    /* 1A634 80019A34 A043CFE1 */   sb        $v1, -0x301F($v0)
    /* 1A638 80019A38 0C005C70 */  jal        func_800171C0
    /* 1A63C 80019A3C 00000000 */   nop
    /* 1A640 80019A40 0C005D6A */  jal        func_800175A8
    /* 1A644 80019A44 00000000 */   nop
    /* 1A648 80019A48 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1A64C 80019A4C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1A650 80019A50 03E00008 */  jr         $ra
    /* 1A654 80019A54 00000000 */   nop
endlabel func_800199F4
    /* 1A658 80019A58 00000000 */  nop
    /* 1A65C 80019A5C 00000000 */  nop
