# Source: 21F0.s
# Address: 0x80001E58

glabel func_80001E58
    /* 2A58 80001E58 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 2A5C 80001E5C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 2A60 80001E60 3C048003 */  lui        $a0, %hi(gViewportStruct)
    /* 2A64 80001E64 0C001B50 */  jal        func_80006D40
    /* 2A68 80001E68 8C84EBFC */   lw        $a0, %lo(gViewportStruct)($a0)
    /* 2A6C 80001E6C 10000001 */  b          .L80001E74
    /* 2A70 80001E70 00000000 */   nop
  .L80001E74:
    /* 2A74 80001E74 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 2A78 80001E78 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 2A7C 80001E7C 03E00008 */  jr         $ra
    /* 2A80 80001E80 00000000 */   nop