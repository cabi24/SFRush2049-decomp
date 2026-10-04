nonmatching func_8001B968, 0x90

glabel func_8001B968
    /* 1C568 8001B968 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1C56C 8001B96C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1C570 8001B970 0C007B7D */  jal        func_8001EDF4
    /* 1C574 8001B974 00000000 */   nop
    /* 1C578 8001B978 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1C57C 8001B97C 10410019 */  beq        $v0, $at, .L8001B9E4
    /* 1C580 8001B980 240601A0 */   addiu     $a2, $zero, 0x1A0
    /* 1C584 8001B984 304E00FF */  andi       $t6, $v0, 0xFF
    /* 1C588 8001B988 01C60019 */  multu      $t6, $a2
    /* 1C58C 8001B98C 3C058005 */  lui        $a1, %hi(D_8004BEB8)
    /* 1C590 8001B990 24A5BEB8 */  addiu      $a1, $a1, %lo(D_8004BEB8)
    /* 1C594 8001B994 00007812 */  mflo       $t7
    /* 1C598 8001B998 00AF1821 */  addu       $v1, $a1, $t7
    /* 1C59C 8001B99C 88780060 */  lwl        $t8, 0x60($v1)
    /* 1C5A0 8001B9A0 98780063 */  lwr        $t8, 0x63($v1)
    /* 1C5A4 8001B9A4 54580010 */  bnel       $v0, $t8, .L8001B9E8
    /* 1C5A8 8001B9A8 00001025 */   or        $v0, $zero, $zero
    /* 1C5AC 8001B9AC 88790024 */  lwl        $t9, 0x24($v1)
    /* 1C5B0 8001B9B0 98790027 */  lwr        $t9, 0x27($v1)
    /* 1C5B4 8001B9B4 304900FF */  andi       $t1, $v0, 0xFF
    /* 1C5B8 8001B9B8 33280002 */  andi       $t0, $t9, 0x2
    /* 1C5BC 8001B9BC 5500000A */  bnel       $t0, $zero, .L8001B9E8
    /* 1C5C0 8001B9C0 00001025 */   or        $v0, $zero, $zero
    /* 1C5C4 8001B9C4 01260019 */  multu      $t1, $a2
    /* 1C5C8 8001B9C8 00005012 */  mflo       $t2
    /* 1C5CC 8001B9CC 00AA5821 */  addu       $t3, $a1, $t2
    /* 1C5D0 8001B9D0 916100C2 */  lbu        $at, 0xC2($t3)
    /* 1C5D4 8001B9D4 916200C3 */  lbu        $v0, 0xC3($t3)
    /* 1C5D8 8001B9D8 00010A00 */  sll        $at, $at, 8
    /* 1C5DC 8001B9DC 10000002 */  b          .L8001B9E8
    /* 1C5E0 8001B9E0 00411025 */   or        $v0, $v0, $at
  .L8001B9E4:
    /* 1C5E4 8001B9E4 00001025 */  or         $v0, $zero, $zero
  .L8001B9E8:
    /* 1C5E8 8001B9E8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1C5EC 8001B9EC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1C5F0 8001B9F0 03E00008 */  jr         $ra
    /* 1C5F4 8001B9F4 00000000 */   nop
endlabel func_8001B968
