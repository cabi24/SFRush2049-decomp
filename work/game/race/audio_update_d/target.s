# Source: game_code.bin (decompressed)
# Address: 0x800F8754

glabel audio_update_d
    /* 800F8754 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800F8758 AFBF0014 */  sw $ra, 20($sp)
    /* 800F875C 0C03E15F */  jal 0x800F857C
    /* 800F8760 00008025 */  or $s0, $zero, $zero
    /* 800F8764 0C02D16F */  jal 0x800B45BC
    /* 800F8768 24040001 */  addiu $a0, $zero, 1
    /* 800F876C 3C108011 */  lui $s0, 0x8011
    /* 800F8770 2610472C */  addiu $s0, $s0, 18220
    /* 800F8774 8E040000 */  lw $a0, 0($s0)
    /* 800F8778 50800005 */  .word 0x50800005
    /* 800F877C 8FBF0014 */  lw $ra, 20($sp)
    /* 800F8780 0C02CD63 */  jal 0x800B358C
    /* 800F8784 00000000 */  nop
    /* 800F8788 AE000000 */  sw $zero, 0($s0)
    /* 800F878C 8FBF0014 */  lw $ra, 20($sp)
    /* 800F8790 3C018011 */  lui $at, 0x8011
    /* 800F8794 A0204728 */  sb $zero, 18216($at)
    /* 800F8798 03E00008 */  jr $ra
    /* 800F879C 27BD0018 */  addiu $sp, $sp, 24
