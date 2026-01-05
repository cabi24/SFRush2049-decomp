# Source: game_code.bin (decompressed)
# Address: 0x800B35A0

glabel bump_sounds_init
    /* 800B35A0 24E70BF0 */  addiu $a3, $a3, 3056
    /* 800B35A4 25299788 */  addiu $t1, $t1, -26744
    /* 800B35A8 254A9450 */  addiu $t2, $t2, -27568
    /* 800B35AC 8C639788 */  lw $v1, -26744($v1)
    /* 800B35B0 24080002 */  addiu $t0, $zero, 2
    /* 800B35B4 1860000A */  blez $v1, 0x800B35E0
    /* 800B35B8 00001025 */  or $v0, $zero, $zero
    /* 800B35BC 3C058015 */  lui $a1, 0x8015
    /* 800B35C0 24A59450 */  addiu $a1, $a1, -27568
    /* 800B35C4 8CAE0000 */  lw $t6, 0($a1)
    /* 800B35C8 508E0006 */  .word 0x508E0006
    /* 800B35CC 00027880 */  sll $t7, $v0, 2
    /* 800B35D0 24420001 */  addiu $v0, $v0, 1
    /* 800B35D4 0043082A */  slt $at, $v0, $v1
    /* 800B35D8 1420FFFA */  bne $at, $zero, 0x800B35C4
    /* 800B35DC 24A50004 */  addiu $a1, $a1, 4
    /* 800B35E0 00027880 */  sll $t7, $v0, 2
    /* 800B35E4 94820034 */  lhu $v0, 52($a0)
    /* 800B35E8 246BFFFF */  addiu $t3, $v1, -1
    /* 800B35EC 000B6080 */  sll $t4, $t3, 2
    /* 800B35F0 0002C140 */  sll $t8, $v0, 5
    /* 800B35F4 00F8C821 */  addu $t9, $a3, $t8
    /* 800B35F8 A3280016 */  sb $t0, 22($t9)
    /* 800B35FC 014C3021 */  addu $a2, $t2, $t4
    /* 800B3600 8CCD0000 */  lw $t5, 0($a2)
    /* 800B3604 014F2821 */  addu $a1, $t2, $t7
    /* 800B3608 AD2B0000 */  sw $t3, 0($t1)
    /* 800B360C ACAD0000 */  sw $t5, 0($a1)
    /* 800B3610 ACC40000 */  sw $a0, 0($a2)
    /* 800B3614 8C84003C */  lw $a0, 60($a0)
    /* 800B3618 01601825 */  or $v1, $t3, $zero
    /* 800B361C 1480FFE5 */  bne $a0, $zero, 0x800B35B4
    /* 800B3620 00000000 */  nop
    /* 800B3624 03E00008 */  jr $ra
    /* 800B3628 00000000 */  nop
