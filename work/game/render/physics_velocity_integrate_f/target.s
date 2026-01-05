# Source: game_code.bin (decompressed)
# Address: 0x8008BD9C

glabel physics_velocity_integrate_f
    /* 8008BD9C 27BDFFB0 */  addiu $sp, $sp, -80
    /* 8008BDA0 AFBF004C */  sw $ra, 76($sp)
    /* 8008BDA4 AFB20048 */  sw $s2, 72($sp)
    /* 8008BDA8 AFB10044 */  sw $s1, 68($sp)
    /* 8008BDAC AFB00040 */  sw $s0, 64($sp)
    /* 8008BDB0 F7B80038 */  .word 0xF7B80038
    /* 8008BDB4 F7B60030 */  .word 0xF7B60030
    /* 8008BDB8 F7B40028 */  .word 0xF7B40028
    /* 8008BDBC AFA50054 */  sw $a1, 84($sp)
    /* 8008BDC0 84820008 */  lh $v0, 8($a0)
    /* 8008BDC4 00055C00 */  sll $t3, $a1, 16
    /* 8008BDC8 000B7403 */  sra $t6, $t3, 16
    /* 8008BDCC 01C05825 */  or $t3, $t6, $zero
    /* 8008BDD0 0002CA00 */  sll $t9, $v0, 8
    /* 8008BDD4 3C0E8015 */  lui $t6, 0x8015
    /* 8008BDD8 0322C821 */  addu $t9, $t9, $v0
    /* 8008BDDC 0019C8C0 */  sll $t9, $t9, 3
    /* 8008BDE0 25CEA250 */  addiu $t6, $t6, -23984
    /* 8008BDE4 032E4021 */  addu $t0, $t9, $t6
    /* 8008BDE8 C50403F0 */  lwc1 $f4, 1008($t0)
    /* 8008BDEC 00027900 */  sll $t7, $v0, 4
    /* 8008BDF0 01E27823 */  subu $t7, $t7, $v0
    /* 8008BDF4 4600218D */  .word 0x4600218D
    /* 8008BDF8 000F78C0 */  sll $t7, $t7, 3
    /* 8008BDFC 3C188015 */  lui $t8, 0x8015
    /* 8008BE00 01E27823 */  subu $t7, $t7, $v0
    /* 8008BE04 44073000 */  .word 0x44073000
    /* 8008BE08 000F78C0 */  sll $t7, $t7, 3
    /* 8008BE0C 27182818 */  addiu $t8, $t8, 10264
    /* 8008BE10 01F83021 */  addu $a2, $t7, $t8
    /* 8008BE14 0007C400 */  sll $t8, $a3, 16
    /* 8008BE18 0018CC03 */  sra $t9, $t8, 16
    /* 8008BE1C 2B230015 */  slti $v1, $t9, 21
    /* 8008BE20 38690001 */  xori $t1, $v1, 0x0001
    /* 8008BE24 11200004 */  beq $t1, $zero, 0x8008BE38
    /* 8008BE28 00805025 */  or $t2, $a0, $zero
    /* 8008BE2C 8CC900E8 */  lw $t1, 232($a2)
    /* 8008BE30 312E0100 */  andi $t6, $t1, 0x0100
    /* 8008BE34 000E482B */  sltu $t1, $zero, $t6
    /* 8008BE38 3C018012 */  lui $at, 0x8012
    /* 8008BE3C C42A389C */  lwc1 $f10, 14492($at)
    /* 8008BE40 C4C800BC */  lwc1 $f8, 188($a2)
    /* 8008BE44 3C014000 */  lui $at, 0x4000
    /* 8008BE48 44813000 */  .word 0x44813000
    /* 8008BE4C C504048C */  lwc1 $f4, 1164($t0)
    /* 8008BE50 460A4501 */  .word 0x460A4501
    /* 8008BE54 C4CA00C0 */  lwc1 $f10, 192($a2)
    /* 8008BE58 46062202 */  .word 0x46062202
    /* 8008BE5C 000B2400 */  sll $a0, $t3, 16
    /* 8008BE60 0004C403 */  sra $t8, $a0, 16
    /* 8008BE64 C4D800C4 */  lwc1 $f24, 196($a2)
    /* 8008BE68 03002025 */  or $a0, $t8, $zero
    /* 8008BE6C 01408825 */  or $s1, $t2, $zero
    /* 8008BE70 01202825 */  or $a1, $t1, $zero
    /* 8008BE74 241200C2 */  addiu $s2, $zero, 194
    /* 8008BE78 0C022DA7 */  jal 0x8008B69C
    /* 8008BE7C 460A4580 */  .word 0x460A4580
    /* 8008BE80 8FBF004C */  lw $ra, 76($sp)
    /* 8008BE84 D7B40028 */  .word 0xD7B40028
    /* 8008BE88 D7B60030 */  .word 0xD7B60030
    /* 8008BE8C D7B80038 */  .word 0xD7B80038
    /* 8008BE90 8FB00040 */  lw $s0, 64($sp)
    /* 8008BE94 8FB10044 */  lw $s1, 68($sp)
    /* 8008BE98 8FB20048 */  lw $s2, 72($sp)
    /* 8008BE9C 03E00008 */  jr $ra
    /* 8008BEA0 27BD0050 */  addiu $sp, $sp, 80
