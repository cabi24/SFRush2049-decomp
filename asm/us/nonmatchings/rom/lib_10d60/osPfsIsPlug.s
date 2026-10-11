nonmatching osPfsIsPlug, 0x18C

glabel osPfsIsPlug
    /* 10D60 80010160 27BDFF90 */  addiu      $sp, $sp, -0x70
    /* 10D64 80010164 AFBF003C */  sw         $ra, 0x3C($sp)
    /* 10D68 80010168 AFB5002C */  sw         $s5, 0x2C($sp)
    /* 10D6C 8001016C AFB20020 */  sw         $s2, 0x20($sp)
    /* 10D70 80010170 AFB1001C */  sw         $s1, 0x1C($sp)
    /* 10D74 80010174 00809025 */  or         $s2, $a0, $zero
    /* 10D78 80010178 AFBE0038 */  sw         $fp, 0x38($sp)
    /* 10D7C 8001017C AFB70034 */  sw         $s7, 0x34($sp)
    /* 10D80 80010180 AFB60030 */  sw         $s6, 0x30($sp)
    /* 10D84 80010184 AFB40028 */  sw         $s4, 0x28($sp)
    /* 10D88 80010188 AFB30024 */  sw         $s3, 0x24($sp)
    /* 10D8C 8001018C AFB00018 */  sw         $s0, 0x18($sp)
    /* 10D90 80010190 AFA50074 */  sw         $a1, 0x74($sp)
    /* 10D94 80010194 0000A825 */  or         $s5, $zero, $zero
    /* 10D98 80010198 0C00396C */  jal        __osSiGetAccess
    /* 10D9C 8001019C 24110003 */   addiu     $s1, $zero, 0x3
    /* 10DA0 800101A0 3C1E8003 */  lui        $fp, %hi(__osPfsRequestType2)
    /* 10DA4 800101A4 3C138003 */  lui        $s3, %hi(__osPfsBuffer)
    /* 10DA8 800101A8 26737F60 */  addiu      $s3, $s3, %lo(__osPfsBuffer)
    /* 10DAC 800101AC 27DE7AE1 */  addiu      $fp, $fp, %lo(__osPfsRequestType2)
    /* 10DB0 800101B0 00008025 */  or         $s0, $zero, $zero
    /* 10DB4 800101B4 27B70054 */  addiu      $s7, $sp, 0x54
    /* 10DB8 800101B8 27B60067 */  addiu      $s6, $sp, 0x67
    /* 10DBC 800101BC 27B40068 */  addiu      $s4, $sp, 0x68
  .L800101C0:
    /* 10DC0 800101C0 0C0040BB */  jal        __osPfsRequestData
    /* 10DC4 800101C4 00002025 */   or        $a0, $zero, $zero
    /* 10DC8 800101C8 24040001 */  addiu      $a0, $zero, 0x1
    /* 10DCC 800101CC 0C00392C */  jal        __osSiRawStartDma
    /* 10DD0 800101D0 02602825 */   or        $a1, $s3, $zero
    /* 10DD4 800101D4 02402025 */  or         $a0, $s2, $zero
    /* 10DD8 800101D8 02802825 */  or         $a1, $s4, $zero
    /* 10DDC 800101DC 0C001C9C */  jal        osRecvMesg
    /* 10DE0 800101E0 24060001 */   addiu     $a2, $zero, 0x1
    /* 10DE4 800101E4 00002025 */  or         $a0, $zero, $zero
    /* 10DE8 800101E8 0C00392C */  jal        __osSiRawStartDma
    /* 10DEC 800101EC 02602825 */   or        $a1, $s3, $zero
    /* 10DF0 800101F0 AFA2006C */  sw         $v0, 0x6C($sp)
    /* 10DF4 800101F4 02402025 */  or         $a0, $s2, $zero
    /* 10DF8 800101F8 02802825 */  or         $a1, $s4, $zero
    /* 10DFC 800101FC 0C001C9C */  jal        osRecvMesg
    /* 10E00 80010200 24060001 */   addiu     $a2, $zero, 0x1
    /* 10E04 80010204 02C02025 */  or         $a0, $s6, $zero
    /* 10E08 80010208 0C0040E8 */  jal        func_800103A0
    /* 10E0C 8001020C 02E02825 */   or        $a1, $s7, $zero
    /* 10E10 80010210 93C30000 */  lbu        $v1, 0x0($fp)
    /* 10E14 80010214 27A20054 */  addiu      $v0, $sp, 0x54
    /* 10E18 80010218 5860000C */  blezl      $v1, .L8001024C
    /* 10E1C 8001021C 02031026 */   xor       $v0, $s0, $v1
  .L80010220:
    /* 10E20 80010220 904E0002 */  lbu        $t6, 0x2($v0)
    /* 10E24 80010224 31CF0004 */  andi       $t7, $t6, 0x4
    /* 10E28 80010228 55E00004 */  bnel       $t7, $zero, .L8001023C
    /* 10E2C 8001022C 26100001 */   addiu     $s0, $s0, 0x1
    /* 10E30 80010230 10000005 */  b          .L80010248
    /* 10E34 80010234 2631FFFF */   addiu     $s1, $s1, -0x1
    /* 10E38 80010238 26100001 */  addiu      $s0, $s0, 0x1
  .L8001023C:
    /* 10E3C 8001023C 0203082A */  slt        $at, $s0, $v1
    /* 10E40 80010240 1420FFF7 */  bnez       $at, .L80010220
    /* 10E44 80010244 24420004 */   addiu     $v0, $v0, 0x4
  .L80010248:
    /* 10E48 80010248 02031026 */  xor        $v0, $s0, $v1
  .L8001024C:
    /* 10E4C 8001024C 14400002 */  bnez       $v0, .L80010258
    /* 10E50 80010250 00008025 */   or        $s0, $zero, $zero
    /* 10E54 80010254 00008825 */  or         $s1, $zero, $zero
  .L80010258:
    /* 10E58 80010258 1E20FFD9 */  bgtz       $s1, .L800101C0
    /* 10E5C 8001025C 00000000 */   nop
    /* 10E60 80010260 18600011 */  blez       $v1, .L800102A8
    /* 10E64 80010264 27A20054 */   addiu     $v0, $sp, 0x54
  .L80010268:
    /* 10E68 80010268 90580003 */  lbu        $t8, 0x3($v0)
    /* 10E6C 8001026C 5700000B */  bnel       $t8, $zero, .L8001029C
    /* 10E70 80010270 26100001 */   addiu     $s0, $s0, 0x1
    /* 10E74 80010274 90590002 */  lbu        $t9, 0x2($v0)
    /* 10E78 80010278 24090001 */  addiu      $t1, $zero, 0x1
    /* 10E7C 8001027C 02095004 */  sllv       $t2, $t1, $s0
    /* 10E80 80010280 33280001 */  andi       $t0, $t9, 0x1
    /* 10E84 80010284 51000005 */  beql       $t0, $zero, .L8001029C
    /* 10E88 80010288 26100001 */   addiu     $s0, $s0, 0x1
    /* 10E8C 8001028C 02AAA825 */  or         $s5, $s5, $t2
    /* 10E90 80010290 32AB00FF */  andi       $t3, $s5, 0xFF
    /* 10E94 80010294 0160A825 */  or         $s5, $t3, $zero
    /* 10E98 80010298 26100001 */  addiu      $s0, $s0, 0x1
  .L8001029C:
    /* 10E9C 8001029C 0203082A */  slt        $at, $s0, $v1
    /* 10EA0 800102A0 1420FFF1 */  bnez       $at, .L80010268
    /* 10EA4 800102A4 24420004 */   addiu     $v0, $v0, 0x4
  .L800102A8:
    /* 10EA8 800102A8 0C00397D */  jal        __osSiRelAccess
    /* 10EAC 800102AC 00000000 */   nop
    /* 10EB0 800102B0 8FAC0074 */  lw         $t4, 0x74($sp)
    /* 10EB4 800102B4 A1950000 */  sb         $s5, 0x0($t4)
    /* 10EB8 800102B8 8FBF003C */  lw         $ra, 0x3C($sp)
    /* 10EBC 800102BC 8FBE0038 */  lw         $fp, 0x38($sp)
    /* 10EC0 800102C0 8FB70034 */  lw         $s7, 0x34($sp)
    /* 10EC4 800102C4 8FB60030 */  lw         $s6, 0x30($sp)
    /* 10EC8 800102C8 8FB5002C */  lw         $s5, 0x2C($sp)
    /* 10ECC 800102CC 8FB40028 */  lw         $s4, 0x28($sp)
    /* 10ED0 800102D0 8FB30024 */  lw         $s3, 0x24($sp)
    /* 10ED4 800102D4 8FB20020 */  lw         $s2, 0x20($sp)
    /* 10ED8 800102D8 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 10EDC 800102DC 8FB00018 */  lw         $s0, 0x18($sp)
    /* 10EE0 800102E0 8FA2006C */  lw         $v0, 0x6C($sp)
    /* 10EE4 800102E4 03E00008 */  jr         $ra
    /* 10EE8 800102E8 27BD0070 */   addiu     $sp, $sp, 0x70
endlabel osPfsIsPlug
