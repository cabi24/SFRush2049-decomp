# Source: game_code.bin (decompressed)
# Address: 0x80091C04

glabel scheduler_recv
    /* 80091C04 27BDFFE0 */  addiu $sp, $sp, -32
    /* 80091C08 AFA40020 */  sw $a0, 32($sp)
    /* 80091C0C AFBF0014 */  sw $ra, 20($sp)
    /* 80091C10 3C048014 */  lui $a0, 0x8014
    /* 80091C14 24842728 */  addiu $a0, $a0, 10024
    /* 80091C18 AFA0001C */  sw $zero, 28($sp)
    /* 80091C1C 00002825 */  or $a1, $zero, $zero
    /* 80091C20 0C001C9C */  jal 0x80007270
    /* 80091C24 24060001 */  addiu $a2, $zero, 1
    /* 80091C28 8FA7001C */  lw $a3, 28($sp)
    /* 80091C2C 0C0246EA */  jal 0x80091BA8
    /* 80091C30 8FA40020 */  lw $a0, 32($sp)
    /* 80091C34 1040000A */  beq $v0, $zero, 0x80091C60
    /* 80091C38 00402025 */  or $a0, $v0, $zero
    /* 80091C3C 0C0246C0 */  jal 0x80091B00
    /* 80091C40 00000000 */  nop
    /* 80091C44 240E0006 */  addiu $t6, $zero, 6
    /* 80091C48 A04E0002 */  sb $t6, 2($v0)
    /* 80091C4C AC440004 */  sw $a0, 4($v0)
    /* 80091C50 908F001A */  lbu $t7, 26($a0)
    /* 80091C54 00403825 */  or $a3, $v0, $zero
    /* 80091C58 25F80001 */  addiu $t8, $t7, 1
    /* 80091C5C A098001A */  sb $t8, 26($a0)
    /* 80091C60 3C048014 */  lui $a0, 0x8014
    /* 80091C64 24842728 */  addiu $a0, $a0, 10024
    /* 80091C68 00002825 */  or $a1, $zero, $zero
    /* 80091C6C 00003025 */  or $a2, $zero, $zero
    /* 80091C70 0C001D78 */  jal 0x800075E0
    /* 80091C74 AFA7001C */  sw $a3, 28($sp)
    /* 80091C78 8FA7001C */  lw $a3, 28($sp)
    /* 80091C7C 3C048014 */  lui $a0, 0x8014
    /* 80091C80 248427A8 */  addiu $a0, $a0, 10152
    /* 80091C84 10E00003 */  beq $a3, $zero, 0x80091C94
    /* 80091C88 00E02825 */  or $a1, $a3, $zero
    /* 80091C8C 0C001D78 */  jal 0x800075E0
    /* 80091C90 00003025 */  or $a2, $zero, $zero
    /* 80091C94 8FBF0014 */  lw $ra, 20($sp)
    /* 80091C98 27BD0020 */  addiu $sp, $sp, 32
    /* 80091C9C 03E00008 */  jr $ra
    /* 80091CA0 00000000 */  nop
