# Source: C990.s
# Address: 0x8000BE50

glabel func_8000BE50
    /* CA50 8000BE50 14800003 */  bnez       $a0, .L8000BE60
    /* CA54 8000BE54 00000000 */   nop
    /* CA58 8000BE58 3C048003 */  lui        $a0, %hi(D_8002C3E0)
    /* CA5C 8000BE5C 8C84C3E0 */  lw         $a0, %lo(D_8002C3E0)($a0)
  .L8000BE60:
    /* CA60 8000BE60 03E00008 */  jr         $ra
    /* CA64 8000BE64 8C820014 */   lw        $v0, 0x14($a0)