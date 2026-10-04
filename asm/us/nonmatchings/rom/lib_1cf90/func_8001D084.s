nonmatching func_8001D084, 0x6C

glabel func_8001D084
    /* 1DC84 8001D084 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1DC88 8001D088 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1DC8C 8001D08C 8C820000 */  lw         $v0, 0x0($a0)
    /* 1DC90 8001D090 3C018005 */  lui        $at, %hi(D_8004FD50)
    /* 1DC94 8001D094 50400005 */  beql       $v0, $zero, .L8001D0AC
    /* 1DC98 8001D098 8C830004 */   lw        $v1, 0x4($a0)
    /* 1DC9C 8001D09C 8C8E0004 */  lw         $t6, 0x4($a0)
    /* 1DCA0 8001D0A0 AC4E0004 */  sw         $t6, 0x4($v0)
    /* 1DCA4 8001D0A4 8C820000 */  lw         $v0, 0x0($a0)
    /* 1DCA8 8001D0A8 8C830004 */  lw         $v1, 0x4($a0)
  .L8001D0AC:
    /* 1DCAC 8001D0AC 10600003 */  beqz       $v1, .L8001D0BC
    /* 1DCB0 8001D0B0 00000000 */   nop
    /* 1DCB4 8001D0B4 10000002 */  b          .L8001D0C0
    /* 1DCB8 8001D0B8 AC620000 */   sw        $v0, 0x0($v1)
  .L8001D0BC:
    /* 1DCBC 8001D0BC AC22FD50 */  sw         $v0, %lo(D_8004FD50)($at)
  .L8001D0C0:
    /* 1DCC0 8001D0C0 8C8F0008 */  lw         $t7, 0x8($a0)
    /* 1DCC4 8001D0C4 8C850034 */  lw         $a1, 0x34($a0)
    /* 1DCC8 8001D0C8 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1DCCC 8001D0CC 31F8FFFF */  andi       $t8, $t7, 0xFFFF
    /* 1DCD0 8001D0D0 10A10003 */  beq        $a1, $at, .L8001D0E0
    /* 1DCD4 8001D0D4 AC980008 */   sw        $t8, 0x8($a0)
    /* 1DCD8 8001D0D8 0C006E31 */  jal        func_8001B8C4
    /* 1DCDC 8001D0DC 00A02025 */   or        $a0, $a1, $zero
  .L8001D0E0:
    /* 1DCE0 8001D0E0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1DCE4 8001D0E4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1DCE8 8001D0E8 03E00008 */  jr         $ra
    /* 1DCEC 8001D0EC 00000000 */   nop
endlabel func_8001D084
