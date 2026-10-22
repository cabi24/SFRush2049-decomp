/* Handwritten function */
nonmatching __osExceptionPreamble, 0x10

glabel __osExceptionPreamble
    /* D580 8000C980 3C1A8001 */  lui        $k0, %hi(__osException) /* handwritten instruction */
    /* D584 8000C984 275AC990 */  addiu      $k0, $k0, %lo(__osException) /* handwritten instruction */
    /* D588 8000C988 03400008 */  jr         $k0
    /* D58C 8000C98C 00000000 */   nop
endlabel __osExceptionPreamble
