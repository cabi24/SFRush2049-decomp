nonmatching __osSetFpcCsr, 0x10

glabel __osSetFpcCsr
    /* E390 8000D790 4442F800 */  cfc1       $v0, $31
    /* E394 8000D794 44C4F800 */  ctc1       $a0, $31
    /* E398 8000D798 03E00008 */  jr         $ra
    /* E39C 8000D79C 00000000 */   nop
endlabel __osSetFpcCsr
