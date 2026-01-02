# Source: B570.s
# Address: 0x8000ACA4

glabel func_8000ACA4
    /* B8A4 8000ACA4 27BDFFF0 */  addiu      $sp, $sp, -0x10
    /* B8A8 8000ACA8 AFB00008 */  sw         $s0, 0x8($sp)
    /* B8AC 8000ACAC 93B00023 */  lbu        $s0, 0x23($sp)
    /* B8B0 8000ACB0 AFB1000C */  sw         $s1, 0xC($sp)
    /* B8B4 8000ACB4 00C08825 */  or         $s1, $a2, $zero
    /* B8B8 8000ACB8 1A000003 */  blez       $s0, .L8000ACC8
    /* B8BC 8000ACBC 240B0080 */   addiu     $t3, $zero, 0x80
    /* B8C0 8000ACC0 10000002 */  b          .L8000ACCC
    /* B8C4 8000ACC4 24020001 */   addiu     $v0, $zero, 0x1
  .L8000ACC8:
    /* B8C8 8000ACC8 8C820060 */  lw         $v0, 0x60($a0)
  .L8000ACCC:
    /* B8CC 8000ACCC 28410080 */  slti       $at, $v0, 0x80
    /* B8D0 8000ACD0 1020000A */  beqz       $at, .L8000ACFC
    /* B8D4 8000ACD4 00401825 */   or        $v1, $v0, $zero
    /* B8D8 8000ACD8 00027040 */  sll        $t6, $v0, 1
    /* B8DC 8000ACDC 00AE1021 */  addu       $v0, $a1, $t6
    /* B8E0 8000ACE0 240A0003 */  addiu      $t2, $zero, 0x3
  .L8000ACE4:
    /* B8E4 8000ACE4 944F0000 */  lhu        $t7, 0x0($v0)
    /* B8E8 8000ACE8 514F0005 */  beql       $t2, $t7, .L8000AD00
    /* B8EC 8000ACEC 240B0080 */   addiu     $t3, $zero, 0x80
    /* B8F0 8000ACF0 24630001 */  addiu      $v1, $v1, 0x1
    /* B8F4 8000ACF4 146BFFFB */  bne        $v1, $t3, .L8000ACE4
    /* B8F8 8000ACF8 24420002 */   addiu     $v0, $v0, 0x2
  .L8000ACFC:
    /* B8FC 8000ACFC 240B0080 */  addiu      $t3, $zero, 0x80
  .L8000AD00:
    /* B900 8000AD00 146B0005 */  bne        $v1, $t3, .L8000AD18
    /* B904 8000AD04 240A0003 */   addiu     $t2, $zero, 0x3
    /* B908 8000AD08 2418FFFF */  addiu      $t8, $zero, -0x1
    /* B90C 8000AD0C ACF80000 */  sw         $t8, 0x0($a3)
    /* B910 8000AD10 10000030 */  b          .L8000ADD4
    /* B914 8000AD14 00001025 */   or        $v0, $zero, $zero
  .L8000AD18:
    /* B918 8000AD18 8FA60024 */  lw         $a2, 0x24($sp)
    /* B91C 8000AD1C 00602025 */  or         $a0, $v1, $zero
    /* B920 8000AD20 24190001 */  addiu      $t9, $zero, 0x1
    /* B924 8000AD24 00604025 */  or         $t0, $v1, $zero
    /* B928 8000AD28 2A210002 */  slti       $at, $s1, 0x2
    /* B92C 8000AD2C 24630001 */  addiu      $v1, $v1, 0x1
    /* B930 8000AD30 14200018 */  bnez       $at, .L8000AD94
    /* B934 8000AD34 ACD90000 */   sw        $t9, 0x0($a2)
    /* B938 8000AD38 28610080 */  slti       $at, $v1, 0x80
    /* B93C 8000AD3C 10200015 */  beqz       $at, .L8000AD94
    /* B940 8000AD40 00036040 */   sll       $t4, $v1, 1
    /* B944 8000AD44 00AC1021 */  addu       $v0, $a1, $t4
    /* B948 8000AD48 944D0000 */  lhu        $t5, 0x0($v0)
  .L8000AD4C:
    /* B94C 8000AD4C 00087040 */  sll        $t6, $t0, 1
    /* B950 8000AD50 00AE4821 */  addu       $t1, $a1, $t6
    /* B954 8000AD54 554D0008 */  bnel       $t2, $t5, .L8000AD78
    /* B958 8000AD58 8CD90000 */   lw        $t9, 0x0($a2)
    /* B95C 8000AD5C A1300000 */  sb         $s0, 0x0($t1)
    /* B960 8000AD60 A1230001 */  sb         $v1, 0x1($t1)
    /* B964 8000AD64 8CCF0000 */  lw         $t7, 0x0($a2)
    /* B968 8000AD68 00604025 */  or         $t0, $v1, $zero
    /* B96C 8000AD6C 25F80001 */  addiu      $t8, $t7, 0x1
    /* B970 8000AD70 ACD80000 */  sw         $t8, 0x0($a2)
    /* B974 8000AD74 8CD90000 */  lw         $t9, 0x0($a2)
  .L8000AD78:
    /* B978 8000AD78 24630001 */  addiu      $v1, $v1, 0x1
    /* B97C 8000AD7C 24420002 */  addiu      $v0, $v0, 0x2
    /* B980 8000AD80 0331082A */  slt        $at, $t9, $s1
    /* B984 8000AD84 10200003 */  beqz       $at, .L8000AD94
    /* B988 8000AD88 28610080 */   slti      $at, $v1, 0x80
    /* B98C 8000AD8C 5420FFEF */  bnel       $at, $zero, .L8000AD4C
    /* B990 8000AD90 944D0000 */   lhu       $t5, 0x0($v0)
  .L8000AD94:
    /* B994 8000AD94 146B0008 */  bne        $v1, $t3, .L8000ADB8
    /* B998 8000AD98 ACE40000 */   sw        $a0, 0x0($a3)
    /* B99C 8000AD9C 8CCC0000 */  lw         $t4, 0x0($a2)
    /* B9A0 8000ADA0 8FAD0028 */  lw         $t5, 0x28($sp)
    /* B9A4 8000ADA4 0191082A */  slt        $at, $t4, $s1
    /* B9A8 8000ADA8 50200004 */  beql       $at, $zero, .L8000ADBC
    /* B9AC 8000ADAC 00087840 */   sll       $t7, $t0, 1
    /* B9B0 8000ADB0 10000007 */  b          .L8000ADD0
    /* B9B4 8000ADB4 ADA80000 */   sw        $t0, 0x0($t5)
  .L8000ADB8:
    /* B9B8 8000ADB8 00087840 */  sll        $t7, $t0, 1
  .L8000ADBC:
    /* B9BC 8000ADBC 00AFC021 */  addu       $t8, $a1, $t7
    /* B9C0 8000ADC0 240E0001 */  addiu      $t6, $zero, 0x1
    /* B9C4 8000ADC4 A70E0000 */  sh         $t6, 0x0($t8)
    /* B9C8 8000ADC8 8FB90028 */  lw         $t9, 0x28($sp)
    /* B9CC 8000ADCC AF200000 */  sw         $zero, 0x0($t9)
  .L8000ADD0:
    /* B9D0 8000ADD0 00001025 */  or         $v0, $zero, $zero
  .L8000ADD4:
    /* B9D4 8000ADD4 8FB00008 */  lw         $s0, 0x8($sp)
    /* B9D8 8000ADD8 8FB1000C */  lw         $s1, 0xC($sp)
    /* B9DC 8000ADDC 03E00008 */  jr         $ra
    /* B9E0 8000ADE0 27BD0010 */   addiu     $sp, $sp, 0x10