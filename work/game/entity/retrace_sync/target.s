# Source: game_code.bin (decompressed)
# Address: 0x80091F34

glabel retrace_sync
    /* 80091F34 27BDFFB8 */  addiu $sp, $sp, -72
    /* 80091F38 AFA40048 */  sw $a0, 72($sp)
    /* 80091F3C AFBF0044 */  sw $ra, 68($sp)
    /* 80091F40 AFA5004C */  sw $a1, 76($sp)
    /* 80091F44 3C048014 */  lui $a0, 0x8014
    /* 80091F48 F7BC0038 */  .word 0xF7BC0038
    /* 80091F4C F7BA0030 */  .word 0xF7BA0030
    /* 80091F50 F7B80028 */  .word 0xF7B80028
    /* 80091F54 F7B60020 */  .word 0xF7B60020
    /* 80091F58 F7B40018 */  .word 0xF7B40018
    /* 80091F5C 24842728 */  addiu $a0, $a0, 10024
    /* 80091F60 00002825 */  or $a1, $zero, $zero
    /* 80091F64 0C001C9C */  jal 0x80007270
    /* 80091F68 24060001 */  addiu $a2, $zero, 1
    /* 80091F6C 3C01C000 */  lui $at, 0xC000
    /* 80091F70 4481D000 */  .word 0x4481D000
    /* 80091F74 8FA40048 */  lw $a0, 72($sp)
    /* 80091F78 C7BC004C */  lwc1 $f28, 76($sp)
    /* 80091F7C 4600D586 */  .word 0x4600D586
    /* 80091F80 0C024729 */  jal 0x80091CA4
    /* 80091F84 4600D606 */  .word 0x4600D606
    /* 80091F88 3C048014 */  lui $a0, 0x8014
    /* 80091F8C 24842728 */  addiu $a0, $a0, 10024
    /* 80091F90 00002825 */  or $a1, $zero, $zero
    /* 80091F94 0C001D78 */  jal 0x800075E0
    /* 80091F98 00003025 */  or $a2, $zero, $zero
    /* 80091F9C 8FBF0044 */  lw $ra, 68($sp)
    /* 80091FA0 D7B40018 */  .word 0xD7B40018
    /* 80091FA4 D7B60020 */  .word 0xD7B60020
    /* 80091FA8 D7B80028 */  .word 0xD7B80028
    /* 80091FAC D7BA0030 */  .word 0xD7BA0030
    /* 80091FB0 D7BC0038 */  .word 0xD7BC0038
    /* 80091FB4 03E00008 */  jr $ra
    /* 80091FB8 27BD0048 */  addiu $sp, $sp, 72
