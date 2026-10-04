nonmatching func_8001E0E0, 0x360

glabel func_8001E0E0
    /* 1ECE0 8001E0E0 3C08007F */  lui        $t0, (0x7F0001 >> 16)
    /* 1ECE4 8001E0E4 35080001 */  ori        $t0, $t0, (0x7F0001 & 0xFFFF)
    /* 1ECE8 8001E0E8 27BDFFF0 */  addiu      $sp, $sp, -0x10
    /* 1ECEC 8001E0EC 00C8082B */  sltu       $at, $a2, $t0
    /* 1ECF0 8001E0F0 F7B40008 */  sdc1       $fs0, 0x8($sp)
    /* 1ECF4 8001E0F4 14200002 */  bnez       $at, .L8001E100
    /* 1ECF8 8001E0F8 AFA40010 */   sw        $a0, 0x10($sp)
    /* 1ECFC 8001E0FC 3C06007F */  lui        $a2, (0x7F0000 >> 16)
  .L8001E100:
    /* 1ED00 8001E100 3C013780 */  lui        $at, (0x37800000 >> 16)
    /* 1ED04 8001E104 44818000 */  mtc1       $at, $ft4
    /* 1ED08 8001E108 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 1ED0C 8001E10C 30D8FFFF */  andi       $t8, $a2, 0xFFFF
    /* 1ED10 8001E110 44982000 */  mtc1       $t8, $ft0
    /* 1ED14 8001E114 44819000 */  mtc1       $at, $ft5
    /* 1ED18 8001E118 3C0A8003 */  lui        $t2, %hi(D_8002CA40)
    /* 1ED1C 8001E11C 00067402 */  srl        $t6, $a2, 16
    /* 1ED20 8001E120 254ACA40 */  addiu      $t2, $t2, %lo(D_8002CA40)
    /* 1ED24 8001E124 3C018003 */  lui        $at, %hi(D_8002D910)
    /* 1ED28 8001E128 3C0B8003 */  lui        $t3, %hi(D_8002CC44)
    /* 1ED2C 8001E12C 3C0C003F */  lui        $t4, (0x3FFFFF >> 16)
    /* 1ED30 8001E130 000E7880 */  sll        $t7, $t6, 2
    /* 1ED34 8001E134 358CFFFF */  ori        $t4, $t4, (0x3FFFFF & 0xFFFF)
    /* 1ED38 8001E138 256BCC44 */  addiu      $t3, $t3, %lo(D_8002CC44)
    /* 1ED3C 8001E13C C434D910 */  lwc1       $fs0, %lo(D_8002D910)($at)
    /* 1ED40 8001E140 8FA30020 */  lw         $v1, 0x20($sp)
    /* 1ED44 8001E144 3C09007F */  lui        $t1, (0x7F0000 >> 16)
    /* 1ED48 8001E148 3C0D0080 */  lui        $t5, (0x800000 >> 16)
    /* 1ED4C 8001E14C 014F1021 */  addu       $v0, $t2, $t7
    /* 1ED50 8001E150 07010005 */  bgez       $t8, .L8001E168
    /* 1ED54 8001E154 468021A0 */   cvt.s.w   $ft1, $ft0
    /* 1ED58 8001E158 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* 1ED5C 8001E15C 44814000 */  mtc1       $at, $ft2
    /* 1ED60 8001E160 00000000 */  nop
    /* 1ED64 8001E164 46083180 */  add.s      $ft1, $ft1, $ft2
  .L8001E168:
    /* 1ED68 8001E168 46103082 */  mul.s      $fv1, $ft1, $ft4
    /* 1ED6C 8001E16C C4440000 */  lwc1       $ft0, 0x0($v0)
    /* 1ED70 8001E170 C4460004 */  lwc1       $ft1, 0x4($v0)
    /* 1ED74 8001E174 006C7824 */  and        $t7, $v1, $t4
    /* 1ED78 8001E178 0003CD82 */  srl        $t9, $v1, 22
    /* 1ED7C 8001E17C 00197080 */  sll        $t6, $t9, 2
    /* 1ED80 8001E180 016E2021 */  addu       $a0, $t3, $t6
    /* 1ED84 8001E184 46029281 */  sub.s      $ft3, $ft5, $fv1
    /* 1ED88 8001E188 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* 1ED8C 8001E18C 46045202 */  mul.s      $ft2, $ft3, $ft0
    /* 1ED90 8001E190 448F2000 */  mtc1       $t7, $ft0
    /* 1ED94 8001E194 46023282 */  mul.s      $ft3, $ft1, $fv1
    /* 1ED98 8001E198 468021A0 */  cvt.s.w    $ft1, $ft0
    /* 1ED9C 8001E19C 05E10004 */  bgez       $t7, .L8001E1B0
    /* 1EDA0 8001E1A0 46085000 */   add.s     $fv0, $ft3, $ft2
    /* 1EDA4 8001E1A4 44815000 */  mtc1       $at, $ft3
    /* 1EDA8 8001E1A8 00000000 */  nop
    /* 1EDAC 8001E1AC 460A3180 */  add.s      $ft1, $ft1, $ft3
  .L8001E1B0:
    /* 1EDB0 8001E1B0 3C013480 */  lui        $at, (0x34800000 >> 16)
    /* 1EDB4 8001E1B4 44814000 */  mtc1       $at, $ft2
    /* 1EDB8 8001E1B8 C48A0000 */  lwc1       $ft3, 0x0($a0)
    /* 1EDBC 8001E1BC 3C018003 */  lui        $at, %hi(D_8002D914)
    /* 1EDC0 8001E1C0 46083302 */  mul.s      $fa0, $ft1, $ft2
    /* 1EDC4 8001E1C4 C4880004 */  lwc1       $ft2, 0x4($a0)
    /* 1EDC8 8001E1C8 8FAE0024 */  lw         $t6, 0x24($sp)
    /* 1EDCC 8001E1CC 01A31823 */  subu       $v1, $t5, $v1
    /* 1EDD0 8001E1D0 460C9101 */  sub.s      $ft0, $ft5, $fa0
    /* 1EDD4 8001E1D4 460A2182 */  mul.s      $ft1, $ft0, $ft3
    /* 1EDD8 8001E1D8 00000000 */  nop
    /* 1EDDC 8001E1DC 460C4102 */  mul.s      $ft0, $ft2, $fa0
    /* 1EDE0 8001E1E0 C428D914 */  lwc1       $ft2, %lo(D_8002D914)($at)
    /* 1EDE4 8001E1E4 006D082B */  sltu       $at, $v1, $t5
    /* 1EDE8 8001E1E8 46062380 */  add.s      $fa1, $ft0, $ft1
    /* 1EDEC 8001E1EC 460E0282 */  mul.s      $ft3, $fv0, $fa1
    /* 1EDF0 8001E1F0 00000000 */  nop
    /* 1EDF4 8001E1F4 46085102 */  mul.s      $ft0, $ft3, $ft2
    /* 1EDF8 8001E1F8 00000000 */  nop
    /* 1EDFC 8001E1FC 46142182 */  mul.s      $ft1, $ft0, $fs0
    /* 1EE00 8001E200 4600328D */  trunc.w.s  $ft3, $ft1
    /* 1EE04 8001E204 44195000 */  mfc1       $t9, $ft3
    /* 1EE08 8001E208 14200002 */  bnez       $at, .L8001E214
    /* 1EE0C 8001E20C A5D90000 */   sh        $t9, 0x0($t6)
    /* 1EE10 8001E210 01201825 */  or         $v1, $t1, $zero
  .L8001E214:
    /* 1EE14 8001E214 006CC824 */  and        $t9, $v1, $t4
    /* 1EE18 8001E218 44994000 */  mtc1       $t9, $ft2
    /* 1EE1C 8001E21C 00037D82 */  srl        $t7, $v1, 22
    /* 1EE20 8001E220 000FC080 */  sll        $t8, $t7, 2
    /* 1EE24 8001E224 01782021 */  addu       $a0, $t3, $t8
    /* 1EE28 8001E228 07210005 */  bgez       $t9, .L8001E240
    /* 1EE2C 8001E22C 46804120 */   cvt.s.w   $ft0, $ft2
    /* 1EE30 8001E230 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* 1EE34 8001E234 44813000 */  mtc1       $at, $ft1
    /* 1EE38 8001E238 00000000 */  nop
    /* 1EE3C 8001E23C 46062100 */  add.s      $ft0, $ft0, $ft1
  .L8001E240:
    /* 1EE40 8001E240 3C013480 */  lui        $at, (0x34800000 >> 16)
    /* 1EE44 8001E244 44815000 */  mtc1       $at, $ft3
    /* 1EE48 8001E248 C4860000 */  lwc1       $ft1, 0x0($a0)
    /* 1EE4C 8001E24C 00077D82 */  srl        $t7, $a3, 22
    /* 1EE50 8001E250 460A2302 */  mul.s      $fa0, $ft0, $ft3
    /* 1EE54 8001E254 C48A0004 */  lwc1       $ft3, 0x4($a0)
    /* 1EE58 8001E258 3C018003 */  lui        $at, %hi(D_8002D918)
    /* 1EE5C 8001E25C 000FC080 */  sll        $t8, $t7, 2
    /* 1EE60 8001E260 00ECC824 */  and        $t9, $a3, $t4
    /* 1EE64 8001E264 460C9201 */  sub.s      $ft2, $ft5, $fa0
    /* 1EE68 8001E268 46064102 */  mul.s      $ft0, $ft2, $ft1
    /* 1EE6C 8001E26C 00000000 */  nop
    /* 1EE70 8001E270 460C5202 */  mul.s      $ft2, $ft3, $fa0
    /* 1EE74 8001E274 46044380 */  add.s      $fa1, $ft2, $ft0
    /* 1EE78 8001E278 460E0002 */  mul.s      $fv0, $fv0, $fa1
    /* 1EE7C 8001E27C 14ED0013 */  bne        $a3, $t5, .L8001E2CC
    /* 1EE80 8001E280 00000000 */   nop
    /* 1EE84 8001E284 C426D918 */  lwc1       $ft1, %lo(D_8002D918)($at)
    /* 1EE88 8001E288 8FB80010 */  lw         $t8, 0x10($sp)
    /* 1EE8C 8001E28C 3C018003 */  lui        $at, %hi(D_8002D91C)
    /* 1EE90 8001E290 46060282 */  mul.s      $ft3, $fv0, $ft1
    /* 1EE94 8001E294 00000000 */  nop
    /* 1EE98 8001E298 46145202 */  mul.s      $ft2, $ft3, $fs0
    /* 1EE9C 8001E29C 4600410D */  trunc.w.s  $ft0, $ft2
    /* 1EEA0 8001E2A0 440F2000 */  mfc1       $t7, $ft0
    /* 1EEA4 8001E2A4 00000000 */  nop
    /* 1EEA8 8001E2A8 A70F0000 */  sh         $t7, 0x0($t8)
    /* 1EEAC 8001E2AC C426D91C */  lwc1       $ft1, %lo(D_8002D91C)($at)
    /* 1EEB0 8001E2B0 46060282 */  mul.s      $ft3, $fv0, $ft1
    /* 1EEB4 8001E2B4 00000000 */  nop
    /* 1EEB8 8001E2B8 46145202 */  mul.s      $ft2, $ft3, $fs0
    /* 1EEBC 8001E2BC 4600410D */  trunc.w.s  $ft0, $ft2
    /* 1EEC0 8001E2C0 440E2000 */  mfc1       $t6, $ft0
    /* 1EEC4 8001E2C4 1000003A */  b          .L8001E3B0
    /* 1EEC8 8001E2C8 A4AE0000 */   sh        $t6, 0x0($a1)
  .L8001E2CC:
    /* 1EECC 8001E2CC 44993000 */  mtc1       $t9, $ft1
    /* 1EED0 8001E2D0 01781021 */  addu       $v0, $t3, $t8
    /* 1EED4 8001E2D4 07210005 */  bgez       $t9, .L8001E2EC
    /* 1EED8 8001E2D8 468032A0 */   cvt.s.w   $ft3, $ft1
    /* 1EEDC 8001E2DC 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* 1EEE0 8001E2E0 44814000 */  mtc1       $at, $ft2
    /* 1EEE4 8001E2E4 00000000 */  nop
    /* 1EEE8 8001E2E8 46085280 */  add.s      $ft3, $ft3, $ft2
  .L8001E2EC:
    /* 1EEEC 8001E2EC 3C013480 */  lui        $at, (0x34800000 >> 16)
    /* 1EEF0 8001E2F0 44812000 */  mtc1       $at, $ft0
    /* 1EEF4 8001E2F4 C4480000 */  lwc1       $ft2, 0x0($v0)
    /* 1EEF8 8001E2F8 8FB80010 */  lw         $t8, 0x10($sp)
    /* 1EEFC 8001E2FC 46045082 */  mul.s      $fv1, $ft3, $ft0
    /* 1EF00 8001E300 C4440004 */  lwc1       $ft0, 0x4($v0)
    /* 1EF04 8001E304 01A73823 */  subu       $a3, $t5, $a3
    /* 1EF08 8001E308 00ED082B */  sltu       $at, $a3, $t5
    /* 1EF0C 8001E30C 46029181 */  sub.s      $ft1, $ft5, $fv1
    /* 1EF10 8001E310 46083282 */  mul.s      $ft3, $ft1, $ft2
    /* 1EF14 8001E314 00000000 */  nop
    /* 1EF18 8001E318 46022182 */  mul.s      $ft1, $ft0, $fv1
    /* 1EF1C 8001E31C 460A3380 */  add.s      $fa1, $ft1, $ft3
    /* 1EF20 8001E320 460E0202 */  mul.s      $ft2, $fv0, $fa1
    /* 1EF24 8001E324 00000000 */  nop
    /* 1EF28 8001E328 46144102 */  mul.s      $ft0, $ft2, $fs0
    /* 1EF2C 8001E32C 4600218D */  trunc.w.s  $ft1, $ft0
    /* 1EF30 8001E330 440F3000 */  mfc1       $t7, $ft1
    /* 1EF34 8001E334 14200002 */  bnez       $at, .L8001E340
    /* 1EF38 8001E338 A70F0000 */   sh        $t7, 0x0($t8)
    /* 1EF3C 8001E33C 01203825 */  or         $a3, $t1, $zero
  .L8001E340:
    /* 1EF40 8001E340 00EC7824 */  and        $t7, $a3, $t4
    /* 1EF44 8001E344 448F5000 */  mtc1       $t7, $ft3
    /* 1EF48 8001E348 0007CD82 */  srl        $t9, $a3, 22
    /* 1EF4C 8001E34C 00197080 */  sll        $t6, $t9, 2
    /* 1EF50 8001E350 016E1021 */  addu       $v0, $t3, $t6
    /* 1EF54 8001E354 05E10005 */  bgez       $t7, .L8001E36C
    /* 1EF58 8001E358 46805220 */   cvt.s.w   $ft2, $ft3
    /* 1EF5C 8001E35C 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* 1EF60 8001E360 44812000 */  mtc1       $at, $ft0
    /* 1EF64 8001E364 00000000 */  nop
    /* 1EF68 8001E368 46044200 */  add.s      $ft2, $ft2, $ft0
  .L8001E36C:
    /* 1EF6C 8001E36C 3C013480 */  lui        $at, (0x34800000 >> 16)
    /* 1EF70 8001E370 44813000 */  mtc1       $at, $ft1
    /* 1EF74 8001E374 C4440000 */  lwc1       $ft0, 0x0($v0)
    /* 1EF78 8001E378 46064082 */  mul.s      $fv1, $ft2, $ft1
    /* 1EF7C 8001E37C C4460004 */  lwc1       $ft1, 0x4($v0)
    /* 1EF80 8001E380 46029281 */  sub.s      $ft3, $ft5, $fv1
    /* 1EF84 8001E384 46045202 */  mul.s      $ft2, $ft3, $ft0
    /* 1EF88 8001E388 00000000 */  nop
    /* 1EF8C 8001E38C 46023282 */  mul.s      $ft3, $ft1, $fv1
    /* 1EF90 8001E390 46085380 */  add.s      $fa1, $ft3, $ft2
    /* 1EF94 8001E394 460E0102 */  mul.s      $ft0, $fv0, $fa1
    /* 1EF98 8001E398 00000000 */  nop
    /* 1EF9C 8001E39C 46142182 */  mul.s      $ft1, $ft0, $fs0
    /* 1EFA0 8001E3A0 4600328D */  trunc.w.s  $ft3, $ft1
    /* 1EFA4 8001E3A4 44195000 */  mfc1       $t9, $ft3
    /* 1EFA8 8001E3A8 00000000 */  nop
    /* 1EFAC 8001E3AC A4B90000 */  sh         $t9, 0x0($a1)
  .L8001E3B0:
    /* 1EFB0 8001E3B0 8FA20028 */  lw         $v0, 0x28($sp)
    /* 1EFB4 8001E3B4 0048082B */  sltu       $at, $v0, $t0
    /* 1EFB8 8001E3B8 54200003 */  bnel       $at, $zero, .L8001E3C8
    /* 1EFBC 8001E3BC 3058FFFF */   andi      $t8, $v0, 0xFFFF
    /* 1EFC0 8001E3C0 01201025 */  or         $v0, $t1, $zero
    /* 1EFC4 8001E3C4 3058FFFF */  andi       $t8, $v0, 0xFFFF
  .L8001E3C8:
    /* 1EFC8 8001E3C8 44984000 */  mtc1       $t8, $ft2
    /* 1EFCC 8001E3CC 00027402 */  srl        $t6, $v0, 16
    /* 1EFD0 8001E3D0 000E7880 */  sll        $t7, $t6, 2
    /* 1EFD4 8001E3D4 014F1821 */  addu       $v1, $t2, $t7
    /* 1EFD8 8001E3D8 07010005 */  bgez       $t8, .L8001E3F0
    /* 1EFDC 8001E3DC 46804120 */   cvt.s.w   $ft0, $ft2
    /* 1EFE0 8001E3E0 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* 1EFE4 8001E3E4 44813000 */  mtc1       $at, $ft1
    /* 1EFE8 8001E3E8 00000000 */  nop
    /* 1EFEC 8001E3EC 46062100 */  add.s      $ft0, $ft0, $ft1
  .L8001E3F0:
    /* 1EFF0 8001E3F0 46102082 */  mul.s      $fv1, $ft0, $ft4
    /* 1EFF4 8001E3F4 C4680000 */  lwc1       $ft2, 0x0($v1)
    /* 1EFF8 8001E3F8 C4640004 */  lwc1       $ft0, 0x4($v1)
    /* 1EFFC 8001E3FC 3C018003 */  lui        $at, %hi(D_8002D920)
    /* 1F000 8001E400 8FAF002C */  lw         $t7, 0x2C($sp)
    /* 1F004 8001E404 46029281 */  sub.s      $ft3, $ft5, $fv1
    /* 1F008 8001E408 46085182 */  mul.s      $ft1, $ft3, $ft2
    /* 1F00C 8001E40C C428D920 */  lwc1       $ft2, %lo(D_8002D920)($at)
    /* 1F010 8001E410 46022282 */  mul.s      $ft3, $ft0, $fv1
    /* 1F014 8001E414 46065000 */  add.s      $fv0, $ft3, $ft1
    /* 1F018 8001E418 46080102 */  mul.s      $ft0, $fv0, $ft2
    /* 1F01C 8001E41C 00000000 */  nop
    /* 1F020 8001E420 46142282 */  mul.s      $ft3, $ft0, $fs0
    /* 1F024 8001E424 4600518D */  trunc.w.s  $ft1, $ft3
    /* 1F028 8001E428 440E3000 */  mfc1       $t6, $ft1
    /* 1F02C 8001E42C 00000000 */  nop
    /* 1F030 8001E430 A5EE0000 */  sh         $t6, 0x0($t7)
    /* 1F034 8001E434 D7B40008 */  ldc1       $fs0, 0x8($sp)
    /* 1F038 8001E438 03E00008 */  jr         $ra
    /* 1F03C 8001E43C 27BD0010 */   addiu     $sp, $sp, 0x10
endlabel func_8001E0E0
