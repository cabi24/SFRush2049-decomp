# Source: E4F0.s
# Address: 0x8000D994

glabel func_8000D994
    /* E594 8000D994 AFA40000 */  sw         $a0, 0x0($sp)
    /* E598 8000D998 AFA50004 */  sw         $a1, 0x4($sp)
    /* E59C 8000D99C AFA60008 */  sw         $a2, 0x8($sp)
    /* E5A0 8000D9A0 AFA7000C */  sw         $a3, 0xC($sp)
    /* E5A4 8000D9A4 DFAF0008 */  ld         $t7, 0x8($sp)
    /* E5A8 8000D9A8 DFAE0000 */  ld         $t6, 0x0($sp)
    /* E5AC 8000D9AC 01EE1014 */  dsllv      $v0, $t6, $t7
    /* E5B0 8000D9B0 0002183C */  dsll32     $v1, $v0, 0
    /* E5B4 8000D9B4 0003183F */  dsra32     $v1, $v1, 0
    /* E5B8 8000D9B8 03E00008 */  jr         $ra
    /* E5BC 8000D9BC 0002103F */   dsra32    $v0, $v0, 0