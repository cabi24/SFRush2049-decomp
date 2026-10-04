nonmatching func_8001906C, 0x40

glabel func_8001906C
    /* 19C6C 8001906C 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 19C70 80019070 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 19C74 80019074 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 19C78 80019078 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 19C7C 8001907C 11C00007 */  beqz       $t6, .L8001909C
    /* 19C80 80019080 AFA40018 */   sw        $a0, 0x18($sp)
    /* 19C84 80019084 0C005165 */  jal        func_80014594
    /* 19C88 80019088 00000000 */   nop
    /* 19C8C 8001908C 0C0063FB */  jal        func_80018FEC
    /* 19C90 80019090 8FA40018 */   lw        $a0, 0x18($sp)
    /* 19C94 80019094 0C005177 */  jal        func_800145DC
    /* 19C98 80019098 00000000 */   nop
  .L8001909C:
    /* 19C9C 8001909C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 19CA0 800190A0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 19CA4 800190A4 03E00008 */  jr         $ra
    /* 19CA8 800190A8 00000000 */   nop
endlabel func_8001906C
