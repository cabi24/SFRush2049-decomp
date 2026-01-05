# Source: game_code.bin (decompressed)
# Address: 0x800F4B8C

glabel graphics_chunk_b
    /* 800F4B8C 27BDFFB8 */  addiu $sp, $sp, -72
    /* 800F4B90 AFBE0040 */  sw $fp, 64($sp)
    /* 800F4B94 3C0E8015 */  lui $t6, 0x8015
    /* 800F4B98 85CEA108 */  lh $t6, -24312($t6)
    /* 800F4B9C 3C1E8015 */  lui $fp, 0x8015
    /* 800F4BA0 83DE978C */  lb $fp, -26740($fp)
    /* 800F4BA4 AFB7003C */  sw $s7, 60($sp)
    /* 800F4BA8 AFBF0044 */  sw $ra, 68($sp)
    /* 800F4BAC AFB60038 */  sw $s6, 56($sp)
    /* 800F4BB0 AFB50034 */  sw $s5, 52($sp)
    /* 800F4BB4 AFB40030 */  sw $s4, 48($sp)
    /* 800F4BB8 AFB3002C */  sw $s3, 44($sp)
    /* 800F4BBC AFB20028 */  sw $s2, 40($sp)
    /* 800F4BC0 AFB10024 */  sw $s1, 36($sp)
    /* 800F4BC4 AFB00020 */  sw $s0, 32($sp)
    /* 800F4BC8 F7B40018 */  .word 0xF7B40018
    /* 800F4BCC 0000B825 */  or $s7, $zero, $zero
    /* 800F4BD0 19C00063 */  blez $t6, 0x800F4D60
    /* 800F4BD4 27DEFFEE */  addiu $fp, $fp, -18
    /* 800F4BD8 3C108015 */  lui $s0, 0x8015
    /* 800F4BDC 3C168015 */  lui $s6, 0x8015
    /* 800F4BE0 3C148014 */  lui $s4, 0x8014
    /* 800F4BE4 3C128015 */  lui $s2, 0x8015
    /* 800F4BE8 4480A000 */  .word 0x4480A000
    /* 800F4BEC 26522818 */  addiu $s2, $s2, 10264
    /* 800F4BF0 26942760 */  addiu $s4, $s4, 10080
    /* 800F4BF4 26D615F8 */  addiu $s6, $s6, 5624
    /* 800F4BF8 2610A118 */  addiu $s0, $s0, -24296
    /* 800F4BFC 24150002 */  addiu $s5, $zero, 2
    /* 800F4C00 241303B8 */  addiu $s3, $zero, 952
    /* 800F4C04 24110001 */  addiu $s1, $zero, 1
    /* 800F4C08 8E020048 */  lw $v0, 72($s0)
    /* 800F4C0C 001E2880 */  sll $a1, $fp, 2
    /* 800F4C10 00BE2823 */  subu $a1, $a1, $fp
    /* 800F4C14 14400007 */  bne $v0, $zero, 0x800F4C34
    /* 800F4C18 00002025 */  or $a0, $zero, $zero
    /* 800F4C1C 920F0001 */  lbu $t7, 1($s0)
    /* 800F4C20 3C198014 */  lui $t9, 0x8014
    /* 800F4C24 27396150 */  addiu $t9, $t9, 24912
    /* 800F4C28 000FC080 */  sll $t8, $t7, 2
    /* 800F4C2C 03191021 */  addu $v0, $t8, $t9
    /* 800F4C30 AE020048 */  sw $v0, 72($s0)
    /* 800F4C34 8C490000 */  lw $t1, 0($v0)
    /* 800F4C38 3C0B8014 */  lui $t3, 0x8014
    /* 800F4C3C 256B4018 */  addiu $t3, $t3, 16408
    /* 800F4C40 8D2A002C */  lw $t2, 44($t1)
    /* 800F4C44 02EB3021 */  addu $a2, $s7, $t3
    /* 800F4C48 000528C0 */  sll $a1, $a1, 3
    /* 800F4C4C 51400045 */  .word 0x51400045
    /* 800F4C50 8FBF0044 */  lw $ra, 68($sp)
    /* 800F4C54 14800008 */  bne $a0, $zero, 0x800F4C78
    /* 800F4C58 02C51021 */  addu $v0, $s6, $a1
    /* 800F4C5C 8E0C0048 */  lw $t4, 72($s0)
    /* 800F4C60 8D8D0000 */  lw $t5, 0($t4)
    /* 800F4C64 8DAE002C */  lw $t6, 44($t5)
    /* 800F4C68 8DCF0000 */  lw $t7, 0($t6)
    /* 800F4C6C 01E51021 */  addu $v0, $t7, $a1
    /* 800F4C70 10000001 */  beq $zero, $zero, 0x800F4C78
    /* 800F4C74 2442066C */  addiu $v0, $v0, 1644
    /* 800F4C78 92180000 */  lbu $t8, 0($s0)
    /* 800F4C7C C4440008 */  lwc1 $f4, 8($v0)
    /* 800F4C80 9448000C */  lhu $t0, 12($v0)
    /* 800F4C84 03130019 */  multu $t8, $s3
    /* 800F4C88 24840001 */  addiu $a0, $a0, 1
    /* 800F4C8C 25090001 */  addiu $t1, $t0, 1
    /* 800F4C90 0000C812 */  mflo $t9
    /* 800F4C94 02591821 */  addu $v1, $s2, $t9
    /* 800F4C98 C46600F0 */  lwc1 $f6, 240($v1)
    /* 800F4C9C A449000C */  sh $t1, 12($v0)
    /* 800F4CA0 46062200 */  .word 0x46062200
    /* 800F4CA4 E4480008 */  swc1 $f8, 8($v0)
    /* 800F4CA8 90CA0000 */  lbu $t2, 0($a2)
    /* 800F4CAC 562A0010 */  .word 0x562A0010
    /* 800F4CB0 944D0010 */  lhu $t5, 16($v0)
    /* 800F4CB4 C4400004 */  lwc1 $f0, 4($v0)
    /* 800F4CB8 944B000E */  lhu $t3, 14($v0)
    /* 800F4CBC 4600A032 */  .word 0x4600A032
    /* 800F4CC0 256C0001 */  addiu $t4, $t3, 1
    /* 800F4CC4 A44C000E */  sh $t4, 14($v0)
    /* 800F4CC8 45030007 */  .word 0x45030007
    /* 800F4CCC C47000F0 */  lwc1 $f16, 240($v1)
    /* 800F4CD0 C46A00F0 */  lwc1 $f10, 240($v1)
    /* 800F4CD4 4600503C */  .word 0x4600503C
    /* 800F4CD8 00000000 */  nop
    /* 800F4CDC 45020004 */  .word 0x45020004
    /* 800F4CE0 944D0010 */  lhu $t5, 16($v0)
    /* 800F4CE4 C47000F0 */  lwc1 $f16, 240($v1)
    /* 800F4CE8 E4500004 */  swc1 $f16, 4($v0)
    /* 800F4CEC 944D0010 */  lhu $t5, 16($v0)
    /* 800F4CF0 960E0040 */  lhu $t6, 64($s0)
    /* 800F4CF4 01AE7821 */  addu $t7, $t5, $t6
    /* 800F4CF8 A44F0010 */  sh $t7, 16($v0)
    /* 800F4CFC 82980000 */  lb $t8, 0($s4)
    /* 800F4D00 1300000A */  beq $t8, $zero, 0x800F4D2C
    /* 800F4D04 00000000 */  nop
    /* 800F4D08 94590012 */  lhu $t9, 18($v0)
    /* 800F4D0C 27280001 */  addiu $t0, $t9, 1
    /* 800F4D10 A4480012 */  sh $t0, 18($v0)
    /* 800F4D14 90C90000 */  lbu $t1, 0($a2)
    /* 800F4D18 16290004 */  bne $s1, $t1, 0x800F4D2C
    /* 800F4D1C 00000000 */  nop
    /* 800F4D20 944A0014 */  lhu $t2, 20($v0)
    /* 800F4D24 254B0001 */  addiu $t3, $t2, 1
    /* 800F4D28 A44B0014 */  sh $t3, 20($v0)
    /* 800F4D2C 1495FFC9 */  bne $a0, $s5, 0x800F4C54
    /* 800F4D30 00000000 */  nop
    /* 800F4D34 3C058015 */  lui $a1, 0x8015
    /* 800F4D38 90A5978C */  lbu $a1, -26740($a1)
    /* 800F4D3C 0C03363B */  jal 0x800CD8EC
    /* 800F4D40 8E040048 */  lw $a0, 72($s0)
    /* 800F4D44 3C0C8015 */  lui $t4, 0x8015
    /* 800F4D48 858CA108 */  lh $t4, -24312($t4)
    /* 800F4D4C 26F70001 */  addiu $s7, $s7, 1
    /* 800F4D50 2610004C */  addiu $s0, $s0, 76
    /* 800F4D54 02EC082A */  slt $at, $s7, $t4
    /* 800F4D58 5420FFAC */  .word 0x5420FFAC
    /* 800F4D5C 8E020048 */  lw $v0, 72($s0)
    /* 800F4D60 8FBF0044 */  lw $ra, 68($sp)
    /* 800F4D64 D7B40018 */  .word 0xD7B40018
    /* 800F4D68 8FB00020 */  lw $s0, 32($sp)
    /* 800F4D6C 8FB10024 */  lw $s1, 36($sp)
    /* 800F4D70 8FB20028 */  lw $s2, 40($sp)
    /* 800F4D74 8FB3002C */  lw $s3, 44($sp)
    /* 800F4D78 8FB40030 */  lw $s4, 48($sp)
    /* 800F4D7C 8FB50034 */  lw $s5, 52($sp)
    /* 800F4D80 8FB60038 */  lw $s6, 56($sp)
    /* 800F4D84 8FB7003C */  lw $s7, 60($sp)
    /* 800F4D88 8FBE0040 */  lw $fp, 64($sp)
    /* 800F4D8C 03E00008 */  jr $ra
    /* 800F4D90 27BD0048 */  addiu $sp, $sp, 72
