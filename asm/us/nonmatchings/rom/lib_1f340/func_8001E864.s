nonmatching func_8001E864, 0xCC

glabel func_8001E864
    /* 1F464 8001E864 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* 1F468 8001E868 AFB6002C */  sw         $s6, 0x2C($sp)
    /* 1F46C 8001E86C AFB50028 */  sw         $s5, 0x28($sp)
    /* 1F470 8001E870 AFB40024 */  sw         $s4, 0x24($sp)
    /* 1F474 8001E874 00A0A025 */  or         $s4, $a1, $zero
    /* 1F478 8001E878 00E0A825 */  or         $s5, $a3, $zero
    /* 1F47C 8001E87C 0080B025 */  or         $s6, $a0, $zero
    /* 1F480 8001E880 AFBF0034 */  sw         $ra, 0x34($sp)
    /* 1F484 8001E884 AFB70030 */  sw         $s7, 0x30($sp)
    /* 1F488 8001E888 AFB30020 */  sw         $s3, 0x20($sp)
    /* 1F48C 8001E88C AFB2001C */  sw         $s2, 0x1C($sp)
    /* 1F490 8001E890 AFB10018 */  sw         $s1, 0x18($sp)
    /* 1F494 8001E894 10C0001A */  beqz       $a2, .L8001E900
    /* 1F498 8001E898 AFB00014 */   sw        $s0, 0x14($sp)
    /* 1F49C 8001E89C 24120001 */  addiu      $s2, $zero, 0x1
    /* 1F4A0 8001E8A0 00C09825 */  or         $s3, $a2, $zero
    /* 1F4A4 8001E8A4 8FB70048 */  lw         $s7, 0x48($sp)
    /* 1F4A8 8001E8A8 02531021 */  addu       $v0, $s2, $s3
  .L8001E8AC:
    /* 1F4AC 8001E8AC 00021043 */  sra        $v0, $v0, 1
    /* 1F4B0 8001E8B0 244EFFFF */  addiu      $t6, $v0, -0x1
    /* 1F4B4 8001E8B4 02AE0019 */  multu      $s5, $t6
    /* 1F4B8 8001E8B8 00408025 */  or         $s0, $v0, $zero
    /* 1F4BC 8001E8BC 02C02025 */  or         $a0, $s6, $zero
    /* 1F4C0 8001E8C0 00007812 */  mflo       $t7
    /* 1F4C4 8001E8C4 028F8821 */  addu       $s1, $s4, $t7
    /* 1F4C8 8001E8C8 02E0F809 */  jalr       $s7
    /* 1F4CC 8001E8CC 02202825 */   or        $a1, $s1, $zero
    /* 1F4D0 8001E8D0 14400003 */  bnez       $v0, .L8001E8E0
    /* 1F4D4 8001E8D4 00000000 */   nop
    /* 1F4D8 8001E8D8 1000000A */  b          .L8001E904
    /* 1F4DC 8001E8DC 02201025 */   or        $v0, $s1, $zero
  .L8001E8E0:
    /* 1F4E0 8001E8E0 04430004 */  bgezl      $v0, .L8001E8F4
    /* 1F4E4 8001E8E4 26120001 */   addiu     $s2, $s0, 0x1
    /* 1F4E8 8001E8E8 10000002 */  b          .L8001E8F4
    /* 1F4EC 8001E8EC 2613FFFF */   addiu     $s3, $s0, -0x1
    /* 1F4F0 8001E8F0 26120001 */  addiu      $s2, $s0, 0x1
  .L8001E8F4:
    /* 1F4F4 8001E8F4 0272082A */  slt        $at, $s3, $s2
    /* 1F4F8 8001E8F8 5020FFEC */  beql       $at, $zero, .L8001E8AC
    /* 1F4FC 8001E8FC 02531021 */   addu      $v0, $s2, $s3
  .L8001E900:
    /* 1F500 8001E900 00001025 */  or         $v0, $zero, $zero
  .L8001E904:
    /* 1F504 8001E904 8FBF0034 */  lw         $ra, 0x34($sp)
    /* 1F508 8001E908 8FB00014 */  lw         $s0, 0x14($sp)
    /* 1F50C 8001E90C 8FB10018 */  lw         $s1, 0x18($sp)
    /* 1F510 8001E910 8FB2001C */  lw         $s2, 0x1C($sp)
    /* 1F514 8001E914 8FB30020 */  lw         $s3, 0x20($sp)
    /* 1F518 8001E918 8FB40024 */  lw         $s4, 0x24($sp)
    /* 1F51C 8001E91C 8FB50028 */  lw         $s5, 0x28($sp)
    /* 1F520 8001E920 8FB6002C */  lw         $s6, 0x2C($sp)
    /* 1F524 8001E924 8FB70030 */  lw         $s7, 0x30($sp)
    /* 1F528 8001E928 03E00008 */  jr         $ra
    /* 1F52C 8001E92C 27BD0038 */   addiu     $sp, $sp, 0x38
endlabel func_8001E864
