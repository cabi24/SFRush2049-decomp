# Source: game_code.bin (decompressed)
# Address: 0x800A5BB8

glabel tire_compound_set
    /* 800A5BB8 27BDFFC0 */  addiu $sp, $sp, -64
    /* 800A5BBC AFB00014 */  sw $s0, 20($sp)
    /* 800A5BC0 3C108003 */  lui $s0, 0x8003
    /* 800A5BC4 AFB10018 */  sw $s1, 24($sp)
    /* 800A5BC8 27B10034 */  addiu $s1, $sp, 52
    /* 800A5BCC 26105440 */  addiu $s0, $s0, 21568
    /* 800A5BD0 AFBF001C */  sw $ra, 28($sp)
    /* 800A5BD4 02002025 */  or $a0, $s0, $zero
    /* 800A5BD8 02202825 */  or $a1, $s1, $zero
    /* 800A5BDC 0C001C9C */  jal 0x80007270
    /* 800A5BE0 00003025 */  or $a2, $zero, $zero
    /* 800A5BE4 2401FFFF */  addiu $at, $zero, -1
    /* 800A5BE8 14410004 */  bne $v0, $at, 0x800A5BFC
    /* 800A5BEC 02002025 */  or $a0, $s0, $zero
    /* 800A5BF0 02202825 */  or $a1, $s1, $zero
    /* 800A5BF4 0C001C9C */  jal 0x80007270
    /* 800A5BF8 24060001 */  addiu $a2, $zero, 1
    /* 800A5BFC 02002025 */  or $a0, $s0, $zero
    /* 800A5C00 00002825 */  or $a1, $zero, $zero
    /* 800A5C04 0C001D78 */  jal 0x800075E0
    /* 800A5C08 24060001 */  addiu $a2, $zero, 1
    /* 800A5C0C 3C028003 */  lui $v0, 0x8003
    /* 800A5C10 2442EB70 */  addiu $v0, $v0, -5264
    /* 800A5C14 844E0000 */  lh $t6, 0($v0)
    /* 800A5C18 3C018015 */  lui $at, 0x8015
    /* 800A5C1C 2418FFFF */  addiu $t8, $zero, -1
    /* 800A5C20 11C00004 */  beq $t6, $zero, 0x800A5C34
    /* 800A5C24 00000000 */  nop
    /* 800A5C28 844F0000 */  lh $t7, 0($v0)
    /* 800A5C2C 55E0FFFF */  .word 0x55E0FFFF
    /* 800A5C30 844F0000 */  lh $t7, 0($v0)
    /* 800A5C34 AC206990 */  sw $zero, 27024($at)
    /* 800A5C38 3C018015 */  lui $at, 0x8015
    /* 800A5C3C AC206CE0 */  sw $zero, 27872($at)
    /* 800A5C40 3C018016 */  lui $at, 0x8016
    /* 800A5C44 AC2013AC */  sw $zero, 5036($at)
    /* 800A5C48 3C018015 */  lui $at, 0x8015
    /* 800A5C4C AC206BAC */  sw $zero, 27564($at)
    /* 800A5C50 3C018016 */  lui $at, 0x8016
    /* 800A5C54 AC2013B4 */  sw $zero, 5044($at)
    /* 800A5C58 3C018016 */  lui $at, 0x8016
    /* 800A5C5C 0C029690 */  jal 0x800A5A40
    /* 800A5C60 A438B254 */  sh $t8, -19884($at)
    /* 800A5C64 3C013F80 */  lui $at, 0x3F80
    /* 800A5C68 44812000 */  .word 0x44812000
    /* 800A5C6C 3C018016 */  lui $at, 0x8016
    /* 800A5C70 3C198012 */  lui $t9, 0x8012
    /* 800A5C74 E42413B8 */  swc1 $f4, 5048($at)
    /* 800A5C78 3C018014 */  lui $at, 0x8014
    /* 800A5C7C A4200618 */  sh $zero, 1560($at)
    /* 800A5C80 3C018014 */  lui $at, 0x8014
    /* 800A5C84 2739EA18 */  addiu $t9, $t9, -5608
    /* 800A5C88 0C029522 */  jal 0x800A5488
    /* 800A5C8C AC3906B8 */  sw $t9, 1720($at)
    /* 800A5C90 3C118015 */  lui $s1, 0x8015
    /* 800A5C94 26316D38 */  addiu $s1, $s1, 27960
    /* 800A5C98 00008025 */  or $s0, $zero, $zero
    /* 800A5C9C 8E28000C */  lw $t0, 12($s1)
    /* 800A5CA0 51000007 */  .word 0x51000007
    /* 800A5CA4 26100001 */  addiu $s0, $s0, 1
    /* 800A5CA8 82290002 */  lb $t1, 2($s1)
    /* 800A5CAC 55200004 */  .word 0x55200004
    /* 800A5CB0 26100001 */  addiu $s0, $s0, 1
    /* 800A5CB4 0C02584C */  jal 0x80096130
    /* 800A5CB8 02002025 */  or $a0, $s0, $zero
    /* 800A5CBC 26100001 */  addiu $s0, $s0, 1
    /* 800A5CC0 2A010040 */  slti $at, $s0, 64
    /* 800A5CC4 1420FFF5 */  bne $at, $zero, 0x800A5C9C
    /* 800A5CC8 26310014 */  addiu $s1, $s1, 20
    /* 800A5CCC 3C028014 */  lui $v0, 0x8014
    /* 800A5CD0 24420BDC */  addiu $v0, $v0, 3036
    /* 800A5CD4 3C118015 */  lui $s1, 0x8015
    /* 800A5CD8 A0400000 */  sb $zero, 0($v0)
    /* 800A5CDC 26316D38 */  addiu $s1, $s1, 27960
    /* 800A5CE0 00008025 */  or $s0, $zero, $zero
    /* 800A5CE4 24030040 */  addiu $v1, $zero, 64
    /* 800A5CE8 8E2A000C */  lw $t2, 12($s1)
    /* 800A5CEC 260B0001 */  addiu $t3, $s0, 1
    /* 800A5CF0 51400003 */  .word 0x51400003
    /* 800A5CF4 26100001 */  addiu $s0, $s0, 1
    /* 800A5CF8 A04B0000 */  sb $t3, 0($v0)
    /* 800A5CFC 26100001 */  addiu $s0, $s0, 1
    /* 800A5D00 1603FFF9 */  bne $s0, $v1, 0x800A5CE8
    /* 800A5D04 26310014 */  addiu $s1, $s1, 20
    /* 800A5D08 0C029478 */  jal 0x800A51E0
    /* 800A5D0C 00002025 */  or $a0, $zero, $zero
    /* 800A5D10 0C029456 */  jal 0x800A5158
    /* 800A5D14 00000000 */  nop
    /* 800A5D18 8FBF001C */  lw $ra, 28($sp)
    /* 800A5D1C 8FB00014 */  lw $s0, 20($sp)
    /* 800A5D20 8FB10018 */  lw $s1, 24($sp)
    /* 800A5D24 03E00008 */  jr $ra
    /* 800A5D28 27BD0040 */  addiu $sp, $sp, 64
