nonmatching func_80024C9C, 0x68

glabel func_80024C9C
    /* 2589C 80024C9C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 258A0 80024CA0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 258A4 80024CA4 C4800000 */  lwc1       $fv0, 0x0($a0)
    /* 258A8 80024CA8 C4820004 */  lwc1       $fv1, 0x4($a0)
    /* 258AC 80024CAC C48E0008 */  lwc1       $fa1, 0x8($a0)
    /* 258B0 80024CB0 46000102 */  mul.s      $ft0, $fv0, $fv0
    /* 258B4 80024CB4 AFA40018 */  sw         $a0, 0x18($sp)
    /* 258B8 80024CB8 46021182 */  mul.s      $ft1, $fv1, $fv1
    /* 258BC 80024CBC 46062200 */  add.s      $ft2, $ft0, $ft1
    /* 258C0 80024CC0 460E7282 */  mul.s      $ft3, $fa1, $fa1
    /* 258C4 80024CC4 0C0038F0 */  jal        sqrtf
    /* 258C8 80024CC8 460A4300 */   add.s     $fa0, $ft2, $ft3
    /* 258CC 80024CCC 8FA40018 */  lw         $a0, 0x18($sp)
    /* 258D0 80024CD0 C4900000 */  lwc1       $ft4, 0x0($a0)
    /* 258D4 80024CD4 C4840004 */  lwc1       $ft0, 0x4($a0)
    /* 258D8 80024CD8 C4880008 */  lwc1       $ft2, 0x8($a0)
    /* 258DC 80024CDC 46008483 */  div.s      $ft5, $ft4, $fv0
    /* 258E0 80024CE0 46002183 */  div.s      $ft1, $ft0, $fv0
    /* 258E4 80024CE4 E4920000 */  swc1       $ft5, 0x0($a0)
    /* 258E8 80024CE8 46004283 */  div.s      $ft3, $ft2, $fv0
    /* 258EC 80024CEC E4860004 */  swc1       $ft1, 0x4($a0)
    /* 258F0 80024CF0 E48A0008 */  swc1       $ft3, 0x8($a0)
    /* 258F4 80024CF4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 258F8 80024CF8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 258FC 80024CFC 03E00008 */  jr         $ra
    /* 25900 80024D00 00000000 */   nop
endlabel func_80024C9C
