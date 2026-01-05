# Source: game_code.bin (decompressed)
# Address: 0x800FAEF4

glabel pause_menu_handler
    /* 800FAEF4 E4201510 */  swc1 $f0, 5392($at)
    /* 800FAEF8 3C018015 */  lui $at, 0x8015
    /* 800FAEFC E42015D8 */  swc1 $f0, 5592($at)
    /* 800FAF00 3C018015 */  lui $at, 0x8015
    /* 800FAF04 E4201514 */  swc1 $f0, 5396($at)
    /* 800FAF08 3C018015 */  lui $at, 0x8015
    /* 800FAF0C E42015DC */  swc1 $f0, 5596($at)
    /* 800FAF10 00021880 */  sll $v1, $v0, 2
    /* 800FAF14 3C0E8015 */  lui $t6, 0x8015
    /* 800FAF18 3C0F8015 */  lui $t7, 0x8015
    /* 800FAF1C 25CE1510 */  addiu $t6, $t6, 5392
    /* 800FAF20 25EF15D8 */  addiu $t7, $t7, 5592
    /* 800FAF24 006E2021 */  addu $a0, $v1, $t6
    /* 800FAF28 006F2821 */  addu $a1, $v1, $t7
    /* 800FAF2C E4A00004 */  swc1 $f0, 4($a1)
    /* 800FAF30 E4A00008 */  swc1 $f0, 8($a1)
    /* 800FAF34 E4A0000C */  swc1 $f0, 12($a1)
    /* 800FAF38 E4800004 */  swc1 $f0, 4($a0)
    /* 800FAF3C E4800008 */  swc1 $f0, 8($a0)
    /* 800FAF40 E480000C */  swc1 $f0, 12($a0)
    /* 800FAF44 E4800000 */  swc1 $f0, 0($a0)
    /* 800FAF48 03E00008 */  jr $ra
    /* 800FAF4C E4A00000 */  swc1 $f0, 0($a1)
