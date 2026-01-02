# Source: 8700.s
# Address: 0x80007B00

glabel func_80007B00
    /* 8700 80007B00 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 8704 80007B04 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 8708 80007B08 AFA40018 */  sw         $a0, 0x18($sp)
    /* 870C 80007B0C AFA60020 */  sw         $a2, 0x20($sp)
    /* 8710 80007B10 0C0035D0 */  jal        func_8000D740
    /* 8714 80007B14 AFA70024 */   sw        $a3, 0x24($sp)
    /* 8718 80007B18 10400003 */  beqz       $v0, .L80007B28
    /* 871C 80007B1C 3C03A410 */   lui       $v1, %hi(DPC_STATUS_REG)
    /* 8720 80007B20 10000013 */  b          .L80007B70
    /* 8724 80007B24 2402FFFF */   addiu     $v0, $zero, -0x1
  .L80007B28:
    /* 8728 80007B28 3463000C */  ori        $v1, $v1, %lo(DPC_STATUS_REG)
    /* 872C 80007B2C 240E0001 */  addiu      $t6, $zero, 0x1
    /* 8730 80007B30 AC6E0000 */  sw         $t6, 0x0($v1)
    /* 8734 80007B34 8C620000 */  lw         $v0, 0x0($v1)
  .L80007B38:
    /* 8738 80007B38 304F0001 */  andi       $t7, $v0, 0x1
    /* 873C 80007B3C 55E0FFFE */  bnel       $t7, $zero, .L80007B38
    /* 8740 80007B40 8C620000 */   lw        $v0, 0x0($v1)
    /* 8744 80007B44 0C003570 */  jal        func_8000D5C0
    /* 8748 80007B48 8FA40018 */   lw        $a0, 0x18($sp)
    /* 874C 80007B4C 3C18A410 */  lui        $t8, %hi(DPC_START_REG)
    /* 8750 80007B50 AF020000 */  sw         $v0, %lo(DPC_START_REG)($t8)
    /* 8754 80007B54 0C003570 */  jal        func_8000D5C0
    /* 8758 80007B58 8FA40018 */   lw        $a0, 0x18($sp)
    /* 875C 80007B5C 8FAB0024 */  lw         $t3, 0x24($sp)
    /* 8760 80007B60 3C0EA410 */  lui        $t6, %hi(DPC_END_REG)
    /* 8764 80007B64 004BC821 */  addu       $t9, $v0, $t3
    /* 8768 80007B68 ADD90004 */  sw         $t9, %lo(DPC_END_REG)($t6)
    /* 876C 80007B6C 00001025 */  or         $v0, $zero, $zero
  .L80007B70:
    /* 8770 80007B70 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 8774 80007B74 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 8778 80007B78 03E00008 */  jr         $ra
    /* 877C 80007B7C 00000000 */   nop