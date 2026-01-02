# Source: E4F0.s
# Address: 0x8000DAE8

glabel func_8000DAE8
    /* E6E8 8000DAE8 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* E6EC 8000DAEC AFA40008 */  sw         $a0, 0x8($sp)
    /* E6F0 8000DAF0 AFA5000C */  sw         $a1, 0xC($sp)
    /* E6F4 8000DAF4 AFA60010 */  sw         $a2, 0x10($sp)
    /* E6F8 8000DAF8 AFA70014 */  sw         $a3, 0x14($sp)
    /* E6FC 8000DAFC DFAF0010 */  ld         $t7, 0x10($sp)
    /* E700 8000DB00 DFAE0008 */  ld         $t6, 0x8($sp)
    /* E704 8000DB04 01CF001E */  ddiv       $zero, $t6, $t7
    /* E708 8000DB08 00000000 */  nop
    /* E70C 8000DB0C 15E00002 */  bnez       $t7, .L8000DB18
    /* E710 8000DB10 00000000 */   nop
    /* E714 8000DB14 0007000D */  break      7
  .L8000DB18:
    /* E718 8000DB18 6401FFFF */  daddiu     $at, $zero, -0x1
    /* E71C 8000DB1C 15E10005 */  bne        $t7, $at, .L8000DB34
    /* E720 8000DB20 64010001 */   daddiu    $at, $zero, 0x1
    /* E724 8000DB24 00010FFC */  dsll32     $at, $at, 31
    /* E728 8000DB28 15C10002 */  bne        $t6, $at, .L8000DB34
    /* E72C 8000DB2C 00000000 */   nop
    /* E730 8000DB30 0006000D */  break      6
  .L8000DB34:
    /* E734 8000DB34 0000C010 */  mfhi       $t8
    /* E738 8000DB38 FFB80000 */  sd         $t8, 0x0($sp)
    /* E73C 8000DB3C 07010003 */  bgez       $t8, .L8000DB4C
    /* E740 8000DB40 00000000 */   nop
    /* E744 8000DB44 1DE00007 */  bgtz       $t7, .L8000DB64
    /* E748 8000DB48 00000000 */   nop
  .L8000DB4C:
    /* E74C 8000DB4C DFB90000 */  ld         $t9, 0x0($sp)
    /* E750 8000DB50 1B200008 */  blez       $t9, .L8000DB74
    /* E754 8000DB54 00000000 */   nop
    /* E758 8000DB58 DFA80010 */  ld         $t0, 0x10($sp)
    /* E75C 8000DB5C 05010005 */  bgez       $t0, .L8000DB74
    /* E760 8000DB60 00000000 */   nop
  .L8000DB64:
    /* E764 8000DB64 DFA90000 */  ld         $t1, 0x0($sp)
    /* E768 8000DB68 DFAA0010 */  ld         $t2, 0x10($sp)
    /* E76C 8000DB6C 012A582D */  daddu      $t3, $t1, $t2
    /* E770 8000DB70 FFAB0000 */  sd         $t3, 0x0($sp)
  .L8000DB74:
    /* E774 8000DB74 8FA20000 */  lw         $v0, 0x0($sp)
    /* E778 8000DB78 8FA30004 */  lw         $v1, 0x4($sp)
    /* E77C 8000DB7C 03E00008 */  jr         $ra
    /* E780 8000DB80 27BD0008 */   addiu     $sp, $sp, 0x8