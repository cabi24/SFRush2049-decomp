# Source: game_code.bin (decompressed)
# Address: 0x800BA46C

glabel audio_priority_find
    /* 800BA46C 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800BA470 AFA4001C */  sw $a0, 28($sp)
    /* 800BA474 00047400 */  sll $t6, $a0, 16
    /* 800BA478 3C068014 */  lui $a2, 0x8014
    /* 800BA47C 24C607F0 */  addiu $a2, $a2, 2032
    /* 800BA480 000E2403 */  sra $a0, $t6, 16
    /* 800BA484 90CE0008 */  lbu $t6, 8($a2)
    /* 800BA488 AFA50018 */  sw $a1, 24($sp)
    /* 800BA48C 0005C400 */  sll $t8, $a1, 16
    /* 800BA490 008E082A */  slt $at, $a0, $t6
    /* 800BA494 00182C03 */  sra $a1, $t8, 16
    /* 800BA498 14200003 */  bne $at, $zero, 0x800BA4A8
    /* 800BA49C 24020001 */  addiu $v0, $zero, 1
    /* 800BA4A0 1000005C */  beq $zero, $zero, 0x800BA614
    /* 800BA4A4 2402FFFF */  addiu $v0, $zero, -1
    /* 800BA4A8 8CCF000C */  lw $t7, 12($a2)
    /* 800BA4AC 0004C100 */  sll $t8, $a0, 4
    /* 800BA4B0 00057080 */  sll $t6, $a1, 2
    /* 800BA4B4 01F81821 */  addu $v1, $t7, $t8
    /* 800BA4B8 90790000 */  lbu $t9, 0($v1)
    /* 800BA4BC 24010002 */  addiu $at, $zero, 2
    /* 800BA4C0 01C57021 */  addu $t6, $t6, $a1
    /* 800BA4C4 17210007 */  bne $t9, $at, 0x800BA4E4
    /* 800BA4C8 000E7100 */  sll $t6, $t6, 4
    /* 800BA4CC 14A00003 */  bne $a1, $zero, 0x800BA4DC
    /* 800BA4D0 00000000 */  nop
    /* 800BA4D4 1000004F */  beq $zero, $zero, 0x800BA614
    /* 800BA4D8 00001025 */  or $v0, $zero, $zero
    /* 800BA4DC 1000004D */  beq $zero, $zero, 0x800BA614
    /* 800BA4E0 2402FFFF */  addiu $v0, $zero, -1
    /* 800BA4E4 3C0F8015 */  lui $t7, 0x8015
    /* 800BA4E8 25EF1CE8 */  addiu $t7, $t7, 7400
    /* 800BA4EC 01CF2021 */  addu $a0, $t6, $t7
    /* 800BA4F0 C4840018 */  lwc1 $f4, 24($a0)
    /* 800BA4F4 C4860020 */  lwc1 $f6, 32($a0)
    /* 800BA4F8 00003025 */  or $a2, $zero, $zero
    /* 800BA4FC E7A40000 */  swc1 $f4, 0($sp)
    /* 800BA500 E7A60004 */  swc1 $f6, 4($sp)
    /* 800BA504 9478000A */  lhu $t8, 10($v1)
    /* 800BA508 24070006 */  addiu $a3, $zero, 6
    /* 800BA50C 5B000041 */  .word 0x5B000041
    /* 800BA510 2402FFFF */  addiu $v0, $zero, -1
    /* 800BA514 8C990024 */  lw $t9, 36($a0)
    /* 800BA518 44807000 */  .word 0x44807000
    /* 800BA51C C480000C */  lwc1 $f0, 12($a0)
    /* 800BA520 44994000 */  .word 0x44994000
    /* 800BA524 C4820014 */  lwc1 $f2, 20($a0)
    /* 800BA528 87A50014 */  lh $a1, 20($sp)
    /* 800BA52C 46804320 */  .word 0x46804320
    /* 800BA530 00C70019 */  multu $a2, $a3
    /* 800BA534 8C6E000C */  lw $t6, 12($v1)
    /* 800BA538 00002012 */  mflo $a0
    /* 800BA53C 01C47821 */  addu $t7, $t6, $a0
    /* 800BA540 85F80000 */  lh $t8, 0($t7)
    /* 800BA544 44985000 */  .word 0x44985000
    /* 800BA548 00000000 */  nop
    /* 800BA54C 46805120 */  .word 0x46805120
    /* 800BA550 46002181 */  .word 0x46002181
    /* 800BA554 E7A60008 */  swc1 $f6, 8($sp)
    /* 800BA558 8C79000C */  lw $t9, 12($v1)
    /* 800BA55C 03247021 */  addu $t6, $t9, $a0
    /* 800BA560 85CF0004 */  lh $t7, 4($t6)
    /* 800BA564 448F4000 */  .word 0x448F4000
    /* 800BA568 00000000 */  nop
    /* 800BA56C 468042A0 */  .word 0x468042A0
    /* 800BA570 46063202 */  .word 0x46063202
    /* 800BA574 46025101 */  .word 0x46025101
    /* 800BA578 46042282 */  .word 0x46042282
    /* 800BA57C E7A4000C */  swc1 $f4, 12($sp)
    /* 800BA580 460A4200 */  .word 0x460A4200
    /* 800BA584 C7AA0004 */  lwc1 $f10, 4($sp)
    /* 800BA588 460C403E */  .word 0x460C403E
    /* 800BA58C 00000000 */  nop
    /* 800BA590 45020018 */  .word 0x45020018
    /* 800BA594 946F000A */  lhu $t7, 10($v1)
    /* 800BA598 46045202 */  .word 0x46045202
    /* 800BA59C C7AA0000 */  lwc1 $f10, 0($sp)
    /* 800BA5A0 24040001 */  addiu $a0, $zero, 1
    /* 800BA5A4 460A3102 */  .word 0x460A3102
    /* 800BA5A8 46044180 */  .word 0x46044180
    /* 800BA5AC 460E303C */  .word 0x460E303C
    /* 800BA5B0 00000000 */  nop
    /* 800BA5B4 45000003 */  .word 0x45000003
    /* 800BA5B8 00000000 */  nop
    /* 800BA5BC 10000001 */  beq $zero, $zero, 0x800BA5C4
    /* 800BA5C0 2404FFFF */  addiu $a0, $zero, -1
    /* 800BA5C4 10400006 */  beq $v0, $zero, 0x800BA5E0
    /* 800BA5C8 00000000 */  nop
    /* 800BA5CC 00042C00 */  sll $a1, $a0, 16
    /* 800BA5D0 0005C403 */  sra $t8, $a1, 16
    /* 800BA5D4 03002825 */  or $a1, $t8, $zero
    /* 800BA5D8 10000005 */  beq $zero, $zero, 0x800BA5F0
    /* 800BA5DC 00001025 */  or $v0, $zero, $zero
    /* 800BA5E0 50A40004 */  .word 0x50A40004
    /* 800BA5E4 946F000A */  lhu $t7, 10($v1)
    /* 800BA5E8 1000000A */  beq $zero, $zero, 0x800BA614
    /* 800BA5EC 00C01025 */  or $v0, $a2, $zero
    /* 800BA5F0 946F000A */  lhu $t7, 10($v1)
    /* 800BA5F4 24C60001 */  addiu $a2, $a2, 1
    /* 800BA5F8 0006CC00 */  sll $t9, $a2, 16
    /* 800BA5FC 00193403 */  sra $a2, $t9, 16
    /* 800BA600 00CF082A */  slt $at, $a2, $t7
    /* 800BA604 1420FFCA */  bne $at, $zero, 0x800BA530
    /* 800BA608 00000000 */  nop
    /* 800BA60C A7A50014 */  sh $a1, 20($sp)
    /* 800BA610 2402FFFF */  addiu $v0, $zero, -1
    /* 800BA614 03E00008 */  jr $ra
    /* 800BA618 27BD0018 */  addiu $sp, $sp, 24
