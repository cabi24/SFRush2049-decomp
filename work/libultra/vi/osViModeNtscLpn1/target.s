# Source: 8440.s
# Address: 0x80007AB4

glabel func_80007AB4
    /* 86B4 80007AB4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 86B8 80007AB8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 86BC 80007ABC 0C0035C4 */  jal        func_8000D710
    /* 86C0 80007AC0 AFA40018 */   sw        $a0, 0x18($sp)
    /* 86C4 80007AC4 10400005 */  beqz       $v0, .L80007ADC
    /* 86C8 80007AC8 00000000 */   nop
  .L80007ACC:
    /* 86CC 80007ACC 0C0035C4 */  jal        func_8000D710
    /* 86D0 80007AD0 00000000 */   nop
    /* 86D4 80007AD4 1440FFFD */  bnez       $v0, .L80007ACC
    /* 86D8 80007AD8 00000000 */   nop
  .L80007ADC:
    /* 86DC 80007ADC 0C003590 */  jal        func_8000D640
    /* 86E0 80007AE0 24040125 */   addiu     $a0, $zero, 0x125
    /* 86E4 80007AE4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 86E8 80007AE8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 86EC 80007AEC 03E00008 */  jr         $ra
    /* 86F0 80007AF0 00000000 */   nop