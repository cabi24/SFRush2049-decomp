nonmatching func_80014CAC, 0x48

glabel func_80014CAC
    /* 158AC 80014CAC 3C01FF00 */  lui        $at, (0xFF000000 >> 16)
    /* 158B0 80014CB0 00811024 */  and        $v0, $a0, $at
    /* 158B4 80014CB4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 158B8 80014CB8 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* 158BC 80014CBC 10410008 */  beq        $v0, $at, .L80014CE0
    /* 158C0 80014CC0 AFBF0014 */   sw        $ra, 0x14($sp)
    /* 158C4 80014CC4 3C01B000 */  lui        $at, (0xB0000000 >> 16)
    /* 158C8 80014CC8 10410005 */  beq        $v0, $at, .L80014CE0
    /* 158CC 80014CCC 3C198004 */   lui       $t9, %hi(D_80038014)
    /* 158D0 80014CD0 8F398014 */  lw         $t9, %lo(D_80038014)($t9)
    /* 158D4 80014CD4 0320F809 */  jalr       $t9
    /* 158D8 80014CD8 00000000 */   nop
    /* 158DC 80014CDC 00402025 */  or         $a0, $v0, $zero
  .L80014CE0:
    /* 158E0 80014CE0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 158E4 80014CE4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 158E8 80014CE8 00801025 */  or         $v0, $a0, $zero
    /* 158EC 80014CEC 03E00008 */  jr         $ra
    /* 158F0 80014CF0 00000000 */   nop
endlabel func_80014CAC
