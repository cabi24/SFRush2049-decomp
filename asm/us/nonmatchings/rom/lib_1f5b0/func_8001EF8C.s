nonmatching func_8001EF8C, 0x1B0

glabel func_8001EF8C
    /* 1FB8C 8001EF8C 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 1FB90 8001EF90 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 1FB94 8001EF94 AFB00018 */  sw         $s0, 0x18($sp)
    /* 1FB98 8001EF98 AFA40030 */  sw         $a0, 0x30($sp)
    /* 1FB9C 8001EF9C AFA50034 */  sw         $a1, 0x34($sp)
    /* 1FBA0 8001EFA0 88890060 */  lwl        $t1, 0x60($a0)
    /* 1FBA4 8001EFA4 98890063 */  lwr        $t1, 0x63($a0)
    /* 1FBA8 8001EFA8 3C188005 */  lui        $t8, %hi(D_800504C8)
    /* 1FBAC 8001EFAC 271804C8 */  addiu      $t8, $t8, %lo(D_800504C8)
    /* 1FBB0 8001EFB0 312900FF */  andi       $t1, $t1, 0xFF
    /* 1FBB4 8001EFB4 00097880 */  sll        $t7, $t1, 2
    /* 1FBB8 8001EFB8 01F81821 */  addu       $v1, $t7, $t8
    /* 1FBBC 8001EFBC 94790002 */  lhu        $t9, 0x2($v1)
    /* 1FBC0 8001EFC0 24010001 */  addiu      $at, $zero, 0x1
    /* 1FBC4 8001EFC4 30B000FF */  andi       $s0, $a1, 0xFF
    /* 1FBC8 8001EFC8 17210009 */  bne        $t9, $at, .L8001EFF0
    /* 1FBCC 8001EFCC 00000000 */   nop
    /* 1FBD0 8001EFD0 908A002E */  lbu        $t2, 0x2E($a0)
    /* 1FBD4 8001EFD4 520A0055 */  beql       $s0, $t2, .L8001F12C
    /* 1FBD8 8001EFD8 8FBF001C */   lw        $ra, 0x1C($sp)
    /* 1FBDC 8001EFDC AFA30020 */  sw         $v1, 0x20($sp)
    /* 1FBE0 8001EFE0 0C007BA7 */  jal        func_8001EE9C
    /* 1FBE4 8001EFE4 A3A90027 */   sb        $t1, 0x27($sp)
    /* 1FBE8 8001EFE8 8FA30020 */  lw         $v1, 0x20($sp)
    /* 1FBEC 8001EFEC 93A90027 */  lbu        $t1, 0x27($sp)
  .L8001EFF0:
    /* 1FBF0 8001EFF0 3C0D8005 */  lui        $t5, %hi(D_80050548)
    /* 1FBF4 8001EFF4 240B0001 */  addiu      $t3, $zero, 0x1
    /* 1FBF8 8001EFF8 240C00FF */  addiu      $t4, $zero, 0xFF
    /* 1FBFC 8001EFFC 25AD0548 */  addiu      $t5, $t5, %lo(D_80050548)
    /* 1FC00 8001F000 A46B0002 */  sh         $t3, 0x2($v1)
    /* 1FC04 8001F004 A06C0000 */  sb         $t4, 0x0($v1)
    /* 1FC08 8001F008 020D4021 */  addu       $t0, $s0, $t5
    /* 1FC0C 8001F00C 91020000 */  lbu        $v0, 0x0($t0)
    /* 1FC10 8001F010 240100FF */  addiu      $at, $zero, 0xFF
    /* 1FC14 8001F014 10410007 */  beq        $v0, $at, .L8001F034
    /* 1FC18 8001F018 A0620001 */   sb        $v0, 0x1($v1)
    /* 1FC1C 8001F01C 910F0000 */  lbu        $t7, 0x0($t0)
    /* 1FC20 8001F020 3C018005 */  lui        $at, %hi(D_800504C8)
    /* 1FC24 8001F024 000FC080 */  sll        $t8, $t7, 2
    /* 1FC28 8001F028 00380821 */  addu       $at, $at, $t8
    /* 1FC2C 8001F02C 1000003B */  b          .L8001F11C
    /* 1FC30 8001F030 A02904C8 */   sb        $t1, %lo(D_800504C8)($at)
  .L8001F034:
    /* 1FC34 8001F034 3C038005 */  lui        $v1, %hi(D_80050A48)
    /* 1FC38 8001F038 24630A48 */  addiu      $v1, $v1, %lo(D_80050A48)
    /* 1FC3C 8001F03C 94670000 */  lhu        $a3, 0x0($v1)
    /* 1FC40 8001F040 3406FFFF */  ori        $a2, $zero, 0xFFFF
    /* 1FC44 8001F044 3404FFFF */  ori        $a0, $zero, 0xFFFF
    /* 1FC48 8001F048 10C7002E */  beq        $a2, $a3, .L8001F104
    /* 1FC4C 8001F04C 3C058005 */   lui       $a1, %hi(D_80050648)
    /* 1FC50 8001F050 0207082A */  slt        $at, $s0, $a3
    /* 1FC54 8001F054 1420001F */  bnez       $at, .L8001F0D4
    /* 1FC58 8001F058 02002025 */   or        $a0, $s0, $zero
    /* 1FC5C 8001F05C 30E3FFFF */  andi       $v1, $a3, 0xFFFF
    /* 1FC60 8001F060 10C3000B */  beq        $a2, $v1, .L8001F090
    /* 1FC64 8001F064 00601025 */   or        $v0, $v1, $zero
    /* 1FC68 8001F068 3C058005 */  lui        $a1, %hi(D_80050648)
    /* 1FC6C 8001F06C 24A50648 */  addiu      $a1, $a1, %lo(D_80050648)
  .L8001F070:
    /* 1FC70 8001F070 0082082A */  slt        $at, $a0, $v0
    /* 1FC74 8001F074 14200006 */  bnez       $at, .L8001F090
    /* 1FC78 8001F078 0003C880 */   sll       $t9, $v1, 2
    /* 1FC7C 8001F07C 00B97021 */  addu       $t6, $a1, $t9
    /* 1FC80 8001F080 A7A3002C */  sh         $v1, 0x2C($sp)
    /* 1FC84 8001F084 95C30000 */  lhu        $v1, 0x0($t6)
    /* 1FC88 8001F088 14C3FFF9 */  bne        $a2, $v1, .L8001F070
    /* 1FC8C 8001F08C 00601025 */   or        $v0, $v1, $zero
  .L8001F090:
    /* 1FC90 8001F090 97AA002C */  lhu        $t2, 0x2C($sp)
    /* 1FC94 8001F094 3C058005 */  lui        $a1, %hi(D_80050648)
    /* 1FC98 8001F098 24A50648 */  addiu      $a1, $a1, %lo(D_80050648)
    /* 1FC9C 8001F09C 000A5880 */  sll        $t3, $t2, 2
    /* 1FCA0 8001F0A0 00AB6021 */  addu       $t4, $a1, $t3
    /* 1FCA4 8001F0A4 A5900000 */  sh         $s0, 0x0($t4)
    /* 1FCA8 8001F0A8 97AF002C */  lhu        $t7, 0x2C($sp)
    /* 1FCAC 8001F0AC 00106880 */  sll        $t5, $s0, 2
    /* 1FCB0 8001F0B0 00AD1021 */  addu       $v0, $a1, $t5
    /* 1FCB4 8001F0B4 3078FFFF */  andi       $t8, $v1, 0xFFFF
    /* 1FCB8 8001F0B8 A4430000 */  sh         $v1, 0x0($v0)
    /* 1FCBC 8001F0BC 10D80017 */  beq        $a2, $t8, .L8001F11C
    /* 1FCC0 8001F0C0 A44F0002 */   sh        $t7, 0x2($v0)
    /* 1FCC4 8001F0C4 0003C880 */  sll        $t9, $v1, 2
    /* 1FCC8 8001F0C8 00B97021 */  addu       $t6, $a1, $t9
    /* 1FCCC 8001F0CC 10000013 */  b          .L8001F11C
    /* 1FCD0 8001F0D0 A5D00002 */   sh        $s0, 0x2($t6)
  .L8001F0D4:
    /* 1FCD4 8001F0D4 3C058005 */  lui        $a1, %hi(D_80050648)
    /* 1FCD8 8001F0D8 24A50648 */  addiu      $a1, $a1, %lo(D_80050648)
    /* 1FCDC 8001F0DC 00105080 */  sll        $t2, $s0, 2
    /* 1FCE0 8001F0E0 00AA1021 */  addu       $v0, $a1, $t2
    /* 1FCE4 8001F0E4 3404FFFF */  ori        $a0, $zero, 0xFFFF
    /* 1FCE8 8001F0E8 00075880 */  sll        $t3, $a3, 2
    /* 1FCEC 8001F0EC A4470000 */  sh         $a3, 0x0($v0)
    /* 1FCF0 8001F0F0 A4440002 */  sh         $a0, 0x2($v0)
    /* 1FCF4 8001F0F4 00AB6021 */  addu       $t4, $a1, $t3
    /* 1FCF8 8001F0F8 A5900002 */  sh         $s0, 0x2($t4)
    /* 1FCFC 8001F0FC 10000007 */  b          .L8001F11C
    /* 1FD00 8001F100 A4700000 */   sh        $s0, 0x0($v1)
  .L8001F104:
    /* 1FD04 8001F104 24A50648 */  addiu      $a1, $a1, %lo(D_80050648)
    /* 1FD08 8001F108 00106880 */  sll        $t5, $s0, 2
    /* 1FD0C 8001F10C 00AD1021 */  addu       $v0, $a1, $t5
    /* 1FD10 8001F110 A4440000 */  sh         $a0, 0x0($v0)
    /* 1FD14 8001F114 A4440002 */  sh         $a0, 0x2($v0)
    /* 1FD18 8001F118 A4700000 */  sh         $s0, 0x0($v1)
  .L8001F11C:
    /* 1FD1C 8001F11C 8FAF0030 */  lw         $t7, 0x30($sp)
    /* 1FD20 8001F120 A1090000 */  sb         $t1, 0x0($t0)
    /* 1FD24 8001F124 A1F0002E */  sb         $s0, 0x2E($t7)
    /* 1FD28 8001F128 8FBF001C */  lw         $ra, 0x1C($sp)
  .L8001F12C:
    /* 1FD2C 8001F12C 8FB00018 */  lw         $s0, 0x18($sp)
    /* 1FD30 8001F130 27BD0030 */  addiu      $sp, $sp, 0x30
    /* 1FD34 8001F134 03E00008 */  jr         $ra
    /* 1FD38 8001F138 00000000 */   nop
endlabel func_8001EF8C
