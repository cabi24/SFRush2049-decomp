# Source: BE40.s
# Address: 0x8000B240

glabel func_8000B240
    /* BE40 8000B240 27BDFEB0 */  addiu      $sp, $sp, -0x150
    /* BE44 8000B244 AFBF002C */  sw         $ra, 0x2C($sp)
    /* BE48 8000B248 AFB50028 */  sw         $s5, 0x28($sp)
    /* BE4C 8000B24C AFB40024 */  sw         $s4, 0x24($sp)
    /* BE50 8000B250 AFB30020 */  sw         $s3, 0x20($sp)
    /* BE54 8000B254 AFB2001C */  sw         $s2, 0x1C($sp)
    /* BE58 8000B258 AFB10018 */  sw         $s1, 0x18($sp)
    /* BE5C 8000B25C AFB00014 */  sw         $s0, 0x14($sp)
    /* BE60 8000B260 AFA50154 */  sw         $a1, 0x154($sp)
    /* BE64 8000B264 8C8E0000 */  lw         $t6, 0x0($a0)
    /* BE68 8000B268 0080A025 */  or         $s4, $a0, $zero
    /* BE6C 8000B26C 00008025 */  or         $s0, $zero, $zero
    /* BE70 8000B270 31CF0001 */  andi       $t7, $t6, 0x1
    /* BE74 8000B274 15E00003 */  bnez       $t7, .L8000B284
    /* BE78 8000B278 00000000 */   nop
    /* BE7C 8000B27C 1000004D */  b          .L8000B3B4
    /* BE80 8000B280 24020005 */   addiu     $v0, $zero, 0x5
  .L8000B284:
    /* BE84 8000B284 0C003CB4 */  jal        func_8000F2D0
    /* BE88 8000B288 02802025 */   or        $a0, $s4, $zero
    /* BE8C 8000B28C 50400004 */  beql       $v0, $zero, .L8000B2A0
    /* BE90 8000B290 92980064 */   lbu       $t8, 0x64($s4)
    /* BE94 8000B294 10000048 */  b          .L8000B3B8
    /* BE98 8000B298 8FBF002C */   lw        $ra, 0x2C($sp)
    /* BE9C 8000B29C 92980064 */  lbu        $t8, 0x64($s4)
  .L8000B2A0:
    /* BEA0 8000B2A0 00009825 */  or         $s3, $zero, $zero
    /* BEA4 8000B2A4 24150080 */  addiu      $s5, $zero, 0x80
    /* BEA8 8000B2A8 1B00003E */  blez       $t8, .L8000B3A4
    /* BEAC 8000B2AC 27B20148 */   addiu     $s2, $sp, 0x148
    /* BEB0 8000B2B0 24110003 */  addiu      $s1, $zero, 0x3
  .L8000B2B4:
    /* BEB4 8000B2B4 02802025 */  or         $a0, $s4, $zero
    /* BEB8 8000B2B8 27A50048 */  addiu      $a1, $sp, 0x48
    /* BEBC 8000B2BC 00003025 */  or         $a2, $zero, $zero
    /* BEC0 8000B2C0 0C003CE9 */  jal        func_8000F3A4
    /* BEC4 8000B2C4 326700FF */   andi      $a3, $s3, 0xFF
    /* BEC8 8000B2C8 10400003 */  beqz       $v0, .L8000B2D8
    /* BECC 8000B2CC 00000000 */   nop
    /* BED0 8000B2D0 10000039 */  b          .L8000B3B8
    /* BED4 8000B2D4 8FBF002C */   lw        $ra, 0x2C($sp)
  .L8000B2D8:
    /* BED8 8000B2D8 5A600004 */  blezl      $s3, .L8000B2EC
    /* BEDC 8000B2DC 8E850060 */   lw        $a1, 0x60($s4)
    /* BEE0 8000B2E0 10000002 */  b          .L8000B2EC
    /* BEE4 8000B2E4 24050001 */   addiu     $a1, $zero, 0x1
    /* BEE8 8000B2E8 8E850060 */  lw         $a1, 0x60($s4)
  .L8000B2EC:
    /* BEEC 8000B2EC 28A10080 */  slti       $at, $a1, 0x80
    /* BEF0 8000B2F0 10200026 */  beqz       $at, .L8000B38C
    /* BEF4 8000B2F4 00A01825 */   or        $v1, $a1, $zero
    /* BEF8 8000B2F8 02A53023 */  subu       $a2, $s5, $a1
    /* BEFC 8000B2FC 30D90003 */  andi       $t9, $a2, 0x3
    /* BF00 8000B300 1320000C */  beqz       $t9, .L8000B334
    /* BF04 8000B304 03252021 */   addu      $a0, $t9, $a1
    /* BF08 8000B308 00054040 */  sll        $t0, $a1, 1
    /* BF0C 8000B30C 27A90048 */  addiu      $t1, $sp, 0x48
    /* BF10 8000B310 01091021 */  addu       $v0, $t0, $t1
  .L8000B314:
    /* BF14 8000B314 944A0000 */  lhu        $t2, 0x0($v0)
    /* BF18 8000B318 24630001 */  addiu      $v1, $v1, 0x1
    /* BF1C 8000B31C 162A0002 */  bne        $s1, $t2, .L8000B328
    /* BF20 8000B320 00000000 */   nop
    /* BF24 8000B324 26100001 */  addiu      $s0, $s0, 0x1
  .L8000B328:
    /* BF28 8000B328 1483FFFA */  bne        $a0, $v1, .L8000B314
    /* BF2C 8000B32C 24420002 */   addiu     $v0, $v0, 0x2
    /* BF30 8000B330 10750016 */  beq        $v1, $s5, .L8000B38C
  .L8000B334:
    /* BF34 8000B334 00035840 */   sll       $t3, $v1, 1
    /* BF38 8000B338 27AC0048 */  addiu      $t4, $sp, 0x48
    /* BF3C 8000B33C 016C1021 */  addu       $v0, $t3, $t4
    /* BF40 8000B340 944D0000 */  lhu        $t5, 0x0($v0)
  .L8000B344:
    /* BF44 8000B344 562D0003 */  bnel       $s1, $t5, .L8000B354
    /* BF48 8000B348 944E0002 */   lhu       $t6, 0x2($v0)
    /* BF4C 8000B34C 26100001 */  addiu      $s0, $s0, 0x1
    /* BF50 8000B350 944E0002 */  lhu        $t6, 0x2($v0)
  .L8000B354:
    /* BF54 8000B354 562E0003 */  bnel       $s1, $t6, .L8000B364
    /* BF58 8000B358 944F0004 */   lhu       $t7, 0x4($v0)
    /* BF5C 8000B35C 26100001 */  addiu      $s0, $s0, 0x1
    /* BF60 8000B360 944F0004 */  lhu        $t7, 0x4($v0)
  .L8000B364:
    /* BF64 8000B364 562F0003 */  bnel       $s1, $t7, .L8000B374
    /* BF68 8000B368 94580006 */   lhu       $t8, 0x6($v0)
    /* BF6C 8000B36C 26100001 */  addiu      $s0, $s0, 0x1
    /* BF70 8000B370 94580006 */  lhu        $t8, 0x6($v0)
  .L8000B374:
    /* BF74 8000B374 24420008 */  addiu      $v0, $v0, 0x8
    /* BF78 8000B378 16380002 */  bne        $s1, $t8, .L8000B384
    /* BF7C 8000B37C 00000000 */   nop
    /* BF80 8000B380 26100001 */  addiu      $s0, $s0, 0x1
  .L8000B384:
    /* BF84 8000B384 5452FFEF */  bnel       $v0, $s2, .L8000B344
    /* BF88 8000B388 944D0000 */   lhu       $t5, 0x0($v0)
  .L8000B38C:
    /* BF8C 8000B38C 92880064 */  lbu        $t0, 0x64($s4)
    /* BF90 8000B390 26730001 */  addiu      $s3, $s3, 0x1
    /* BF94 8000B394 327900FF */  andi       $t9, $s3, 0xFF
    /* BF98 8000B398 0328082A */  slt        $at, $t9, $t0
    /* BF9C 8000B39C 1420FFC5 */  bnez       $at, .L8000B2B4
    /* BFA0 8000B3A0 03209825 */   or        $s3, $t9, $zero
  .L8000B3A4:
    /* BFA4 8000B3A4 8FAA0154 */  lw         $t2, 0x154($sp)
    /* BFA8 8000B3A8 00104A00 */  sll        $t1, $s0, 8
    /* BFAC 8000B3AC 00001025 */  or         $v0, $zero, $zero
    /* BFB0 8000B3B0 AD490000 */  sw         $t1, 0x0($t2)
  .L8000B3B4:
    /* BFB4 8000B3B4 8FBF002C */  lw         $ra, 0x2C($sp)
  .L8000B3B8:
    /* BFB8 8000B3B8 8FB00014 */  lw         $s0, 0x14($sp)
    /* BFBC 8000B3BC 8FB10018 */  lw         $s1, 0x18($sp)
    /* BFC0 8000B3C0 8FB2001C */  lw         $s2, 0x1C($sp)
    /* BFC4 8000B3C4 8FB30020 */  lw         $s3, 0x20($sp)
    /* BFC8 8000B3C8 8FB40024 */  lw         $s4, 0x24($sp)
    /* BFCC 8000B3CC 8FB50028 */  lw         $s5, 0x28($sp)
    /* BFD0 8000B3D0 03E00008 */  jr         $ra
    /* BFD4 8000B3D4 27BD0150 */   addiu     $sp, $sp, 0x150