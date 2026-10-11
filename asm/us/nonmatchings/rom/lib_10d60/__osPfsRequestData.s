nonmatching __osPfsRequestData, 0xB4

glabel __osPfsRequestData
    /* 10EEC 800102EC 27BDFFF0 */  addiu      $sp, $sp, -0x10
    /* 10EF0 800102F0 3C058003 */  lui        $a1, %hi(__osPfsBuffer)
    /* 10EF4 800102F4 3C018003 */  lui        $at, %hi(__osPfsRequestType)
    /* 10EF8 800102F8 24A27F60 */  addiu      $v0, $a1, %lo(__osPfsBuffer)
    /* 10EFC 800102FC AFA40010 */  sw         $a0, 0x10($sp)
    /* 10F00 80010300 A0247AE0 */  sb         $a0, %lo(__osPfsRequestType)($at)
    /* 10F04 80010304 240F0001 */  addiu      $t7, $zero, 0x1
    /* 10F08 80010308 AC4F003C */  sw         $t7, 0x3C($v0)
    /* 10F0C 8001030C 3C068003 */  lui        $a2, %hi(__osPfsRequestType2)
    /* 10F10 80010310 24C67AE1 */  addiu      $a2, $a2, %lo(__osPfsRequestType2)
    /* 10F14 80010314 90CD0000 */  lbu        $t5, 0x0($a2)
    /* 10F18 80010318 241800FF */  addiu      $t8, $zero, 0xFF
    /* 10F1C 8001031C 24190001 */  addiu      $t9, $zero, 0x1
    /* 10F20 80010320 24080003 */  addiu      $t0, $zero, 0x3
    /* 10F24 80010324 240900FF */  addiu      $t1, $zero, 0xFF
    /* 10F28 80010328 240A00FF */  addiu      $t2, $zero, 0xFF
    /* 10F2C 8001032C 240B00FF */  addiu      $t3, $zero, 0xFF
    /* 10F30 80010330 240C00FF */  addiu      $t4, $zero, 0xFF
    /* 10F34 80010334 A3B80004 */  sb         $t8, 0x4($sp)
    /* 10F38 80010338 A3B90005 */  sb         $t9, 0x5($sp)
    /* 10F3C 8001033C A3A80006 */  sb         $t0, 0x6($sp)
    /* 10F40 80010340 A3A40007 */  sb         $a0, 0x7($sp)
    /* 10F44 80010344 A3A90008 */  sb         $t1, 0x8($sp)
    /* 10F48 80010348 A3AA0009 */  sb         $t2, 0x9($sp)
    /* 10F4C 8001034C A3AB000A */  sb         $t3, 0xA($sp)
    /* 10F50 80010350 A3AC000B */  sb         $t4, 0xB($sp)
    /* 10F54 80010354 19A0000E */  blez       $t5, .L80010390
    /* 10F58 80010358 00001825 */   or        $v1, $zero, $zero
    /* 10F5C 8001035C 27A40004 */  addiu      $a0, $sp, 0x4
    /* 10F60 80010360 8C810000 */  lw         $at, 0x0($a0)
  .L80010364:
    /* 10F64 80010364 24630001 */  addiu      $v1, $v1, 0x1
    /* 10F68 80010368 24420008 */  addiu      $v0, $v0, 0x8
    /* 10F6C 8001036C A841FFF8 */  swl        $at, -0x8($v0)
    /* 10F70 80010370 B841FFFB */  swr        $at, -0x5($v0)
    /* 10F74 80010374 8C8F0004 */  lw         $t7, 0x4($a0)
    /* 10F78 80010378 A84FFFFC */  swl        $t7, -0x4($v0)
    /* 10F7C 8001037C B84FFFFF */  swr        $t7, -0x1($v0)
    /* 10F80 80010380 90D80000 */  lbu        $t8, 0x0($a2)
    /* 10F84 80010384 0078082A */  slt        $at, $v1, $t8
    /* 10F88 80010388 5420FFF6 */  bnel       $at, $zero, .L80010364
    /* 10F8C 8001038C 8C810000 */   lw        $at, 0x0($a0)
  .L80010390:
    /* 10F90 80010390 241900FE */  addiu      $t9, $zero, 0xFE
    /* 10F94 80010394 A0590000 */  sb         $t9, 0x0($v0)
    /* 10F98 80010398 03E00008 */  jr         $ra
    /* 10F9C 8001039C 27BD0010 */   addiu     $sp, $sp, 0x10
endlabel __osPfsRequestData
