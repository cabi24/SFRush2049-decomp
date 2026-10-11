/* Handwritten function */
nonmatching osCauseGet, 0xC

glabel osCauseGet
    /* E7B0 8000DBB0 40026800 */  mfc0       $v0, $13 /* handwritten instruction */
    /* E7B4 8000DBB4 03E00008 */  jr         $ra
    /* E7B8 8000DBB8 00000000 */   nop
endlabel osCauseGet
    /* E7BC 8000DBBC 00000000 */  nop
