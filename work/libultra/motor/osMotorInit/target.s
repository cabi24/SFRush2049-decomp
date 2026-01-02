# Source: AB20.s
# Address: 0x80009F20

glabel func_80009F20
    /* AB20 80009F20 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* AB24 80009F24 AFBF001C */  sw         $ra, 0x1C($sp)
    /* AB28 80009F28 AFB00018 */  sw         $s0, 0x18($sp)
    /* AB2C 80009F2C 8C8E0008 */  lw         $t6, 0x8($a0)
    /* AB30 80009F30 8C990000 */  lw         $t9, 0x0($a0)
    /* AB34 80009F34 3C188003 */  lui        $t8, %hi(D_80037B30)
    /* AB38 80009F38 27187B30 */  addiu      $t8, $t8, %lo(D_80037B30)
    /* AB3C 80009F3C 000E7980 */  sll        $t7, $t6, 6
    /* AB40 80009F40 33290008 */  andi       $t1, $t9, 0x8
    /* AB44 80009F44 00808025 */  or         $s0, $a0, $zero
    /* AB48 80009F48 00A03825 */  or         $a3, $a1, $zero
    /* AB4C 80009F4C 15200003 */  bnez       $t1, .L80009F5C
    /* AB50 80009F50 01F84021 */   addu      $t0, $t7, $t8
    /* AB54 80009F54 10000047 */  b          .L8000A074
    /* AB58 80009F58 24020005 */   addiu     $v0, $zero, 0x5
  .L80009F5C:
    /* AB5C 80009F5C AFA70034 */  sw         $a3, 0x34($sp)
    /* AB60 80009F60 0C00396C */  jal        func_8000E5B0
    /* AB64 80009F64 AFA80024 */   sw        $t0, 0x24($sp)
    /* AB68 80009F68 8E0B0008 */  lw         $t3, 0x8($s0)
    /* AB6C 80009F6C 3C068003 */  lui        $a2, %hi(D_80037B30)
    /* AB70 80009F70 24C67B30 */  addiu      $a2, $a2, %lo(D_80037B30)
    /* AB74 80009F74 000B6180 */  sll        $t4, $t3, 6
    /* AB78 80009F78 00CC6821 */  addu       $t5, $a2, $t4
    /* AB7C 80009F7C 240A0001 */  addiu      $t2, $zero, 0x1
    /* AB80 80009F80 ADAA003C */  sw         $t2, 0x3C($t5)
    /* AB84 80009F84 8FA80024 */  lw         $t0, 0x24($sp)
    /* AB88 80009F88 8E0E0008 */  lw         $t6, 0x8($s0)
    /* AB8C 80009F8C 8FA70034 */  lw         $a3, 0x34($sp)
    /* AB90 80009F90 00001825 */  or         $v1, $zero, $zero
    /* AB94 80009F94 010E4021 */  addu       $t0, $t0, $t6
    /* AB98 80009F98 01001025 */  or         $v0, $t0, $zero
    /* AB9C 80009F9C 24040020 */  addiu      $a0, $zero, 0x20
  .L80009FA0:
    /* ABA0 80009FA0 24630004 */  addiu      $v1, $v1, 0x4
    /* ABA4 80009FA4 A0470007 */  sb         $a3, 0x7($v0)
    /* ABA8 80009FA8 A0470008 */  sb         $a3, 0x8($v0)
    /* ABAC 80009FAC A0470009 */  sb         $a3, 0x9($v0)
    /* ABB0 80009FB0 24420004 */  addiu      $v0, $v0, 0x4
    /* ABB4 80009FB4 1464FFFA */  bne        $v1, $a0, .L80009FA0
    /* ABB8 80009FB8 A0470002 */   sb        $a3, 0x2($v0)
    /* ABBC 80009FBC 240F00FE */  addiu      $t7, $zero, 0xFE
    /* ABC0 80009FC0 3C018003 */  lui        $at, %hi(D_80037AE0)
    /* ABC4 80009FC4 A02F7AE0 */  sb         $t7, %lo(D_80037AE0)($at)
    /* ABC8 80009FC8 8E180008 */  lw         $t8, 0x8($s0)
    /* ABCC 80009FCC AFA80024 */  sw         $t0, 0x24($sp)
    /* ABD0 80009FD0 AFA70034 */  sw         $a3, 0x34($sp)
    /* ABD4 80009FD4 0018C980 */  sll        $t9, $t8, 6
    /* ABD8 80009FD8 00D92821 */  addu       $a1, $a2, $t9
    /* ABDC 80009FDC 0C00392C */  jal        func_8000E4B0
    /* ABE0 80009FE0 24040001 */   addiu     $a0, $zero, 0x1
    /* ABE4 80009FE4 8E040004 */  lw         $a0, 0x4($s0)
    /* ABE8 80009FE8 00002825 */  or         $a1, $zero, $zero
    /* ABEC 80009FEC 0C001C9C */  jal        func_80007270
    /* ABF0 80009FF0 24060001 */   addiu     $a2, $zero, 0x1
    /* ABF4 80009FF4 8E090008 */  lw         $t1, 0x8($s0)
    /* ABF8 80009FF8 3C0C8003 */  lui        $t4, %hi(D_80037B30)
    /* ABFC 80009FFC 258C7B30 */  addiu      $t4, $t4, %lo(D_80037B30)
    /* AC00 8000A000 00095980 */  sll        $t3, $t1, 6
    /* AC04 8000A004 016C2821 */  addu       $a1, $t3, $t4
    /* AC08 8000A008 0C00392C */  jal        func_8000E4B0
    /* AC0C 8000A00C 00002025 */   or        $a0, $zero, $zero
    /* AC10 8000A010 8E040004 */  lw         $a0, 0x4($s0)
    /* AC14 8000A014 00002825 */  or         $a1, $zero, $zero
    /* AC18 8000A018 0C001C9C */  jal        func_80007270
    /* AC1C 8000A01C 24060001 */   addiu     $a2, $zero, 0x1
    /* AC20 8000A020 8FA80024 */  lw         $t0, 0x24($sp)
    /* AC24 8000A024 8FA70034 */  lw         $a3, 0x34($sp)
    /* AC28 8000A028 91100002 */  lbu        $s0, 0x2($t0)
    /* AC2C 8000A02C 320A00C0 */  andi       $t2, $s0, 0xC0
    /* AC30 8000A030 1540000D */  bnez       $t2, .L8000A068
    /* AC34 8000A034 01408025 */   or        $s0, $t2, $zero
    /* AC38 8000A038 54E00007 */  bnel       $a3, $zero, .L8000A058
    /* AC3C 8000A03C 910E0026 */   lbu       $t6, 0x26($t0)
    /* AC40 8000A040 910D0026 */  lbu        $t5, 0x26($t0)
    /* AC44 8000A044 11A00008 */  beqz       $t5, .L8000A068
    /* AC48 8000A048 00000000 */   nop
    /* AC4C 8000A04C 10000006 */  b          .L8000A068
    /* AC50 8000A050 24100004 */   addiu     $s0, $zero, 0x4
    /* AC54 8000A054 910E0026 */  lbu        $t6, 0x26($t0)
  .L8000A058:
    /* AC58 8000A058 240100EB */  addiu      $at, $zero, 0xEB
    /* AC5C 8000A05C 11C10002 */  beq        $t6, $at, .L8000A068
    /* AC60 8000A060 00000000 */   nop
    /* AC64 8000A064 24100004 */  addiu      $s0, $zero, 0x4
  .L8000A068:
    /* AC68 8000A068 0C00397D */  jal        func_8000E5F4
    /* AC6C 8000A06C 00000000 */   nop
    /* AC70 8000A070 02001025 */  or         $v0, $s0, $zero
  .L8000A074:
    /* AC74 8000A074 8FBF001C */  lw         $ra, 0x1C($sp)
    /* AC78 8000A078 8FB00018 */  lw         $s0, 0x18($sp)
    /* AC7C 8000A07C 27BD0030 */  addiu      $sp, $sp, 0x30
    /* AC80 8000A080 03E00008 */  jr         $ra
    /* AC84 8000A084 00000000 */   nop