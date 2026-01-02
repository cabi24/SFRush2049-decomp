# Source: E4F0.s
# Address: 0x8000D91C

glabel func_8000D91C
    /* E51C 8000D91C AFA40000 */  sw         $a0, 0x0($sp)
    /* E520 8000D920 AFA50004 */  sw         $a1, 0x4($sp)
    /* E524 8000D924 AFA60008 */  sw         $a2, 0x8($sp)
    /* E528 8000D928 AFA7000C */  sw         $a3, 0xC($sp)
    /* E52C 8000D92C DFAF0008 */  ld         $t7, 0x8($sp)
    /* E530 8000D930 DFAE0000 */  ld         $t6, 0x0($sp)
    /* E534 8000D934 01CF001F */  ddivu      $zero, $t6, $t7
    /* E538 8000D938 15E00002 */  bnez       $t7, .L8000D944
    /* E53C 8000D93C 00000000 */   nop
    /* E540 8000D940 0007000D */  break      7
  .L8000D944:
    /* E544 8000D944 00001010 */  mfhi       $v0
    /* E548 8000D948 0002183C */  dsll32     $v1, $v0, 0
    /* E54C 8000D94C 0003183F */  dsra32     $v1, $v1, 0
    /* E550 8000D950 03E00008 */  jr         $ra
    /* E554 8000D954 0002103F */   dsra32    $v0, $v0, 0