nonmatching func_80024FB0, 0x24

glabel func_80024FB0
    /* 25BB0 80024FB0 3C0E8004 */  lui        $t6, %hi(D_80038290)
    /* 25BB4 80024FB4 91CE8290 */  lbu        $t6, %lo(D_80038290)($t6)
    /* 25BB8 80024FB8 11C00004 */  beqz       $t6, .L80024FCC
    /* 25BBC 80024FBC 00000000 */   nop
    /* 25BC0 80024FC0 808F11DE */  lb         $t7, 0x11DE($a0)
    /* 25BC4 80024FC4 000FC040 */  sll        $t8, $t7, 1
    /* 25BC8 80024FC8 A09811DE */  sb         $t8, 0x11DE($a0)
  .L80024FCC:
    /* 25BCC 80024FCC 03E00008 */  jr         $ra
    /* 25BD0 80024FD0 00000000 */   nop
endlabel func_80024FB0
