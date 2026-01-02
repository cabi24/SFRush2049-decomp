# Source: 83D0.s
# Address: 0x800077D0

glabel func_800077D0
    /* 83D0 800077D0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 83D4 800077D4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 83D8 800077D8 0C00312C */  jal        func_8000C4B0
    /* 83DC 800077DC 00000000 */   nop
    /* 83E0 800077E0 3C0E8003 */  lui        $t6, %hi(D_8002C464)
    /* 83E4 800077E4 8DCEC464 */  lw         $t6, %lo(D_8002C464)($t6)
    /* 83E8 800077E8 00402025 */  or         $a0, $v0, $zero
    /* 83EC 800077EC 8DCF0004 */  lw         $t7, 0x4($t6)
    /* 83F0 800077F0 0C003148 */  jal        func_8000C520
    /* 83F4 800077F4 AFAF0018 */   sw        $t7, 0x18($sp)
    /* 83F8 800077F8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 83FC 800077FC 8FA20018 */  lw         $v0, 0x18($sp)
    /* 8400 80007800 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 8404 80007804 03E00008 */  jr         $ra
    /* 8408 80007808 00000000 */   nop