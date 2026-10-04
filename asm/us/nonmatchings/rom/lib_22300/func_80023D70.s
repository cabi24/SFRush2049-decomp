nonmatching func_80023D70, 0x48

glabel func_80023D70
    /* 24970 80023D70 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 24974 80023D74 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 24978 80023D78 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 2497C 80023D7C 8CA20000 */  lw         $v0, 0x0($a1)
    /* 24980 80023D80 00A07025 */  or         $t6, $a1, $zero
    /* 24984 80023D84 8DC70004 */  lw         $a3, 0x4($t6)
    /* 24988 80023D88 00022A02 */  srl        $a1, $v0, 8
    /* 2498C 80023D8C 00023402 */  srl        $a2, $v0, 16
    /* 24990 80023D90 00073C00 */  sll        $a3, $a3, 16
    /* 24994 80023D94 00073C03 */  sra        $a3, $a3, 16
    /* 24998 80023D98 30C600FF */  andi       $a2, $a2, 0xFF
    /* 2499C 80023D9C 0C008ED4 */  jal        func_80023B50
    /* 249A0 80023DA0 30A500FF */   andi      $a1, $a1, 0xFF
    /* 249A4 80023DA4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 249A8 80023DA8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 249AC 80023DAC 00001025 */  or         $v0, $zero, $zero
    /* 249B0 80023DB0 03E00008 */  jr         $ra
    /* 249B4 80023DB4 00000000 */   nop
endlabel func_80023D70
