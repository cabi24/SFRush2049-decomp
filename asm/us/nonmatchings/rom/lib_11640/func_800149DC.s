nonmatching func_800149DC, 0x28

glabel func_800149DC
    /* 155DC 800149DC 00047880 */  sll        $t7, $a0, 2
    /* 155E0 800149E0 01E47823 */  subu       $t7, $t7, $a0
    /* 155E4 800149E4 3C0E8004 */  lui        $t6, %hi(D_80038294)
    /* 155E8 800149E8 8DCE8294 */  lw         $t6, %lo(D_80038294)($t6)
    /* 155EC 800149EC 000F7880 */  sll        $t7, $t7, 2
    /* 155F0 800149F0 01E47821 */  addu       $t7, $t7, $a0
    /* 155F4 800149F4 000F78C0 */  sll        $t7, $t7, 3
    /* 155F8 800149F8 01CFC021 */  addu       $t8, $t6, $t7
    /* 155FC 800149FC 03E00008 */  jr         $ra
    /* 15600 80014A00 A3000001 */   sb        $zero, 0x1($t8)
endlabel func_800149DC
