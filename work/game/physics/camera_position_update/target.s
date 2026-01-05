# Source: game_code.bin (decompressed)
# Address: 0x800AEB54

glabel camera_position_update
    /* 800AEB54 27BDFF50 */  addiu $sp, $sp, -176
    /* 800AEB58 3C028016 */  lui $v0, 0x8016
    /* 800AEB5C 3C038016 */  lui $v1, 0x8016
    /* 800AEB60 AFBF0054 */  sw $ra, 84($sp)
    /* 800AEB64 AFBE0050 */  sw $fp, 80($sp)
    /* 800AEB68 AFB7004C */  sw $s7, 76($sp)
    /* 800AEB6C AFB60048 */  sw $s6, 72($sp)
    /* 800AEB70 AFB50044 */  sw $s5, 68($sp)
    /* 800AEB74 AFB40040 */  sw $s4, 64($sp)
    /* 800AEB78 AFB3003C */  sw $s3, 60($sp)
    /* 800AEB7C AFB20038 */  sw $s2, 56($sp)
    /* 800AEB80 AFB10034 */  sw $s1, 52($sp)
    /* 800AEB84 AFB00030 */  sw $s0, 48($sp)
    /* 800AEB88 F7B60028 */  .word 0xF7B60028
    /* 800AEB8C F7B40020 */  .word 0xF7B40020
    /* 800AEB90 246313D0 */  addiu $v1, $v1, 5072
    /* 800AEB94 244213C0 */  addiu $v0, $v0, 5056
    /* 800AEB98 24420004 */  addiu $v0, $v0, 4
    /* 800AEB9C 0043082B */  sltu $at, $v0, $v1
    /* 800AEBA0 A040FFFD */  sb $zero, -3($v0)
    /* 800AEBA4 A040FFFE */  sb $zero, -2($v0)
    /* 800AEBA8 A040FFFF */  sb $zero, -1($v0)
    /* 800AEBAC 1420FFFA */  bne $at, $zero, 0x800AEB98
    /* 800AEBB0 A040FFFC */  sb $zero, -4($v0)
    /* 800AEBB4 3C0E8016 */  lui $t6, 0x8016
    /* 800AEBB8 81CE13A8 */  lb $t6, 5032($t6)
    /* 800AEBBC 3C0F8015 */  lui $t7, 0x8015
    /* 800AEBC0 51C00048 */  .word 0x51C00048
    /* 800AEBC4 8FBF0054 */  lw $ra, 84($sp)
    /* 800AEBC8 8DEFA110 */  lw $t7, -24304($t7)
    /* 800AEBCC 24010006 */  addiu $at, $zero, 6
    /* 800AEBD0 3C1E8015 */  lui $fp, 0x8015
    /* 800AEBD4 15E10042 */  bne $t7, $at, 0x800AECE0
    /* 800AEBD8 27DEA108 */  addiu $fp, $fp, -24312
    /* 800AEBDC 87C20000 */  lh $v0, 0($fp)
    /* 800AEBE0 3C088015 */  lui $t0, 0x8015
    /* 800AEBE4 25082818 */  addiu $t0, $t0, 10264
    /* 800AEBE8 2443FFFF */  addiu $v1, $v0, -1
    /* 800AEBEC 1860003C */  blez $v1, 0x800AECE0
    /* 800AEBF0 0000A025 */  or $s4, $zero, $zero
    /* 800AEBF4 3C013F00 */  lui $at, 0x3F00
    /* 800AEBF8 4481B000 */  .word 0x4481B000
    /* 800AEBFC 3C0140A0 */  lui $at, 0x40A0
    /* 800AEC00 3C128016 */  lui $s2, 0x8016
    /* 800AEC04 4481A000 */  .word 0x4481A000
    /* 800AEC08 265213C0 */  addiu $s2, $s2, 5056
    /* 800AEC0C 27B70068 */  addiu $s7, $sp, 104
    /* 800AEC10 27B6008C */  addiu $s6, $sp, 140
    /* 800AEC14 27B50098 */  addiu $s5, $sp, 152
    /* 800AEC18 24130001 */  addiu $s3, $zero, 1
    /* 800AEC1C C500000C */  lwc1 $f0, 12($t0)
    /* 800AEC20 C5040008 */  lwc1 $f4, 8($t0)
    /* 800AEC24 C5060010 */  lwc1 $f6, 16($t0)
    /* 800AEC28 46140000 */  .word 0x46140000
    /* 800AEC2C 28410002 */  slti $at, $v0, 2
    /* 800AEC30 02608825 */  or $s1, $s3, $zero
    /* 800AEC34 E7A40098 */  swc1 $f4, 152($sp)
    /* 800AEC38 E7A0009C */  swc1 $f0, 156($sp)
    /* 800AEC3C 14200024 */  bne $at, $zero, 0x800AECD0
    /* 800AEC40 E7A600A0 */  swc1 $f6, 160($sp)
    /* 800AEC44 3C108015 */  lui $s0, 0x8015
    /* 800AEC48 26102BD0 */  addiu $s0, $s0, 11216
    /* 800AEC4C AFA8005C */  sw $t0, 92($sp)
    /* 800AEC50 C600000C */  lwc1 $f0, 12($s0)
    /* 800AEC54 C6080008 */  lwc1 $f8, 8($s0)
    /* 800AEC58 C60A0010 */  lwc1 $f10, 16($s0)
    /* 800AEC5C 46140000 */  .word 0x46140000
    /* 800AEC60 4407B000 */  .word 0x4407B000
    /* 800AEC64 3418FFFF */  ori $t8, $zero, 0xFFFF
    /* 800AEC68 AFB80014 */  sw $t8, 20($sp)
    /* 800AEC6C E7A00090 */  swc1 $f0, 144($sp)
    /* 800AEC70 02A02025 */  or $a0, $s5, $zero
    /* 800AEC74 02C02825 */  or $a1, $s6, $zero
    /* 800AEC78 02E03025 */  or $a2, $s7, $zero
    /* 800AEC7C AFA00010 */  sw $zero, 16($sp)
    /* 800AEC80 E7A8008C */  swc1 $f8, 140($sp)
    /* 800AEC84 0C02B756 */  jal 0x800ADD58
    /* 800AEC88 E7AA0094 */  swc1 $f10, 148($sp)
    /* 800AEC8C 10400008 */  beq $v0, $zero, 0x800AECB0
    /* 800AEC90 0014C880 */  sll $t9, $s4, 2
    /* 800AEC94 02594821 */  addu $t1, $s2, $t9
    /* 800AEC98 00115880 */  sll $t3, $s1, 2
    /* 800AEC9C 01315021 */  addu $t2, $t1, $s1
    /* 800AECA0 024B6021 */  addu $t4, $s2, $t3
    /* 800AECA4 A1530000 */  sb $s3, 0($t2)
    /* 800AECA8 01946821 */  addu $t5, $t4, $s4
    /* 800AECAC A1B30000 */  sb $s3, 0($t5)
    /* 800AECB0 87C20000 */  lh $v0, 0($fp)
    /* 800AECB4 26310001 */  addiu $s1, $s1, 1
    /* 800AECB8 261003B8 */  addiu $s0, $s0, 952
    /* 800AECBC 0222082A */  slt $at, $s1, $v0
    /* 800AECC0 5420FFE4 */  .word 0x5420FFE4
    /* 800AECC4 C600000C */  lwc1 $f0, 12($s0)
    /* 800AECC8 2443FFFF */  addiu $v1, $v0, -1
    /* 800AECCC 8FA8005C */  lw $t0, 92($sp)
    /* 800AECD0 26940001 */  addiu $s4, $s4, 1
    /* 800AECD4 0283082A */  slt $at, $s4, $v1
    /* 800AECD8 1420FFD0 */  bne $at, $zero, 0x800AEC1C
    /* 800AECDC 250803B8 */  addiu $t0, $t0, 952
    /* 800AECE0 8FBF0054 */  lw $ra, 84($sp)
    /* 800AECE4 D7B40020 */  .word 0xD7B40020
    /* 800AECE8 D7B60028 */  .word 0xD7B60028
    /* 800AECEC 8FB00030 */  lw $s0, 48($sp)
    /* 800AECF0 8FB10034 */  lw $s1, 52($sp)
    /* 800AECF4 8FB20038 */  lw $s2, 56($sp)
    /* 800AECF8 8FB3003C */  lw $s3, 60($sp)
    /* 800AECFC 8FB40040 */  lw $s4, 64($sp)
    /* 800AED00 8FB50044 */  lw $s5, 68($sp)
    /* 800AED04 8FB60048 */  lw $s6, 72($sp)
    /* 800AED08 8FB7004C */  lw $s7, 76($sp)
    /* 800AED0C 8FBE0050 */  lw $fp, 80($sp)
    /* 800AED10 03E00008 */  jr $ra
    /* 800AED14 27BD00B0 */  addiu $sp, $sp, 176
