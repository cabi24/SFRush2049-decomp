# Source: E4F0.s
# Address: 0x8000D8F0

glabel func_8000D8F0
    /* E4F0 8000D8F0 AFA40000 */  sw         $a0, 0x0($sp)
    /* E4F4 8000D8F4 AFA50004 */  sw         $a1, 0x4($sp)
    /* E4F8 8000D8F8 AFA60008 */  sw         $a2, 0x8($sp)
    /* E4FC 8000D8FC AFA7000C */  sw         $a3, 0xC($sp)
    /* E500 8000D900 DFAF0008 */  ld         $t7, 0x8($sp)
    /* E504 8000D904 DFAE0000 */  ld         $t6, 0x0($sp)
    /* E508 8000D908 01EE1016 */  dsrlv      $v0, $t6, $t7
    /* E50C 8000D90C 0002183C */  dsll32     $v1, $v0, 0
    /* E510 8000D910 0003183F */  dsra32     $v1, $v1, 0
    /* E514 8000D914 03E00008 */  jr         $ra
    /* E518 8000D918 0002103F */   dsra32    $v0, $v0, 0