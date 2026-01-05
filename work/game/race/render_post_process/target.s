# Source: game_code.bin (decompressed)
# Address: 0x800FADE0

glabel render_post_process
    /* 800FADE0 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800FADE4 AFB4002C */  sw $s4, 44($sp)
    /* 800FADE8 3C148015 */  lui $s4, 0x8015
    /* 800FADEC 26941AD0 */  addiu $s4, $s4, 6864
    /* 800FADF0 868E0000 */  lh $t6, 0($s4)
    /* 800FADF4 AFB10020 */  sw $s1, 32($sp)
    /* 800FADF8 AFBF003C */  sw $ra, 60($sp)
    /* 800FADFC AFB70038 */  sw $s7, 56($sp)
    /* 800FAE00 AFB60034 */  sw $s6, 52($sp)
    /* 800FAE04 AFB50030 */  sw $s5, 48($sp)
    /* 800FAE08 AFB30028 */  sw $s3, 40($sp)
    /* 800FAE0C AFB20024 */  sw $s2, 36($sp)
    /* 800FAE10 AFB0001C */  sw $s0, 28($sp)
    /* 800FAE14 F7B40010 */  .word 0xF7B40010
    /* 800FAE18 19C0001B */  blez $t6, 0x800FAE88
    /* 800FAE1C 00008825 */  or $s1, $zero, $zero
    /* 800FAE20 3C128015 */  lui $s2, 0x8015
    /* 800FAE24 3C138011 */  lui $s3, 0x8011
    /* 800FAE28 3C158015 */  lui $s5, 0x8015
    /* 800FAE2C 4480A000 */  .word 0x4480A000
    /* 800FAE30 26B52818 */  addiu $s5, $s5, 10264
    /* 800FAE34 267306B4 */  addiu $s3, $s3, 1716
    /* 800FAE38 26520B70 */  addiu $s2, $s2, 2928
    /* 800FAE3C 24170007 */  addiu $s7, $zero, 7
    /* 800FAE40 241603B8 */  addiu $s6, $zero, 952
    /* 800FAE44 924F0094 */  lbu $t7, 148($s2)
    /* 800FAE48 01F60019 */  multu $t7, $s6
    /* 800FAE4C 0000C012 */  mflo $t8
    /* 800FAE50 02B88021 */  addu $s0, $s5, $t8
    /* 800FAE54 A211035C */  sb $s1, 860($s0)
    /* 800FAE58 0C03A48D */  jal 0x800E9234
    /* 800FAE5C 02002025 */  or $a0, $s0, $zero
    /* 800FAE60 A217035D */  sb $s7, 861($s0)
    /* 800FAE64 0C03EB54 */  jal 0x800FAD50
    /* 800FAE68 02002025 */  or $a0, $s0, $zero
    /* 800FAE6C 86990000 */  lh $t9, 0($s4)
    /* 800FAE70 26310001 */  addiu $s1, $s1, 1
    /* 800FAE74 26520098 */  addiu $s2, $s2, 152
    /* 800FAE78 0239082A */  slt $at, $s1, $t9
    /* 800FAE7C 26730004 */  addiu $s3, $s3, 4
    /* 800FAE80 1420FFF0 */  bne $at, $zero, 0x800FAE44
    /* 800FAE84 E674FFFC */  swc1 $f20, -4($s3)
    /* 800FAE88 3C018011 */  lui $at, 0x8011
    /* 800FAE8C 3C088011 */  lui $t0, 0x8011
    /* 800FAE90 24110002 */  addiu $s1, $zero, 2
    /* 800FAE94 25080680 */  addiu $t0, $t0, 1664
    /* 800FAE98 A0200680 */  sb $zero, 1664($at)
    /* 800FAE9C A0200681 */  sb $zero, 1665($at)
    /* 800FAEA0 02281021 */  addu $v0, $s1, $t0
    /* 800FAEA4 A0400001 */  sb $zero, 1($v0)
    /* 800FAEA8 A0400002 */  sb $zero, 2($v0)
    /* 800FAEAC A0400003 */  sb $zero, 3($v0)
    /* 800FAEB0 A0400000 */  sb $zero, 0($v0)
    /* 800FAEB4 8FBF003C */  lw $ra, 60($sp)
    /* 800FAEB8 8FB70038 */  lw $s7, 56($sp)
    /* 800FAEBC 8FB60034 */  lw $s6, 52($sp)
    /* 800FAEC0 8FB50030 */  lw $s5, 48($sp)
    /* 800FAEC4 8FB4002C */  lw $s4, 44($sp)
    /* 800FAEC8 8FB30028 */  lw $s3, 40($sp)
    /* 800FAECC 8FB20024 */  lw $s2, 36($sp)
    /* 800FAED0 8FB10020 */  lw $s1, 32($sp)
    /* 800FAED4 8FB0001C */  lw $s0, 28($sp)
    /* 800FAED8 D7B40010 */  .word 0xD7B40010
    /* 800FAEDC 03E00008 */  jr $ra
    /* 800FAEE0 27BD0040 */  addiu $sp, $sp, 64
