# Source: game_code.bin (decompressed)
# Address: 0x800DDFAC

glabel display_settings
    /* 800DDFAC 27BDFFD0 */  addiu $sp, $sp, -48
    /* 800DDFB0 2401000A */  addiu $at, $zero, 10
    /* 800DDFB4 AFBF002C */  sw $ra, 44($sp)
    /* 800DDFB8 AFB50028 */  sw $s5, 40($sp)
    /* 800DDFBC AFB40024 */  sw $s4, 36($sp)
    /* 800DDFC0 AFB30020 */  sw $s3, 32($sp)
    /* 800DDFC4 AFB2001C */  sw $s2, 28($sp)
    /* 800DDFC8 AFB10018 */  sw $s1, 24($sp)
    /* 800DDFCC AFB00014 */  sw $s0, 20($sp)
    /* 800DDFD0 AFA40030 */  sw $a0, 48($sp)
    /* 800DDFD4 AFA60038 */  sw $a2, 56($sp)
    /* 800DDFD8 14A10004 */  bne $a1, $at, 0x800DDFEC
    /* 800DDFDC AFA7003C */  sw $a3, 60($sp)
    /* 800DDFE0 24130001 */  addiu $s3, $zero, 1
    /* 800DDFE4 3C018011 */  lui $at, 0x8011
    /* 800DDFE8 A0336DB4 */  sb $s3, 28084($at)
    /* 800DDFEC 3C0E8011 */  lui $t6, 0x8011
    /* 800DDFF0 81CE6DB8 */  lb $t6, 28088($t6)
    /* 800DDFF4 3C198015 */  lui $t9, 0x8015
    /* 800DDFF8 24130001 */  addiu $s3, $zero, 1
    /* 800DDFFC 15C00012 */  bne $t6, $zero, 0x800DE048
    /* 800DE000 27393FD8 */  addiu $t9, $t9, 16344
    /* 800DE004 3C038015 */  lui $v1, 0x8015
    /* 800DE008 3C048015 */  lui $a0, 0x8015
    /* 800DE00C 24844138 */  addiu $a0, $a0, 16696
    /* 800DE010 24633FD8 */  addiu $v1, $v1, 16344
    /* 800DE014 24120002 */  addiu $s2, $zero, 2
    /* 800DE018 00008025 */  or $s0, $zero, $zero
    /* 800DE01C 00601025 */  or $v0, $v1, $zero
    /* 800DE020 26100001 */  addiu $s0, $s0, 1
    /* 800DE024 2442002C */  addiu $v0, $v0, 44
    /* 800DE028 AC40FFE8 */  sw $zero, -24($v0)
    /* 800DE02C 1612FFFC */  bne $s0, $s2, 0x800DE020
    /* 800DE030 A053FFD4 */  sb $s3, -44($v0)
    /* 800DE034 24630058 */  addiu $v1, $v1, 88
    /* 800DE038 5464FFF8 */  .word 0x5464FFF8
    /* 800DE03C 00008025 */  or $s0, $zero, $zero
    /* 800DE040 3C018011 */  lui $at, 0x8011
    /* 800DE044 A0336DB8 */  sb $s3, 28088($at)
    /* 800DE048 8FAF0030 */  lw $t7, 48($sp)
    /* 800DE04C 24120002 */  addiu $s2, $zero, 2
    /* 800DE050 00001025 */  or $v0, $zero, $zero
    /* 800DE054 000FC080 */  sll $t8, $t7, 2
    /* 800DE058 030FC023 */  subu $t8, $t8, $t7
    /* 800DE05C 0018C080 */  sll $t8, $t8, 2
    /* 800DE060 030FC023 */  subu $t8, $t8, $t7
    /* 800DE064 0018C0C0 */  sll $t8, $t8, 3
    /* 800DE068 03191821 */  addu $v1, $t8, $t9
    /* 800DE06C 0060A025 */  or $s4, $v1, $zero
    /* 800DE070 8E880014 */  lw $t0, 20($s4)
    /* 800DE074 51000005 */  .word 0x51000005
    /* 800DE078 3C014040 */  lui $at, 0x4040
    /* 800DE07C 24420001 */  addiu $v0, $v0, 1
    /* 800DE080 1452FFFB */  bne $v0, $s2, 0x800DE070
    /* 800DE084 2694002C */  addiu $s4, $s4, 44
    /* 800DE088 3C014040 */  lui $at, 0x4040
    /* 800DE08C 44812000 */  .word 0x44812000
    /* 800DE090 A2800000 */  sb $zero, 0($s4)
    /* 800DE094 3C018003 */  lui $at, 0x8003
    /* 800DE098 C426AFB4 */  lwc1 $f6, -20556($at)
    /* 800DE09C 3C0B8003 */  lui $t3, 0x8003
    /* 800DE0A0 256BE8E8 */  addiu $t3, $t3, -5912
    /* 800DE0A4 46062202 */  .word 0x46062202
    /* 800DE0A8 8D6C027C */  lw $t4, 636($t3)
    /* 800DE0AC 00027080 */  sll $t6, $v0, 2
    /* 800DE0B0 01C27023 */  subu $t6, $t6, $v0
    /* 800DE0B4 000E7080 */  sll $t6, $t6, 2
    /* 800DE0B8 01C27023 */  subu $t6, $t6, $v0
    /* 800DE0BC 000E7080 */  sll $t6, $t6, 2
    /* 800DE0C0 4600428D */  .word 0x4600428D
    /* 800DE0C4 A2930001 */  sb $s3, 1($s4)
    /* 800DE0C8 A2800002 */  sb $zero, 2($s4)
    /* 800DE0CC AE850014 */  sw $a1, 20($s4)
    /* 800DE0D0 440A5000 */  .word 0x440A5000
    /* 800DE0D4 006E8821 */  addu $s1, $v1, $t6
    /* 800DE0D8 AFA50034 */  sw $a1, 52($sp)
    /* 800DE0DC 014C6821 */  addu $t5, $t2, $t4
    /* 800DE0E0 AE8D0028 */  sw $t5, 40($s4)
    /* 800DE0E4 00008025 */  or $s0, $zero, $zero
    /* 800DE0E8 24150004 */  addiu $s5, $zero, 4
    /* 800DE0EC 0C0377CA */  jal 0x800DDF28
    /* 800DE0F0 02002025 */  or $a0, $s0, $zero
    /* 800DE0F4 A2220004 */  sb $v0, 4($s1)
    /* 800DE0F8 0C0377A9 */  jal 0x800DDEA4
    /* 800DE0FC 02002025 */  or $a0, $s0, $zero
    /* 800DE100 26100001 */  addiu $s0, $s0, 1
    /* 800DE104 26310001 */  addiu $s1, $s1, 1
    /* 800DE108 A2220007 */  sb $v0, 7($s1)
    /* 800DE10C 1615FFF7 */  bne $s0, $s5, 0x800DE0EC
    /* 800DE110 A220000B */  sb $zero, 11($s1)
    /* 800DE114 2404FFFF */  addiu $a0, $zero, -1
    /* 800DE118 0C032456 */  jal 0x800C9158
    /* 800DE11C 00002825 */  or $a1, $zero, $zero
    /* 800DE120 2404FFFF */  addiu $a0, $zero, -1
    /* 800DE124 0C032130 */  jal 0x800C84C0
    /* 800DE128 00002825 */  or $a1, $zero, $zero
    /* 800DE12C 8FAF0034 */  lw $t7, 52($sp)
    /* 800DE130 2404FFFF */  addiu $a0, $zero, -1
    /* 800DE134 55F30009 */  .word 0x55F30009
    /* 800DE138 8FB90034 */  lw $t9, 52($sp)
    /* 800DE13C 0C032456 */  jal 0x800C9158
    /* 800DE140 24050001 */  addiu $a1, $zero, 1
    /* 800DE144 2404FFFF */  addiu $a0, $zero, -1
    /* 800DE148 0C032130 */  jal 0x800C84C0
    /* 800DE14C 24050001 */  addiu $a1, $zero, 1
    /* 800DE150 10000014 */  beq $zero, $zero, 0x800DE1A4
    /* 800DE154 8FA80030 */  lw $t0, 48($sp)
    /* 800DE158 8FB90034 */  lw $t9, 52($sp)
    /* 800DE15C 8FB80030 */  lw $t8, 48($sp)
    /* 800DE160 24050001 */  addiu $a1, $zero, 1
    /* 800DE164 17320009 */  bne $t9, $s2, 0x800DE18C
    /* 800DE168 A2980003 */  sb $t8, 3($s4)
    /* 800DE16C 2404FFFF */  addiu $a0, $zero, -1
    /* 800DE170 0C032456 */  jal 0x800C9158
    /* 800DE174 24050001 */  addiu $a1, $zero, 1
    /* 800DE178 2404FFFF */  addiu $a0, $zero, -1
    /* 800DE17C 0C032130 */  jal 0x800C84C0
    /* 800DE180 24050001 */  addiu $a1, $zero, 1
    /* 800DE184 10000007 */  beq $zero, $zero, 0x800DE1A4
    /* 800DE188 8FA80030 */  lw $t0, 48($sp)
    /* 800DE18C 0C032456 */  jal 0x800C9158
    /* 800DE190 82840003 */  lb $a0, 3($s4)
    /* 800DE194 82840003 */  lb $a0, 3($s4)
    /* 800DE198 0C032130 */  jal 0x800C84C0
    /* 800DE19C 24050001 */  addiu $a1, $zero, 1
    /* 800DE1A0 8FA80030 */  lw $t0, 48($sp)
    /* 800DE1A4 3C028014 */  lui $v0, 0x8014
    /* 800DE1A8 8FAB0038 */  lw $t3, 56($sp)
    /* 800DE1AC 00084880 */  sll $t1, $t0, 2
    /* 800DE1B0 01284823 */  subu $t1, $t1, $t0
    /* 800DE1B4 00094980 */  sll $t1, $t1, 6
    /* 800DE1B8 01284821 */  addu $t1, $t1, $t0
    /* 800DE1BC 00094880 */  sll $t1, $t1, 2
    /* 800DE1C0 00491021 */  addu $v0, $v0, $t1
    /* 800DE1C4 80424036 */  lb $v0, 16438($v0)
    /* 800DE1C8 8FAA003C */  lw $t2, 60($sp)
    /* 800DE1CC 8FAC0040 */  lw $t4, 64($sp)
    /* 800DE1D0 8FAD0044 */  lw $t5, 68($sp)
    /* 800DE1D4 8FBF002C */  lw $ra, 44($sp)
    /* 800DE1D8 A68B0018 */  sh $t3, 24($s4)
    /* 800DE1DC A2820010 */  sb $v0, 16($s4)
    /* 800DE1E0 A68A001A */  sh $t2, 26($s4)
    /* 800DE1E4 A68C001C */  sh $t4, 28($s4)
    /* 800DE1E8 A68D001E */  sh $t5, 30($s4)
    /* 800DE1EC 8FB40024 */  lw $s4, 36($sp)
    /* 800DE1F0 8FB00014 */  lw $s0, 20($sp)
    /* 800DE1F4 8FB10018 */  lw $s1, 24($sp)
    /* 800DE1F8 8FB2001C */  lw $s2, 28($sp)
    /* 800DE1FC 8FB30020 */  lw $s3, 32($sp)
    /* 800DE200 8FB50028 */  lw $s5, 40($sp)
    /* 800DE204 03E00008 */  jr $ra
    /* 800DE208 27BD0030 */  addiu $sp, $sp, 48
