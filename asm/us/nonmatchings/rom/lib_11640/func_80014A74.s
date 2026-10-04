nonmatching func_80014A74, 0x7C

glabel func_80014A74
    /* 15674 80014A74 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 15678 80014A78 00047880 */  sll        $t7, $a0, 2
    /* 1567C 80014A7C 01E47823 */  subu       $t7, $t7, $a0
    /* 15680 80014A80 3C188004 */  lui        $t8, %hi(D_80038294)
    /* 15684 80014A84 8F188294 */  lw         $t8, %lo(D_80038294)($t8)
    /* 15688 80014A88 000F7880 */  sll        $t7, $t7, 2
    /* 1568C 80014A8C 01E47821 */  addu       $t7, $t7, $a0
    /* 15690 80014A90 000F78C0 */  sll        $t7, $t7, 3
    /* 15694 80014A94 AFA70034 */  sw         $a3, 0x34($sp)
    /* 15698 80014A98 01F81021 */  addu       $v0, $t7, $t8
    /* 1569C 80014A9C 8FB90034 */  lw         $t9, 0x34($sp)
    /* 156A0 80014AA0 8FA90038 */  lw         $t1, 0x38($sp)
    /* 156A4 80014AA4 AFA60030 */  sw         $a2, 0x30($sp)
    /* 156A8 80014AA8 00A03025 */  or         $a2, $a1, $zero
    /* 156AC 80014AAC AFBF0024 */  sw         $ra, 0x24($sp)
    /* 156B0 80014AB0 AFA40028 */  sw         $a0, 0x28($sp)
    /* 156B4 80014AB4 AFA5002C */  sw         $a1, 0x2C($sp)
    /* 156B8 80014AB8 24480044 */  addiu      $t0, $v0, 0x44
    /* 156BC 80014ABC 244A0046 */  addiu      $t2, $v0, 0x46
    /* 156C0 80014AC0 8FA70030 */  lw         $a3, 0x30($sp)
    /* 156C4 80014AC4 AFAA001C */  sw         $t2, 0x1C($sp)
    /* 156C8 80014AC8 AFA80014 */  sw         $t0, 0x14($sp)
    /* 156CC 80014ACC 24450040 */  addiu      $a1, $v0, 0x40
    /* 156D0 80014AD0 24440042 */  addiu      $a0, $v0, 0x42
    /* 156D4 80014AD4 AFB90010 */  sw         $t9, 0x10($sp)
    /* 156D8 80014AD8 0C007838 */  jal        func_8001E0E0
    /* 156DC 80014ADC AFA90018 */   sw        $t1, 0x18($sp)
    /* 156E0 80014AE0 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 156E4 80014AE4 27BD0028 */  addiu      $sp, $sp, 0x28
    /* 156E8 80014AE8 03E00008 */  jr         $ra
    /* 156EC 80014AEC 00000000 */   nop
endlabel func_80014A74
