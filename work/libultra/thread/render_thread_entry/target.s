# Source: game_code.bin (decompressed)
# Address: 0x800E7808

glabel render_thread_entry
    /* 800E7808 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800E780C AFA40040 */  sw $a0, 64($sp)
    /* 800E7810 AFBF002C */  sw $ra, 44($sp)
    /* 800E7814 3C040100 */  lui $a0, 0x0100
    /* 800E7818 AFB50028 */  sw $s5, 40($sp)
    /* 800E781C AFB40024 */  sw $s4, 36($sp)
    /* 800E7820 AFB30020 */  sw $s3, 32($sp)
    /* 800E7824 AFB2001C */  sw $s2, 28($sp)
    /* 800E7828 AFB10018 */  sw $s1, 24($sp)
    /* 800E782C AFB00014 */  sw $s0, 20($sp)
    /* 800E7830 AFA00034 */  sw $zero, 52($sp)
    /* 800E7834 0C00127C */  jal 0x800049F0
    /* 800E7838 34840E00 */  ori $a0, $a0, 0x0E00
    /* 800E783C 3C148018 */  lui $s4, 0x8018
    /* 800E7840 2694A498 */  addiu $s4, $s4, -23400
    /* 800E7844 3C058018 */  lui $a1, 0x8018
    /* 800E7848 24A5A4B8 */  addiu $a1, $a1, -23368
    /* 800E784C 02802025 */  or $a0, $s4, $zero
    /* 800E7850 0C001A80 */  jal 0x80006A00
    /* 800E7854 24060008 */  addiu $a2, $zero, 8
    /* 800E7858 0C039DC4 */  jal 0x800E7710
    /* 800E785C 00000000 */  nop
    /* 800E7860 3C158015 */  lui $s5, 0x8015
    /* 800E7864 26B525F0 */  addiu $s5, $s5, 9712
    /* 800E7868 A6A00000 */  sh $zero, 0($s5)
    /* 800E786C 0C039D8B */  jal 0x800E762C
    /* 800E7870 00002025 */  or $a0, $zero, $zero
    /* 800E7874 3C048003 */  lui $a0, 0x8003
    /* 800E7878 2484E8E8 */  addiu $a0, $a0, -5912
    /* 800E787C 27A50038 */  addiu $a1, $sp, 56
    /* 800E7880 0C000175 */  jal 0x800005D4
    /* 800E7884 02803025 */  or $a2, $s4, $zero
    /* 800E7888 2413FFFF */  addiu $s3, $zero, -1
    /* 800E788C 24110001 */  addiu $s1, $zero, 1
    /* 800E7890 27B00034 */  addiu $s0, $sp, 52
    /* 800E7894 02802025 */  or $a0, $s4, $zero
    /* 800E7898 02002825 */  or $a1, $s0, $zero
    /* 800E789C 0C001C9C */  jal 0x80007270
    /* 800E78A0 02203025 */  or $a2, $s1, $zero
    /* 800E78A4 02802025 */  or $a0, $s4, $zero
    /* 800E78A8 02002825 */  or $a1, $s0, $zero
    /* 800E78AC 0C001C9C */  jal 0x80007270
    /* 800E78B0 00003025 */  or $a2, $zero, $zero
    /* 800E78B4 5453FFFC */  .word 0x5453FFFC
    /* 800E78B8 02802025 */  or $a0, $s4, $zero
    /* 800E78BC 0C039CF6 */  jal 0x800E73D8
    /* 800E78C0 00000000 */  nop
    /* 800E78C4 5040FFF4 */  .word 0x5040FFF4
    /* 800E78C8 02802025 */  or $a0, $s4, $zero
    /* 800E78CC 96AF0000 */  lhu $t7, 0($s5)
    /* 800E78D0 51E0FFF1 */  .word 0x51E0FFF1
    /* 800E78D4 02802025 */  or $a0, $s4, $zero
    /* 800E78D8 0C039ABE */  jal 0x800E6AF8
    /* 800E78DC 00000000 */  nop
    /* 800E78E0 1000FFED */  beq $zero, $zero, 0x800E7898
    /* 800E78E4 02802025 */  or $a0, $s4, $zero
    /* 800E78E8 00000000 */  nop
    /* 800E78EC 00000000 */  nop
    /* 800E78F0 8FBF002C */  lw $ra, 44($sp)
    /* 800E78F4 8FB00014 */  lw $s0, 20($sp)
    /* 800E78F8 8FB10018 */  lw $s1, 24($sp)
    /* 800E78FC 8FB2001C */  lw $s2, 28($sp)
    /* 800E7900 8FB30020 */  lw $s3, 32($sp)
    /* 800E7904 8FB40024 */  lw $s4, 36($sp)
    /* 800E7908 8FB50028 */  lw $s5, 40($sp)
    /* 800E790C 03E00008 */  jr $ra
    /* 800E7910 27BD0040 */  addiu $sp, $sp, 64
