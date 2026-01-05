# Source: game_code.bin (decompressed)
# Address: 0x800A4B6C

glabel wheel_params_set
    /* 800A4B6C 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800A4B70 AFBF001C */  sw $ra, 28($sp)
    /* 800A4B74 AFB10018 */  sw $s1, 24($sp)
    /* 800A4B78 0C02562E */  jal 0x800958B8
    /* 800A4B7C AFB00014 */  sw $s0, 20($sp)
    /* 800A4B80 3C028011 */  lui $v0, 0x8011
    /* 800A4B84 2442028C */  addiu $v0, $v0, 652
    /* 800A4B88 804E0000 */  lb $t6, 0($v0)
    /* 800A4B8C 3C048015 */  lui $a0, 0x8015
    /* 800A4B90 3C018011 */  lui $at, 0x8011
    /* 800A4B94 11C00004 */  beq $t6, $zero, 0x800A4BA8
    /* 800A4B98 24842770 */  addiu $a0, $a0, 10096
    /* 800A4B9C 804F0000 */  lb $t7, 0($v0)
    /* 800A4BA0 55E0FFFF */  .word 0x55E0FFFF
    /* 800A4BA4 804F0000 */  lb $t7, 0($v0)
    /* 800A4BA8 3C028014 */  lui $v0, 0x8014
    /* 800A4BAC 24426160 */  addiu $v0, $v0, 24928
    /* 800A4BB0 8C580008 */  lw $t8, 8($v0)
    /* 800A4BB4 3C0E8014 */  lui $t6, 0x8014
    /* 800A4BB8 00002825 */  or $a1, $zero, $zero
    /* 800A4BBC 13000004 */  beq $t8, $zero, 0x800A4BD0
    /* 800A4BC0 24060001 */  addiu $a2, $zero, 1
    /* 800A4BC4 8C590008 */  lw $t9, 8($v0)
    /* 800A4BC8 5720FFFF */  .word 0x5720FFFF
    /* 800A4BCC 8C590008 */  lw $t9, 8($v0)
    /* 800A4BD0 8DCE4C48 */  lw $t6, 19528($t6)
    /* 800A4BD4 A0200284 */  sb $zero, 644($at)
    /* 800A4BD8 0C001C9C */  jal 0x80007270
    /* 800A4BDC AFAE0038 */  sw $t6, 56($sp)
    /* 800A4BE0 8FA50038 */  lw $a1, 56($sp)
    /* 800A4BE4 0C0257F6 */  jal 0x80095FD8
    /* 800A4BE8 00003025 */  or $a2, $zero, $zero
    /* 800A4BEC 3C048015 */  lui $a0, 0x8015
    /* 800A4BF0 24842770 */  addiu $a0, $a0, 10096
    /* 800A4BF4 00002825 */  or $a1, $zero, $zero
    /* 800A4BF8 0C001D78 */  jal 0x800075E0
    /* 800A4BFC 00003025 */  or $a2, $zero, $zero
    /* 800A4C00 3C0F8014 */  lui $t7, 0x8014
    /* 800A4C04 8DEF6100 */  lw $t7, 24832($t7)
    /* 800A4C08 3C048015 */  lui $a0, 0x8015
    /* 800A4C0C 24842770 */  addiu $a0, $a0, 10096
    /* 800A4C10 00002825 */  or $a1, $zero, $zero
    /* 800A4C14 24060001 */  addiu $a2, $zero, 1
    /* 800A4C18 0C001C9C */  jal 0x80007270
    /* 800A4C1C AFAF0020 */  sw $t7, 32($sp)
    /* 800A4C20 8FA50020 */  lw $a1, 32($sp)
    /* 800A4C24 0C0257F6 */  jal 0x80095FD8
    /* 800A4C28 00003025 */  or $a2, $zero, $zero
    /* 800A4C2C 3C048015 */  lui $a0, 0x8015
    /* 800A4C30 24842770 */  addiu $a0, $a0, 10096
    /* 800A4C34 00002825 */  or $a1, $zero, $zero
    /* 800A4C38 0C001D78 */  jal 0x800075E0
    /* 800A4C3C 00003025 */  or $a2, $zero, $zero
    /* 800A4C40 8FBF001C */  lw $ra, 28($sp)
    /* 800A4C44 8FB00014 */  lw $s0, 20($sp)
    /* 800A4C48 8FB10018 */  lw $s1, 24($sp)
    /* 800A4C4C 03E00008 */  jr $ra
    /* 800A4C50 27BD0040 */  addiu $sp, $sp, 64
