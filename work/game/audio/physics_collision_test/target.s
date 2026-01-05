# Source: game_code.bin (decompressed)
# Address: 0x800B9194

glabel physics_collision_test
    /* 800B9194 27BDFFD0 */  addiu $sp, $sp, -48
    /* 800B9198 3C048014 */  lui $a0, 0x8014
    /* 800B919C 2484F1E0 */  addiu $a0, $a0, -3616
    /* 800B91A0 3C0E8014 */  lui $t6, 0x8014
    /* 800B91A4 AFBF002C */  sw $ra, 44($sp)
    /* 800B91A8 25CEC378 */  addiu $t6, $t6, -15496
    /* 800B91AC 240F0064 */  addiu $t7, $zero, 100
    /* 800B91B0 24180058 */  addiu $t8, $zero, 88
    /* 800B91B4 AFB30028 */  sw $s3, 40($sp)
    /* 800B91B8 AFB20024 */  sw $s2, 36($sp)
    /* 800B91BC AFB10020 */  sw $s1, 32($sp)
    /* 800B91C0 AFB0001C */  sw $s0, 28($sp)
    /* 800B91C4 AC980008 */  sw $t8, 8($a0)
    /* 800B91C8 AC8F0004 */  sw $t7, 4($a0)
    /* 800B91CC AC8E000C */  sw $t6, 12($a0)
    /* 800B91D0 0C02C134 */  jal 0x800B04D0
    /* 800B91D4 A0800000 */  sb $zero, 0($a0)
    /* 800B91D8 3C028014 */  lui $v0, 0x8014
    /* 800B91DC 3C038014 */  lui $v1, 0x8014
    /* 800B91E0 246392F0 */  addiu $v1, $v1, -27920
    /* 800B91E4 244292D8 */  addiu $v0, $v0, -27944
    /* 800B91E8 24420004 */  addiu $v0, $v0, 4
    /* 800B91EC 0043082B */  sltu $at, $v0, $v1
    /* 800B91F0 1420FFFD */  bne $at, $zero, 0x800B91E8
    /* 800B91F4 AC40FFFC */  sw $zero, -4($v0)
    /* 800B91F8 3C198015 */  lui $t9, 0x8015
    /* 800B91FC 83396994 */  lb $t9, 27028($t9)
    /* 800B9200 3C118011 */  lui $s1, 0x8011
    /* 800B9204 3C088015 */  lui $t0, 0x8015
    /* 800B9208 17200004 */  bne $t9, $zero, 0x800B921C
    /* 800B920C 26317480 */  addiu $s1, $s1, 29824
    /* 800B9210 8108978C */  lb $t0, -26740($t0)
    /* 800B9214 29010006 */  slti $at, $t0, 6
    /* 800B9218 14200013 */  bne $at, $zero, 0x800B9268
    /* 800B921C 3C108014 */  lui $s0, 0x8014
    /* 800B9220 3C138014 */  lui $s3, 0x8014
    /* 800B9224 3C128014 */  lui $s2, 0x8014
    /* 800B9228 26520BDC */  addiu $s2, $s2, 3036
    /* 800B922C 2673F38C */  addiu $s3, $s3, -3188
    /* 800B9230 2610F380 */  addiu $s0, $s0, -3200
    /* 800B9234 92470000 */  lbu $a3, 0($s2)
    /* 800B9238 240B0001 */  addiu $t3, $zero, 1
    /* 800B923C AFAB0010 */  sw $t3, 16($sp)
    /* 800B9240 24E7FFFF */  addiu $a3, $a3, -1
    /* 800B9244 00074E00 */  sll $t1, $a3, 24
    /* 800B9248 00093E03 */  sra $a3, $t1, 24
    /* 800B924C 8E240000 */  lw $a0, 0($s1)
    /* 800B9250 02002825 */  or $a1, $s0, $zero
    /* 800B9254 0C02C93B */  jal 0x800B24EC
    /* 800B9258 00003025 */  or $a2, $zero, $zero
    /* 800B925C 26100002 */  addiu $s0, $s0, 2
    /* 800B9260 1613FFF4 */  bne $s0, $s3, 0x800B9234
    /* 800B9264 26310004 */  addiu $s1, $s1, 4
    /* 800B9268 8FBF002C */  lw $ra, 44($sp)
    /* 800B926C 8FB0001C */  lw $s0, 28($sp)
    /* 800B9270 8FB10020 */  lw $s1, 32($sp)
    /* 800B9274 8FB20024 */  lw $s2, 36($sp)
    /* 800B9278 8FB30028 */  lw $s3, 40($sp)
    /* 800B927C 03E00008 */  jr $ra
    /* 800B9280 27BD0030 */  addiu $sp, $sp, 48
