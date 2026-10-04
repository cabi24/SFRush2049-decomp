nonmatching func_80016E68, 0x78

glabel func_80016E68
    /* 17A68 80016E68 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 17A6C 80016E6C AFA40020 */  sw         $a0, 0x20($sp)
    /* 17A70 80016E70 97AE0022 */  lhu        $t6, 0x22($sp)
    /* 17A74 80016E74 3C0F8001 */  lui        $t7, %hi(func_80016E40)
    /* 17A78 80016E78 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 17A7C 80016E7C 3C018004 */  lui        $at, %hi(D_80042674)
    /* 17A80 80016E80 25EF6E40 */  addiu      $t7, $t7, %lo(func_80016E40)
    /* 17A84 80016E84 3C048004 */  lui        $a0, %hi(D_80042670)
    /* 17A88 80016E88 3C058004 */  lui        $a1, %hi(D_80038610)
    /* 17A8C 80016E8C 3C068004 */  lui        $a2, %hi(D_80038608)
    /* 17A90 80016E90 8CC68608 */  lw         $a2, %lo(D_80038608)($a2)
    /* 17A94 80016E94 24A58610 */  addiu      $a1, $a1, %lo(D_80038610)
    /* 17A98 80016E98 24842670 */  addiu      $a0, $a0, %lo(D_80042670)
    /* 17A9C 80016E9C AFAF0010 */  sw         $t7, 0x10($sp)
    /* 17AA0 80016EA0 24070008 */  addiu      $a3, $zero, 0x8
    /* 17AA4 80016EA4 0C007A19 */  jal        func_8001E864
    /* 17AA8 80016EA8 A42E2674 */   sh        $t6, %lo(D_80042674)($at)
    /* 17AAC 80016EAC 3C018004 */  lui        $at, %hi(D_80042678)
    /* 17AB0 80016EB0 10400006 */  beqz       $v0, .L80016ECC
    /* 17AB4 80016EB4 AC222678 */   sw        $v0, %lo(D_80042678)($at)
    /* 17AB8 80016EB8 3C188004 */  lui        $t8, %hi(D_80042678)
    /* 17ABC 80016EBC 8F182678 */  lw         $t8, %lo(D_80042678)($t8)
    /* 17AC0 80016EC0 8B020000 */  lwl        $v0, 0x0($t8)
    /* 17AC4 80016EC4 10000002 */  b          .L80016ED0
    /* 17AC8 80016EC8 9B020003 */   lwr       $v0, 0x3($t8)
  .L80016ECC:
    /* 17ACC 80016ECC 00001025 */  or         $v0, $zero, $zero
  .L80016ED0:
    /* 17AD0 80016ED0 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 17AD4 80016ED4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 17AD8 80016ED8 03E00008 */  jr         $ra
    /* 17ADC 80016EDC 00000000 */   nop
endlabel func_80016E68
