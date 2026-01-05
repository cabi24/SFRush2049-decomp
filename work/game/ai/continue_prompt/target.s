# Source: game_code.bin (decompressed)
# Address: 0x800DB758

glabel continue_prompt
    /* 800DB758 27BDFFC8 */  addiu $sp, $sp, -56
    /* 800DB75C F7B40010 */  .word 0xF7B40010
    /* 800DB760 AFBF0034 */  sw $ra, 52($sp)
    /* 800DB764 AFB2002C */  sw $s2, 44($sp)
    /* 800DB768 AFB10028 */  sw $s1, 40($sp)
    /* 800DB76C F7B60018 */  .word 0xF7B60018
    /* 800DB770 4480A000 */  .word 0x4480A000
    /* 800DB774 3C018012 */  lui $at, 0x8012
    /* 800DB778 AFB30030 */  sw $s3, 48($sp)
    /* 800DB77C AFB00024 */  sw $s0, 36($sp)
    /* 800DB780 C4364280 */  lwc1 $f22, 17024($at)
    /* 800DB784 24110001 */  addiu $s1, $zero, 1
    /* 800DB788 0C032484 */  jal 0x800C9210
    /* 800DB78C 00009025 */  or $s2, $zero, $zero
    /* 800DB790 8FBF0034 */  lw $ra, 52($sp)
    /* 800DB794 D7B40010 */  .word 0xD7B40010
    /* 800DB798 D7B60018 */  .word 0xD7B60018
    /* 800DB79C 8FB00024 */  lw $s0, 36($sp)
    /* 800DB7A0 8FB10028 */  lw $s1, 40($sp)
    /* 800DB7A4 8FB2002C */  lw $s2, 44($sp)
    /* 800DB7A8 8FB30030 */  lw $s3, 48($sp)
    /* 800DB7AC 03E00008 */  jr $ra
    /* 800DB7B0 27BD0038 */  addiu $sp, $sp, 56
