# Source: game_code.bin (decompressed)
# Address: 0x800AC3D8

glabel differential_output
    /* 800AC3D8 27BDFFB0 */  addiu $sp, $sp, -80
    /* 800AC3DC AFB40038 */  sw $s4, 56($sp)
    /* 800AC3E0 0005A400 */  sll $s4, $a1, 16
    /* 800AC3E4 00147403 */  sra $t6, $s4, 16
    /* 800AC3E8 AFB20030 */  sw $s2, 48($sp)
    /* 800AC3EC AFB00028 */  sw $s0, 40($sp)
    /* 800AC3F0 01C0A025 */  or $s4, $t6, $zero
    /* 800AC3F4 AFBF004C */  sw $ra, 76($sp)
    /* 800AC3F8 AFBE0048 */  sw $fp, 72($sp)
    /* 800AC3FC AFB70044 */  sw $s7, 68($sp)
    /* 800AC400 AFB60040 */  sw $s6, 64($sp)
    /* 800AC404 AFB5003C */  sw $s5, 60($sp)
    /* 800AC408 AFB30034 */  sw $s3, 52($sp)
    /* 800AC40C AFB1002C */  sw $s1, 44($sp)
    /* 800AC410 AFA40050 */  sw $a0, 80($sp)
    /* 800AC414 AFA50054 */  sw $a1, 84($sp)
    /* 800AC418 2412FFFF */  addiu $s2, $zero, -1
    /* 800AC41C 10800065 */  beq $a0, $zero, 0x800AC5B4
    /* 800AC420 00808025 */  or $s0, $a0, $zero
    /* 800AC424 3C1E8015 */  lui $fp, 0x8015
    /* 800AC428 3C178015 */  lui $s7, 0x8015
    /* 800AC42C 3C168015 */  lui $s6, 0x8015
    /* 800AC430 26D69B80 */  addiu $s6, $s6, -25728
    /* 800AC434 26F79D90 */  addiu $s7, $s7, -25200
    /* 800AC438 27DE9D94 */  addiu $fp, $fp, -25196
    /* 800AC43C 24150068 */  addiu $s5, $zero, 104
    /* 800AC440 2413FFFF */  addiu $s3, $zero, -1
    /* 800AC444 00142C00 */  sll $a1, $s4, 16
    /* 800AC448 0005C403 */  sra $t8, $a1, 16
    /* 800AC44C 24190001 */  addiu $t9, $zero, 1
    /* 800AC450 AFD00000 */  sw $s0, 0($fp)
    /* 800AC454 AFB90014 */  sw $t9, 20($sp)
    /* 800AC458 03002825 */  or $a1, $t8, $zero
    /* 800AC45C 02002025 */  or $a0, $s0, $zero
    /* 800AC460 00003025 */  or $a2, $zero, $zero
    /* 800AC464 24070001 */  addiu $a3, $zero, 1
    /* 800AC468 AFB30010 */  sw $s3, 16($sp)
    /* 800AC46C 0C02AF32 */  jal 0x800ABCC8
    /* 800AC470 AFA00018 */  sw $zero, 24($sp)
    /* 800AC474 14400042 */  bne $v0, $zero, 0x800AC580
    /* 800AC478 02002025 */  or $a0, $s0, $zero
    /* 800AC47C 3C088014 */  lui $t0, 0x8014
    /* 800AC480 25080BDC */  addiu $t0, $t0, 3036
    /* 800AC484 91060000 */  lbu $a2, 0($t0)
    /* 800AC488 00148C00 */  sll $s1, $s4, 16
    /* 800AC48C 00115C03 */  sra $t3, $s1, 16
    /* 800AC490 24C6FFFF */  addiu $a2, $a2, -1
    /* 800AC494 00064E00 */  sll $t1, $a2, 24
    /* 800AC498 00093603 */  sra $a2, $t1, 24
    /* 800AC49C 01608825 */  or $s1, $t3, $zero
    /* 800AC4A0 00002825 */  or $a1, $zero, $zero
    /* 800AC4A4 0C024B8B */  jal 0x80092E2C
    /* 800AC4A8 24070001 */  addiu $a3, $zero, 1
    /* 800AC4AC 3C0C8011 */  lui $t4, 0x8011
    /* 800AC4B0 8D8C74B4 */  lw $t4, 29876($t4)
    /* 800AC4B4 00402025 */  or $a0, $v0, $zero
    /* 800AC4B8 26050010 */  addiu $a1, $s0, 16
    /* 800AC4BC 318D0008 */  andi $t5, $t4, 0x0008
    /* 800AC4C0 11A0000C */  beq $t5, $zero, 0x800AC4F4
    /* 800AC4C4 00113400 */  sll $a2, $s1, 16
    /* 800AC4C8 8E020040 */  lw $v0, 64($s0)
    /* 800AC4CC 00113400 */  sll $a2, $s1, 16
    /* 800AC4D0 00067403 */  sra $t6, $a2, 16
    /* 800AC4D4 01C03025 */  or $a2, $t6, $zero
    /* 800AC4D8 26050010 */  addiu $a1, $s0, 16
    /* 800AC4DC 0C02389B */  jal 0x8008E26C
    /* 800AC4E0 34470E00 */  ori $a3, $v0, 0x0E00
    /* 800AC4E4 00028C00 */  sll $s1, $v0, 16
    /* 800AC4E8 00117C03 */  sra $t7, $s1, 16
    /* 800AC4EC 10000008 */  beq $zero, $zero, 0x800AC510
    /* 800AC4F0 01E08825 */  or $s1, $t7, $zero
    /* 800AC4F4 0006C403 */  sra $t8, $a2, 16
    /* 800AC4F8 03003025 */  or $a2, $t8, $zero
    /* 800AC4FC 0C02389B */  jal 0x8008E26C
    /* 800AC500 8E070040 */  lw $a3, 64($s0)
    /* 800AC504 00028C00 */  sll $s1, $v0, 16
    /* 800AC508 0011CC03 */  sra $t9, $s1, 16
    /* 800AC50C 03208825 */  or $s1, $t9, $zero
    /* 800AC510 C6040050 */  lwc1 $f4, 80($s0)
    /* 800AC514 C606005C */  lwc1 $f6, 92($s0)
    /* 800AC518 46062032 */  .word 0x46062032
    /* 800AC51C 00000000 */  nop
    /* 800AC520 45000007 */  .word 0x45000007
    /* 800AC524 00000000 */  nop
    /* 800AC528 C6080054 */  lwc1 $f8, 84($s0)
    /* 800AC52C C60A0060 */  lwc1 $f10, 96($s0)
    /* 800AC530 460A4032 */  .word 0x460A4032
    /* 800AC534 00000000 */  nop
    /* 800AC538 45030009 */  .word 0x45030009
    /* 800AC53C 86E90000 */  lh $t1, 0($s7)
    /* 800AC540 16930006 */  bne $s4, $s3, 0x800AC55C
    /* 800AC544 26040050 */  addiu $a0, $s0, 80
    /* 800AC548 00113400 */  sll $a2, $s1, 16
    /* 800AC54C 00064403 */  sra $t0, $a2, 16
    /* 800AC550 01003025 */  or $a2, $t0, $zero
    /* 800AC554 0C029F84 */  jal 0x800A7E10
    /* 800AC558 2605005C */  addiu $a1, $s0, 92
    /* 800AC55C 86E90000 */  lh $t1, 0($s7)
    /* 800AC560 16690002 */  bne $s3, $t1, 0x800AC56C
    /* 800AC564 00000000 */  nop
    /* 800AC568 A6F10000 */  sh $s1, 0($s7)
    /* 800AC56C 56530005 */  .word 0x56530005
    /* 800AC570 86020044 */  lh $v0, 68($s0)
    /* 800AC574 00119400 */  sll $s2, $s1, 16
    /* 800AC578 00125403 */  sra $t2, $s2, 16
    /* 800AC57C 01409025 */  or $s2, $t2, $zero
    /* 800AC580 86020044 */  lh $v0, 68($s0)
    /* 800AC584 00008025 */  or $s0, $zero, $zero
    /* 800AC588 04400007 */  bltz $v0, 0x800AC5A8
    /* 800AC58C 00000000 */  nop
    /* 800AC590 00550019 */  multu $v0, $s5
    /* 800AC594 8ECC0000 */  lw $t4, 0($s6)
    /* 800AC598 00005812 */  mflo $t3
    /* 800AC59C 016C8021 */  addu $s0, $t3, $t4
    /* 800AC5A0 10000001 */  beq $zero, $zero, 0x800AC5A8
    /* 800AC5A4 00000000 */  nop
    /* 800AC5A8 5600FFA7 */  .word 0x5600FFA7
    /* 800AC5AC 00142C00 */  sll $a1, $s4, 16
    /* 800AC5B0 8FB00050 */  lw $s0, 80($sp)
    /* 800AC5B4 8FAD0050 */  lw $t5, 80($sp)
    /* 800AC5B8 3C168015 */  lui $s6, 0x8015
    /* 800AC5BC 26D69B80 */  addiu $s6, $s6, -25728
    /* 800AC5C0 11A0001B */  beq $t5, $zero, 0x800AC630
    /* 800AC5C4 24150068 */  addiu $s5, $zero, 104
    /* 800AC5C8 86020046 */  lh $v0, 70($s0)
    /* 800AC5CC 0442000B */  .word 0x0442000B
    /* 800AC5D0 86020044 */  lh $v0, 68($s0)
    /* 800AC5D4 00550019 */  multu $v0, $s5
    /* 800AC5D8 8ECF0000 */  lw $t7, 0($s6)
    /* 800AC5DC 00122C00 */  sll $a1, $s2, 16
    /* 800AC5E0 0005C403 */  sra $t8, $a1, 16
    /* 800AC5E4 03002825 */  or $a1, $t8, $zero
    /* 800AC5E8 00007012 */  mflo $t6
    /* 800AC5EC 01CF2021 */  addu $a0, $t6, $t7
    /* 800AC5F0 0C02B0F6 */  jal 0x800AC3D8
    /* 800AC5F4 00000000 */  nop
    /* 800AC5F8 86020044 */  lh $v0, 68($s0)
    /* 800AC5FC 26520001 */  addiu $s2, $s2, 1
    /* 800AC600 0012CC00 */  sll $t9, $s2, 16
    /* 800AC604 04400007 */  bltz $v0, 0x800AC624
    /* 800AC608 00199403 */  sra $s2, $t9, 16
    /* 800AC60C 00550019 */  multu $v0, $s5
    /* 800AC610 8ECA0000 */  lw $t2, 0($s6)
    /* 800AC614 00004812 */  mflo $t1
    /* 800AC618 012A8021 */  addu $s0, $t1, $t2
    /* 800AC61C 10000002 */  beq $zero, $zero, 0x800AC628
    /* 800AC620 00000000 */  nop
    /* 800AC624 00008025 */  or $s0, $zero, $zero
    /* 800AC628 5600FFE8 */  .word 0x5600FFE8
    /* 800AC62C 86020046 */  lh $v0, 70($s0)
    /* 800AC630 8FBF004C */  lw $ra, 76($sp)
    /* 800AC634 8FB00028 */  lw $s0, 40($sp)
    /* 800AC638 8FB1002C */  lw $s1, 44($sp)
    /* 800AC63C 8FB20030 */  lw $s2, 48($sp)
    /* 800AC640 8FB30034 */  lw $s3, 52($sp)
    /* 800AC644 8FB40038 */  lw $s4, 56($sp)
    /* 800AC648 8FB5003C */  lw $s5, 60($sp)
    /* 800AC64C 8FB60040 */  lw $s6, 64($sp)
    /* 800AC650 8FB70044 */  lw $s7, 68($sp)
    /* 800AC654 8FBE0048 */  lw $fp, 72($sp)
    /* 800AC658 03E00008 */  jr $ra
    /* 800AC65C 27BD0050 */  addiu $sp, $sp, 80
