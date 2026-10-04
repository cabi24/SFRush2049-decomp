nonmatching func_8001EAA0, 0x4C

glabel func_8001EAA0
    /* 1F6A0 8001EAA0 3C038005 */  lui        $v1, %hi(D_80050C50)
    /* 1F6A4 8001EAA4 8C630C50 */  lw         $v1, %lo(D_80050C50)($v1)
    /* 1F6A8 8001EAA8 5060000E */  beql       $v1, $zero, .L8001EAE4
    /* 1F6AC 8001EAAC 00001025 */   or        $v0, $zero, $zero
    /* 1F6B0 8001EAB0 88620008 */  lwl        $v0, 0x8($v1)
  .L8001EAB4:
    /* 1F6B4 8001EAB4 9862000B */  lwr        $v0, 0xB($v1)
    /* 1F6B8 8001EAB8 14820003 */  bne        $a0, $v0, .L8001EAC8
    /* 1F6BC 8001EABC 0082082B */   sltu      $at, $a0, $v0
    /* 1F6C0 8001EAC0 03E00008 */  jr         $ra
    /* 1F6C4 8001EAC4 00601025 */   or        $v0, $v1, $zero
  .L8001EAC8:
    /* 1F6C8 8001EAC8 14200005 */  bnez       $at, .L8001EAE0
    /* 1F6CC 8001EACC 00600821 */   addu      $at, $v1, $zero
    /* 1F6D0 8001EAD0 88630000 */  lwl        $v1, 0x0($v1)
    /* 1F6D4 8001EAD4 98230003 */  lwr        $v1, 0x3($at)
    /* 1F6D8 8001EAD8 5460FFF6 */  bnel       $v1, $zero, .L8001EAB4
    /* 1F6DC 8001EADC 88620008 */   lwl       $v0, 0x8($v1)
  .L8001EAE0:
    /* 1F6E0 8001EAE0 00001025 */  or         $v0, $zero, $zero
  .L8001EAE4:
    /* 1F6E4 8001EAE4 03E00008 */  jr         $ra
    /* 1F6E8 8001EAE8 00000000 */   nop
endlabel func_8001EAA0
