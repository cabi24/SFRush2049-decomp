# Source: game_code.bin (decompressed)
# Address: 0x800AA454

glabel car_reset_position
    /* 800AA454 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800AA458 AFBF0014 */  sw $ra, 20($sp)
    /* 800AA45C 8E700014 */  lw $s0, 20($s3)
    /* 800AA460 8E63005C */  lw $v1, 92($s3)
    /* 800AA464 2416FFFF */  addiu $s6, $zero, -1
    /* 800AA468 8E6E0024 */  lw $t6, 36($s3)
    /* 800AA46C 8E620054 */  lw $v0, 84($s3)
    /* 800AA470 01C37823 */  subu $t7, $t6, $v1
    /* 800AA474 01E2A023 */  subu $s4, $t7, $v0
    /* 800AA478 16800007 */  bne $s4, $zero, 0x800AA498
    /* 800AA47C 00000000 */  nop
    /* 800AA480 14400005 */  bne $v0, $zero, 0x800AA498
    /* 800AA484 00000000 */  nop
    /* 800AA488 14600003 */  bne $v1, $zero, 0x800AA498
    /* 800AA48C 00000000 */  nop
    /* 800AA490 10000061 */  beq $zero, $zero, 0x800AA618
    /* 800AA494 0200A025 */  or $s4, $s0, $zero
    /* 800AA498 56960004 */  .word 0x56960004
    /* 800AA49C 8E780014 */  lw $t8, 20($s3)
    /* 800AA4A0 1000005D */  beq $zero, $zero, 0x800AA618
    /* 800AA4A4 2694FFFF */  addiu $s4, $s4, -1
    /* 800AA4A8 8E780014 */  lw $t8, 20($s3)
    /* 800AA4AC 02003025 */  or $a2, $s0, $zero
    /* 800AA4B0 0218C821 */  addu $t9, $s0, $t8
    /* 800AA4B4 272EFEFA */  addiu $t6, $t9, -262
    /* 800AA4B8 004E082B */  sltu $at, $v0, $t6
    /* 800AA4BC 54200057 */  .word 0x54200057
    /* 800AA4C0 8E620000 */  lw $v0, 0($s3)
    /* 800AA4C4 8E640020 */  lw $a0, 32($s3)
    /* 800AA4C8 32120003 */  andi $s2, $s0, 0x0003
    /* 800AA4CC 00129023 */  subu $s2, $zero, $s2
    /* 800AA4D0 02408825 */  or $s1, $s2, $zero
    /* 800AA4D4 0010A840 */  sll $s5, $s0, 1
    /* 800AA4D8 0C001F1A */  jal 0x80007C68
    /* 800AA4DC 00902821 */  addu $a1, $a0, $s0
    /* 800AA4E0 8E6F0058 */  lw $t7, 88($s3)
    /* 800AA4E4 8E790054 */  lw $t9, 84($s3)
    /* 800AA4E8 8E640034 */  lw $a0, 52($s3)
    /* 800AA4EC 01F0C023 */  subu $t8, $t7, $s0
    /* 800AA4F0 8E6F0044 */  lw $t7, 68($s3)
    /* 800AA4F4 03307023 */  subu $t6, $t9, $s0
    /* 800AA4F8 8E79002C */  lw $t9, 44($s3)
    /* 800AA4FC AE780058 */  sw $t8, 88($s3)
    /* 800AA500 AE6E0054 */  sw $t6, 84($s3)
    /* 800AA504 00047040 */  sll $t6, $a0, 1
    /* 800AA508 01F0C023 */  subu $t8, $t7, $s0
    /* 800AA50C AE780044 */  sw $t8, 68($s3)
    /* 800AA510 032E1821 */  addu $v1, $t9, $t6
    /* 800AA514 9462FFFE */  lhu $v0, -2($v1)
    /* 800AA518 2463FFFE */  addiu $v1, $v1, -2
    /* 800AA51C 2484FFFF */  addiu $a0, $a0, -1
    /* 800AA520 0050082B */  sltu $at, $v0, $s0
    /* 800AA524 14200003 */  bne $at, $zero, 0x800AA534
    /* 800AA528 00507823 */  subu $t7, $v0, $s0
    /* 800AA52C 10000002 */  beq $zero, $zero, 0x800AA538
    /* 800AA530 A46F0000 */  sh $t7, 0($v1)
    /* 800AA534 A4600000 */  sh $zero, 0($v1)
    /* 800AA538 5480FFF7 */  .word 0x5480FFF7
    /* 800AA53C 9462FFFE */  lhu $v0, -2($v1)
    /* 800AA540 8E780028 */  lw $t8, 40($s3)
    /* 800AA544 02002025 */  or $a0, $s0, $zero
    /* 800AA548 1220000F */  beq $s1, $zero, 0x800AA588
    /* 800AA54C 03151821 */  addu $v1, $t8, $s5
    /* 800AA550 02508821 */  addu $s1, $s2, $s0
    /* 800AA554 9462FFFE */  lhu $v0, -2($v1)
    /* 800AA558 2463FFFE */  addiu $v1, $v1, -2
    /* 800AA55C 2484FFFF */  addiu $a0, $a0, -1
    /* 800AA560 0050082B */  sltu $at, $v0, $s0
    /* 800AA564 14200003 */  bne $at, $zero, 0x800AA574
    /* 800AA568 0050C823 */  subu $t9, $v0, $s0
    /* 800AA56C 10000002 */  beq $zero, $zero, 0x800AA578
    /* 800AA570 A4790000 */  sh $t9, 0($v1)
    /* 800AA574 A4600000 */  sh $zero, 0($v1)
    /* 800AA578 5624FFF7 */  .word 0x5624FFF7
    /* 800AA57C 9462FFFE */  lhu $v0, -2($v1)
    /* 800AA580 50800025 */  .word 0x50800025
    /* 800AA584 0290A021 */  addu $s4, $s4, $s0
    /* 800AA588 9462FFFE */  lhu $v0, -2($v1)
    /* 800AA58C 2463FFFE */  addiu $v1, $v1, -2
    /* 800AA590 2484FFFC */  addiu $a0, $a0, -4
    /* 800AA594 0050082B */  sltu $at, $v0, $s0
    /* 800AA598 14200003 */  bne $at, $zero, 0x800AA5A8
    /* 800AA59C 00507023 */  subu $t6, $v0, $s0
    /* 800AA5A0 10000002 */  beq $zero, $zero, 0x800AA5AC
    /* 800AA5A4 A46E0000 */  sh $t6, 0($v1)
    /* 800AA5A8 A4600000 */  sh $zero, 0($v1)
    /* 800AA5AC 9462FFFE */  lhu $v0, -2($v1)
    /* 800AA5B0 2463FFFE */  addiu $v1, $v1, -2
    /* 800AA5B4 0050082B */  sltu $at, $v0, $s0
    /* 800AA5B8 14200003 */  bne $at, $zero, 0x800AA5C8
    /* 800AA5BC 00507823 */  subu $t7, $v0, $s0
    /* 800AA5C0 10000002 */  beq $zero, $zero, 0x800AA5CC
    /* 800AA5C4 A46F0000 */  sh $t7, 0($v1)
    /* 800AA5C8 A4600000 */  sh $zero, 0($v1)
    /* 800AA5CC 9462FFFE */  lhu $v0, -2($v1)
    /* 800AA5D0 2463FFFE */  addiu $v1, $v1, -2
    /* 800AA5D4 0050082B */  sltu $at, $v0, $s0
    /* 800AA5D8 14200003 */  bne $at, $zero, 0x800AA5E8
    /* 800AA5DC 0050C023 */  subu $t8, $v0, $s0
    /* 800AA5E0 10000002 */  beq $zero, $zero, 0x800AA5EC
    /* 800AA5E4 A4780000 */  sh $t8, 0($v1)
    /* 800AA5E8 A4600000 */  sh $zero, 0($v1)
    /* 800AA5EC 9462FFFE */  lhu $v0, -2($v1)
    /* 800AA5F0 2463FFFE */  addiu $v1, $v1, -2
    /* 800AA5F4 0050082B */  sltu $at, $v0, $s0
    /* 800AA5F8 14200003 */  bne $at, $zero, 0x800AA608
    /* 800AA5FC 0050C823 */  subu $t9, $v0, $s0
    /* 800AA600 10000002 */  beq $zero, $zero, 0x800AA60C
    /* 800AA604 A4790000 */  sh $t9, 0($v1)
    /* 800AA608 A4600000 */  sh $zero, 0($v1)
    /* 800AA60C 5480FFDF */  .word 0x5480FFDF
    /* 800AA610 9462FFFE */  lhu $v0, -2($v1)
    /* 800AA614 0290A021 */  addu $s4, $s4, $s0
    /* 800AA618 8E620000 */  lw $v0, 0($s3)
    /* 800AA61C 8C4E0004 */  lw $t6, 4($v0)
    /* 800AA620 51C00036 */  .word 0x51C00036
    /* 800AA624 8FBF0014 */  lw $ra, 20($sp)
    /* 800AA628 8C470004 */  lw $a3, 4($v0)
    /* 800AA62C 8E6F0020 */  lw $t7, 32($s3)
    /* 800AA630 8E780054 */  lw $t8, 84($s3)
    /* 800AA634 8E63005C */  lw $v1, 92($s3)
    /* 800AA638 0287082B */  sltu $at, $s4, $a3
    /* 800AA63C 01F8C821 */  addu $t9, $t7, $t8
    /* 800AA640 00409025 */  or $s2, $v0, $zero
    /* 800AA644 00E08825 */  or $s1, $a3, $zero
    /* 800AA648 10200002 */  beq $at, $zero, 0x800AA654
    /* 800AA64C 03232021 */  addu $a0, $t9, $v1
    /* 800AA650 02808825 */  or $s1, $s4, $zero
    /* 800AA654 16200003 */  bne $s1, $zero, 0x800AA664
    /* 800AA658 00F17023 */  subu $t6, $a3, $s1
    /* 800AA65C 1000000D */  beq $zero, $zero, 0x800AA694
    /* 800AA660 00001025 */  or $v0, $zero, $zero
    /* 800AA664 AE4E0004 */  sw $t6, 4($s2)
    /* 800AA668 8E450000 */  lw $a1, 0($s2)
    /* 800AA66C 0C001F1A */  jal 0x80007C68
    /* 800AA670 02203025 */  or $a2, $s1, $zero
    /* 800AA674 8E4F0000 */  lw $t7, 0($s2)
    /* 800AA678 8E590008 */  lw $t9, 8($s2)
    /* 800AA67C 02201025 */  or $v0, $s1, $zero
    /* 800AA680 01F1C021 */  addu $t8, $t7, $s1
    /* 800AA684 03317021 */  addu $t6, $t9, $s1
    /* 800AA688 AE580000 */  sw $t8, 0($s2)
    /* 800AA68C AE4E0008 */  sw $t6, 8($s2)
    /* 800AA690 8E63005C */  lw $v1, 92($s3)
    /* 800AA694 00627821 */  addu $t7, $v1, $v0
    /* 800AA698 2DE10003 */  sltiu $at, $t7, 3
    /* 800AA69C AE6F005C */  sw $t7, 92($s3)
    /* 800AA6A0 1420000E */  bne $at, $zero, 0x800AA6DC
    /* 800AA6A4 01E01825 */  or $v1, $t7, $zero
    /* 800AA6A8 8E780020 */  lw $t8, 32($s3)
    /* 800AA6AC 8E790054 */  lw $t9, 84($s3)
    /* 800AA6B0 8E63005C */  lw $v1, 92($s3)
    /* 800AA6B4 03191021 */  addu $v0, $t8, $t9
    /* 800AA6B8 904E0000 */  lbu $t6, 0($v0)
    /* 800AA6BC 8E780040 */  lw $t8, 64($s3)
    /* 800AA6C0 AE6E0030 */  sw $t6, 48($s3)
    /* 800AA6C4 030EC804 */  .word 0x030EC804
    /* 800AA6C8 904E0001 */  lbu $t6, 1($v0)
    /* 800AA6CC 8E78003C */  lw $t8, 60($s3)
    /* 800AA6D0 01D97826 */  xor $t7, $t6, $t9
    /* 800AA6D4 01F87024 */  and $t6, $t7, $t8
    /* 800AA6D8 AE6E0030 */  sw $t6, 48($s3)
    /* 800AA6DC 2C610106 */  sltiu $at, $v1, 262
    /* 800AA6E0 50200006 */  .word 0x50200006
    /* 800AA6E4 8FBF0014 */  lw $ra, 20($sp)
    /* 800AA6E8 8E790000 */  lw $t9, 0($s3)
    /* 800AA6EC 8F2F0004 */  lw $t7, 4($t9)
    /* 800AA6F0 55E0FF5E */  .word 0x55E0FF5E
    /* 800AA6F4 8E6E0024 */  lw $t6, 36($s3)
    /* 800AA6F8 8FBF0014 */  lw $ra, 20($sp)
    /* 800AA6FC 27BD0018 */  addiu $sp, $sp, 24
    /* 800AA700 03E00008 */  jr $ra
    /* 800AA704 00000000 */  nop
