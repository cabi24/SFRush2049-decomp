nonmatching func_8001F898, 0xBC

glabel func_8001F898
    /* 20498 8001F898 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 2049C 8001F89C AFBF001C */  sw         $ra, 0x1C($sp)
    /* 204A0 8001F8A0 AFA40020 */  sw         $a0, 0x20($sp)
    /* 204A4 8001F8A4 AFB10018 */  sw         $s1, 0x18($sp)
    /* 204A8 8001F8A8 AFB00014 */  sw         $s0, 0x14($sp)
    /* 204AC 8001F8AC 93A40023 */  lbu        $a0, 0x23($sp)
    /* 204B0 8001F8B0 240500FF */  addiu      $a1, $zero, 0xFF
    /* 204B4 8001F8B4 3406FFFF */  ori        $a2, $zero, 0xFFFF
    /* 204B8 8001F8B8 0C007C4F */  jal        func_8001F13C
    /* 204BC 8001F8BC 24070001 */   addiu     $a3, $zero, 0x1
    /* 204C0 8001F8C0 2401FFFF */  addiu      $at, $zero, -0x1
    /* 204C4 8001F8C4 1041001D */  beq        $v0, $at, .L8001F93C
    /* 204C8 8001F8C8 00408825 */   or        $s1, $v0, $zero
    /* 204CC 8001F8CC 00117080 */  sll        $t6, $s1, 2
    /* 204D0 8001F8D0 01D17023 */  subu       $t6, $t6, $s1
    /* 204D4 8001F8D4 000E7080 */  sll        $t6, $t6, 2
    /* 204D8 8001F8D8 01D17021 */  addu       $t6, $t6, $s1
    /* 204DC 8001F8DC 3C0F8005 */  lui        $t7, %hi(D_8004BEB8)
    /* 204E0 8001F8E0 25EFBEB8 */  addiu      $t7, $t7, %lo(D_8004BEB8)
    /* 204E4 8001F8E4 000E7140 */  sll        $t6, $t6, 5
    /* 204E8 8001F8E8 01CF8021 */  addu       $s0, $t6, $t7
    /* 204EC 8001F8EC 24020001 */  addiu      $v0, $zero, 0x1
    /* 204F0 8001F8F0 A20200BD */  sb         $v0, 0xBD($s0)
    /* 204F4 8001F8F4 A202004C */  sb         $v0, 0x4C($s0)
    /* 204F8 8001F8F8 0C007AC4 */  jal        func_8001EB10
    /* 204FC 8001F8FC 02002025 */   or        $a0, $s0, $zero
    /* 20500 8001F900 2401FF00 */  addiu      $at, $zero, -0x100
    /* 20504 8001F904 0221C025 */  or         $t8, $s1, $at
    /* 20508 8001F908 AA180060 */  swl        $t8, 0x60($s0)
    /* 2050C 8001F90C BA180063 */  swr        $t8, 0x63($s0)
    /* 20510 8001F910 0C00519F */  jal        func_8001467C
    /* 20514 8001F914 02202025 */   or        $a0, $s1, $zero
    /* 20518 8001F918 50400004 */  beql       $v0, $zero, .L8001F92C
    /* 2051C 8001F91C AA000000 */   swl       $zero, 0x0($s0)
    /* 20520 8001F920 0C0052BC */  jal        func_80014AF0
    /* 20524 8001F924 02202025 */   or        $a0, $s1, $zero
    /* 20528 8001F928 AA000000 */  swl        $zero, 0x0($s0)
  .L8001F92C:
    /* 2052C 8001F92C BA000003 */  swr        $zero, 0x3($s0)
    /* 20530 8001F930 02002025 */  or         $a0, $s0, $zero
    /* 20534 8001F934 0C007BE3 */  jal        func_8001EF8C
    /* 20538 8001F938 93A50023 */   lbu       $a1, 0x23($sp)
  .L8001F93C:
    /* 2053C 8001F93C 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 20540 8001F940 02201025 */  or         $v0, $s1, $zero
    /* 20544 8001F944 8FB10018 */  lw         $s1, 0x18($sp)
    /* 20548 8001F948 8FB00014 */  lw         $s0, 0x14($sp)
    /* 2054C 8001F94C 03E00008 */  jr         $ra
    /* 20550 8001F950 27BD0020 */   addiu     $sp, $sp, 0x20
endlabel func_8001F898
