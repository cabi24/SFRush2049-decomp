nonmatching func_80014FEC, 0x6C

glabel func_80014FEC
    /* 15BEC 80014FEC 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 15BF0 80014FF0 AFBF0024 */  sw         $ra, 0x24($sp)
    /* 15BF4 80014FF4 AFB20020 */  sw         $s2, 0x20($sp)
    /* 15BF8 80014FF8 AFB1001C */  sw         $s1, 0x1C($sp)
    /* 15BFC 80014FFC AFB00018 */  sw         $s0, 0x18($sp)
    /* 15C00 80015000 94860000 */  lhu        $a2, 0x0($a0)
    /* 15C04 80015004 3412FFFF */  ori        $s2, $zero, 0xFFFF
    /* 15C08 80015008 00808025 */  or         $s0, $a0, $zero
    /* 15C0C 8001500C 1246000C */  beq        $s2, $a2, .L80015040
    /* 15C10 80015010 00A08825 */   or        $s1, $a1, $zero
    /* 15C14 80015014 30C4FFFF */  andi       $a0, $a2, 0xFFFF
  .L80015018:
    /* 15C18 80015018 0C0053AF */  jal        func_80014EBC
    /* 15C1C 8001501C 02202825 */   or        $a1, $s1, $zero
    /* 15C20 80015020 10400003 */  beqz       $v0, .L80015030
    /* 15C24 80015024 24450008 */   addiu     $a1, $v0, 0x8
    /* 15C28 80015028 0C0055C8 */  jal        func_80015720
    /* 15C2C 8001502C 96040000 */   lhu       $a0, 0x0($s0)
  .L80015030:
    /* 15C30 80015030 96060002 */  lhu        $a2, 0x2($s0)
    /* 15C34 80015034 26100002 */  addiu      $s0, $s0, 0x2
    /* 15C38 80015038 5646FFF7 */  bnel       $s2, $a2, .L80015018
    /* 15C3C 8001503C 30C4FFFF */   andi      $a0, $a2, 0xFFFF
  .L80015040:
    /* 15C40 80015040 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 15C44 80015044 8FB00018 */  lw         $s0, 0x18($sp)
    /* 15C48 80015048 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 15C4C 8001504C 8FB20020 */  lw         $s2, 0x20($sp)
    /* 15C50 80015050 03E00008 */  jr         $ra
    /* 15C54 80015054 27BD0028 */   addiu     $sp, $sp, 0x28
endlabel func_80014FEC
