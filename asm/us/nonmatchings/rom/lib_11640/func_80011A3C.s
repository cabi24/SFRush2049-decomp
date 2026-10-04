nonmatching func_80011A3C, 0x28

glabel func_80011A3C
    /* 1263C 80011A3C C484004C */  lwc1       $ft0, 0x4C($a0)
    /* 12640 80011A40 44803000 */  mtc1       $zero, $ft1
    /* 12644 80011A44 948F0028 */  lhu        $t7, 0x28($a0)
    /* 12648 80011A48 240E0003 */  addiu      $t6, $zero, 0x3
    /* 1264C 80011A4C A08E005C */  sb         $t6, 0x5C($a0)
    /* 12650 80011A50 AC800050 */  sw         $zero, 0x50($a0)
    /* 12654 80011A54 E4840058 */  swc1       $ft0, 0x58($a0)
    /* 12658 80011A58 E4860054 */  swc1       $ft1, 0x54($a0)
    /* 1265C 80011A5C 03E00008 */  jr         $ra
    /* 12660 80011A60 A48F0048 */   sh        $t7, 0x48($a0)
endlabel func_80011A3C
