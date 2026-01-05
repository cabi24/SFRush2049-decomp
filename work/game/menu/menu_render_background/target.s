# Source: game_code.bin (decompressed)
# Address: 0x800E0A00

glabel menu_render_background
    /* 800E0A00 C4284390 */  lwc1 $f8, 17296($at)
    /* 800E0A04 46045181 */  .word 0x46045181
    /* 800E0A08 00057880 */  sll $t7, $a1, 2
    /* 800E0A0C 01E57823 */  subu $t7, $t7, $a1
    /* 800E0A10 000F7880 */  sll $t7, $t7, 2
    /* 800E0A14 4606403C */  .word 0x4606403C
    /* 800E0A18 00000000 */  nop
    /* 800E0A1C 45020014 */  .word 0x45020014
    /* 800E0A20 8662065C */  lh $v0, 1628($s3)
    /* 800E0A24 C52A0000 */  lwc1 $f10, 0($t1)
    /* 800E0A28 01E57821 */  addu $t7, $t7, $a1
    /* 800E0A2C 3C048011 */  lui $a0, 0x8011
    /* 800E0A30 E46A0000 */  swc1 $f10, 0($v1)
    /* 800E0A34 92780008 */  lbu $t8, 8($s3)
    /* 800E0A38 AFA80094 */  sw $t0, 148($sp)
    /* 800E0A3C 24060001 */  addiu $a2, $zero, 1
    /* 800E0A40 01F8C821 */  addu $t9, $t7, $t8
    /* 800E0A44 00992021 */  addu $a0, $a0, $t9
    /* 800E0A48 808415CD */  lb $a0, 5581($a0)
    /* 800E0A4C 24070004 */  addiu $a3, $zero, 4
    /* 800E0A50 0C0248D8 */  jal 0x80092360
    /* 800E0A54 2484001A */  addiu $a0, $a0, 26
    /* 800E0A58 8FA80094 */  lw $t0, 148($sp)
    /* 800E0A5C 3C018014 */  lui $at, 0x8014
    /* 800E0A60 8FA500DC */  lw $a1, 220($sp)
    /* 800E0A64 00280821 */  addu $at, $at, $t0
    /* 800E0A68 AC2207E0 */  sw $v0, 2016($at)
    /* 800E0A6C 8662065C */  lh $v0, 1628($s3)
    /* 800E0A70 24010001 */  addiu $at, $zero, 1
    /* 800E0A74 24040019 */  addiu $a0, $zero, 25
    /* 800E0A78 1441000B */  bne $v0, $at, 0x800E0AA8
    /* 800E0A7C 24060002 */  addiu $a2, $zero, 2
    /* 800E0A80 24070001 */  addiu $a3, $zero, 1
    /* 800E0A84 0C0248D8 */  jal 0x80092360
    /* 800E0A88 AFA80094 */  sw $t0, 148($sp)
    /* 800E0A8C 8FA80094 */  lw $t0, 148($sp)
    /* 800E0A90 3C018014 */  lui $at, 0x8014
    /* 800E0A94 240E0002 */  addiu $t6, $zero, 2
    /* 800E0A98 00280821 */  addu $at, $at, $t0
    /* 800E0A9C AC2207C0 */  sw $v0, 1984($at)
    /* 800E0AA0 1000000D */  beq $zero, $zero, 0x800E0AD8
    /* 800E0AA4 A66E065C */  sh $t6, 1628($s3)
    /* 800E0AA8 24010003 */  addiu $at, $zero, 3
    /* 800E0AAC 1441000A */  bne $v0, $at, 0x800E0AD8
    /* 800E0AB0 3C0F8014 */  lui $t7, 0x8014
    /* 800E0AB4 25EF07C0 */  addiu $t7, $t7, 1984
    /* 800E0AB8 010F1021 */  addu $v0, $t0, $t7
    /* 800E0ABC 8C440000 */  lw $a0, 0($v0)
    /* 800E0AC0 0C024701 */  jal 0x80091C04
    /* 800E0AC4 AFA20090 */  sw $v0, 144($sp)
    /* 800E0AC8 8FA20090 */  lw $v0, 144($sp)
    /* 800E0ACC 2418FFFF */  addiu $t8, $zero, -1
    /* 800E0AD0 AC580000 */  sw $t8, 0($v0)
    /* 800E0AD4 A660065C */  sh $zero, 1628($s3)
    /* 800E0AD8 8FBF0084 */  lw $ra, 132($sp)
    /* 800E0ADC D7B40030 */  .word 0xD7B40030
    /* 800E0AE0 D7B60038 */  .word 0xD7B60038
    /* 800E0AE4 D7B80040 */  .word 0xD7B80040
    /* 800E0AE8 D7BA0048 */  .word 0xD7BA0048
    /* 800E0AEC D7BC0050 */  .word 0xD7BC0050
    /* 800E0AF0 D7BE0058 */  .word 0xD7BE0058
    /* 800E0AF4 8FB00060 */  lw $s0, 96($sp)
    /* 800E0AF8 8FB10064 */  lw $s1, 100($sp)
    /* 800E0AFC 8FB20068 */  lw $s2, 104($sp)
    /* 800E0B00 8FB3006C */  lw $s3, 108($sp)
    /* 800E0B04 8FB40070 */  lw $s4, 112($sp)
    /* 800E0B08 8FB50074 */  lw $s5, 116($sp)
    /* 800E0B0C 8FB60078 */  lw $s6, 120($sp)
    /* 800E0B10 8FB7007C */  lw $s7, 124($sp)
    /* 800E0B14 8FBE0080 */  lw $fp, 128($sp)
    /* 800E0B18 03E00008 */  jr $ra
    /* 800E0B1C 27BD00E0 */  addiu $sp, $sp, 224
