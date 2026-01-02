# Source: E4F0.s
# Address: 0x8000DB84

glabel func_8000DB84
    /* E784 8000DB84 AFA40000 */  sw         $a0, 0x0($sp)
    /* E788 8000DB88 AFA50004 */  sw         $a1, 0x4($sp)
    /* E78C 8000DB8C AFA60008 */  sw         $a2, 0x8($sp)
    /* E790 8000DB90 AFA7000C */  sw         $a3, 0xC($sp)
    /* E794 8000DB94 DFAF0008 */  ld         $t7, 0x8($sp)
    /* E798 8000DB98 DFAE0000 */  ld         $t6, 0x0($sp)
    /* E79C 8000DB9C 01EE1017 */  dsrav      $v0, $t6, $t7
    /* E7A0 8000DBA0 0002183C */  dsll32     $v1, $v0, 0
    /* E7A4 8000DBA4 0003183F */  dsra32     $v1, $v1, 0
    /* E7A8 8000DBA8 03E00008 */  jr         $ra
    /* E7AC 8000DBAC 0002103F */   dsra32    $v0, $v0, 0