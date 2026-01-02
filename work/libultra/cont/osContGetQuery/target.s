# Source: A330.s
# Address: 0x800097AC

glabel func_800097AC
    /* A3AC 800097AC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* A3B0 800097B0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* A3B4 800097B4 00802825 */  or         $a1, $a0, $zero
    /* A3B8 800097B8 0C00256B */  jal        func_800095AC
    /* A3BC 800097BC 27A4001F */   addiu     $a0, $sp, 0x1F
    /* A3C0 800097C0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* A3C4 800097C4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* A3C8 800097C8 03E00008 */  jr         $ra
    /* A3CC 800097CC 00000000 */   nop