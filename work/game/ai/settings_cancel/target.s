# Source: game_code.bin (decompressed)
# Address: 0x800DEBAC

glabel settings_cancel
    /* 800DEBAC 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800DEBB0 2401FFFF */  addiu $at, $zero, -1
    /* 800DEBB4 AFBF0014 */  sw $ra, 20($sp)
    /* 800DEBB8 00804025 */  or $t0, $a0, $zero
    /* 800DEBBC 00A04825 */  or $t1, $a1, $zero
    /* 800DEBC0 1081002E */  beq $a0, $at, 0x800DEC7C
    /* 800DEBC4 00C05025 */  or $t2, $a2, $zero
    /* 800DEBC8 3C048014 */  lui $a0, 0x8014
    /* 800DEBCC 24842728 */  addiu $a0, $a0, 10024
    /* 800DEBD0 00002825 */  or $a1, $zero, $zero
    /* 800DEBD4 24060001 */  addiu $a2, $zero, 1
    /* 800DEBD8 AFA70024 */  sw $a3, 36($sp)
    /* 800DEBDC AFA80018 */  sw $t0, 24($sp)
    /* 800DEBE0 AFA9001C */  sw $t1, 28($sp)
    /* 800DEBE4 0C001C9C */  jal 0x80007270
    /* 800DEBE8 AFAA0020 */  sw $t2, 32($sp)
    /* 800DEBEC 8FA80018 */  lw $t0, 24($sp)
    /* 800DEBF0 8FA70024 */  lw $a3, 36($sp)
    /* 800DEBF4 8FA9001C */  lw $t1, 28($sp)
    /* 800DEBF8 8FAA0020 */  lw $t2, 32($sp)
    /* 800DEBFC 0C0354B3 */  jal 0x800D52CC
    /* 800DEC00 01002025 */  or $a0, $t0, $zero
    /* 800DEC04 C5240000 */  lwc1 $f4, 0($t1)
    /* 800DEC08 8FA20028 */  lw $v0, 40($sp)
    /* 800DEC0C 3C048014 */  lui $a0, 0x8014
    /* 800DEC10 E504000C */  swc1 $f4, 12($t0)
    /* 800DEC14 C5260004 */  lwc1 $f6, 4($t1)
    /* 800DEC18 24842728 */  addiu $a0, $a0, 10024
    /* 800DEC1C 00002825 */  or $a1, $zero, $zero
    /* 800DEC20 E5060010 */  swc1 $f6, 16($t0)
    /* 800DEC24 C5280008 */  lwc1 $f8, 8($t1)
    /* 800DEC28 00003025 */  or $a2, $zero, $zero
    /* 800DEC2C E5080014 */  swc1 $f8, 20($t0)
    /* 800DEC30 C54A0000 */  lwc1 $f10, 0($t2)
    /* 800DEC34 E50A0018 */  swc1 $f10, 24($t0)
    /* 800DEC38 C5440004 */  lwc1 $f4, 4($t2)
    /* 800DEC3C E504001C */  swc1 $f4, 28($t0)
    /* 800DEC40 C5460008 */  lwc1 $f6, 8($t2)
    /* 800DEC44 E5060020 */  swc1 $f6, 32($t0)
    /* 800DEC48 C4E80000 */  lwc1 $f8, 0($a3)
    /* 800DEC4C E5080024 */  swc1 $f8, 36($t0)
    /* 800DEC50 C4EA0004 */  lwc1 $f10, 4($a3)
    /* 800DEC54 E50A0028 */  swc1 $f10, 40($t0)
    /* 800DEC58 C4E40008 */  lwc1 $f4, 8($a3)
    /* 800DEC5C E504002C */  swc1 $f4, 44($t0)
    /* 800DEC60 C4460000 */  lwc1 $f6, 0($v0)
    /* 800DEC64 E5060030 */  swc1 $f6, 48($t0)
    /* 800DEC68 C4480004 */  lwc1 $f8, 4($v0)
    /* 800DEC6C E5080034 */  swc1 $f8, 52($t0)
    /* 800DEC70 C44A0008 */  lwc1 $f10, 8($v0)
    /* 800DEC74 0C001D78 */  jal 0x800075E0
    /* 800DEC78 E50A0038 */  swc1 $f10, 56($t0)
    /* 800DEC7C 8FBF0014 */  lw $ra, 20($sp)
    /* 800DEC80 27BD0018 */  addiu $sp, $sp, 24
    /* 800DEC84 03E00008 */  jr $ra
    /* 800DEC88 00000000 */  nop
