nonmatching func_80018EB4, 0x6C

glabel func_80018EB4
    /* 19AB4 80018EB4 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 19AB8 80018EB8 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 19ABC 80018EBC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 19AC0 80018EC0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 19AC4 80018EC4 11C00011 */  beqz       $t6, .L80018F0C
    /* 19AC8 80018EC8 AFA5001C */   sw        $a1, 0x1C($sp)
    /* 19ACC 80018ECC 0C005D91 */  jal        func_80017644
    /* 19AD0 80018ED0 00000000 */   nop
    /* 19AD4 80018ED4 2401FFFF */  addiu      $at, $zero, -0x1
    /* 19AD8 80018ED8 1041000C */  beq        $v0, $at, .L80018F0C
    /* 19ADC 80018EDC 00027800 */   sll       $t7, $v0, 0
    /* 19AE0 80018EE0 05E0000A */  bltz       $t7, .L80018F0C
    /* 19AE4 80018EE4 93B8001F */   lbu       $t8, 0x1F($sp)
    /* 19AE8 80018EE8 00024240 */  sll        $t0, $v0, 9
    /* 19AEC 80018EEC 01024023 */  subu       $t0, $t0, $v0
    /* 19AF0 80018EF0 000840C0 */  sll        $t0, $t0, 3
    /* 19AF4 80018EF4 3C018004 */  lui        $at, %hi(D_80043EB8 + 0xFC5)
    /* 19AF8 80018EF8 00280821 */  addu       $at, $at, $t0
    /* 19AFC 80018EFC 2F190001 */  sltiu      $t9, $t8, 0x1
    /* 19B00 80018F00 A0394E7D */  sb         $t9, %lo(D_80043EB8 + 0xFC5)($at)
    /* 19B04 80018F04 10000002 */  b          .L80018F10
    /* 19B08 80018F08 24020001 */   addiu     $v0, $zero, 0x1
  .L80018F0C:
    /* 19B0C 80018F0C 00001025 */  or         $v0, $zero, $zero
  .L80018F10:
    /* 19B10 80018F10 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 19B14 80018F14 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 19B18 80018F18 03E00008 */  jr         $ra
    /* 19B1C 80018F1C 00000000 */   nop
endlabel func_80018EB4
