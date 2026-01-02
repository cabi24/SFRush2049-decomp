# Source: E9A0.s
# Address: 0x8000DDA0

glabel func_8000DDA0
    /* E9A0 8000DDA0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* E9A4 8000DDA4 3C03A460 */  lui        $v1, %hi(PI_STATUS_REG)
    /* E9A8 8000DDA8 34630010 */  ori        $v1, $v1, %lo(PI_STATUS_REG)
    /* E9AC 8000DDAC AFBF001C */  sw         $ra, 0x1C($sp)
    /* E9B0 8000DDB0 AFB00018 */  sw         $s0, 0x18($sp)
    /* E9B4 8000DDB4 AFA60028 */  sw         $a2, 0x28($sp)
    /* E9B8 8000DDB8 8C620000 */  lw         $v0, 0x0($v1)
    /* E9BC 8000DDBC 00808025 */  or         $s0, $a0, $zero
    /* E9C0 8000DDC0 304E0003 */  andi       $t6, $v0, 0x3
    /* E9C4 8000DDC4 51C00006 */  beql       $t6, $zero, .L8000DDE0
    /* E9C8 8000DDC8 92020009 */   lbu       $v0, 0x9($s0)
    /* E9CC 8000DDCC 8C620000 */  lw         $v0, 0x0($v1)
  .L8000DDD0:
    /* E9D0 8000DDD0 304F0003 */  andi       $t7, $v0, 0x3
    /* E9D4 8000DDD4 55E0FFFE */  bnel       $t7, $zero, .L8000DDD0
    /* E9D8 8000DDD8 8C620000 */   lw        $v0, 0x0($v1)
    /* E9DC 8000DDDC 92020009 */  lbu        $v0, 0x9($s0)
  .L8000DDE0:
    /* E9E0 8000DDE0 3C048003 */  lui        $a0, %hi(D_8002C3A0)
    /* E9E4 8000DDE4 92190004 */  lbu        $t9, 0x4($s0)
    /* E9E8 8000DDE8 0002C080 */  sll        $t8, $v0, 2
    /* E9EC 8000DDEC 00982021 */  addu       $a0, $a0, $t8
    /* E9F0 8000DDF0 8C84C3A0 */  lw         $a0, %lo(D_8002C3A0)($a0)
    /* E9F4 8000DDF4 90880004 */  lbu        $t0, 0x4($a0)
    /* E9F8 8000DDF8 5328003D */  beql       $t9, $t0, .L8000DEF0
    /* E9FC 8000DDFC 00E02025 */   or        $a0, $a3, $zero
    /* EA00 8000DE00 14400019 */  bnez       $v0, .L8000DE68
    /* EA04 8000DE04 00801825 */   or        $v1, $a0, $zero
    /* EA08 8000DE08 92020005 */  lbu        $v0, 0x5($s0)
    /* EA0C 8000DE0C 90890005 */  lbu        $t1, 0x5($a0)
    /* EA10 8000DE10 3C0AA460 */  lui        $t2, %hi(PI_BSD_DOM1_LAT_REG)
    /* EA14 8000DE14 3C0CA460 */  lui        $t4, %hi(PI_BSD_DOM1_PGS_REG)
    /* EA18 8000DE18 10490002 */  beq        $v0, $t1, .L8000DE24
    /* EA1C 8000DE1C 3C0EA460 */   lui       $t6, %hi(PI_BSD_DOM1_RLS_REG)
    /* EA20 8000DE20 AD420014 */  sw         $v0, %lo(PI_BSD_DOM1_LAT_REG)($t2)
  .L8000DE24:
    /* EA24 8000DE24 92020006 */  lbu        $v0, 0x6($s0)
    /* EA28 8000DE28 906B0006 */  lbu        $t3, 0x6($v1)
    /* EA2C 8000DE2C 3C18A460 */  lui        $t8, %hi(PI_BSD_DOM1_PWD_REG)
    /* EA30 8000DE30 504B0003 */  beql       $v0, $t3, .L8000DE40
    /* EA34 8000DE34 92020007 */   lbu       $v0, 0x7($s0)
    /* EA38 8000DE38 AD82001C */  sw         $v0, %lo(PI_BSD_DOM1_PGS_REG)($t4)
    /* EA3C 8000DE3C 92020007 */  lbu        $v0, 0x7($s0)
  .L8000DE40:
    /* EA40 8000DE40 906D0007 */  lbu        $t5, 0x7($v1)
    /* EA44 8000DE44 504D0003 */  beql       $v0, $t5, .L8000DE54
    /* EA48 8000DE48 92020008 */   lbu       $v0, 0x8($s0)
    /* EA4C 8000DE4C ADC20020 */  sw         $v0, %lo(PI_BSD_DOM1_RLS_REG)($t6)
    /* EA50 8000DE50 92020008 */  lbu        $v0, 0x8($s0)
  .L8000DE54:
    /* EA54 8000DE54 906F0008 */  lbu        $t7, 0x8($v1)
    /* EA58 8000DE58 504F001B */  beql       $v0, $t7, .L8000DEC8
    /* EA5C 8000DE5C 920F0004 */   lbu       $t7, 0x4($s0)
    /* EA60 8000DE60 10000018 */  b          .L8000DEC4
    /* EA64 8000DE64 AF020018 */   sw        $v0, %lo(PI_BSD_DOM1_PWD_REG)($t8)
  .L8000DE68:
    /* EA68 8000DE68 92020005 */  lbu        $v0, 0x5($s0)
    /* EA6C 8000DE6C 90790005 */  lbu        $t9, 0x5($v1)
    /* EA70 8000DE70 3C08A460 */  lui        $t0, %hi(PI_BSD_DOM2_LAT_REG)
    /* EA74 8000DE74 3C0AA460 */  lui        $t2, %hi(PI_BSD_DOM2_PGS_REG)
    /* EA78 8000DE78 10590002 */  beq        $v0, $t9, .L8000DE84
    /* EA7C 8000DE7C 3C0CA460 */   lui       $t4, %hi(PI_BSD_DOM2_RLS_REG)
    /* EA80 8000DE80 AD020024 */  sw         $v0, %lo(PI_BSD_DOM2_LAT_REG)($t0)
  .L8000DE84:
    /* EA84 8000DE84 92020006 */  lbu        $v0, 0x6($s0)
    /* EA88 8000DE88 90690006 */  lbu        $t1, 0x6($v1)
    /* EA8C 8000DE8C 3C0EA460 */  lui        $t6, %hi(PI_BSD_DOM2_LWD_REG)
    /* EA90 8000DE90 50490003 */  beql       $v0, $t1, .L8000DEA0
    /* EA94 8000DE94 92020007 */   lbu       $v0, 0x7($s0)
    /* EA98 8000DE98 AD42002C */  sw         $v0, %lo(PI_BSD_DOM2_PGS_REG)($t2)
    /* EA9C 8000DE9C 92020007 */  lbu        $v0, 0x7($s0)
  .L8000DEA0:
    /* EAA0 8000DEA0 906B0007 */  lbu        $t3, 0x7($v1)
    /* EAA4 8000DEA4 504B0003 */  beql       $v0, $t3, .L8000DEB4
    /* EAA8 8000DEA8 92020008 */   lbu       $v0, 0x8($s0)
    /* EAAC 8000DEAC AD820030 */  sw         $v0, %lo(PI_BSD_DOM2_RLS_REG)($t4)
    /* EAB0 8000DEB0 92020008 */  lbu        $v0, 0x8($s0)
  .L8000DEB4:
    /* EAB4 8000DEB4 906D0008 */  lbu        $t5, 0x8($v1)
    /* EAB8 8000DEB8 504D0003 */  beql       $v0, $t5, .L8000DEC8
    /* EABC 8000DEBC 920F0004 */   lbu       $t7, 0x4($s0)
    /* EAC0 8000DEC0 ADC20028 */  sw         $v0, %lo(PI_BSD_DOM2_LWD_REG)($t6)
  .L8000DEC4:
    /* EAC4 8000DEC4 920F0004 */  lbu        $t7, 0x4($s0)
  .L8000DEC8:
    /* EAC8 8000DEC8 A06F0004 */  sb         $t7, 0x4($v1)
    /* EACC 8000DECC 92180005 */  lbu        $t8, 0x5($s0)
    /* EAD0 8000DED0 A0780005 */  sb         $t8, 0x5($v1)
    /* EAD4 8000DED4 92190006 */  lbu        $t9, 0x6($s0)
    /* EAD8 8000DED8 A0790006 */  sb         $t9, 0x6($v1)
    /* EADC 8000DEDC 92080007 */  lbu        $t0, 0x7($s0)
    /* EAE0 8000DEE0 A0680007 */  sb         $t0, 0x7($v1)
    /* EAE4 8000DEE4 92090008 */  lbu        $t1, 0x8($s0)
    /* EAE8 8000DEE8 A0690008 */  sb         $t1, 0x8($v1)
    /* EAEC 8000DEEC 00E02025 */  or         $a0, $a3, $zero
  .L8000DEF0:
    /* EAF0 8000DEF0 0C003570 */  jal        func_8000D5C0
    /* EAF4 8000DEF4 AFA50024 */   sw        $a1, 0x24($sp)
    /* EAF8 8000DEF8 8FA50024 */  lw         $a1, 0x24($sp)
    /* EAFC 8000DEFC 3C0AA460 */  lui        $t2, %hi(PI_DRAM_ADDR_REG)
    /* EB00 8000DF00 AD420000 */  sw         $v0, %lo(PI_DRAM_ADDR_REG)($t2)
    /* EB04 8000DF04 8FAC0028 */  lw         $t4, 0x28($sp)
    /* EB08 8000DF08 8E0B000C */  lw         $t3, 0xC($s0)
    /* EB0C 8000DF0C 3C011FFF */  lui        $at, (0x1FFFFFFF >> 16)
    /* EB10 8000DF10 3421FFFF */  ori        $at, $at, (0x1FFFFFFF & 0xFFFF)
    /* EB14 8000DF14 016C6825 */  or         $t5, $t3, $t4
    /* EB18 8000DF18 01A17024 */  and        $t6, $t5, $at
    /* EB1C 8000DF1C 3C0FA460 */  lui        $t7, %hi(PI_CART_ADDR_REG)
    /* EB20 8000DF20 10A00006 */  beqz       $a1, .L8000DF3C
    /* EB24 8000DF24 ADEE0004 */   sw        $t6, %lo(PI_CART_ADDR_REG)($t7)
    /* EB28 8000DF28 24010001 */  addiu      $at, $zero, 0x1
    /* EB2C 8000DF2C 10A10008 */  beq        $a1, $at, .L8000DF50
    /* EB30 8000DF30 8FA90030 */   lw        $t1, 0x30($sp)
    /* EB34 8000DF34 1000000D */  b          .L8000DF6C
    /* EB38 8000DF38 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8000DF3C:
    /* EB3C 8000DF3C 8FB80030 */  lw         $t8, 0x30($sp)
    /* EB40 8000DF40 3C08A460 */  lui        $t0, %hi(PI_WR_LEN_REG)
    /* EB44 8000DF44 2719FFFF */  addiu      $t9, $t8, -0x1
    /* EB48 8000DF48 10000007 */  b          .L8000DF68
    /* EB4C 8000DF4C AD19000C */   sw        $t9, %lo(PI_WR_LEN_REG)($t0)
  .L8000DF50:
    /* EB50 8000DF50 252AFFFF */  addiu      $t2, $t1, -0x1
    /* EB54 8000DF54 3C0BA460 */  lui        $t3, %hi(PI_RD_LEN_REG)
    /* EB58 8000DF58 10000003 */  b          .L8000DF68
    /* EB5C 8000DF5C AD6A0008 */   sw        $t2, %lo(PI_RD_LEN_REG)($t3)
    /* EB60 8000DF60 10000002 */  b          .L8000DF6C
    /* EB64 8000DF64 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8000DF68:
    /* EB68 8000DF68 00001025 */  or         $v0, $zero, $zero
  .L8000DF6C:
    /* EB6C 8000DF6C 8FBF001C */  lw         $ra, 0x1C($sp)
    /* EB70 8000DF70 8FB00018 */  lw         $s0, 0x18($sp)
    /* EB74 8000DF74 27BD0020 */  addiu      $sp, $sp, 0x20
    /* EB78 8000DF78 03E00008 */  jr         $ra
    /* EB7C 8000DF7C 00000000 */   nop