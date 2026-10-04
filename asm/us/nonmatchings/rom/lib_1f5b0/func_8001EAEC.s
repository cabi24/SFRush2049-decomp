nonmatching func_8001EAEC, 0x24

glabel func_8001EAEC
    /* 1F6EC 8001EAEC 3C048005 */  lui        $a0, %hi(D_80050A4C)
    /* 1F6F0 8001EAF0 24840A4C */  addiu      $a0, $a0, %lo(D_80050A4C)
    /* 1F6F4 8001EAF4 2405FFFF */  addiu      $a1, $zero, -0x1
  .L8001EAF8:
    /* 1F6F8 8001EAF8 8C820000 */  lw         $v0, 0x0($a0)
    /* 1F6FC 8001EAFC 244E0001 */  addiu      $t6, $v0, 0x1
    /* 1F700 8001EB00 1045FFFD */  beq        $v0, $a1, .L8001EAF8
    /* 1F704 8001EB04 AC8E0000 */   sw        $t6, 0x0($a0)
    /* 1F708 8001EB08 03E00008 */  jr         $ra
    /* 1F70C 8001EB0C 00000000 */   nop
endlabel func_8001EAEC
