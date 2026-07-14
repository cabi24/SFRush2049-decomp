nonmatching __osPopThread, 0x10

glabel __osPopThread
    /* DD0C 8000D10C 8C820000 */  lw         $v0, 0x0($a0)
    /* DD10 8000D110 8C590000 */  lw         $t9, 0x0($v0)
    /* DD14 8000D114 03E00008 */  jr         $ra
    /* DD18 8000D118 AC990000 */   sw        $t9, 0x0($a0)
endlabel __osPopThread
