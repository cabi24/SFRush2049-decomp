# Source: E4F0.s
# Address: 0x8000D9FC

glabel func_8000D9FC
    /* E5FC 8000D9FC AFA40000 */  sw         $a0, 0x0($sp)
    /* E600 8000DA00 AFA50004 */  sw         $a1, 0x4($sp)
    /* E604 8000DA04 AFA60008 */  sw         $a2, 0x8($sp)
    /* E608 8000DA08 AFA7000C */  sw         $a3, 0xC($sp)
    /* E60C 8000DA0C DFAF0008 */  ld         $t7, 0x8($sp)
    /* E610 8000DA10 DFAE0000 */  ld         $t6, 0x0($sp)
    /* E614 8000DA14 01CF001E */  ddiv       $zero, $t6, $t7
    /* E618 8000DA18 00000000 */  nop
    /* E61C 8000DA1C 15E00002 */  bnez       $t7, .L8000DA28
    /* E620 8000DA20 00000000 */   nop
    /* E624 8000DA24 0007000D */  break      7
  .L8000DA28:
    /* E628 8000DA28 6401FFFF */  daddiu     $at, $zero, -0x1
    /* E62C 8000DA2C 15E10005 */  bne        $t7, $at, .L8000DA44
    /* E630 8000DA30 64010001 */   daddiu    $at, $zero, 0x1
    /* E634 8000DA34 00010FFC */  dsll32     $at, $at, 31
    /* E638 8000DA38 15C10002 */  bne        $t6, $at, .L8000DA44
    /* E63C 8000DA3C 00000000 */   nop
    /* E640 8000DA40 0006000D */  break      6
  .L8000DA44:
    /* E644 8000DA44 00001012 */  mflo       $v0
    /* E648 8000DA48 0002183C */  dsll32     $v1, $v0, 0
    /* E64C 8000DA4C 0003183F */  dsra32     $v1, $v1, 0
    /* E650 8000DA50 03E00008 */  jr         $ra
    /* E654 8000DA54 0002103F */   dsra32    $v0, $v0, 0