# Source: D580.s
# Address: 0x8000CFC4

glabel func_8000CFC4
    /* DBC4 8000CFC4 3C058003 */  lui        $a1, %hi(D_8002C3E0)
    /* DBC8 8000CFC8 8CA5C3E0 */  lw         $a1, %lo(D_8002C3E0)($a1)
    /* DBCC 8000CFCC 40086000 */  mfc0       $t0, $12 /* handwritten instruction */
    /* DBD0 8000CFD0 8CBB0018 */  lw         $k1, 0x18($a1) /* handwritten instruction */
    /* DBD4 8000CFD4 35080002 */  ori        $t0, $t0, 0x2
    /* DBD8 8000CFD8 ACA80118 */  sw         $t0, 0x118($a1)
    /* DBDC 8000CFDC FCB00098 */  sd         $s0, 0x98($a1)
    /* DBE0 8000CFE0 FCB100A0 */  sd         $s1, 0xA0($a1)
    /* DBE4 8000CFE4 FCB200A8 */  sd         $s2, 0xA8($a1)
    /* DBE8 8000CFE8 FCB300B0 */  sd         $s3, 0xB0($a1)
    /* DBEC 8000CFEC FCB400B8 */  sd         $s4, 0xB8($a1)
    /* DBF0 8000CFF0 FCB500C0 */  sd         $s5, 0xC0($a1)
    /* DBF4 8000CFF4 FCB600C8 */  sd         $s6, 0xC8($a1)
    /* DBF8 8000CFF8 FCB700D0 */  sd         $s7, 0xD0($a1)
    /* DBFC 8000CFFC FCBC00E8 */  sd         $gp, 0xE8($a1)
    /* DC00 8000D000 FCBD00F0 */  sd         $sp, 0xF0($a1)
    /* DC04 8000D004 FCBE00F8 */  sd         $fp, 0xF8($a1)
    /* DC08 8000D008 FCBF0100 */  sd         $ra, 0x100($a1)
    /* DC0C 8000D00C 13600009 */  beqz       $k1, .L8000D034 /* handwritten instruction */
    /* DC10 8000D010 ACBF011C */   sw        $ra, 0x11C($a1)
    /* DC14 8000D014 445BF800 */  cfc1       $k1, $31
    /* DC18 8000D018 F4B40180 */  sdc1       $fs0, 0x180($a1)
    /* DC1C 8000D01C F4B60188 */  sdc1       $fs1, 0x188($a1)
    /* DC20 8000D020 F4B80190 */  sdc1       $fs2, 0x190($a1)
    /* DC24 8000D024 F4BA0198 */  sdc1       $fs3, 0x198($a1)
    /* DC28 8000D028 F4BC01A0 */  sdc1       $fs4, 0x1A0($a1)
    /* DC2C 8000D02C F4BE01A8 */  sdc1       $fs5, 0x1A8($a1)
    /* DC30 8000D030 ACBB012C */  sw         $k1, 0x12C($a1) /* handwritten instruction */
  .L8000D034:
    /* DC34 8000D034 8CBB0118 */  lw         $k1, 0x118($a1) /* handwritten instruction */
    /* DC38 8000D038 3369FF00 */  andi       $t1, $k1, 0xFF00 /* handwritten instruction */
    /* DC3C 8000D03C 1120000D */  beqz       $t1, .L8000D074
    /* DC40 8000D040 00000000 */   nop
    /* DC44 8000D044 3C088003 */  lui        $t0, %hi(D_8002C370)
    /* DC48 8000D048 2508C370 */  addiu      $t0, $t0, %lo(D_8002C370)
    /* DC4C 8000D04C 8D080000 */  lw         $t0, 0x0($t0)
    /* DC50 8000D050 2401FFFF */  addiu      $at, $zero, -0x1
    /* DC54 8000D054 01014026 */  xor        $t0, $t0, $at
    /* DC58 8000D058 3C01FFFF */  lui        $at, (0xFFFF00FF >> 16)
    /* DC5C 8000D05C 3108FF00 */  andi       $t0, $t0, 0xFF00
    /* DC60 8000D060 342100FF */  ori        $at, $at, (0xFFFF00FF & 0xFFFF)
    /* DC64 8000D064 01284825 */  or         $t1, $t1, $t0
    /* DC68 8000D068 0361D824 */  and        $k1, $k1, $at
    /* DC6C 8000D06C 0369D825 */  or         $k1, $k1, $t1
    /* DC70 8000D070 ACBB0118 */  sw         $k1, 0x118($a1) /* handwritten instruction */
  .L8000D074:
    /* DC74 8000D074 3C1BA430 */  lui        $k1, %hi(MI_INTR_MASK_REG) /* handwritten instruction */
    /* DC78 8000D078 8F7B000C */  lw         $k1, %lo(MI_INTR_MASK_REG)($k1) /* handwritten instruction */
    /* DC7C 8000D07C 1360000B */  beqz       $k1, .L8000D0AC /* handwritten instruction */
    /* DC80 8000D080 00000000 */   nop
    /* DC84 8000D084 3C1A8003 */  lui        $k0, %hi(D_8002C370) /* handwritten instruction */
    /* DC88 8000D088 275AC370 */  addiu      $k0, $k0, %lo(D_8002C370) /* handwritten instruction */
    /* DC8C 8000D08C 8F5A0000 */  lw         $k0, 0x0($k0) /* handwritten instruction */
    /* DC90 8000D090 8CA80128 */  lw         $t0, 0x128($a1)
    /* DC94 8000D094 2401FFFF */  addiu      $at, $zero, -0x1
    /* DC98 8000D098 001AD402 */  srl        $k0, $k0, 16
    /* DC9C 8000D09C 0341D026 */  xor        $k0, $k0, $at
    /* DCA0 8000D0A0 335A003F */  andi       $k0, $k0, 0x3F /* handwritten instruction */
    /* DCA4 8000D0A4 0348D024 */  and        $k0, $k0, $t0
    /* DCA8 8000D0A8 037AD825 */  or         $k1, $k1, $k0
  .L8000D0AC:
    /* DCAC 8000D0AC 10800003 */  beqz       $a0, .L8000D0BC
    /* DCB0 8000D0B0 ACBB0128 */   sw        $k1, 0x128($a1) /* handwritten instruction */
    /* DCB4 8000D0B4 0C003431 */  jal        func_8000D0C4
    /* DCB8 8000D0B8 00000000 */   nop
  .L8000D0BC:
    /* DCBC 8000D0BC 08003447 */  j          func_8000D11C
    /* DCC0 8000D0C0 00000000 */   nop