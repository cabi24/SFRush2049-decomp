nonmatching func_80020F98, 0x44

glabel func_80020F98
    /* 21B98 80020F98 AFA50004 */  sw         $a1, 0x4($sp)
    /* 21B9C 80020F9C 30A500FF */  andi       $a1, $a1, 0xFF
    /* 21BA0 80020FA0 AFA40000 */  sw         $a0, 0x0($sp)
    /* 21BA4 80020FA4 240100FF */  addiu      $at, $zero, 0xFF
    /* 21BA8 80020FA8 10A10007 */  beq        $a1, $at, .L80020FC8
    /* 21BAC 80020FAC 308400FF */   andi      $a0, $a0, 0xFF
    /* 21BB0 80020FB0 00057100 */  sll        $t6, $a1, 4
    /* 21BB4 80020FB4 01C47821 */  addu       $t7, $t6, $a0
    /* 21BB8 80020FB8 3C028005 */  lui        $v0, %hi(D_80050C60)
    /* 21BBC 80020FBC 004F1021 */  addu       $v0, $v0, $t7
    /* 21BC0 80020FC0 03E00008 */  jr         $ra
    /* 21BC4 80020FC4 90420C60 */   lbu       $v0, %lo(D_80050C60)($v0)
  .L80020FC8:
    /* 21BC8 80020FC8 3C028005 */  lui        $v0, %hi(D_80050CE0)
    /* 21BCC 80020FCC 00441021 */  addu       $v0, $v0, $a0
    /* 21BD0 80020FD0 90420CE0 */  lbu        $v0, %lo(D_80050CE0)($v0)
    /* 21BD4 80020FD4 03E00008 */  jr         $ra
    /* 21BD8 80020FD8 00000000 */   nop
endlabel func_80020F98
