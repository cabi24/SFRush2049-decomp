# Source: game_code.bin (decompressed)
# Address: 0x800A14C0

glabel adjust_maxpath_due_to_lap
    /* 800A14C0 1653FFEE */  bne $s2, $s3, 0x800A147C
    /* 800A14C4 26100304 */  addiu $s0, $s0, 772
    /* 800A14C8 3C048015 */  lui $a0, 0x8015
    /* 800A14CC 248497D0 */  addiu $a0, $a0, -26672
    /* 800A14D0 00002825 */  or $a1, $zero, $zero
    /* 800A14D4 0C001D78 */  jal 0x800075E0
    /* 800A14D8 00003025 */  or $a2, $zero, $zero
    /* 800A14DC 8FBF002C */  lw $ra, 44($sp)
    /* 800A14E0 8FB00018 */  lw $s0, 24($sp)
    /* 800A14E4 8FB1001C */  lw $s1, 28($sp)
    /* 800A14E8 8FB20020 */  lw $s2, 32($sp)
    /* 800A14EC 8FB30024 */  lw $s3, 36($sp)
    /* 800A14F0 8FB40028 */  lw $s4, 40($sp)
    /* 800A14F4 03E00008 */  jr $ra
    /* 800A14F8 27BD0058 */  addiu $sp, $sp, 88
