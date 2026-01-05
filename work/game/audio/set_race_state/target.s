# Source: game_code.bin (decompressed)
# Address: 0x800B6138

glabel set_race_state
    /* 800B6138 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800B613C AFBF0014 */  sw $ra, 20($sp)
    /* 800B6140 0C02D809 */  jal 0x800B6024
    /* 800B6144 00000000 */  nop
    /* 800B6148 3C048015 */  lui $a0, 0x8015
    /* 800B614C 2484A108 */  addiu $a0, $a0, -24312
    /* 800B6150 848E0000 */  lh $t6, 0($a0)
    /* 800B6154 3C028015 */  lui $v0, 0x8015
    /* 800B6158 2442A118 */  addiu $v0, $v0, -24296
    /* 800B615C 19C0000E */  blez $t6, 0x800B6198
    /* 800B6160 00001825 */  or $v1, $zero, $zero
    /* 800B6164 44800000 */  .word 0x44800000
    /* 800B6168 00000000 */  nop
    /* 800B616C AC400008 */  sw $zero, 8($v0)
    /* 800B6170 AC400004 */  sw $zero, 4($v0)
    /* 800B6174 AC40000C */  sw $zero, 12($v0)
    /* 800B6178 E4400010 */  swc1 $f0, 16($v0)
    /* 800B617C E4400014 */  swc1 $f0, 20($v0)
    /* 800B6180 848F0000 */  lh $t7, 0($a0)
    /* 800B6184 24630001 */  addiu $v1, $v1, 1
    /* 800B6188 2442004C */  addiu $v0, $v0, 76
    /* 800B618C 006F082A */  slt $at, $v1, $t7
    /* 800B6190 5420FFF7 */  .word 0x5420FFF7
    /* 800B6194 AC400008 */  sw $zero, 8($v0)
    /* 800B6198 8FBF0014 */  lw $ra, 20($sp)
    /* 800B619C 27BD0018 */  addiu $sp, $sp, 24
    /* 800B61A0 03E00008 */  jr $ra
    /* 800B61A4 00000000 */  nop
