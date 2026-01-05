# Source: game_code.bin (decompressed)
# Address: 0x800EE820

glabel effect_spawn
    /* 800EE820 27BDFFD8 */  addiu $sp, $sp, -40
    /* 800EE824 AFBF0024 */  sw $ra, 36($sp)
    /* 800EE828 AFB20020 */  sw $s2, 32($sp)
    /* 800EE82C AFB1001C */  sw $s1, 28($sp)
    /* 800EE830 0C039C0E */  jal 0x800E7038
    /* 800EE834 AFB00018 */  sw $s0, 24($sp)
    /* 800EE838 3C118014 */  lui $s1, 0x8014
    /* 800EE83C 2631C228 */  addiu $s1, $s1, -15832
    /* 800EE840 0C031DF1 */  jal 0x800C77C4
    /* 800EE844 02202025 */  or $a0, $s1, $zero
    /* 800EE848 3C108014 */  lui $s0, 0x8014
    /* 800EE84C 3C128014 */  lui $s2, 0x8014
    /* 800EE850 2652C090 */  addiu $s2, $s2, -16240
    /* 800EE854 2610C068 */  addiu $s0, $s0, -16280
    /* 800EE858 02002025 */  or $a0, $s0, $zero
    /* 800EE85C 02202825 */  or $a1, $s1, $zero
    /* 800EE860 0C001F1A */  jal 0x80007C68
    /* 800EE864 2406000A */  addiu $a2, $zero, 10
    /* 800EE868 2610000A */  addiu $s0, $s0, 10
    /* 800EE86C 5612FFFB */  .word 0x5612FFFB
    /* 800EE870 02002025 */  or $a0, $s0, $zero
    /* 800EE874 8FBF0024 */  lw $ra, 36($sp)
    /* 800EE878 8FB00018 */  lw $s0, 24($sp)
    /* 800EE87C 8FB1001C */  lw $s1, 28($sp)
    /* 800EE880 8FB20020 */  lw $s2, 32($sp)
    /* 800EE884 03E00008 */  jr $ra
    /* 800EE888 27BD0028 */  addiu $sp, $sp, 40
