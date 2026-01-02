# Source: A5F0.s
# Address: 0x80009B64

glabel func_80009B64
    /* A764 80009B64 AFA40000 */  sw         $a0, 0x0($sp)
    /* A768 80009B68 AFA50004 */  sw         $a1, 0x4($sp)
    /* A76C 80009B6C DFAE0000 */  ld         $t6, 0x0($sp)
    /* A770 80009B70 44AE2000 */  dmtc1      $t6, $ft0
    /* A774 80009B74 03E00008 */  jr         $ra
    /* A778 80009B78 46A02021 */   cvt.d.l   $fv0, $ft0