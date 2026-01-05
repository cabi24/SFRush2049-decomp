# Source: game_code.bin (decompressed)
# Address: 0x800AD008

glabel abs_brake_control
    /* 800AD008 27BDFFF0 */  addiu $sp, $sp, -16
    /* 800AD00C 460C8182 */  .word 0x460C8182
    /* 800AD010 00000000 */  nop
    /* 800AD014 460C1202 */  .word 0x460C1202
    /* 800AD018 00000000 */  nop
    /* 800AD01C 460E8282 */  .word 0x460E8282
    /* 800AD020 46062001 */  .word 0x46062001
    /* 800AD024 C4C6001C */  lwc1 $f6, 28($a2)
    /* 800AD028 E4C0000C */  swc1 $f0, 12($a2)
    /* 800AD02C 460A4100 */  .word 0x460A4100
    /* 800AD030 460E9202 */  .word 0x460E9202
    /* 800AD034 E4C40018 */  swc1 $f4, 24($a2)
    /* 800AD038 E7A60004 */  swc1 $f6, 4($sp)
    /* 800AD03C C7AA0004 */  lwc1 $f10, 4($sp)
    /* 800AD040 C4C20014 */  lwc1 $f2, 20($a2)
    /* 800AD044 C4D00020 */  lwc1 $f16, 32($a2)
    /* 800AD048 460C5102 */  .word 0x460C5102
    /* 800AD04C 27BD0010 */  addiu $sp, $sp, 16
    /* 800AD050 460C9182 */  .word 0x460C9182
    /* 800AD054 46044001 */  .word 0x46044001
    /* 800AD058 460E5202 */  .word 0x460E5202
    /* 800AD05C E4C00010 */  swc1 $f0, 16($a2)
    /* 800AD060 460E1282 */  .word 0x460E1282
    /* 800AD064 46083100 */  .word 0x46083100
    /* 800AD068 460C8182 */  .word 0x460C8182
    /* 800AD06C 00000000 */  nop
    /* 800AD070 460C1202 */  .word 0x460C1202
    /* 800AD074 E4C4001C */  swc1 $f4, 28($a2)
    /* 800AD078 460E8102 */  .word 0x460E8102
    /* 800AD07C 46065001 */  .word 0x46065001
    /* 800AD080 E4C00014 */  swc1 $f0, 20($a2)
    /* 800AD084 46044280 */  .word 0x46044280
    /* 800AD088 03E00008 */  jr $ra
    /* 800AD08C E4CA0020 */  swc1 $f10, 32($a2)
