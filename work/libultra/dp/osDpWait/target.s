# Source: 8700.s
# Address: 0x80007B80

glabel func_80007B80
    /* 8780 80007B80 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 8784 80007B84 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 8788 80007B88 0C003590 */  jal        func_8000D640
    /* 878C 80007B8C 24040400 */   addiu     $a0, $zero, 0x400
    /* 8790 80007B90 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 8794 80007B94 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 8798 80007B98 03E00008 */  jr         $ra
    /* 879C 80007B9C 00000000 */   nop