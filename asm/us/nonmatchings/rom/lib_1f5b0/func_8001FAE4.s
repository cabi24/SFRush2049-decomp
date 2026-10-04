nonmatching func_8001FAE4, 0xC8

glabel func_8001FAE4
    /* 206E4 8001FAE4 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 206E8 8001FAE8 3C028005 */  lui        $v0, %hi(D_8004FA18)
    /* 206EC 8001FAEC 9042FA18 */  lbu        $v0, %lo(D_8004FA18)($v0)
    /* 206F0 8001FAF0 AFB2001C */  sw         $s2, 0x1C($sp)
    /* 206F4 8001FAF4 AFB10018 */  sw         $s1, 0x18($sp)
    /* 206F8 8001FAF8 309200FF */  andi       $s2, $a0, 0xFF
    /* 206FC 8001FAFC AFBF0024 */  sw         $ra, 0x24($sp)
    /* 20700 8001FB00 AFB30020 */  sw         $s3, 0x20($sp)
    /* 20704 8001FB04 AFB00014 */  sw         $s0, 0x14($sp)
    /* 20708 8001FB08 AFA40028 */  sw         $a0, 0x28($sp)
    /* 2070C 8001FB0C 18400020 */  blez       $v0, .L8001FB90
    /* 20710 8001FB10 00008825 */   or        $s1, $zero, $zero
    /* 20714 8001FB14 3C108005 */  lui        $s0, %hi(D_8004BEB8)
    /* 20718 8001FB18 2610BEB8 */  addiu      $s0, $s0, %lo(D_8004BEB8)
    /* 2071C 8001FB1C 2413FFFC */  addiu      $s3, $zero, -0x4
  .L8001FB20:
    /* 20720 8001FB20 8A0E0000 */  lwl        $t6, 0x0($s0)
    /* 20724 8001FB24 9A0E0003 */  lwr        $t6, 0x3($s0)
    /* 20728 8001FB28 11C00011 */  beqz       $t6, .L8001FB70
    /* 2072C 8001FB2C 00000000 */   nop
    /* 20730 8001FB30 12400006 */  beqz       $s2, .L8001FB4C
    /* 20734 8001FB34 00000000 */   nop
    /* 20738 8001FB38 52400012 */  beql       $s2, $zero, .L8001FB84
    /* 2073C 8001FB3C 26310001 */   addiu     $s1, $s1, 0x1
    /* 20740 8001FB40 920F004C */  lbu        $t7, 0x4C($s0)
    /* 20744 8001FB44 55E0000F */  bnel       $t7, $zero, .L8001FB84
    /* 20748 8001FB48 26310001 */   addiu     $s1, $s1, 0x1
  .L8001FB4C:
    /* 2074C 8001FB4C 0C007E74 */  jal        func_8001F9D0
    /* 20750 8001FB50 02002025 */   or        $a0, $s0, $zero
    /* 20754 8001FB54 8A180024 */  lwl        $t8, 0x24($s0)
    /* 20758 8001FB58 9A180027 */  lwr        $t8, 0x27($s0)
    /* 2075C 8001FB5C AA000000 */  swl        $zero, 0x0($s0)
    /* 20760 8001FB60 BA000003 */  swr        $zero, 0x3($s0)
    /* 20764 8001FB64 0313C824 */  and        $t9, $t8, $s3
    /* 20768 8001FB68 AA190024 */  swl        $t9, 0x24($s0)
    /* 2076C 8001FB6C BA190027 */  swr        $t9, 0x27($s0)
  .L8001FB70:
    /* 20770 8001FB70 0C0052CF */  jal        func_80014B3C
    /* 20774 8001FB74 02202025 */   or        $a0, $s1, $zero
    /* 20778 8001FB78 3C028005 */  lui        $v0, %hi(D_8004FA18)
    /* 2077C 8001FB7C 9042FA18 */  lbu        $v0, %lo(D_8004FA18)($v0)
    /* 20780 8001FB80 26310001 */  addiu      $s1, $s1, 0x1
  .L8001FB84:
    /* 20784 8001FB84 0222082A */  slt        $at, $s1, $v0
    /* 20788 8001FB88 1420FFE5 */  bnez       $at, .L8001FB20
    /* 2078C 8001FB8C 261001A0 */   addiu     $s0, $s0, 0x1A0
  .L8001FB90:
    /* 20790 8001FB90 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 20794 8001FB94 8FB00014 */  lw         $s0, 0x14($sp)
    /* 20798 8001FB98 8FB10018 */  lw         $s1, 0x18($sp)
    /* 2079C 8001FB9C 8FB2001C */  lw         $s2, 0x1C($sp)
    /* 207A0 8001FBA0 8FB30020 */  lw         $s3, 0x20($sp)
    /* 207A4 8001FBA4 03E00008 */  jr         $ra
    /* 207A8 8001FBA8 27BD0028 */   addiu     $sp, $sp, 0x28
endlabel func_8001FAE4
    /* 207AC 8001FBAC 00000000 */  nop
