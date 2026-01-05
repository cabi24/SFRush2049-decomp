# Source: game_code.bin (decompressed)
# Address: 0x800B4738

glabel voice_stop_2
    /* 800B4738 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800B473C AFBF0014 */  sw $ra, 20($sp)
    /* 800B4740 AFA4001C */  sw $a0, 28($sp)
    /* 800B4744 AFA60020 */  sw $a2, 32($sp)
    /* 800B4748 8D190000 */  lw $t9, 0($t0)
    /* 800B474C 308E00FF */  andi $t6, $a0, 0x00FF
    /* 800B4750 01C02025 */  or $a0, $t6, $zero
    /* 800B4754 00067E00 */  sll $t7, $a2, 24
    /* 800B4758 000F3603 */  sra $a2, $t7, 24
    /* 800B475C 8F2E002C */  lw $t6, 44($t9)
    /* 800B4760 248FFFEB */  addiu $t7, $a0, -21
    /* 800B4764 2DE1000B */  sltiu $at, $t7, 11
    /* 800B4768 10200027 */  beq $at, $zero, 0x800B4808
    /* 800B476C 8DC70000 */  lw $a3, 0($t6)
    /* 800B4770 000F7880 */  sll $t7, $t7, 2
    /* 800B4774 3C018012 */  lui $at, 0x8012
    /* 800B4778 002F0821 */  addu $at, $at, $t7
    /* 800B477C 8C2F3C38 */  lw $t7, 15416($at)
    /* 800B4780 01E00008 */  jr $t7
    /* 800B4784 00000000 */  nop
    /* 800B4788 10000012 */  beq $zero, $zero, 0x800B47D4
    /* 800B478C 24E20050 */  addiu $v0, $a3, 80
    /* 800B4790 10000010 */  beq $zero, $zero, 0x800B47D4
    /* 800B4794 24E20051 */  addiu $v0, $a3, 81
    /* 800B4798 1000000E */  beq $zero, $zero, 0x800B47D4
    /* 800B479C 24E20052 */  addiu $v0, $a3, 82
    /* 800B47A0 1000000C */  beq $zero, $zero, 0x800B47D4
    /* 800B47A4 24E20053 */  addiu $v0, $a3, 83
    /* 800B47A8 1000000A */  beq $zero, $zero, 0x800B47D4
    /* 800B47AC 24E20054 */  addiu $v0, $a3, 84
    /* 800B47B0 10000008 */  beq $zero, $zero, 0x800B47D4
    /* 800B47B4 24E20055 */  addiu $v0, $a3, 85
    /* 800B47B8 10000006 */  beq $zero, $zero, 0x800B47D4
    /* 800B47BC 24E20056 */  addiu $v0, $a3, 86
    /* 800B47C0 10000004 */  beq $zero, $zero, 0x800B47D4
    /* 800B47C4 24E20057 */  addiu $v0, $a3, 87
    /* 800B47C8 10000002 */  beq $zero, $zero, 0x800B47D4
    /* 800B47CC 24E20058 */  addiu $v0, $a3, 88
    /* 800B47D0 24E20059 */  addiu $v0, $a3, 89
    /* 800B47D4 80580000 */  lb $t8, 0($v0)
    /* 800B47D8 24E4004C */  addiu $a0, $a3, 76
    /* 800B47DC 24050010 */  addiu $a1, $zero, 16
    /* 800B47E0 50D8000A */  .word 0x50D8000A
    /* 800B47E4 8FBF0014 */  lw $ra, 20($sp)
    /* 800B47E8 0C02D19B */  jal 0x800B466C
    /* 800B47EC A0460000 */  sb $a2, 0($v0)
    /* 800B47F0 ACE20048 */  sw $v0, 72($a3)
    /* 800B47F4 8D190000 */  lw $t9, 0($t0)
    /* 800B47F8 24E50048 */  addiu $a1, $a3, 72
    /* 800B47FC 24060014 */  addiu $a2, $zero, 20
    /* 800B4800 0C028941 */  jal 0x800A2504
    /* 800B4804 8F240008 */  lw $a0, 8($t9)
    /* 800B4808 8FBF0014 */  lw $ra, 20($sp)
    /* 800B480C 27BD0018 */  addiu $sp, $sp, 24
    /* 800B4810 03E00008 */  jr $ra
    /* 800B4814 00000000 */  nop
