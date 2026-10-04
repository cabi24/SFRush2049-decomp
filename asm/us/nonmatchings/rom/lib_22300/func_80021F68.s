nonmatching func_80021F68, 0x1C

glabel func_80021F68
    /* 22B68 80021F68 AFA50004 */  sw         $a1, 0x4($sp)
    /* 22B6C 80021F6C A880001C */  swl        $zero, 0x1C($a0)
    /* 22B70 80021F70 A8800020 */  swl        $zero, 0x20($a0)
    /* 22B74 80021F74 B8800023 */  swr        $zero, 0x23($a0)
    /* 22B78 80021F78 B880001F */  swr        $zero, 0x1F($a0)
    /* 22B7C 80021F7C 03E00008 */  jr         $ra
    /* 22B80 80021F80 00001025 */   or        $v0, $zero, $zero
endlabel func_80021F68
