nonmatching func_8001FA18, 0xCC

glabel func_8001FA18
    /* 20618 8001FA18 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 2061C 8001FA1C 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 20620 8001FA20 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 20624 8001FA24 AFB50028 */  sw         $s5, 0x28($sp)
    /* 20628 8001FA28 AFBF002C */  sw         $ra, 0x2C($sp)
    /* 2062C 8001FA2C AFB40024 */  sw         $s4, 0x24($sp)
    /* 20630 8001FA30 AFB30020 */  sw         $s3, 0x20($sp)
    /* 20634 8001FA34 AFB2001C */  sw         $s2, 0x1C($sp)
    /* 20638 8001FA38 AFB10018 */  sw         $s1, 0x18($sp)
    /* 2063C 8001FA3C AFB00014 */  sw         $s0, 0x14($sp)
    /* 20640 8001FA40 11C0001E */  beqz       $t6, .L8001FABC
    /* 20644 8001FA44 2415FFFF */   addiu     $s5, $zero, -0x1
    /* 20648 8001FA48 0C007B7D */  jal        func_8001EDF4
    /* 2064C 8001FA4C 00000000 */   nop
    /* 20650 8001FA50 2412FFFF */  addiu      $s2, $zero, -0x1
    /* 20654 8001FA54 10520019 */  beq        $v0, $s2, .L8001FABC
    /* 20658 8001FA58 00402825 */   or        $a1, $v0, $zero
    /* 2065C 8001FA5C 3C138005 */  lui        $s3, %hi(D_8004BEB8)
    /* 20660 8001FA60 2673BEB8 */  addiu      $s3, $s3, %lo(D_8004BEB8)
    /* 20664 8001FA64 241401A0 */  addiu      $s4, $zero, 0x1A0
  .L8001FA68:
    /* 20668 8001FA68 30B000FF */  andi       $s0, $a1, 0xFF
    /* 2066C 8001FA6C 02140019 */  multu      $s0, $s4
    /* 20670 8001FA70 00007812 */  mflo       $t7
    /* 20674 8001FA74 026F2021 */  addu       $a0, $s3, $t7
    /* 20678 8001FA78 88980060 */  lwl        $t8, 0x60($a0)
    /* 2067C 8001FA7C 98980063 */  lwr        $t8, 0x63($a0)
    /* 20680 8001FA80 88910010 */  lwl        $s1, 0x10($a0)
    /* 20684 8001FA84 98910013 */  lwr        $s1, 0x13($a0)
    /* 20688 8001FA88 14B8000A */  bne        $a1, $t8, .L8001FAB4
    /* 2068C 8001FA8C 00000000 */   nop
    /* 20690 8001FA90 88990000 */  lwl        $t9, 0x0($a0)
    /* 20694 8001FA94 98990003 */  lwr        $t9, 0x3($a0)
    /* 20698 8001FA98 0000A825 */  or         $s5, $zero, $zero
    /* 2069C 8001FA9C 13200003 */  beqz       $t9, .L8001FAAC
    /* 206A0 8001FAA0 00000000 */   nop
    /* 206A4 8001FAA4 0C007E74 */  jal        func_8001F9D0
    /* 206A8 8001FAA8 00000000 */   nop
  .L8001FAAC:
    /* 206AC 8001FAAC 0C0052CF */  jal        func_80014B3C
    /* 206B0 8001FAB0 02002025 */   or        $a0, $s0, $zero
  .L8001FAB4:
    /* 206B4 8001FAB4 1632FFEC */  bne        $s1, $s2, .L8001FA68
    /* 206B8 8001FAB8 02202825 */   or        $a1, $s1, $zero
  .L8001FABC:
    /* 206BC 8001FABC 8FBF002C */  lw         $ra, 0x2C($sp)
    /* 206C0 8001FAC0 02A01025 */  or         $v0, $s5, $zero
    /* 206C4 8001FAC4 8FB50028 */  lw         $s5, 0x28($sp)
    /* 206C8 8001FAC8 8FB00014 */  lw         $s0, 0x14($sp)
    /* 206CC 8001FACC 8FB10018 */  lw         $s1, 0x18($sp)
    /* 206D0 8001FAD0 8FB2001C */  lw         $s2, 0x1C($sp)
    /* 206D4 8001FAD4 8FB30020 */  lw         $s3, 0x20($sp)
    /* 206D8 8001FAD8 8FB40024 */  lw         $s4, 0x24($sp)
    /* 206DC 8001FADC 03E00008 */  jr         $ra
    /* 206E0 8001FAE0 27BD0030 */   addiu     $sp, $sp, 0x30
endlabel func_8001FA18
