nonmatching func_80021BC0, 0x30

glabel func_80021BC0
    /* 227C0 80021BC0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 227C4 80021BC4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 227C8 80021BC8 AFA40018 */  sw         $a0, 0x18($sp)
    /* 227CC 80021BCC 0C007AC4 */  jal        func_8001EB10
    /* 227D0 80021BD0 AFA5001C */   sw        $a1, 0x1C($sp)
    /* 227D4 80021BD4 0C007DBB */  jal        func_8001F6EC
    /* 227D8 80021BD8 8FA40018 */   lw        $a0, 0x18($sp)
    /* 227DC 80021BDC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 227E0 80021BE0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 227E4 80021BE4 24020001 */  addiu      $v0, $zero, 0x1
    /* 227E8 80021BE8 03E00008 */  jr         $ra
    /* 227EC 80021BEC 00000000 */   nop
endlabel func_80021BC0
