nonmatching func_800190AC, 0x98

glabel func_800190AC
    /* 19CAC 800190AC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 19CB0 800190B0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 19CB4 800190B4 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 19CB8 800190B8 0C005D91 */  jal        func_80017644
    /* 19CBC 800190BC AFA60020 */   sw        $a2, 0x20($sp)
    /* 19CC0 800190C0 2401FFFF */  addiu      $at, $zero, -0x1
    /* 19CC4 800190C4 8FA5001C */  lw         $a1, 0x1C($sp)
    /* 19CC8 800190C8 1041001A */  beq        $v0, $at, .L80019134
    /* 19CCC 800190CC 8FA60020 */   lw        $a2, 0x20($sp)
    /* 19CD0 800190D0 00027000 */  sll        $t6, $v0, 0
    /* 19CD4 800190D4 05C00009 */  bltz       $t6, .L800190FC
    /* 19CD8 800190D8 00027A40 */   sll       $t7, $v0, 9
    /* 19CDC 800190DC 01E27823 */  subu       $t7, $t7, $v0
    /* 19CE0 800190E0 3C188004 */  lui        $t8, %hi(D_80043EB8)
    /* 19CE4 800190E4 27183EB8 */  addiu      $t8, $t8, %lo(D_80043EB8)
    /* 19CE8 800190E8 000F78C0 */  sll        $t7, $t7, 3
    /* 19CEC 800190EC 01F81821 */  addu       $v1, $t7, $t8
    /* 19CF0 800190F0 AC650110 */  sw         $a1, 0x110($v1)
    /* 19CF4 800190F4 1000000F */  b          .L80019134
    /* 19CF8 800190F8 AC660114 */   sw        $a2, 0x114($v1)
  .L800190FC:
    /* 19CFC 800190FC 3C017FFF */  lui        $at, (0x7FFFFFFF >> 16)
    /* 19D00 80019100 3421FFFF */  ori        $at, $at, (0x7FFFFFFF & 0xFFFF)
    /* 19D04 80019104 00412024 */  and        $a0, $v0, $at
    /* 19D08 80019108 0004CA40 */  sll        $t9, $a0, 9
    /* 19D0C 8001910C 0324C823 */  subu       $t9, $t9, $a0
    /* 19D10 80019110 3C088004 */  lui        $t0, %hi(D_80043EB8)
    /* 19D14 80019114 25083EB8 */  addiu      $t0, $t0, %lo(D_80043EB8)
    /* 19D18 80019118 0019C8C0 */  sll        $t9, $t9, 3
    /* 19D1C 8001911C 03281821 */  addu       $v1, $t9, $t0
    /* 19D20 80019120 90690FEE */  lbu        $t1, 0xFEE($v1)
    /* 19D24 80019124 AC650FE4 */  sw         $a1, 0xFE4($v1)
    /* 19D28 80019128 AC660FE8 */  sw         $a2, 0xFE8($v1)
    /* 19D2C 8001912C 352A0010 */  ori        $t2, $t1, 0x10
    /* 19D30 80019130 A06A0FEE */  sb         $t2, 0xFEE($v1)
  .L80019134:
    /* 19D34 80019134 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 19D38 80019138 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 19D3C 8001913C 03E00008 */  jr         $ra
    /* 19D40 80019140 00000000 */   nop
endlabel func_800190AC
