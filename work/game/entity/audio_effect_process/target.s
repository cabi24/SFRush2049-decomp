# Source: game_code.bin (decompressed)
# Address: 0x800960D4

glabel audio_effect_process
    /* 800960D4 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800960D8 AFA40020 */  sw $a0, 32($sp)
    /* 800960DC AFBF001C */  sw $ra, 28($sp)
    /* 800960E0 3C048015 */  lui $a0, 0x8015
    /* 800960E4 AFB10018 */  sw $s1, 24($sp)
    /* 800960E8 AFB00014 */  sw $s0, 20($sp)
    /* 800960EC 24842770 */  addiu $a0, $a0, 10096
    /* 800960F0 00002825 */  or $a1, $zero, $zero
    /* 800960F4 0C001C9C */  jal 0x80007270
    /* 800960F8 24060001 */  addiu $a2, $zero, 1
    /* 800960FC 8FA50020 */  lw $a1, 32($sp)
    /* 80096100 0C0257F6 */  jal 0x80095FD8
    /* 80096104 00003025 */  or $a2, $zero, $zero
    /* 80096108 3C048015 */  lui $a0, 0x8015
    /* 8009610C 24842770 */  addiu $a0, $a0, 10096
    /* 80096110 00002825 */  or $a1, $zero, $zero
    /* 80096114 0C001D78 */  jal 0x800075E0
    /* 80096118 00003025 */  or $a2, $zero, $zero
    /* 8009611C 8FBF001C */  lw $ra, 28($sp)
    /* 80096120 8FB00014 */  lw $s0, 20($sp)
    /* 80096124 8FB10018 */  lw $s1, 24($sp)
    /* 80096128 03E00008 */  jr $ra
    /* 8009612C 27BD0020 */  addiu $sp, $sp, 32
