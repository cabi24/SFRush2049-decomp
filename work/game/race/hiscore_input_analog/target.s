# Source: game_code.bin (decompressed)
# Address: 0x800FBB00

glabel hiscore_input_analog
    /* 800FBB00 24060010 */  addiu $a2, $zero, 16
    /* 800FBB04 3C048015 */  lui $a0, 0x8015
    /* 800FBB08 2484256C */  addiu $a0, $a0, 9580
    /* 800FBB0C 00002825 */  or $a1, $zero, $zero
    /* 800FBB10 0C0009E4 */  jal 0x80002790
    /* 800FBB14 24060004 */  addiu $a2, $zero, 4
    /* 800FBB18 0C0E4234 */  jal 0x803908D0
    /* 800FBB1C 00000000 */  nop
    /* 800FBB20 3C028015 */  lui $v0, 0x8015
    /* 800FBB24 8C42A110 */  lw $v0, -24304($v0)
    /* 800FBB28 24010004 */  addiu $at, $zero, 4
    /* 800FBB2C 14410019 */  bne $v0, $at, 0x800FBB94
    /* 800FBB30 3C048015 */  lui $a0, 0x8015
    /* 800FBB34 24842038 */  addiu $a0, $a0, 8248
    /* 800FBB38 00002825 */  or $a1, $zero, $zero
    /* 800FBB3C 0C0009E4 */  jal 0x80002790
    /* 800FBB40 240601E0 */  addiu $a2, $zero, 480
    /* 800FBB44 3C048015 */  lui $a0, 0x8015
    /* 800FBB48 3C118015 */  lui $s1, 0x8015
    /* 800FBB4C 26316BA8 */  addiu $s1, $s1, 27560
    /* 800FBB50 248469B8 */  addiu $a0, $a0, 27064
    /* 800FBB54 3C101500 */  lui $s0, 0x1500
    /* 800FBB58 00002825 */  or $a1, $zero, $zero
    /* 800FBB5C 2406007C */  addiu $a2, $zero, 124
    /* 800FBB60 0C0009E4 */  jal 0x80002790
    /* 800FBB64 AFA4005C */  sw $a0, 92($sp)
    /* 800FBB68 8FA4005C */  lw $a0, 92($sp)
    /* 800FBB6C 8C980000 */  lw $t8, 0($a0)
    /* 800FBB70 2484007C */  addiu $a0, $a0, 124
    /* 800FBB74 0310C825 */  or $t9, $t8, $s0
    /* 800FBB78 1491FFF7 */  bne $a0, $s1, 0x800FBB58
    /* 800FBB7C AC99FF84 */  sw $t9, -124($a0)
    /* 800FBB80 44802000 */  .word 0x44802000
    /* 800FBB84 3C018016 */  lui $at, 0x8016
    /* 800FBB88 E424139C */  swc1 $f4, 5020($at)
    /* 800FBB8C 3C018015 */  lui $at, 0x8015
    /* 800FBB90 AC202738 */  sw $zero, 10040($at)
    /* 800FBB94 3C0E8011 */  lui $t6, 0x8011
    /* 800FBB98 8DCE74B4 */  lw $t6, 29876($t6)
    /* 800FBB9C 24180001 */  addiu $t8, $zero, 1
    /* 800FBBA0 3C018012 */  lui $at, 0x8012
    /* 800FBBA4 31CF0008 */  andi $t7, $t6, 0x0008
    /* 800FBBA8 15E00002 */  bne $t7, $zero, 0x800FBBB4
    /* 800FBBAC 3C198011 */  lui $t9, 0x8011
    /* 800FBBB0 A038EAE4 */  sb $t8, -5404($at)
    /* 800FBBB4 873964AA */  lh $t9, 25770($t9)
    /* 800FBBB8 3C018015 */  lui $at, 0x8015
    /* 800FBBBC 24020001 */  addiu $v0, $zero, 1
    /* 800FBBC0 A0391AD8 */  sb $t9, 6872($at)
    /* 800FBBC4 3C018011 */  lui $at, 0x8011
    /* 800FBBC8 A02046F4 */  sb $zero, 18164($at)
    /* 800FBBCC 8FBF004C */  lw $ra, 76($sp)
    /* 800FBBD0 8FB00028 */  lw $s0, 40($sp)
    /* 800FBBD4 8FB1002C */  lw $s1, 44($sp)
    /* 800FBBD8 8FB20030 */  lw $s2, 48($sp)
    /* 800FBBDC 8FB30034 */  lw $s3, 52($sp)
    /* 800FBBE0 8FB40038 */  lw $s4, 56($sp)
    /* 800FBBE4 8FB5003C */  lw $s5, 60($sp)
    /* 800FBBE8 8FB60040 */  lw $s6, 64($sp)
    /* 800FBBEC 8FB70044 */  lw $s7, 68($sp)
    /* 800FBBF0 8FBE0048 */  lw $fp, 72($sp)
    /* 800FBBF4 03E00008 */  jr $ra
    /* 800FBBF8 27BD0100 */  addiu $sp, $sp, 256
