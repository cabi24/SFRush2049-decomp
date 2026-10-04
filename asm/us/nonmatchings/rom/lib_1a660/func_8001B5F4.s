nonmatching func_8001B5F4, 0x150

glabel func_8001B5F4
    /* 1C1F4 8001B5F4 27BDFFC0 */  addiu      $sp, $sp, -0x40
    /* 1C1F8 8001B5F8 AFBF003C */  sw         $ra, 0x3C($sp)
    /* 1C1FC 8001B5FC AFB70034 */  sw         $s7, 0x34($sp)
    /* 1C200 8001B600 AFB40028 */  sw         $s4, 0x28($sp)
    /* 1C204 8001B604 30B4FFFF */  andi       $s4, $a1, 0xFFFF
    /* 1C208 8001B608 AFBE0038 */  sw         $fp, 0x38($sp)
    /* 1C20C 8001B60C AFB60030 */  sw         $s6, 0x30($sp)
    /* 1C210 8001B610 AFB5002C */  sw         $s5, 0x2C($sp)
    /* 1C214 8001B614 AFB30024 */  sw         $s3, 0x24($sp)
    /* 1C218 8001B618 AFB20020 */  sw         $s2, 0x20($sp)
    /* 1C21C 8001B61C AFB1001C */  sw         $s1, 0x1C($sp)
    /* 1C220 8001B620 AFB00018 */  sw         $s0, 0x18($sp)
    /* 1C224 8001B624 AFA50044 */  sw         $a1, 0x44($sp)
    /* 1C228 8001B628 0C007B7D */  jal        func_8001EDF4
    /* 1C22C 8001B62C 2417FFFF */   addiu     $s7, $zero, -0x1
    /* 1C230 8001B630 241EFFFF */  addiu      $fp, $zero, -0x1
    /* 1C234 8001B634 105E0036 */  beq        $v0, $fp, .L8001B710
    /* 1C238 8001B638 00408025 */   or        $s0, $v0, $zero
    /* 1C23C 8001B63C 3C158005 */  lui        $s5, %hi(D_8004BEB8)
    /* 1C240 8001B640 26B5BEB8 */  addiu      $s5, $s5, %lo(D_8004BEB8)
    /* 1C244 8001B644 241601A0 */  addiu      $s6, $zero, 0x1A0
    /* 1C248 8001B648 320200FF */  andi       $v0, $s0, 0xFF
  .L8001B64C:
    /* 1C24C 8001B64C 00560019 */  multu      $v0, $s6
    /* 1C250 8001B650 001439C3 */  sra        $a3, $s4, 7
    /* 1C254 8001B654 00409825 */  or         $s3, $v0, $zero
    /* 1C258 8001B658 30E700FF */  andi       $a3, $a3, 0xFF
    /* 1C25C 8001B65C 24040080 */  addiu      $a0, $zero, 0x80
    /* 1C260 8001B660 00007012 */  mflo       $t6
    /* 1C264 8001B664 02AE8821 */  addu       $s1, $s5, $t6
    /* 1C268 8001B668 8A2F0060 */  lwl        $t7, 0x60($s1)
    /* 1C26C 8001B66C 9A2F0063 */  lwr        $t7, 0x63($s1)
    /* 1C270 8001B670 160F0023 */  bne        $s0, $t7, .L8001B700
    /* 1C274 8001B674 00000000 */   nop
    /* 1C278 8001B678 8A380024 */  lwl        $t8, 0x24($s1)
    /* 1C27C 8001B67C 9A380027 */  lwr        $t8, 0x27($s1)
    /* 1C280 8001B680 02809025 */  or         $s2, $s4, $zero
    /* 1C284 8001B684 3252007F */  andi       $s2, $s2, 0x7F
    /* 1C288 8001B688 33190002 */  andi       $t9, $t8, 0x2
    /* 1C28C 8001B68C 1320000D */  beqz       $t9, .L8001B6C4
    /* 1C290 8001B690 0000B825 */   or        $s7, $zero, $zero
    /* 1C294 8001B694 305000FF */  andi       $s0, $v0, 0xFF
    /* 1C298 8001B698 320500FF */  andi       $a1, $s0, 0xFF
    /* 1C29C 8001B69C 24040080 */  addiu      $a0, $zero, 0x80
    /* 1C2A0 8001B6A0 0C008184 */  jal        func_80020610
    /* 1C2A4 8001B6A4 92260055 */   lbu       $a2, 0x55($s1)
    /* 1C2A8 8001B6A8 24040081 */  addiu      $a0, $zero, 0x81
    /* 1C2AC 8001B6AC 320500FF */  andi       $a1, $s0, 0xFF
    /* 1C2B0 8001B6B0 92260055 */  lbu        $a2, 0x55($s1)
    /* 1C2B4 8001B6B4 0C008184 */  jal        func_80020610
    /* 1C2B8 8001B6B8 324700FF */   andi      $a3, $s2, 0xFF
    /* 1C2BC 8001B6BC 1000000A */  b          .L8001B6E8
    /* 1C2C0 8001B6C0 00000000 */   nop
  .L8001B6C4:
    /* 1C2C4 8001B6C4 305000FF */  andi       $s0, $v0, 0xFF
    /* 1C2C8 8001B6C8 320500FF */  andi       $a1, $s0, 0xFF
    /* 1C2CC 8001B6CC 0C008184 */  jal        func_80020610
    /* 1C2D0 8001B6D0 9226004B */   lbu       $a2, 0x4B($s1)
    /* 1C2D4 8001B6D4 24040081 */  addiu      $a0, $zero, 0x81
    /* 1C2D8 8001B6D8 320500FF */  andi       $a1, $s0, 0xFF
    /* 1C2DC 8001B6DC 9226004B */  lbu        $a2, 0x4B($s1)
    /* 1C2E0 8001B6E0 0C008184 */  jal        func_80020610
    /* 1C2E4 8001B6E4 324700FF */   andi      $a3, $s2, 0xFF
  .L8001B6E8:
    /* 1C2E8 8001B6E8 02760019 */  multu      $s3, $s6
    /* 1C2EC 8001B6EC 00004012 */  mflo       $t0
    /* 1C2F0 8001B6F0 02A84821 */  addu       $t1, $s5, $t0
    /* 1C2F4 8001B6F4 89300010 */  lwl        $s0, 0x10($t1)
    /* 1C2F8 8001B6F8 10000003 */  b          .L8001B708
    /* 1C2FC 8001B6FC 99300013 */   lwr       $s0, 0x13($t1)
  .L8001B700:
    /* 1C300 8001B700 10000004 */  b          .L8001B714
    /* 1C304 8001B704 02E01025 */   or        $v0, $s7, $zero
  .L8001B708:
    /* 1C308 8001B708 561EFFD0 */  bnel       $s0, $fp, .L8001B64C
    /* 1C30C 8001B70C 320200FF */   andi      $v0, $s0, 0xFF
  .L8001B710:
    /* 1C310 8001B710 02E01025 */  or         $v0, $s7, $zero
  .L8001B714:
    /* 1C314 8001B714 8FBF003C */  lw         $ra, 0x3C($sp)
    /* 1C318 8001B718 8FB00018 */  lw         $s0, 0x18($sp)
    /* 1C31C 8001B71C 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 1C320 8001B720 8FB20020 */  lw         $s2, 0x20($sp)
    /* 1C324 8001B724 8FB30024 */  lw         $s3, 0x24($sp)
    /* 1C328 8001B728 8FB40028 */  lw         $s4, 0x28($sp)
    /* 1C32C 8001B72C 8FB5002C */  lw         $s5, 0x2C($sp)
    /* 1C330 8001B730 8FB60030 */  lw         $s6, 0x30($sp)
    /* 1C334 8001B734 8FB70034 */  lw         $s7, 0x34($sp)
    /* 1C338 8001B738 8FBE0038 */  lw         $fp, 0x38($sp)
    /* 1C33C 8001B73C 03E00008 */  jr         $ra
    /* 1C340 8001B740 27BD0040 */   addiu     $sp, $sp, 0x40
endlabel func_8001B5F4
