nonmatching func_800105C4, 0x58

glabel func_800105C4
    /* 111C4 800105C4 3C0E8004 */  lui        $t6, %hi(D_80038020)
    /* 111C8 800105C8 8DCE8020 */  lw         $t6, %lo(D_80038020)($t6)
    /* 111CC 800105CC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 111D0 800105D0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 111D4 800105D4 15C0000D */  bnez       $t6, .L8001060C
    /* 111D8 800105D8 3C048003 */   lui       $a0, %hi(D_80037FE0)
    /* 111DC 800105DC 3C058003 */  lui        $a1, %hi(D_80037FF8)
    /* 111E0 800105E0 24A57FF8 */  addiu      $a1, $a1, %lo(D_80037FF8)
    /* 111E4 800105E4 24847FE0 */  addiu      $a0, $a0, %lo(D_80037FE0)
    /* 111E8 800105E8 0C001A80 */  jal        osCreateMesgQueue
    /* 111EC 800105EC 24060001 */   addiu     $a2, $zero, 0x1
    /* 111F0 800105F0 3C0F8003 */  lui        $t7, %hi(D_80037FA0)
    /* 111F4 800105F4 25EF7FA0 */  addiu      $t7, $t7, %lo(D_80037FA0)
    /* 111F8 800105F8 3C188001 */  lui        $t8, %hi(func_80010450)
    /* 111FC 800105FC A1E00000 */  sb         $zero, 0x0($t7)
    /* 11200 80010600 27180450 */  addiu      $t8, $t8, %lo(func_80010450)
    /* 11204 80010604 3C018004 */  lui        $at, %hi(D_80038020)
    /* 11208 80010608 AC388020 */  sw         $t8, %lo(D_80038020)($at)
  .L8001060C:
    /* 1120C 8001060C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 11210 80010610 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 11214 80010614 03E00008 */  jr         $ra
    /* 11218 80010618 00000000 */   nop
endlabel func_800105C4
