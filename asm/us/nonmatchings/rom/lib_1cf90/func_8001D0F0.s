nonmatching func_8001D0F0, 0xD0

glabel func_8001D0F0
    /* 1DCF0 8001D0F0 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 1DCF4 8001D0F4 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 1DCF8 8001D0F8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1DCFC 8001D0FC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1DD00 8001D100 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 1DD04 8001D104 AFA60020 */  sw         $a2, 0x20($sp)
    /* 1DD08 8001D108 11C00028 */  beqz       $t6, .L8001D1AC
    /* 1DD0C 8001D10C AFA70024 */   sw        $a3, 0x24($sp)
    /* 1DD10 8001D110 0C005165 */  jal        func_80014594
    /* 1DD14 8001D114 AFA40018 */   sw        $a0, 0x18($sp)
    /* 1DD18 8001D118 8FAF001C */  lw         $t7, 0x1C($sp)
    /* 1DD1C 8001D11C 8FA40018 */  lw         $a0, 0x18($sp)
    /* 1DD20 8001D120 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* 1DD24 8001D124 8DF90000 */  lw         $t9, 0x0($t7)
    /* 1DD28 8001D128 AC99000C */  sw         $t9, 0xC($a0)
    /* 1DD2C 8001D12C 8DF80004 */  lw         $t8, 0x4($t7)
    /* 1DD30 8001D130 AC980010 */  sw         $t8, 0x10($a0)
    /* 1DD34 8001D134 8DF90008 */  lw         $t9, 0x8($t7)
    /* 1DD38 8001D138 AC990014 */  sw         $t9, 0x14($a0)
    /* 1DD3C 8001D13C 8FA80020 */  lw         $t0, 0x20($sp)
    /* 1DD40 8001D140 8D0A0000 */  lw         $t2, 0x0($t0)
    /* 1DD44 8001D144 AC8A0018 */  sw         $t2, 0x18($a0)
    /* 1DD48 8001D148 8D090004 */  lw         $t1, 0x4($t0)
    /* 1DD4C 8001D14C AC89001C */  sw         $t1, 0x1C($a0)
    /* 1DD50 8001D150 8D0A0008 */  lw         $t2, 0x8($t0)
    /* 1DD54 8001D154 AC8A0020 */  sw         $t2, 0x20($a0)
    /* 1DD58 8001D158 93AB0027 */  lbu        $t3, 0x27($sp)
    /* 1DD5C 8001D15C 448B2000 */  mtc1       $t3, $ft0
    /* 1DD60 8001D160 05610004 */  bgez       $t3, .L8001D174
    /* 1DD64 8001D164 468021A0 */   cvt.s.w   $ft1, $ft0
    /* 1DD68 8001D168 44814000 */  mtc1       $at, $ft2
    /* 1DD6C 8001D16C 00000000 */  nop
    /* 1DD70 8001D170 46083180 */  add.s      $ft1, $ft1, $ft2
  .L8001D174:
    /* 1DD74 8001D174 3C0142FE */  lui        $at, (0x42FE0000 >> 16)
    /* 1DD78 8001D178 44815000 */  mtc1       $at, $ft3
    /* 1DD7C 8001D17C C490002C */  lwc1       $ft4, 0x2C($a0)
    /* 1DD80 8001D180 460A3003 */  div.s      $fv0, $ft1, $ft3
    /* 1DD84 8001D184 4610003C */  c.lt.s     $fv0, $ft4
    /* 1DD88 8001D188 E4800028 */  swc1       $fv0, 0x28($a0)
    /* 1DD8C 8001D18C 45000003 */  bc1f       .L8001D19C
    /* 1DD90 8001D190 00000000 */   nop
    /* 1DD94 8001D194 C4920028 */  lwc1       $ft5, 0x28($a0)
    /* 1DD98 8001D198 E492002C */  swc1       $ft5, 0x2C($a0)
  .L8001D19C:
    /* 1DD9C 8001D19C 0C005177 */  jal        func_800145DC
    /* 1DDA0 8001D1A0 00000000 */   nop
    /* 1DDA4 8001D1A4 10000002 */  b          .L8001D1B0
    /* 1DDA8 8001D1A8 24020001 */   addiu     $v0, $zero, 0x1
  .L8001D1AC:
    /* 1DDAC 8001D1AC 00001025 */  or         $v0, $zero, $zero
  .L8001D1B0:
    /* 1DDB0 8001D1B0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1DDB4 8001D1B4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1DDB8 8001D1B8 03E00008 */  jr         $ra
    /* 1DDBC 8001D1BC 00000000 */   nop
endlabel func_8001D0F0
