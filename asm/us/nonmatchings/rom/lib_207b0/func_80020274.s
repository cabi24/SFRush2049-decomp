nonmatching func_80020274, 0x50

glabel func_80020274
    /* 20E74 80020274 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 20E78 80020278 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 20E7C 8002027C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 20E80 80020280 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 20E84 80020284 51C0000C */  beql       $t6, $zero, .L800202B8
    /* 20E88 80020288 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 20E8C 8002028C 0C005165 */  jal        func_80014594
    /* 20E90 80020290 00000000 */   nop
    /* 20E94 80020294 0C00639B */  jal        func_80018E6C
    /* 20E98 80020298 00000000 */   nop
    /* 20E9C 8002029C 0C00755E */  jal        func_8001D578
    /* 20EA0 800202A0 00000000 */   nop
    /* 20EA4 800202A4 0C007EB9 */  jal        func_8001FAE4
    /* 20EA8 800202A8 00002025 */   or        $a0, $zero, $zero
    /* 20EAC 800202AC 0C005177 */  jal        func_800145DC
    /* 20EB0 800202B0 00000000 */   nop
    /* 20EB4 800202B4 8FBF0014 */  lw         $ra, 0x14($sp)
  .L800202B8:
    /* 20EB8 800202B8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 20EBC 800202BC 03E00008 */  jr         $ra
    /* 20EC0 800202C0 00000000 */   nop
endlabel func_80020274
