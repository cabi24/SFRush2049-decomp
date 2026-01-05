# Source: game_code.bin (decompressed)
# Address: 0x800AED64

glabel camera_target_track
    /* 800AED64 27BDFFC8 */  addiu $sp, $sp, -56
    /* 800AED68 3C0E8011 */  lui $t6, 0x8011
    /* 800AED6C 8DCE0260 */  lw $t6, 608($t6)
    /* 800AED70 AFA70044 */  sw $a3, 68($sp)
    /* 800AED74 00803825 */  or $a3, $a0, $zero
    /* 800AED78 AFBF001C */  sw $ra, 28($sp)
    /* 800AED7C AFB10018 */  sw $s1, 24($sp)
    /* 800AED80 AFB00014 */  sw $s0, 20($sp)
    /* 800AED84 AFA40038 */  sw $a0, 56($sp)
    /* 800AED88 AFA5003C */  sw $a1, 60($sp)
    /* 800AED8C 15C00003 */  bne $t6, $zero, 0x800AED9C
    /* 800AED90 AFA60040 */  sw $a2, 64($sp)
    /* 800AED94 1000008D */  beq $zero, $zero, 0x800AEFCC
    /* 800AED98 2402FFFF */  addiu $v0, $zero, -1
    /* 800AED9C 3C048014 */  lui $a0, 0x8014
    /* 800AEDA0 24842728 */  addiu $a0, $a0, 10024
    /* 800AEDA4 00002825 */  or $a1, $zero, $zero
    /* 800AEDA8 24060001 */  addiu $a2, $zero, 1
    /* 800AEDAC 0C001C9C */  jal 0x80007270
    /* 800AEDB0 AFA70038 */  sw $a3, 56($sp)
    /* 800AEDB4 0C02BB48 */  jal 0x800AED20
    /* 800AEDB8 00000000 */  nop
    /* 800AEDBC 8FA70038 */  lw $a3, 56($sp)
    /* 800AEDC0 14400008 */  bne $v0, $zero, 0x800AEDE4
    /* 800AEDC4 00401825 */  or $v1, $v0, $zero
    /* 800AEDC8 3C048014 */  lui $a0, 0x8014
    /* 800AEDCC 24842728 */  addiu $a0, $a0, 10024
    /* 800AEDD0 00002825 */  or $a1, $zero, $zero
    /* 800AEDD4 0C001D78 */  jal 0x800075E0
    /* 800AEDD8 00003025 */  or $a2, $zero, $zero
    /* 800AEDDC 1000007B */  beq $zero, $zero, 0x800AEFCC
    /* 800AEDE0 2402FFFF */  addiu $v0, $zero, -1
    /* 800AEDE4 C4E40000 */  lwc1 $f4, 0($a3)
    /* 800AEDE8 C7A20048 */  lwc1 $f2, 72($sp)
    /* 800AEDEC 44806000 */  .word 0x44806000
    /* 800AEDF0 E444000C */  swc1 $f4, 12($v0)
    /* 800AEDF4 C4E60004 */  lwc1 $f6, 4($a3)
    /* 800AEDF8 460C103C */  .word 0x460C103C
    /* 800AEDFC 3C013F80 */  lui $at, 0x3F80
    /* 800AEE00 E4460010 */  swc1 $f6, 16($v0)
    /* 800AEE04 C4E80008 */  lwc1 $f8, 8($a3)
    /* 800AEE08 E4480014 */  swc1 $f8, 20($v0)
    /* 800AEE0C C7AA0040 */  lwc1 $f10, 64($sp)
    /* 800AEE10 45000005 */  .word 0x45000005
    /* 800AEE14 E44A0018 */  swc1 $f10, 24($v0)
    /* 800AEE18 3C013F80 */  lui $at, 0x3F80
    /* 800AEE1C 44817000 */  .word 0x44817000
    /* 800AEE20 1000000B */  beq $zero, $zero, 0x800AEE50
    /* 800AEE24 E44C001C */  swc1 $f12, 28($v0)
    /* 800AEE28 44817000 */  .word 0x44817000
    /* 800AEE2C 00000000 */  nop
    /* 800AEE30 4602703C */  .word 0x4602703C
    /* 800AEE34 00000000 */  nop
    /* 800AEE38 45020004 */  .word 0x45020004
    /* 800AEE3C 46001006 */  .word 0x46001006
    /* 800AEE40 10000002 */  beq $zero, $zero, 0x800AEE4C
    /* 800AEE44 46007006 */  .word 0x46007006
    /* 800AEE48 46001006 */  .word 0x46001006
    /* 800AEE4C E440001C */  swc1 $f0, 28($v0)
    /* 800AEE50 C7A2004C */  lwc1 $f2, 76($sp)
    /* 800AEE54 460C103C */  .word 0x460C103C
    /* 800AEE58 00000000 */  nop
    /* 800AEE5C 45020004 */  .word 0x45020004
    /* 800AEE60 4602703C */  .word 0x4602703C
    /* 800AEE64 10000009 */  beq $zero, $zero, 0x800AEE8C
    /* 800AEE68 E44C0020 */  swc1 $f12, 32($v0)
    /* 800AEE6C 4602703C */  .word 0x4602703C
    /* 800AEE70 00000000 */  nop
    /* 800AEE74 45020004 */  .word 0x45020004
    /* 800AEE78 46001006 */  .word 0x46001006
    /* 800AEE7C 10000002 */  beq $zero, $zero, 0x800AEE88
    /* 800AEE80 46007006 */  .word 0x46007006
    /* 800AEE84 46001006 */  .word 0x46001006
    /* 800AEE88 E4400020 */  swc1 $f0, 32($v0)
    /* 800AEE8C E46E0024 */  swc1 $f14, 36($v1)
    /* 800AEE90 0C0246C0 */  jal 0x80091B00
    /* 800AEE94 AFA30030 */  sw $v1, 48($sp)
    /* 800AEE98 240F0002 */  addiu $t7, $zero, 2
    /* 800AEE9C A04F0002 */  sb $t7, 2($v0)
    /* 800AEEA0 8FB80058 */  lw $t8, 88($sp)
    /* 800AEEA4 AC580004 */  sw $t8, 4($v0)
    /* 800AEEA8 0C02489E */  jal 0x80092278
    /* 800AEEAC AFA20034 */  sw $v0, 52($sp)
    /* 800AEEB0 8FA70034 */  lw $a3, 52($sp)
    /* 800AEEB4 8FA30050 */  lw $v1, 80($sp)
    /* 800AEEB8 8FAA0030 */  lw $t2, 48($sp)
    /* 800AEEBC 24090001 */  addiu $t1, $zero, 1
    /* 800AEEC0 ACE20014 */  sw $v0, 20($a3)
    /* 800AEEC4 AC490014 */  sw $t1, 20($v0)
    /* 800AEEC8 8CF90014 */  lw $t9, 20($a3)
    /* 800AEECC 44806000 */  .word 0x44806000
    /* 800AEED0 3C01C000 */  lui $at, 0xC000
    /* 800AEED4 AF230034 */  sw $v1, 52($t9)
    /* 800AEED8 8FAE0054 */  lw $t6, 84($sp)
    /* 800AEEDC 8CEF0014 */  lw $t7, 20($a3)
    /* 800AEEE0 44812000 */  .word 0x44812000
    /* 800AEEE4 3C048014 */  lui $a0, 0x8014
    /* 800AEEE8 ADEE0038 */  sw $t6, 56($t7)
    /* 800AEEEC 93B8005F */  lbu $t8, 95($sp)
    /* 800AEEF0 8CF90014 */  lw $t9, 20($a3)
    /* 800AEEF4 24842728 */  addiu $a0, $a0, 10024
    /* 800AEEF8 00002825 */  or $a1, $zero, $zero
    /* 800AEEFC A338001B */  sb $t8, 27($t9)
    /* 800AEF00 8CEE0014 */  lw $t6, 20($a3)
    /* 800AEF04 24180001 */  addiu $t8, $zero, 1
    /* 800AEF08 00003025 */  or $a2, $zero, $zero
    /* 800AEF0C ADC90010 */  sw $t1, 16($t6)
    /* 800AEF10 8CEF0014 */  lw $t7, 20($a3)
    /* 800AEF14 3C0E8012 */  lui $t6, 0x8012
    /* 800AEF18 01C37021 */  addu $t6, $t6, $v1
    /* 800AEF1C A1E00018 */  sb $zero, 24($t7)
    /* 800AEF20 8CF90014 */  lw $t9, 20($a3)
    /* 800AEF24 A338001A */  sb $t8, 26($t9)
    /* 800AEF28 91CEF5CC */  lbu $t6, -2612($t6)
    /* 800AEF2C 8CEF0014 */  lw $t7, 20($a3)
    /* 800AEF30 A1EE0019 */  sb $t6, 25($t7)
    /* 800AEF34 8CF80014 */  lw $t8, 20($a3)
    /* 800AEF38 E4EC0008 */  swc1 $f12, 8($a3)
    /* 800AEF3C E4EC000C */  swc1 $f12, 12($a3)
    /* 800AEF40 E4E40010 */  swc1 $f4, 16($a3)
    /* 800AEF44 AF0A0040 */  sw $t2, 64($t8)
    /* 800AEF48 8CF90014 */  lw $t9, 20($a3)
    /* 800AEF4C 8F28000C */  lw $t0, 12($t9)
    /* 800AEF50 AD480028 */  sw $t0, 40($t2)
    /* 800AEF54 AFA70034 */  sw $a3, 52($sp)
    /* 800AEF58 0C001D78 */  jal 0x800075E0
    /* 800AEF5C AFA80020 */  sw $t0, 32($sp)
    /* 800AEF60 3C048014 */  lui $a0, 0x8014
    /* 800AEF64 8FA50034 */  lw $a1, 52($sp)
    /* 800AEF68 248427A8 */  addiu $a0, $a0, 10152
    /* 800AEF6C 0C001D78 */  jal 0x800075E0
    /* 800AEF70 00003025 */  or $a2, $zero, $zero
    /* 800AEF74 3C048014 */  lui $a0, 0x8014
    /* 800AEF78 24842728 */  addiu $a0, $a0, 10024
    /* 800AEF7C 00002825 */  or $a1, $zero, $zero
    /* 800AEF80 0C001C9C */  jal 0x80007270
    /* 800AEF84 24060001 */  addiu $a2, $zero, 1
    /* 800AEF88 8FA50030 */  lw $a1, 48($sp)
    /* 800AEF8C 3C048014 */  lui $a0, 0x8014
    /* 800AEF90 248461E8 */  addiu $a0, $a0, 25064
    /* 800AEF94 80AE0008 */  lb $t6, 8($a1)
    /* 800AEF98 15C00006 */  bne $t6, $zero, 0x800AEFB4
    /* 800AEF9C 00000000 */  nop
    /* 800AEFA0 0C0247EF */  jal 0x80091FBC
    /* 800AEFA4 8C860008 */  lw $a2, 8($a0)
    /* 800AEFA8 8FB80030 */  lw $t8, 48($sp)
    /* 800AEFAC 240F0001 */  addiu $t7, $zero, 1
    /* 800AEFB0 A30F0009 */  sb $t7, 9($t8)
    /* 800AEFB4 3C048014 */  lui $a0, 0x8014
    /* 800AEFB8 24842728 */  addiu $a0, $a0, 10024
    /* 800AEFBC 00002825 */  or $a1, $zero, $zero
    /* 800AEFC0 0C001D78 */  jal 0x800075E0
    /* 800AEFC4 00003025 */  or $a2, $zero, $zero
    /* 800AEFC8 8FA20020 */  lw $v0, 32($sp)
    /* 800AEFCC 8FBF001C */  lw $ra, 28($sp)
    /* 800AEFD0 8FB00014 */  lw $s0, 20($sp)
    /* 800AEFD4 8FB10018 */  lw $s1, 24($sp)
    /* 800AEFD8 03E00008 */  jr $ra
    /* 800AEFDC 27BD0038 */  addiu $sp, $sp, 56
