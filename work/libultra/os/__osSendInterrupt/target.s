# Source: D580.s
# Address: 0x8000CEDC

glabel func_8000CEDC
    /* DADC 8000CEDC 3C0A8003 */  lui        $t2, %hi(D_80036710)
    /* DAE0 8000CEE0 254A6710 */  addiu      $t2, $t2, %lo(D_80036710)
    /* DAE4 8000CEE4 01445021 */  addu       $t2, $t2, $a0
    /* DAE8 8000CEE8 8D490000 */  lw         $t1, 0x0($t2)
    /* DAEC 8000CEEC 03E09025 */  or         $s2, $ra, $zero
    /* DAF0 8000CEF0 11200025 */  beqz       $t1, .L8000CF88
    /* DAF4 8000CEF4 00000000 */   nop
    /* DAF8 8000CEF8 8D2B0008 */  lw         $t3, 0x8($t1)
    /* DAFC 8000CEFC 8D2C0010 */  lw         $t4, 0x10($t1)
    /* DB00 8000CF00 016C082A */  slt        $at, $t3, $t4
    /* DB04 8000CF04 10200020 */  beqz       $at, .L8000CF88
    /* DB08 8000CF08 00000000 */   nop
    /* DB0C 8000CF0C 8D2D000C */  lw         $t5, 0xC($t1)
    /* DB10 8000CF10 01AB6821 */  addu       $t5, $t5, $t3
    /* DB14 8000CF14 01AC001A */  div        $zero, $t5, $t4
    /* DB18 8000CF18 15800002 */  bnez       $t4, .L8000CF24
    /* DB1C 8000CF1C 00000000 */   nop
    /* DB20 8000CF20 0007000D */  break      7
  .L8000CF24:
    /* DB24 8000CF24 2401FFFF */  addiu      $at, $zero, -0x1
    /* DB28 8000CF28 15810004 */  bne        $t4, $at, .L8000CF3C
    /* DB2C 8000CF2C 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* DB30 8000CF30 15A10002 */  bne        $t5, $at, .L8000CF3C
    /* DB34 8000CF34 00000000 */   nop
    /* DB38 8000CF38 0006000D */  break      6
  .L8000CF3C:
    /* DB3C 8000CF3C 8D2C0014 */  lw         $t4, 0x14($t1)
    /* DB40 8000CF40 00006810 */  mfhi       $t5
    /* DB44 8000CF44 000D6880 */  sll        $t5, $t5, 2
    /* DB48 8000CF48 018D6021 */  addu       $t4, $t4, $t5
    /* DB4C 8000CF4C 8D4D0004 */  lw         $t5, 0x4($t2)
    /* DB50 8000CF50 256A0001 */  addiu      $t2, $t3, 0x1
    /* DB54 8000CF54 AD8D0000 */  sw         $t5, 0x0($t4)
    /* DB58 8000CF58 AD2A0008 */  sw         $t2, 0x8($t1)
    /* DB5C 8000CF5C 8D2A0000 */  lw         $t2, 0x0($t1)
    /* DB60 8000CF60 8D4B0000 */  lw         $t3, 0x0($t2)
    /* DB64 8000CF64 11600008 */  beqz       $t3, .L8000CF88
    /* DB68 8000CF68 00000000 */   nop
    /* DB6C 8000CF6C 0C003443 */  jal        func_8000D10C
    /* DB70 8000CF70 01202025 */   or        $a0, $t1, $zero
    /* DB74 8000CF74 00405025 */  or         $t2, $v0, $zero
    /* DB78 8000CF78 3C048003 */  lui        $a0, %hi(D_8002C3D8)
    /* DB7C 8000CF7C 01402825 */  or         $a1, $t2, $zero
    /* DB80 8000CF80 0C003431 */  jal        func_8000D0C4
    /* DB84 8000CF84 2484C3D8 */   addiu     $a0, $a0, %lo(D_8002C3D8)
  .L8000CF88:
    /* DB88 8000CF88 02400008 */  jr         $s2
    /* DB8C 8000CF8C 00000000 */   nop
  .L8000CF90:
    /* DB90 8000CF90 3C013000 */  lui        $at, (0x30000000 >> 16)
    /* DB94 8000CF94 01014824 */  and        $t1, $t0, $at
    /* DB98 8000CF98 00094F02 */  srl        $t1, $t1, 28
    /* DB9C 8000CF9C 240A0001 */  addiu      $t2, $zero, 0x1
    /* DBA0 8000CFA0 152AFFC2 */  bne        $t1, $t2, .L8000CEAC
    /* DBA4 8000CFA4 00000000 */   nop
    /* DBA8 8000CFA8 8F5B0118 */  lw         $k1, 0x118($k0) /* handwritten instruction */
    /* DBAC 8000CFAC 3C012000 */  lui        $at, (0x20000000 >> 16)
    /* DBB0 8000CFB0 24090001 */  addiu      $t1, $zero, 0x1
    /* DBB4 8000CFB4 0361D825 */  or         $k1, $k1, $at
    /* DBB8 8000CFB8 AF490018 */  sw         $t1, 0x18($k0) /* handwritten instruction */
    /* DBBC 8000CFBC 1000FFB5 */  b          .L8000CE94
    /* DBC0 8000CFC0 AF5B0118 */   sw        $k1, 0x118($k0) /* handwritten instruction */