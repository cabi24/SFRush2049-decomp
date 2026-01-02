# Source: BFE0.s
# Address: 0x8000B3E0

glabel func_8000B3E0
    /* BFE0 8000B3E0 27BDFB38 */  addiu      $sp, $sp, -0x4C8
    /* BFE4 8000B3E4 AFBF0044 */  sw         $ra, 0x44($sp)
    /* BFE8 8000B3E8 AFBE0040 */  sw         $fp, 0x40($sp)
    /* BFEC 8000B3EC AFB3002C */  sw         $s3, 0x2C($sp)
    /* BFF0 8000B3F0 00809825 */  or         $s3, $a0, $zero
    /* BFF4 8000B3F4 AFB7003C */  sw         $s7, 0x3C($sp)
    /* BFF8 8000B3F8 AFB60038 */  sw         $s6, 0x38($sp)
    /* BFFC 8000B3FC AFB50034 */  sw         $s5, 0x34($sp)
    /* C000 8000B400 AFB40030 */  sw         $s4, 0x30($sp)
    /* C004 8000B404 AFB20028 */  sw         $s2, 0x28($sp)
    /* C008 8000B408 AFB10024 */  sw         $s1, 0x24($sp)
    /* C00C 8000B40C AFB00020 */  sw         $s0, 0x20($sp)
    /* C010 8000B410 AFA00074 */  sw         $zero, 0x74($sp)
    /* C014 8000B414 0C003CB4 */  jal        func_8000F2D0
    /* C018 8000B418 241E00FE */   addiu     $fp, $zero, 0xFE
    /* C01C 8000B41C 24010002 */  addiu      $at, $zero, 0x2
    /* C020 8000B420 14410004 */  bne        $v0, $at, .L8000B434
    /* C024 8000B424 0040B825 */   or        $s7, $v0, $zero
    /* C028 8000B428 0C003C49 */  jal        func_8000F124
    /* C02C 8000B42C 02602025 */   or        $a0, $s3, $zero
    /* C030 8000B430 0040B825 */  or         $s7, $v0, $zero
  .L8000B434:
    /* C034 8000B434 10400003 */  beqz       $v0, .L8000B444
    /* C038 8000B438 02602025 */   or        $a0, $s3, $zero
    /* C03C 8000B43C 1000012A */  b          .L8000B8E8
    /* C040 8000B440 02E01025 */   or        $v0, $s7, $zero
  .L8000B444:
    /* C044 8000B444 0C002E46 */  jal        func_8000B918
    /* C048 8000B448 27A50078 */   addiu     $a1, $sp, 0x78
    /* C04C 8000B44C 50400004 */  beql       $v0, $zero, .L8000B460
    /* C050 8000B450 8E620050 */   lw        $v0, 0x50($s3)
    /* C054 8000B454 10000125 */  b          .L8000B8EC
    /* C058 8000B458 8FBF0044 */   lw        $ra, 0x44($sp)
    /* C05C 8000B45C 8E620050 */  lw         $v0, 0x50($s3)
  .L8000B460:
    /* C060 8000B460 0000A025 */  or         $s4, $zero, $zero
    /* C064 8000B464 27B502BC */  addiu      $s5, $sp, 0x2BC
    /* C068 8000B468 18400082 */  blez       $v0, .L8000B674
    /* C06C 8000B46C 27B204BC */   addiu     $s2, $sp, 0x4BC
    /* C070 8000B470 8E6E005C */  lw         $t6, 0x5C($s3)
  .L8000B474:
    /* C074 8000B474 8E640004 */  lw         $a0, 0x4($s3)
    /* C078 8000B478 8E650008 */  lw         $a1, 0x8($s3)
    /* C07C 8000B47C 01D43021 */  addu       $a2, $t6, $s4
    /* C080 8000B480 30CFFFFF */  andi       $t7, $a2, 0xFFFF
    /* C084 8000B484 01E03025 */  or         $a2, $t7, $zero
    /* C088 8000B488 0C003A34 */  jal        func_8000E8D0
    /* C08C 8000B48C 27A7029C */   addiu     $a3, $sp, 0x29C
    /* C090 8000B490 10400003 */  beqz       $v0, .L8000B4A0
    /* C094 8000B494 0040B825 */   or        $s7, $v0, $zero
    /* C098 8000B498 10000114 */  b          .L8000B8EC
    /* C09C 8000B49C 8FBF0044 */   lw        $ra, 0x44($sp)
  .L8000B4A0:
    /* C0A0 8000B4A0 97A202A0 */  lhu        $v0, 0x2A0($sp)
    /* C0A4 8000B4A4 8FB8029C */  lw         $t8, 0x29C($sp)
    /* C0A8 8000B4A8 14400003 */  bnez       $v0, .L8000B4B8
    /* C0AC 8000B4AC 00000000 */   nop
    /* C0B0 8000B4B0 5300006B */  beql       $t8, $zero, .L8000B660
    /* C0B4 8000B4B4 8E620050 */   lw        $v0, 0x50($s3)
  .L8000B4B8:
    /* C0B8 8000B4B8 10400003 */  beqz       $v0, .L8000B4C8
    /* C0BC 8000B4BC 8FB9029C */   lw        $t9, 0x29C($sp)
    /* C0C0 8000B4C0 17200003 */  bnez       $t9, .L8000B4D0
    /* C0C4 8000B4C4 27A902A2 */   addiu     $t1, $sp, 0x2A2
  .L8000B4C8:
    /* C0C8 8000B4C8 10000044 */  b          .L8000B5DC
    /* C0CC 8000B4CC 2404FFFF */   addiu     $a0, $zero, -0x1
  .L8000B4D0:
    /* C0D0 8000B4D0 95210000 */  lhu        $at, 0x0($t1)
    /* C0D4 8000B4D4 00002025 */  or         $a0, $zero, $zero
    /* C0D8 8000B4D8 00008025 */  or         $s0, $zero, $zero
    /* C0DC 8000B4DC A6410000 */  sh         $at, 0x0($s2)
    /* C0E0 8000B4E0 8E6D0060 */  lw         $t5, 0x60($s3)
    /* C0E4 8000B4E4 97AC04BC */  lhu        $t4, 0x4BC($sp)
    /* C0E8 8000B4E8 241600FF */  addiu      $s6, $zero, 0xFF
    /* C0EC 8000B4EC 93AE04BC */  lbu        $t6, 0x4BC($sp)
    /* C0F0 8000B4F0 018D082A */  slt        $at, $t4, $t5
    /* C0F4 8000B4F4 14200039 */  bnez       $at, .L8000B5DC
    /* C0F8 8000B4F8 00000000 */   nop
    /* C0FC 8000B4FC 926F0064 */  lbu        $t7, 0x64($s3)
    /* C100 8000B500 93B804BD */  lbu        $t8, 0x4BD($sp)
    /* C104 8000B504 01CF082A */  slt        $at, $t6, $t7
    /* C108 8000B508 10200034 */  beqz       $at, .L8000B5DC
    /* C10C 8000B50C 00000000 */   nop
    /* C110 8000B510 1B000032 */  blez       $t8, .L8000B5DC
    /* C114 8000B514 2B010080 */   slti      $at, $t8, 0x80
    /* C118 8000B518 10200030 */  beqz       $at, .L8000B5DC
    /* C11C 8000B51C 02C08825 */   or        $s1, $s6, $zero
    /* C120 8000B520 93B904BC */  lbu        $t9, 0x4BC($sp)
  .L8000B524:
    /* C124 8000B524 13310010 */  beq        $t9, $s1, .L8000B568
    /* C128 8000B528 0320B025 */   or        $s6, $t9, $zero
    /* C12C 8000B52C 133E0008 */  beq        $t9, $fp, .L8000B550
    /* C130 8000B530 03208825 */   or        $s1, $t9, $zero
    /* C134 8000B534 02602025 */  or         $a0, $s3, $zero
    /* C138 8000B538 02A02825 */  or         $a1, $s5, $zero
    /* C13C 8000B53C 00003025 */  or         $a2, $zero, $zero
    /* C140 8000B540 0C003CE9 */  jal        func_8000F3A4
    /* C144 8000B544 332700FF */   andi      $a3, $t9, 0xFF
    /* C148 8000B548 0040B825 */  or         $s7, $v0, $zero
    /* C14C 8000B54C 32DE00FF */  andi       $fp, $s6, 0xFF
  .L8000B550:
    /* C150 8000B550 12E00005 */  beqz       $s7, .L8000B568
    /* C154 8000B554 24010003 */   addiu     $at, $zero, 0x3
    /* C158 8000B558 52E10004 */  beql       $s7, $at, .L8000B56C
    /* C15C 8000B55C 96410000 */   lhu       $at, 0x0($s2)
    /* C160 8000B560 100000E1 */  b          .L8000B8E8
    /* C164 8000B564 02E01025 */   or        $v0, $s7, $zero
  .L8000B568:
    /* C168 8000B568 96410000 */  lhu        $at, 0x0($s2)
  .L8000B56C:
    /* C16C 8000B56C 02602025 */  or         $a0, $s3, $zero
    /* C170 8000B570 27A60078 */  addiu      $a2, $sp, 0x78
    /* C174 8000B574 A7A10004 */  sh         $at, 0x4($sp)
    /* C178 8000B578 0C002EFC */  jal        func_8000BBF0
    /* C17C 8000B57C 8FA50004 */   lw        $a1, 0x4($sp)
    /* C180 8000B580 00502023 */  subu       $a0, $v0, $s0
    /* C184 8000B584 14800015 */  bnez       $a0, .L8000B5DC
    /* C188 8000B588 93AB04BD */   lbu       $t3, 0x4BD($sp)
    /* C18C 8000B58C 000B6040 */  sll        $t4, $t3, 1
    /* C190 8000B590 02AC6821 */  addu       $t5, $s5, $t4
    /* C194 8000B594 95A10000 */  lhu        $at, 0x0($t5)
    /* C198 8000B598 24100001 */  addiu      $s0, $zero, 0x1
    /* C19C 8000B59C A6410000 */  sh         $at, 0x0($s2)
    /* C1A0 8000B5A0 8E790060 */  lw         $t9, 0x60($s3)
    /* C1A4 8000B5A4 97B804BC */  lhu        $t8, 0x4BC($sp)
    /* C1A8 8000B5A8 93AB04BD */  lbu        $t3, 0x4BD($sp)
    /* C1AC 8000B5AC 93AA04BC */  lbu        $t2, 0x4BC($sp)
    /* C1B0 8000B5B0 0319082A */  slt        $at, $t8, $t9
    /* C1B4 8000B5B4 14200009 */  bnez       $at, .L8000B5DC
    /* C1B8 8000B5B8 00000000 */   nop
    /* C1BC 8000B5BC 92690064 */  lbu        $t1, 0x64($s3)
    /* C1C0 8000B5C0 0149082A */  slt        $at, $t2, $t1
    /* C1C4 8000B5C4 10200005 */  beqz       $at, .L8000B5DC
    /* C1C8 8000B5C8 00000000 */   nop
    /* C1CC 8000B5CC 19600003 */  blez       $t3, .L8000B5DC
    /* C1D0 8000B5D0 29610080 */   slti      $at, $t3, 0x80
    /* C1D4 8000B5D4 5420FFD3 */  bnel       $at, $zero, .L8000B524
    /* C1D8 8000B5D8 93B904BC */   lbu       $t9, 0x4BC($sp)
  .L8000B5DC:
    /* C1DC 8000B5DC 14800003 */  bnez       $a0, .L8000B5EC
    /* C1E0 8000B5E0 97AC04BC */   lhu       $t4, 0x4BC($sp)
    /* C1E4 8000B5E4 24010001 */  addiu      $at, $zero, 0x1
    /* C1E8 8000B5E8 1181001C */  beq        $t4, $at, .L8000B65C
  .L8000B5EC:
    /* C1EC 8000B5EC 27A4029C */   addiu     $a0, $sp, 0x29C
    /* C1F0 8000B5F0 0C002164 */  jal        func_80008590
    /* C1F4 8000B5F4 24050020 */   addiu     $a1, $zero, 0x20
    /* C1F8 8000B5F8 926E0065 */  lbu        $t6, 0x65($s3)
    /* C1FC 8000B5FC 02602025 */  or         $a0, $s3, $zero
    /* C200 8000B600 51C00008 */  beql       $t6, $zero, .L8000B624
    /* C204 8000B604 8E6D005C */   lw        $t5, 0x5C($s3)
    /* C208 8000B608 0C003A14 */  jal        func_8000E850
    /* C20C 8000B60C 00002825 */   or        $a1, $zero, $zero
    /* C210 8000B610 50400004 */  beql       $v0, $zero, .L8000B624
    /* C214 8000B614 8E6D005C */   lw        $t5, 0x5C($s3)
    /* C218 8000B618 100000B4 */  b          .L8000B8EC
    /* C21C 8000B61C 8FBF0044 */   lw        $ra, 0x44($sp)
    /* C220 8000B620 8E6D005C */  lw         $t5, 0x5C($s3)
  .L8000B624:
    /* C224 8000B624 8E640004 */  lw         $a0, 0x4($s3)
    /* C228 8000B628 8E650008 */  lw         $a1, 0x8($s3)
    /* C22C 8000B62C 01B43021 */  addu       $a2, $t5, $s4
    /* C230 8000B630 30CFFFFF */  andi       $t7, $a2, 0xFFFF
    /* C234 8000B634 01E03025 */  or         $a2, $t7, $zero
    /* C238 8000B638 AFA00010 */  sw         $zero, 0x10($sp)
    /* C23C 8000B63C 0C003DA0 */  jal        func_8000F680
    /* C240 8000B640 27A7029C */   addiu     $a3, $sp, 0x29C
    /* C244 8000B644 10400003 */  beqz       $v0, .L8000B654
    /* C248 8000B648 8FB80074 */   lw        $t8, 0x74($sp)
    /* C24C 8000B64C 100000A7 */  b          .L8000B8EC
    /* C250 8000B650 8FBF0044 */   lw        $ra, 0x44($sp)
  .L8000B654:
    /* C254 8000B654 27190001 */  addiu      $t9, $t8, 0x1
    /* C258 8000B658 AFB90074 */  sw         $t9, 0x74($sp)
  .L8000B65C:
    /* C25C 8000B65C 8E620050 */  lw         $v0, 0x50($s3)
  .L8000B660:
    /* C260 8000B660 26940001 */  addiu      $s4, $s4, 0x1
    /* C264 8000B664 0282082A */  slt        $at, $s4, $v0
    /* C268 8000B668 5420FF82 */  bnel       $at, $zero, .L8000B474
    /* C26C 8000B66C 8E6E005C */   lw        $t6, 0x5C($s3)
    /* C270 8000B670 0000A025 */  or         $s4, $zero, $zero
  .L8000B674:
    /* C274 8000B674 18400021 */  blez       $v0, .L8000B6FC
    /* C278 8000B678 27B502BC */   addiu     $s5, $sp, 0x2BC
    /* C27C 8000B67C 8E6A005C */  lw         $t2, 0x5C($s3)
  .L8000B680:
    /* C280 8000B680 8E640004 */  lw         $a0, 0x4($s3)
    /* C284 8000B684 8E650008 */  lw         $a1, 0x8($s3)
    /* C288 8000B688 01543021 */  addu       $a2, $t2, $s4
    /* C28C 8000B68C 30C9FFFF */  andi       $t1, $a2, 0xFFFF
    /* C290 8000B690 01203025 */  or         $a2, $t1, $zero
    /* C294 8000B694 0C003A34 */  jal        func_8000E8D0
    /* C298 8000B698 27A7029C */   addiu     $a3, $sp, 0x29C
    /* C29C 8000B69C 10400003 */  beqz       $v0, .L8000B6AC
    /* C2A0 8000B6A0 97AE02A0 */   lhu       $t6, 0x2A0($sp)
    /* C2A4 8000B6A4 10000091 */  b          .L8000B8EC
    /* C2A8 8000B6A8 8FBF0044 */   lw        $ra, 0x44($sp)
  .L8000B6AC:
    /* C2AC 8000B6AC 00145840 */  sll        $t3, $s4, 1
    /* C2B0 8000B6B0 27AC027C */  addiu      $t4, $sp, 0x27C
    /* C2B4 8000B6B4 11C0000B */  beqz       $t6, .L8000B6E4
    /* C2B8 8000B6B8 016C2021 */   addu      $a0, $t3, $t4
    /* C2BC 8000B6BC 8FAD029C */  lw         $t5, 0x29C($sp)
    /* C2C0 8000B6C0 97AF02A2 */  lhu        $t7, 0x2A2($sp)
    /* C2C4 8000B6C4 51A00008 */  beql       $t5, $zero, .L8000B6E8
    /* C2C8 8000B6C8 A4800000 */   sh        $zero, 0x0($a0)
    /* C2CC 8000B6CC 96780062 */  lhu        $t8, 0x62($s3)
    /* C2D0 8000B6D0 01F8082A */  slt        $at, $t7, $t8
    /* C2D4 8000B6D4 54200004 */  bnel       $at, $zero, .L8000B6E8
    /* C2D8 8000B6D8 A4800000 */   sh        $zero, 0x0($a0)
    /* C2DC 8000B6DC 10000002 */  b          .L8000B6E8
    /* C2E0 8000B6E0 A48F0000 */   sh        $t7, 0x0($a0)
  .L8000B6E4:
    /* C2E4 8000B6E4 A4800000 */  sh         $zero, 0x0($a0)
  .L8000B6E8:
    /* C2E8 8000B6E8 8E790050 */  lw         $t9, 0x50($s3)
    /* C2EC 8000B6EC 26940001 */  addiu      $s4, $s4, 0x1
    /* C2F0 8000B6F0 0299082A */  slt        $at, $s4, $t9
    /* C2F4 8000B6F4 5420FFE2 */  bnel       $at, $zero, .L8000B680
    /* C2F8 8000B6F8 8E6A005C */   lw        $t2, 0x5C($s3)
  .L8000B6FC:
    /* C2FC 8000B6FC 926A0064 */  lbu        $t2, 0x64($s3)
    /* C300 8000B700 0000B025 */  or         $s6, $zero, $zero
    /* C304 8000B704 241E0003 */  addiu      $fp, $zero, 0x3
    /* C308 8000B708 1940006B */  blez       $t2, .L8000B8B8
    /* C30C 8000B70C 27B203BC */   addiu     $s2, $sp, 0x3BC
    /* C310 8000B710 27B00060 */  addiu      $s0, $sp, 0x60
  .L8000B714:
    /* C314 8000B714 02602025 */  or         $a0, $s3, $zero
    /* C318 8000B718 02A02825 */  or         $a1, $s5, $zero
    /* C31C 8000B71C 00003025 */  or         $a2, $zero, $zero
    /* C320 8000B720 0C003CE9 */  jal        func_8000F3A4
    /* C324 8000B724 32C700FF */   andi      $a3, $s6, 0xFF
    /* C328 8000B728 10400006 */  beqz       $v0, .L8000B744
    /* C32C 8000B72C 27A804BC */   addiu     $t0, $sp, 0x4BC
    /* C330 8000B730 24010003 */  addiu      $at, $zero, 0x3
    /* C334 8000B734 50410004 */  beql       $v0, $at, .L8000B748
    /* C338 8000B738 02C08825 */   or        $s1, $s6, $zero
    /* C33C 8000B73C 1000006B */  b          .L8000B8EC
    /* C340 8000B740 8FBF0044 */   lw        $ra, 0x44($sp)
  .L8000B744:
    /* C344 8000B744 02C08825 */  or         $s1, $s6, $zero
  .L8000B748:
    /* C348 8000B748 1AC00003 */  blez       $s6, .L8000B758
    /* C34C 8000B74C 0000A025 */   or        $s4, $zero, $zero
    /* C350 8000B750 10000002 */  b          .L8000B75C
    /* C354 8000B754 24050001 */   addiu     $a1, $zero, 0x1
  .L8000B758:
    /* C358 8000B758 8E650060 */  lw         $a1, 0x60($s3)
  .L8000B75C:
    /* C35C 8000B75C 18A0001C */  blez       $a1, .L8000B7D0
    /* C360 8000B760 30A70003 */   andi      $a3, $a1, 0x3
    /* C364 8000B764 10E0000B */  beqz       $a3, .L8000B794
    /* C368 8000B768 00E02025 */   or        $a0, $a3, $zero
    /* C36C 8000B76C 00143040 */  sll        $a2, $s4, 1
    /* C370 8000B770 02461821 */  addu       $v1, $s2, $a2
    /* C374 8000B774 02A61021 */  addu       $v0, $s5, $a2
  .L8000B778:
    /* C378 8000B778 94490000 */  lhu        $t1, 0x0($v0)
    /* C37C 8000B77C 26940001 */  addiu      $s4, $s4, 0x1
    /* C380 8000B780 24630002 */  addiu      $v1, $v1, 0x2
    /* C384 8000B784 24420002 */  addiu      $v0, $v0, 0x2
    /* C388 8000B788 1494FFFB */  bne        $a0, $s4, .L8000B778
    /* C38C 8000B78C A469FFFE */   sh        $t1, -0x2($v1)
    /* C390 8000B790 1285000F */  beq        $s4, $a1, .L8000B7D0
  .L8000B794:
    /* C394 8000B794 00143040 */   sll       $a2, $s4, 1
    /* C398 8000B798 02461821 */  addu       $v1, $s2, $a2
    /* C39C 8000B79C 02A61021 */  addu       $v0, $s5, $a2
  .L8000B7A0:
    /* C3A0 8000B7A0 944C0002 */  lhu        $t4, 0x2($v0)
    /* C3A4 8000B7A4 944E0004 */  lhu        $t6, 0x4($v0)
    /* C3A8 8000B7A8 944D0006 */  lhu        $t5, 0x6($v0)
    /* C3AC 8000B7AC 944B0000 */  lhu        $t3, 0x0($v0)
    /* C3B0 8000B7B0 26940004 */  addiu      $s4, $s4, 0x4
    /* C3B4 8000B7B4 24630008 */  addiu      $v1, $v1, 0x8
    /* C3B8 8000B7B8 24420008 */  addiu      $v0, $v0, 0x8
    /* C3BC 8000B7BC A46CFFFA */  sh         $t4, -0x6($v1)
    /* C3C0 8000B7C0 A46EFFFC */  sh         $t6, -0x4($v1)
    /* C3C4 8000B7C4 A46DFFFE */  sh         $t5, -0x2($v1)
    /* C3C8 8000B7C8 1685FFF5 */  bne        $s4, $a1, .L8000B7A0
    /* C3CC 8000B7CC A46BFFF8 */   sh        $t3, -0x8($v1)
  .L8000B7D0:
    /* C3D0 8000B7D0 2A810080 */  slti       $at, $s4, 0x80
    /* C3D4 8000B7D4 10200007 */  beqz       $at, .L8000B7F4
    /* C3D8 8000B7D8 02402825 */   or        $a1, $s2, $zero
    /* C3DC 8000B7DC 0014C040 */  sll        $t8, $s4, 1
    /* C3E0 8000B7E0 02581821 */  addu       $v1, $s2, $t8
  .L8000B7E4:
    /* C3E4 8000B7E4 24630002 */  addiu      $v1, $v1, 0x2
    /* C3E8 8000B7E8 0068082B */  sltu       $at, $v1, $t0
    /* C3EC 8000B7EC 1420FFFD */  bnez       $at, .L8000B7E4
    /* C3F0 8000B7F0 A47EFFFE */   sh        $fp, -0x2($v1)
  .L8000B7F4:
    /* C3F4 8000B7F4 8E6F0050 */  lw         $t7, 0x50($s3)
    /* C3F8 8000B7F8 0000A025 */  or         $s4, $zero, $zero
    /* C3FC 8000B7FC 24060001 */  addiu      $a2, $zero, 0x1
    /* C400 8000B800 19E00021 */  blez       $t7, .L8000B888
    /* C404 8000B804 32C700FF */   andi      $a3, $s6, 0xFF
    /* C408 8000B808 27A4027C */  addiu      $a0, $sp, 0x27C
    /* C40C 8000B80C 90990000 */  lbu        $t9, 0x0($a0)
  .L8000B810:
    /* C410 8000B810 56390018 */  bnel       $s1, $t9, .L8000B874
    /* C414 8000B814 8E6E0050 */   lw        $t6, 0x50($s3)
    /* C418 8000B818 948A0000 */  lhu        $t2, 0x0($a0)
    /* C41C 8000B81C 96690062 */  lhu        $t1, 0x62($s3)
    /* C420 8000B820 0149082A */  slt        $at, $t2, $t1
    /* C424 8000B824 54200013 */  bnel       $at, $zero, .L8000B874
    /* C428 8000B828 8E6E0050 */   lw        $t6, 0x50($s3)
    /* C42C 8000B82C 90830001 */  lbu        $v1, 0x1($a0)
  .L8000B830:
    /* C430 8000B830 00031040 */  sll        $v0, $v1, 1
    /* C434 8000B834 02A25821 */  addu       $t3, $s5, $v0
    /* C438 8000B838 95610000 */  lhu        $at, 0x0($t3)
    /* C43C 8000B83C 02426821 */  addu       $t5, $s2, $v0
    /* C440 8000B840 A6010000 */  sh         $at, 0x0($s0)
    /* C444 8000B844 A5A10000 */  sh         $at, 0x0($t5)
    /* C448 8000B848 96010000 */  lhu        $at, 0x0($s0)
    /* C44C 8000B84C A4810000 */  sh         $at, 0x0($a0)
    /* C450 8000B850 90890000 */  lbu        $t1, 0x0($a0)
    /* C454 8000B854 56290007 */  bnel       $s1, $t1, .L8000B874
    /* C458 8000B858 8E6E0050 */   lw        $t6, 0x50($s3)
    /* C45C 8000B85C 966B0062 */  lhu        $t3, 0x62($s3)
    /* C460 8000B860 302CFFFF */  andi       $t4, $at, 0xFFFF
    /* C464 8000B864 018B082A */  slt        $at, $t4, $t3
    /* C468 8000B868 5020FFF1 */  beql       $at, $zero, .L8000B830
    /* C46C 8000B86C 90830001 */   lbu       $v1, 0x1($a0)
    /* C470 8000B870 8E6E0050 */  lw         $t6, 0x50($s3)
  .L8000B874:
    /* C474 8000B874 26940001 */  addiu      $s4, $s4, 0x1
    /* C478 8000B878 24840002 */  addiu      $a0, $a0, 0x2
    /* C47C 8000B87C 028E082A */  slt        $at, $s4, $t6
    /* C480 8000B880 5420FFE3 */  bnel       $at, $zero, .L8000B810
    /* C484 8000B884 90990000 */   lbu       $t9, 0x0($a0)
  .L8000B888:
    /* C488 8000B888 0C003CE9 */  jal        func_8000F3A4
    /* C48C 8000B88C 02602025 */   or        $a0, $s3, $zero
    /* C490 8000B890 50400004 */  beql       $v0, $zero, .L8000B8A4
    /* C494 8000B894 926D0064 */   lbu       $t5, 0x64($s3)
    /* C498 8000B898 10000014 */  b          .L8000B8EC
    /* C49C 8000B89C 8FBF0044 */   lw        $ra, 0x44($sp)
    /* C4A0 8000B8A0 926D0064 */  lbu        $t5, 0x64($s3)
  .L8000B8A4:
    /* C4A4 8000B8A4 26D60001 */  addiu      $s6, $s6, 0x1
    /* C4A8 8000B8A8 32D800FF */  andi       $t8, $s6, 0xFF
    /* C4AC 8000B8AC 030D082A */  slt        $at, $t8, $t5
    /* C4B0 8000B8B0 1420FF98 */  bnez       $at, .L8000B714
    /* C4B4 8000B8B4 0300B025 */   or        $s6, $t8, $zero
  .L8000B8B8:
    /* C4B8 8000B8B8 8FAF0074 */  lw         $t7, 0x74($sp)
    /* C4BC 8000B8BC 00001025 */  or         $v0, $zero, $zero
    /* C4C0 8000B8C0 51E00006 */  beql       $t7, $zero, .L8000B8DC
    /* C4C4 8000B8C4 8E690000 */   lw        $t1, 0x0($s3)
    /* C4C8 8000B8C8 8E790000 */  lw         $t9, 0x0($s3)
    /* C4CC 8000B8CC 372A0002 */  ori        $t2, $t9, 0x2
    /* C4D0 8000B8D0 10000005 */  b          .L8000B8E8
    /* C4D4 8000B8D4 AE6A0000 */   sw        $t2, 0x0($s3)
    /* C4D8 8000B8D8 8E690000 */  lw         $t1, 0x0($s3)
  .L8000B8DC:
    /* C4DC 8000B8DC 2401FFFD */  addiu      $at, $zero, -0x3
    /* C4E0 8000B8E0 01216024 */  and        $t4, $t1, $at
    /* C4E4 8000B8E4 AE6C0000 */  sw         $t4, 0x0($s3)
  .L8000B8E8:
    /* C4E8 8000B8E8 8FBF0044 */  lw         $ra, 0x44($sp)
  .L8000B8EC:
    /* C4EC 8000B8EC 8FB00020 */  lw         $s0, 0x20($sp)
    /* C4F0 8000B8F0 8FB10024 */  lw         $s1, 0x24($sp)
    /* C4F4 8000B8F4 8FB20028 */  lw         $s2, 0x28($sp)
    /* C4F8 8000B8F8 8FB3002C */  lw         $s3, 0x2C($sp)
    /* C4FC 8000B8FC 8FB40030 */  lw         $s4, 0x30($sp)
    /* C500 8000B900 8FB50034 */  lw         $s5, 0x34($sp)
    /* C504 8000B904 8FB60038 */  lw         $s6, 0x38($sp)
    /* C508 8000B908 8FB7003C */  lw         $s7, 0x3C($sp)
    /* C50C 8000B90C 8FBE0040 */  lw         $fp, 0x40($sp)
    /* C510 8000B910 03E00008 */  jr         $ra
    /* C514 8000B914 27BD04C8 */   addiu     $sp, $sp, 0x4C8