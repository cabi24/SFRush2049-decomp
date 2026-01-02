# Source: 9660.s
# Address: 0x80008BB4

glabel func_80008BB4
    /* 97B4 80008BB4 27BDFF98 */  addiu      $sp, $sp, -0x68
    /* 97B8 80008BB8 44856000 */  mtc1       $a1, $fa0
    /* 97BC 80008BBC 44867000 */  mtc1       $a2, $fa1
    /* 97C0 80008BC0 C7A40078 */  lwc1       $ft0, 0x78($sp)
    /* 97C4 80008BC4 C7A6007C */  lwc1       $ft1, 0x7C($sp)
    /* 97C8 80008BC8 C7A80080 */  lwc1       $ft2, 0x80($sp)
    /* 97CC 80008BCC C7AA0084 */  lwc1       $ft3, 0x84($sp)
    /* 97D0 80008BD0 AFBF0024 */  sw         $ra, 0x24($sp)
    /* 97D4 80008BD4 AFA40068 */  sw         $a0, 0x68($sp)
    /* 97D8 80008BD8 44056000 */  mfc1       $a1, $fa0
    /* 97DC 80008BDC 44067000 */  mfc1       $a2, $fa1
    /* 97E0 80008BE0 AFA70074 */  sw         $a3, 0x74($sp)
    /* 97E4 80008BE4 27A40028 */  addiu      $a0, $sp, 0x28
    /* 97E8 80008BE8 E7A40010 */  swc1       $ft0, 0x10($sp)
    /* 97EC 80008BEC E7A60014 */  swc1       $ft1, 0x14($sp)
    /* 97F0 80008BF0 E7A80018 */  swc1       $ft2, 0x18($sp)
    /* 97F4 80008BF4 0C002298 */  jal        func_80008A60
    /* 97F8 80008BF8 E7AA001C */   swc1      $ft3, 0x1C($sp)
    /* 97FC 80008BFC 27A40028 */  addiu      $a0, $sp, 0x28
    /* 9800 80008C00 0C002478 */  jal        func_800091E0
    /* 9804 80008C04 8FA50068 */   lw        $a1, 0x68($sp)
    /* 9808 80008C08 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 980C 80008C0C 27BD0068 */  addiu      $sp, $sp, 0x68
    /* 9810 80008C10 03E00008 */  jr         $ra
    /* 9814 80008C14 00000000 */   nop