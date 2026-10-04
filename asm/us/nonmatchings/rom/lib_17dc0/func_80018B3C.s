nonmatching func_80018B3C, 0x50

glabel func_80018B3C
    /* 1973C 80018B3C 3C058005 */  lui        $a1, %hi(D_8004BE80)
    /* 19740 80018B40 24A5BE80 */  addiu      $a1, $a1, %lo(D_8004BE80)
    /* 19744 80018B44 8CA20000 */  lw         $v0, 0x0($a1)
    /* 19748 80018B48 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1974C 80018B4C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 19750 80018B50 8C430F68 */  lw         $v1, 0xF68($v0)
    /* 19754 80018B54 5060000A */  beql       $v1, $zero, .L80018B80
    /* 19758 80018B58 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 1975C 80018B5C AC430F6C */  sw         $v1, 0xF6C($v0)
    /* 19760 80018B60 8CAE0000 */  lw         $t6, 0x0($a1)
    /* 19764 80018B64 ADC40F74 */  sw         $a0, 0xF74($t6)
    /* 19768 80018B68 8CAF0000 */  lw         $t7, 0x0($a1)
    /* 1976C 80018B6C 0C00628C */  jal        func_80018A30
    /* 19770 80018B70 ADE00F70 */   sw        $zero, 0xF70($t7)
    /* 19774 80018B74 0C00625F */  jal        func_8001897C
    /* 19778 80018B78 00000000 */   nop
    /* 1977C 80018B7C 8FBF0014 */  lw         $ra, 0x14($sp)
  .L80018B80:
    /* 19780 80018B80 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 19784 80018B84 03E00008 */  jr         $ra
    /* 19788 80018B88 00000000 */   nop
endlabel func_80018B3C
