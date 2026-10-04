nonmatching func_8001C390, 0x3C

glabel func_8001C390
    /* 1CF90 8001C390 3C038005 */  lui        $v1, %hi(D_8004FA18)
    /* 1CF94 8001C394 9063FA18 */  lbu        $v1, %lo(D_8004FA18)($v1)
    /* 1CF98 8001C398 3C0E8005 */  lui        $t6, %hi(D_8004FA50)
    /* 1CF9C 8001C39C 25C2FA50 */  addiu      $v0, $t6, %lo(D_8004FA50)
    /* 1CFA0 8001C3A0 18600008 */  blez       $v1, .L8001C3C4
    /* 1CFA4 8001C3A4 00037880 */   sll       $t7, $v1, 2
    /* 1CFA8 8001C3A8 01E37823 */  subu       $t7, $t7, $v1
    /* 1CFAC 8001C3AC 000F78C0 */  sll        $t7, $t7, 3
    /* 1CFB0 8001C3B0 01E22021 */  addu       $a0, $t7, $v0
  .L8001C3B4:
    /* 1CFB4 8001C3B4 24420018 */  addiu      $v0, $v0, 0x18
    /* 1CFB8 8001C3B8 0044082B */  sltu       $at, $v0, $a0
    /* 1CFBC 8001C3BC 1420FFFD */  bnez       $at, .L8001C3B4
    /* 1CFC0 8001C3C0 A040FFE8 */   sb        $zero, -0x18($v0)
  .L8001C3C4:
    /* 1CFC4 8001C3C4 03E00008 */  jr         $ra
    /* 1CFC8 8001C3C8 00000000 */   nop
endlabel func_8001C390
