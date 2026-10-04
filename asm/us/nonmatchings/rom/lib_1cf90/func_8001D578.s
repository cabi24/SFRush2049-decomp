nonmatching func_8001D578, 0x48

glabel func_8001D578
    /* 1E178 8001D578 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 1E17C 8001D57C AFB00014 */  sw         $s0, 0x14($sp)
    /* 1E180 8001D580 3C108005 */  lui        $s0, %hi(D_8004FD50)
    /* 1E184 8001D584 8E10FD50 */  lw         $s0, %lo(D_8004FD50)($s0)
    /* 1E188 8001D588 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 1E18C 8001D58C AFB10018 */  sw         $s1, 0x18($sp)
    /* 1E190 8001D590 52000007 */  beql       $s0, $zero, .L8001D5B0
    /* 1E194 8001D594 8FBF001C */   lw        $ra, 0x1C($sp)
  .L8001D598:
    /* 1E198 8001D598 8E110000 */  lw         $s1, 0x0($s0)
    /* 1E19C 8001D59C 0C007533 */  jal        func_8001D4CC
    /* 1E1A0 8001D5A0 02002025 */   or        $a0, $s0, $zero
    /* 1E1A4 8001D5A4 1620FFFC */  bnez       $s1, .L8001D598
    /* 1E1A8 8001D5A8 02208025 */   or        $s0, $s1, $zero
    /* 1E1AC 8001D5AC 8FBF001C */  lw         $ra, 0x1C($sp)
  .L8001D5B0:
    /* 1E1B0 8001D5B0 8FB00014 */  lw         $s0, 0x14($sp)
    /* 1E1B4 8001D5B4 8FB10018 */  lw         $s1, 0x18($sp)
    /* 1E1B8 8001D5B8 03E00008 */  jr         $ra
    /* 1E1BC 8001D5BC 27BD0020 */   addiu     $sp, $sp, 0x20
endlabel func_8001D578
