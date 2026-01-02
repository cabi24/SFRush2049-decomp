# Source: A5F0.s
# Address: 0x80009A0C

glabel func_80009A0C
    /* A60C 80009A0C 46006109 */  trunc.l.s  $ft0, $fa0
    /* A610 80009A10 44222000 */  dmfc1      $v0, $ft0
    /* A614 80009A14 00000000 */  nop
    /* A618 80009A18 0002183C */  dsll32     $v1, $v0, 0
    /* A61C 80009A1C 0003183F */  dsra32     $v1, $v1, 0
    /* A620 80009A20 03E00008 */  jr         $ra
    /* A624 80009A24 0002103F */   dsra32    $v0, $v0, 0