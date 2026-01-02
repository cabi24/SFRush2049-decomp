# Source: A5F0.s
# Address: 0x800099F0

glabel func_800099F0
    /* A5F0 800099F0 46206109 */  trunc.l.d  $ft0, $fa0
    /* A5F4 800099F4 44222000 */  dmfc1      $v0, $ft0
    /* A5F8 800099F8 00000000 */  nop
    /* A5FC 800099FC 0002183C */  dsll32     $v1, $v0, 0
    /* A600 80009A00 0003183F */  dsra32     $v1, $v1, 0
    /* A604 80009A04 03E00008 */  jr         $ra
    /* A608 80009A08 0002103F */   dsra32    $v0, $v0, 0