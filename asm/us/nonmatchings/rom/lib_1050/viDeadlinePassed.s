nonmatching viDeadlinePassed, 0x20

glabel viDeadlinePassed
    /* 21BC 800015BC 3C0E8003 */  lui        $t6, %hi(gViAccumTime)
    /* 21C0 800015C0 3C0F8003 */  lui        $t7, %hi(gViTickCounter)
    /* 21C4 800015C4 8DEFEB64 */  lw         $t7, %lo(gViTickCounter)($t7)
    /* 21C8 800015C8 8DCEEB9C */  lw         $t6, %lo(gViAccumTime)($t6)
    /* 21CC 800015CC 01CF1023 */  subu       $v0, $t6, $t7
    /* 21D0 800015D0 28580001 */  slti       $t8, $v0, 0x1
    /* 21D4 800015D4 03E00008 */  jr         $ra
    /* 21D8 800015D8 03001025 */   or        $v0, $t8, $zero
endlabel viDeadlinePassed
