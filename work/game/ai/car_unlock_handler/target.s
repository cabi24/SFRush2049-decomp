# Source: game_code.bin (decompressed)
# Address: 0x800D197C

glabel car_unlock_handler
    /* 800D197C 27BDFFD8 */  addiu $sp, $sp, -40
    /* 800D1980 AFA40028 */  sw $a0, 40($sp)
    /* 800D1984 AFBF0014 */  sw $ra, 20($sp)
    /* 800D1988 AFA5002C */  sw $a1, 44($sp)
    /* 800D198C AFA60030 */  sw $a2, 48($sp)
    /* 800D1990 3C048014 */  lui $a0, 0x8014
    /* 800D1994 AFA70034 */  sw $a3, 52($sp)
    /* 800D1998 24842728 */  addiu $a0, $a0, 10024
    /* 800D199C 24060001 */  addiu $a2, $zero, 1
    /* 800D19A0 0C001C9C */  jal 0x80007270
    /* 800D19A4 00002825 */  or $a1, $zero, $zero
    /* 800D19A8 0C034636 */  jal 0x800D18D8
    /* 800D19AC 00000000 */  nop
    /* 800D19B0 8FAE0028 */  lw $t6, 40($sp)
    /* 800D19B4 3C018015 */  lui $at, 0x8015
    /* 800D19B8 00404025 */  or $t0, $v0, $zero
    /* 800D19BC A04E0034 */  sb $t6, 52($v0)
    /* 800D19C0 C4202748 */  lwc1 $f0, 10056($at)
    /* 800D19C4 27A5003C */  addiu $a1, $sp, 60
    /* 800D19C8 00002025 */  or $a0, $zero, $zero
    /* 800D19CC E4400010 */  swc1 $f0, 16($v0)
    /* 800D19D0 C7A4002C */  lwc1 $f4, 44($sp)
    /* 800D19D4 00401825 */  or $v1, $v0, $zero
    /* 800D19D8 2406FFFC */  addiu $a2, $zero, -4
    /* 800D19DC E4440014 */  swc1 $f4, 20($v0)
    /* 800D19E0 8FAF0030 */  lw $t7, 48($sp)
    /* 800D19E4 2407FFFF */  addiu $a3, $zero, -1
    /* 800D19E8 AC4F0030 */  sw $t7, 48($v0)
    /* 800D19EC 8FB80034 */  lw $t8, 52($sp)
    /* 800D19F0 A040000E */  sb $zero, 14($v0)
    /* 800D19F4 A0400018 */  sb $zero, 24($v0)
    /* 800D19F8 AC58002C */  sw $t8, 44($v0)
    /* 800D19FC 8FB90038 */  lw $t9, 56($sp)
    /* 800D1A00 1B20000B */  blez $t9, 0x800D1A30
    /* 800D1A04 24A90003 */  addiu $t1, $a1, 3
    /* 800D1A08 01262824 */  and $a1, $t1, $a2
    /* 800D1A0C 8CAA0000 */  lw $t2, 0($a1)
    /* 800D1A10 24840001 */  addiu $a0, $a0, 1
    /* 800D1A14 24630004 */  addiu $v1, $v1, 4
    /* 800D1A18 AC6A0018 */  sw $t2, 24($v1)
    /* 800D1A1C 8FAB0038 */  lw $t3, 56($sp)
    /* 800D1A20 24A50004 */  addiu $a1, $a1, 4
    /* 800D1A24 008B082A */  slt $at, $a0, $t3
    /* 800D1A28 5420FFF7 */  .word 0x5420FFF7
    /* 800D1A2C 24A90003 */  addiu $t1, $a1, 3
    /* 800D1A30 28810004 */  slti $at, $a0, 4
    /* 800D1A34 10200008 */  beq $at, $zero, 0x800D1A58
    /* 800D1A38 01002825 */  or $a1, $t0, $zero
    /* 800D1A3C 00046080 */  sll $t4, $a0, 2
    /* 800D1A40 004C1821 */  addu $v1, $v0, $t4
    /* 800D1A44 24840001 */  addiu $a0, $a0, 1
    /* 800D1A48 28810004 */  slti $at, $a0, 4
    /* 800D1A4C 24630004 */  addiu $v1, $v1, 4
    /* 800D1A50 1420FFFC */  bne $at, $zero, 0x800D1A44
    /* 800D1A54 AC670018 */  sw $a3, 24($v1)
    /* 800D1A58 3C048015 */  lui $a0, 0x8015
    /* 800D1A5C 2407FFFF */  addiu $a3, $zero, -1
    /* 800D1A60 24849860 */  addiu $a0, $a0, -26528
    /* 800D1A64 AD070038 */  sw $a3, 56($t0)
    /* 800D1A68 AFA80024 */  sw $t0, 36($sp)
    /* 800D1A6C 0C0247EF */  jal 0x80091FBC
    /* 800D1A70 8C860008 */  lw $a2, 8($a0)
    /* 800D1A74 8FA80024 */  lw $t0, 36($sp)
    /* 800D1A78 240D0001 */  addiu $t5, $zero, 1
    /* 800D1A7C 3C048014 */  lui $a0, 0x8014
    /* 800D1A80 8D0E0008 */  lw $t6, 8($t0)
    /* 800D1A84 A10D000D */  sb $t5, 13($t0)
    /* 800D1A88 24842728 */  addiu $a0, $a0, 10024
    /* 800D1A8C 00002825 */  or $a1, $zero, $zero
    /* 800D1A90 00003025 */  or $a2, $zero, $zero
    /* 800D1A94 0C001D78 */  jal 0x800075E0
    /* 800D1A98 AFAE0018 */  sw $t6, 24($sp)
    /* 800D1A9C 8FBF0014 */  lw $ra, 20($sp)
    /* 800D1AA0 8FA20018 */  lw $v0, 24($sp)
    /* 800D1AA4 27BD0028 */  addiu $sp, $sp, 40
    /* 800D1AA8 03E00008 */  jr $ra
    /* 800D1AAC 00000000 */  nop
