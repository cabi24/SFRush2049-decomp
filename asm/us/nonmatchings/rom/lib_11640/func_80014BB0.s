nonmatching func_80014BB0, 0x28

glabel func_80014BB0
    /* 157B0 80014BB0 00047880 */  sll        $t7, $a0, 2
    /* 157B4 80014BB4 01E47823 */  subu       $t7, $t7, $a0
    /* 157B8 80014BB8 3C0E8004 */  lui        $t6, %hi(D_80038294)
    /* 157BC 80014BBC 8DCE8294 */  lw         $t6, %lo(D_80038294)($t6)
    /* 157C0 80014BC0 000F7880 */  sll        $t7, $t7, 2
    /* 157C4 80014BC4 01E47821 */  addu       $t7, $t7, $a0
    /* 157C8 80014BC8 000F78C0 */  sll        $t7, $t7, 3
    /* 157CC 80014BCC 01CFC021 */  addu       $t8, $t6, $t7
    /* 157D0 80014BD0 03E00008 */  jr         $ra
    /* 157D4 80014BD4 A3000000 */   sb        $zero, 0x0($t8)
endlabel func_80014BB0
