# Source: A810.s
# Address: 0x80009C10

glabel func_80009C10
    /* A810 80009C10 27BDFF90 */  addiu      $sp, $sp, -0x70
    /* A814 80009C14 AFBF001C */  sw         $ra, 0x1C($sp)
    /* A818 80009C18 AFB00018 */  sw         $s0, 0x18($sp)
    /* A81C 80009C1C 00A08025 */  or         $s0, $a1, $zero
    /* A820 80009C20 AFA40070 */  sw         $a0, 0x70($sp)
    /* A824 80009C24 0C00396C */  jal        func_8000E5B0
    /* A828 80009C28 AFA60078 */   sw        $a2, 0x78($sp)
    /* A82C 80009C2C 8FA40070 */  lw         $a0, 0x70($sp)
    /* A830 80009C30 0C003988 */  jal        func_8000E620
    /* A834 80009C34 8FA50078 */   lw        $a1, 0x78($sp)
    /* A838 80009C38 0C00397D */  jal        func_8000E5F4
    /* A83C 80009C3C AFA2006C */   sw        $v0, 0x6C($sp)
    /* A840 80009C40 8FA3006C */  lw         $v1, 0x6C($sp)
    /* A844 80009C44 8FAE0070 */  lw         $t6, 0x70($sp)
    /* A848 80009C48 02002025 */  or         $a0, $s0, $zero
    /* A84C 80009C4C 50600004 */  beql       $v1, $zero, .L80009C60
    /* A850 80009C50 AE0E0004 */   sw        $t6, 0x4($s0)
    /* A854 80009C54 1000006B */  b          .L80009E04
    /* A858 80009C58 00601025 */   or        $v0, $v1, $zero
    /* A85C 80009C5C AE0E0004 */  sw         $t6, 0x4($s0)
  .L80009C60:
    /* A860 80009C60 8FAF0078 */  lw         $t7, 0x78($sp)
    /* A864 80009C64 AE000000 */  sw         $zero, 0x0($s0)
    /* A868 80009C68 0C002786 */  jal        func_80009E18
    /* A86C 80009C6C AE0F0008 */   sw        $t7, 0x8($s0)
    /* A870 80009C70 10400003 */  beqz       $v0, .L80009C80
    /* A874 80009C74 02002025 */   or        $a0, $s0, $zero
    /* A878 80009C78 10000063 */  b          .L80009E08
    /* A87C 80009C7C 8FBF001C */   lw        $ra, 0x1C($sp)
  .L80009C80:
    /* A880 80009C80 0C003A14 */  jal        func_8000E850
    /* A884 80009C84 00002825 */   or        $a1, $zero, $zero
    /* A888 80009C88 10400003 */  beqz       $v0, .L80009C98
    /* A88C 80009C8C 24060001 */   addiu     $a2, $zero, 0x1
    /* A890 80009C90 1000005D */  b          .L80009E08
    /* A894 80009C94 8FBF001C */   lw        $ra, 0x1C($sp)
  .L80009C98:
    /* A898 80009C98 8E040004 */  lw         $a0, 0x4($s0)
    /* A89C 80009C9C 8E050008 */  lw         $a1, 0x8($s0)
    /* A8A0 80009CA0 0C003A34 */  jal        func_8000E8D0
    /* A8A4 80009CA4 27A70048 */   addiu     $a3, $sp, 0x48
    /* A8A8 80009CA8 10400003 */  beqz       $v0, .L80009CB8
    /* A8AC 80009CAC 27A40048 */   addiu     $a0, $sp, 0x48
    /* A8B0 80009CB0 10000055 */  b          .L80009E08
    /* A8B4 80009CB4 8FBF001C */   lw        $ra, 0x1C($sp)
  .L80009CB8:
    /* A8B8 80009CB8 27A5006A */  addiu      $a1, $sp, 0x6A
    /* A8BC 80009CBC 0C003ADD */  jal        func_8000EB74
    /* A8C0 80009CC0 27A60068 */   addiu     $a2, $sp, 0x68
    /* A8C4 80009CC4 97B8006A */  lhu        $t8, 0x6A($sp)
    /* A8C8 80009CC8 97B90064 */  lhu        $t9, 0x64($sp)
    /* A8CC 80009CCC 27A50048 */  addiu      $a1, $sp, 0x48
    /* A8D0 80009CD0 AFA50044 */  sw         $a1, 0x44($sp)
    /* A8D4 80009CD4 17190004 */  bne        $t8, $t9, .L80009CE8
    /* A8D8 80009CD8 97A90068 */   lhu       $t1, 0x68($sp)
    /* A8DC 80009CDC 97AA0066 */  lhu        $t2, 0x66($sp)
    /* A8E0 80009CE0 512A000A */  beql       $t1, $t2, .L80009D0C
    /* A8E4 80009CE4 97AD0060 */   lhu       $t5, 0x60($sp)
  .L80009CE8:
    /* A8E8 80009CE8 0C003BF0 */  jal        func_8000EFC0
    /* A8EC 80009CEC 02002025 */   or        $a0, $s0, $zero
    /* A8F0 80009CF0 50400006 */  beql       $v0, $zero, .L80009D0C
    /* A8F4 80009CF4 97AD0060 */   lhu       $t5, 0x60($sp)
    /* A8F8 80009CF8 8E0B0000 */  lw         $t3, 0x0($s0)
    /* A8FC 80009CFC 356C0004 */  ori        $t4, $t3, 0x4
    /* A900 80009D00 10000040 */  b          .L80009E04
    /* A904 80009D04 AE0C0000 */   sw        $t4, 0x0($s0)
    /* A908 80009D08 97AD0060 */  lhu        $t5, 0x60($sp)
  .L80009D0C:
    /* A90C 80009D0C 27A50048 */  addiu      $a1, $sp, 0x48
    /* A910 80009D10 02002025 */  or         $a0, $s0, $zero
    /* A914 80009D14 31AE0001 */  andi       $t6, $t5, 0x1
    /* A918 80009D18 55C00016 */  bnel       $t6, $zero, .L80009D74
    /* A91C 80009D1C 8FA40044 */   lw        $a0, 0x44($sp)
    /* A920 80009D20 0C003B1C */  jal        func_8000EC70
    /* A924 80009D24 27A60020 */   addiu     $a2, $sp, 0x20
    /* A928 80009D28 10400009 */  beqz       $v0, .L80009D50
    /* A92C 80009D2C 00401825 */   or        $v1, $v0, $zero
    /* A930 80009D30 2401000A */  addiu      $at, $zero, 0xA
    /* A934 80009D34 14410004 */  bne        $v0, $at, .L80009D48
    /* A938 80009D38 00000000 */   nop
    /* A93C 80009D3C 8E0F0000 */  lw         $t7, 0x0($s0)
    /* A940 80009D40 35F80004 */  ori        $t8, $t7, 0x4
    /* A944 80009D44 AE180000 */  sw         $t8, 0x0($s0)
  .L80009D48:
    /* A948 80009D48 1000002E */  b          .L80009E04
    /* A94C 80009D4C 00601025 */   or        $v0, $v1, $zero
  .L80009D50:
    /* A950 80009D50 97A90038 */  lhu        $t1, 0x38($sp)
    /* A954 80009D54 27B90020 */  addiu      $t9, $sp, 0x20
    /* A958 80009D58 AFB90044 */  sw         $t9, 0x44($sp)
    /* A95C 80009D5C 312A0001 */  andi       $t2, $t1, 0x1
    /* A960 80009D60 55400004 */  bnel       $t2, $zero, .L80009D74
    /* A964 80009D64 8FA40044 */   lw        $a0, 0x44($sp)
    /* A968 80009D68 10000026 */  b          .L80009E04
    /* A96C 80009D6C 2402000B */   addiu     $v0, $zero, 0xB
    /* A970 80009D70 8FA40044 */  lw         $a0, 0x44($sp)
  .L80009D74:
    /* A974 80009D74 2605000C */  addiu      $a1, $s0, 0xC
    /* A978 80009D78 0C0034AC */  jal        func_8000D2B0
    /* A97C 80009D7C 24060020 */   addiu     $a2, $zero, 0x20
    /* A980 80009D80 8FAB0044 */  lw         $t3, 0x44($sp)
    /* A984 80009D84 24190010 */  addiu      $t9, $zero, 0x10
    /* A988 80009D88 24090008 */  addiu      $t1, $zero, 0x8
    /* A98C 80009D8C 916C001B */  lbu        $t4, 0x1B($t3)
    /* A990 80009D90 8E040004 */  lw         $a0, 0x4($s0)
    /* A994 80009D94 8E050008 */  lw         $a1, 0x8($s0)
    /* A998 80009D98 AE0C004C */  sw         $t4, 0x4C($s0)
    /* A99C 80009D9C 8FAD0044 */  lw         $t5, 0x44($sp)
    /* A9A0 80009DA0 24060007 */  addiu      $a2, $zero, 0x7
    /* A9A4 80009DA4 2607002C */  addiu      $a3, $s0, 0x2C
    /* A9A8 80009DA8 91AE001A */  lbu        $t6, 0x1A($t5)
    /* A9AC 80009DAC AE190050 */  sw         $t9, 0x50($s0)
    /* A9B0 80009DB0 AE090054 */  sw         $t1, 0x54($s0)
    /* A9B4 80009DB4 31C200FF */  andi       $v0, $t6, 0xFF
    /* A9B8 80009DB8 000218C0 */  sll        $v1, $v0, 3
    /* A9BC 80009DBC 00027840 */  sll        $t7, $v0, 1
    /* A9C0 80009DC0 24680008 */  addiu      $t0, $v1, 0x8
    /* A9C4 80009DC4 25F80003 */  addiu      $t8, $t7, 0x3
    /* A9C8 80009DC8 01035021 */  addu       $t2, $t0, $v1
    /* A9CC 80009DCC AE180060 */  sw         $t8, 0x60($s0)
    /* A9D0 80009DD0 AE080058 */  sw         $t0, 0x58($s0)
    /* A9D4 80009DD4 AE0A005C */  sw         $t2, 0x5C($s0)
    /* A9D8 80009DD8 0C003A34 */  jal        func_8000E8D0
    /* A9DC 80009DDC A20E0064 */   sb        $t6, 0x64($s0)
    /* A9E0 80009DE0 10400003 */  beqz       $v0, .L80009DF0
    /* A9E4 80009DE4 00000000 */   nop
    /* A9E8 80009DE8 10000007 */  b          .L80009E08
    /* A9EC 80009DEC 8FBF001C */   lw        $ra, 0x1C($sp)
  .L80009DF0:
    /* A9F0 80009DF0 0C002CF8 */  jal        func_8000B3E0
    /* A9F4 80009DF4 02002025 */   or        $a0, $s0, $zero
    /* A9F8 80009DF8 8E0B0000 */  lw         $t3, 0x0($s0)
    /* A9FC 80009DFC 356C0001 */  ori        $t4, $t3, 0x1
    /* AA00 80009E00 AE0C0000 */  sw         $t4, 0x0($s0)
  .L80009E04:
    /* AA04 80009E04 8FBF001C */  lw         $ra, 0x1C($sp)
  .L80009E08:
    /* AA08 80009E08 8FB00018 */  lw         $s0, 0x18($sp)
    /* AA0C 80009E0C 27BD0070 */  addiu      $sp, $sp, 0x70
    /* AA10 80009E10 03E00008 */  jr         $ra
    /* AA14 80009E14 00000000 */   nop