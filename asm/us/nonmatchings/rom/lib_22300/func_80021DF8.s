nonmatching func_80021DF8, 0x7C

glabel func_80021DF8
    /* 229F8 80021DF8 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 229FC 80021DFC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 22A00 80021E00 AFA40018 */  sw         $a0, 0x18($sp)
    /* 22A04 80021E04 0C0079E4 */  jal        func_8001E790
    /* 22A08 80021E08 AFA5001C */   sw        $a1, 0x1C($sp)
    /* 22A0C 80021E0C 8FAE001C */  lw         $t6, 0x1C($sp)
    /* 22A10 80021E10 304F00FF */  andi       $t7, $v0, 0xFF
    /* 22A14 80021E14 8DC30000 */  lw         $v1, 0x0($t6)
    /* 22A18 80021E18 0003C202 */  srl        $t8, $v1, 8
    /* 22A1C 80021E1C 331900FF */  andi       $t9, $t8, 0xFF
    /* 22A20 80021E20 01F9082A */  slt        $at, $t7, $t9
    /* 22A24 80021E24 1420000E */  bnez       $at, .L80021E60
    /* 22A28 80021E28 00032402 */   srl       $a0, $v1, 16
    /* 22A2C 80021E2C 0C005B08 */  jal        func_80016C20
    /* 22A30 80021E30 3084FFFF */   andi      $a0, $a0, 0xFFFF
    /* 22A34 80021E34 1040000A */  beqz       $v0, .L80021E60
    /* 22A38 80021E38 8FA30018 */   lw        $v1, 0x18($sp)
    /* 22A3C 80021E3C A8620000 */  swl        $v0, 0x0($v1)
    /* 22A40 80021E40 B8620003 */  swr        $v0, 0x3($v1)
    /* 22A44 80021E44 8FA8001C */  lw         $t0, 0x1C($sp)
    /* 22A48 80021E48 8D090004 */  lw         $t1, 0x4($t0)
    /* 22A4C 80021E4C 312AFFFF */  andi       $t2, $t1, 0xFFFF
    /* 22A50 80021E50 000A58C0 */  sll        $t3, $t2, 3
    /* 22A54 80021E54 01626021 */  addu       $t4, $t3, $v0
    /* 22A58 80021E58 A86C0004 */  swl        $t4, 0x4($v1)
    /* 22A5C 80021E5C B86C0007 */  swr        $t4, 0x7($v1)
  .L80021E60:
    /* 22A60 80021E60 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 22A64 80021E64 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 22A68 80021E68 00001025 */  or         $v0, $zero, $zero
    /* 22A6C 80021E6C 03E00008 */  jr         $ra
    /* 22A70 80021E70 00000000 */   nop
endlabel func_80021DF8
