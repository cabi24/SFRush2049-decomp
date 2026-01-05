# Source: game_code.bin (decompressed)
# Address: 0x8010FCC0

glabel synced_model_render
    /* 8010FCC0 27BDFFE0 */  addiu $sp, $sp, -32
    /* 8010FCC4 AFA40020 */  sw $a0, 32($sp)
    /* 8010FCC8 AFBF001C */  sw $ra, 28($sp)
    /* 8010FCCC 3C048015 */  lui $a0, 0x8015
    /* 8010FCD0 AFB10018 */  sw $s1, 24($sp)
    /* 8010FCD4 AFB00014 */  sw $s0, 20($sp)
    /* 8010FCD8 24842770 */  addiu $a0, $a0, 10096
    /* 8010FCDC 00002825 */  or $a1, $zero, $zero
    /* 8010FCE0 0C001C9C */  jal 0x80007270
    /* 8010FCE4 24060001 */  addiu $a2, $zero, 1
    /* 8010FCE8 8FA50020 */  lw $a1, 32($sp)
    /* 8010FCEC 0C0257F6 */  jal 0x80095FD8
    /* 8010FCF0 00003025 */  or $a2, $zero, $zero
    /* 8010FCF4 3C048015 */  lui $a0, 0x8015
    /* 8010FCF8 24842770 */  addiu $a0, $a0, 10096
    /* 8010FCFC 00002825 */  or $a1, $zero, $zero
    /* 8010FD00 0C001D78 */  jal 0x800075E0
    /* 8010FD04 00003025 */  or $a2, $zero, $zero
    /* 8010FD08 8FBF001C */  lw $ra, 28($sp)
    /* 8010FD0C 8FB00014 */  lw $s0, 20($sp)
    /* 8010FD10 8FB10018 */  lw $s1, 24($sp)
    /* 8010FD14 03E00008 */  jr $ra
    /* 8010FD18 27BD0020 */  addiu $sp, $sp, 32
