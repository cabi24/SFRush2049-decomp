# Source: F160.s
# Address: 0x8000E7B4

glabel func_8000E7B4
    /* F3B4 8000E7B4 3C028003 */  lui        $v0, %hi(__osPfsBuffer)
    /* F3B8 8000E7B8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* F3BC 8000E7BC 24427F60 */  addiu      $v0, $v0, %lo(__osPfsBuffer)
    /* F3C0 8000E7C0 1880000B */  blez       $a0, .L8000E7F0
    /* F3C4 8000E7C4 00001825 */   or        $v1, $zero, $zero
    /* F3C8 8000E7C8 30870003 */  andi       $a3, $a0, 0x3
    /* F3CC 8000E7CC 10E00005 */  beqz       $a3, .L8000E7E4
    /* F3D0 8000E7D0 00E03025 */   or        $a2, $a3, $zero
  .L8000E7D4:
    /* F3D4 8000E7D4 24630001 */  addiu      $v1, $v1, 0x1
    /* F3D8 8000E7D8 14C3FFFE */  bne        $a2, $v1, .L8000E7D4
    /* F3DC 8000E7DC 24420001 */   addiu     $v0, $v0, 0x1
    /* F3E0 8000E7E0 10640003 */  beq        $v1, $a0, .L8000E7F0
  .L8000E7E4:
    /* F3E4 8000E7E4 24630004 */   addiu     $v1, $v1, 0x4
    /* F3E8 8000E7E8 1464FFFE */  bne        $v1, $a0, .L8000E7E4
    /* F3EC 8000E7EC 24420004 */   addiu     $v0, $v0, 0x4
  .L8000E7F0:
    /* F3F0 8000E7F0 88410000 */  lwl        $at, 0x0($v0)
    /* F3F4 8000E7F4 98410003 */  lwr        $at, 0x3($v0)
    /* F3F8 8000E7F8 27AE000C */  addiu      $t6, $sp, 0xC
    /* F3FC 8000E7FC ADC10000 */  sw         $at, 0x0($t6)
    /* F400 8000E800 90410004 */  lbu        $at, 0x4($v0)
    /* F404 8000E804 A1C10004 */  sb         $at, 0x4($t6)
    /* F408 8000E808 90580005 */  lbu        $t8, 0x5($v0)
    /* F40C 8000E80C A1D80005 */  sb         $t8, 0x5($t6)
    /* F410 8000E810 93B9000D */  lbu        $t9, 0xD($sp)
    /* F414 8000E814 332800C0 */  andi       $t0, $t9, 0xC0
    /* F418 8000E818 00084903 */  sra        $t1, $t0, 4
    /* F41C 8000E81C 312A00FF */  andi       $t2, $t1, 0xFF
    /* F420 8000E820 15400008 */  bnez       $t2, .L8000E844
    /* F424 8000E824 A0A90003 */   sb        $t1, 0x3($a1)
    /* F428 8000E828 93AB0010 */  lbu        $t3, 0x10($sp)
    /* F42C 8000E82C 93AD000F */  lbu        $t5, 0xF($sp)
    /* F430 8000E830 000B6200 */  sll        $t4, $t3, 8
    /* F434 8000E834 018D7825 */  or         $t7, $t4, $t5
    /* F438 8000E838 A4AF0000 */  sh         $t7, 0x0($a1)
    /* F43C 8000E83C 93AE0011 */  lbu        $t6, 0x11($sp)
    /* F440 8000E840 A0AE0002 */  sb         $t6, 0x2($a1)
  .L8000E844:
    /* F444 8000E844 03E00008 */  jr         $ra
    /* F448 8000E848 27BD0018 */   addiu     $sp, $sp, 0x18