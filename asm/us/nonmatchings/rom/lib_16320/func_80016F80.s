nonmatching func_80016F80, 0x98

glabel func_80016F80
    /* 17B80 80016F80 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 17B84 80016F84 AFA40020 */  sw         $a0, 0x20($sp)
    /* 17B88 80016F88 97AE0022 */  lhu        $t6, 0x22($sp)
    /* 17B8C 80016F8C AFA50024 */  sw         $a1, 0x24($sp)
    /* 17B90 80016F90 3C0F8001 */  lui        $t7, %hi(func_80016F58)
    /* 17B94 80016F94 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 17B98 80016F98 3C018004 */  lui        $at, %hi(D_80042694)
    /* 17B9C 80016F9C 25EF6F58 */  addiu      $t7, $t7, %lo(func_80016F58)
    /* 17BA0 80016FA0 3C058004 */  lui        $a1, %hi(D_8003CE20)
    /* 17BA4 80016FA4 3C048004 */  lui        $a0, %hi(D_80042690)
    /* 17BA8 80016FA8 3C068004 */  lui        $a2, %hi(D_8003CE18)
    /* 17BAC 80016FAC 8CC6CE18 */  lw         $a2, %lo(D_8003CE18)($a2)
    /* 17BB0 80016FB0 24842690 */  addiu      $a0, $a0, %lo(D_80042690)
    /* 17BB4 80016FB4 24A5CE20 */  addiu      $a1, $a1, %lo(D_8003CE20)
    /* 17BB8 80016FB8 AFAF0010 */  sw         $t7, 0x10($sp)
    /* 17BBC 80016FBC 2407000C */  addiu      $a3, $zero, 0xC
    /* 17BC0 80016FC0 0C007A19 */  jal        func_8001E864
    /* 17BC4 80016FC4 A42E2694 */   sh        $t6, %lo(D_80042694)($at)
    /* 17BC8 80016FC8 1040000C */  beqz       $v0, .L80016FFC
    /* 17BCC 80016FCC 00401825 */   or        $v1, $v0, $zero
    /* 17BD0 80016FD0 90410006 */  lbu        $at, 0x6($v0)
    /* 17BD4 80016FD4 90580007 */  lbu        $t8, 0x7($v0)
    /* 17BD8 80016FD8 8FB90024 */  lw         $t9, 0x24($sp)
    /* 17BDC 80016FDC 00010A00 */  sll        $at, $at, 8
    /* 17BE0 80016FE0 0301C025 */  or         $t8, $t8, $at
    /* 17BE4 80016FE4 A7380000 */  sh         $t8, 0x0($t9)
    /* 17BE8 80016FE8 88420000 */  lwl        $v0, 0x0($v0)
    /* 17BEC 80016FEC 3C018004 */  lui        $at, %hi(D_8004269C)
    /* 17BF0 80016FF0 98620003 */  lwr        $v0, 0x3($v1)
    /* 17BF4 80016FF4 10000004 */  b          .L80017008
    /* 17BF8 80016FF8 AC23269C */   sw        $v1, %lo(D_8004269C)($at)
  .L80016FFC:
    /* 17BFC 80016FFC 3C018004 */  lui        $at, %hi(D_8004269C)
    /* 17C00 80017000 AC23269C */  sw         $v1, %lo(D_8004269C)($at)
    /* 17C04 80017004 00001025 */  or         $v0, $zero, $zero
  .L80017008:
    /* 17C08 80017008 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 17C0C 8001700C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 17C10 80017010 03E00008 */  jr         $ra
    /* 17C14 80017014 00000000 */   nop
endlabel func_80016F80
