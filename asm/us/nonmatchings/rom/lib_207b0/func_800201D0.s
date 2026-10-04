nonmatching func_800201D0, 0x30

glabel func_800201D0
    /* 20DD0 800201D0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 20DD4 800201D4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 20DD8 800201D8 0C007B7D */  jal        func_8001EDF4
    /* 20DDC 800201DC AFA40018 */   sw        $a0, 0x18($sp)
    /* 20DE0 800201E0 2401FFFF */  addiu      $at, $zero, -0x1
    /* 20DE4 800201E4 10410003 */  beq        $v0, $at, .L800201F4
    /* 20DE8 800201E8 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 20DEC 800201EC 10000002 */  b          .L800201F8
    /* 20DF0 800201F0 8FA20018 */   lw        $v0, 0x18($sp)
  .L800201F4:
    /* 20DF4 800201F4 2402FFFF */  addiu      $v0, $zero, -0x1
  .L800201F8:
    /* 20DF8 800201F8 03E00008 */  jr         $ra
    /* 20DFC 800201FC 27BD0018 */   addiu     $sp, $sp, 0x18
endlabel func_800201D0
