nonmatching func_80014F80, 0x6C

glabel func_80014F80
    /* 15B80 80014F80 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 15B84 80014F84 AFBF0024 */  sw         $ra, 0x24($sp)
    /* 15B88 80014F88 AFB20020 */  sw         $s2, 0x20($sp)
    /* 15B8C 80014F8C AFB1001C */  sw         $s1, 0x1C($sp)
    /* 15B90 80014F90 AFB00018 */  sw         $s0, 0x18($sp)
    /* 15B94 80014F94 94860000 */  lhu        $a2, 0x0($a0)
    /* 15B98 80014F98 3412FFFF */  ori        $s2, $zero, 0xFFFF
    /* 15B9C 80014F9C 00808025 */  or         $s0, $a0, $zero
    /* 15BA0 80014FA0 1246000C */  beq        $s2, $a2, .L80014FD4
    /* 15BA4 80014FA4 00A08825 */   or        $s1, $a1, $zero
    /* 15BA8 80014FA8 30C4FFFF */  andi       $a0, $a2, 0xFFFF
  .L80014FAC:
    /* 15BAC 80014FAC 0C0053A4 */  jal        func_80014E90
    /* 15BB0 80014FB0 02202825 */   or        $a1, $s1, $zero
    /* 15BB4 80014FB4 10400003 */  beqz       $v0, .L80014FC4
    /* 15BB8 80014FB8 24450008 */   addiu     $a1, $v0, 0x8
    /* 15BBC 80014FBC 0C00575A */  jal        func_80015D68
    /* 15BC0 80014FC0 96040000 */   lhu       $a0, 0x0($s0)
  .L80014FC4:
    /* 15BC4 80014FC4 96060002 */  lhu        $a2, 0x2($s0)
    /* 15BC8 80014FC8 26100002 */  addiu      $s0, $s0, 0x2
    /* 15BCC 80014FCC 5646FFF7 */  bnel       $s2, $a2, .L80014FAC
    /* 15BD0 80014FD0 30C4FFFF */   andi      $a0, $a2, 0xFFFF
  .L80014FD4:
    /* 15BD4 80014FD4 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 15BD8 80014FD8 8FB00018 */  lw         $s0, 0x18($sp)
    /* 15BDC 80014FDC 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 15BE0 80014FE0 8FB20020 */  lw         $s2, 0x20($sp)
    /* 15BE4 80014FE4 03E00008 */  jr         $ra
    /* 15BE8 80014FE8 27BD0028 */   addiu     $sp, $sp, 0x28
endlabel func_80014F80
