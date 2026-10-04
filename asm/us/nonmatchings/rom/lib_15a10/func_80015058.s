nonmatching func_80015058, 0x70

glabel func_80015058
    /* 15C58 80015058 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 15C5C 8001505C AFBF0024 */  sw         $ra, 0x24($sp)
    /* 15C60 80015060 AFB20020 */  sw         $s2, 0x20($sp)
    /* 15C64 80015064 AFB1001C */  sw         $s1, 0x1C($sp)
    /* 15C68 80015068 AFB00018 */  sw         $s0, 0x18($sp)
    /* 15C6C 8001506C 94860000 */  lhu        $a2, 0x0($a0)
    /* 15C70 80015070 3412FFFF */  ori        $s2, $zero, 0xFFFF
    /* 15C74 80015074 00808025 */  or         $s0, $a0, $zero
    /* 15C78 80015078 1246000D */  beq        $s2, $a2, .L800150B0
    /* 15C7C 8001507C 00A08825 */   or        $s1, $a1, $zero
    /* 15C80 80015080 30C4FFFF */  andi       $a0, $a2, 0xFFFF
  .L80015084:
    /* 15C84 80015084 0C0053BA */  jal        func_80014EE8
    /* 15C88 80015088 02202825 */   or        $a1, $s1, $zero
    /* 15C8C 8001508C 10400004 */  beqz       $v0, .L800150A0
    /* 15C90 80015090 2445000C */   addiu     $a1, $v0, 0xC
    /* 15C94 80015094 96040000 */  lhu        $a0, 0x0($s0)
    /* 15C98 80015098 0C005683 */  jal        func_80015A0C
    /* 15C9C 8001509C 9446000A */   lhu       $a2, 0xA($v0)
  .L800150A0:
    /* 15CA0 800150A0 96060002 */  lhu        $a2, 0x2($s0)
    /* 15CA4 800150A4 26100002 */  addiu      $s0, $s0, 0x2
    /* 15CA8 800150A8 5646FFF6 */  bnel       $s2, $a2, .L80015084
    /* 15CAC 800150AC 30C4FFFF */   andi      $a0, $a2, 0xFFFF
  .L800150B0:
    /* 15CB0 800150B0 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 15CB4 800150B4 8FB00018 */  lw         $s0, 0x18($sp)
    /* 15CB8 800150B8 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 15CBC 800150BC 8FB20020 */  lw         $s2, 0x20($sp)
    /* 15CC0 800150C0 03E00008 */  jr         $ra
    /* 15CC4 800150C4 27BD0028 */   addiu     $sp, $sp, 0x28
endlabel func_80015058
