nonmatching func_8001F954, 0x7C

glabel func_8001F954
    /* 20554 8001F954 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 20558 8001F958 2401FFFF */  addiu      $at, $zero, -0x1
    /* 2055C 8001F95C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 20560 8001F960 10810017 */  beq        $a0, $at, .L8001F9C0
    /* 20564 8001F964 00802825 */   or        $a1, $a0, $zero
    /* 20568 8001F968 0C00519F */  jal        func_8001467C
    /* 2056C 8001F96C AFA50020 */   sw        $a1, 0x20($sp)
    /* 20570 8001F970 10400005 */  beqz       $v0, .L8001F988
    /* 20574 8001F974 8FA50020 */   lw        $a1, 0x20($sp)
    /* 20578 8001F978 00A02025 */  or         $a0, $a1, $zero
    /* 2057C 8001F97C 0C0052BC */  jal        func_80014AF0
    /* 20580 8001F980 AFA50020 */   sw        $a1, 0x20($sp)
    /* 20584 8001F984 8FA50020 */  lw         $a1, 0x20($sp)
  .L8001F988:
    /* 20588 8001F988 00057080 */  sll        $t6, $a1, 2
    /* 2058C 8001F98C 01C57023 */  subu       $t6, $t6, $a1
    /* 20590 8001F990 000E7080 */  sll        $t6, $t6, 2
    /* 20594 8001F994 01C57021 */  addu       $t6, $t6, $a1
    /* 20598 8001F998 3C0F8005 */  lui        $t7, %hi(D_8004BEB8)
    /* 2059C 8001F99C 25EFBEB8 */  addiu      $t7, $t7, %lo(D_8004BEB8)
    /* 205A0 8001F9A0 000E7140 */  sll        $t6, $t6, 5
    /* 205A4 8001F9A4 01CF2021 */  addu       $a0, $t6, $t7
    /* 205A8 8001F9A8 A8850060 */  swl        $a1, 0x60($a0)
    /* 205AC 8001F9AC B8850063 */  swr        $a1, 0x63($a0)
    /* 205B0 8001F9B0 0C007DBB */  jal        func_8001F6EC
    /* 205B4 8001F9B4 AFA4001C */   sw        $a0, 0x1C($sp)
    /* 205B8 8001F9B8 8FA4001C */  lw         $a0, 0x1C($sp)
    /* 205BC 8001F9BC A08000BD */  sb         $zero, 0xBD($a0)
  .L8001F9C0:
    /* 205C0 8001F9C0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 205C4 8001F9C4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 205C8 8001F9C8 03E00008 */  jr         $ra
    /* 205CC 8001F9CC 00000000 */   nop
endlabel func_8001F954
