# Source: game_code.bin (decompressed)
# Address: 0x800BB800

glabel collision_car_wall
    /* 800BB800 2442C378 */  addiu $v0, $v0, -15496
    /* 800BB804 2463C308 */  addiu $v1, $v1, -15608
    /* 800BB808 AC20C300 */  sw $zero, -15616($at)
    /* 800BB80C AC20C304 */  sw $zero, -15612($at)
    /* 800BB810 24630010 */  addiu $v1, $v1, 16
    /* 800BB814 AC60FFF4 */  sw $zero, -12($v1)
    /* 800BB818 AC60FFF8 */  sw $zero, -8($v1)
    /* 800BB81C AC60FFFC */  sw $zero, -4($v1)
    /* 800BB820 1462FFFB */  bne $v1, $v0, 0x800BB810
    /* 800BB824 AC60FFF0 */  sw $zero, -16($v1)
    /* 800BB828 3C018014 */  lui $at, 0x8014
    /* 800BB82C 03E00008 */  jr $ra
    /* 800BB830 AC20F1DC */  sw $zero, -3620($at)
