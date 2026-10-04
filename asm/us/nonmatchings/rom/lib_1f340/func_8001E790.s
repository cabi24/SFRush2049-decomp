nonmatching func_8001E790, 0x2C

glabel func_8001E790
    /* 1F390 8001E790 3C038003 */  lui        $v1, %hi(D_8002CC50)
    /* 1F394 8001E794 2463CC50 */  addiu      $v1, $v1, %lo(D_8002CC50)
    /* 1F398 8001E798 8C6E0000 */  lw         $t6, 0x0($v1)
    /* 1F39C 8001E79C 3C01A835 */  lui        $at, (0xA8351D63 >> 16)
    /* 1F3A0 8001E7A0 34211D63 */  ori        $at, $at, (0xA8351D63 & 0xFFFF)
    /* 1F3A4 8001E7A4 01C10019 */  multu      $t6, $at
    /* 1F3A8 8001E7A8 00007812 */  mflo       $t7
    /* 1F3AC 8001E7AC 000F1182 */  srl        $v0, $t7, 6
    /* 1F3B0 8001E7B0 AC6F0000 */  sw         $t7, 0x0($v1)
    /* 1F3B4 8001E7B4 03E00008 */  jr         $ra
    /* 1F3B8 8001E7B8 3042FFFF */   andi      $v0, $v0, 0xFFFF
endlabel func_8001E790
