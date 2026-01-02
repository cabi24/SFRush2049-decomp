# Source: A5F0.s
# Address: 0x80009B7C

glabel func_80009B7C
    /* A77C 80009B7C AFA40000 */  sw         $a0, 0x0($sp)
    /* A780 80009B80 AFA50004 */  sw         $a1, 0x4($sp)
    /* A784 80009B84 DFAE0000 */  ld         $t6, 0x0($sp)
    /* A788 80009B88 44AE2000 */  dmtc1      $t6, $ft0
    /* A78C 80009B8C 03E00008 */  jr         $ra
    /* A790 80009B90 46A02020 */   cvt.s.l   $fv0, $ft0