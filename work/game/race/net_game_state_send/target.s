# Source: game_code.bin (decompressed)
# Address: 0x800F38BC

glabel net_game_state_send
    /* 800F38BC 8C98000C */  lw $t8, 12($a0)
    /* 800F38C0 8C990000 */  lw $t9, 0($a0)
    /* 800F38C4 24C60004 */  addiu $a2, $a2, 4
    /* 800F38C8 24420008 */  addiu $v0, $v0, 8
    /* 800F38CC 24840010 */  addiu $a0, $a0, 16
    /* 800F38D0 A44E002E */  sh $t6, 46($v0)
    /* 800F38D4 A44F0030 */  sh $t7, 48($v0)
    /* 800F38D8 A4580032 */  sh $t8, 50($v0)
    /* 800F38DC 14D4FFF5 */  bne $a2, $s4, 0x800F38B4
    /* 800F38E0 A459002C */  sh $t9, 44($v0)
    /* 800F38E4 02604025 */  or $t0, $s3, $zero
    /* 800F38E8 25290004 */  addiu $t1, $t1, 4
    /* 800F38EC 24630030 */  addiu $v1, $v1, 48
    /* 800F38F0 1676FFDF */  bne $s3, $s6, 0x800F3870
    /* 800F38F4 26730001 */  addiu $s3, $s3, 1
    /* 800F38F8 00004025 */  or $t0, $zero, $zero
    /* 800F38FC 2414000C */  addiu $s4, $zero, 12
    /* 800F3900 00003025 */  or $a2, $zero, $zero
    /* 800F3904 2413000C */  addiu $s3, $zero, 12
    /* 800F3908 01B50019 */  multu $t5, $s5
    /* 800F390C 0000C812 */  mflo $t9
    /* 800F3910 272E3039 */  addiu $t6, $t9, 12345
    /* 800F3914 000E1402 */  srl $v0, $t6, 16
    /* 800F3918 304F7FFF */  andi $t7, $v0, 0x7FFF
    /* 800F391C 01F3001B */  divu $t7, $s3
    /* 800F3920 01E01025 */  or $v0, $t7, $zero
    /* 800F3924 00001010 */  mfhi $v0
    /* 800F3928 3058FFFF */  andi $t8, $v0, 0xFFFF
    /* 800F392C 03001025 */  or $v0, $t8, $zero
    /* 800F3930 03002025 */  or $a0, $t8, $zero
    /* 800F3934 AE4E0000 */  sw $t6, 0($s2)
    /* 800F3938 01C06825 */  or $t5, $t6, $zero
    /* 800F393C 16600002 */  bne $s3, $zero, 0x800F3948
    /* 800F3940 00000000 */  nop
    /* 800F3944 0007000D */  .word 0x0007000D
    /* 800F3948 10D8FFEF */  beq $a2, $t8, 0x800F3908
    /* 800F394C 00000000 */  nop
    /* 800F3950 5AC00062 */  .word 0x5AC00062
    /* 800F3954 24C60001 */  addiu $a2, $a2, 1
    /* 800F3958 32C20001 */  andi $v0, $s6, 0x0001
    /* 800F395C 1040001F */  beq $v0, $zero, 0x800F39DC
    /* 800F3960 00065840 */  sll $t3, $a2, 1
    /* 800F3964 8FB90108 */  lw $t9, 264($sp)
    /* 800F3968 001EC080 */  sll $t8, $fp, 2
    /* 800F396C 031EC021 */  addu $t8, $t8, $fp
    /* 800F3970 00197080 */  sll $t6, $t9, 2
    /* 800F3974 01D97021 */  addu $t6, $t6, $t9
    /* 800F3978 000E7080 */  sll $t6, $t6, 2
    /* 800F397C 0018C080 */  sll $t8, $t8, 2
    /* 800F3980 01D97023 */  subu $t6, $t6, $t9
    /* 800F3984 000E7080 */  sll $t6, $t6, 2
    /* 800F3988 031EC023 */  subu $t8, $t8, $fp
    /* 800F398C 0018C080 */  sll $t8, $t8, 2
    /* 800F3990 022E7821 */  addu $t7, $s1, $t6
    /* 800F3994 01F82821 */  addu $a1, $t7, $t8
    /* 800F3998 0004C840 */  sll $t9, $a0, 1
    /* 800F399C 00B91021 */  addu $v0, $a1, $t9
    /* 800F39A0 00AB1821 */  addu $v1, $a1, $t3
    /* 800F39A4 846E0034 */  lh $t6, 52($v1)
    /* 800F39A8 844F0034 */  lh $t7, 52($v0)
    /* 800F39AC 24080001 */  addiu $t0, $zero, 1
    /* 800F39B0 01CFC026 */  xor $t8, $t6, $t7
    /* 800F39B4 A4780034 */  sh $t8, 52($v1)
    /* 800F39B8 846E0034 */  lh $t6, 52($v1)
    /* 800F39BC 84590034 */  lh $t9, 52($v0)
    /* 800F39C0 032E7826 */  xor $t7, $t9, $t6
    /* 800F39C4 A44F0034 */  sh $t7, 52($v0)
    /* 800F39C8 84780034 */  lh $t8, 52($v1)
    /* 800F39CC 84590034 */  lh $t9, 52($v0)
    /* 800F39D0 03197026 */  xor $t6, $t8, $t9
    /* 800F39D4 1116003F */  beq $t0, $s6, 0x800F3AD4
    /* 800F39D8 A46E0034 */  sh $t6, 52($v1)
    /* 800F39DC 27B80108 */  addiu $t8, $sp, 264
    /* 800F39E0 00087880 */  sll $t7, $t0, 2
    /* 800F39E4 0016C880 */  sll $t9, $s6, 2
    /* 800F39E8 03386021 */  addu $t4, $t9, $t8
    /* 800F39EC 01F84821 */  addu $t1, $t7, $t8
    /* 800F39F0 00045040 */  sll $t2, $a0, 1
    /* 800F39F4 8D2E0000 */  lw $t6, 0($t1)
    /* 800F39F8 001EC080 */  sll $t8, $fp, 2
    /* 800F39FC 031EC021 */  addu $t8, $t8, $fp
    /* 800F3A00 000E7880 */  sll $t7, $t6, 2
    /* 800F3A04 01EE7821 */  addu $t7, $t7, $t6
    /* 800F3A08 000F7880 */  sll $t7, $t7, 2
    /* 800F3A0C 0018C080 */  sll $t8, $t8, 2
    /* 800F3A10 01EE7823 */  subu $t7, $t7, $t6
    /* 800F3A14 000F7880 */  sll $t7, $t7, 2
    /* 800F3A18 031EC023 */  subu $t8, $t8, $fp
    /* 800F3A1C 0018C080 */  sll $t8, $t8, 2
    /* 800F3A20 022FC821 */  addu $t9, $s1, $t7
    /* 800F3A24 03383821 */  addu $a3, $t9, $t8
    /* 800F3A28 00EB1021 */  addu $v0, $a3, $t3
    /* 800F3A2C 00EA2021 */  addu $a0, $a3, $t2
    /* 800F3A30 848F0034 */  lh $t7, 52($a0)
    /* 800F3A34 844E0034 */  lh $t6, 52($v0)
    /* 800F3A38 25290008 */  addiu $t1, $t1, 8
    /* 800F3A3C 01CFC826 */  xor $t9, $t6, $t7
    /* 800F3A40 A4590034 */  sh $t9, 52($v0)
    /* 800F3A44 844E0034 */  lh $t6, 52($v0)
    /* 800F3A48 84980034 */  lh $t8, 52($a0)
    /* 800F3A4C 030E7826 */  xor $t7, $t8, $t6
    /* 800F3A50 A48F0034 */  sh $t7, 52($a0)
    /* 800F3A54 84590034 */  lh $t9, 52($v0)
    /* 800F3A58 84980034 */  lh $t8, 52($a0)
    /* 800F3A5C 8D2FFFFC */  lw $t7, -4($t1)
    /* 800F3A60 03387026 */  xor $t6, $t9, $t8
    /* 800F3A64 A44E0034 */  sh $t6, 52($v0)
    /* 800F3A68 000FC880 */  sll $t9, $t7, 2
    /* 800F3A6C 001E7080 */  sll $t6, $fp, 2
    /* 800F3A70 032FC821 */  addu $t9, $t9, $t7
    /* 800F3A74 0019C880 */  sll $t9, $t9, 2
    /* 800F3A78 01DE7021 */  addu $t6, $t6, $fp
    /* 800F3A7C 000E7080 */  sll $t6, $t6, 2
    /* 800F3A80 032FC823 */  subu $t9, $t9, $t7
    /* 800F3A84 0019C880 */  sll $t9, $t9, 2
    /* 800F3A88 01DE7023 */  subu $t6, $t6, $fp
    /* 800F3A8C 000E7080 */  sll $t6, $t6, 2
    /* 800F3A90 0239C021 */  addu $t8, $s1, $t9
    /* 800F3A94 030E4021 */  addu $t0, $t8, $t6
    /* 800F3A98 010B1821 */  addu $v1, $t0, $t3
    /* 800F3A9C 010A2821 */  addu $a1, $t0, $t2
    /* 800F3AA0 84B90034 */  lh $t9, 52($a1)
    /* 800F3AA4 846F0034 */  lh $t7, 52($v1)
    /* 800F3AA8 01F9C026 */  xor $t8, $t7, $t9
    /* 800F3AAC A4780034 */  sh $t8, 52($v1)
    /* 800F3AB0 846F0034 */  lh $t7, 52($v1)
    /* 800F3AB4 84AE0034 */  lh $t6, 52($a1)
    /* 800F3AB8 01CFC826 */  xor $t9, $t6, $t7
    /* 800F3ABC A4B90034 */  sh $t9, 52($a1)
    /* 800F3AC0 84780034 */  lh $t8, 52($v1)
    /* 800F3AC4 84AE0034 */  lh $t6, 52($a1)
    /* 800F3AC8 030E7826 */  xor $t7, $t8, $t6
    /* 800F3ACC 152CFFC9 */  bne $t1, $t4, 0x800F39F4
    /* 800F3AD0 A46F0034 */  sh $t7, 52($v1)
    /* 800F3AD4 00004025 */  or $t0, $zero, $zero
    /* 800F3AD8 24C60001 */  addiu $a2, $a2, 1
    /* 800F3ADC 14D4FF8A */  bne $a2, $s4, 0x800F3908
    /* 800F3AE0 00000000 */  nop
    /* 800F3AE4 3C0B8015 */  lui $t3, 0x8015
    /* 800F3AE8 816B4628 */  lb $t3, 17960($t3)
    /* 800F3AEC 3C078015 */  lui $a3, 0x8015
    /* 800F3AF0 24E743D8 */  addiu $a3, $a3, 17368
    /* 800F3AF4 056000C9 */  bltz $t3, 0x800F3E1C
    /* 800F3AF8 24130001 */  addiu $s3, $zero, 1
    /* 800F3AFC 3C148011 */  lui $s4, 0x8011
    /* 800F3B00 3C0C8011 */  lui $t4, 0x8011
    /* 800F3B04 3C098015 */  lui $t1, 0x8015
    /* 800F3B08 25294FD0 */  addiu $t1, $t1, 20432
    /* 800F3B0C 258C1794 */  addiu $t4, $t4, 6036
    /* 800F3B10 269417A4 */  addiu $s4, $s4, 6052
    /* 800F3B14 240A0004 */  addiu $t2, $zero, 4
    /* 800F3B18 83F90008 */  lb $t9, 8($ra)
    /* 800F3B1C 1619006E */  bne $s0, $t9, 0x800F3CD8
    /* 800F3B20 00000000 */  nop
    /* 800F3B24 01B50019 */  multu $t5, $s5
    /* 800F3B28 26E3FFFF */  addiu $v1, $s7, -1
    /* 800F3B2C 3079FFFF */  andi $t9, $v1, 0xFFFF
    /* 800F3B30 27230001 */  addiu $v1, $t9, 1
    /* 800F3B34 0000C012 */  mflo $t8
    /* 800F3B38 270E3039 */  addiu $t6, $t8, 12345
    /* 800F3B3C 000E1402 */  srl $v0, $t6, 16
    /* 800F3B40 304F7FFF */  andi $t7, $v0, 0x7FFF
    /* 800F3B44 01E3001B */  divu $t7, $v1
    /* 800F3B48 0000C010 */  mfhi $t8
    /* 800F3B4C 01C06825 */  or $t5, $t6, $zero
    /* 800F3B50 AE4E0000 */  sw $t6, 0($s2)
    /* 800F3B54 01B50019 */  multu $t5, $s5
    /* 800F3B58 01E01025 */  or $v0, $t7, $zero
    /* 800F3B5C A0F80000 */  sb $t8, 0($a3)
    /* 800F3B60 14600002 */  bne $v1, $zero, 0x800F3B6C
    /* 800F3B64 00000000 */  nop
    /* 800F3B68 0007000D */  .word 0x0007000D
    /* 800F3B6C 00007012 */  mflo $t6
    /* 800F3B70 25CF3039 */  addiu $t7, $t6, 12345
    /* 800F3B74 000FCC02 */  srl $t9, $t7, 16
    /* 800F3B78 01F50019 */  multu $t7, $s5
    /* 800F3B7C AE4F0000 */  sw $t7, 0($s2)
    /* 800F3B80 332E0001 */  andi $t6, $t9, 0x0001
    /* 800F3B84 A0EE0001 */  sb $t6, 1($a3)
    /* 800F3B88 00007812 */  mflo $t7
    /* 800F3B8C 25F93039 */  addiu $t9, $t7, 12345
    /* 800F3B90 00191402 */  srl $v0, $t9, 16
    /* 800F3B94 30587FFF */  andi $t8, $v0, 0x7FFF
    /* 800F3B98 330E0001 */  andi $t6, $t8, 0x0001
    /* 800F3B9C 31CFFFFF */  andi $t7, $t6, 0xFFFF
    /* 800F3BA0 01E01025 */  or $v0, $t7, $zero
    /* 800F3BA4 A0EF0002 */  sb $t7, 2($a3)
    /* 800F3BA8 AE590000 */  sw $t9, 0($s2)
    /* 800F3BAC 11000044 */  beq $t0, $zero, 0x800F3CC0
    /* 800F3BB0 03206825 */  or $t5, $t9, $zero
    /* 800F3BB4 90E20000 */  lbu $v0, 0($a3)
    /* 800F3BB8 90F9FFFB */  lbu $t9, -5($a3)
    /* 800F3BBC 00003025 */  or $a2, $zero, $zero
    /* 800F3BC0 0122C021 */  addu $t8, $t1, $v0
    /* 800F3BC4 54590005 */  .word 0x54590005
    /* 800F3BC8 930E0000 */  lbu $t6, 0($t8)
    /* 800F3BCC 00D7082A */  slt $at, $a2, $s7
    /* 800F3BD0 54200004 */  .word 0x54200004
    /* 800F3BD4 244F0001 */  addiu $t7, $v0, 1
    /* 800F3BD8 930E0000 */  lbu $t6, 0($t8)
    /* 800F3BDC 154E0015 */  bne $t2, $t6, 0x800F3C34
    /* 800F3BE0 244F0001 */  addiu $t7, $v0, 1
    /* 800F3BE4 31E200FF */  andi $v0, $t7, 0x00FF
    /* 800F3BE8 0057082A */  slt $at, $v0, $s7
    /* 800F3BEC A0EF0000 */  sb $t7, 0($a3)
    /* 800F3BF0 14200004 */  bne $at, $zero, 0x800F3C04
    /* 800F3BF4 00402825 */  or $a1, $v0, $zero
    /* 800F3BF8 300200FF */  andi $v0, $zero, 0x00FF
    /* 800F3BFC 00402825 */  or $a1, $v0, $zero
    /* 800F3C00 A0E00000 */  sb $zero, 0($a3)
    /* 800F3C04 90F9FFFB */  lbu $t9, -5($a3)
    /* 800F3C08 24C60001 */  addiu $a2, $a2, 1
    /* 800F3C0C 54B90005 */  .word 0x54B90005
    /* 800F3C10 0122C021 */  addu $t8, $t1, $v0
    /* 800F3C14 00D7082A */  slt $at, $a2, $s7
    /* 800F3C18 5420FFF2 */  .word 0x5420FFF2
    /* 800F3C1C 244F0001 */  addiu $t7, $v0, 1
    /* 800F3C20 0122C021 */  addu $t8, $t1, $v0
    /* 800F3C24 930E0000 */  lbu $t6, 0($t8)
    /* 800F3C28 514EFFEE */  .word 0x514EFFEE
    /* 800F3C2C 244F0001 */  addiu $t7, $v0, 1
    /* 800F3C30 00003025 */  or $a2, $zero, $zero
    /* 800F3C34 1900001E */  blez $t0, 0x800F3CB0
    /* 800F3C38 3C028015 */  lui $v0, 0x8015
    /* 800F3C3C 90E50000 */  lbu $a1, 0($a3)
    /* 800F3C40 244243D8 */  addiu $v0, $v0, 17368
    /* 800F3C44 904F0000 */  lbu $t7, 0($v0)
    /* 800F3C48 54AF0017 */  .word 0x54AF0017
    /* 800F3C4C 24C60001 */  addiu $a2, $a2, 1
    /* 800F3C50 90E30001 */  lbu $v1, 1($a3)
    /* 800F3C54 90590001 */  lbu $t9, 1($v0)
    /* 800F3C58 54790013 */  .word 0x54790013
    /* 800F3C5C 24C60001 */  addiu $a2, $a2, 1
    /* 800F3C60 90E40002 */  lbu $a0, 2($a3)
    /* 800F3C64 90580002 */  lbu $t8, 2($v0)
    /* 800F3C68 1498000E */  bne $a0, $t8, 0x800F3CA4
    /* 800F3C6C 00047040 */  sll $t6, $a0, 1
    /* 800F3C70 01C31025 */  or $v0, $t6, $v1
    /* 800F3C74 24420001 */  addiu $v0, $v0, 1
    /* 800F3C78 04410004 */  bgez $v0, 0x800F3C8C
    /* 800F3C7C 304F0003 */  andi $t7, $v0, 0x0003
    /* 800F3C80 11E00002 */  beq $t7, $zero, 0x800F3C8C
    /* 800F3C84 00000000 */  nop
    /* 800F3C88 25EFFFFC */  addiu $t7, $t7, -4
    /* 800F3C8C 000FC043 */  sra $t8, $t7, 1
    /* 800F3C90 31F90001 */  andi $t9, $t7, 0x0001
    /* 800F3C94 330E0001 */  andi $t6, $t8, 0x0001
    /* 800F3C98 A0F90001 */  sb $t9, 1($a3)
    /* 800F3C9C 10000004 */  beq $zero, $zero, 0x800F3CB0
    /* 800F3CA0 A0EE0002 */  sb $t6, 2($a3)
    /* 800F3CA4 24C60001 */  addiu $a2, $a2, 1
    /* 800F3CA8 14C8FFE6 */  bne $a2, $t0, 0x800F3C44
    /* 800F3CAC 24420005 */  addiu $v0, $v0, 5
    /* 800F3CB0 50C80004 */  .word 0x50C80004
    /* 800F3CB4 90EF0000 */  lbu $t7, 0($a3)
    /* 800F3CB8 1000FFDE */  beq $zero, $zero, 0x800F3C34
    /* 800F3CBC 00003025 */  or $a2, $zero, $zero
    /* 800F3CC0 90EF0000 */  lbu $t7, 0($a3)
    /* 800F3CC4 012F1021 */  addu $v0, $t1, $t7
    /* 800F3CC8 90590000 */  lbu $t9, 0($v0)
    /* 800F3CCC 27380001 */  addiu $t8, $t9, 1
    /* 800F3CD0 10000010 */  beq $zero, $zero, 0x800F3D14
    /* 800F3CD4 A0580000 */  sb $t8, 0($v0)
    /* 800F3CD8 0117001A */  div $t0, $s7
    /* 800F3CDC 00007010 */  mfhi $t6
    /* 800F3CE0 0117782A */  slt $t7, $t0, $s7
    /* 800F3CE4 39EF0001 */  xori $t7, $t7, 0x0001
    /* 800F3CE8 A0EE0000 */  sb $t6, 0($a3)
    /* 800F3CEC 16E00002 */  bne $s7, $zero, 0x800F3CF8
    /* 800F3CF0 00000000 */  nop
    /* 800F3CF4 0007000D */  .word 0x0007000D
    /* 800F3CF8 2401FFFF */  addiu $at, $zero, -1
    /* 800F3CFC 16E10004 */  bne $s7, $at, 0x800F3D10
    /* 800F3D00 3C018000 */  lui $at, 0x8000
    /* 800F3D04 15010002 */  bne $t0, $at, 0x800F3D10
    /* 800F3D08 00000000 */  nop
    /* 800F3D0C 0006000D */  .word 0x0006000D
    /* 800F3D10 A0EF0001 */  sb $t7, 1($a3)
    /* 800F3D14 01B50019 */  multu $t5, $s5
    /* 800F3D18 83F90008 */  lb $t9, 8($ra)
    /* 800F3D1C 0019C080 */  sll $t8, $t9, 2
    /* 800F3D20 01987021 */  addu $t6, $t4, $t8
    /* 800F3D24 95C30002 */  lhu $v1, 2($t6)
    /* 800F3D28 246E0001 */  addiu $t6, $v1, 1
    /* 800F3D2C 00007812 */  mflo $t7
    /* 800F3D30 25F93039 */  addiu $t9, $t7, 12345
    /* 800F3D34 00191402 */  srl $v0, $t9, 16
    /* 800F3D38 30587FFF */  andi $t8, $v0, 0x7FFF
    /* 800F3D3C 030E001B */  divu $t8, $t6
    /* 800F3D40 03001025 */  or $v0, $t8, $zero
    /* 800F3D44 00001010 */  mfhi $v0
    /* 800F3D48 304FFFFF */  andi $t7, $v0, 0xFFFF
    /* 800F3D4C 29E10004 */  slti $at, $t7, 4
    /* 800F3D50 01E01025 */  or $v0, $t7, $zero
    /* 800F3D54 AE590000 */  sw $t9, 0($s2)
    /* 800F3D58 03206825 */  or $t5, $t9, $zero
    /* 800F3D5C 15C00002 */  bne $t6, $zero, 0x800F3D68
    /* 800F3D60 00000000 */  nop
    /* 800F3D64 0007000D */  .word 0x0007000D
    /* 800F3D68 01E02025 */  or $a0, $t7, $zero
    /* 800F3D6C 50200004 */  .word 0x50200004
    /* 800F3D70 28810007 */  slti $at, $a0, 7
    /* 800F3D74 1000000D */  beq $zero, $zero, 0x800F3DAC
    /* 800F3D78 A0E00003 */  sb $zero, 3($a3)
    /* 800F3D7C 28810007 */  slti $at, $a0, 7
    /* 800F3D80 50200004 */  .word 0x50200004
    /* 800F3D84 28810009 */  slti $at, $a0, 9
    /* 800F3D88 10000007 */  beq $zero, $zero, 0x800F3DA8
    /* 800F3D8C 24030001 */  addiu $v1, $zero, 1
    /* 800F3D90 28810009 */  slti $at, $a0, 9
    /* 800F3D94 10200003 */  beq $at, $zero, 0x800F3DA4
    /* 800F3D98 02001025 */  or $v0, $s0, $zero
    /* 800F3D9C 10000001 */  beq $zero, $zero, 0x800F3DA4
    /* 800F3DA0 24020002 */  addiu $v0, $zero, 2
    /* 800F3DA4 00401825 */  or $v1, $v0, $zero
    /* 800F3DA8 A0E30003 */  sb $v1, 3($a3)
    /* 800F3DAC 01B50019 */  multu $t5, $s5
    /* 800F3DB0 83F90008 */  lb $t9, 8($ra)
    /* 800F3DB4 02604025 */  or $t0, $s3, $zero
    /* 800F3DB8 0168082A */  slt $at, $t3, $t0
    /* 800F3DBC 0019C080 */  sll $t8, $t9, 2
    /* 800F3DC0 02987021 */  addu $t6, $s4, $t8
    /* 800F3DC4 95C30002 */  lhu $v1, 2($t6)
    /* 800F3DC8 26730001 */  addiu $s3, $s3, 1
    /* 800F3DCC 24E70005 */  addiu $a3, $a3, 5
    /* 800F3DD0 246E0001 */  addiu $t6, $v1, 1
    /* 800F3DD4 00007812 */  mflo $t7
    /* 800F3DD8 25F93039 */  addiu $t9, $t7, 12345
    /* 800F3DDC 00191402 */  srl $v0, $t9, 16
    /* 800F3DE0 30587FFF */  andi $t8, $v0, 0x7FFF
    /* 800F3DE4 030E001B */  divu $t8, $t6
    /* 800F3DE8 03001025 */  or $v0, $t8, $zero
    /* 800F3DEC 00001010 */  mfhi $v0
    /* 800F3DF0 304FFFFF */  andi $t7, $v0, 0xFFFF
    /* 800F3DF4 01E01025 */  or $v0, $t7, $zero
    /* 800F3DF8 A0EFFFFF */  sb $t7, -1($a3)
    /* 800F3DFC AE590000 */  sw $t9, 0($s2)
    /* 800F3E00 03206825 */  or $t5, $t9, $zero
    /* 800F3E04 15C00002 */  bne $t6, $zero, 0x800F3E10
    /* 800F3E08 00000000 */  nop
    /* 800F3E0C 0007000D */  .word 0x0007000D
    /* 800F3E10 5020FF42 */  .word 0x5020FF42
    /* 800F3E14 83F90008 */  lb $t9, 8($ra)
    /* 800F3E18 00004025 */  or $t0, $zero, $zero
    /* 800F3E1C 24140001 */  addiu $s4, $zero, 1
    /* 800F3E20 169E0003 */  bne $s4, $fp, 0x800F3E30
    /* 800F3E24 24190006 */  addiu $t9, $zero, 6
    /* 800F3E28 10000002 */  beq $zero, $zero, 0x800F3E34
    /* 800F3E2C 00001825 */  or $v1, $zero, $zero
    /* 800F3E30 24030002 */  addiu $v1, $zero, 2
    /* 800F3E34 03236823 */  subu $t5, $t9, $v1
    /* 800F3E38 19A0006C */  blez $t5, 0x800F3FEC
    /* 800F3E3C 3C098015 */  lui $t1, 0x8015
    /* 800F3E40 3C128015 */  lui $s2, 0x8015
    /* 800F3E44 3C0C8011 */  lui $t4, 0x8011
    /* 800F3E48 3C0B8015 */  lui $t3, 0x8015
    /* 800F3E4C 256B5140 */  addiu $t3, $t3, 20800
    /* 800F3E50 258C176C */  addiu $t4, $t4, 5996
    /* 800F3E54 26525148 */  addiu $s2, $s2, 20808
    /* 800F3E58 25294450 */  addiu $t1, $t1, 17488
    /* 800F3E5C 24130001 */  addiu $s3, $zero, 1
    /* 800F3E60 240A0006 */  addiu $t2, $zero, 6
    /* 800F3E64 8FA3005C */  lw $v1, 92($sp)
    /* 800F3E68 8FA20054 */  lw $v0, 84($sp)
    /* 800F3E6C 93F80006 */  lbu $t8, 6($ra)
    /* 800F3E70 00003025 */  or $a2, $zero, $zero
    /* 800F3E74 5300000C */  .word 0x5300000C
    /* 800F3E78 A5200002 */  sh $zero, 2($t1)
    /* 800F3E7C 906E0001 */  lbu $t6, 1($v1)
    /* 800F3E80 0008C040 */  sll $t8, $t0, 1
    /* 800F3E84 000E7880 */  sll $t7, $t6, 2
    /* 800F3E88 01EE7823 */  subu $t7, $t7, $t6
    /* 800F3E8C 000F7880 */  sll $t7, $t7, 2
    /* 800F3E90 024FC821 */  addu $t9, $s2, $t7
    /* 800F3E94 03387021 */  addu $t6, $t9, $t8
    /* 800F3E98 95CF0000 */  lhu $t7, 0($t6)
    /* 800F3E9C 10000002 */  beq $zero, $zero, 0x800F3EA8
    /* 800F3EA0 A52F0002 */  sh $t7, 2($t1)
    /* 800F3EA4 A5200002 */  sh $zero, 2($t1)
    /* 800F3EA8 83F90009 */  lb $t9, 9($ra)
    /* 800F3EAC 1B20003B */  blez $t9, 0x800F3F9C
    /* 800F3EB0 0008C080 */  sll $t8, $t0, 2
    /* 800F3EB4 0308C021 */  addu $t8, $t8, $t0
    /* 800F3EB8 0018C080 */  sll $t8, $t8, 2
    /* 800F3EBC 0308C023 */  subu $t8, $t8, $t0
    /* 800F3EC0 0018C080 */  sll $t8, $t8, 2
    /* 800F3EC4 02382021 */  addu $a0, $s1, $t8
    /* 800F3EC8 93EE0006 */  lbu $t6, 6($ra)
    /* 800F3ECC 11C0000A */  beq $t6, $zero, 0x800F3EF8
    /* 800F3ED0 00000000 */  nop
    /* 800F3ED4 906F0001 */  lbu $t7, 1($v1)
    /* 800F3ED8 016FC821 */  addu $t9, $t3, $t7
    /* 800F3EDC 93380000 */  lbu $t8, 0($t9)
    /* 800F3EE0 00D8082A */  slt $at, $a2, $t8
    /* 800F3EE4 10200004 */  beq $at, $zero, 0x800F3EF8
    /* 800F3EE8 00000000 */  nop
    /* 800F3EEC A08A0004 */  sb $t2, 4($a0)
    /* 800F3EF0 10000023 */  beq $zero, $zero, 0x800F3F80
    /* 800F3EF4 A080001C */  sb $zero, 28($a0)
    /* 800F3EF8 00D00019 */  multu $a2, $s0
    /* 800F3EFC 000870C0 */  sll $t6, $t0, 3
    /* 800F3F00 01C87021 */  addu $t6, $t6, $t0
    /* 800F3F04 A0800004 */  sb $zero, 4($a0)
    /* 800F3F08 03EE2821 */  addu $a1, $ra, $t6
    /* 800F3F0C 00001025 */  or $v0, $zero, $zero
    /* 800F3F10 01263821 */  addu $a3, $t1, $a2
    /* 800F3F14 00001812 */  mflo $v1
    /* 800F3F18 00000000 */  nop
    /* 800F3F1C 00000000 */  nop
    /* 800F3F20 000378C3 */  sra $t7, $v1, 3
    /* 800F3F24 00AFC821 */  addu $t9, $a1, $t7
    /* 800F3F28 93380014 */  lbu $t8, 20($t9)
    /* 800F3F2C 306E0007 */  andi $t6, $v1, 0x0007
    /* 800F3F30 24630001 */  addiu $v1, $v1, 1
    /* 800F3F34 01D87807 */  .word 0x01D87807
    /* 800F3F38 908E0004 */  lbu $t6, 4($a0)
    /* 800F3F3C 31F90001 */  andi $t9, $t7, 0x0001
    /* 800F3F40 0059C004 */  .word 0x0059C004
    /* 800F3F44 24420001 */  addiu $v0, $v0, 1
    /* 800F3F48 01D87825 */  or $t7, $t6, $t8
    /* 800F3F4C 1450FFF4 */  bne $v0, $s0, 0x800F3F20
    /* 800F3F50 A08F0004 */  sb $t7, 4($a0)
    /* 800F3F54 90F90004 */  lbu $t9, 4($a3)
    /* 800F3F58 8FA3005C */  lw $v1, 92($sp)
    /* 800F3F5C 8FA20054 */  lw $v0, 84($sp)
    /* 800F3F60 00197080 */  sll $t6, $t9, 2
    /* 800F3F64 018EC021 */  addu $t8, $t4, $t6
    /* 800F3F68 8F0F0000 */  lw $t7, 0($t8)
    /* 800F3F6C A0EF001C */  sb $t7, 28($a3)
    /* 800F3F70 95390002 */  lhu $t9, 2($t1)
    /* 800F3F74 31EE00FF */  andi $t6, $t7, 0x00FF
    /* 800F3F78 032EC021 */  addu $t8, $t9, $t6
    /* 800F3F7C A5380002 */  sh $t8, 2($t1)
    /* 800F3F80 8FAF00F8 */  lw $t7, 248($sp)
    /* 800F3F84 24C60001 */  addiu $a2, $a2, 1
    /* 800F3F88 24840001 */  addiu $a0, $a0, 1
    /* 800F3F8C 81F90009 */  lb $t9, 9($t7)
    /* 800F3F90 00D9082A */  slt $at, $a2, $t9
    /* 800F3F94 5420FFCD */  .word 0x5420FFCD
    /* 800F3F98 93EE0006 */  lbu $t6, 6($ra)
    /* 800F3F9C 00C2082A */  slt $at, $a2, $v0
    /* 800F3FA0 1020000E */  beq $at, $zero, 0x800F3FDC
    /* 800F3FA4 2529004C */  addiu $t1, $t1, 76
    /* 800F3FA8 00087080 */  sll $t6, $t0, 2
    /* 800F3FAC 01C87021 */  addu $t6, $t6, $t0
    /* 800F3FB0 000E7080 */  sll $t6, $t6, 2
    /* 800F3FB4 01C87023 */  subu $t6, $t6, $t0
    /* 800F3FB8 000E7080 */  sll $t6, $t6, 2
    /* 800F3FBC 022EC021 */  addu $t8, $s1, $t6
    /* 800F3FC0 03062021 */  addu $a0, $t8, $a2
    /* 800F3FC4 24C60001 */  addiu $a2, $a2, 1
    /* 800F3FC8 00C2082A */  slt $at, $a2, $v0
    /* 800F3FCC 24840001 */  addiu $a0, $a0, 1
    /* 800F3FD0 A08A0003 */  sb $t2, 3($a0)
    /* 800F3FD4 1420FFFB */  bne $at, $zero, 0x800F3FC4
    /* 800F3FD8 A080001B */  sb $zero, 27($a0)
    /* 800F3FDC 02604025 */  or $t0, $s3, $zero
    /* 800F3FE0 010D082A */  slt $at, $t0, $t5
    /* 800F3FE4 1420FFA1 */  bne $at, $zero, 0x800F3E6C
    /* 800F3FE8 26730001 */  addiu $s3, $s3, 1
    /* 800F3FEC 29010006 */  slti $at, $t0, 6
    /* 800F3FF0 8FA20054 */  lw $v0, 84($sp)
    /* 800F3FF4 1020001C */  beq $at, $zero, 0x800F4068
    /* 800F3FF8 240A0006 */  addiu $t2, $zero, 6
    /* 800F3FFC 00087880 */  sll $t7, $t0, 2
    /* 800F4000 01E87821 */  addu $t7, $t7, $t0
    /* 800F4004 000F7880 */  sll $t7, $t7, 2
    /* 800F4008 01E87823 */  subu $t7, $t7, $t0
    /* 800F400C 000F7880 */  sll $t7, $t7, 2
    /* 800F4010 022F4821 */  addu $t1, $s1, $t7
    /* 800F4014 25130001 */  addiu $s3, $t0, 1
    /* 800F4018 A5200002 */  sh $zero, 2($t1)
    /* 800F401C 1840000D */  blez $v0, 0x800F4054
    /* 800F4020 00003025 */  or $a2, $zero, $zero
    /* 800F4024 0008C880 */  sll $t9, $t0, 2
    /* 800F4028 0328C821 */  addu $t9, $t9, $t0
    /* 800F402C 0019C880 */  sll $t9, $t9, 2
    /* 800F4030 0328C823 */  subu $t9, $t9, $t0
    /* 800F4034 0019C880 */  sll $t9, $t9, 2
    /* 800F4038 02392021 */  addu $a0, $s1, $t9
    /* 800F403C 24C60001 */  addiu $a2, $a2, 1
    /* 800F4040 00C2082A */  slt $at, $a2, $v0
    /* 800F4044 24840001 */  addiu $a0, $a0, 1
    /* 800F4048 A08A0003 */  sb $t2, 3($a0)
    /* 800F404C 1420FFFB */  bne $at, $zero, 0x800F403C
    /* 800F4050 A080001B */  sb $zero, 27($a0)
    /* 800F4054 02604025 */  or $t0, $s3, $zero
    /* 800F4058 29010006 */  slti $at, $t0, 6
    /* 800F405C 26730001 */  addiu $s3, $s3, 1
    /* 800F4060 1420FFED */  bne $at, $zero, 0x800F4018
    /* 800F4064 2529004C */  addiu $t1, $t1, 76
    /* 800F4068 00004025 */  or $t0, $zero, $zero
    /* 800F406C 24130001 */  addiu $s3, $zero, 1
    /* 800F4070 27A90108 */  addiu $t1, $sp, 264
    /* 800F4074 AD280000 */  sw $t0, 0($t1)
    /* 800F4078 02604025 */  or $t0, $s3, $zero
    /* 800F407C 29010006 */  slti $at, $t0, 6
    /* 800F4080 26730001 */  addiu $s3, $s3, 1
    /* 800F4084 1420FFFB */  bne $at, $zero, 0x800F4074
    /* 800F4088 25290004 */  addiu $t1, $t1, 4
