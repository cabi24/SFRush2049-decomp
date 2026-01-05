# Source: game_code.bin (decompressed)
# Address: 0x8008BA84

glabel physics_velocity_integrate_c
    /* 8008BA84 27BDFFB0 */  addiu $sp, $sp, -80
    /* 8008BA88 AFBF004C */  sw $ra, 76($sp)
    /* 8008BA8C AFB20048 */  sw $s2, 72($sp)
    /* 8008BA90 AFB10044 */  sw $s1, 68($sp)
    /* 8008BA94 AFB00040 */  sw $s0, 64($sp)
    /* 8008BA98 F7B80038 */  .word 0xF7B80038
    /* 8008BA9C F7B60030 */  .word 0xF7B60030
    /* 8008BAA0 F7B40028 */  .word 0xF7B40028
    /* 8008BAA4 AFA50054 */  sw $a1, 84($sp)
    /* 8008BAA8 84820008 */  lh $v0, 8($a0)
    /* 8008BAAC 00055C00 */  sll $t3, $a1, 16
    /* 8008BAB0 000B7403 */  sra $t6, $t3, 16
    /* 8008BAB4 01C05825 */  or $t3, $t6, $zero
    /* 8008BAB8 0002CA00 */  sll $t9, $v0, 8
    /* 8008BABC 3C0E8015 */  lui $t6, 0x8015
    /* 8008BAC0 0322C821 */  addu $t9, $t9, $v0
    /* 8008BAC4 0019C8C0 */  sll $t9, $t9, 3
    /* 8008BAC8 25CEA250 */  addiu $t6, $t6, -23984
    /* 8008BACC 032E4021 */  addu $t0, $t9, $t6
    /* 8008BAD0 C50403F0 */  lwc1 $f4, 1008($t0)
    /* 8008BAD4 00027900 */  sll $t7, $v0, 4
    /* 8008BAD8 01E27823 */  subu $t7, $t7, $v0
    /* 8008BADC 4600218D */  .word 0x4600218D
    /* 8008BAE0 000F78C0 */  sll $t7, $t7, 3
    /* 8008BAE4 3C188015 */  lui $t8, 0x8015
    /* 8008BAE8 01E27823 */  subu $t7, $t7, $v0
    /* 8008BAEC 44073000 */  .word 0x44073000
    /* 8008BAF0 000F78C0 */  sll $t7, $t7, 3
    /* 8008BAF4 27182818 */  addiu $t8, $t8, 10264
    /* 8008BAF8 01F83021 */  addu $a2, $t7, $t8
    /* 8008BAFC 0007C400 */  sll $t8, $a3, 16
    /* 8008BB00 0018CC03 */  sra $t9, $t8, 16
    /* 8008BB04 2B230015 */  slti $v1, $t9, 21
    /* 8008BB08 38690001 */  xori $t1, $v1, 0x0001
    /* 8008BB0C 11200004 */  beq $t1, $zero, 0x8008BB20
    /* 8008BB10 00805025 */  or $t2, $a0, $zero
    /* 8008BB14 8CC900E8 */  lw $t1, 232($a2)
    /* 8008BB18 312E4000 */  andi $t6, $t1, 0x4000
    /* 8008BB1C 000E482B */  sltu $t1, $zero, $t6
    /* 8008BB20 3C018012 */  lui $at, 0x8012
    /* 8008BB24 C42A3890 */  lwc1 $f10, 14480($at)
    /* 8008BB28 C4C800C8 */  lwc1 $f8, 200($a2)
    /* 8008BB2C 3C014000 */  lui $at, 0x4000
    /* 8008BB30 44813000 */  .word 0x44813000
    /* 8008BB34 C50404E8 */  lwc1 $f4, 1256($t0)
    /* 8008BB38 460A4500 */  .word 0x460A4500
    /* 8008BB3C C4CA00CC */  lwc1 $f10, 204($a2)
    /* 8008BB40 46062202 */  .word 0x46062202
    /* 8008BB44 000B2400 */  sll $a0, $t3, 16
    /* 8008BB48 0004C403 */  sra $t8, $a0, 16
    /* 8008BB4C C4D800D0 */  lwc1 $f24, 208($a2)
    /* 8008BB50 03002025 */  or $a0, $t8, $zero
    /* 8008BB54 01408825 */  or $s1, $t2, $zero
    /* 8008BB58 01202825 */  or $a1, $t1, $zero
    /* 8008BB5C 241200C2 */  addiu $s2, $zero, 194
    /* 8008BB60 0C022DA7 */  jal 0x8008B69C
    /* 8008BB64 460A4580 */  .word 0x460A4580
    /* 8008BB68 8FBF004C */  lw $ra, 76($sp)
    /* 8008BB6C D7B40028 */  .word 0xD7B40028
    /* 8008BB70 D7B60030 */  .word 0xD7B60030
    /* 8008BB74 D7B80038 */  .word 0xD7B80038
    /* 8008BB78 8FB00040 */  lw $s0, 64($sp)
    /* 8008BB7C 8FB10044 */  lw $s1, 68($sp)
    /* 8008BB80 8FB20048 */  lw $s2, 72($sp)
    /* 8008BB84 03E00008 */  jr $ra
    /* 8008BB88 27BD0050 */  addiu $sp, $sp, 80
