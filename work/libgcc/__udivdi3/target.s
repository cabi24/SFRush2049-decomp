# Source: E4F0.s
# Address: 0x8000D958

glabel func_8000D958
    /* E558 8000D958 AFA40000 */  sw         $a0, 0x0($sp)
    /* E55C 8000D95C AFA50004 */  sw         $a1, 0x4($sp)
    /* E560 8000D960 AFA60008 */  sw         $a2, 0x8($sp)
    /* E564 8000D964 AFA7000C */  sw         $a3, 0xC($sp)
    /* E568 8000D968 DFAF0008 */  ld         $t7, 0x8($sp)
    /* E56C 8000D96C DFAE0000 */  ld         $t6, 0x0($sp)
    /* E570 8000D970 01CF001F */  ddivu      $zero, $t6, $t7
    /* E574 8000D974 15E00002 */  bnez       $t7, .L8000D980
    /* E578 8000D978 00000000 */   nop
    /* E57C 8000D97C 0007000D */  break      7
  .L8000D980:
    /* E580 8000D980 00001012 */  mflo       $v0
    /* E584 8000D984 0002183C */  dsll32     $v1, $v0, 0
    /* E588 8000D988 0003183F */  dsra32     $v1, $v1, 0
    /* E58C 8000D98C 03E00008 */  jr         $ra
    /* E590 8000D990 0002103F */   dsra32    $v0, $v0, 0