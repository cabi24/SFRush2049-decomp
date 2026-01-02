# Source: A5F0.s
# Address: 0x80009B94

glabel func_80009B94
    /* A794 80009B94 AFA40000 */  sw         $a0, 0x0($sp)
    /* A798 80009B98 AFA50004 */  sw         $a1, 0x4($sp)
    /* A79C 80009B9C DFAE0000 */  ld         $t6, 0x0($sp)
    /* A7A0 80009BA0 44AE2000 */  dmtc1      $t6, $ft0
    /* A7A4 80009BA4 05C10006 */  bgez       $t6, .L80009BC0
    /* A7A8 80009BA8 46A02021 */   cvt.d.l   $fv0, $ft0
    /* A7AC 80009BAC 3C0141F0 */  lui        $at, (0x41F00000 >> 16)
    /* A7B0 80009BB0 44813800 */  mtc1       $at, $ft1f
    /* A7B4 80009BB4 44803000 */  mtc1       $zero, $ft1
    /* A7B8 80009BB8 00000000 */  nop
    /* A7BC 80009BBC 46260000 */  add.d      $fv0, $fv0, $ft1
  .L80009BC0:
    /* A7C0 80009BC0 03E00008 */  jr         $ra
    /* A7C4 80009BC4 00000000 */   nop