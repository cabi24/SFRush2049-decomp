# Source: game_code.bin (decompressed)
# Address: 0x800DEC8C

glabel main_menu_render_handler
    /* 800DEC8C 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800DEC90 3C028015 */  lui $v0, 0x8015
    /* 800DEC94 8442A108 */  lh $v0, -24312($v0)
    /* 800DEC98 AFB10020 */  sw $s1, 32($sp)
    /* 800DEC9C AFBF003C */  sw $ra, 60($sp)
    /* 800DECA0 AFB70038 */  sw $s7, 56($sp)
    /* 800DECA4 AFB60034 */  sw $s6, 52($sp)
    /* 800DECA8 AFB50030 */  sw $s5, 48($sp)
    /* 800DECAC AFB4002C */  sw $s4, 44($sp)
    /* 800DECB0 AFB30028 */  sw $s3, 40($sp)
    /* 800DECB4 AFB20024 */  sw $s2, 36($sp)
    /* 800DECB8 AFB0001C */  sw $s0, 28($sp)
    /* 800DECBC 18400023 */  blez $v0, 0x800DED4C
    /* 800DECC0 00008825 */  or $s1, $zero, $zero
    /* 800DECC4 3C108015 */  lui $s0, 0x8015
    /* 800DECC8 3C168015 */  lui $s6, 0x8015
    /* 800DECCC 3C158011 */  lui $s5, 0x8011
    /* 800DECD0 3C138015 */  lui $s3, 0x8015
    /* 800DECD4 2673A250 */  addiu $s3, $s3, -23984
    /* 800DECD8 26B541B0 */  addiu $s5, $s5, 16816
    /* 800DECDC 26D60B70 */  addiu $s6, $s6, 2928
    /* 800DECE0 2610A118 */  addiu $s0, $s0, -24296
    /* 800DECE4 24170098 */  addiu $s7, $zero, 152
    /* 800DECE8 24140808 */  addiu $s4, $zero, 2056
    /* 800DECEC 2412FFFF */  addiu $s2, $zero, -1
    /* 800DECF0 8E030044 */  lw $v1, 68($s0)
    /* 800DECF4 52430012 */  .word 0x52430012
    /* 800DECF8 26310001 */  addiu $s1, $s1, 1
    /* 800DECFC 920E0000 */  lbu $t6, 0($s0)
    /* 800DED00 00602025 */  or $a0, $v1, $zero
    /* 800DED04 02A03025 */  or $a2, $s5, $zero
    /* 800DED08 01D40019 */  multu $t6, $s4
    /* 800DED0C 00007812 */  mflo $t7
    /* 800DED10 026F2821 */  addu $a1, $s3, $t7
    /* 800DED14 24A5022C */  addiu $a1, $a1, 556
    /* 800DED18 02370019 */  multu $s1, $s7
    /* 800DED1C 0000C012 */  mflo $t8
    /* 800DED20 02D81021 */  addu $v0, $s6, $t8
    /* 800DED24 2459000C */  addiu $t9, $v0, 12
    /* 800DED28 AFB90010 */  sw $t9, 16($sp)
    /* 800DED2C 0C037AEB */  jal 0x800DEBAC
    /* 800DED30 24470018 */  addiu $a3, $v0, 24
    /* 800DED34 3C028015 */  lui $v0, 0x8015
    /* 800DED38 8442A108 */  lh $v0, -24312($v0)
    /* 800DED3C 26310001 */  addiu $s1, $s1, 1
    /* 800DED40 0222082A */  slt $at, $s1, $v0
    /* 800DED44 1420FFEA */  bne $at, $zero, 0x800DECF0
    /* 800DED48 2610004C */  addiu $s0, $s0, 76
    /* 800DED4C 8FBF003C */  lw $ra, 60($sp)
    /* 800DED50 8FB0001C */  lw $s0, 28($sp)
    /* 800DED54 8FB10020 */  lw $s1, 32($sp)
    /* 800DED58 8FB20024 */  lw $s2, 36($sp)
    /* 800DED5C 8FB30028 */  lw $s3, 40($sp)
    /* 800DED60 8FB4002C */  lw $s4, 44($sp)
    /* 800DED64 8FB50030 */  lw $s5, 48($sp)
    /* 800DED68 8FB60034 */  lw $s6, 52($sp)
    /* 800DED6C 8FB70038 */  lw $s7, 56($sp)
    /* 800DED70 03E00008 */  jr $ra
    /* 800DED74 27BD0040 */  addiu $sp, $sp, 64
