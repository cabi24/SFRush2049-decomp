nonmatching func_80010DD8, 0xA8

glabel func_80010DD8
    /* 119D8 80010DD8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 119DC 80010DDC AFA5001C */  sw         $a1, 0x1C($sp)
    /* 119E0 80010DE0 30A5FFFF */  andi       $a1, $a1, 0xFFFF
    /* 119E4 80010DE4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 119E8 80010DE8 AFA40018 */  sw         $a0, 0x18($sp)
    /* 119EC 80010DEC 10A0001E */  beqz       $a1, .L80010E68
    /* 119F0 80010DF0 00A03025 */   or        $a2, $a1, $zero
    /* 119F4 80010DF4 00A40019 */  multu      $a1, $a0
    /* 119F8 80010DF8 240103E8 */  addiu      $at, $zero, 0x3E8
    /* 119FC 80010DFC 240700C0 */  addiu      $a3, $zero, 0xC0
    /* 11A00 80010E00 3C198004 */  lui        $t9, %hi(D_80038018)
    /* 11A04 80010E04 8F398018 */  lw         $t9, %lo(D_80038018)($t9)
    /* 11A08 80010E08 00807025 */  or         $t6, $a0, $zero
    /* 11A0C 80010E0C 00002825 */  or         $a1, $zero, $zero
    /* 11A10 80010E10 00001012 */  mflo       $v0
    /* 11A14 80010E14 00000000 */  nop
    /* 11A18 80010E18 00000000 */  nop
    /* 11A1C 80010E1C 0041001B */  divu       $zero, $v0, $at
    /* 11A20 80010E20 00001012 */  mflo       $v0
    /* 11A24 80010E24 3C018004 */  lui        $at, %hi(D_800382F4)
    /* 11A28 80010E28 00000000 */  nop
    /* 11A2C 80010E2C 0047001B */  divu       $zero, $v0, $a3
    /* 11A30 80010E30 00007810 */  mfhi       $t7
    /* 11A34 80010E34 00EFC023 */  subu       $t8, $a3, $t7
    /* 11A38 80010E38 03021821 */  addu       $v1, $t8, $v0
    /* 11A3C 80010E3C AC2382F4 */  sw         $v1, %lo(D_800382F4)($at)
    /* 11A40 80010E40 14E00002 */  bnez       $a3, .L80010E4C
    /* 11A44 80010E44 00000000 */   nop
    /* 11A48 80010E48 0007000D */  break      7
  .L80010E4C:
    /* 11A4C 80010E4C 3C018004 */  lui        $at, %hi(D_800382E8)
    /* 11A50 80010E50 AC2082E8 */  sw         $zero, %lo(D_800382E8)($at)
    /* 11A54 80010E54 0320F809 */  jalr       $t9
    /* 11A58 80010E58 00032040 */   sll       $a0, $v1, 1
    /* 11A5C 80010E5C 3C018004 */  lui        $at, %hi(D_800382F0)
    /* 11A60 80010E60 10000003 */  b          .L80010E70
    /* 11A64 80010E64 AC2282F0 */   sw        $v0, %lo(D_800382F0)($at)
  .L80010E68:
    /* 11A68 80010E68 3C018004 */  lui        $at, %hi(D_800382F0)
    /* 11A6C 80010E6C AC2082F0 */  sw         $zero, %lo(D_800382F0)($at)
  .L80010E70:
    /* 11A70 80010E70 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 11A74 80010E74 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 11A78 80010E78 03E00008 */  jr         $ra
    /* 11A7C 80010E7C 00000000 */   nop
endlabel func_80010DD8
