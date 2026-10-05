nonmatching func_80018B8C, 0x60

glabel func_80018B8C
    /* 1978C 80018B8C 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 19790 80018B90 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 19794 80018B94 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 19798 80018B98 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1979C 80018B9C 51C0000F */  beql       $t6, $zero, .L80018BDC
    /* 197A0 80018BA0 00001025 */   or        $v0, $zero, $zero
    /* 197A4 80018BA4 0C005D91 */  jal        func_80017644
    /* 197A8 80018BA8 00000000 */   nop
    /* 197AC 80018BAC 2401FFFF */  addiu      $at, $zero, -0x1
    /* 197B0 80018BB0 10410009 */  beq        $v0, $at, .L80018BD8
    /* 197B4 80018BB4 00027800 */   sll       $t7, $v0, 0
    /* 197B8 80018BB8 05E00007 */  bltz       $t7, .L80018BD8
    /* 197BC 80018BBC 0002C240 */   sll       $t8, $v0, 9
    /* 197C0 80018BC0 0302C023 */  subu       $t8, $t8, $v0
    /* 197C4 80018BC4 0018C0C0 */  sll        $t8, $t8, 3
    /* 197C8 80018BC8 3C028004 */  lui        $v0, %hi(D_80043EB8 + 0xFC6)
    /* 197CC 80018BCC 00581021 */  addu       $v0, $v0, $t8
    /* 197D0 80018BD0 10000002 */  b          .L80018BDC
    /* 197D4 80018BD4 94424E7E */   lhu       $v0, %lo(D_80043EB8 + 0xFC6)($v0)
  .L80018BD8:
    /* 197D8 80018BD8 00001025 */  or         $v0, $zero, $zero
  .L80018BDC:
    /* 197DC 80018BDC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 197E0 80018BE0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 197E4 80018BE4 03E00008 */  jr         $ra
    /* 197E8 80018BE8 00000000 */   nop
endlabel func_80018B8C
