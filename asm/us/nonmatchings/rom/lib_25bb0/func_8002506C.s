nonmatching func_8002506C, 0x40

glabel func_8002506C
    /* 25C6C 8002506C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 25C70 80025070 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 25C74 80025074 00802825 */  or         $a1, $a0, $zero
    /* 25C78 80025078 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 25C7C 8002507C AFA40018 */  sw         $a0, 0x18($sp)
    /* 25C80 80025080 AFA70024 */  sw         $a3, 0x24($sp)
    /* 25C84 80025084 0C0041C5 */  jal        func_80010714
    /* 25C88 80025088 8FA4001C */   lw        $a0, 0x1C($sp)
    /* 25C8C 8002508C 8FA40024 */  lw         $a0, 0x24($sp)
    /* 25C90 80025090 00002825 */  or         $a1, $zero, $zero
    /* 25C94 80025094 0C001D78 */  jal        osJamMesg
    /* 25C98 80025098 24060001 */   addiu     $a2, $zero, 0x1
    /* 25C9C 8002509C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 25CA0 800250A0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 25CA4 800250A4 03E00008 */  jr         $ra
    /* 25CA8 800250A8 00000000 */   nop
endlabel func_8002506C
