# Source: game_code.bin (decompressed)
# Address: 0x8008BC94

glabel physics_velocity_integrate_e
    /* 8008BC94 27BDFFB0 */  addiu $sp, $sp, -80
    /* 8008BC98 AFBF004C */  sw $ra, 76($sp)
    /* 8008BC9C AFB20048 */  sw $s2, 72($sp)
    /* 8008BCA0 AFB10044 */  sw $s1, 68($sp)
    /* 8008BCA4 AFB00040 */  sw $s0, 64($sp)
    /* 8008BCA8 F7B80038 */  .word 0xF7B80038
    /* 8008BCAC F7B60030 */  .word 0xF7B60030
    /* 8008BCB0 F7B40028 */  .word 0xF7B40028
    /* 8008BCB4 AFA50054 */  sw $a1, 84($sp)
    /* 8008BCB8 84820008 */  lh $v0, 8($a0)
    /* 8008BCBC 00055C00 */  sll $t3, $a1, 16
    /* 8008BCC0 000B7403 */  sra $t6, $t3, 16
    /* 8008BCC4 01C05825 */  or $t3, $t6, $zero
    /* 8008BCC8 0002CA00 */  sll $t9, $v0, 8
    /* 8008BCCC 3C0E8015 */  lui $t6, 0x8015
    /* 8008BCD0 0322C821 */  addu $t9, $t9, $v0
    /* 8008BCD4 0019C8C0 */  sll $t9, $t9, 3
    /* 8008BCD8 25CEA250 */  addiu $t6, $t6, -23984
    /* 8008BCDC 032E4021 */  addu $t0, $t9, $t6
    /* 8008BCE0 C50403F0 */  lwc1 $f4, 1008($t0)
    /* 8008BCE4 00027900 */  sll $t7, $v0, 4
    /* 8008BCE8 01E27823 */  subu $t7, $t7, $v0
    /* 8008BCEC 4600218D */  .word 0x4600218D
    /* 8008BCF0 000F78C0 */  sll $t7, $t7, 3
    /* 8008BCF4 3C188015 */  lui $t8, 0x8015
    /* 8008BCF8 01E27823 */  subu $t7, $t7, $v0
    /* 8008BCFC 44073000 */  .word 0x44073000
    /* 8008BD00 000F78C0 */  sll $t7, $t7, 3
    /* 8008BD04 27182818 */  addiu $t8, $t8, 10264
    /* 8008BD08 01F83021 */  addu $a2, $t7, $t8
    /* 8008BD0C 0007C400 */  sll $t8, $a3, 16
    /* 8008BD10 0018CC03 */  sra $t9, $t8, 16
    /* 8008BD14 2B230015 */  slti $v1, $t9, 21
    /* 8008BD18 38690001 */  xori $t1, $v1, 0x0001
    /* 8008BD1C 11200004 */  beq $t1, $zero, 0x8008BD30
    /* 8008BD20 00805025 */  or $t2, $a0, $zero
    /* 8008BD24 8CC900E8 */  lw $t1, 232($a2)
    /* 8008BD28 312E2000 */  andi $t6, $t1, 0x2000
    /* 8008BD2C 000E482B */  sltu $t1, $zero, $t6
    /* 8008BD30 3C018012 */  lui $at, 0x8012
    /* 8008BD34 C42A3898 */  lwc1 $f10, 14488($at)
    /* 8008BD38 C4C800D4 */  lwc1 $f8, 212($a2)
    /* 8008BD3C 3C014000 */  lui $at, 0x4000
    /* 8008BD40 44813000 */  .word 0x44813000
    /* 8008BD44 C5040544 */  lwc1 $f4, 1348($t0)
    /* 8008BD48 460A4501 */  .word 0x460A4501
    /* 8008BD4C C4CA00D8 */  lwc1 $f10, 216($a2)
    /* 8008BD50 46062202 */  .word 0x46062202
    /* 8008BD54 000B2400 */  sll $a0, $t3, 16
    /* 8008BD58 0004C403 */  sra $t8, $a0, 16
    /* 8008BD5C C4D800DC */  lwc1 $f24, 220($a2)
    /* 8008BD60 03002025 */  or $a0, $t8, $zero
    /* 8008BD64 01408825 */  or $s1, $t2, $zero
    /* 8008BD68 01202825 */  or $a1, $t1, $zero
    /* 8008BD6C 241200C2 */  addiu $s2, $zero, 194
    /* 8008BD70 0C022DA7 */  jal 0x8008B69C
    /* 8008BD74 460A4580 */  .word 0x460A4580
    /* 8008BD78 8FBF004C */  lw $ra, 76($sp)
    /* 8008BD7C D7B40028 */  .word 0xD7B40028
    /* 8008BD80 D7B60030 */  .word 0xD7B60030
    /* 8008BD84 D7B80038 */  .word 0xD7B80038
    /* 8008BD88 8FB00040 */  lw $s0, 64($sp)
    /* 8008BD8C 8FB10044 */  lw $s1, 68($sp)
    /* 8008BD90 8FB20048 */  lw $s2, 72($sp)
    /* 8008BD94 03E00008 */  jr $ra
    /* 8008BD98 27BD0050 */  addiu $sp, $sp, 80
