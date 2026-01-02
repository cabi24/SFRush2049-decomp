# Source: 9AB0.s
# Address: 0x80009168

glabel func_80009168
    /* 9D68 80009168 27BDFF90 */  addiu      $sp, $sp, -0x70
    /* 9D6C 8000916C 44856000 */  mtc1       $a1, $fa0
    /* 9D70 80009170 44867000 */  mtc1       $a2, $fa1
    /* 9D74 80009174 C7A40080 */  lwc1       $ft0, 0x80($sp)
    /* 9D78 80009178 C7A60084 */  lwc1       $ft1, 0x84($sp)
    /* 9D7C 8000917C C7A80088 */  lwc1       $ft2, 0x88($sp)
    /* 9D80 80009180 C7AA008C */  lwc1       $ft3, 0x8C($sp)
    /* 9D84 80009184 C7B00090 */  lwc1       $ft4, 0x90($sp)
    /* 9D88 80009188 C7B20094 */  lwc1       $ft5, 0x94($sp)
    /* 9D8C 8000918C AFBF002C */  sw         $ra, 0x2C($sp)
    /* 9D90 80009190 AFA40070 */  sw         $a0, 0x70($sp)
    /* 9D94 80009194 44056000 */  mfc1       $a1, $fa0
    /* 9D98 80009198 44067000 */  mfc1       $a2, $fa1
    /* 9D9C 8000919C AFA7007C */  sw         $a3, 0x7C($sp)
    /* 9DA0 800091A0 27A40030 */  addiu      $a0, $sp, 0x30
    /* 9DA4 800091A4 E7A40010 */  swc1       $ft0, 0x10($sp)
    /* 9DA8 800091A8 E7A60014 */  swc1       $ft1, 0x14($sp)
    /* 9DAC 800091AC E7A80018 */  swc1       $ft2, 0x18($sp)
    /* 9DB0 800091B0 E7AA001C */  swc1       $ft3, 0x1C($sp)
    /* 9DB4 800091B4 E7B00020 */  swc1       $ft4, 0x20($sp)
    /* 9DB8 800091B8 0C0023AC */  jal        func_80008EB0
    /* 9DBC 800091BC E7B20024 */   swc1      $ft5, 0x24($sp)
    /* 9DC0 800091C0 27A40030 */  addiu      $a0, $sp, 0x30
    /* 9DC4 800091C4 0C002478 */  jal        func_800091E0
    /* 9DC8 800091C8 8FA50070 */   lw        $a1, 0x70($sp)
    /* 9DCC 800091CC 8FBF002C */  lw         $ra, 0x2C($sp)
    /* 9DD0 800091D0 27BD0070 */  addiu      $sp, $sp, 0x70
    /* 9DD4 800091D4 03E00008 */  jr         $ra
    /* 9DD8 800091D8 00000000 */   nop