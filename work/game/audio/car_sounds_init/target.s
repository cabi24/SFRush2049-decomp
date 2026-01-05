# Source: game_code.bin (decompressed)
# Address: 0x800B2C40

glabel car_sounds_init
    /* 800B2C40 25089228 */  addiu $t0, $t0, -28120
    /* 800B2C44 24E78928 */  addiu $a3, $a3, -30424
    /* 800B2C48 24C68910 */  addiu $a2, $a2, -30448
    /* 800B2C4C 24A588F8 */  addiu $a1, $a1, -30472
    /* 800B2C50 248488E0 */  addiu $a0, $a0, -30496
    /* 800B2C54 A42B88B6 */  sh $t3, -30538($at)
    /* 800B2C58 AC2388B0 */  sw $v1, -30544($at)
    /* 800B2C5C 2402FFFF */  addiu $v0, $zero, -1
    /* 800B2C60 AC670048 */  sw $a3, 72($v1)
    /* 800B2C64 24E70060 */  addiu $a3, $a3, 96
    /* 800B2C68 AC650018 */  sw $a1, 24($v1)
    /* 800B2C6C AC660030 */  sw $a2, 48($v1)
    /* 800B2C70 AC640000 */  sw $a0, 0($v1)
    /* 800B2C74 24840060 */  addiu $a0, $a0, 96
    /* 800B2C78 24C60060 */  addiu $a2, $a2, 96
    /* 800B2C7C 24A50060 */  addiu $a1, $a1, 96
    /* 800B2C80 A462001E */  sh $v0, 30($v1)
    /* 800B2C84 A4620036 */  sh $v0, 54($v1)
    /* 800B2C88 A462004E */  sh $v0, 78($v1)
    /* 800B2C8C 24630060 */  addiu $v1, $v1, 96
    /* 800B2C90 14E8FFF3 */  bne $a3, $t0, 0x800B2C60
    /* 800B2C94 A462FFA6 */  sh $v0, -90($v1)
    /* 800B2C98 AC600000 */  sw $zero, 0($v1)
    /* 800B2C9C A4620006 */  sh $v0, 6($v1)
    /* 800B2CA0 3C018014 */  lui $at, 0x8014
    /* 800B2CA4 AC2091F0 */  sw $zero, -28176($at)
    /* 800B2CA8 3C018013 */  lui $at, 0x8013
    /* 800B2CAC 03E00008 */  jr $ra
    /* 800B2CB0 A420E66C */  sh $zero, -6548($at)
