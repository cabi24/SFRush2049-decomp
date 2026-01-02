# Source: A5F0.s
# Address: 0x80009BC8

glabel func_80009BC8
    /* A7C8 80009BC8 AFA40000 */  sw         $a0, 0x0($sp)
    /* A7CC 80009BCC AFA50004 */  sw         $a1, 0x4($sp)
    /* A7D0 80009BD0 DFAE0000 */  ld         $t6, 0x0($sp)
    /* A7D4 80009BD4 44AE2000 */  dmtc1      $t6, $ft0
    /* A7D8 80009BD8 05C10005 */  bgez       $t6, .L80009BF0
    /* A7DC 80009BDC 46A02020 */   cvt.s.l   $fv0, $ft0
    /* A7E0 80009BE0 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* A7E4 80009BE4 44813000 */  mtc1       $at, $ft1
    /* A7E8 80009BE8 00000000 */  nop
    /* A7EC 80009BEC 46060000 */  add.s      $fv0, $fv0, $ft1
  .L80009BF0:
    /* A7F0 80009BF0 03E00008 */  jr         $ra
    /* A7F4 80009BF4 00000000 */   nop