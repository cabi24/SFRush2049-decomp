nonmatching func_80018D40, 0xEC

glabel func_80018D40
    /* 19940 80018D40 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 19944 80018D44 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 19948 80018D48 0C005D91 */  jal        func_80017644
    /* 1994C 80018D4C AFB00018 */   sw        $s0, 0x18($sp)
    /* 19950 80018D50 2401FFFF */  addiu      $at, $zero, -0x1
    /* 19954 80018D54 10410030 */  beq        $v0, $at, .L80018E18
    /* 19958 80018D58 00408025 */   or        $s0, $v0, $zero
    /* 1995C 80018D5C 00027000 */  sll        $t6, $v0, 0
    /* 19960 80018D60 05C0001D */  bltz       $t6, .L80018DD8
    /* 19964 80018D64 24060FF8 */   addiu     $a2, $zero, 0xFF8
    /* 19968 80018D68 00460019 */  multu      $v0, $a2
    /* 1996C 80018D6C 3C058004 */  lui        $a1, %hi(D_80043EB8)
    /* 19970 80018D70 24A53EB8 */  addiu      $a1, $a1, %lo(D_80043EB8)
    /* 19974 80018D74 00007812 */  mflo       $t7
    /* 19978 80018D78 00AF1821 */  addu       $v1, $a1, $t7
    /* 1997C 80018D7C 90780FC0 */  lbu        $t8, 0xFC0($v1)
    /* 19980 80018D80 1300000F */  beqz       $t8, .L80018DC0
    /* 19984 80018D84 00000000 */   nop
    /* 19988 80018D88 90790FC1 */  lbu        $t9, 0xFC1($v1)
    /* 1998C 80018D8C 1720000C */  bnez       $t9, .L80018DC0
    /* 19990 80018D90 00000000 */   nop
    /* 19994 80018D94 02060019 */  multu      $s0, $a2
    /* 19998 80018D98 00004012 */  mflo       $t0
    /* 1999C 80018D9C 00A82021 */  addu       $a0, $a1, $t0
    /* 199A0 80018DA0 A0800FC0 */  sb         $zero, 0xFC0($a0)
    /* 199A4 80018DA4 0C005CD3 */  jal        func_8001734C
    /* 199A8 80018DA8 AFA40024 */   sw        $a0, 0x24($sp)
    /* 199AC 80018DAC 0C005CA7 */  jal        func_8001729C
    /* 199B0 80018DB0 8FA40024 */   lw        $a0, 0x24($sp)
    /* 199B4 80018DB4 3C058004 */  lui        $a1, %hi(D_80043EB8)
    /* 199B8 80018DB8 24A53EB8 */  addiu      $a1, $a1, %lo(D_80043EB8)
    /* 199BC 80018DBC 24060FF8 */  addiu      $a2, $zero, 0xFF8
  .L80018DC0:
    /* 199C0 80018DC0 02060019 */  multu      $s0, $a2
    /* 199C4 80018DC4 24090001 */  addiu      $t1, $zero, 0x1
    /* 199C8 80018DC8 00005012 */  mflo       $t2
    /* 199CC 80018DCC 00AA5821 */  addu       $t3, $a1, $t2
    /* 199D0 80018DD0 10000011 */  b          .L80018E18
    /* 199D4 80018DD4 A1690FC1 */   sb        $t1, 0xFC1($t3)
  .L80018DD8:
    /* 199D8 80018DD8 3C017FFF */  lui        $at, (0x7FFFFFFF >> 16)
    /* 199DC 80018DDC 3421FFFF */  ori        $at, $at, (0x7FFFFFFF & 0xFFFF)
    /* 199E0 80018DE0 00418024 */  and        $s0, $v0, $at
    /* 199E4 80018DE4 00106240 */  sll        $t4, $s0, 9
    /* 199E8 80018DE8 01906023 */  subu       $t4, $t4, $s0
    /* 199EC 80018DEC 3C0D8004 */  lui        $t5, %hi(D_80043EB8)
    /* 199F0 80018DF0 25AD3EB8 */  addiu      $t5, $t5, %lo(D_80043EB8)
    /* 199F4 80018DF4 000C60C0 */  sll        $t4, $t4, 3
    /* 199F8 80018DF8 018D2021 */  addu       $a0, $t4, $t5
    /* 199FC 80018DFC 908E0FC0 */  lbu        $t6, 0xFC0($a0)
    /* 19A00 80018E00 51C00006 */  beql       $t6, $zero, .L80018E1C
    /* 19A04 80018E04 8FBF001C */   lw        $ra, 0x1C($sp)
    /* 19A08 80018E08 908F0FC1 */  lbu        $t7, 0xFC1($a0)
    /* 19A0C 80018E0C 55E00003 */  bnel       $t7, $zero, .L80018E1C
    /* 19A10 80018E10 8FBF001C */   lw        $ra, 0x1C($sp)
    /* 19A14 80018E14 AC800FF0 */  sw         $zero, 0xFF0($a0)
  .L80018E18:
    /* 19A18 80018E18 8FBF001C */  lw         $ra, 0x1C($sp)
  .L80018E1C:
    /* 19A1C 80018E1C 8FB00018 */  lw         $s0, 0x18($sp)
    /* 19A20 80018E20 27BD0028 */  addiu      $sp, $sp, 0x28
    /* 19A24 80018E24 03E00008 */  jr         $ra
    /* 19A28 80018E28 00000000 */   nop
endlabel func_80018D40
