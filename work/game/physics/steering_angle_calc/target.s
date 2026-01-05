# Source: game_code.bin (decompressed)
# Address: 0x800AD0A0

glabel steering_angle_calc
    /* 800AD0A0 27BDFFF0 */  addiu $sp, $sp, -16
    /* 800AD0A4 460C8182 */  .word 0x460C8182
    /* 800AD0A8 00000000 */  nop
    /* 800AD0AC 460C1202 */  .word 0x460C1202
    /* 800AD0B0 00000000 */  nop
    /* 800AD0B4 460E8282 */  .word 0x460E8282
    /* 800AD0B8 46062001 */  .word 0x46062001
    /* 800AD0BC C4C60010 */  lwc1 $f6, 16($a2)
    /* 800AD0C0 E4C00000 */  swc1 $f0, 0($a2)
    /* 800AD0C4 460A4100 */  .word 0x460A4100
    /* 800AD0C8 460E9202 */  .word 0x460E9202
    /* 800AD0CC E4C4000C */  swc1 $f4, 12($a2)
    /* 800AD0D0 E7A60004 */  swc1 $f6, 4($sp)
    /* 800AD0D4 C7AA0004 */  lwc1 $f10, 4($sp)
    /* 800AD0D8 C4C20008 */  lwc1 $f2, 8($a2)
    /* 800AD0DC C4D00014 */  lwc1 $f16, 20($a2)
    /* 800AD0E0 460C5102 */  .word 0x460C5102
    /* 800AD0E4 27BD0010 */  addiu $sp, $sp, 16
    /* 800AD0E8 460C9182 */  .word 0x460C9182
    /* 800AD0EC 46044001 */  .word 0x46044001
    /* 800AD0F0 460E5202 */  .word 0x460E5202
    /* 800AD0F4 E4C00004 */  swc1 $f0, 4($a2)
    /* 800AD0F8 460E1282 */  .word 0x460E1282
    /* 800AD0FC 46083100 */  .word 0x46083100
    /* 800AD100 460C8182 */  .word 0x460C8182
    /* 800AD104 00000000 */  nop
    /* 800AD108 460C1202 */  .word 0x460C1202
    /* 800AD10C E4C40010 */  swc1 $f4, 16($a2)
    /* 800AD110 460E8102 */  .word 0x460E8102
    /* 800AD114 46065001 */  .word 0x46065001
    /* 800AD118 E4C00008 */  swc1 $f0, 8($a2)
    /* 800AD11C 46044280 */  .word 0x46044280
    /* 800AD120 03E00008 */  jr $ra
    /* 800AD124 E4CA0014 */  swc1 $f10, 20($a2)
