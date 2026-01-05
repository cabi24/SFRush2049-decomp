# Source: game_code.bin (decompressed)
# Address: 0x800DC794

glabel tournament_unlock_check
    /* 800DC794 27BDFFC8 */  addiu $sp, $sp, -56
    /* 800DC798 AFBF0034 */  sw $ra, 52($sp)
    /* 800DC79C AFB60030 */  sw $s6, 48($sp)
    /* 800DC7A0 AFB5002C */  sw $s5, 44($sp)
    /* 800DC7A4 AFB40028 */  sw $s4, 40($sp)
    /* 800DC7A8 AFB30024 */  sw $s3, 36($sp)
    /* 800DC7AC AFB20020 */  sw $s2, 32($sp)
    /* 800DC7B0 AFB1001C */  sw $s1, 28($sp)
    /* 800DC7B4 AFB00018 */  sw $s0, 24($sp)
    /* 800DC7B8 90AE0000 */  lbu $t6, 0($a1)
    /* 800DC7BC 00809025 */  or $s2, $a0, $zero
    /* 800DC7C0 00A0A825 */  or $s5, $a1, $zero
    /* 800DC7C4 A08E0002 */  sb $t6, 2($a0)
    /* 800DC7C8 90AF0001 */  lbu $t7, 1($a1)
    /* 800DC7CC 00C0B025 */  or $s6, $a2, $zero
    /* 800DC7D0 00808025 */  or $s0, $a0, $zero
    /* 800DC7D4 A08F0003 */  sb $t7, 3($a0)
    /* 800DC7D8 90B80002 */  lbu $t8, 2($a1)
    /* 800DC7DC 00008825 */  or $s1, $zero, $zero
    /* 800DC7E0 24130008 */  addiu $s3, $zero, 8
    /* 800DC7E4 A0980004 */  sb $t8, 4($a0)
    /* 800DC7E8 90B90003 */  lbu $t9, 3($a1)
    /* 800DC7EC 24140014 */  addiu $s4, $zero, 20
    /* 800DC7F0 A0990005 */  sb $t9, 5($a0)
    /* 800DC7F4 90C80000 */  lbu $t0, 0($a2)
    /* 800DC7F8 A0880006 */  sb $t0, 6($a0)
    /* 800DC7FC 90C90001 */  lbu $t1, 1($a2)
    /* 800DC800 A0890007 */  sb $t1, 7($a0)
    /* 800DC804 90CA0002 */  lbu $t2, 2($a2)
    /* 800DC808 A08A0008 */  sb $t2, 8($a0)
    /* 800DC80C 90CB0003 */  lbu $t3, 3($a2)
    /* 800DC810 A08B0009 */  sb $t3, 9($a0)
    /* 800DC814 56330009 */  .word 0x56330009
    /* 800DC818 8E19000C */  lw $t9, 12($s0)
    /* 800DC81C 8E0D000C */  lw $t5, 12($s0)
    /* 800DC820 264C0002 */  addiu $t4, $s2, 2
    /* 800DC824 ADAC0004 */  sw $t4, 4($t5)
    /* 800DC828 8E0F000C */  lw $t7, 12($s0)
    /* 800DC82C 92AE0003 */  lbu $t6, 3($s5)
    /* 800DC830 10000007 */  beq $zero, $zero, 0x800DC850
    /* 800DC834 A1EE0018 */  sb $t6, 24($t7)
    /* 800DC838 8E19000C */  lw $t9, 12($s0)
    /* 800DC83C 26580006 */  addiu $t8, $s2, 6
    /* 800DC840 AF380004 */  sw $t8, 4($t9)
    /* 800DC844 8E09000C */  lw $t1, 12($s0)
    /* 800DC848 92C80003 */  lbu $t0, 3($s6)
    /* 800DC84C A1280018 */  sb $t0, 24($t1)
    /* 800DC850 0C0253B2 */  jal 0x80094EC8
    /* 800DC854 8E04000C */  lw $a0, 12($s0)
    /* 800DC858 26310004 */  addiu $s1, $s1, 4
    /* 800DC85C 1634FFED */  bne $s1, $s4, 0x800DC814
    /* 800DC860 26100004 */  addiu $s0, $s0, 4
    /* 800DC864 8FBF0034 */  lw $ra, 52($sp)
    /* 800DC868 8FB00018 */  lw $s0, 24($sp)
    /* 800DC86C 8FB1001C */  lw $s1, 28($sp)
    /* 800DC870 8FB20020 */  lw $s2, 32($sp)
    /* 800DC874 8FB30024 */  lw $s3, 36($sp)
    /* 800DC878 8FB40028 */  lw $s4, 40($sp)
    /* 800DC87C 8FB5002C */  lw $s5, 44($sp)
    /* 800DC880 8FB60030 */  lw $s6, 48($sp)
    /* 800DC884 03E00008 */  jr $ra
    /* 800DC888 27BD0038 */  addiu $sp, $sp, 56
