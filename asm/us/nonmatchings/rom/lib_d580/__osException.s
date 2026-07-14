/* Handwritten function */
nonmatching __osException, 0x10

glabel __osException
    /* D580 8000C980 3C1A8001 */  lui        $k0, %hi(__osExceptionPreamble) /* handwritten instruction */
    /* D584 8000C984 275AC990 */  addiu      $k0, $k0, %lo(__osExceptionPreamble) /* handwritten instruction */
    /* D588 8000C988 03400008 */  jr         $k0
    /* D58C 8000C98C 00000000 */   nop
endlabel __osException
