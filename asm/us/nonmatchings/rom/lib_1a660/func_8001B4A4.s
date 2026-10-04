nonmatching func_8001B4A4, 0x150

glabel func_8001B4A4
    /* 1C0A4 8001B4A4 27BDFFC0 */  addiu      $sp, $sp, -0x40
    /* 1C0A8 8001B4A8 AFBF003C */  sw         $ra, 0x3C($sp)
    /* 1C0AC 8001B4AC AFB70034 */  sw         $s7, 0x34($sp)
    /* 1C0B0 8001B4B0 AFB40028 */  sw         $s4, 0x28($sp)
    /* 1C0B4 8001B4B4 30B4FFFF */  andi       $s4, $a1, 0xFFFF
    /* 1C0B8 8001B4B8 AFBE0038 */  sw         $fp, 0x38($sp)
    /* 1C0BC 8001B4BC AFB60030 */  sw         $s6, 0x30($sp)
    /* 1C0C0 8001B4C0 AFB5002C */  sw         $s5, 0x2C($sp)
    /* 1C0C4 8001B4C4 AFB30024 */  sw         $s3, 0x24($sp)
    /* 1C0C8 8001B4C8 AFB20020 */  sw         $s2, 0x20($sp)
    /* 1C0CC 8001B4CC AFB1001C */  sw         $s1, 0x1C($sp)
    /* 1C0D0 8001B4D0 AFB00018 */  sw         $s0, 0x18($sp)
    /* 1C0D4 8001B4D4 AFA50044 */  sw         $a1, 0x44($sp)
    /* 1C0D8 8001B4D8 0C007B7D */  jal        func_8001EDF4
    /* 1C0DC 8001B4DC 2417FFFF */   addiu     $s7, $zero, -0x1
    /* 1C0E0 8001B4E0 241EFFFF */  addiu      $fp, $zero, -0x1
    /* 1C0E4 8001B4E4 105E0036 */  beq        $v0, $fp, .L8001B5C0
    /* 1C0E8 8001B4E8 00408025 */   or        $s0, $v0, $zero
    /* 1C0EC 8001B4EC 3C158005 */  lui        $s5, %hi(D_8004BEB8)
    /* 1C0F0 8001B4F0 26B5BEB8 */  addiu      $s5, $s5, %lo(D_8004BEB8)
    /* 1C0F4 8001B4F4 241601A0 */  addiu      $s6, $zero, 0x1A0
    /* 1C0F8 8001B4F8 320200FF */  andi       $v0, $s0, 0xFF
  .L8001B4FC:
    /* 1C0FC 8001B4FC 00560019 */  multu      $v0, $s6
    /* 1C100 8001B500 001439C3 */  sra        $a3, $s4, 7
    /* 1C104 8001B504 00409825 */  or         $s3, $v0, $zero
    /* 1C108 8001B508 30E700FF */  andi       $a3, $a3, 0xFF
    /* 1C10C 8001B50C 24040084 */  addiu      $a0, $zero, 0x84
    /* 1C110 8001B510 00007012 */  mflo       $t6
    /* 1C114 8001B514 02AE8821 */  addu       $s1, $s5, $t6
    /* 1C118 8001B518 8A2F0060 */  lwl        $t7, 0x60($s1)
    /* 1C11C 8001B51C 9A2F0063 */  lwr        $t7, 0x63($s1)
    /* 1C120 8001B520 160F0023 */  bne        $s0, $t7, .L8001B5B0
    /* 1C124 8001B524 00000000 */   nop
    /* 1C128 8001B528 8A380024 */  lwl        $t8, 0x24($s1)
    /* 1C12C 8001B52C 9A380027 */  lwr        $t8, 0x27($s1)
    /* 1C130 8001B530 02809025 */  or         $s2, $s4, $zero
    /* 1C134 8001B534 3252007F */  andi       $s2, $s2, 0x7F
    /* 1C138 8001B538 33190002 */  andi       $t9, $t8, 0x2
    /* 1C13C 8001B53C 1320000D */  beqz       $t9, .L8001B574
    /* 1C140 8001B540 0000B825 */   or        $s7, $zero, $zero
    /* 1C144 8001B544 305000FF */  andi       $s0, $v0, 0xFF
    /* 1C148 8001B548 320500FF */  andi       $a1, $s0, 0xFF
    /* 1C14C 8001B54C 24040084 */  addiu      $a0, $zero, 0x84
    /* 1C150 8001B550 0C008184 */  jal        func_80020610
    /* 1C154 8001B554 92260055 */   lbu       $a2, 0x55($s1)
    /* 1C158 8001B558 24040085 */  addiu      $a0, $zero, 0x85
    /* 1C15C 8001B55C 320500FF */  andi       $a1, $s0, 0xFF
    /* 1C160 8001B560 92260055 */  lbu        $a2, 0x55($s1)
    /* 1C164 8001B564 0C008184 */  jal        func_80020610
    /* 1C168 8001B568 324700FF */   andi      $a3, $s2, 0xFF
    /* 1C16C 8001B56C 1000000A */  b          .L8001B598
    /* 1C170 8001B570 00000000 */   nop
  .L8001B574:
    /* 1C174 8001B574 305000FF */  andi       $s0, $v0, 0xFF
    /* 1C178 8001B578 320500FF */  andi       $a1, $s0, 0xFF
    /* 1C17C 8001B57C 0C008184 */  jal        func_80020610
    /* 1C180 8001B580 9226004B */   lbu       $a2, 0x4B($s1)
    /* 1C184 8001B584 24040085 */  addiu      $a0, $zero, 0x85
    /* 1C188 8001B588 320500FF */  andi       $a1, $s0, 0xFF
    /* 1C18C 8001B58C 9226004B */  lbu        $a2, 0x4B($s1)
    /* 1C190 8001B590 0C008184 */  jal        func_80020610
    /* 1C194 8001B594 324700FF */   andi      $a3, $s2, 0xFF
  .L8001B598:
    /* 1C198 8001B598 02760019 */  multu      $s3, $s6
    /* 1C19C 8001B59C 00004012 */  mflo       $t0
    /* 1C1A0 8001B5A0 02A84821 */  addu       $t1, $s5, $t0
    /* 1C1A4 8001B5A4 89300010 */  lwl        $s0, 0x10($t1)
    /* 1C1A8 8001B5A8 10000003 */  b          .L8001B5B8
    /* 1C1AC 8001B5AC 99300013 */   lwr       $s0, 0x13($t1)
  .L8001B5B0:
    /* 1C1B0 8001B5B0 10000004 */  b          .L8001B5C4
    /* 1C1B4 8001B5B4 02E01025 */   or        $v0, $s7, $zero
  .L8001B5B8:
    /* 1C1B8 8001B5B8 561EFFD0 */  bnel       $s0, $fp, .L8001B4FC
    /* 1C1BC 8001B5BC 320200FF */   andi      $v0, $s0, 0xFF
  .L8001B5C0:
    /* 1C1C0 8001B5C0 02E01025 */  or         $v0, $s7, $zero
  .L8001B5C4:
    /* 1C1C4 8001B5C4 8FBF003C */  lw         $ra, 0x3C($sp)
    /* 1C1C8 8001B5C8 8FB00018 */  lw         $s0, 0x18($sp)
    /* 1C1CC 8001B5CC 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 1C1D0 8001B5D0 8FB20020 */  lw         $s2, 0x20($sp)
    /* 1C1D4 8001B5D4 8FB30024 */  lw         $s3, 0x24($sp)
    /* 1C1D8 8001B5D8 8FB40028 */  lw         $s4, 0x28($sp)
    /* 1C1DC 8001B5DC 8FB5002C */  lw         $s5, 0x2C($sp)
    /* 1C1E0 8001B5E0 8FB60030 */  lw         $s6, 0x30($sp)
    /* 1C1E4 8001B5E4 8FB70034 */  lw         $s7, 0x34($sp)
    /* 1C1E8 8001B5E8 8FBE0038 */  lw         $fp, 0x38($sp)
    /* 1C1EC 8001B5EC 03E00008 */  jr         $ra
    /* 1C1F0 8001B5F0 27BD0040 */   addiu     $sp, $sp, 0x40
endlabel func_8001B4A4
