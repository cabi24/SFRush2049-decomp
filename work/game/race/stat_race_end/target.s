# Source: game_code.bin (decompressed)
# Address: 0x800FE4BC

glabel stat_race_end
    /* 800FE4BC 27BDFFD8 */  addiu $sp, $sp, -40
    /* 800FE4C0 01EE7821 */  addu $t7, $t7, $t6
    /* 800FE4C4 8DEFFEF4 */  lw $t7, -268($t7)
    /* 800FE4C8 AFBF001C */  sw $ra, 28($sp)
    /* 800FE4CC 3C048015 */  lui $a0, 0x8015
    /* 800FE4D0 AFB10018 */  sw $s1, 24($sp)
    /* 800FE4D4 AFB00014 */  sw $s0, 20($sp)
    /* 800FE4D8 24842770 */  addiu $a0, $a0, 10096
    /* 800FE4DC 00002825 */  or $a1, $zero, $zero
    /* 800FE4E0 24060001 */  addiu $a2, $zero, 1
    /* 800FE4E4 0C001C9C */  jal 0x80007270
    /* 800FE4E8 AFAF0020 */  sw $t7, 32($sp)
    /* 800FE4EC 8FA50020 */  lw $a1, 32($sp)
    /* 800FE4F0 0C0257F6 */  jal 0x80095FD8
    /* 800FE4F4 00003025 */  or $a2, $zero, $zero
    /* 800FE4F8 3C048015 */  lui $a0, 0x8015
    /* 800FE4FC 24842770 */  addiu $a0, $a0, 10096
    /* 800FE500 00002825 */  or $a1, $zero, $zero
    /* 800FE504 0C001D78 */  jal 0x800075E0
    /* 800FE508 00003025 */  or $a2, $zero, $zero
    /* 800FE50C 8FBF001C */  lw $ra, 28($sp)
    /* 800FE510 8FB00014 */  lw $s0, 20($sp)
    /* 800FE514 8FB10018 */  lw $s1, 24($sp)
    /* 800FE518 03E00008 */  jr $ra
    /* 800FE51C 27BD0028 */  addiu $sp, $sp, 40
