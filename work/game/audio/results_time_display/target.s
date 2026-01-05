# Source: game_code.bin (decompressed)
# Address: 0x800BF0A4

glabel results_time_display
    /* 800BF0A4 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800BF0A8 AFA40020 */  sw $a0, 32($sp)
    /* 800BF0AC AFBF0014 */  sw $ra, 20($sp)
    /* 800BF0B0 3C048014 */  lui $a0, 0x8014
    /* 800BF0B4 24842728 */  addiu $a0, $a0, 10024
    /* 800BF0B8 00002825 */  or $a1, $zero, $zero
    /* 800BF0BC 0C001C9C */  jal 0x80007270
    /* 800BF0C0 24060001 */  addiu $a2, $zero, 1
    /* 800BF0C4 0C0246EA */  jal 0x80091BA8
    /* 800BF0C8 8FA40020 */  lw $a0, 32($sp)
    /* 800BF0CC 10400012 */  beq $v0, $zero, 0x800BF118
    /* 800BF0D0 00402825 */  or $a1, $v0, $zero
    /* 800BF0D4 8C430010 */  lw $v1, 16($v0)
    /* 800BF0D8 24010001 */  addiu $at, $zero, 1
    /* 800BF0DC 10610003 */  beq $v1, $at, 0x800BF0EC
    /* 800BF0E0 24010003 */  addiu $at, $zero, 3
    /* 800BF0E4 54610004 */  .word 0x54610004
    /* 800BF0E8 24010002 */  addiu $at, $zero, 2
    /* 800BF0EC 1000000B */  beq $zero, $zero, 0x800BF11C
    /* 800BF0F0 24030001 */  addiu $v1, $zero, 1
    /* 800BF0F4 24010002 */  addiu $at, $zero, 2
    /* 800BF0F8 10610003 */  beq $v1, $at, 0x800BF108
    /* 800BF0FC 00000000 */  nop
    /* 800BF100 10000006 */  beq $zero, $zero, 0x800BF11C
    /* 800BF104 00001825 */  or $v1, $zero, $zero
    /* 800BF108 0C0261FA */  jal 0x800987E8
    /* 800BF10C 8CA4003C */  lw $a0, 60($a1)
    /* 800BF110 10000002 */  beq $zero, $zero, 0x800BF11C
    /* 800BF114 00401825 */  or $v1, $v0, $zero
    /* 800BF118 00001825 */  or $v1, $zero, $zero
    /* 800BF11C 3C048014 */  lui $a0, 0x8014
    /* 800BF120 24842728 */  addiu $a0, $a0, 10024
    /* 800BF124 00002825 */  or $a1, $zero, $zero
    /* 800BF128 00003025 */  or $a2, $zero, $zero
    /* 800BF12C 0C001D78 */  jal 0x800075E0
    /* 800BF130 AFA3001C */  sw $v1, 28($sp)
    /* 800BF134 8FBF0014 */  lw $ra, 20($sp)
    /* 800BF138 8FA2001C */  lw $v0, 28($sp)
    /* 800BF13C 27BD0020 */  addiu $sp, $sp, 32
    /* 800BF140 03E00008 */  jr $ra
    /* 800BF144 00000000 */  nop
