# Source: F700.s
# Address: 0x8000F2D0

glabel func_8000F2D0
    /* FED0 8000F2D0 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* FED4 8000F2D4 AFBF001C */  sw         $ra, 0x1C($sp)
    /* FED8 8000F2D8 AFB00018 */  sw         $s0, 0x18($sp)
    /* FEDC 8000F2DC 908E0065 */  lbu        $t6, 0x65($a0)
    /* FEE0 8000F2E0 00808025 */  or         $s0, $a0, $zero
    /* FEE4 8000F2E4 51C0000F */  beql       $t6, $zero, .L8000F324
    /* FEE8 8000F2E8 8E040004 */   lw        $a0, 0x4($s0)
    /* FEEC 8000F2EC 0C003A14 */  jal        func_8000E850
    /* FEF0 8000F2F0 00002825 */   or        $a1, $zero, $zero
    /* FEF4 8000F2F4 24010002 */  addiu      $at, $zero, 0x2
    /* FEF8 8000F2F8 14410005 */  bne        $v0, $at, .L8000F310
    /* FEFC 8000F2FC 00401825 */   or        $v1, $v0, $zero
    /* FF00 8000F300 02002025 */  or         $a0, $s0, $zero
    /* FF04 8000F304 0C003A14 */  jal        func_8000E850
    /* FF08 8000F308 00002825 */   or        $a1, $zero, $zero
    /* FF0C 8000F30C 00401825 */  or         $v1, $v0, $zero
  .L8000F310:
    /* FF10 8000F310 50400004 */  beql       $v0, $zero, .L8000F324
    /* FF14 8000F314 8E040004 */   lw        $a0, 0x4($s0)
    /* FF18 8000F318 1000001D */  b          .L8000F390
    /* FF1C 8000F31C 00601025 */   or        $v0, $v1, $zero
    /* FF20 8000F320 8E040004 */  lw         $a0, 0x4($s0)
  .L8000F324:
    /* FF24 8000F324 8E050008 */  lw         $a1, 0x8($s0)
    /* FF28 8000F328 24060001 */  addiu      $a2, $zero, 0x1
    /* FF2C 8000F32C 0C003A34 */  jal        func_8000E8D0
    /* FF30 8000F330 27A70028 */   addiu     $a3, $sp, 0x28
    /* FF34 8000F334 1040000D */  beqz       $v0, .L8000F36C
    /* FF38 8000F338 24010002 */   addiu     $at, $zero, 0x2
    /* FF3C 8000F33C 10410003 */  beq        $v0, $at, .L8000F34C
    /* FF40 8000F340 24060001 */   addiu     $a2, $zero, 0x1
    /* FF44 8000F344 10000013 */  b          .L8000F394
    /* FF48 8000F348 8FBF001C */   lw        $ra, 0x1C($sp)
  .L8000F34C:
    /* FF4C 8000F34C 8E040004 */  lw         $a0, 0x4($s0)
    /* FF50 8000F350 8E050008 */  lw         $a1, 0x8($s0)
    /* FF54 8000F354 0C003A34 */  jal        func_8000E8D0
    /* FF58 8000F358 27A70028 */   addiu     $a3, $sp, 0x28
    /* FF5C 8000F35C 50400004 */  beql       $v0, $zero, .L8000F370
    /* FF60 8000F360 2604000C */   addiu     $a0, $s0, 0xC
    /* FF64 8000F364 1000000B */  b          .L8000F394
    /* FF68 8000F368 8FBF001C */   lw        $ra, 0x1C($sp)
  .L8000F36C:
    /* FF6C 8000F36C 2604000C */  addiu      $a0, $s0, 0xC
  .L8000F370:
    /* FF70 8000F370 27A50028 */  addiu      $a1, $sp, 0x28
    /* FF74 8000F374 0C003E34 */  jal        func_8000F8D0
    /* FF78 8000F378 24060020 */   addiu     $a2, $zero, 0x20
    /* FF7C 8000F37C 50400004 */  beql       $v0, $zero, .L8000F390
    /* FF80 8000F380 00001025 */   or        $v0, $zero, $zero
    /* FF84 8000F384 10000002 */  b          .L8000F390
    /* FF88 8000F388 24020002 */   addiu     $v0, $zero, 0x2
    /* FF8C 8000F38C 00001025 */  or         $v0, $zero, $zero
  .L8000F390:
    /* FF90 8000F390 8FBF001C */  lw         $ra, 0x1C($sp)
  .L8000F394:
    /* FF94 8000F394 8FB00018 */  lw         $s0, 0x18($sp)
    /* FF98 8000F398 27BD0048 */  addiu      $sp, $sp, 0x48
    /* FF9C 8000F39C 03E00008 */  jr         $ra
    /* FFA0 8000F3A0 00000000 */   nop