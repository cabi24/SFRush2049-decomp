nonmatching func_8001B1D0, 0xCC

glabel func_8001B1D0
    /* 1BDD0 8001B1D0 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* 1BDD4 8001B1D4 AFBF002C */  sw         $ra, 0x2C($sp)
    /* 1BDD8 8001B1D8 AFA40038 */  sw         $a0, 0x38($sp)
    /* 1BDDC 8001B1DC 2407FFFF */  addiu      $a3, $zero, -0x1
    /* 1BDE0 8001B1E0 3084FFFF */  andi       $a0, $a0, 0xFFFF
    /* 1BDE4 8001B1E4 AFA5003C */  sw         $a1, 0x3C($sp)
    /* 1BDE8 8001B1E8 AFA60040 */  sw         $a2, 0x40($sp)
    /* 1BDEC 8001B1EC 0C005C10 */  jal        func_80017040
    /* 1BDF0 8001B1F0 AFA70030 */   sw        $a3, 0x30($sp)
    /* 1BDF4 8001B1F4 93A6003F */  lbu        $a2, 0x3F($sp)
    /* 1BDF8 8001B1F8 8FA70030 */  lw         $a3, 0x30($sp)
    /* 1BDFC 8001B1FC 10400022 */  beqz       $v0, .L8001B288
    /* 1BE00 8001B200 00401825 */   or        $v1, $v0, $zero
    /* 1BE04 8001B204 240400FF */  addiu      $a0, $zero, 0xFF
    /* 1BE08 8001B208 14860002 */  bne        $a0, $a2, .L8001B214
    /* 1BE0C 8001B20C 93AE0043 */   lbu       $t6, 0x43($sp)
    /* 1BE10 8001B210 90460006 */  lbu        $a2, 0x6($v0)
  .L8001B214:
    /* 1BE14 8001B214 148E0003 */  bne        $a0, $t6, .L8001B224
    /* 1BE18 8001B218 240C00FF */   addiu     $t4, $zero, 0xFF
    /* 1BE1C 8001B21C 904F0007 */  lbu        $t7, 0x7($v0)
    /* 1BE20 8001B220 A3AF0043 */  sb         $t7, 0x43($sp)
  .L8001B224:
    /* 1BE24 8001B224 90610002 */  lbu        $at, 0x2($v1)
    /* 1BE28 8001B228 90780003 */  lbu        $t8, 0x3($v1)
    /* 1BE2C 8001B22C 90680005 */  lbu        $t0, 0x5($v1)
    /* 1BE30 8001B230 90650008 */  lbu        $a1, 0x8($v1)
    /* 1BE34 8001B234 906B0004 */  lbu        $t3, 0x4($v1)
    /* 1BE38 8001B238 240D00FF */  addiu      $t5, $zero, 0xFF
    /* 1BE3C 8001B23C 240E00FF */  addiu      $t6, $zero, 0xFF
    /* 1BE40 8001B240 00010A00 */  sll        $at, $at, 8
    /* 1BE44 8001B244 AFAE001C */  sw         $t6, 0x1C($sp)
    /* 1BE48 8001B248 AFAD0014 */  sw         $t5, 0x14($sp)
    /* 1BE4C 8001B24C AFA00018 */  sw         $zero, 0x18($sp)
    /* 1BE50 8001B250 AFAC0010 */  sw         $t4, 0x10($sp)
    /* 1BE54 8001B254 0301C025 */  or         $t8, $t8, $at
    /* 1BE58 8001B258 906F0009 */  lbu        $t7, 0x9($v1)
    /* 1BE5C 8001B25C 0018CC00 */  sll        $t9, $t8, 16
    /* 1BE60 8001B260 00084A00 */  sll        $t1, $t0, 8
    /* 1BE64 8001B264 03295025 */  or         $t2, $t9, $t1
    /* 1BE68 8001B268 34A50080 */  ori        $a1, $a1, 0x80
    /* 1BE6C 8001B26C 30A500FF */  andi       $a1, $a1, 0xFF
    /* 1BE70 8001B270 AFA00024 */  sw         $zero, 0x24($sp)
    /* 1BE74 8001B274 93A70043 */  lbu        $a3, 0x43($sp)
    /* 1BE78 8001B278 014B2025 */  or         $a0, $t2, $t3
    /* 1BE7C 8001B27C 0C00689C */  jal        func_8001A270
    /* 1BE80 8001B280 AFAF0020 */   sw        $t7, 0x20($sp)
    /* 1BE84 8001B284 00403825 */  or         $a3, $v0, $zero
  .L8001B288:
    /* 1BE88 8001B288 8FBF002C */  lw         $ra, 0x2C($sp)
    /* 1BE8C 8001B28C 27BD0038 */  addiu      $sp, $sp, 0x38
    /* 1BE90 8001B290 00E01025 */  or         $v0, $a3, $zero
    /* 1BE94 8001B294 03E00008 */  jr         $ra
    /* 1BE98 8001B298 00000000 */   nop
endlabel func_8001B1D0
