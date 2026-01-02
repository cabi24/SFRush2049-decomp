# Source: B9F0.s
# Address: 0x8000ADF0

glabel func_8000ADF0
    /* B9F0 8000ADF0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* B9F4 8000ADF4 AFBF001C */  sw         $ra, 0x1C($sp)
    /* B9F8 8000ADF8 AFB00018 */  sw         $s0, 0x18($sp)
    /* B9FC 8000ADFC AFA50024 */  sw         $a1, 0x24($sp)
    /* BA00 8000AE00 AFA60028 */  sw         $a2, 0x28($sp)
    /* BA04 8000AE04 90AF0000 */  lbu        $t7, 0x0($a1)
    /* BA08 8000AE08 90E20000 */  lbu        $v0, 0x0($a3)
    /* BA0C 8000AE0C 00E08025 */  or         $s0, $a3, $zero
    /* BA10 8000AE10 00A07025 */  or         $t6, $a1, $zero
    /* BA14 8000AE14 104F000A */  beq        $v0, $t7, .L8000AE40
    /* BA18 8000AE18 00003025 */   or        $a2, $zero, $zero
    /* BA1C 8000AE1C A0A20000 */  sb         $v0, 0x0($a1)
    /* BA20 8000AE20 91C70000 */  lbu        $a3, 0x0($t6)
    /* BA24 8000AE24 AFA40020 */  sw         $a0, 0x20($sp)
    /* BA28 8000AE28 0C003CE9 */  jal        func_8000F3A4
    /* BA2C 8000AE2C 8FA50028 */   lw        $a1, 0x28($sp)
    /* BA30 8000AE30 10400003 */  beqz       $v0, .L8000AE40
    /* BA34 8000AE34 8FA40020 */   lw        $a0, 0x20($sp)
    /* BA38 8000AE38 1000001E */  b          .L8000AEB4
    /* BA3C 8000AE3C 8FBF001C */   lw        $ra, 0x1C($sp)
  .L8000AE40:
    /* BA40 8000AE40 92190001 */  lbu        $t9, 0x1($s0)
    /* BA44 8000AE44 8FB80028 */  lw         $t8, 0x28($sp)
    /* BA48 8000AE48 00194040 */  sll        $t0, $t9, 1
    /* BA4C 8000AE4C 03084821 */  addu       $t1, $t8, $t0
    /* BA50 8000AE50 95210000 */  lhu        $at, 0x0($t1)
    /* BA54 8000AE54 A6010000 */  sh         $at, 0x0($s0)
    /* BA58 8000AE58 8C8C0060 */  lw         $t4, 0x60($a0)
    /* BA5C 8000AE5C 3023FFFF */  andi       $v1, $at, 0xFFFF
    /* BA60 8000AE60 006C082A */  slt        $at, $v1, $t4
    /* BA64 8000AE64 5420000C */  bnel       $at, $zero, .L8000AE98
    /* BA68 8000AE68 24010001 */   addiu     $at, $zero, 0x1
    /* BA6C 8000AE6C 920D0000 */  lbu        $t5, 0x0($s0)
    /* BA70 8000AE70 908F0064 */  lbu        $t7, 0x64($a0)
    /* BA74 8000AE74 01AF082A */  slt        $at, $t5, $t7
    /* BA78 8000AE78 50200007 */  beql       $at, $zero, .L8000AE98
    /* BA7C 8000AE7C 24010001 */   addiu     $at, $zero, 0x1
    /* BA80 8000AE80 92020001 */  lbu        $v0, 0x1($s0)
    /* BA84 8000AE84 18400003 */  blez       $v0, .L8000AE94
    /* BA88 8000AE88 28410080 */   slti      $at, $v0, 0x80
    /* BA8C 8000AE8C 14200008 */  bnez       $at, .L8000AEB0
    /* BA90 8000AE90 00001025 */   or        $v0, $zero, $zero
  .L8000AE94:
    /* BA94 8000AE94 24010001 */  addiu      $at, $zero, 0x1
  .L8000AE98:
    /* BA98 8000AE98 14610003 */  bne        $v1, $at, .L8000AEA8
    /* BA9C 8000AE9C 00000000 */   nop
    /* BAA0 8000AEA0 10000003 */  b          .L8000AEB0
    /* BAA4 8000AEA4 24020005 */   addiu     $v0, $zero, 0x5
  .L8000AEA8:
    /* BAA8 8000AEA8 10000001 */  b          .L8000AEB0
    /* BAAC 8000AEAC 24020003 */   addiu     $v0, $zero, 0x3
  .L8000AEB0:
    /* BAB0 8000AEB0 8FBF001C */  lw         $ra, 0x1C($sp)
  .L8000AEB4:
    /* BAB4 8000AEB4 8FB00018 */  lw         $s0, 0x18($sp)
    /* BAB8 8000AEB8 27BD0020 */  addiu      $sp, $sp, 0x20
    /* BABC 8000AEBC 03E00008 */  jr         $ra
    /* BAC0 8000AEC0 00000000 */   nop