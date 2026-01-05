# Source: game_code.bin (decompressed)
# Address: 0x800D2C2C

glabel race_timer_display
    /* 800D2C2C 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800D2C30 3C018012 */  lui $at, 0x8012
    /* 800D2C34 AFA50024 */  sw $a1, 36($sp)
    /* 800D2C38 00803825 */  or $a3, $a0, $zero
    /* 800D2C3C C420417C */  lwc1 $f0, 16764($at)
    /* 800D2C40 18C00021 */  blez $a2, 0x800D2CC8
    /* 800D2C44 00001025 */  or $v0, $zero, $zero
    /* 800D2C48 8C640004 */  lw $a0, 4($v1)
    /* 800D2C4C 84E50000 */  lh $a1, 0($a3)
    /* 800D2C50 84E80002 */  lh $t0, 2($a3)
    /* 800D2C54 84E90004 */  lh $t1, 4($a3)
    /* 800D2C58 848A0000 */  lh $t2, 0($a0)
    /* 800D2C5C 848C0002 */  lh $t4, 2($a0)
    /* 800D2C60 848E0004 */  lh $t6, 4($a0)
    /* 800D2C64 01455823 */  subu $t3, $t2, $a1
    /* 800D2C68 448B2000 */  .word 0x448B2000
    /* 800D2C6C 01886823 */  subu $t5, $t4, $t0
    /* 800D2C70 448D3000 */  .word 0x448D3000
    /* 800D2C74 468020A0 */  .word 0x468020A0
    /* 800D2C78 01C97823 */  subu $t7, $t6, $t1
    /* 800D2C7C 448F4000 */  .word 0x448F4000
    /* 800D2C80 46803320 */  .word 0x46803320
    /* 800D2C84 46021282 */  .word 0x46021282
    /* 800D2C88 00000000 */  nop
    /* 800D2C8C 460C6482 */  .word 0x460C6482
    /* 800D2C90 468043A0 */  .word 0x468043A0
    /* 800D2C94 46125100 */  .word 0x46125100
    /* 800D2C98 460E7182 */  .word 0x460E7182
    /* 800D2C9C 46062400 */  .word 0x46062400
    /* 800D2CA0 4600803C */  .word 0x4600803C
    /* 800D2CA4 00000000 */  nop
    /* 800D2CA8 45000003 */  .word 0x45000003
    /* 800D2CAC 00000000 */  nop
    /* 800D2CB0 46008006 */  .word 0x46008006
    /* 800D2CB4 A7A20006 */  sh $v0, 6($sp)
    /* 800D2CB8 24420001 */  addiu $v0, $v0, 1
    /* 800D2CBC 0046082A */  slt $at, $v0, $a2
    /* 800D2CC0 1420FFE5 */  bne $at, $zero, 0x800D2C58
    /* 800D2CC4 24840008 */  addiu $a0, $a0, 8
    /* 800D2CC8 87A20006 */  lh $v0, 6($sp)
    /* 800D2CCC 03E00008 */  jr $ra
    /* 800D2CD0 27BD0020 */  addiu $sp, $sp, 32
