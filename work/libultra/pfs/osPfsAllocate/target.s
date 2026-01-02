# Source: AF50.s
# Address: 0x8000A350

glabel func_8000A350
    /* AF50 8000A350 27BDFF88 */  addiu      $sp, $sp, -0x78
    /* AF54 8000A354 AFBF003C */  sw         $ra, 0x3C($sp)
    /* AF58 8000A358 AFBE0038 */  sw         $fp, 0x38($sp)
    /* AF5C 8000A35C AFB70034 */  sw         $s7, 0x34($sp)
    /* AF60 8000A360 AFB60030 */  sw         $s6, 0x30($sp)
    /* AF64 8000A364 AFB5002C */  sw         $s5, 0x2C($sp)
    /* AF68 8000A368 AFB40028 */  sw         $s4, 0x28($sp)
    /* AF6C 8000A36C AFB30024 */  sw         $s3, 0x24($sp)
    /* AF70 8000A370 AFB20020 */  sw         $s2, 0x20($sp)
    /* AF74 8000A374 AFB1001C */  sw         $s1, 0x1C($sp)
    /* AF78 8000A378 AFB00018 */  sw         $s0, 0x18($sp)
    /* AF7C 8000A37C AFA5007C */  sw         $a1, 0x7C($sp)
    /* AF80 8000A380 8C8E0000 */  lw         $t6, 0x0($a0)
    /* AF84 8000A384 00809025 */  or         $s2, $a0, $zero
    /* AF88 8000A388 00E0A825 */  or         $s5, $a3, $zero
    /* AF8C 8000A38C 31CF0001 */  andi       $t7, $t6, 0x1
    /* AF90 8000A390 30B7FFFF */  andi       $s7, $a1, 0xFFFF
    /* AF94 8000A394 15E00003 */  bnez       $t7, .L8000A3A4
    /* AF98 8000A398 00C0F025 */   or        $fp, $a2, $zero
    /* AF9C 8000A39C 10000051 */  b          .L8000A4E4
    /* AFA0 8000A3A0 24020005 */   addiu     $v0, $zero, 0x5
  .L8000A3A4:
    /* AFA4 8000A3A4 0C003CB4 */  jal        func_8000F2D0
    /* AFA8 8000A3A8 02402025 */   or        $a0, $s2, $zero
    /* AFAC 8000A3AC 50400004 */  beql       $v0, $zero, .L8000A3C0
    /* AFB0 8000A3B0 8E580050 */   lw        $t8, 0x50($s2)
    /* AFB4 8000A3B4 1000004C */  b          .L8000A4E8
    /* AFB8 8000A3B8 8FBF003C */   lw        $ra, 0x3C($sp)
    /* AFBC 8000A3BC 8E580050 */  lw         $t8, 0x50($s2)
  .L8000A3C0:
    /* AFC0 8000A3C0 00009825 */  or         $s3, $zero, $zero
    /* AFC4 8000A3C4 27B60050 */  addiu      $s6, $sp, 0x50
    /* AFC8 8000A3C8 1B000042 */  blez       $t8, .L8000A4D4
    /* AFCC 8000A3CC 8FB40088 */   lw        $s4, 0x88($sp)
    /* AFD0 8000A3D0 27B10060 */  addiu      $s1, $sp, 0x60
    /* AFD4 8000A3D4 27B00054 */  addiu      $s0, $sp, 0x54
    /* AFD8 8000A3D8 8E59005C */  lw         $t9, 0x5C($s2)
  .L8000A3DC:
    /* AFDC 8000A3DC 8E440004 */  lw         $a0, 0x4($s2)
    /* AFE0 8000A3E0 8E450008 */  lw         $a1, 0x8($s2)
    /* AFE4 8000A3E4 03333021 */  addu       $a2, $t9, $s3
    /* AFE8 8000A3E8 30C8FFFF */  andi       $t0, $a2, 0xFFFF
    /* AFEC 8000A3EC 01003025 */  or         $a2, $t0, $zero
    /* AFF0 8000A3F0 0C003A34 */  jal        func_8000E8D0
    /* AFF4 8000A3F4 02C03825 */   or        $a3, $s6, $zero
    /* AFF8 8000A3F8 50400004 */  beql       $v0, $zero, .L8000A40C
    /* AFFC 8000A3FC 8E440004 */   lw        $a0, 0x4($s2)
    /* B000 8000A400 10000039 */  b          .L8000A4E8
    /* B004 8000A404 8FBF003C */   lw        $ra, 0x3C($sp)
    /* B008 8000A408 8E440004 */  lw         $a0, 0x4($s2)
  .L8000A40C:
    /* B00C 8000A40C 0C003988 */  jal        func_8000E620
    /* B010 8000A410 8E450008 */   lw        $a1, 0x8($s2)
    /* B014 8000A414 10400003 */  beqz       $v0, .L8000A424
    /* B018 8000A418 00403025 */   or        $a2, $v0, $zero
    /* B01C 8000A41C 10000032 */  b          .L8000A4E8
    /* B020 8000A420 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8000A424:
    /* B024 8000A424 97A90054 */  lhu        $t1, 0x54($sp)
    /* B028 8000A428 8FAA0050 */  lw         $t2, 0x50($sp)
    /* B02C 8000A42C 56E90025 */  bnel       $s7, $t1, .L8000A4C4
    /* B030 8000A430 8E580050 */   lw        $t8, 0x50($s2)
    /* B034 8000A434 555E0023 */  bnel       $t2, $fp, .L8000A4C4
    /* B038 8000A438 8E580050 */   lw        $t8, 0x50($s2)
    /* B03C 8000A43C 12A0000C */  beqz       $s5, .L8000A470
    /* B040 8000A440 00002025 */   or        $a0, $zero, $zero
    /* B044 8000A444 27A20050 */  addiu      $v0, $sp, 0x50
    /* B048 8000A448 02A01825 */  or         $v1, $s5, $zero
  .L8000A44C:
    /* B04C 8000A44C 904B0010 */  lbu        $t3, 0x10($v0)
    /* B050 8000A450 906C0000 */  lbu        $t4, 0x0($v1)
    /* B054 8000A454 24420001 */  addiu      $v0, $v0, 0x1
    /* B058 8000A458 116C0003 */  beq        $t3, $t4, .L8000A468
    /* B05C 8000A45C 00000000 */   nop
    /* B060 8000A460 10000003 */  b          .L8000A470
    /* B064 8000A464 24040001 */   addiu     $a0, $zero, 0x1
  .L8000A468:
    /* B068 8000A468 1451FFF8 */  bne        $v0, $s1, .L8000A44C
    /* B06C 8000A46C 24630001 */   addiu     $v1, $v1, 0x1
  .L8000A470:
    /* B070 8000A470 1280000D */  beqz       $s4, .L8000A4A8
    /* B074 8000A474 00000000 */   nop
    /* B078 8000A478 1480000B */  bnez       $a0, .L8000A4A8
    /* B07C 8000A47C 27A20050 */   addiu     $v0, $sp, 0x50
    /* B080 8000A480 02801825 */  or         $v1, $s4, $zero
  .L8000A484:
    /* B084 8000A484 904D000C */  lbu        $t5, 0xC($v0)
    /* B088 8000A488 906E0000 */  lbu        $t6, 0x0($v1)
    /* B08C 8000A48C 24420001 */  addiu      $v0, $v0, 0x1
    /* B090 8000A490 11AE0003 */  beq        $t5, $t6, .L8000A4A0
    /* B094 8000A494 00000000 */   nop
    /* B098 8000A498 10000003 */  b          .L8000A4A8
    /* B09C 8000A49C 24040001 */   addiu     $a0, $zero, 0x1
  .L8000A4A0:
    /* B0A0 8000A4A0 1450FFF8 */  bne        $v0, $s0, .L8000A484
    /* B0A4 8000A4A4 24630001 */   addiu     $v1, $v1, 0x1
  .L8000A4A8:
    /* B0A8 8000A4A8 54800006 */  bnel       $a0, $zero, .L8000A4C4
    /* B0AC 8000A4AC 8E580050 */   lw        $t8, 0x50($s2)
    /* B0B0 8000A4B0 8FAF008C */  lw         $t7, 0x8C($sp)
    /* B0B4 8000A4B4 00C01025 */  or         $v0, $a2, $zero
    /* B0B8 8000A4B8 1000000A */  b          .L8000A4E4
    /* B0BC 8000A4BC ADF30000 */   sw        $s3, 0x0($t7)
    /* B0C0 8000A4C0 8E580050 */  lw         $t8, 0x50($s2)
  .L8000A4C4:
    /* B0C4 8000A4C4 26730001 */  addiu      $s3, $s3, 0x1
    /* B0C8 8000A4C8 0278082A */  slt        $at, $s3, $t8
    /* B0CC 8000A4CC 5420FFC3 */  bnel       $at, $zero, .L8000A3DC
    /* B0D0 8000A4D0 8E59005C */   lw        $t9, 0x5C($s2)
  .L8000A4D4:
    /* B0D4 8000A4D4 8FA8008C */  lw         $t0, 0x8C($sp)
    /* B0D8 8000A4D8 2419FFFF */  addiu      $t9, $zero, -0x1
    /* B0DC 8000A4DC 24020005 */  addiu      $v0, $zero, 0x5
    /* B0E0 8000A4E0 AD190000 */  sw         $t9, 0x0($t0)
  .L8000A4E4:
    /* B0E4 8000A4E4 8FBF003C */  lw         $ra, 0x3C($sp)
  .L8000A4E8:
    /* B0E8 8000A4E8 8FB00018 */  lw         $s0, 0x18($sp)
    /* B0EC 8000A4EC 8FB1001C */  lw         $s1, 0x1C($sp)
    /* B0F0 8000A4F0 8FB20020 */  lw         $s2, 0x20($sp)
    /* B0F4 8000A4F4 8FB30024 */  lw         $s3, 0x24($sp)
    /* B0F8 8000A4F8 8FB40028 */  lw         $s4, 0x28($sp)
    /* B0FC 8000A4FC 8FB5002C */  lw         $s5, 0x2C($sp)
    /* B100 8000A500 8FB60030 */  lw         $s6, 0x30($sp)
    /* B104 8000A504 8FB70034 */  lw         $s7, 0x34($sp)
    /* B108 8000A508 8FBE0038 */  lw         $fp, 0x38($sp)
    /* B10C 8000A50C 03E00008 */  jr         $ra
    /* B110 8000A510 27BD0078 */   addiu     $sp, $sp, 0x78