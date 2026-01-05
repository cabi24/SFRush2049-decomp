# Source: game_code.bin (decompressed)
# Address: 0x8009DC50

glabel track_segment_setup
    /* 8009DC50 27BDFFD0 */  addiu $sp, $sp, -48
    /* 8009DC54 AFB1001C */  sw $s1, 28($sp)
    /* 8009DC58 AFB40028 */  sw $s4, 40($sp)
    /* 8009DC5C AFB30024 */  sw $s3, 36($sp)
    /* 8009DC60 AFB20020 */  sw $s2, 32($sp)
    /* 8009DC64 AFB00018 */  sw $s0, 24($sp)
    /* 8009DC68 3C118013 */  lui $s1, 0x8013
    /* 8009DC6C 240E0100 */  addiu $t6, $zero, 256
    /* 8009DC70 00808025 */  or $s0, $a0, $zero
    /* 8009DC74 00A09825 */  or $s3, $a1, $zero
    /* 8009DC78 AFBF002C */  sw $ra, 44($sp)
    /* 8009DC7C 00AEA004 */  .word 0x00AEA004
    /* 8009DC80 2631E700 */  addiu $s1, $s1, -6400
    /* 8009DC84 24120044 */  addiu $s2, $zero, 68
    /* 8009DC88 8E0F0000 */  lw $t7, 0($s0)
    /* 8009DC8C 01F4C024 */  and $t8, $t7, $s4
    /* 8009DC90 57000004 */  .word 0x57000004
    /* 8009DC94 86020016 */  lh $v0, 22($s0)
    /* 8009DC98 10000017 */  beq $zero, $zero, 0x8009DCF8
    /* 8009DC9C 24020001 */  addiu $v0, $zero, 1
    /* 8009DCA0 86020016 */  lh $v0, 22($s0)
    /* 8009DCA4 0442000C */  .word 0x0442000C
    /* 8009DCA8 86020018 */  lh $v0, 24($s0)
    /* 8009DCAC 00520019 */  multu $v0, $s2
    /* 8009DCB0 02602825 */  or $a1, $s3, $zero
    /* 8009DCB4 0000C812 */  mflo $t9
    /* 8009DCB8 02392021 */  addu $a0, $s1, $t9
    /* 8009DCBC 0C027714 */  jal 0x8009DC50
    /* 8009DCC0 00000000 */  nop
    /* 8009DCC4 50400004 */  .word 0x50400004
    /* 8009DCC8 86020018 */  lh $v0, 24($s0)
    /* 8009DCCC 1000000A */  beq $zero, $zero, 0x8009DCF8
    /* 8009DCD0 24020001 */  addiu $v0, $zero, 1
    /* 8009DCD4 86020018 */  lh $v0, 24($s0)
    /* 8009DCD8 04420007 */  .word 0x04420007
    /* 8009DCDC 00001025 */  or $v0, $zero, $zero
    /* 8009DCE0 00520019 */  multu $v0, $s2
    /* 8009DCE4 00004012 */  mflo $t0
    /* 8009DCE8 02288021 */  addu $s0, $s1, $t0
    /* 8009DCEC 1000FFE7 */  beq $zero, $zero, 0x8009DC8C
    /* 8009DCF0 8E0F0000 */  lw $t7, 0($s0)
    /* 8009DCF4 00001025 */  or $v0, $zero, $zero
    /* 8009DCF8 8FBF002C */  lw $ra, 44($sp)
    /* 8009DCFC 8FB00018 */  lw $s0, 24($sp)
    /* 8009DD00 8FB1001C */  lw $s1, 28($sp)
    /* 8009DD04 8FB20020 */  lw $s2, 32($sp)
    /* 8009DD08 8FB30024 */  lw $s3, 36($sp)
    /* 8009DD0C 8FB40028 */  lw $s4, 40($sp)
    /* 8009DD10 03E00008 */  jr $ra
    /* 8009DD14 27BD0030 */  addiu $sp, $sp, 48
