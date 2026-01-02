# Source: 8DD0.s
# Address: 0x800081D0

glabel func_800081D0
    /* 8DD0 800081D0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 8DD4 800081D4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 8DD8 800081D8 AFA40020 */  sw         $a0, 0x20($sp)
    /* 8DDC 800081DC 0C003704 */  jal        func_8000DC10
    /* 8DE0 800081E0 AFA50024 */   sw        $a1, 0x24($sp)
    /* 8DE4 800081E4 8FA40020 */  lw         $a0, 0x20($sp)
    /* 8DE8 800081E8 0C003720 */  jal        func_8000DC80
    /* 8DEC 800081EC 8FA50024 */   lw        $a1, 0x24($sp)
    /* 8DF0 800081F0 0C003715 */  jal        func_8000DC54
    /* 8DF4 800081F4 AFA2001C */   sw        $v0, 0x1C($sp)
    /* 8DF8 800081F8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 8DFC 800081FC 8FA2001C */  lw         $v0, 0x1C($sp)
    /* 8E00 80008200 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 8E04 80008204 03E00008 */  jr         $ra
    /* 8E08 80008208 00000000 */   nop