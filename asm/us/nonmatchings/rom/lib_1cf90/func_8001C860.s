nonmatching func_8001C860, 0x43C

glabel func_8001C860
    /* 1D460 8001C860 27BDFF38 */  addiu      $sp, $sp, -0xC8
    /* 1D464 8001C864 F7BE0040 */  sdc1       $fs5, 0x40($sp)
    /* 1D468 8001C868 4480F000 */  mtc1       $zero, $fs5
    /* 1D46C 8001C86C 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 1D470 8001C870 44812000 */  mtc1       $at, $ft0
    /* 1D474 8001C874 AFB00048 */  sw         $s0, 0x48($sp)
    /* 1D478 8001C878 AFBF0064 */  sw         $ra, 0x64($sp)
    /* 1D47C 8001C87C AFB60060 */  sw         $s6, 0x60($sp)
    /* 1D480 8001C880 AFB5005C */  sw         $s5, 0x5C($sp)
    /* 1D484 8001C884 AFB40058 */  sw         $s4, 0x58($sp)
    /* 1D488 8001C888 AFB30054 */  sw         $s3, 0x54($sp)
    /* 1D48C 8001C88C AFB20050 */  sw         $s2, 0x50($sp)
    /* 1D490 8001C890 AFB1004C */  sw         $s1, 0x4C($sp)
    /* 1D494 8001C894 F7BC0038 */  sdc1       $fs4, 0x38($sp)
    /* 1D498 8001C898 F7BA0030 */  sdc1       $fs3, 0x30($sp)
    /* 1D49C 8001C89C F7B80028 */  sdc1       $fs2, 0x28($sp)
    /* 1D4A0 8001C8A0 F7B60020 */  sdc1       $fs1, 0x20($sp)
    /* 1D4A4 8001C8A4 F7B40018 */  sdc1       $fs0, 0x18($sp)
    /* 1D4A8 8001C8A8 AFA700D4 */  sw         $a3, 0xD4($sp)
    /* 1D4AC 8001C8AC E4BE0000 */  swc1       $fs5, 0x0($a1)
    /* 1D4B0 8001C8B0 3C108005 */  lui        $s0, %hi(D_8004FD54)
    /* 1D4B4 8001C8B4 E4C40000 */  swc1       $ft0, 0x0($a2)
    /* 1D4B8 8001C8B8 8E10FD54 */  lw         $s0, %lo(D_8004FD54)($s0)
    /* 1D4BC 8001C8BC 00808825 */  or         $s1, $a0, $zero
    /* 1D4C0 8001C8C0 00A0A025 */  or         $s4, $a1, $zero
    /* 1D4C4 8001C8C4 00C0A825 */  or         $s5, $a2, $zero
    /* 1D4C8 8001C8C8 00009025 */  or         $s2, $zero, $zero
    /* 1D4CC 8001C8CC E7BE0080 */  swc1       $fs5, 0x80($sp)
    /* 1D4D0 8001C8D0 E7BE0084 */  swc1       $fs5, 0x84($sp)
    /* 1D4D4 8001C8D4 120000CE */  beqz       $s0, .L8001CC10
    /* 1D4D8 8001C8D8 4600F706 */   mov.s     $fs4, $fs5
    /* 1D4DC 8001C8DC 3C018003 */  lui        $at, %hi(D_8002D900)
    /* 1D4E0 8001C8E0 4480D800 */  mtc1       $zero, $fs3f
    /* 1D4E4 8001C8E4 4480D000 */  mtc1       $zero, $fs3
    /* 1D4E8 8001C8E8 C438D900 */  lwc1       $fs2, %lo(D_8002D900)($at)
    /* 1D4EC 8001C8EC 3C160008 */  lui        $s6, (0x80000 >> 16)
    /* 1D4F0 8001C8F0 27B300A0 */  addiu      $s3, $sp, 0xA0
    /* 1D4F4 8001C8F4 C626000C */  lwc1       $ft1, 0xC($s1)
  .L8001C8F8:
    /* 1D4F8 8001C8F8 C608000C */  lwc1       $ft2, 0xC($s0)
    /* 1D4FC 8001C8FC C62A0010 */  lwc1       $ft3, 0x10($s1)
    /* 1D500 8001C900 C6040010 */  lwc1       $ft0, 0x10($s0)
    /* 1D504 8001C904 46083001 */  sub.s      $fv0, $ft1, $ft2
    /* 1D508 8001C908 C6080014 */  lwc1       $ft2, 0x14($s0)
    /* 1D50C 8001C90C C6260014 */  lwc1       $ft1, 0x14($s1)
    /* 1D510 8001C910 46045081 */  sub.s      $fv1, $ft3, $ft0
    /* 1D514 8001C914 46000282 */  mul.s      $ft3, $fv0, $fv0
    /* 1D518 8001C918 46083381 */  sub.s      $fa1, $ft1, $ft2
    /* 1D51C 8001C91C 46021102 */  mul.s      $ft0, $fv1, $fv1
    /* 1D520 8001C920 46045180 */  add.s      $ft1, $ft3, $ft0
    /* 1D524 8001C924 460E7202 */  mul.s      $ft2, $fa1, $fa1
    /* 1D528 8001C928 0C0038F0 */  jal        sqrtf
    /* 1D52C 8001C92C 46083300 */   add.s     $fa0, $ft1, $ft2
    /* 1D530 8001C930 C6300024 */  lwc1       $ft4, 0x24($s1)
    /* 1D534 8001C934 46000586 */  mov.s      $fs1, $fv0
    /* 1D538 8001C938 4610003E */  c.le.s     $fv0, $ft4
    /* 1D53C 8001C93C 00000000 */  nop
    /* 1D540 8001C940 450200B0 */  bc1fl      .L8001CC04
    /* 1D544 8001C944 8E100000 */   lw        $s0, 0x0($s0)
    /* 1D548 8001C948 46100083 */  div.s      $fv1, $fv0, $ft4
    /* 1D54C 8001C94C 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 1D550 8001C950 44819000 */  mtc1       $at, $ft5
    /* 1D554 8001C954 C62E0030 */  lwc1       $fa1, 0x30($s1)
    /* 1D558 8001C958 C62C002C */  lwc1       $fa0, 0x2C($s1)
    /* 1D55C 8001C95C 460E9281 */  sub.s      $ft3, $ft5, $fa1
    /* 1D560 8001C960 46025102 */  mul.s      $ft0, $ft3, $fv1
    /* 1D564 8001C964 00000000 */  nop
    /* 1D568 8001C968 46027182 */  mul.s      $ft1, $fa1, $fv1
    /* 1D56C 8001C96C 00000000 */  nop
    /* 1D570 8001C970 46023202 */  mul.s      $ft2, $ft1, $fv1
    /* 1D574 8001C974 46082280 */  add.s      $ft3, $ft0, $ft2
    /* 1D578 8001C978 C6240028 */  lwc1       $ft0, 0x28($s1)
    /* 1D57C 8001C97C 460A9181 */  sub.s      $ft1, $ft5, $ft3
    /* 1D580 8001C980 460C2201 */  sub.s      $ft2, $ft0, $fa0
    /* 1D584 8001C984 46064282 */  mul.s      $ft3, $ft2, $ft1
    /* 1D588 8001C988 C6080084 */  lwc1       $ft2, 0x84($s0)
    /* 1D58C 8001C98C 460A6100 */  add.s      $ft0, $fa0, $ft3
    /* 1D590 8001C990 C68A0000 */  lwc1       $ft3, 0x0($s4)
    /* 1D594 8001C994 46044182 */  mul.s      $ft1, $ft2, $ft0
    /* 1D598 8001C998 46065200 */  add.s      $ft2, $ft3, $ft1
    /* 1D59C 8001C99C E6880000 */  swc1       $ft2, 0x0($s4)
    /* 1D5A0 8001C9A0 8E220008 */  lw         $v0, 0x8($s1)
    /* 1D5A4 8001C9A4 00567024 */  and        $t6, $v0, $s6
    /* 1D5A8 8001C9A8 15C00095 */  bnez       $t6, .L8001CC00
    /* 1D5AC 8001C9AC 304F0008 */   andi      $t7, $v0, 0x8
    /* 1D5B0 8001C9B0 55E00006 */  bnel       $t7, $zero, .L8001C9CC
    /* 1D5B4 8001C9B4 C6040018 */   lwc1      $ft0, 0x18($s0)
    /* 1D5B8 8001C9B8 8E180008 */  lw         $t8, 0x8($s0)
    /* 1D5BC 8001C9BC 33190001 */  andi       $t9, $t8, 0x1
    /* 1D5C0 8001C9C0 53200044 */  beql       $t9, $zero, .L8001CAD4
    /* 1D5C4 8001C9C4 461EB032 */   c.eq.s    $fs1, $fs5
    /* 1D5C8 8001C9C8 C6040018 */  lwc1       $ft0, 0x18($s0)
  .L8001C9CC:
    /* 1D5CC 8001C9CC C62A0018 */  lwc1       $ft3, 0x18($s1)
    /* 1D5D0 8001C9D0 C606001C */  lwc1       $ft1, 0x1C($s0)
    /* 1D5D4 8001C9D4 C628001C */  lwc1       $ft2, 0x1C($s1)
    /* 1D5D8 8001C9D8 460A2001 */  sub.s      $fv0, $ft0, $ft3
    /* 1D5DC 8001C9DC C62A0020 */  lwc1       $ft3, 0x20($s1)
    /* 1D5E0 8001C9E0 C6040020 */  lwc1       $ft0, 0x20($s0)
    /* 1D5E4 8001C9E4 46083081 */  sub.s      $fv1, $ft1, $ft2
    /* 1D5E8 8001C9E8 46000182 */  mul.s      $ft1, $fv0, $fv0
    /* 1D5EC 8001C9EC 460A2381 */  sub.s      $fa1, $ft0, $ft3
    /* 1D5F0 8001C9F0 46021202 */  mul.s      $ft2, $fv1, $fv1
    /* 1D5F4 8001C9F4 46083100 */  add.s      $ft0, $ft1, $ft2
    /* 1D5F8 8001C9F8 460E7282 */  mul.s      $ft3, $fa1, $fa1
    /* 1D5FC 8001C9FC 0C0038F0 */  jal        sqrtf
    /* 1D600 8001CA00 460A2300 */   add.s     $fa0, $ft0, $ft3
    /* 1D604 8001CA04 4600F03C */  c.lt.s     $fs5, $fv0
    /* 1D608 8001CA08 46000506 */  mov.s      $fs0, $fv0
    /* 1D60C 8001CA0C 45020031 */  bc1fl      .L8001CAD4
    /* 1D610 8001CA10 461EB032 */   c.eq.s    $fs1, $fs5
    /* 1D614 8001CA14 C6280018 */  lwc1       $ft2, 0x18($s1)
    /* 1D618 8001CA18 C626000C */  lwc1       $ft1, 0xC($s1)
    /* 1D61C 8001CA1C 46184102 */  mul.s      $ft0, $ft2, $fs2
    /* 1D620 8001CA20 C608000C */  lwc1       $ft2, 0xC($s0)
    /* 1D624 8001CA24 46043280 */  add.s      $ft3, $ft1, $ft0
    /* 1D628 8001CA28 C6060018 */  lwc1       $ft1, 0x18($s0)
    /* 1D62C 8001CA2C 46183102 */  mul.s      $ft0, $ft1, $fs2
    /* 1D630 8001CA30 46044180 */  add.s      $ft1, $ft2, $ft0
    /* 1D634 8001CA34 C624001C */  lwc1       $ft0, 0x1C($s1)
    /* 1D638 8001CA38 C6280010 */  lwc1       $ft2, 0x10($s1)
    /* 1D63C 8001CA3C 46065001 */  sub.s      $fv0, $ft3, $ft1
    /* 1D640 8001CA40 46182282 */  mul.s      $ft3, $ft0, $fs2
    /* 1D644 8001CA44 C6040010 */  lwc1       $ft0, 0x10($s0)
    /* 1D648 8001CA48 460A4180 */  add.s      $ft1, $ft2, $ft3
    /* 1D64C 8001CA4C C608001C */  lwc1       $ft2, 0x1C($s0)
    /* 1D650 8001CA50 46184282 */  mul.s      $ft3, $ft2, $fs2
    /* 1D654 8001CA54 460A2200 */  add.s      $ft2, $ft0, $ft3
    /* 1D658 8001CA58 C62A0020 */  lwc1       $ft3, 0x20($s1)
    /* 1D65C 8001CA5C C6240014 */  lwc1       $ft0, 0x14($s1)
    /* 1D660 8001CA60 46083081 */  sub.s      $fv1, $ft1, $ft2
    /* 1D664 8001CA64 46185182 */  mul.s      $ft1, $ft3, $fs2
    /* 1D668 8001CA68 C60A0014 */  lwc1       $ft3, 0x14($s0)
    /* 1D66C 8001CA6C 46062200 */  add.s      $ft2, $ft0, $ft1
    /* 1D670 8001CA70 C6040020 */  lwc1       $ft0, 0x20($s0)
    /* 1D674 8001CA74 46182182 */  mul.s      $ft1, $ft0, $fs2
    /* 1D678 8001CA78 46065100 */  add.s      $ft0, $ft3, $ft1
    /* 1D67C 8001CA7C 46000282 */  mul.s      $ft3, $fv0, $fv0
    /* 1D680 8001CA80 00000000 */  nop
    /* 1D684 8001CA84 46021182 */  mul.s      $ft1, $fv1, $fv1
    /* 1D688 8001CA88 46044381 */  sub.s      $fa1, $ft2, $ft0
    /* 1D68C 8001CA8C 460E7102 */  mul.s      $ft0, $fa1, $fa1
    /* 1D690 8001CA90 46065200 */  add.s      $ft2, $ft3, $ft1
    /* 1D694 8001CA94 0C0038F0 */  jal        sqrtf
    /* 1D698 8001CA98 46044300 */   add.s     $fa0, $ft2, $ft0
    /* 1D69C 8001CA9C 4616003C */  c.lt.s     $fv0, $fs1
    /* 1D6A0 8001CAA0 00000000 */  nop
    /* 1D6A4 8001CAA4 45020007 */  bc1fl      .L8001CAC4
    /* 1D6A8 8001CAA8 C6000080 */   lwc1      $fv0, 0x80($s0)
    /* 1D6AC 8001CAAC C6000080 */  lwc1       $fv0, 0x80($s0)
    /* 1D6B0 8001CAB0 46140281 */  sub.s      $ft3, $fv0, $fs0
    /* 1D6B4 8001CAB4 460A0183 */  div.s      $ft1, $fv0, $ft3
    /* 1D6B8 8001CAB8 10000005 */  b          .L8001CAD0
    /* 1D6BC 8001CABC E6A60000 */   swc1      $ft1, 0x0($s5)
    /* 1D6C0 8001CAC0 C6000080 */  lwc1       $fv0, 0x80($s0)
  .L8001CAC4:
    /* 1D6C4 8001CAC4 46140200 */  add.s      $ft2, $fv0, $fs0
    /* 1D6C8 8001CAC8 46080103 */  div.s      $ft0, $fv0, $ft2
    /* 1D6CC 8001CACC E6A40000 */  swc1       $ft0, 0x0($s5)
  .L8001CAD0:
    /* 1D6D0 8001CAD0 461EB032 */  c.eq.s     $fs1, $fs5
  .L8001CAD4:
    /* 1D6D4 8001CAD4 26040048 */  addiu      $a0, $s0, 0x48
    /* 1D6D8 8001CAD8 2625000C */  addiu      $a1, $s1, 0xC
    /* 1D6DC 8001CADC 45030049 */  bc1tl      .L8001CC04
    /* 1D6E0 8001CAE0 8E100000 */   lw        $s0, 0x0($s0)
    /* 1D6E4 8001CAE4 0C0092FC */  jal        func_80024BF0
    /* 1D6E8 8001CAE8 02603025 */   or        $a2, $s3, $zero
    /* 1D6EC 8001CAEC C7AA00A8 */  lwc1       $ft3, 0xA8($sp)
    /* 1D6F0 8001CAF0 460AF03E */  c.le.s     $fs5, $ft3
    /* 1D6F4 8001CAF4 00000000 */  nop
    /* 1D6F8 8001CAF8 45020014 */  bc1fl      .L8001CB4C
    /* 1D6FC 8001CAFC C602007C */   lwc1      $fv1, 0x7C($s0)
    /* 1D700 8001CB00 C6020078 */  lwc1       $fv1, 0x78($s0)
    /* 1D704 8001CB04 3C013FF0 */  lui        $at, (0x3FF00000 >> 16)
    /* 1D708 8001CB08 4602503C */  c.lt.s     $ft3, $fv1
    /* 1D70C 8001CB0C 00000000 */  nop
    /* 1D710 8001CB10 45020008 */  bc1fl      .L8001CB34
    /* 1D714 8001CB14 44810800 */   mtc1      $at, $fv0f
    /* 1D718 8001CB18 46025183 */  div.s      $ft1, $ft3, $fv1
    /* 1D71C 8001CB1C 4600E221 */  cvt.d.s    $ft2, $fs4
    /* 1D720 8001CB20 46003021 */  cvt.d.s    $fv0, $ft1
    /* 1D724 8001CB24 46204100 */  add.d      $ft0, $ft2, $fv0
    /* 1D728 8001CB28 10000018 */  b          .L8001CB8C
    /* 1D72C 8001CB2C 46202720 */   cvt.s.d   $fs4, $ft0
    /* 1D730 8001CB30 44810800 */  mtc1       $at, $fv0f
  .L8001CB34:
    /* 1D734 8001CB34 44800000 */  mtc1       $zero, $fv0
    /* 1D738 8001CB38 4600E221 */  cvt.d.s    $ft2, $fs4
    /* 1D73C 8001CB3C 46204100 */  add.d      $ft0, $ft2, $fv0
    /* 1D740 8001CB40 10000012 */  b          .L8001CB8C
    /* 1D744 8001CB44 46202720 */   cvt.s.d   $fs4, $ft0
    /* 1D748 8001CB48 C602007C */  lwc1       $fv1, 0x7C($s0)
  .L8001CB4C:
    /* 1D74C 8001CB4C C7A600A8 */  lwc1       $ft1, 0xA8($sp)
    /* 1D750 8001CB50 4600E121 */  cvt.d.s    $ft0, $fs4
    /* 1D754 8001CB54 46001287 */  neg.s      $ft3, $fv1
    /* 1D758 8001CB58 3C01BFF0 */  lui        $at, (0xBFF00000 >> 16)
    /* 1D75C 8001CB5C 4606503C */  c.lt.s     $ft3, $ft1
    /* 1D760 8001CB60 00000000 */  nop
    /* 1D764 8001CB64 45020005 */  bc1fl      .L8001CB7C
    /* 1D768 8001CB68 44810800 */   mtc1      $at, $fv0f
    /* 1D76C 8001CB6C 46023203 */  div.s      $ft2, $ft1, $fv1
    /* 1D770 8001CB70 10000004 */  b          .L8001CB84
    /* 1D774 8001CB74 46004021 */   cvt.d.s   $fv0, $ft2
    /* 1D778 8001CB78 44810800 */  mtc1       $at, $fv0f
  .L8001CB7C:
    /* 1D77C 8001CB7C 44800000 */  mtc1       $zero, $fv0
    /* 1D780 8001CB80 00000000 */  nop
  .L8001CB84:
    /* 1D784 8001CB84 46202280 */  add.d      $ft3, $ft0, $fv0
    /* 1D788 8001CB88 46205720 */  cvt.s.d    $fs4, $ft3
  .L8001CB8C:
    /* 1D78C 8001CB8C C7A600A0 */  lwc1       $ft1, 0xA0($sp)
    /* 1D790 8001CB90 C7A400A4 */  lwc1       $ft0, 0xA4($sp)
    /* 1D794 8001CB94 46003221 */  cvt.d.s    $ft2, $ft1
    /* 1D798 8001CB98 4628D032 */  c.eq.d     $fs3, $ft2
    /* 1D79C 8001CB9C 00000000 */  nop
    /* 1D7A0 8001CBA0 4500000D */  bc1f       .L8001CBD8
    /* 1D7A4 8001CBA4 00000000 */   nop
    /* 1D7A8 8001CBA8 460022A1 */  cvt.d.s    $ft3, $ft0
    /* 1D7AC 8001CBAC C7A600A8 */  lwc1       $ft1, 0xA8($sp)
    /* 1D7B0 8001CBB0 462AD032 */  c.eq.d     $fs3, $ft3
    /* 1D7B4 8001CBB4 00000000 */  nop
    /* 1D7B8 8001CBB8 45000007 */  bc1f       .L8001CBD8
    /* 1D7BC 8001CBBC 00000000 */   nop
    /* 1D7C0 8001CBC0 44804000 */  mtc1       $zero, $ft2
    /* 1D7C4 8001CBC4 00000000 */  nop
    /* 1D7C8 8001CBC8 46083032 */  c.eq.s     $ft1, $ft2
    /* 1D7CC 8001CBCC 00000000 */  nop
    /* 1D7D0 8001CBD0 45030004 */  bc1tl      .L8001CBE4
    /* 1D7D4 8001CBD4 C7A40080 */   lwc1      $ft0, 0x80($sp)
  .L8001CBD8:
    /* 1D7D8 8001CBD8 0C009327 */  jal        func_80024C9C
    /* 1D7DC 8001CBDC 02602025 */   or        $a0, $s3, $zero
    /* 1D7E0 8001CBE0 C7A40080 */  lwc1       $ft0, 0x80($sp)
  .L8001CBE4:
    /* 1D7E4 8001CBE4 C7AA00A0 */  lwc1       $ft3, 0xA0($sp)
    /* 1D7E8 8001CBE8 C7A80084 */  lwc1       $ft2, 0x84($sp)
    /* 1D7EC 8001CBEC 460A2180 */  add.s      $ft1, $ft0, $ft3
    /* 1D7F0 8001CBF0 C7A400A4 */  lwc1       $ft0, 0xA4($sp)
    /* 1D7F4 8001CBF4 46044280 */  add.s      $ft3, $ft2, $ft0
    /* 1D7F8 8001CBF8 E7A60080 */  swc1       $ft1, 0x80($sp)
    /* 1D7FC 8001CBFC E7AA0084 */  swc1       $ft3, 0x84($sp)
  .L8001CC00:
    /* 1D800 8001CC00 8E100000 */  lw         $s0, 0x0($s0)
  .L8001CC04:
    /* 1D804 8001CC04 26520001 */  addiu      $s2, $s2, 0x1
    /* 1D808 8001CC08 5600FF3B */  bnel       $s0, $zero, .L8001C8F8
    /* 1D80C 8001CC0C C626000C */   lwc1      $ft1, 0xC($s1)
  .L8001CC10:
    /* 1D810 8001CC10 12400012 */  beqz       $s2, .L8001CC5C
    /* 1D814 8001CC14 C7A40080 */   lwc1      $ft0, 0x80($sp)
    /* 1D818 8001CC18 44923000 */  mtc1       $s2, $ft1
    /* 1D81C 8001CC1C 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* 1D820 8001CC20 06410004 */  bgez       $s2, .L8001CC34
    /* 1D824 8001CC24 46803020 */   cvt.s.w   $fv0, $ft1
    /* 1D828 8001CC28 44814000 */  mtc1       $at, $ft2
    /* 1D82C 8001CC2C 00000000 */  nop
    /* 1D830 8001CC30 46080000 */  add.s      $fv0, $fv0, $ft2
  .L8001CC34:
    /* 1D834 8001CC34 46002283 */  div.s      $ft3, $ft0, $fv0
    /* 1D838 8001CC38 8FA800D4 */  lw         $t0, 0xD4($sp)
    /* 1D83C 8001CC3C 4600E103 */  div.s      $ft0, $fs4, $fv0
    /* 1D840 8001CC40 E50A0000 */  swc1       $ft3, 0x0($t0)
    /* 1D844 8001CC44 C7A60084 */  lwc1       $ft1, 0x84($sp)
    /* 1D848 8001CC48 8FA900D8 */  lw         $t1, 0xD8($sp)
    /* 1D84C 8001CC4C 46003203 */  div.s      $ft2, $ft1, $fv0
    /* 1D850 8001CC50 E5280000 */  swc1       $ft2, 0x0($t1)
    /* 1D854 8001CC54 8FAA00DC */  lw         $t2, 0xDC($sp)
    /* 1D858 8001CC58 E5440000 */  swc1       $ft0, 0x0($t2)
  .L8001CC5C:
    /* 1D85C 8001CC5C 8FBF0064 */  lw         $ra, 0x64($sp)
    /* 1D860 8001CC60 D7B40018 */  ldc1       $fs0, 0x18($sp)
    /* 1D864 8001CC64 D7B60020 */  ldc1       $fs1, 0x20($sp)
    /* 1D868 8001CC68 D7B80028 */  ldc1       $fs2, 0x28($sp)
    /* 1D86C 8001CC6C D7BA0030 */  ldc1       $fs3, 0x30($sp)
    /* 1D870 8001CC70 D7BC0038 */  ldc1       $fs4, 0x38($sp)
    /* 1D874 8001CC74 D7BE0040 */  ldc1       $fs5, 0x40($sp)
    /* 1D878 8001CC78 8FB00048 */  lw         $s0, 0x48($sp)
    /* 1D87C 8001CC7C 8FB1004C */  lw         $s1, 0x4C($sp)
    /* 1D880 8001CC80 8FB20050 */  lw         $s2, 0x50($sp)
    /* 1D884 8001CC84 8FB30054 */  lw         $s3, 0x54($sp)
    /* 1D888 8001CC88 8FB40058 */  lw         $s4, 0x58($sp)
    /* 1D88C 8001CC8C 8FB5005C */  lw         $s5, 0x5C($sp)
    /* 1D890 8001CC90 8FB60060 */  lw         $s6, 0x60($sp)
    /* 1D894 8001CC94 03E00008 */  jr         $ra
    /* 1D898 8001CC98 27BD00C8 */   addiu     $sp, $sp, 0xC8
endlabel func_8001C860
