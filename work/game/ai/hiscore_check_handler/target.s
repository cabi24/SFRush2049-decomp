# Source: game_code.bin (decompressed)
# Address: 0x800DBFC0

glabel hiscore_check_handler
    /* 800DBFC0 11030005 */  beq $t0, $v1, 0x800DBFD8
    /* 800DBFC4 24010003 */  addiu $at, $zero, 3
    /* 800DBFC8 10610003 */  beq $v1, $at, 0x800DBFD8
    /* 800DBFCC 00000000 */  nop
    /* 800DBFD0 0C02D0E3 */  jal 0x800B438C
    /* 800DBFD4 00000000 */  nop
    /* 800DBFD8 3C0F8015 */  lui $t7, 0x8015
    /* 800DBFDC 81EF7244 */  lb $t7, 29252($t7)
    /* 800DBFE0 11E0001C */  beq $t7, $zero, 0x800DC054
    /* 800DBFE4 3C018011 */  lui $at, 0x8011
    /* 800DBFE8 A0204650 */  sb $zero, 18000($at)
    /* 800DBFEC 0C02D3EC */  jal 0x800B4FB0
    /* 800DBFF0 00002025 */  or $a0, $zero, $zero
    /* 800DBFF4 0C0004F7 */  jal 0x800013DC
    /* 800DBFF8 00000000 */  nop
    /* 800DBFFC 3C048014 */  lui $a0, 0x8014
    /* 800DC000 24842728 */  addiu $a0, $a0, 10024
    /* 800DC004 00002825 */  or $a1, $zero, $zero
    /* 800DC008 0C001C9C */  jal 0x80007270
    /* 800DC00C 24060001 */  addiu $a2, $zero, 1
    /* 800DC010 0C0246C0 */  jal 0x80091B00
    /* 800DC014 00000000 */  nop
    /* 800DC018 AFA20044 */  sw $v0, 68($sp)
    /* 800DC01C 24180007 */  addiu $t8, $zero, 7
    /* 800DC020 3C048014 */  lui $a0, 0x8014
    /* 800DC024 24842728 */  addiu $a0, $a0, 10024
    /* 800DC028 A0580002 */  sb $t8, 2($v0)
    /* 800DC02C 00002825 */  or $a1, $zero, $zero
    /* 800DC030 0C001D78 */  jal 0x800075E0
    /* 800DC034 00003025 */  or $a2, $zero, $zero
    /* 800DC038 3C048014 */  lui $a0, 0x8014
    /* 800DC03C 248427A8 */  addiu $a0, $a0, 10152
    /* 800DC040 8FA50044 */  lw $a1, 68($sp)
    /* 800DC044 0C001D78 */  jal 0x800075E0
    /* 800DC048 00003025 */  or $a2, $zero, $zero
    /* 800DC04C 0C035681 */  jal 0x800D5A04
    /* 800DC050 00000000 */  nop
    /* 800DC054 8FBF003C */  lw $ra, 60($sp)
    /* 800DC058 D7B40010 */  .word 0xD7B40010
    /* 800DC05C D7B60018 */  .word 0xD7B60018
    /* 800DC060 8FB00024 */  lw $s0, 36($sp)
    /* 800DC064 8FB10028 */  lw $s1, 40($sp)
    /* 800DC068 8FB2002C */  lw $s2, 44($sp)
    /* 800DC06C 8FB30030 */  lw $s3, 48($sp)
    /* 800DC070 8FB40034 */  lw $s4, 52($sp)
    /* 800DC074 8FB50038 */  lw $s5, 56($sp)
    /* 800DC078 03E00008 */  jr $ra
    /* 800DC07C 27BD0110 */  addiu $sp, $sp, 272
