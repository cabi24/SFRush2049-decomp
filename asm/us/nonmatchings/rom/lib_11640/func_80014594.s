nonmatching func_80014594, 0x48

glabel func_80014594
    /* 15194 80014594 3C028003 */  lui        $v0, %hi(D_8002C5DC)
    /* 15198 80014598 8C42C5DC */  lw         $v0, %lo(D_8002C5DC)($v0)
    /* 1519C 8001459C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 151A0 800145A0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 151A4 800145A4 14400007 */  bnez       $v0, .L800145C4
    /* 151A8 800145A8 3C048004 */   lui       $a0, %hi(D_80038368)
    /* 151AC 800145AC 24848368 */  addiu      $a0, $a0, %lo(D_80038368)
    /* 151B0 800145B0 00002825 */  or         $a1, $zero, $zero
    /* 151B4 800145B4 0C001C9C */  jal        osRecvMesg
    /* 151B8 800145B8 24060001 */   addiu     $a2, $zero, 0x1
    /* 151BC 800145BC 3C028003 */  lui        $v0, %hi(D_8002C5DC)
    /* 151C0 800145C0 8C42C5DC */  lw         $v0, %lo(D_8002C5DC)($v0)
  .L800145C4:
    /* 151C4 800145C4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 151C8 800145C8 244E0001 */  addiu      $t6, $v0, 0x1
    /* 151CC 800145CC 3C018003 */  lui        $at, %hi(D_8002C5DC)
    /* 151D0 800145D0 AC2EC5DC */  sw         $t6, %lo(D_8002C5DC)($at)
    /* 151D4 800145D4 03E00008 */  jr         $ra
    /* 151D8 800145D8 27BD0018 */   addiu     $sp, $sp, 0x18
endlabel func_80014594
