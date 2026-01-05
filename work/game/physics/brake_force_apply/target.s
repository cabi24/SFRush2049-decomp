# Source: game_code.bin (decompressed)
# Address: 0x800AC75C

glabel brake_force_apply
    /* 800AC75C 27BDFFB8 */  addiu $sp, $sp, -72
    /* 800AC760 AFA40048 */  sw $a0, 72($sp)
    /* 800AC764 AFBF003C */  sw $ra, 60($sp)
    /* 800AC768 3C048015 */  lui $a0, 0x8015
    /* 800AC76C 3C058015 */  lui $a1, 0x8015
    /* 800AC770 AFBE0038 */  sw $fp, 56($sp)
    /* 800AC774 AFB70034 */  sw $s7, 52($sp)
    /* 800AC778 AFB60030 */  sw $s6, 48($sp)
    /* 800AC77C AFB5002C */  sw $s5, 44($sp)
    /* 800AC780 AFB40028 */  sw $s4, 40($sp)
    /* 800AC784 AFB30024 */  sw $s3, 36($sp)
    /* 800AC788 AFB20020 */  sw $s2, 32($sp)
    /* 800AC78C AFB1001C */  sw $s1, 28($sp)
    /* 800AC790 AFB00018 */  sw $s0, 24($sp)
    /* 800AC794 24A56BD4 */  addiu $a1, $a1, 27604
    /* 800AC798 24846BB0 */  addiu $a0, $a0, 27568
    /* 800AC79C 0C001A80 */  jal 0x80006A00
    /* 800AC7A0 24060001 */  addiu $a2, $zero, 1
    /* 800AC7A4 3C048003 */  lui $a0, 0x8003
    /* 800AC7A8 24844690 */  addiu $a0, $a0, 18064
    /* 800AC7AC 0C0020E0 */  jal 0x80008380
    /* 800AC7B0 24050003 */  addiu $a1, $zero, 3
    /* 800AC7B4 3C048015 */  lui $a0, 0x8015
    /* 800AC7B8 24846BB0 */  addiu $a0, $a0, 27568
    /* 800AC7BC 27A50044 */  addiu $a1, $sp, 68
    /* 800AC7C0 0C001C9C */  jal 0x80007270
    /* 800AC7C4 24060001 */  addiu $a2, $zero, 1
    /* 800AC7C8 3C1E8015 */  lui $fp, 0x8015
    /* 800AC7CC 0C025C59 */  jal 0x80097164
    /* 800AC7D0 8FDE69AC */  lw $fp, 27052($fp)
    /* 800AC7D4 3C048003 */  lui $a0, 0x8003
    /* 800AC7D8 24845440 */  addiu $a0, $a0, 21568
    /* 800AC7DC 00002825 */  or $a1, $zero, $zero
    /* 800AC7E0 0C001D78 */  jal 0x800075E0
    /* 800AC7E4 24060001 */  addiu $a2, $zero, 1
    /* 800AC7E8 1000FFF2 */  beq $zero, $zero, 0x800AC7B4
    /* 800AC7EC 00000000 */  nop
    /* 800AC7F0 8FBF003C */  lw $ra, 60($sp)
    /* 800AC7F4 8FB00018 */  lw $s0, 24($sp)
    /* 800AC7F8 8FB1001C */  lw $s1, 28($sp)
    /* 800AC7FC 8FB20020 */  lw $s2, 32($sp)
    /* 800AC800 8FB30024 */  lw $s3, 36($sp)
    /* 800AC804 8FB40028 */  lw $s4, 40($sp)
    /* 800AC808 8FB5002C */  lw $s5, 44($sp)
    /* 800AC80C 8FB60030 */  lw $s6, 48($sp)
    /* 800AC810 8FB70034 */  lw $s7, 52($sp)
    /* 800AC814 8FBE0038 */  lw $fp, 56($sp)
    /* 800AC818 03E00008 */  jr $ra
    /* 800AC81C 27BD0048 */  addiu $sp, $sp, 72
