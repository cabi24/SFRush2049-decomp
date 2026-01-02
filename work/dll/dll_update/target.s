# Source: CC50.s
# Address: 0x8000C11C

glabel func_8000C11C
    /* CD1C 8000C11C 3C0E8003 */  lui        $t6, %hi(D_8002C3F0)
    /* CD20 8000C120 8DCEC3F0 */  lw         $t6, %lo(D_8002C3F0)($t6)
    /* CD24 8000C124 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* CD28 8000C128 AFBF0014 */  sw         $ra, 0x14($sp)
    /* CD2C 8000C12C 8DCF0000 */  lw         $t7, 0x0($t6)
    /* CD30 8000C130 11EE0054 */  beq        $t7, $t6, .L8000C284
    /* CD34 8000C134 00000000 */   nop
  .L8000C138:
    /* CD38 8000C138 3C188003 */  lui        $t8, %hi(D_8002C3F0)
    /* CD3C 8000C13C 8F18C3F0 */  lw         $t8, %lo(D_8002C3F0)($t8)
    /* CD40 8000C140 8F190000 */  lw         $t9, 0x0($t8)
    /* CD44 8000C144 17380006 */  bne        $t9, $t8, .L8000C160
    /* CD48 8000C148 AFB90024 */   sw        $t9, 0x24($sp)
    /* CD4C 8000C14C 0C003EE4 */  jal        func_8000FB90
    /* CD50 8000C150 00002025 */   or        $a0, $zero, $zero
    /* CD54 8000C154 3C018003 */  lui        $at, %hi(D_80037C60)
    /* CD58 8000C158 1000004A */  b          .L8000C284
    /* CD5C 8000C15C AC207C60 */   sw        $zero, %lo(D_80037C60)($at)
  .L8000C160:
    /* CD60 8000C160 0C00325C */  jal        func_8000C970
    /* CD64 8000C164 00000000 */   nop
    /* CD68 8000C168 AFA20020 */  sw         $v0, 0x20($sp)
    /* CD6C 8000C16C 3C098003 */  lui        $t1, %hi(D_80037C60)
    /* CD70 8000C170 8D297C60 */  lw         $t1, %lo(D_80037C60)($t1)
    /* CD74 8000C174 8FA80020 */  lw         $t0, 0x20($sp)
    /* CD78 8000C178 8FAF0024 */  lw         $t7, 0x24($sp)
    /* CD7C 8000C17C 3C018003 */  lui        $at, %hi(D_80037C60)
    /* CD80 8000C180 01095023 */  subu       $t2, $t0, $t1
    /* CD84 8000C184 AFAA001C */  sw         $t2, 0x1C($sp)
    /* CD88 8000C188 AC287C60 */  sw         $t0, %lo(D_80037C60)($at)
    /* CD8C 8000C18C 8DEE0010 */  lw         $t6, 0x10($t7)
    /* CD90 8000C190 8FAB001C */  lw         $t3, 0x1C($sp)
    /* CD94 8000C194 240C0000 */  addiu      $t4, $zero, 0x0
    /* CD98 8000C198 01E0C825 */  or         $t9, $t7, $zero
    /* CD9C 8000C19C 01CC082B */  sltu       $at, $t6, $t4
    /* CDA0 8000C1A0 8DEF0014 */  lw         $t7, 0x14($t7)
    /* CDA4 8000C1A4 14200016 */  bnez       $at, .L8000C200
    /* CDA8 8000C1A8 01606825 */   or        $t5, $t3, $zero
    /* CDAC 8000C1AC 018E082B */  sltu       $at, $t4, $t6
    /* CDB0 8000C1B0 14200003 */  bnez       $at, .L8000C1C0
    /* CDB4 8000C1B4 01AF082B */   sltu      $at, $t5, $t7
    /* CDB8 8000C1B8 10200011 */  beqz       $at, .L8000C200
    /* CDBC 8000C1BC 00000000 */   nop
  .L8000C1C0:
    /* CDC0 8000C1C0 27380010 */  addiu      $t8, $t9, 0x10
    /* CDC4 8000C1C4 AFB80018 */  sw         $t8, 0x18($sp)
    /* CDC8 8000C1C8 8F2B0014 */  lw         $t3, 0x14($t9)
    /* CDCC 8000C1CC 8F2A0010 */  lw         $t2, 0x10($t9)
    /* CDD0 8000C1D0 016D082B */  sltu       $at, $t3, $t5
    /* CDD4 8000C1D4 014C4023 */  subu       $t0, $t2, $t4
    /* CDD8 8000C1D8 01014023 */  subu       $t0, $t0, $at
    /* CDDC 8000C1DC 016D4823 */  subu       $t1, $t3, $t5
    /* CDE0 8000C1E0 AF290014 */  sw         $t1, 0x14($t9)
    /* CDE4 8000C1E4 AF280010 */  sw         $t0, 0x10($t9)
    /* CDE8 8000C1E8 8FAE0024 */  lw         $t6, 0x24($sp)
    /* CDEC 8000C1EC 8DC40010 */  lw         $a0, 0x10($t6)
    /* CDF0 8000C1F0 0C0030A5 */  jal        func_8000C294
    /* CDF4 8000C1F4 8DC50014 */   lw        $a1, 0x14($t6)
    /* CDF8 8000C1F8 10000022 */  b          .L8000C284
    /* CDFC 8000C1FC 00000000 */   nop
  .L8000C200:
    /* CE00 8000C200 8FAF0024 */  lw         $t7, 0x24($sp)
    /* CE04 8000C204 8DF80000 */  lw         $t8, 0x0($t7)
    /* CE08 8000C208 8DEA0004 */  lw         $t2, 0x4($t7)
    /* CE0C 8000C20C AD580000 */  sw         $t8, 0x0($t2)
    /* CE10 8000C210 8FAB0024 */  lw         $t3, 0x24($sp)
    /* CE14 8000C214 8D6C0004 */  lw         $t4, 0x4($t3)
    /* CE18 8000C218 8D6D0000 */  lw         $t5, 0x0($t3)
    /* CE1C 8000C21C ADAC0004 */  sw         $t4, 0x4($t5)
    /* CE20 8000C220 8FA80024 */  lw         $t0, 0x24($sp)
    /* CE24 8000C224 AD000000 */  sw         $zero, 0x0($t0)
    /* CE28 8000C228 8FA90024 */  lw         $t1, 0x24($sp)
    /* CE2C 8000C22C AD200004 */  sw         $zero, 0x4($t1)
    /* CE30 8000C230 8FB90024 */  lw         $t9, 0x24($sp)
    /* CE34 8000C234 8F2E0018 */  lw         $t6, 0x18($t9)
    /* CE38 8000C238 11C00005 */  beqz       $t6, .L8000C250
    /* CE3C 8000C23C 00000000 */   nop
    /* CE40 8000C240 01C02025 */  or         $a0, $t6, $zero
    /* CE44 8000C244 8F25001C */  lw         $a1, 0x1C($t9)
    /* CE48 8000C248 0C001D78 */  jal        func_800075E0
    /* CE4C 8000C24C 00003025 */   or        $a2, $zero, $zero
  .L8000C250:
    /* CE50 8000C250 8FAF0024 */  lw         $t7, 0x24($sp)
    /* CE54 8000C254 8DF80008 */  lw         $t8, 0x8($t7)
    /* CE58 8000C258 8DF9000C */  lw         $t9, 0xC($t7)
    /* CE5C 8000C25C 17000003 */  bnez       $t8, .L8000C26C
    /* CE60 8000C260 00000000 */   nop
    /* CE64 8000C264 1320FFB4 */  beqz       $t9, .L8000C138
    /* CE68 8000C268 00000000 */   nop
  .L8000C26C:
    /* CE6C 8000C26C ADF80010 */  sw         $t8, 0x10($t7)
    /* CE70 8000C270 ADF90014 */  sw         $t9, 0x14($t7)
    /* CE74 8000C274 0C0030C2 */  jal        func_8000C308
    /* CE78 8000C278 8FA40024 */   lw        $a0, 0x24($sp)
    /* CE7C 8000C27C 1000FFAE */  b          .L8000C138
    /* CE80 8000C280 00000000 */   nop
  .L8000C284:
    /* CE84 8000C284 8FBF0014 */  lw         $ra, 0x14($sp)
    /* CE88 8000C288 27BD0028 */  addiu      $sp, $sp, 0x28
    /* CE8C 8000C28C 03E00008 */  jr         $ra
    /* CE90 8000C290 00000000 */   nop