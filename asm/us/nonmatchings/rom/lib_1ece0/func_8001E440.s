nonmatching func_8001E440, 0xCC

glabel func_8001E440
    /* 1F040 8001E440 AFA40000 */  sw         $a0, 0x0($sp)
    /* 1F044 8001E444 3084FFFF */  andi       $a0, $a0, 0xFFFF
    /* 1F048 8001E448 44842000 */  mtc1       $a0, $ft0
    /* 1F04C 8001E44C 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* 1F050 8001E450 04810004 */  bgez       $a0, .L8001E464
    /* 1F054 8001E454 468021A0 */   cvt.s.w   $ft1, $ft0
    /* 1F058 8001E458 44814000 */  mtc1       $at, $ft2
    /* 1F05C 8001E45C 00000000 */  nop
    /* 1F060 8001E460 46083180 */  add.s      $ft1, $ft1, $ft2
  .L8001E464:
    /* 1F064 8001E464 3C018003 */  lui        $at, %hi(D_8002D924)
    /* 1F068 8001E468 C42AD924 */  lwc1       $ft3, %lo(D_8002D924)($at)
    /* 1F06C 8001E46C 24020001 */  addiu      $v0, $zero, 0x1
    /* 1F070 8001E470 3C014F00 */  lui        $at, (0x4F000000 >> 16)
    /* 1F074 8001E474 460A3402 */  mul.s      $ft4, $ft1, $ft3
    /* 1F078 8001E478 444EF800 */  cfc1       $t6, $31
    /* 1F07C 8001E47C 44C2F800 */  ctc1       $v0, $31
    /* 1F080 8001E480 00000000 */  nop
    /* 1F084 8001E484 460084A4 */  cvt.w.s    $ft5, $ft4
    /* 1F088 8001E488 4442F800 */  cfc1       $v0, $31
    /* 1F08C 8001E48C 00000000 */  nop
    /* 1F090 8001E490 30420078 */  andi       $v0, $v0, 0x78
    /* 1F094 8001E494 50400017 */  beql       $v0, $zero, .L8001E4F4
    /* 1F098 8001E498 44029000 */   mfc1      $v0, $ft5
    /* 1F09C 8001E49C 44819000 */  mtc1       $at, $ft5
    /* 1F0A0 8001E4A0 24020001 */  addiu      $v0, $zero, 0x1
    /* 1F0A4 8001E4A4 46128481 */  sub.s      $ft5, $ft4, $ft5
    /* 1F0A8 8001E4A8 44C2F800 */  ctc1       $v0, $31
    /* 1F0AC 8001E4AC 00000000 */  nop
    /* 1F0B0 8001E4B0 460094A4 */  cvt.w.s    $ft5, $ft5
    /* 1F0B4 8001E4B4 4442F800 */  cfc1       $v0, $31
    /* 1F0B8 8001E4B8 00000000 */  nop
    /* 1F0BC 8001E4BC 30420078 */  andi       $v0, $v0, 0x78
    /* 1F0C0 8001E4C0 54400008 */  bnel       $v0, $zero, .L8001E4E4
    /* 1F0C4 8001E4C4 2402FFFF */   addiu     $v0, $zero, -0x1
    /* 1F0C8 8001E4C8 44029000 */  mfc1       $v0, $ft5
    /* 1F0CC 8001E4CC 3C018000 */  lui        $at, (0x80000000 >> 16)
    /* 1F0D0 8001E4D0 44CEF800 */  ctc1       $t6, $31
    /* 1F0D4 8001E4D4 00411025 */  or         $v0, $v0, $at
    /* 1F0D8 8001E4D8 03E00008 */  jr         $ra
    /* 1F0DC 8001E4DC 3042FFFF */   andi      $v0, $v0, 0xFFFF
    /* 1F0E0 8001E4E0 2402FFFF */  addiu      $v0, $zero, -0x1
  .L8001E4E4:
    /* 1F0E4 8001E4E4 44CEF800 */  ctc1       $t6, $31
    /* 1F0E8 8001E4E8 03E00008 */  jr         $ra
    /* 1F0EC 8001E4EC 3042FFFF */   andi      $v0, $v0, 0xFFFF
    /* 1F0F0 8001E4F0 44029000 */  mfc1       $v0, $ft5
  .L8001E4F4:
    /* 1F0F4 8001E4F4 00000000 */  nop
    /* 1F0F8 8001E4F8 0442FFFA */  bltzl      $v0, .L8001E4E4
    /* 1F0FC 8001E4FC 2402FFFF */   addiu     $v0, $zero, -0x1
    /* 1F100 8001E500 44CEF800 */  ctc1       $t6, $31
    /* 1F104 8001E504 03E00008 */  jr         $ra
    /* 1F108 8001E508 3042FFFF */   andi      $v0, $v0, 0xFFFF
endlabel func_8001E440
