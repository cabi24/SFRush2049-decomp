nonmatching func_8001DA74, 0x194

glabel func_8001DA74
    /* 1E674 8001DA74 3C088005 */  lui        $t0, %hi(D_8004FF20)
    /* 1E678 8001DA78 2508FF20 */  addiu      $t0, $t0, %lo(D_8004FF20)
    /* 1E67C 8001DA7C 91030000 */  lbu        $v1, 0x0($t0)
    /* 1E680 8001DA80 44856000 */  mtc1       $a1, $fa0
    /* 1E684 8001DA84 44867000 */  mtc1       $a2, $fa1
    /* 1E688 8001DA88 AFA7000C */  sw         $a3, 0xC($sp)
    /* 1E68C 8001DA8C 00001025 */  or         $v0, $zero, $zero
    /* 1E690 8001DA90 1860000B */  blez       $v1, .L8001DAC0
    /* 1E694 8001DA94 00602825 */   or        $a1, $v1, $zero
    /* 1E698 8001DA98 3C078005 */  lui        $a3, %hi(D_8004FDA0)
    /* 1E69C 8001DA9C 24E7FDA0 */  addiu      $a3, $a3, %lo(D_8004FDA0)
    /* 1E6A0 8001DAA0 8C860038 */  lw         $a2, 0x38($a0)
  .L8001DAA4:
    /* 1E6A4 8001DAA4 8CEE0000 */  lw         $t6, 0x0($a3)
    /* 1E6A8 8001DAA8 10CE0005 */  beq        $a2, $t6, .L8001DAC0
    /* 1E6AC 8001DAAC 00000000 */   nop
    /* 1E6B0 8001DAB0 24420001 */  addiu      $v0, $v0, 0x1
    /* 1E6B4 8001DAB4 0045082A */  slt        $at, $v0, $a1
    /* 1E6B8 8001DAB8 1420FFFA */  bnez       $at, .L8001DAA4
    /* 1E6BC 8001DABC 24E7000C */   addiu     $a3, $a3, 0xC
  .L8001DAC0:
    /* 1E6C0 8001DAC0 14450010 */  bne        $v0, $a1, .L8001DB04
    /* 1E6C4 8001DAC4 24010020 */   addiu     $at, $zero, 0x20
    /* 1E6C8 8001DAC8 14A10003 */  bne        $a1, $at, .L8001DAD8
    /* 1E6CC 8001DACC 00027880 */   sll       $t7, $v0, 2
    /* 1E6D0 8001DAD0 03E00008 */  jr         $ra
    /* 1E6D4 8001DAD4 00001025 */   or        $v0, $zero, $zero
  .L8001DAD8:
    /* 1E6D8 8001DAD8 01E27823 */  subu       $t7, $t7, $v0
    /* 1E6DC 8001DADC 3C188005 */  lui        $t8, %hi(D_8004FDA0)
    /* 1E6E0 8001DAE0 2718FDA0 */  addiu      $t8, $t8, %lo(D_8004FDA0)
    /* 1E6E4 8001DAE4 000F7880 */  sll        $t7, $t7, 2
    /* 1E6E8 8001DAE8 01F83821 */  addu       $a3, $t7, $t8
    /* 1E6EC 8001DAEC ACE00004 */  sw         $zero, 0x4($a3)
    /* 1E6F0 8001DAF0 ACE00008 */  sw         $zero, 0x8($a3)
    /* 1E6F4 8001DAF4 8C990038 */  lw         $t9, 0x38($a0)
    /* 1E6F8 8001DAF8 246A0001 */  addiu      $t2, $v1, 0x1
    /* 1E6FC 8001DAFC A10A0000 */  sb         $t2, 0x0($t0)
    /* 1E700 8001DB00 ACF90000 */  sw         $t9, 0x0($a3)
  .L8001DB04:
    /* 1E704 8001DB04 3C098005 */  lui        $t1, %hi(D_800502A8)
    /* 1E708 8001DB08 252902A8 */  addiu      $t1, $t1, %lo(D_800502A8)
    /* 1E70C 8001DB0C 91230000 */  lbu        $v1, 0x0($t1)
    /* 1E710 8001DB10 24010020 */  addiu      $at, $zero, 0x20
    /* 1E714 8001DB14 00025880 */  sll        $t3, $v0, 2
    /* 1E718 8001DB18 14610003 */  bne        $v1, $at, .L8001DB28
    /* 1E71C 8001DB1C 01625823 */   subu      $t3, $t3, $v0
    /* 1E720 8001DB20 03E00008 */  jr         $ra
    /* 1E724 8001DB24 00001025 */   or        $v0, $zero, $zero
  .L8001DB28:
    /* 1E728 8001DB28 3C0C8005 */  lui        $t4, %hi(D_8004FDA0)
    /* 1E72C 8001DB2C 258CFDA0 */  addiu      $t4, $t4, %lo(D_8004FDA0)
    /* 1E730 8001DB30 000B5880 */  sll        $t3, $t3, 2
    /* 1E734 8001DB34 016C3821 */  addu       $a3, $t3, $t4
    /* 1E738 8001DB38 8CE60004 */  lw         $a2, 0x4($a3)
    /* 1E73C 8001DB3C 0003C8C0 */  sll        $t9, $v1, 3
    /* 1E740 8001DB40 0323C823 */  subu       $t9, $t9, $v1
    /* 1E744 8001DB44 10C0001C */  beqz       $a2, .L8001DBB8
    /* 1E748 8001DB48 00C02825 */   or        $a1, $a2, $zero
    /* 1E74C 8001DB4C 8CA20000 */  lw         $v0, 0x0($a1)
    /* 1E750 8001DB50 000368C0 */  sll        $t5, $v1, 3
    /* 1E754 8001DB54 01A36823 */  subu       $t5, $t5, $v1
    /* 1E758 8001DB58 1040000A */  beqz       $v0, .L8001DB84
    /* 1E75C 8001DB5C 000D6880 */   sll       $t5, $t5, 2
    /* 1E760 8001DB60 C4A40004 */  lwc1       $ft0, 0x4($a1)
  .L8001DB64:
    /* 1E764 8001DB64 460C203C */  c.lt.s     $ft0, $fa0
    /* 1E768 8001DB68 00000000 */  nop
    /* 1E76C 8001DB6C 45010005 */  bc1t       .L8001DB84
    /* 1E770 8001DB70 00000000 */   nop
    /* 1E774 8001DB74 00402825 */  or         $a1, $v0, $zero
    /* 1E778 8001DB78 8C420000 */  lw         $v0, 0x0($v0)
    /* 1E77C 8001DB7C 5440FFF9 */  bnel       $v0, $zero, .L8001DB64
    /* 1E780 8001DB80 C4A40004 */   lwc1      $ft0, 0x4($a1)
  .L8001DB84:
    /* 1E784 8001DB84 3C0E8005 */  lui        $t6, %hi(D_8004FF28)
    /* 1E788 8001DB88 25CEFF28 */  addiu      $t6, $t6, %lo(D_8004FF28)
    /* 1E78C 8001DB8C 01AE4021 */  addu       $t0, $t5, $t6
    /* 1E790 8001DB90 AD020000 */  sw         $v0, 0x0($t0)
    /* 1E794 8001DB94 ACA80000 */  sw         $t0, 0x0($a1)
    /* 1E798 8001DB98 91230000 */  lbu        $v1, 0x0($t1)
    /* 1E79C 8001DB9C 3C188005 */  lui        $t8, %hi(D_8004FF28)
    /* 1E7A0 8001DBA0 2718FF28 */  addiu      $t8, $t8, %lo(D_8004FF28)
    /* 1E7A4 8001DBA4 000378C0 */  sll        $t7, $v1, 3
    /* 1E7A8 8001DBA8 01E37823 */  subu       $t7, $t7, $v1
    /* 1E7AC 8001DBAC 000F7880 */  sll        $t7, $t7, 2
    /* 1E7B0 8001DBB0 10000007 */  b          .L8001DBD0
    /* 1E7B4 8001DBB4 01F84021 */   addu      $t0, $t7, $t8
  .L8001DBB8:
    /* 1E7B8 8001DBB8 3C0A8005 */  lui        $t2, %hi(D_8004FF28)
    /* 1E7BC 8001DBBC 254AFF28 */  addiu      $t2, $t2, %lo(D_8004FF28)
    /* 1E7C0 8001DBC0 0019C880 */  sll        $t9, $t9, 2
    /* 1E7C4 8001DBC4 032A4021 */  addu       $t0, $t9, $t2
    /* 1E7C8 8001DBC8 AD060000 */  sw         $a2, 0x0($t0)
    /* 1E7CC 8001DBCC ACE80004 */  sw         $t0, 0x4($a3)
  .L8001DBD0:
    /* 1E7D0 8001DBD0 C7A60014 */  lwc1       $ft1, 0x14($sp)
    /* 1E7D4 8001DBD4 C7A8000C */  lwc1       $ft2, 0xC($sp)
    /* 1E7D8 8001DBD8 C7AA0010 */  lwc1       $ft3, 0x10($sp)
    /* 1E7DC 8001DBDC 246B0001 */  addiu      $t3, $v1, 0x1
    /* 1E7E0 8001DBE0 AD040018 */  sw         $a0, 0x18($t0)
    /* 1E7E4 8001DBE4 E50E0008 */  swc1       $fa1, 0x8($t0)
    /* 1E7E8 8001DBE8 A12B0000 */  sb         $t3, 0x0($t1)
    /* 1E7EC 8001DBEC E50C0004 */  swc1       $fa0, 0x4($t0)
    /* 1E7F0 8001DBF0 24020001 */  addiu      $v0, $zero, 0x1
    /* 1E7F4 8001DBF4 E5060014 */  swc1       $ft1, 0x14($t0)
    /* 1E7F8 8001DBF8 E508000C */  swc1       $ft2, 0xC($t0)
    /* 1E7FC 8001DBFC E50A0010 */  swc1       $ft3, 0x10($t0)
    /* 1E800 8001DC00 03E00008 */  jr         $ra
    /* 1E804 8001DC04 00000000 */   nop
endlabel func_8001DA74
