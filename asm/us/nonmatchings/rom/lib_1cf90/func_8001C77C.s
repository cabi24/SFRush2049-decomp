nonmatching func_8001C77C, 0x78

glabel func_8001C77C
    /* 1D37C 8001C77C 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 1D380 8001C780 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 1D384 8001C784 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 1D388 8001C788 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 1D38C 8001C78C AFA50024 */  sw         $a1, 0x24($sp)
    /* 1D390 8001C790 AFA60028 */  sw         $a2, 0x28($sp)
    /* 1D394 8001C794 11C00013 */  beqz       $t6, .L8001C7E4
    /* 1D398 8001C798 AFA7002C */   sw        $a3, 0x2C($sp)
    /* 1D39C 8001C79C 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1D3A0 8001C7A0 50810011 */  beql       $a0, $at, .L8001C7E8
    /* 1D3A4 8001C7A4 8FBF001C */   lw        $ra, 0x1C($sp)
    /* 1D3A8 8001C7A8 0C005165 */  jal        func_80014594
    /* 1D3AC 8001C7AC AFA40020 */   sw        $a0, 0x20($sp)
    /* 1D3B0 8001C7B0 93AF0033 */  lbu        $t7, 0x33($sp)
    /* 1D3B4 8001C7B4 93A50027 */  lbu        $a1, 0x27($sp)
    /* 1D3B8 8001C7B8 93A6002B */  lbu        $a2, 0x2B($sp)
    /* 1D3BC 8001C7BC 93A7002F */  lbu        $a3, 0x2F($sp)
    /* 1D3C0 8001C7C0 000FC400 */  sll        $t8, $t7, 16
    /* 1D3C4 8001C7C4 8FA40020 */  lw         $a0, 0x20($sp)
    /* 1D3C8 8001C7C8 AFB80010 */  sw         $t8, 0x10($sp)
    /* 1D3CC 8001C7CC 00052C00 */  sll        $a1, $a1, 16
    /* 1D3D0 8001C7D0 00063400 */  sll        $a2, $a2, 16
    /* 1D3D4 8001C7D4 0C00529D */  jal        func_80014A74
    /* 1D3D8 8001C7D8 00073C00 */   sll       $a3, $a3, 16
    /* 1D3DC 8001C7DC 0C005177 */  jal        func_800145DC
    /* 1D3E0 8001C7E0 00000000 */   nop
  .L8001C7E4:
    /* 1D3E4 8001C7E4 8FBF001C */  lw         $ra, 0x1C($sp)
  .L8001C7E8:
    /* 1D3E8 8001C7E8 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 1D3EC 8001C7EC 03E00008 */  jr         $ra
    /* 1D3F0 8001C7F0 00000000 */   nop
endlabel func_8001C77C
