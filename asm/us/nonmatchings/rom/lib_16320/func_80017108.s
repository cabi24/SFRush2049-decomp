nonmatching func_80017108, 0xAC

glabel func_80017108
    /* 17D08 80017108 3C018004 */  lui        $at, %hi(D_800385A0)
    /* 17D0C 8001710C AC2085A0 */  sw         $zero, %lo(D_800385A0)($at)
    /* 17D10 80017110 3C018004 */  lui        $at, %hi(D_80038608)
    /* 17D14 80017114 AC208608 */  sw         $zero, %lo(D_80038608)($at)
    /* 17D18 80017118 3C018004 */  lui        $at, %hi(D_8003C610)
    /* 17D1C 8001711C AC20C610 */  sw         $zero, %lo(D_8003C610)($at)
    /* 17D20 80017120 3C018004 */  lui        $at, %hi(D_8003CE18)
    /* 17D24 80017124 AC20CE18 */  sw         $zero, %lo(D_8003CE18)($at)
    /* 17D28 80017128 3C018004 */  lui        $at, %hi(D_80042228)
    /* 17D2C 8001712C AC202228 */  sw         $zero, %lo(D_80042228)($at)
    /* 17D30 80017130 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 17D34 80017134 3C018004 */  lui        $at, %hi(D_8003DA20)
    /* 17D38 80017138 3C028004 */  lui        $v0, %hi(D_8003DA28)
    /* 17D3C 8001713C 3C038004 */  lui        $v1, %hi(D_8003E228)
    /* 17D40 80017140 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 17D44 80017144 AC20DA20 */  sw         $zero, %lo(D_8003DA20)($at)
    /* 17D48 80017148 2463E228 */  addiu      $v1, $v1, %lo(D_8003E228)
    /* 17D4C 8001714C 2442DA28 */  addiu      $v0, $v0, %lo(D_8003DA28)
  .L80017150:
    /* 17D50 80017150 00000A02 */  srl        $at, $zero, 8
    /* 17D54 80017154 24420010 */  addiu      $v0, $v0, 0x10
    /* 17D58 80017158 A041FFF4 */  sb         $at, -0xC($v0)
    /* 17D5C 8001715C A041FFF6 */  sb         $at, -0xA($v0)
    /* 17D60 80017160 A041FFF8 */  sb         $at, -0x8($v0)
    /* 17D64 80017164 A041FFFA */  sb         $at, -0x6($v0)
    /* 17D68 80017168 A041FFFC */  sb         $at, -0x4($v0)
    /* 17D6C 8001716C A041FFFE */  sb         $at, -0x2($v0)
    /* 17D70 80017170 A040FFF5 */  sb         $zero, -0xB($v0)
    /* 17D74 80017174 A040FFF7 */  sb         $zero, -0x9($v0)
    /* 17D78 80017178 A040FFF9 */  sb         $zero, -0x7($v0)
    /* 17D7C 8001717C A040FFFB */  sb         $zero, -0x5($v0)
    /* 17D80 80017180 A040FFFD */  sb         $zero, -0x3($v0)
    /* 17D84 80017184 A040FFFF */  sb         $zero, -0x1($v0)
    /* 17D88 80017188 A041FFF0 */  sb         $at, -0x10($v0)
    /* 17D8C 8001718C A040FFF1 */  sb         $zero, -0xF($v0)
    /* 17D90 80017190 A041FFF2 */  sb         $at, -0xE($v0)
    /* 17D94 80017194 1443FFEE */  bne        $v0, $v1, .L80017150
    /* 17D98 80017198 A040FFF3 */   sb        $zero, -0xD($v0)
    /* 17D9C 8001719C 0C00533D */  jal        func_80014CF4
    /* 17DA0 800171A0 00000000 */   nop
    /* 17DA4 800171A4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 17DA8 800171A8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 17DAC 800171AC 03E00008 */  jr         $ra
    /* 17DB0 800171B0 00000000 */   nop
endlabel func_80017108
    /* 17DB4 800171B4 00000000 */  nop
    /* 17DB8 800171B8 00000000 */  nop
    /* 17DBC 800171BC 00000000 */  nop
