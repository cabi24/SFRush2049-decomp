# Source: game_code.bin (decompressed)
# Address: 0x800FE5B0

glabel stat_race_update
    /* 800FE5B0 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800FE5B4 AFBF0014 */  sw $ra, 20($sp)
    /* 800FE5B8 00A04825 */  or $t1, $a1, $zero
    /* 800FE5BC 1080005B */  beq $a0, $zero, 0x800FE72C
    /* 800FE5C0 00C04025 */  or $t0, $a2, $zero
    /* 800FE5C4 8C820008 */  lw $v0, 8($a0)
    /* 800FE5C8 50400005 */  .word 0x50400005
    /* 800FE5CC 8C850000 */  lw $a1, 0($a0)
    /* 800FE5D0 944E0010 */  lhu $t6, 16($v0)
    /* 800FE5D4 55C00010 */  .word 0x55C00010
    /* 800FE5D8 944F0010 */  lhu $t7, 16($v0)
    /* 800FE5DC 8C850000 */  lw $a1, 0($a0)
    /* 800FE5E0 AFA9001C */  sw $t1, 28($sp)
    /* 800FE5E4 AFA80020 */  sw $t0, 32($sp)
    /* 800FE5E8 AFA70024 */  sw $a3, 36($sp)
    /* 800FE5EC AFA40018 */  sw $a0, 24($sp)
    /* 800FE5F0 0C03BD6C */  jal 0x800EF5B0
    /* 800FE5F4 24060001 */  addiu $a2, $zero, 1
    /* 800FE5F8 8FA40018 */  lw $a0, 24($sp)
    /* 800FE5FC 8FA70024 */  lw $a3, 36($sp)
    /* 800FE600 8FA80020 */  lw $t0, 32($sp)
    /* 800FE604 8C820008 */  lw $v0, 8($a0)
    /* 800FE608 8FA9001C */  lw $t1, 28($sp)
    /* 800FE60C 50400048 */  .word 0x50400048
    /* 800FE610 8FBF0014 */  lw $ra, 20($sp)
    /* 800FE614 944F0010 */  lhu $t7, 16($v0)
    /* 800FE618 01E8001A */  div $t7, $t0
    /* 800FE61C 00001812 */  mflo $v1
    /* 800FE620 15000002 */  bne $t0, $zero, 0x800FE62C
    /* 800FE624 00000000 */  nop
    /* 800FE628 0007000D */  .word 0x0007000D
    /* 800FE62C 2401FFFF */  addiu $at, $zero, -1
    /* 800FE630 15010004 */  bne $t0, $at, 0x800FE644
    /* 800FE634 3C018000 */  lui $at, 0x8000
    /* 800FE638 15E10002 */  bne $t7, $at, 0x800FE644
    /* 800FE63C 00000000 */  nop
    /* 800FE640 0006000D */  .word 0x0006000D
    /* 800FE644 14600002 */  bne $v1, $zero, 0x800FE650
    /* 800FE648 00000000 */  nop
    /* 800FE64C 24030001 */  addiu $v1, $zero, 1
    /* 800FE650 0123001A */  div $t1, $v1
    /* 800FE654 00001010 */  mfhi $v0
    /* 800FE658 0000C012 */  mflo $t8
    /* 800FE65C 908D0019 */  lbu $t5, 25($a0)
    /* 800FE660 00402825 */  or $a1, $v0, $zero
    /* 800FE664 03070019 */  multu $t8, $a3
    /* 800FE668 00622823 */  subu $a1, $v1, $v0
    /* 800FE66C 14600002 */  bne $v1, $zero, 0x800FE678
    /* 800FE670 00000000 */  nop
    /* 800FE674 0007000D */  .word 0x0007000D
    /* 800FE678 2401FFFF */  addiu $at, $zero, -1
    /* 800FE67C 14610004 */  bne $v1, $at, 0x800FE690
    /* 800FE680 3C018000 */  lui $at, 0x8000
    /* 800FE684 15210002 */  bne $t1, $at, 0x800FE690
    /* 800FE688 00000000 */  nop
    /* 800FE68C 0006000D */  .word 0x0006000D
    /* 800FE690 0000C812 */  mflo $t9
    /* 800FE694 A499001C */  sh $t9, 28($a0)
    /* 800FE698 848A001C */  lh $t2, 28($a0)
    /* 800FE69C 01475821 */  addu $t3, $t2, $a3
    /* 800FE6A0 256CFFFF */  addiu $t4, $t3, -1
    /* 800FE6A4 11A00018 */  beq $t5, $zero, 0x800FE708
    /* 800FE6A8 A48C001E */  sh $t4, 30($a0)
    /* 800FE6AC 8C8E0008 */  lw $t6, 8($a0)
    /* 800FE6B0 24A5FFFF */  addiu $a1, $a1, -1
    /* 800FE6B4 95CF0010 */  lhu $t7, 16($t6)
    /* 800FE6B8 01E8001A */  div $t7, $t0
    /* 800FE6BC 0000C010 */  mfhi $t8
    /* 800FE6C0 15000002 */  bne $t0, $zero, 0x800FE6CC
    /* 800FE6C4 00000000 */  nop
    /* 800FE6C8 0007000D */  .word 0x0007000D
    /* 800FE6CC 2401FFFF */  addiu $at, $zero, -1
    /* 800FE6D0 15010004 */  bne $t0, $at, 0x800FE6E4
    /* 800FE6D4 3C018000 */  lui $at, 0x8000
    /* 800FE6D8 15E10002 */  bne $t7, $at, 0x800FE6E4
    /* 800FE6DC 00000000 */  nop
    /* 800FE6E0 0006000D */  .word 0x0006000D
    /* 800FE6E4 00A80019 */  multu $a1, $t0
    /* 800FE6E8 0000C812 */  mflo $t9
    /* 800FE6EC 03195021 */  addu $t2, $t8, $t9
    /* 800FE6F0 A48A0020 */  sh $t2, 32($a0)
    /* 800FE6F4 848B0020 */  lh $t3, 32($a0)
    /* 800FE6F8 01686021 */  addu $t4, $t3, $t0
    /* 800FE6FC 258DFFFF */  addiu $t5, $t4, -1
    /* 800FE700 10000008 */  beq $zero, $zero, 0x800FE724
    /* 800FE704 A48D0022 */  sh $t5, 34($a0)
    /* 800FE708 00480019 */  multu $v0, $t0
    /* 800FE70C 00007012 */  mflo $t6
    /* 800FE710 A48E0020 */  sh $t6, 32($a0)
    /* 800FE714 848F0020 */  lh $t7, 32($a0)
    /* 800FE718 01E8C021 */  addu $t8, $t7, $t0
    /* 800FE71C 2719FFFF */  addiu $t9, $t8, -1
    /* 800FE720 A4990022 */  sh $t9, 34($a0)
    /* 800FE724 0C0253B2 */  jal 0x80094EC8
    /* 800FE728 00000000 */  nop
    /* 800FE72C 8FBF0014 */  lw $ra, 20($sp)
    /* 800FE730 27BD0018 */  addiu $sp, $sp, 24
    /* 800FE734 03E00008 */  jr $ra
    /* 800FE738 00000000 */  nop
