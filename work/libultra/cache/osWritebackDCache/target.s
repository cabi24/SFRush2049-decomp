# Source: 88A0.s
# Address: 0x80007CA0

glabel func_80007CA0
    /* 88A0 80007CA0 18A00011 */  blez       $a1, .L80007CE8
    /* 88A4 80007CA4 00000000 */   nop
    /* 88A8 80007CA8 240B2000 */  addiu      $t3, $zero, 0x2000
    /* 88AC 80007CAC 00AB082B */  sltu       $at, $a1, $t3
    /* 88B0 80007CB0 1020000F */  beqz       $at, .L80007CF0
    /* 88B4 80007CB4 00000000 */   nop
    /* 88B8 80007CB8 00804025 */  or         $t0, $a0, $zero
    /* 88BC 80007CBC 00854821 */  addu       $t1, $a0, $a1
    /* 88C0 80007CC0 0109082B */  sltu       $at, $t0, $t1
    /* 88C4 80007CC4 10200008 */  beqz       $at, .L80007CE8
    /* 88C8 80007CC8 00000000 */   nop
    /* 88CC 80007CCC 310A000F */  andi       $t2, $t0, 0xF
    /* 88D0 80007CD0 2529FFF0 */  addiu      $t1, $t1, -0x10
    /* 88D4 80007CD4 010A4023 */  subu       $t0, $t0, $t2
  .L80007CD8:
    /* 88D8 80007CD8 BD190000 */  cache      0x19, 0x0($t0) /* handwritten instruction */
    /* 88DC 80007CDC 0109082B */  sltu       $at, $t0, $t1
    /* 88E0 80007CE0 1420FFFD */  bnez       $at, .L80007CD8
    /* 88E4 80007CE4 25080010 */   addiu     $t0, $t0, 0x10
  .L80007CE8:
    /* 88E8 80007CE8 03E00008 */  jr         $ra
    /* 88EC 80007CEC 00000000 */   nop
  .L80007CF0:
    /* 88F0 80007CF0 3C088000 */  lui        $t0, 0x8000
    /* 88F4 80007CF4 010B4821 */  addu       $t1, $t0, $t3
    /* 88F8 80007CF8 2529FFF0 */  addiu      $t1, $t1, -0x10
  .L80007CFC:
    /* 88FC 80007CFC BD010000 */  cache      0x01, 0x0($t0) /* handwritten instruction */
    /* 8900 80007D00 0109082B */  sltu       $at, $t0, $t1
    /* 8904 80007D04 1420FFFD */  bnez       $at, .L80007CFC
    /* 8908 80007D08 25080010 */   addiu     $t0, $t0, (0x80000010 & 0xFFFF)
    /* 890C 80007D0C 03E00008 */  jr         $ra
    /* 8910 80007D10 00000000 */   nop