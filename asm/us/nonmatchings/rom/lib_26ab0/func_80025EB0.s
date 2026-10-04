nonmatching func_80025EB0, 0xC4

glabel func_80025EB0
    /* 26AB0 80025EB0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 26AB4 80025EB4 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 26AB8 80025EB8 AFB00018 */  sw         $s0, 0x18($sp)
    /* 26ABC 80025EBC AFA50024 */  sw         $a1, 0x24($sp)
    /* 26AC0 80025EC0 00808025 */  or         $s0, $a0, $zero
    /* 26AC4 80025EC4 AFA60028 */  sw         $a2, 0x28($sp)
    /* 26AC8 80025EC8 AFA7002C */  sw         $a3, 0x2C($sp)
    /* 26ACC 80025ECC 0C002164 */  jal        bzero
    /* 26AD0 80025ED0 24050190 */   addiu     $a1, $zero, 0x190
    /* 26AD4 80025ED4 240E0028 */  addiu      $t6, $zero, 0x28
    /* 26AD8 80025ED8 A60E0166 */  sh         $t6, 0x166($s0)
    /* 26ADC 80025EDC 8FAF0030 */  lw         $t7, 0x30($sp)
    /* 26AE0 80025EE0 260411AC */  addiu      $a0, $s0, 0x11AC
    /* 26AE4 80025EE4 260511C4 */  addiu      $a1, $s0, 0x11C4
    /* 26AE8 80025EE8 AE0F0190 */  sw         $t7, 0x190($s0)
    /* 26AEC 80025EEC 8FB80024 */  lw         $t8, 0x24($sp)
    /* 26AF0 80025EF0 24060001 */  addiu      $a2, $zero, 0x1
    /* 26AF4 80025EF4 AE180194 */  sw         $t8, 0x194($s0)
    /* 26AF8 80025EF8 8FB90028 */  lw         $t9, 0x28($sp)
    /* 26AFC 80025EFC AE190198 */  sw         $t9, 0x198($s0)
    /* 26B00 80025F00 8FA8002C */  lw         $t0, 0x2C($sp)
    /* 26B04 80025F04 AE0011A8 */  sw         $zero, 0x11A8($s0)
    /* 26B08 80025F08 0C001A80 */  jal        osCreateMesgQueue
    /* 26B0C 80025F0C AE08019C */   sw        $t0, 0x19C($s0)
    /* 26B10 80025F10 8FA90024 */  lw         $t1, 0x24($sp)
    /* 26B14 80025F14 8FAA0030 */  lw         $t2, 0x30($sp)
    /* 26B18 80025F18 3002FFFF */  andi       $v0, $zero, 0xFFFF
    /* 26B1C 80025F1C 11200003 */  beqz       $t1, .L80025F2C
    /* 26B20 80025F20 240C0002 */   addiu     $t4, $zero, 0x2
    /* 26B24 80025F24 15400003 */  bnez       $t2, .L80025F34
    /* 26B28 80025F28 240B0001 */   addiu     $t3, $zero, 0x1
  .L80025F2C:
    /* 26B2C 80025F2C 10000002 */  b          .L80025F38
    /* 26B30 80025F30 A20011DD */   sb        $zero, 0x11DD($s0)
  .L80025F34:
    /* 26B34 80025F34 A20B11DD */  sb         $t3, 0x11DD($s0)
  .L80025F38:
    /* 26B38 80025F38 A20011DC */  sb         $zero, 0x11DC($s0)
    /* 26B3C 80025F3C A60011A2 */  sh         $zero, 0x11A2($s0)
    /* 26B40 80025F40 AE0211A4 */  sw         $v0, 0x11A4($s0)
    /* 26B44 80025F44 A60211A0 */  sh         $v0, 0x11A0($s0)
    /* 26B48 80025F48 AE0011C8 */  sw         $zero, 0x11C8($s0)
    /* 26B4C 80025F4C AE0011D0 */  sw         $zero, 0x11D0($s0)
    /* 26B50 80025F50 AE0011CC */  sw         $zero, 0x11CC($s0)
    /* 26B54 80025F54 AE0011D8 */  sw         $zero, 0x11D8($s0)
    /* 26B58 80025F58 AE0011D4 */  sw         $zero, 0x11D4($s0)
    /* 26B5C 80025F5C A20C11DE */  sb         $t4, 0x11DE($s0)
    /* 26B60 80025F60 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 26B64 80025F64 8FB00018 */  lw         $s0, 0x18($sp)
    /* 26B68 80025F68 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 26B6C 80025F6C 03E00008 */  jr         $ra
    /* 26B70 80025F70 00000000 */   nop
endlabel func_80025EB0
