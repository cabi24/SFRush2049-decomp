# Source: game_code.bin (decompressed)
# Address: 0x800A5B60

glabel render_state_init
    /* 800A5B60 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800A5B64 AC2013B4 */  sw $zero, 5044($at)
    /* 800A5B68 AFBF0014 */  sw $ra, 20($sp)
    /* 800A5B6C 3C018016 */  lui $at, 0x8016
    /* 800A5B70 240EFFFF */  addiu $t6, $zero, -1
    /* 800A5B74 0C029690 */  jal 0x800A5A40
    /* 800A5B78 A42EB254 */  sh $t6, -19884($at)
    /* 800A5B7C 3C013F80 */  lui $at, 0x3F80
    /* 800A5B80 44812000 */  .word 0x44812000
    /* 800A5B84 3C018016 */  lui $at, 0x8016
    /* 800A5B88 8FBF0014 */  lw $ra, 20($sp)
    /* 800A5B8C E42413B8 */  swc1 $f4, 5048($at)
    /* 800A5B90 3C018014 */  lui $at, 0x8014
    /* 800A5B94 A4200618 */  sh $zero, 1560($at)
    /* 800A5B98 3C0F8012 */  lui $t7, 0x8012
    /* 800A5B9C 25EFEA18 */  addiu $t7, $t7, -5608
    /* 800A5BA0 3C018014 */  lui $at, 0x8014
    /* 800A5BA4 AC2F06B8 */  sw $t7, 1720($at)
    /* 800A5BA8 03E00008 */  jr $ra
    /* 800A5BAC 27BD0018 */  addiu $sp, $sp, 24
