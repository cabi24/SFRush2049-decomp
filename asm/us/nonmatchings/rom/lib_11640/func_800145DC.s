nonmatching func_800145DC, 0x48

glabel func_800145DC
    /* 151DC 800145DC 3C038003 */  lui        $v1, %hi(D_8002C5DC)
    /* 151E0 800145E0 2463C5DC */  addiu      $v1, $v1, %lo(D_8002C5DC)
    /* 151E4 800145E4 8C620000 */  lw         $v0, 0x0($v1)
    /* 151E8 800145E8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 151EC 800145EC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 151F0 800145F0 18400008 */  blez       $v0, .L80014614
    /* 151F4 800145F4 244EFFFF */   addiu     $t6, $v0, -0x1
    /* 151F8 800145F8 15C00006 */  bnez       $t6, .L80014614
    /* 151FC 800145FC AC6E0000 */   sw        $t6, 0x0($v1)
    /* 15200 80014600 3C048004 */  lui        $a0, %hi(D_80038368)
    /* 15204 80014604 24848368 */  addiu      $a0, $a0, %lo(D_80038368)
    /* 15208 80014608 00002825 */  or         $a1, $zero, $zero
    /* 1520C 8001460C 0C001D78 */  jal        osJamMesg
    /* 15210 80014610 00003025 */   or        $a2, $zero, $zero
  .L80014614:
    /* 15214 80014614 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 15218 80014618 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1521C 8001461C 03E00008 */  jr         $ra
    /* 15220 80014620 00000000 */   nop
endlabel func_800145DC
