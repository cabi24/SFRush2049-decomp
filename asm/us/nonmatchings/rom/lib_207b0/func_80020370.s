nonmatching func_80020370, 0x7C

glabel func_80020370
    /* 20F70 80020370 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 20F74 80020374 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 20F78 80020378 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 20F7C 8002037C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 20F80 80020380 AFA40018 */  sw         $a0, 0x18($sp)
    /* 20F84 80020384 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 20F88 80020388 11C00014 */  beqz       $t6, .L800203DC
    /* 20F8C 8002038C 30A600FF */   andi      $a2, $a1, 0xFF
    /* 20F90 80020390 0C005165 */  jal        func_80014594
    /* 20F94 80020394 A3A6001F */   sb        $a2, 0x1F($sp)
    /* 20F98 80020398 0C005C10 */  jal        func_80017040
    /* 20F9C 8002039C 97A4001A */   lhu       $a0, 0x1A($sp)
    /* 20FA0 800203A0 93A6001F */  lbu        $a2, 0x1F($sp)
    /* 20FA4 800203A4 1040000B */  beqz       $v0, .L800203D4
    /* 20FA8 800203A8 00401825 */   or        $v1, $v0, $zero
    /* 20FAC 800203AC 240100FE */  addiu      $at, $zero, 0xFE
    /* 20FB0 800203B0 10C10007 */  beq        $a2, $at, .L800203D0
    /* 20FB4 800203B4 240F001F */   addiu     $t7, $zero, 0x1F
    /* 20FB8 800203B8 A0660009 */  sb         $a2, 0x9($v1)
    /* 20FBC 800203BC 30C400FF */  andi       $a0, $a2, 0xFF
    /* 20FC0 800203C0 0C007067 */  jal        func_8001C19C
    /* 20FC4 800203C4 24050003 */   addiu     $a1, $zero, 0x3
    /* 20FC8 800203C8 10000002 */  b          .L800203D4
    /* 20FCC 800203CC 00000000 */   nop
  .L800203D0:
    /* 20FD0 800203D0 A04F0009 */  sb         $t7, 0x9($v0)
  .L800203D4:
    /* 20FD4 800203D4 0C005177 */  jal        func_800145DC
    /* 20FD8 800203D8 00000000 */   nop
  .L800203DC:
    /* 20FDC 800203DC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 20FE0 800203E0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 20FE4 800203E4 03E00008 */  jr         $ra
    /* 20FE8 800203E8 00000000 */   nop
endlabel func_80020370
