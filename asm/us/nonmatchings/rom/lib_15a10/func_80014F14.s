nonmatching func_80014F14, 0x6C

glabel func_80014F14
    /* 15B14 80014F14 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 15B18 80014F18 AFBF0024 */  sw         $ra, 0x24($sp)
    /* 15B1C 80014F1C AFB20020 */  sw         $s2, 0x20($sp)
    /* 15B20 80014F20 AFB1001C */  sw         $s1, 0x1C($sp)
    /* 15B24 80014F24 AFB00018 */  sw         $s0, 0x18($sp)
    /* 15B28 80014F28 94860000 */  lhu        $a2, 0x0($a0)
    /* 15B2C 80014F2C 3412FFFF */  ori        $s2, $zero, 0xFFFF
    /* 15B30 80014F30 00808025 */  or         $s0, $a0, $zero
    /* 15B34 80014F34 1246000C */  beq        $s2, $a2, .L80014F68
    /* 15B38 80014F38 00A08825 */   or        $s1, $a1, $zero
    /* 15B3C 80014F3C 30C4FFFF */  andi       $a0, $a2, 0xFFFF
  .L80014F40:
    /* 15B40 80014F40 0C005399 */  jal        func_80014E64
    /* 15B44 80014F44 02202825 */   or        $a1, $s1, $zero
    /* 15B48 80014F48 10400003 */  beqz       $v0, .L80014F58
    /* 15B4C 80014F4C 24450008 */   addiu     $a1, $v0, 0x8
    /* 15B50 80014F50 0C0059C7 */  jal        func_8001671C
    /* 15B54 80014F54 96040000 */   lhu       $a0, 0x0($s0)
  .L80014F58:
    /* 15B58 80014F58 96060002 */  lhu        $a2, 0x2($s0)
    /* 15B5C 80014F5C 26100002 */  addiu      $s0, $s0, 0x2
    /* 15B60 80014F60 5646FFF7 */  bnel       $s2, $a2, .L80014F40
    /* 15B64 80014F64 30C4FFFF */   andi      $a0, $a2, 0xFFFF
  .L80014F68:
    /* 15B68 80014F68 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 15B6C 80014F6C 8FB00018 */  lw         $s0, 0x18($sp)
    /* 15B70 80014F70 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 15B74 80014F74 8FB20020 */  lw         $s2, 0x20($sp)
    /* 15B78 80014F78 03E00008 */  jr         $ra
    /* 15B7C 80014F7C 27BD0028 */   addiu     $sp, $sp, 0x28
endlabel func_80014F14
