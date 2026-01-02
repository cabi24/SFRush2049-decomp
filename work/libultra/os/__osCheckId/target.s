# Source: F700.s
# Address: 0x8000EFC0

glabel func_8000EFC0
    /* FBC0 8000EFC0 27BDFFA0 */  addiu      $sp, $sp, -0x60
    /* FBC4 8000EFC4 AFBF003C */  sw         $ra, 0x3C($sp)
    /* FBC8 8000EFC8 AFB60038 */  sw         $s6, 0x38($sp)
    /* FBCC 8000EFCC AFB50034 */  sw         $s5, 0x34($sp)
    /* FBD0 8000EFD0 AFB40030 */  sw         $s4, 0x30($sp)
    /* FBD4 8000EFD4 AFB3002C */  sw         $s3, 0x2C($sp)
    /* FBD8 8000EFD8 AFB20028 */  sw         $s2, 0x28($sp)
    /* FBDC 8000EFDC AFB10024 */  sw         $s1, 0x24($sp)
    /* FBE0 8000EFE0 AFB00020 */  sw         $s0, 0x20($sp)
    /* FBE4 8000EFE4 908E0065 */  lbu        $t6, 0x65($a0)
    /* FBE8 8000EFE8 00A09025 */  or         $s2, $a1, $zero
    /* FBEC 8000EFEC 00809825 */  or         $s3, $a0, $zero
    /* FBF0 8000EFF0 51C00008 */  beql       $t6, $zero, .L8000F014
    /* FBF4 8000EFF4 240F0001 */   addiu     $t7, $zero, 0x1
    /* FBF8 8000EFF8 0C003A14 */  jal        func_8000E850
    /* FBFC 8000EFFC 00002825 */   or        $a1, $zero, $zero
    /* FC00 8000F000 50400004 */  beql       $v0, $zero, .L8000F014
    /* FC04 8000F004 240F0001 */   addiu     $t7, $zero, 0x1
    /* FC08 8000F008 1000003D */  b          .L8000F100
    /* FC0C 8000F00C 8FBF003C */   lw        $ra, 0x3C($sp)
    /* FC10 8000F010 240F0001 */  addiu      $t7, $zero, 0x1
  .L8000F014:
    /* FC14 8000F014 24180003 */  addiu      $t8, $zero, 0x3
    /* FC18 8000F018 24190004 */  addiu      $t9, $zero, 0x4
    /* FC1C 8000F01C 24080006 */  addiu      $t0, $zero, 0x6
    /* FC20 8000F020 A7AF0058 */  sh         $t7, 0x58($sp)
    /* FC24 8000F024 A7B8005A */  sh         $t8, 0x5A($sp)
    /* FC28 8000F028 A7B9005C */  sh         $t9, 0x5C($sp)
    /* FC2C 8000F02C A7A8005E */  sh         $t0, 0x5E($sp)
    /* FC30 8000F030 24110001 */  addiu      $s1, $zero, 0x1
    /* FC34 8000F034 27B0005A */  addiu      $s0, $sp, 0x5A
    /* FC38 8000F038 27B60050 */  addiu      $s6, $sp, 0x50
    /* FC3C 8000F03C 24150004 */  addiu      $s5, $zero, 0x4
    /* FC40 8000F040 27B40052 */  addiu      $s4, $sp, 0x52
  .L8000F044:
    /* FC44 8000F044 8E640004 */  lw         $a0, 0x4($s3)
    /* FC48 8000F048 8E650008 */  lw         $a1, 0x8($s3)
    /* FC4C 8000F04C 96060000 */  lhu        $a2, 0x0($s0)
    /* FC50 8000F050 0C003A34 */  jal        func_8000E8D0
    /* FC54 8000F054 02403825 */   or        $a3, $s2, $zero
    /* FC58 8000F058 10400003 */  beqz       $v0, .L8000F068
    /* FC5C 8000F05C 02402025 */   or        $a0, $s2, $zero
    /* FC60 8000F060 10000027 */  b          .L8000F100
    /* FC64 8000F064 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8000F068:
    /* FC68 8000F068 02802825 */  or         $a1, $s4, $zero
    /* FC6C 8000F06C 0C003ADD */  jal        func_8000EB74
    /* FC70 8000F070 02C03025 */   or        $a2, $s6, $zero
    /* FC74 8000F074 97A90052 */  lhu        $t1, 0x52($sp)
    /* FC78 8000F078 964A001C */  lhu        $t2, 0x1C($s2)
    /* FC7C 8000F07C 97AB0050 */  lhu        $t3, 0x50($sp)
    /* FC80 8000F080 552A0005 */  bnel       $t1, $t2, .L8000F098
    /* FC84 8000F084 26310001 */   addiu     $s1, $s1, 0x1
    /* FC88 8000F088 964C001E */  lhu        $t4, 0x1E($s2)
    /* FC8C 8000F08C 116C0004 */  beq        $t3, $t4, .L8000F0A0
    /* FC90 8000F090 00000000 */   nop
    /* FC94 8000F094 26310001 */  addiu      $s1, $s1, 0x1
  .L8000F098:
    /* FC98 8000F098 1635FFEA */  bne        $s1, $s5, .L8000F044
    /* FC9C 8000F09C 26100002 */   addiu     $s0, $s0, 0x2
  .L8000F0A0:
    /* FCA0 8000F0A0 16350003 */  bne        $s1, $s5, .L8000F0B0
    /* FCA4 8000F0A4 00008025 */   or        $s0, $zero, $zero
    /* FCA8 8000F0A8 10000014 */  b          .L8000F0FC
    /* FCAC 8000F0AC 2402000A */   addiu     $v0, $zero, 0xA
  .L8000F0B0:
    /* FCB0 8000F0B0 27B40058 */  addiu      $s4, $sp, 0x58
  .L8000F0B4:
    /* FCB4 8000F0B4 1211000D */  beq        $s0, $s1, .L8000F0EC
    /* FCB8 8000F0B8 00106840 */   sll       $t5, $s0, 1
    /* FCBC 8000F0BC 028D7021 */  addu       $t6, $s4, $t5
    /* FCC0 8000F0C0 95C60000 */  lhu        $a2, 0x0($t6)
    /* FCC4 8000F0C4 8E640004 */  lw         $a0, 0x4($s3)
    /* FCC8 8000F0C8 8E650008 */  lw         $a1, 0x8($s3)
    /* FCCC 8000F0CC 240F0001 */  addiu      $t7, $zero, 0x1
    /* FCD0 8000F0D0 AFAF0010 */  sw         $t7, 0x10($sp)
    /* FCD4 8000F0D4 0C003DA0 */  jal        func_8000F680
    /* FCD8 8000F0D8 02403825 */   or        $a3, $s2, $zero
    /* FCDC 8000F0DC 50400004 */  beql       $v0, $zero, .L8000F0F0
    /* FCE0 8000F0E0 26100001 */   addiu     $s0, $s0, 0x1
    /* FCE4 8000F0E4 10000006 */  b          .L8000F100
    /* FCE8 8000F0E8 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8000F0EC:
    /* FCEC 8000F0EC 26100001 */  addiu      $s0, $s0, 0x1
  .L8000F0F0:
    /* FCF0 8000F0F0 1615FFF0 */  bne        $s0, $s5, .L8000F0B4
    /* FCF4 8000F0F4 00000000 */   nop
    /* FCF8 8000F0F8 00001025 */  or         $v0, $zero, $zero
  .L8000F0FC:
    /* FCFC 8000F0FC 8FBF003C */  lw         $ra, 0x3C($sp)
  .L8000F100:
    /* FD00 8000F100 8FB00020 */  lw         $s0, 0x20($sp)
    /* FD04 8000F104 8FB10024 */  lw         $s1, 0x24($sp)
    /* FD08 8000F108 8FB20028 */  lw         $s2, 0x28($sp)
    /* FD0C 8000F10C 8FB3002C */  lw         $s3, 0x2C($sp)
    /* FD10 8000F110 8FB40030 */  lw         $s4, 0x30($sp)
    /* FD14 8000F114 8FB50034 */  lw         $s5, 0x34($sp)
    /* FD18 8000F118 8FB60038 */  lw         $s6, 0x38($sp)
    /* FD1C 8000F11C 03E00008 */  jr         $ra
    /* FD20 8000F120 27BD0060 */   addiu     $sp, $sp, 0x60