# Source: game_code.bin (decompressed)
# Address: 0x8008A398

glabel audio_sync_wait
    /* 8008A398 27BDFFE8 */  addiu $sp, $sp, -24
    /* 8008A39C 308EFFFF */  andi $t6, $a0, 0xFFFF
    /* 8008A3A0 AFBF0014 */  sw $ra, 20($sp)
    /* 8008A3A4 11CF0004 */  beq $t6, $t7, 0x8008A3B8
    /* 8008A3A8 AFA40018 */  sw $a0, 24($sp)
    /* 8008A3AC A10E0003 */  sb $t6, 3($t0)
    /* 8008A3B0 3C018013 */  lui $at, 0x8013
    /* 8008A3B4 AC20E6D0 */  sw $zero, -6448($at)
    /* 8008A3B8 01002025 */  or $a0, $t0, $zero
    /* 8008A3BC 24050004 */  addiu $a1, $zero, 4
    /* 8008A3C0 24060001 */  addiu $a2, $zero, 1
    /* 8008A3C4 0C022852 */  jal 0x8008A148
    /* 8008A3C8 00003825 */  or $a3, $zero, $zero
    /* 8008A3CC 0C021E38 */  jal 0x800878E0
    /* 8008A3D0 24040020 */  addiu $a0, $zero, 32
    /* 8008A3D4 8FBF0014 */  lw $ra, 20($sp)
    /* 8008A3D8 27BD0018 */  addiu $sp, $sp, 24
    /* 8008A3DC 03E00008 */  jr $ra
    /* 8008A3E0 00000000 */  nop
