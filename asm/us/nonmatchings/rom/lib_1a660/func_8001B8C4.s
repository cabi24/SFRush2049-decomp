nonmatching func_8001B8C4, 0xA4

glabel func_8001B8C4
    /* 1C4C4 8001B8C4 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 1C4C8 8001B8C8 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 1C4CC 8001B8CC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 1C4D0 8001B8D0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1C4D4 8001B8D4 11C0001F */  beqz       $t6, .L8001B954
    /* 1C4D8 8001B8D8 2409FFFF */   addiu     $t1, $zero, -0x1
    /* 1C4DC 8001B8DC 0C007B7D */  jal        func_8001EDF4
    /* 1C4E0 8001B8E0 AFA90018 */   sw        $t1, 0x18($sp)
    /* 1C4E4 8001B8E4 2408FFFF */  addiu      $t0, $zero, -0x1
    /* 1C4E8 8001B8E8 8FA90018 */  lw         $t1, 0x18($sp)
    /* 1C4EC 8001B8EC 10480019 */  beq        $v0, $t0, .L8001B954
    /* 1C4F0 8001B8F0 00402025 */   or        $a0, $v0, $zero
    /* 1C4F4 8001B8F4 3C068005 */  lui        $a2, %hi(D_8004BEB8)
    /* 1C4F8 8001B8F8 24C6BEB8 */  addiu      $a2, $a2, %lo(D_8004BEB8)
    /* 1C4FC 8001B8FC 240701A0 */  addiu      $a3, $zero, 0x1A0
    /* 1C500 8001B900 308500FF */  andi       $a1, $a0, 0xFF
  .L8001B904:
    /* 1C504 8001B904 00A70019 */  multu      $a1, $a3
    /* 1C508 8001B908 00007812 */  mflo       $t7
    /* 1C50C 8001B90C 00CF1821 */  addu       $v1, $a2, $t7
    /* 1C510 8001B910 88780060 */  lwl        $t8, 0x60($v1)
    /* 1C514 8001B914 98780063 */  lwr        $t8, 0x63($v1)
    /* 1C518 8001B918 14980007 */  bne        $a0, $t8, .L8001B938
    /* 1C51C 8001B91C 00000000 */   nop
    /* 1C520 8001B920 88790024 */  lwl        $t9, 0x24($v1)
    /* 1C524 8001B924 98790027 */  lwr        $t9, 0x27($v1)
    /* 1C528 8001B928 00004825 */  or         $t1, $zero, $zero
    /* 1C52C 8001B92C 372A0008 */  ori        $t2, $t9, 0x8
    /* 1C530 8001B930 A86A0024 */  swl        $t2, 0x24($v1)
    /* 1C534 8001B934 B86A0027 */  swr        $t2, 0x27($v1)
  .L8001B938:
    /* 1C538 8001B938 00A70019 */  multu      $a1, $a3
    /* 1C53C 8001B93C 00005812 */  mflo       $t3
    /* 1C540 8001B940 00CB6021 */  addu       $t4, $a2, $t3
    /* 1C544 8001B944 89840010 */  lwl        $a0, 0x10($t4)
    /* 1C548 8001B948 99840013 */  lwr        $a0, 0x13($t4)
    /* 1C54C 8001B94C 5488FFED */  bnel       $a0, $t0, .L8001B904
    /* 1C550 8001B950 308500FF */   andi      $a1, $a0, 0xFF
  .L8001B954:
    /* 1C554 8001B954 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1C558 8001B958 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 1C55C 8001B95C 01201025 */  or         $v0, $t1, $zero
    /* 1C560 8001B960 03E00008 */  jr         $ra
    /* 1C564 8001B964 00000000 */   nop
endlabel func_8001B8C4
