nonmatching inflate_flush_window, 0x1C

glabel inflate_flush_window
    /* 5950 80004D50 3C018003 */  lui        $at, %hi(gDisplayListHead)
    /* 5954 80004D54 AC2454C4 */  sw         $a0, %lo(gDisplayListHead)($at)
    /* 5958 80004D58 3C018003 */  lui        $at, %hi(gDisplayListEnd)
    /* 595C 80004D5C AC2554CC */  sw         $a1, %lo(gDisplayListEnd)($at)
    /* 5960 80004D60 3C018003 */  lui        $at, %hi(gDisplayListSize)
    /* 5964 80004D64 03E00008 */  jr         $ra
    /* 5968 80004D68 AC2054C8 */   sw        $zero, %lo(gDisplayListSize)($at)
endlabel inflate_flush_window
