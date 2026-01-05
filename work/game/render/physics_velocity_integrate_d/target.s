# Source: game_code.bin (decompressed)
# Address: 0x8008BB8C

glabel physics_velocity_integrate_d
    /* 8008BB8C 27BDFFB0 */  addiu $sp, $sp, -80
    /* 8008BB90 AFBF004C */  sw $ra, 76($sp)
    /* 8008BB94 AFB20048 */  sw $s2, 72($sp)
    /* 8008BB98 AFB10044 */  sw $s1, 68($sp)
    /* 8008BB9C AFB00040 */  sw $s0, 64($sp)
    /* 8008BBA0 F7B80038 */  .word 0xF7B80038
    /* 8008BBA4 F7B60030 */  .word 0xF7B60030
    /* 8008BBA8 F7B40028 */  .word 0xF7B40028
    /* 8008BBAC AFA50054 */  sw $a1, 84($sp)
    /* 8008BBB0 84820008 */  lh $v0, 8($a0)
    /* 8008BBB4 00055C00 */  sll $t3, $a1, 16
    /* 8008BBB8 000B7403 */  sra $t6, $t3, 16
    /* 8008BBBC 01C05825 */  or $t3, $t6, $zero
    /* 8008BBC0 0002CA00 */  sll $t9, $v0, 8
    /* 8008BBC4 3C0E8015 */  lui $t6, 0x8015
    /* 8008BBC8 0322C821 */  addu $t9, $t9, $v0
    /* 8008BBCC 0019C8C0 */  sll $t9, $t9, 3
    /* 8008BBD0 25CEA250 */  addiu $t6, $t6, -23984
    /* 8008BBD4 032E4021 */  addu $t0, $t9, $t6
    /* 8008BBD8 C50403F0 */  lwc1 $f4, 1008($t0)
    /* 8008BBDC 00027900 */  sll $t7, $v0, 4
    /* 8008BBE0 01E27823 */  subu $t7, $t7, $v0
    /* 8008BBE4 4600218D */  .word 0x4600218D
    /* 8008BBE8 000F78C0 */  sll $t7, $t7, 3
    /* 8008BBEC 3C188015 */  lui $t8, 0x8015
    /* 8008BBF0 01E27823 */  subu $t7, $t7, $v0
    /* 8008BBF4 44073000 */  .word 0x44073000
    /* 8008BBF8 000F78C0 */  sll $t7, $t7, 3
    /* 8008BBFC 27182818 */  addiu $t8, $t8, 10264
    /* 8008BC00 01F83021 */  addu $a2, $t7, $t8
    /* 8008BC04 0007C400 */  sll $t8, $a3, 16
    /* 8008BC08 0018CC03 */  sra $t9, $t8, 16
    /* 8008BC0C 2B230015 */  slti $v1, $t9, 21
    /* 8008BC10 38690001 */  xori $t1, $v1, 0x0001
    /* 8008BC14 11200004 */  beq $t1, $zero, 0x8008BC28
    /* 8008BC18 00805025 */  or $t2, $a0, $zero
    /* 8008BC1C 8CC900E8 */  lw $t1, 232($a2)
    /* 8008BC20 312E0080 */  andi $t6, $t1, 0x0080
    /* 8008BC24 000E482B */  sltu $t1, $zero, $t6
    /* 8008BC28 3C018012 */  lui $at, 0x8012
    /* 8008BC2C C42A3894 */  lwc1 $f10, 14484($at)
    /* 8008BC30 C4C800B0 */  lwc1 $f8, 176($a2)
    /* 8008BC34 3C014000 */  lui $at, 0x4000
    /* 8008BC38 44813000 */  .word 0x44813000
    /* 8008BC3C C5040430 */  lwc1 $f4, 1072($t0)
    /* 8008BC40 460A4500 */  .word 0x460A4500
    /* 8008BC44 C4CA00B4 */  lwc1 $f10, 180($a2)
    /* 8008BC48 46062202 */  .word 0x46062202
    /* 8008BC4C 000B2400 */  sll $a0, $t3, 16
    /* 8008BC50 0004C403 */  sra $t8, $a0, 16
    /* 8008BC54 C4D800B8 */  lwc1 $f24, 184($a2)
    /* 8008BC58 03002025 */  or $a0, $t8, $zero
    /* 8008BC5C 01408825 */  or $s1, $t2, $zero
    /* 8008BC60 01202825 */  or $a1, $t1, $zero
    /* 8008BC64 241200C2 */  addiu $s2, $zero, 194
    /* 8008BC68 0C022DA7 */  jal 0x8008B69C
    /* 8008BC6C 460A4580 */  .word 0x460A4580
    /* 8008BC70 8FBF004C */  lw $ra, 76($sp)
    /* 8008BC74 D7B40028 */  .word 0xD7B40028
    /* 8008BC78 D7B60030 */  .word 0xD7B60030
    /* 8008BC7C D7B80038 */  .word 0xD7B80038
    /* 8008BC80 8FB00040 */  lw $s0, 64($sp)
    /* 8008BC84 8FB10044 */  lw $s1, 68($sp)
    /* 8008BC88 8FB20048 */  lw $s2, 72($sp)
    /* 8008BC8C 03E00008 */  jr $ra
    /* 8008BC90 27BD0050 */  addiu $sp, $sp, 80
