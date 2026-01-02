# Source: E4F0.s
# Address: 0x8000DA58

glabel func_8000DA58
    /* E658 8000DA58 AFA40000 */  sw         $a0, 0x0($sp)
    /* E65C 8000DA5C AFA50004 */  sw         $a1, 0x4($sp)
    /* E660 8000DA60 AFA60008 */  sw         $a2, 0x8($sp)
    /* E664 8000DA64 AFA7000C */  sw         $a3, 0xC($sp)
    /* E668 8000DA68 DFAF0008 */  ld         $t7, 0x8($sp)
    /* E66C 8000DA6C DFAE0000 */  ld         $t6, 0x0($sp)
    /* E670 8000DA70 01CF001D */  dmultu     $t6, $t7
    /* E674 8000DA74 00001012 */  mflo       $v0
    /* E678 8000DA78 0002183C */  dsll32     $v1, $v0, 0
    /* E67C 8000DA7C 0003183F */  dsra32     $v1, $v1, 0
    /* E680 8000DA80 03E00008 */  jr         $ra
    /* E684 8000DA84 0002103F */   dsra32    $v0, $v0, 0