nonmatching func_800225AC, 0x30

glabel func_800225AC
    /* 231AC 800225AC AFA40000 */  sw         $a0, 0x0($sp)
    /* 231B0 800225B0 8CA30000 */  lw         $v1, 0x0($a1)
    /* 231B4 800225B4 3C018005 */  lui        $at, %hi(D_8004BE98)
    /* 231B8 800225B8 00001025 */  or         $v0, $zero, $zero
    /* 231BC 800225BC 0003C202 */  srl        $t8, $v1, 8
    /* 231C0 800225C0 331900FF */  andi       $t9, $t8, 0xFF
    /* 231C4 800225C4 00194040 */  sll        $t0, $t9, 1
    /* 231C8 800225C8 00037402 */  srl        $t6, $v1, 16
    /* 231CC 800225CC 31CF00FF */  andi       $t7, $t6, 0xFF
    /* 231D0 800225D0 00280821 */  addu       $at, $at, $t0
    /* 231D4 800225D4 03E00008 */  jr         $ra
    /* 231D8 800225D8 A42FBE98 */   sh        $t7, %lo(D_8004BE98)($at)
endlabel func_800225AC
