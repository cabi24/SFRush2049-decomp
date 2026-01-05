# Source: game_code.bin (decompressed)
# Address: 0x800C55E4

glabel effect_cleanup
    /* 800C55E4 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800C55E8 3C028015 */  lui $v0, 0x8015
    /* 800C55EC 8C42A110 */  lw $v0, -24304($v0)
    /* 800C55F0 AFA40018 */  sw $a0, 24($sp)
    /* 800C55F4 AFA5001C */  sw $a1, 28($sp)
    /* 800C55F8 AFA60020 */  sw $a2, 32($sp)
    /* 800C55FC 00047600 */  sll $t6, $a0, 24
    /* 800C5600 0005C600 */  sll $t8, $a1, 24
    /* 800C5604 00064600 */  sll $t0, $a2, 24
    /* 800C5608 24010006 */  addiu $at, $zero, 6
    /* 800C560C 00083603 */  sra $a2, $t0, 24
    /* 800C5610 00182E03 */  sra $a1, $t8, 24
    /* 800C5614 000E2603 */  sra $a0, $t6, 24
    /* 800C5618 10410004 */  beq $v0, $at, 0x800C562C
    /* 800C561C AFBF0014 */  sw $ra, 20($sp)
    /* 800C5620 24010004 */  addiu $at, $zero, 4
    /* 800C5624 54410004 */  .word 0x54410004
    /* 800C5628 8FBF0014 */  lw $ra, 20($sp)
    /* 800C562C 0C0E452D */  jal 0x803914B4
    /* 800C5630 00000000 */  nop
    /* 800C5634 8FBF0014 */  lw $ra, 20($sp)
    /* 800C5638 27BD0018 */  addiu $sp, $sp, 24
    /* 800C563C 03E00008 */  jr $ra
    /* 800C5640 00000000 */  nop
