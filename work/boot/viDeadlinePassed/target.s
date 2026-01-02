# Source: 1050.s
# Address: 0x800015BC

glabel func_800015BC
    /* 21BC 800015BC 3C0E8003 */  lui        $t6, %hi(D_8002EB9C)
    /* 21C0 800015C0 3C0F8003 */  lui        $t7, %hi(D_8002EB64)
    /* 21C4 800015C4 8DEFEB64 */  lw         $t7, %lo(D_8002EB64)($t7)
    /* 21C8 800015C8 8DCEEB9C */  lw         $t6, %lo(D_8002EB9C)($t6)
    /* 21CC 800015CC 01CF1023 */  subu       $v0, $t6, $t7
    /* 21D0 800015D0 28580001 */  slti       $t8, $v0, 0x1
    /* 21D4 800015D4 03E00008 */  jr         $ra
    /* 21D8 800015D8 03001025 */   or        $v0, $t8, $zero