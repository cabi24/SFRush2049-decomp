nonmatching func_8001EB10, 0x1D0

glabel func_8001EB10
    /* 1F710 8001EB10 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1F714 8001EB14 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1F718 8001EB18 0C008611 */  jal        func_80021844
    /* 1F71C 8001EB1C AFA40018 */   sw        $a0, 0x18($sp)
    /* 1F720 8001EB20 8FA40018 */  lw         $a0, 0x18($sp)
    /* 1F724 8001EB24 2403FFFF */  addiu      $v1, $zero, -0x1
    /* 1F728 8001EB28 888E0060 */  lwl        $t6, 0x60($a0)
    /* 1F72C 8001EB2C 988E0063 */  lwr        $t6, 0x63($a0)
    /* 1F730 8001EB30 506E0068 */  beql       $v1, $t6, .L8001ECD4
    /* 1F734 8001EB34 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 1F738 8001EB38 88850014 */  lwl        $a1, 0x14($a0)
    /* 1F73C 8001EB3C 98850017 */  lwr        $a1, 0x17($a0)
    /* 1F740 8001EB40 240701A0 */  addiu      $a3, $zero, 0x1A0
    /* 1F744 8001EB44 10650016 */  beq        $v1, $a1, .L8001EBA0
    /* 1F748 8001EB48 30B800FF */   andi      $t8, $a1, 0xFF
    /* 1F74C 8001EB4C 03070019 */  multu      $t8, $a3
    /* 1F750 8001EB50 888F0010 */  lwl        $t7, 0x10($a0)
    /* 1F754 8001EB54 988F0013 */  lwr        $t7, 0x13($a0)
    /* 1F758 8001EB58 3C068005 */  lui        $a2, %hi(D_8004BEB8)
    /* 1F75C 8001EB5C 24C6BEB8 */  addiu      $a2, $a2, %lo(D_8004BEB8)
    /* 1F760 8001EB60 0000C812 */  mflo       $t9
    /* 1F764 8001EB64 00D94021 */  addu       $t0, $a2, $t9
    /* 1F768 8001EB68 A90F0010 */  swl        $t7, 0x10($t0)
    /* 1F76C 8001EB6C B90F0013 */  swr        $t7, 0x13($t0)
    /* 1F770 8001EB70 88820010 */  lwl        $v0, 0x10($a0)
    /* 1F774 8001EB74 98820013 */  lwr        $v0, 0x13($a0)
    /* 1F778 8001EB78 10620055 */  beq        $v1, $v0, .L8001ECD0
    /* 1F77C 8001EB7C 304A00FF */   andi      $t2, $v0, 0xFF
    /* 1F780 8001EB80 01470019 */  multu      $t2, $a3
    /* 1F784 8001EB84 88890014 */  lwl        $t1, 0x14($a0)
    /* 1F788 8001EB88 98890017 */  lwr        $t1, 0x17($a0)
    /* 1F78C 8001EB8C 00005812 */  mflo       $t3
    /* 1F790 8001EB90 00CB6021 */  addu       $t4, $a2, $t3
    /* 1F794 8001EB94 A9890014 */  swl        $t1, 0x14($t4)
    /* 1F798 8001EB98 1000004D */  b          .L8001ECD0
    /* 1F79C 8001EB9C B9890017 */   swr       $t1, 0x17($t4)
  .L8001EBA0:
    /* 1F7A0 8001EBA0 88820010 */  lwl        $v0, 0x10($a0)
    /* 1F7A4 8001EBA4 98820013 */  lwr        $v0, 0x13($a0)
    /* 1F7A8 8001EBA8 5062001C */  beql       $v1, $v0, .L8001EC1C
    /* 1F7AC 8001EBAC 88820018 */   lwl       $v0, 0x18($a0)
    /* 1F7B0 8001EBB0 888D0018 */  lwl        $t5, 0x18($a0)
    /* 1F7B4 8001EBB4 988D001B */  lwr        $t5, 0x1B($a0)
    /* 1F7B8 8001EBB8 240701A0 */  addiu      $a3, $zero, 0x1A0
    /* 1F7BC 8001EBBC 3C068005 */  lui        $a2, %hi(D_8004BEB8)
    /* 1F7C0 8001EBC0 A9A2000C */  swl        $v0, 0xC($t5)
    /* 1F7C4 8001EBC4 B9A2000F */  swr        $v0, 0xF($t5)
    /* 1F7C8 8001EBC8 888E0010 */  lwl        $t6, 0x10($a0)
    /* 1F7CC 8001EBCC 988E0013 */  lwr        $t6, 0x13($a0)
    /* 1F7D0 8001EBD0 24C6BEB8 */  addiu      $a2, $a2, %lo(D_8004BEB8)
    /* 1F7D4 8001EBD4 31D800FF */  andi       $t8, $t6, 0xFF
    /* 1F7D8 8001EBD8 03070019 */  multu      $t8, $a3
    /* 1F7DC 8001EBDC 0000C812 */  mflo       $t9
    /* 1F7E0 8001EBE0 00D97821 */  addu       $t7, $a2, $t9
    /* 1F7E4 8001EBE4 A9E30014 */  swl        $v1, 0x14($t7)
    /* 1F7E8 8001EBE8 B9E30017 */  swr        $v1, 0x17($t7)
    /* 1F7EC 8001EBEC 888A0010 */  lwl        $t2, 0x10($a0)
    /* 1F7F0 8001EBF0 988A0013 */  lwr        $t2, 0x13($a0)
    /* 1F7F4 8001EBF4 88880018 */  lwl        $t0, 0x18($a0)
    /* 1F7F8 8001EBF8 9888001B */  lwr        $t0, 0x1B($a0)
    /* 1F7FC 8001EBFC 314B00FF */  andi       $t3, $t2, 0xFF
    /* 1F800 8001EC00 01670019 */  multu      $t3, $a3
    /* 1F804 8001EC04 00004812 */  mflo       $t1
    /* 1F808 8001EC08 00C96021 */  addu       $t4, $a2, $t1
    /* 1F80C 8001EC0C A9880018 */  swl        $t0, 0x18($t4)
    /* 1F810 8001EC10 1000002F */  b          .L8001ECD0
    /* 1F814 8001EC14 B988001B */   swr       $t0, 0x1B($t4)
    /* 1F818 8001EC18 88820018 */  lwl        $v0, 0x18($a0)
  .L8001EC1C:
    /* 1F81C 8001EC1C 9882001B */  lwr        $v0, 0x1B($a0)
    /* 1F820 8001EC20 3C058005 */  lui        $a1, %hi(D_80050C54)
    /* 1F824 8001EC24 24A50C54 */  addiu      $a1, $a1, %lo(D_80050C54)
    /* 1F828 8001EC28 88430004 */  lwl        $v1, 0x4($v0)
    /* 1F82C 8001EC2C 98430007 */  lwr        $v1, 0x7($v0)
    /* 1F830 8001EC30 50600007 */  beql       $v1, $zero, .L8001EC50
    /* 1F834 8001EC34 884E0000 */   lwl       $t6, 0x0($v0)
    /* 1F838 8001EC38 884D0000 */  lwl        $t5, 0x0($v0)
    /* 1F83C 8001EC3C 984D0003 */  lwr        $t5, 0x3($v0)
    /* 1F840 8001EC40 A86D0000 */  swl        $t5, 0x0($v1)
    /* 1F844 8001EC44 10000005 */  b          .L8001EC5C
    /* 1F848 8001EC48 B86D0003 */   swr       $t5, 0x3($v1)
    /* 1F84C 8001EC4C 884E0000 */  lwl        $t6, 0x0($v0)
  .L8001EC50:
    /* 1F850 8001EC50 984E0003 */  lwr        $t6, 0x3($v0)
    /* 1F854 8001EC54 3C018005 */  lui        $at, %hi(D_80050C50)
    /* 1F858 8001EC58 AC2E0C50 */  sw         $t6, %lo(D_80050C50)($at)
  .L8001EC5C:
    /* 1F85C 8001EC5C 88820018 */  lwl        $v0, 0x18($a0)
    /* 1F860 8001EC60 9882001B */  lwr        $v0, 0x1B($a0)
    /* 1F864 8001EC64 88430000 */  lwl        $v1, 0x0($v0)
    /* 1F868 8001EC68 98430003 */  lwr        $v1, 0x3($v0)
    /* 1F86C 8001EC6C 50600008 */  beql       $v1, $zero, .L8001EC90
    /* 1F870 8001EC70 8CB90000 */   lw        $t9, 0x0($a1)
    /* 1F874 8001EC74 88580004 */  lwl        $t8, 0x4($v0)
    /* 1F878 8001EC78 98580007 */  lwr        $t8, 0x7($v0)
    /* 1F87C 8001EC7C A8780004 */  swl        $t8, 0x4($v1)
    /* 1F880 8001EC80 B8780007 */  swr        $t8, 0x7($v1)
    /* 1F884 8001EC84 88820018 */  lwl        $v0, 0x18($a0)
    /* 1F888 8001EC88 9882001B */  lwr        $v0, 0x1B($a0)
    /* 1F88C 8001EC8C 8CB90000 */  lw         $t9, 0x0($a1)
  .L8001EC90:
    /* 1F890 8001EC90 A8590000 */  swl        $t9, 0x0($v0)
    /* 1F894 8001EC94 B8590003 */  swr        $t9, 0x3($v0)
    /* 1F898 8001EC98 8CA30000 */  lw         $v1, 0x0($a1)
    /* 1F89C 8001EC9C 50600006 */  beql       $v1, $zero, .L8001ECB8
    /* 1F8A0 8001ECA0 888A0018 */   lwl       $t2, 0x18($a0)
    /* 1F8A4 8001ECA4 888F0018 */  lwl        $t7, 0x18($a0)
    /* 1F8A8 8001ECA8 988F001B */  lwr        $t7, 0x1B($a0)
    /* 1F8AC 8001ECAC A86F0004 */  swl        $t7, 0x4($v1)
    /* 1F8B0 8001ECB0 B86F0007 */  swr        $t7, 0x7($v1)
    /* 1F8B4 8001ECB4 888A0018 */  lwl        $t2, 0x18($a0)
  .L8001ECB8:
    /* 1F8B8 8001ECB8 988A001B */  lwr        $t2, 0x1B($a0)
    /* 1F8BC 8001ECBC A9400004 */  swl        $zero, 0x4($t2)
    /* 1F8C0 8001ECC0 B9400007 */  swr        $zero, 0x7($t2)
    /* 1F8C4 8001ECC4 888B0018 */  lwl        $t3, 0x18($a0)
    /* 1F8C8 8001ECC8 988B001B */  lwr        $t3, 0x1B($a0)
    /* 1F8CC 8001ECCC ACAB0000 */  sw         $t3, 0x0($a1)
  .L8001ECD0:
    /* 1F8D0 8001ECD0 8FBF0014 */  lw         $ra, 0x14($sp)
  .L8001ECD4:
    /* 1F8D4 8001ECD4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1F8D8 8001ECD8 03E00008 */  jr         $ra
    /* 1F8DC 8001ECDC 00000000 */   nop
endlabel func_8001EB10
