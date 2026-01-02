# Source: 1050.s
# Address: 0x80000F88

glabel func_80000F88
    /* 1B88 80000F88 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1B8C 80000F8C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1B90 80000F90 AFA40018 */  sw         $a0, 0x18($sp)
    /* 1B94 80000F94 8FAE0018 */  lw         $t6, 0x18($sp)
    /* 1B98 80000F98 24010001 */  addiu      $at, $zero, 0x1
    /* 1B9C 80000F9C 8DCF0274 */  lw         $t7, 0x274($t6)
    /* 1BA0 80000FA0 8DF80010 */  lw         $t8, 0x10($t7)
    /* 1BA4 80000FA4 1701000A */  bne        $t8, $at, .L80000FD0
    /* 1BA8 80000FA8 00000000 */   nop
    /* 1BAC 80000FAC 8FB90018 */  lw         $t9, 0x18($sp)
    /* 1BB0 80000FB0 8F280274 */  lw         $t0, 0x274($t9)
    /* 1BB4 80000FB4 8D090004 */  lw         $t1, 0x4($t0)
    /* 1BB8 80000FB8 352A0010 */  ori        $t2, $t1, 0x10
    /* 1BBC 80000FBC AD0A0004 */  sw         $t2, 0x4($t0)
    /* 1BC0 80000FC0 0C001EE0 */  jal        func_80007B80
    /* 1BC4 80000FC4 00000000 */   nop
    /* 1BC8 80000FC8 10000001 */  b          .L80000FD0
    /* 1BCC 80000FCC 00000000 */   nop
  .L80000FD0:
    /* 1BD0 80000FD0 10000001 */  b          .L80000FD8
    /* 1BD4 80000FD4 00000000 */   nop
  .L80000FD8:
    /* 1BD8 80000FD8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1BDC 80000FDC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1BE0 80000FE0 03E00008 */  jr         $ra
    /* 1BE4 80000FE4 00000000 */   nop