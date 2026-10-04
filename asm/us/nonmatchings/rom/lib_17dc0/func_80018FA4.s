nonmatching func_80018FA4, 0x48

glabel func_80018FA4
    /* 19BA4 80018FA4 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 19BA8 80018FA8 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 19BAC 80018FAC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 19BB0 80018FB0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 19BB4 80018FB4 AFA40018 */  sw         $a0, 0x18($sp)
    /* 19BB8 80018FB8 11C00008 */  beqz       $t6, .L80018FDC
    /* 19BBC 80018FBC AFA5001C */   sw        $a1, 0x1C($sp)
    /* 19BC0 80018FC0 0C005165 */  jal        func_80014594
    /* 19BC4 80018FC4 00000000 */   nop
    /* 19BC8 80018FC8 8FA40018 */  lw         $a0, 0x18($sp)
    /* 19BCC 80018FCC 0C0063C8 */  jal        func_80018F20
    /* 19BD0 80018FD0 97A5001E */   lhu       $a1, 0x1E($sp)
    /* 19BD4 80018FD4 0C005177 */  jal        func_800145DC
    /* 19BD8 80018FD8 00000000 */   nop
  .L80018FDC:
    /* 19BDC 80018FDC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 19BE0 80018FE0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 19BE4 80018FE4 03E00008 */  jr         $ra
    /* 19BE8 80018FE8 00000000 */   nop
endlabel func_80018FA4
