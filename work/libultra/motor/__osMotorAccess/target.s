# Source: AB20.s
# Address: 0x8000A088

glabel func_8000A088
    /* AC88 8000A088 27BDFFB0 */  addiu      $sp, $sp, -0x50
    /* AC8C 8000A08C AFBF0014 */  sw         $ra, 0x14($sp)
    /* AC90 8000A090 00803025 */  or         $a2, $a0, $zero
    /* AC94 8000A094 240E00FF */  addiu      $t6, $zero, 0xFF
    /* AC98 8000A098 240F0023 */  addiu      $t7, $zero, 0x23
    /* AC9C 8000A09C 24180001 */  addiu      $t8, $zero, 0x1
    /* ACA0 8000A0A0 24190003 */  addiu      $t9, $zero, 0x3
    /* ACA4 8000A0A4 240800C0 */  addiu      $t0, $zero, 0xC0
    /* ACA8 8000A0A8 A3AE0024 */  sb         $t6, 0x24($sp)
    /* ACAC 8000A0AC A3AF0025 */  sb         $t7, 0x25($sp)
    /* ACB0 8000A0B0 A3B80026 */  sb         $t8, 0x26($sp)
    /* ACB4 8000A0B4 A3B90027 */  sb         $t9, 0x27($sp)
    /* ACB8 8000A0B8 A3A80028 */  sb         $t0, 0x28($sp)
    /* ACBC 8000A0BC AFA60050 */  sw         $a2, 0x50($sp)
    /* ACC0 8000A0C0 24040600 */  addiu      $a0, $zero, 0x600
    /* ACC4 8000A0C4 0C003E7C */  jal        func_8000F9F0
    /* ACC8 8000A0C8 AFA5004C */   sw        $a1, 0x4C($sp)
    /* ACCC 8000A0CC 8FA60050 */  lw         $a2, 0x50($sp)
    /* ACD0 8000A0D0 3449C000 */  ori        $t1, $v0, 0xC000
    /* ACD4 8000A0D4 A3A90029 */  sb         $t1, 0x29($sp)
    /* ACD8 8000A0D8 10C00012 */  beqz       $a2, .L8000A124
    /* ACDC 8000A0DC 8FA3004C */   lw        $v1, 0x4C($sp)
    /* ACE0 8000A0E0 18C00010 */  blez       $a2, .L8000A124
    /* ACE4 8000A0E4 00001025 */   or        $v0, $zero, $zero
    /* ACE8 8000A0E8 30C50003 */  andi       $a1, $a2, 0x3
    /* ACEC 8000A0EC 10A00006 */  beqz       $a1, .L8000A108
    /* ACF0 8000A0F0 00A02025 */   or        $a0, $a1, $zero
  .L8000A0F4:
    /* ACF4 8000A0F4 24420001 */  addiu      $v0, $v0, 0x1
    /* ACF8 8000A0F8 A0600000 */  sb         $zero, 0x0($v1)
    /* ACFC 8000A0FC 1482FFFD */  bne        $a0, $v0, .L8000A0F4
    /* AD00 8000A100 24630001 */   addiu     $v1, $v1, 0x1
    /* AD04 8000A104 10460007 */  beq        $v0, $a2, .L8000A124
  .L8000A108:
    /* AD08 8000A108 24420004 */   addiu     $v0, $v0, 0x4
    /* AD0C 8000A10C A0600001 */  sb         $zero, 0x1($v1)
    /* AD10 8000A110 A0600002 */  sb         $zero, 0x2($v1)
    /* AD14 8000A114 A0600003 */  sb         $zero, 0x3($v1)
    /* AD18 8000A118 24630004 */  addiu      $v1, $v1, 0x4
    /* AD1C 8000A11C 1446FFFA */  bne        $v0, $a2, .L8000A108
    /* AD20 8000A120 A060FFFC */   sb        $zero, -0x4($v1)
  .L8000A124:
    /* AD24 8000A124 27AA0024 */  addiu      $t2, $sp, 0x24
    /* AD28 8000A128 254C0024 */  addiu      $t4, $t2, 0x24
    /* AD2C 8000A12C 00606825 */  or         $t5, $v1, $zero
  .L8000A130:
    /* AD30 8000A130 8D410000 */  lw         $at, 0x0($t2)
    /* AD34 8000A134 254A000C */  addiu      $t2, $t2, 0xC
    /* AD38 8000A138 25AD000C */  addiu      $t5, $t5, 0xC
    /* AD3C 8000A13C A9A1FFF4 */  swl        $at, -0xC($t5)
    /* AD40 8000A140 B9A1FFF7 */  swr        $at, -0x9($t5)
    /* AD44 8000A144 8D41FFF8 */  lw         $at, -0x8($t2)
    /* AD48 8000A148 A9A1FFF8 */  swl        $at, -0x8($t5)
    /* AD4C 8000A14C B9A1FFFB */  swr        $at, -0x5($t5)
    /* AD50 8000A150 8D41FFFC */  lw         $at, -0x4($t2)
    /* AD54 8000A154 A9A1FFFC */  swl        $at, -0x4($t5)
    /* AD58 8000A158 154CFFF5 */  bne        $t2, $t4, .L8000A130
    /* AD5C 8000A15C B9A1FFFF */   swr       $at, -0x1($t5)
    /* AD60 8000A160 91410000 */  lbu        $at, 0x0($t2)
    /* AD64 8000A164 240E00FE */  addiu      $t6, $zero, 0xFE
    /* AD68 8000A168 24630027 */  addiu      $v1, $v1, 0x27
    /* AD6C 8000A16C A1A10000 */  sb         $at, 0x0($t5)
    /* AD70 8000A170 914C0001 */  lbu        $t4, 0x1($t2)
    /* AD74 8000A174 A1AC0001 */  sb         $t4, 0x1($t5)
    /* AD78 8000A178 91410002 */  lbu        $at, 0x2($t2)
    /* AD7C 8000A17C A1A10002 */  sb         $at, 0x2($t5)
    /* AD80 8000A180 A06E0000 */  sb         $t6, 0x0($v1)
    /* AD84 8000A184 8FBF0014 */  lw         $ra, 0x14($sp)
    /* AD88 8000A188 27BD0050 */  addiu      $sp, $sp, 0x50
    /* AD8C 8000A18C 03E00008 */  jr         $ra
    /* AD90 8000A190 00000000 */   nop