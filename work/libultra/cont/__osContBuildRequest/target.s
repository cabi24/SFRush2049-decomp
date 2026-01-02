# Source: F160.s
# Address: 0x8000E6F0

glabel func_8000E6F0
    /* F2F0 8000E6F0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* F2F4 8000E6F4 3C068003 */  lui        $a2, %hi(__osPfsBuffer)
    /* F2F8 8000E6F8 240700FE */  addiu      $a3, $zero, 0xFE
    /* F2FC 8000E6FC 3C018003 */  lui        $at, %hi(__osPfsRequestType)
    /* F300 8000E700 24C27F60 */  addiu      $v0, $a2, %lo(__osPfsBuffer)
    /* F304 8000E704 AFA5001C */  sw         $a1, 0x1C($sp)
    /* F308 8000E708 A0277AE0 */  sb         $a3, %lo(__osPfsRequestType)($at)
    /* F30C 8000E70C 240F0001 */  addiu      $t7, $zero, 0x1
    /* F310 8000E710 AC4F003C */  sw         $t7, 0x3C($v0)
    /* F314 8000E714 24180001 */  addiu      $t8, $zero, 0x1
    /* F318 8000E718 24190003 */  addiu      $t9, $zero, 0x3
    /* F31C 8000E71C 240800FF */  addiu      $t0, $zero, 0xFF
    /* F320 8000E720 240900FF */  addiu      $t1, $zero, 0xFF
    /* F324 8000E724 240A00FF */  addiu      $t2, $zero, 0xFF
    /* F328 8000E728 A3B8000C */  sb         $t8, 0xC($sp)
    /* F32C 8000E72C A3B9000D */  sb         $t9, 0xD($sp)
    /* F330 8000E730 A3A5000E */  sb         $a1, 0xE($sp)
    /* F334 8000E734 A3A8000F */  sb         $t0, 0xF($sp)
    /* F338 8000E738 A3A90010 */  sb         $t1, 0x10($sp)
    /* F33C 8000E73C A3AA0011 */  sb         $t2, 0x11($sp)
    /* F340 8000E740 18800010 */  blez       $a0, .L8000E784
    /* F344 8000E744 00001825 */   or        $v1, $zero, $zero
    /* F348 8000E748 30860003 */  andi       $a2, $a0, 0x3
    /* F34C 8000E74C 10C00006 */  beqz       $a2, .L8000E768
    /* F350 8000E750 00C02825 */   or        $a1, $a2, $zero
  .L8000E754:
    /* F354 8000E754 24630001 */  addiu      $v1, $v1, 0x1
    /* F358 8000E758 A0400000 */  sb         $zero, 0x0($v0)
    /* F35C 8000E75C 14A3FFFD */  bne        $a1, $v1, .L8000E754
    /* F360 8000E760 24420001 */   addiu     $v0, $v0, 0x1
    /* F364 8000E764 10640007 */  beq        $v1, $a0, .L8000E784
  .L8000E768:
    /* F368 8000E768 24630004 */   addiu     $v1, $v1, 0x4
    /* F36C 8000E76C A0400001 */  sb         $zero, 0x1($v0)
    /* F370 8000E770 A0400002 */  sb         $zero, 0x2($v0)
    /* F374 8000E774 A0400003 */  sb         $zero, 0x3($v0)
    /* F378 8000E778 24420004 */  addiu      $v0, $v0, 0x4
    /* F37C 8000E77C 1464FFFA */  bne        $v1, $a0, .L8000E768
    /* F380 8000E780 A040FFFC */   sb        $zero, -0x4($v0)
  .L8000E784:
    /* F384 8000E784 27AB000C */  addiu      $t3, $sp, 0xC
    /* F388 8000E788 8D610000 */  lw         $at, 0x0($t3)
    /* F38C 8000E78C 24420006 */  addiu      $v0, $v0, 0x6
    /* F390 8000E790 A841FFFA */  swl        $at, -0x6($v0)
    /* F394 8000E794 B841FFFD */  swr        $at, -0x3($v0)
    /* F398 8000E798 91610004 */  lbu        $at, 0x4($t3)
    /* F39C 8000E79C A041FFFE */  sb         $at, -0x2($v0)
    /* F3A0 8000E7A0 916D0005 */  lbu        $t5, 0x5($t3)
    /* F3A4 8000E7A4 A0470000 */  sb         $a3, 0x0($v0)
    /* F3A8 8000E7A8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* F3AC 8000E7AC 03E00008 */  jr         $ra
    /* F3B0 8000E7B0 A04DFFFF */   sb        $t5, -0x1($v0)