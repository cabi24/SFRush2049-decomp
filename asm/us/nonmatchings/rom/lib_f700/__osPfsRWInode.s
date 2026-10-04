nonmatching __osPfsRWInode, 0x2D4

glabel __osPfsRWInode
    /* FFA4 8000F3A4 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* FFA8 8000F3A8 AFB40030 */  sw         $s4, 0x30($sp)
    /* FFAC 8000F3AC 30D400FF */  andi       $s4, $a2, 0xFF
    /* FFB0 8000F3B0 AFB20028 */  sw         $s2, 0x28($sp)
    /* FFB4 8000F3B4 00809025 */  or         $s2, $a0, $zero
    /* FFB8 8000F3B8 AFBF0034 */  sw         $ra, 0x34($sp)
    /* FFBC 8000F3BC AFB3002C */  sw         $s3, 0x2C($sp)
    /* FFC0 8000F3C0 AFB10024 */  sw         $s1, 0x24($sp)
    /* FFC4 8000F3C4 AFB00020 */  sw         $s0, 0x20($sp)
    /* FFC8 8000F3C8 AFA5005C */  sw         $a1, 0x5C($sp)
    /* FFCC 8000F3CC AFA60060 */  sw         $a2, 0x60($sp)
    /* FFD0 8000F3D0 16800011 */  bnez       $s4, .L8000F418
    /* FFD4 8000F3D4 AFA70064 */   sw        $a3, 0x64($sp)
    /* FFD8 8000F3D8 3C0F8003 */  lui        $t7, %hi(__osSiChannelMask)
    /* FFDC 8000F3DC 91EFC4D4 */  lbu        $t7, %lo(__osSiChannelMask)($t7)
    /* FFE0 8000F3E0 93B80067 */  lbu        $t8, 0x67($sp)
    /* FFE4 8000F3E4 3C198003 */  lui        $t9, %hi(__osSiLastChannel)
    /* FFE8 8000F3E8 55F8000C */  bnel       $t7, $t8, .L8000F41C
    /* FFEC 8000F3EC 92490065 */   lbu       $t1, 0x65($s2)
    /* FFF0 8000F3F0 8F39C4D0 */  lw         $t9, %lo(__osSiLastChannel)($t9)
    /* FFF4 8000F3F4 8C880008 */  lw         $t0, 0x8($a0)
    /* FFF8 8000F3F8 3C048003 */  lui        $a0, %hi(__osContPifInode)
    /* FFFC 8000F3FC 24847CB0 */  addiu      $a0, $a0, %lo(__osContPifInode)
    /* 10000 8000F400 57280006 */  bnel       $t9, $t0, .L8000F41C
    /* 10004 8000F404 92490065 */   lbu       $t1, 0x65($s2)
    /* 10008 8000F408 0C0034AC */  jal        bcopy
    /* 1000C 8000F40C 24060100 */   addiu     $a2, $zero, 0x100
    /* 10010 8000F410 10000091 */  b          .L8000F658
    /* 10014 8000F414 00001025 */   or        $v0, $zero, $zero
  .L8000F418:
    /* 10018 8000F418 92490065 */  lbu        $t1, 0x65($s2)
  .L8000F41C:
    /* 1001C 8000F41C 02402025 */  or         $a0, $s2, $zero
    /* 10020 8000F420 51200008 */  beql       $t1, $zero, .L8000F444
    /* 10024 8000F424 93A30067 */   lbu       $v1, 0x67($sp)
    /* 10028 8000F428 0C003A14 */  jal        __osPfsSelectBank
    /* 1002C 8000F42C 00002825 */   or        $a1, $zero, $zero
    /* 10030 8000F430 50400004 */  beql       $v0, $zero, .L8000F444
    /* 10034 8000F434 93A30067 */   lbu       $v1, 0x67($sp)
    /* 10038 8000F438 10000088 */  b          .L8000F65C
    /* 1003C 8000F43C 8FBF0034 */   lw        $ra, 0x34($sp)
    /* 10040 8000F440 93A30067 */  lbu        $v1, 0x67($sp)
  .L8000F444:
    /* 10044 8000F444 24020001 */  addiu      $v0, $zero, 0x1
    /* 10048 8000F448 240A0001 */  addiu      $t2, $zero, 0x1
    /* 1004C 8000F44C 58600004 */  blezl      $v1, .L8000F460
    /* 10050 8000F450 8E4B0060 */   lw        $t3, 0x60($s2)
    /* 10054 8000F454 10000003 */  b          .L8000F464
    /* 10058 8000F458 AFAA0048 */   sw        $t2, 0x48($sp)
    /* 1005C 8000F45C 8E4B0060 */  lw         $t3, 0x60($s2)
  .L8000F460:
    /* 10060 8000F460 AFAB0048 */  sw         $t3, 0x48($sp)
  .L8000F464:
    /* 10064 8000F464 1454000C */  bne        $v0, $s4, .L8000F498
    /* 10068 8000F468 8FAD0048 */   lw        $t5, 0x48($sp)
    /* 1006C 8000F46C 8FAC005C */  lw         $t4, 0x5C($sp)
    /* 10070 8000F470 000D2823 */  negu       $a1, $t5
    /* 10074 8000F474 00057840 */  sll        $t7, $a1, 1
    /* 10078 8000F478 000D7040 */  sll        $t6, $t5, 1
    /* 1007C 8000F47C 25E50100 */  addiu      $a1, $t7, 0x100
    /* 10080 8000F480 AFA3003C */  sw         $v1, 0x3C($sp)
    /* 10084 8000F484 0C003AC0 */  jal        __osSumcalc
    /* 10088 8000F488 018E2021 */   addu      $a0, $t4, $t6
    /* 1008C 8000F48C 8FB8005C */  lw         $t8, 0x5C($sp)
    /* 10090 8000F490 8FA3003C */  lw         $v1, 0x3C($sp)
    /* 10094 8000F494 A3020001 */  sb         $v0, 0x1($t8)
  .L8000F498:
    /* 10098 8000F498 00008025 */  or         $s0, $zero, $zero
    /* 1009C 8000F49C 8FB1005C */  lw         $s1, 0x5C($sp)
    /* 100A0 8000F4A0 000398C0 */  sll        $s3, $v1, 3
  .L8000F4A4:
    /* 100A4 8000F4A4 24020001 */  addiu      $v0, $zero, 0x1
    /* 100A8 8000F4A8 14540016 */  bne        $v0, $s4, .L8000F504
    /* 100AC 8000F4AC 8E450008 */   lw        $a1, 0x8($s2)
    /* 100B0 8000F4B0 8E590054 */  lw         $t9, 0x54($s2)
    /* 100B4 8000F4B4 8E440004 */  lw         $a0, 0x4($s2)
    /* 100B8 8000F4B8 AFA00010 */  sw         $zero, 0x10($sp)
    /* 100BC 8000F4BC 03334021 */  addu       $t0, $t9, $s3
    /* 100C0 8000F4C0 01103021 */  addu       $a2, $t0, $s0
    /* 100C4 8000F4C4 30C9FFFF */  andi       $t1, $a2, 0xFFFF
    /* 100C8 8000F4C8 01203025 */  or         $a2, $t1, $zero
    /* 100CC 8000F4CC 0C003DA0 */  jal        __osContRamWrite
    /* 100D0 8000F4D0 02203825 */   or        $a3, $s1, $zero
    /* 100D4 8000F4D4 8E4A0058 */  lw         $t2, 0x58($s2)
    /* 100D8 8000F4D8 8E440004 */  lw         $a0, 0x4($s2)
    /* 100DC 8000F4DC 8E450008 */  lw         $a1, 0x8($s2)
    /* 100E0 8000F4E0 01535821 */  addu       $t3, $t2, $s3
    /* 100E4 8000F4E4 01703021 */  addu       $a2, $t3, $s0
    /* 100E8 8000F4E8 30CCFFFF */  andi       $t4, $a2, 0xFFFF
    /* 100EC 8000F4EC 01803025 */  or         $a2, $t4, $zero
    /* 100F0 8000F4F0 AFA00010 */  sw         $zero, 0x10($sp)
    /* 100F4 8000F4F4 0C003DA0 */  jal        __osContRamWrite
    /* 100F8 8000F4F8 02203825 */   or        $a3, $s1, $zero
    /* 100FC 8000F4FC 1000000A */  b          .L8000F528
    /* 10100 8000F500 00401825 */   or        $v1, $v0, $zero
  .L8000F504:
    /* 10104 8000F504 8E4E0054 */  lw         $t6, 0x54($s2)
    /* 10108 8000F508 8E440004 */  lw         $a0, 0x4($s2)
    /* 1010C 8000F50C 02203825 */  or         $a3, $s1, $zero
    /* 10110 8000F510 01D36821 */  addu       $t5, $t6, $s3
    /* 10114 8000F514 01B03021 */  addu       $a2, $t5, $s0
    /* 10118 8000F518 30CFFFFF */  andi       $t7, $a2, 0xFFFF
    /* 1011C 8000F51C 0C003A34 */  jal        __osContRamRead
    /* 10120 8000F520 01E03025 */   or        $a2, $t7, $zero
    /* 10124 8000F524 00401825 */  or         $v1, $v0, $zero
  .L8000F528:
    /* 10128 8000F528 10400003 */  beqz       $v0, .L8000F538
    /* 1012C 8000F52C 26100001 */   addiu     $s0, $s0, 0x1
    /* 10130 8000F530 10000049 */  b          .L8000F658
    /* 10134 8000F534 00601025 */   or        $v0, $v1, $zero
  .L8000F538:
    /* 10138 8000F538 2A010008 */  slti       $at, $s0, 0x8
    /* 1013C 8000F53C 1420FFD9 */  bnez       $at, .L8000F4A4
    /* 10140 8000F540 26310020 */   addiu     $s1, $s1, 0x20
    /* 10144 8000F544 16800038 */  bnez       $s4, .L8000F628
    /* 10148 8000F548 8FB90048 */   lw        $t9, 0x48($sp)
    /* 1014C 8000F54C 8FB8005C */  lw         $t8, 0x5C($sp)
    /* 10150 8000F550 00192823 */  negu       $a1, $t9
    /* 10154 8000F554 00054840 */  sll        $t1, $a1, 1
    /* 10158 8000F558 00194040 */  sll        $t0, $t9, 1
    /* 1015C 8000F55C 25250100 */  addiu      $a1, $t1, 0x100
    /* 10160 8000F560 0308A021 */  addu       $s4, $t8, $t0
    /* 10164 8000F564 02802025 */  or         $a0, $s4, $zero
    /* 10168 8000F568 0C003AC0 */  jal        __osSumcalc
    /* 1016C 8000F56C AFA5003C */   sw        $a1, 0x3C($sp)
    /* 10170 8000F570 8FAB005C */  lw         $t3, 0x5C($sp)
    /* 10174 8000F574 304A00FF */  andi       $t2, $v0, 0xFF
    /* 10178 8000F578 00008025 */  or         $s0, $zero, $zero
    /* 1017C 8000F57C 916C0001 */  lbu        $t4, 0x1($t3)
    /* 10180 8000F580 01608825 */  or         $s1, $t3, $zero
    /* 10184 8000F584 514C0029 */  beql       $t2, $t4, .L8000F62C
    /* 10188 8000F588 93AB0067 */   lbu       $t3, 0x67($sp)
  .L8000F58C:
    /* 1018C 8000F58C 8E4E0058 */  lw         $t6, 0x58($s2)
    /* 10190 8000F590 8E440004 */  lw         $a0, 0x4($s2)
    /* 10194 8000F594 8E450008 */  lw         $a1, 0x8($s2)
    /* 10198 8000F598 01D36821 */  addu       $t5, $t6, $s3
    /* 1019C 8000F59C 01B03021 */  addu       $a2, $t5, $s0
    /* 101A0 8000F5A0 30CFFFFF */  andi       $t7, $a2, 0xFFFF
    /* 101A4 8000F5A4 01E03025 */  or         $a2, $t7, $zero
    /* 101A8 8000F5A8 0C003A34 */  jal        __osContRamRead
    /* 101AC 8000F5AC 02203825 */   or        $a3, $s1, $zero
    /* 101B0 8000F5B0 26100001 */  addiu      $s0, $s0, 0x1
    /* 101B4 8000F5B4 2A010008 */  slti       $at, $s0, 0x8
    /* 101B8 8000F5B8 1420FFF4 */  bnez       $at, .L8000F58C
    /* 101BC 8000F5BC 26310020 */   addiu     $s1, $s1, 0x20
    /* 101C0 8000F5C0 02802025 */  or         $a0, $s4, $zero
    /* 101C4 8000F5C4 0C003AC0 */  jal        __osSumcalc
    /* 101C8 8000F5C8 8FA5003C */   lw        $a1, 0x3C($sp)
    /* 101CC 8000F5CC 8FA8005C */  lw         $t0, 0x5C($sp)
    /* 101D0 8000F5D0 305800FF */  andi       $t8, $v0, 0xFF
    /* 101D4 8000F5D4 00008025 */  or         $s0, $zero, $zero
    /* 101D8 8000F5D8 91190001 */  lbu        $t9, 0x1($t0)
    /* 101DC 8000F5DC 8FB1005C */  lw         $s1, 0x5C($sp)
    /* 101E0 8000F5E0 13190003 */  beq        $t8, $t9, .L8000F5F0
    /* 101E4 8000F5E4 00000000 */   nop
    /* 101E8 8000F5E8 1000001B */  b          .L8000F658
    /* 101EC 8000F5EC 24020003 */   addiu     $v0, $zero, 0x3
  .L8000F5F0:
    /* 101F0 8000F5F0 8E490054 */  lw         $t1, 0x54($s2)
    /* 101F4 8000F5F4 8E440004 */  lw         $a0, 0x4($s2)
    /* 101F8 8000F5F8 8E450008 */  lw         $a1, 0x8($s2)
    /* 101FC 8000F5FC 01335021 */  addu       $t2, $t1, $s3
    /* 10200 8000F600 01503021 */  addu       $a2, $t2, $s0
    /* 10204 8000F604 30CCFFFF */  andi       $t4, $a2, 0xFFFF
    /* 10208 8000F608 01803025 */  or         $a2, $t4, $zero
    /* 1020C 8000F60C AFA00010 */  sw         $zero, 0x10($sp)
    /* 10210 8000F610 0C003DA0 */  jal        __osContRamWrite
    /* 10214 8000F614 02203825 */   or        $a3, $s1, $zero
    /* 10218 8000F618 26100001 */  addiu      $s0, $s0, 0x1
    /* 1021C 8000F61C 24010008 */  addiu      $at, $zero, 0x8
    /* 10220 8000F620 1601FFF3 */  bne        $s0, $at, .L8000F5F0
    /* 10224 8000F624 26310020 */   addiu     $s1, $s1, 0x20
  .L8000F628:
    /* 10228 8000F628 93AB0067 */  lbu        $t3, 0x67($sp)
  .L8000F62C:
    /* 1022C 8000F62C 3C018003 */  lui        $at, %hi(__osSiChannelMask)
    /* 10230 8000F630 3C058003 */  lui        $a1, %hi(__osContPifInode)
    /* 10234 8000F634 24A57CB0 */  addiu      $a1, $a1, %lo(__osContPifInode)
    /* 10238 8000F638 8FA4005C */  lw         $a0, 0x5C($sp)
    /* 1023C 8000F63C 24060100 */  addiu      $a2, $zero, 0x100
    /* 10240 8000F640 0C0034AC */  jal        bcopy
    /* 10244 8000F644 A02BC4D4 */   sb        $t3, %lo(__osSiChannelMask)($at)
    /* 10248 8000F648 8E4E0008 */  lw         $t6, 0x8($s2)
    /* 1024C 8000F64C 3C018003 */  lui        $at, %hi(__osSiLastChannel)
    /* 10250 8000F650 00001025 */  or         $v0, $zero, $zero
    /* 10254 8000F654 AC2EC4D0 */  sw         $t6, %lo(__osSiLastChannel)($at)
  .L8000F658:
    /* 10258 8000F658 8FBF0034 */  lw         $ra, 0x34($sp)
  .L8000F65C:
    /* 1025C 8000F65C 8FB00020 */  lw         $s0, 0x20($sp)
    /* 10260 8000F660 8FB10024 */  lw         $s1, 0x24($sp)
    /* 10264 8000F664 8FB20028 */  lw         $s2, 0x28($sp)
    /* 10268 8000F668 8FB3002C */  lw         $s3, 0x2C($sp)
    /* 1026C 8000F66C 8FB40030 */  lw         $s4, 0x30($sp)
    /* 10270 8000F670 03E00008 */  jr         $ra
    /* 10274 8000F674 27BD0058 */   addiu     $sp, $sp, 0x58
endlabel __osPfsRWInode
    /* 10278 8000F678 00000000 */  nop
    /* 1027C 8000F67C 00000000 */  nop
