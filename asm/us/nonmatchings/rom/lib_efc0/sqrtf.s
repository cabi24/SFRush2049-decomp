nonmatching sqrtf, 0x8

glabel sqrtf
    /* EFC0 8000E3C0 03E00008 */  jr         $ra
    /* EFC4 8000E3C4 46006004 */   sqrt.s    $fv0, $fa0
endlabel sqrtf
    /* EFC8 8000E3C8 00000000 */  nop
    /* EFCC 8000E3CC 00000000 */  nop
