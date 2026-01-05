# Source: game_code.bin (decompressed)
# Address: 0x8009731C

glabel entity_bounds_update
    /* 8009731C 27BDFFD8 */  addiu $sp, $sp, -40
    /* 80097320 AFA40028 */  sw $a0, 40($sp)
    /* 80097324 AFBF0014 */  sw $ra, 20($sp)
    /* 80097328 3C048015 */  lui $a0, 0x8015
    /* 8009732C 24842770 */  addiu $a0, $a0, 10096
    /* 80097330 00002825 */  or $a1, $zero, $zero
    /* 80097334 0C001C9C */  jal 0x80007270
    /* 80097338 24060001 */  addiu $a2, $zero, 1
    /* 8009733C 8FA50028 */  lw $a1, 40($sp)
    /* 80097340 0C0257E3 */  jal 0x80095F8C
    /* 80097344 00A02025 */  or $a0, $a1, $zero
    /* 80097348 00402025 */  or $a0, $v0, $zero
    /* 8009734C 0C0257BD */  jal 0x80095EF4
    /* 80097350 00003025 */  or $a2, $zero, $zero
    /* 80097354 8C4E000C */  lw $t6, 12($v0)
    /* 80097358 3C048015 */  lui $a0, 0x8015
    /* 8009735C 24842770 */  addiu $a0, $a0, 10096
    /* 80097360 00002825 */  or $a1, $zero, $zero
    /* 80097364 00003025 */  or $a2, $zero, $zero
    /* 80097368 0C001D78 */  jal 0x800075E0
    /* 8009736C AFAE001C */  sw $t6, 28($sp)
    /* 80097370 8FBF0014 */  lw $ra, 20($sp)
    /* 80097374 8FA2001C */  lw $v0, 28($sp)
    /* 80097378 27BD0028 */  addiu $sp, $sp, 40
    /* 8009737C 03E00008 */  jr $ra
    /* 80097380 00000000 */  nop
