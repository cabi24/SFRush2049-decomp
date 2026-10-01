nonmatching osPfsReAllocate, 0x18

glabel osPfsReAllocate
    /* CA50 8000BE50 14800003 */  bnez       $a0, .L8000BE60
    /* CA54 8000BE54 00000000 */   nop
    /* CA58 8000BE58 3C048003 */  lui        $a0, %hi(__osRunningThread)
    /* CA5C 8000BE5C 8C84C3E0 */  lw         $a0, %lo(__osRunningThread)($a0)
  .L8000BE60:
    /* CA60 8000BE60 03E00008 */  jr         $ra
    /* CA64 8000BE64 8C820014 */   lw        $v0, 0x14($a0)
endlabel osPfsReAllocate
    /* CA68 8000BE68 00000000 */  nop
    /* CA6C 8000BE6C 00000000 */  nop
