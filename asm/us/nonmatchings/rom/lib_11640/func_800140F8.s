nonmatching func_800140F8, 0x48

glabel func_800140F8
    /* 14CF8 800140F8 340EFFFF */  ori        $t6, $zero, 0xFFFF
    /* 14CFC 800140FC 3C018004 */  lui        $at, %hi(D_80038360)
    /* 14D00 80014100 A42E8360 */  sh         $t6, %lo(D_80038360)($at)
    /* 14D04 80014104 3C198004 */  lui        $t9, %hi(D_80038008)
    /* 14D08 80014108 8F398008 */  lw         $t9, %lo(D_80038008)($t9)
    /* 14D0C 8001410C 3C018004 */  lui        $at, %hi(D_80038020)
    /* 14D10 80014110 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 14D14 80014114 AC208020 */  sw         $zero, %lo(D_80038020)($at)
    /* 14D18 80014118 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 14D1C 8001411C 3C018004 */  lui        $at, %hi(D_80038024)
    /* 14D20 80014120 3C048001 */  lui        $a0, %hi(func_80013DEC)
    /* 14D24 80014124 AC208024 */  sw         $zero, %lo(D_80038024)($at)
    /* 14D28 80014128 0320F809 */  jalr       $t9
    /* 14D2C 8001412C 24843DEC */   addiu     $a0, $a0, %lo(func_80013DEC)
    /* 14D30 80014130 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 14D34 80014134 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 14D38 80014138 03E00008 */  jr         $ra
    /* 14D3C 8001413C 00000000 */   nop
endlabel func_800140F8
