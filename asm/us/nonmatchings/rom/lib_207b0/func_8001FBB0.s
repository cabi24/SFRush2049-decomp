nonmatching func_8001FBB0, 0x17C

glabel func_8001FBB0
    /* 207B0 8001FBB0 27BDFFC0 */  addiu      $sp, $sp, -0x40
    /* 207B4 8001FBB4 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 207B8 8001FBB8 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 207BC 8001FBBC AFBE0038 */  sw         $fp, 0x38($sp)
    /* 207C0 8001FBC0 AFB60030 */  sw         $s6, 0x30($sp)
    /* 207C4 8001FBC4 AFB00018 */  sw         $s0, 0x18($sp)
    /* 207C8 8001FBC8 00808025 */  or         $s0, $a0, $zero
    /* 207CC 8001FBCC 30B6FFFF */  andi       $s6, $a1, 0xFFFF
    /* 207D0 8001FBD0 AFBF003C */  sw         $ra, 0x3C($sp)
    /* 207D4 8001FBD4 AFB70034 */  sw         $s7, 0x34($sp)
    /* 207D8 8001FBD8 AFB5002C */  sw         $s5, 0x2C($sp)
    /* 207DC 8001FBDC AFB40028 */  sw         $s4, 0x28($sp)
    /* 207E0 8001FBE0 AFB30024 */  sw         $s3, 0x24($sp)
    /* 207E4 8001FBE4 AFB20020 */  sw         $s2, 0x20($sp)
    /* 207E8 8001FBE8 AFB1001C */  sw         $s1, 0x1C($sp)
    /* 207EC 8001FBEC AFA50044 */  sw         $a1, 0x44($sp)
    /* 207F0 8001FBF0 11C00041 */  beqz       $t6, .L8001FCF8
    /* 207F4 8001FBF4 241EFFFF */   addiu     $fp, $zero, -0x1
    /* 207F8 8001FBF8 0C005165 */  jal        func_80014594
    /* 207FC 8001FBFC 00000000 */   nop
    /* 20800 8001FC00 0C007B7D */  jal        func_8001EDF4
    /* 20804 8001FC04 02002025 */   or        $a0, $s0, $zero
    /* 20808 8001FC08 2417FFFF */  addiu      $s7, $zero, -0x1
    /* 2080C 8001FC0C 10570038 */  beq        $v0, $s7, .L8001FCF0
    /* 20810 8001FC10 00408025 */   or        $s0, $v0, $zero
    /* 20814 8001FC14 3C148005 */  lui        $s4, %hi(D_8004BEB8)
    /* 20818 8001FC18 2694BEB8 */  addiu      $s4, $s4, %lo(D_8004BEB8)
    /* 2081C 8001FC1C 241501A0 */  addiu      $s5, $zero, 0x1A0
    /* 20820 8001FC20 320200FF */  andi       $v0, $s0, 0xFF
  .L8001FC24:
    /* 20824 8001FC24 00550019 */  multu      $v0, $s5
    /* 20828 8001FC28 001639C3 */  sra        $a3, $s6, 7
    /* 2082C 8001FC2C 00409825 */  or         $s3, $v0, $zero
    /* 20830 8001FC30 30E700FF */  andi       $a3, $a3, 0xFF
    /* 20834 8001FC34 24040001 */  addiu      $a0, $zero, 0x1
    /* 20838 8001FC38 00007812 */  mflo       $t7
    /* 2083C 8001FC3C 028F8821 */  addu       $s1, $s4, $t7
    /* 20840 8001FC40 8A380060 */  lwl        $t8, 0x60($s1)
    /* 20844 8001FC44 9A380063 */  lwr        $t8, 0x63($s1)
    /* 20848 8001FC48 16180023 */  bne        $s0, $t8, .L8001FCD8
    /* 2084C 8001FC4C 00000000 */   nop
    /* 20850 8001FC50 8A390024 */  lwl        $t9, 0x24($s1)
    /* 20854 8001FC54 9A390027 */  lwr        $t9, 0x27($s1)
    /* 20858 8001FC58 02C09025 */  or         $s2, $s6, $zero
    /* 2085C 8001FC5C 3252007F */  andi       $s2, $s2, 0x7F
    /* 20860 8001FC60 33280002 */  andi       $t0, $t9, 0x2
    /* 20864 8001FC64 1100000D */  beqz       $t0, .L8001FC9C
    /* 20868 8001FC68 0000F025 */   or        $fp, $zero, $zero
    /* 2086C 8001FC6C 305000FF */  andi       $s0, $v0, 0xFF
    /* 20870 8001FC70 320500FF */  andi       $a1, $s0, 0xFF
    /* 20874 8001FC74 24040001 */  addiu      $a0, $zero, 0x1
    /* 20878 8001FC78 0C008184 */  jal        func_80020610
    /* 2087C 8001FC7C 92260055 */   lbu       $a2, 0x55($s1)
    /* 20880 8001FC80 24040021 */  addiu      $a0, $zero, 0x21
    /* 20884 8001FC84 320500FF */  andi       $a1, $s0, 0xFF
    /* 20888 8001FC88 92260055 */  lbu        $a2, 0x55($s1)
    /* 2088C 8001FC8C 0C008184 */  jal        func_80020610
    /* 20890 8001FC90 324700FF */   andi      $a3, $s2, 0xFF
    /* 20894 8001FC94 1000000A */  b          .L8001FCC0
    /* 20898 8001FC98 00000000 */   nop
  .L8001FC9C:
    /* 2089C 8001FC9C 305000FF */  andi       $s0, $v0, 0xFF
    /* 208A0 8001FCA0 320500FF */  andi       $a1, $s0, 0xFF
    /* 208A4 8001FCA4 0C008184 */  jal        func_80020610
    /* 208A8 8001FCA8 9226004B */   lbu       $a2, 0x4B($s1)
    /* 208AC 8001FCAC 24040021 */  addiu      $a0, $zero, 0x21
    /* 208B0 8001FCB0 320500FF */  andi       $a1, $s0, 0xFF
    /* 208B4 8001FCB4 9226004B */  lbu        $a2, 0x4B($s1)
    /* 208B8 8001FCB8 0C008184 */  jal        func_80020610
    /* 208BC 8001FCBC 324700FF */   andi      $a3, $s2, 0xFF
  .L8001FCC0:
    /* 208C0 8001FCC0 02750019 */  multu      $s3, $s5
    /* 208C4 8001FCC4 00004812 */  mflo       $t1
    /* 208C8 8001FCC8 02895021 */  addu       $t2, $s4, $t1
    /* 208CC 8001FCCC 89500010 */  lwl        $s0, 0x10($t2)
    /* 208D0 8001FCD0 10000005 */  b          .L8001FCE8
    /* 208D4 8001FCD4 99500013 */   lwr       $s0, 0x13($t2)
  .L8001FCD8:
    /* 208D8 8001FCD8 0C005177 */  jal        func_800145DC
    /* 208DC 8001FCDC 00000000 */   nop
    /* 208E0 8001FCE0 10000006 */  b          .L8001FCFC
    /* 208E4 8001FCE4 03C01025 */   or        $v0, $fp, $zero
  .L8001FCE8:
    /* 208E8 8001FCE8 5617FFCE */  bnel       $s0, $s7, .L8001FC24
    /* 208EC 8001FCEC 320200FF */   andi      $v0, $s0, 0xFF
  .L8001FCF0:
    /* 208F0 8001FCF0 0C005177 */  jal        func_800145DC
    /* 208F4 8001FCF4 00000000 */   nop
  .L8001FCF8:
    /* 208F8 8001FCF8 03C01025 */  or         $v0, $fp, $zero
  .L8001FCFC:
    /* 208FC 8001FCFC 8FBF003C */  lw         $ra, 0x3C($sp)
    /* 20900 8001FD00 8FB00018 */  lw         $s0, 0x18($sp)
    /* 20904 8001FD04 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 20908 8001FD08 8FB20020 */  lw         $s2, 0x20($sp)
    /* 2090C 8001FD0C 8FB30024 */  lw         $s3, 0x24($sp)
    /* 20910 8001FD10 8FB40028 */  lw         $s4, 0x28($sp)
    /* 20914 8001FD14 8FB5002C */  lw         $s5, 0x2C($sp)
    /* 20918 8001FD18 8FB60030 */  lw         $s6, 0x30($sp)
    /* 2091C 8001FD1C 8FB70034 */  lw         $s7, 0x34($sp)
    /* 20920 8001FD20 8FBE0038 */  lw         $fp, 0x38($sp)
    /* 20924 8001FD24 03E00008 */  jr         $ra
    /* 20928 8001FD28 27BD0040 */   addiu     $sp, $sp, 0x40
endlabel func_8001FBB0
