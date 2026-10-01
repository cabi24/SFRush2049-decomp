nonmatching osDpWait, 0x20

glabel osDpWait
    /* 8780 80007B80 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 8784 80007B84 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 8788 80007B88 0C003590 */  jal        __osSpSetStatus
    /* 878C 80007B8C 24040400 */   addiu     $a0, $zero, 0x400
    /* 8790 80007B90 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 8794 80007B94 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 8798 80007B98 03E00008 */  jr         $ra
    /* 879C 80007B9C 00000000 */   nop
endlabel osDpWait
    /* 87A0 80007BA0 00000000 */  nop
    /* 87A4 80007BA4 00000000 */  nop
    /* 87A8 80007BA8 00000000 */  nop
    /* 87AC 80007BAC 00000000 */  nop
    /* 87B0 80007BB0 00000000 */  nop
    /* 87B4 80007BB4 00000000 */  nop
    /* 87B8 80007BB8 00000000 */  nop
    /* 87BC 80007BBC 00000000 */  nop
    /* 87C0 80007BC0 00000000 */  nop
    /* 87C4 80007BC4 00000000 */  nop
    /* 87C8 80007BC8 00000000 */  nop
    /* 87CC 80007BCC 00000000 */  nop
    /* 87D0 80007BD0 00000000 */  nop
    /* 87D4 80007BD4 00000000 */  nop
    /* 87D8 80007BD8 00000000 */  nop
    /* 87DC 80007BDC 00000000 */  nop
    /* 87E0 80007BE0 00000000 */  nop
    /* 87E4 80007BE4 00000000 */  nop
    /* 87E8 80007BE8 00000000 */  nop
    /* 87EC 80007BEC 00000000 */  nop
    /* 87F0 80007BF0 00000000 */  nop
    /* 87F4 80007BF4 00000000 */  nop
    /* 87F8 80007BF8 00000000 */  nop
    /* 87FC 80007BFC 00000000 */  nop
