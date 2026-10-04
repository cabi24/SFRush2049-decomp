nonmatching func_8001D5C0, 0xA0

glabel func_8001D5C0
    /* 1E1C0 8001D5C0 00803825 */  or         $a3, $a0, $zero
    /* 1E1C4 8001D5C4 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* 1E1C8 8001D5C8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1E1CC 8001D5CC AFA70048 */  sw         $a3, 0x48($sp)
    /* 1E1D0 8001D5D0 24E5003C */  addiu      $a1, $a3, 0x3C
    /* 1E1D4 8001D5D4 24E60024 */  addiu      $a2, $a3, 0x24
    /* 1E1D8 8001D5D8 0C009341 */  jal        func_80024D04
    /* 1E1DC 8001D5DC 24840030 */   addiu     $a0, $a0, 0x30
    /* 1E1E0 8001D5E0 8FA70048 */  lw         $a3, 0x48($sp)
    /* 1E1E4 8001D5E4 27A50018 */  addiu      $a1, $sp, 0x18
    /* 1E1E8 8001D5E8 C4E40030 */  lwc1       $ft0, 0x30($a3)
    /* 1E1EC 8001D5EC 24E40048 */  addiu      $a0, $a3, 0x48
    /* 1E1F0 8001D5F0 E7A40018 */  swc1       $ft0, 0x18($sp)
    /* 1E1F4 8001D5F4 C4E60034 */  lwc1       $ft1, 0x34($a3)
    /* 1E1F8 8001D5F8 E7A60024 */  swc1       $ft1, 0x24($sp)
    /* 1E1FC 8001D5FC C4E80038 */  lwc1       $ft2, 0x38($a3)
    /* 1E200 8001D600 E7A80030 */  swc1       $ft2, 0x30($sp)
    /* 1E204 8001D604 C4EA003C */  lwc1       $ft3, 0x3C($a3)
    /* 1E208 8001D608 E7AA001C */  swc1       $ft3, 0x1C($sp)
    /* 1E20C 8001D60C C4F00040 */  lwc1       $ft4, 0x40($a3)
    /* 1E210 8001D610 E7B00028 */  swc1       $ft4, 0x28($sp)
    /* 1E214 8001D614 C4F20044 */  lwc1       $ft5, 0x44($a3)
    /* 1E218 8001D618 E7B20034 */  swc1       $ft5, 0x34($sp)
    /* 1E21C 8001D61C C4E40024 */  lwc1       $ft0, 0x24($a3)
    /* 1E220 8001D620 E7A40020 */  swc1       $ft0, 0x20($sp)
    /* 1E224 8001D624 C4E60028 */  lwc1       $ft1, 0x28($a3)
    /* 1E228 8001D628 E7A6002C */  swc1       $ft1, 0x2C($sp)
    /* 1E22C 8001D62C C4E8002C */  lwc1       $ft2, 0x2C($a3)
    /* 1E230 8001D630 E7A80038 */  swc1       $ft2, 0x38($sp)
    /* 1E234 8001D634 C4EA000C */  lwc1       $ft3, 0xC($a3)
    /* 1E238 8001D638 E7AA003C */  swc1       $ft3, 0x3C($sp)
    /* 1E23C 8001D63C C4F00010 */  lwc1       $ft4, 0x10($a3)
    /* 1E240 8001D640 E7B00040 */  swc1       $ft4, 0x40($sp)
    /* 1E244 8001D644 C4F20014 */  lwc1       $ft5, 0x14($a3)
    /* 1E248 8001D648 0C00935D */  jal        func_80024D74
    /* 1E24C 8001D64C E7B20044 */   swc1      $ft5, 0x44($sp)
    /* 1E250 8001D650 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1E254 8001D654 27BD0048 */  addiu      $sp, $sp, 0x48
    /* 1E258 8001D658 03E00008 */  jr         $ra
    /* 1E25C 8001D65C 00000000 */   nop
endlabel func_8001D5C0
