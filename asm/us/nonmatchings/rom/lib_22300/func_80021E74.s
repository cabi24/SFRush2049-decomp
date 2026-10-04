nonmatching func_80021E74, 0x94

glabel func_80021E74
    /* 22A74 80021E74 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 22A78 80021E78 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 22A7C 80021E7C 00803025 */  or         $a2, $a0, $zero
    /* 22A80 80021E80 8CA40000 */  lw         $a0, 0x0($a1)
    /* 22A84 80021E84 AFA60018 */  sw         $a2, 0x18($sp)
    /* 22A88 80021E88 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 22A8C 80021E8C 00042402 */  srl        $a0, $a0, 16
    /* 22A90 80021E90 0C005B08 */  jal        func_80016C20
    /* 22A94 80021E94 3084FFFF */   andi      $a0, $a0, 0xFFFF
    /* 22A98 80021E98 8FA5001C */  lw         $a1, 0x1C($sp)
    /* 22A9C 80021E9C 8FA60018 */  lw         $a2, 0x18($sp)
    /* 22AA0 80021EA0 10400013 */  beqz       $v0, .L80021EF0
    /* 22AA4 80021EA4 00401825 */   or        $v1, $v0, $zero
    /* 22AA8 80021EA8 88CE0000 */  lwl        $t6, 0x0($a2)
    /* 22AAC 80021EAC 88CF0004 */  lwl        $t7, 0x4($a2)
    /* 22AB0 80021EB0 98CE0003 */  lwr        $t6, 0x3($a2)
    /* 22AB4 80021EB4 98CF0007 */  lwr        $t7, 0x7($a2)
    /* 22AB8 80021EB8 A8C30000 */  swl        $v1, 0x0($a2)
    /* 22ABC 80021EBC A8CE0008 */  swl        $t6, 0x8($a2)
    /* 22AC0 80021EC0 A8CF000C */  swl        $t7, 0xC($a2)
    /* 22AC4 80021EC4 B8C30003 */  swr        $v1, 0x3($a2)
    /* 22AC8 80021EC8 B8CE000B */  swr        $t6, 0xB($a2)
    /* 22ACC 80021ECC B8CF000F */  swr        $t7, 0xF($a2)
    /* 22AD0 80021ED0 8CB80004 */  lw         $t8, 0x4($a1)
    /* 22AD4 80021ED4 3319FFFF */  andi       $t9, $t8, 0xFFFF
    /* 22AD8 80021ED8 001940C0 */  sll        $t0, $t9, 3
    /* 22ADC 80021EDC 01024821 */  addu       $t1, $t0, $v0
    /* 22AE0 80021EE0 A8C90004 */  swl        $t1, 0x4($a2)
    /* 22AE4 80021EE4 B8C90007 */  swr        $t1, 0x7($a2)
    /* 22AE8 80021EE8 10000003 */  b          .L80021EF8
    /* 22AEC 80021EEC 00001025 */   or        $v0, $zero, $zero
  .L80021EF0:
    /* 22AF0 80021EF0 0C0086F0 */  jal        func_80021BC0
    /* 22AF4 80021EF4 00C02025 */   or        $a0, $a2, $zero
  .L80021EF8:
    /* 22AF8 80021EF8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 22AFC 80021EFC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 22B00 80021F00 03E00008 */  jr         $ra
    /* 22B04 80021F04 00000000 */   nop
endlabel func_80021E74
