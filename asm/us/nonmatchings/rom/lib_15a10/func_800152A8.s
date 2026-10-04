nonmatching func_800152A8, 0x70

glabel func_800152A8
    /* 15EA8 800152A8 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 15EAC 800152AC AFBF001C */  sw         $ra, 0x1C($sp)
    /* 15EB0 800152B0 AFB10018 */  sw         $s1, 0x18($sp)
    /* 15EB4 800152B4 AFB00014 */  sw         $s0, 0x14($sp)
    /* 15EB8 800152B8 948E0000 */  lhu        $t6, 0x0($a0)
    /* 15EBC 800152BC 3402FFFF */  ori        $v0, $zero, 0xFFFF
    /* 15EC0 800152C0 00808825 */  or         $s1, $a0, $zero
    /* 15EC4 800152C4 104E0005 */  beq        $v0, $t6, .L800152DC
    /* 15EC8 800152C8 00808025 */   or        $s0, $a0, $zero
    /* 15ECC 800152CC 960F0002 */  lhu        $t7, 0x2($s0)
  .L800152D0:
    /* 15ED0 800152D0 26100002 */  addiu      $s0, $s0, 0x2
    /* 15ED4 800152D4 544FFFFE */  bnel       $v0, $t7, .L800152D0
    /* 15ED8 800152D8 960F0002 */   lhu       $t7, 0x2($s0)
  .L800152DC:
    /* 15EDC 800152DC 2610FFFE */  addiu      $s0, $s0, -0x2
    /* 15EE0 800152E0 0211082B */  sltu       $at, $s0, $s1
    /* 15EE4 800152E4 54200008 */  bnel       $at, $zero, .L80015308
    /* 15EE8 800152E8 8FBF001C */   lw        $ra, 0x1C($sp)
  .L800152EC:
    /* 15EEC 800152EC 0C0058EA */  jal        func_800163A8
    /* 15EF0 800152F0 96040000 */   lhu       $a0, 0x0($s0)
    /* 15EF4 800152F4 2610FFFE */  addiu      $s0, $s0, -0x2
    /* 15EF8 800152F8 0211082B */  sltu       $at, $s0, $s1
    /* 15EFC 800152FC 1020FFFB */  beqz       $at, .L800152EC
    /* 15F00 80015300 00000000 */   nop
    /* 15F04 80015304 8FBF001C */  lw         $ra, 0x1C($sp)
  .L80015308:
    /* 15F08 80015308 8FB00014 */  lw         $s0, 0x14($sp)
    /* 15F0C 8001530C 8FB10018 */  lw         $s1, 0x18($sp)
    /* 15F10 80015310 03E00008 */  jr         $ra
    /* 15F14 80015314 27BD0020 */   addiu     $sp, $sp, 0x20
endlabel func_800152A8
