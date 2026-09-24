nonmatching viGetTimeToDeadline, 0x44

glabel viGetTimeToDeadline
    /* 2178 80001578 3C0E8003 */  lui        $t6, %hi(gViAccumTime)
    /* 217C 8000157C 3C0F8003 */  lui        $t7, %hi(gViTickCounter)
    /* 2180 80001580 8DEFEB64 */  lw         $t7, %lo(gViTickCounter)($t7)
    /* 2184 80001584 8DCEEB9C */  lw         $t6, %lo(gViAccumTime)($t6)
    /* 2188 80001588 3C018003 */  lui        $at, %hi(gScaleSecondsPerTick)
    /* 218C 8000158C C428AFB8 */  lwc1       $ft2, %lo(gScaleSecondsPerTick)($at)
    /* 2190 80001590 01CFC023 */  subu       $t8, $t6, $t7
    /* 2194 80001594 44982000 */  mtc1       $t8, $ft0
    /* 2198 80001598 00000000 */  nop
    /* 219C 8000159C 468021A0 */  cvt.s.w    $ft1, $ft0
    /* 21A0 800015A0 46083002 */  mul.s      $fv0, $ft1, $ft2
    /* 21A4 800015A4 03E00008 */  jr         $ra
    /* 21A8 800015A8 00000000 */   nop
    /* 21AC 800015AC 03E00008 */  jr         $ra
    /* 21B0 800015B0 00000000 */   nop
    /* 21B4 800015B4 03E00008 */  jr         $ra
    /* 21B8 800015B8 00000000 */   nop
endlabel viGetTimeToDeadline
