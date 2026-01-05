# Source: game_code.bin (decompressed)
# Address: 0x800A0B00

glabel MaxPath
    /* 800A0B00 93D80015 */  lbu $t8, 21($fp)
    /* 800A0B04 24010001 */  addiu $at, $zero, 1
    /* 800A0B08 50410004 */  .word 0x50410004
    /* 800A0B0C 240E0800 */  addiu $t6, $zero, 2048
    /* 800A0B10 54C20010 */  .word 0x54C20010
    /* 800A0B14 240F0400 */  addiu $t7, $zero, 1024
    /* 800A0B18 240E0800 */  addiu $t6, $zero, 2048
    /* 800A0B1C 01C3001A */  div $t6, $v1
    /* 800A0B20 00009812 */  mflo $s3
    /* 800A0B24 14600002 */  bne $v1, $zero, 0x800A0B30
    /* 800A0B28 00000000 */  nop
    /* 800A0B2C 0007000D */  .word 0x0007000D
    /* 800A0B30 2401FFFF */  addiu $at, $zero, -1
    /* 800A0B34 14610004 */  bne $v1, $at, 0x800A0B48
    /* 800A0B38 3C018000 */  lui $at, 0x8000
    /* 800A0B3C 15C10002 */  bne $t6, $at, 0x800A0B48
    /* 800A0B40 00000000 */  nop
    /* 800A0B44 0006000D */  .word 0x0006000D
    /* 800A0B48 1000000E */  beq $zero, $zero, 0x800A0B84
    /* 800A0B4C 93D80015 */  lbu $t8, 21($fp)
    /* 800A0B50 240F0400 */  addiu $t7, $zero, 1024
    /* 800A0B54 01E3001A */  div $t7, $v1
    /* 800A0B58 00009812 */  mflo $s3
    /* 800A0B5C 14600002 */  bne $v1, $zero, 0x800A0B68
    /* 800A0B60 00000000 */  nop
    /* 800A0B64 0007000D */  .word 0x0007000D
    /* 800A0B68 2401FFFF */  addiu $at, $zero, -1
    /* 800A0B6C 14610004 */  bne $v1, $at, 0x800A0B80
    /* 800A0B70 3C018000 */  lui $at, 0x8000
    /* 800A0B74 15E10002 */  bne $t7, $at, 0x800A0B80
    /* 800A0B78 00000000 */  nop
    /* 800A0B7C 0006000D */  .word 0x0006000D
    /* 800A0B80 93D80015 */  lbu $t8, 21($fp)
    /* 800A0B84 00008025 */  or $s0, $zero, $zero
    /* 800A0B88 24040008 */  addiu $a0, $zero, 8
    /* 800A0B8C 33190008 */  andi $t9, $t8, 0x0008
    /* 800A0B90 13200005 */  beq $t9, $zero, 0x800A0BA8
    /* 800A0B94 00000000 */  nop
    /* 800A0B98 0C021C17 */  jal 0x8008705C
    /* 800A0B9C 24040008 */  addiu $a0, $zero, 8
    /* 800A0BA0 10000004 */  beq $zero, $zero, 0x800A0BB4
    /* 800A0BA4 93CE0015 */  lbu $t6, 21($fp)
    /* 800A0BA8 0C021E38 */  jal 0x800878E0
    /* 800A0BAC 24100001 */  addiu $s0, $zero, 1
    /* 800A0BB0 93CE0015 */  lbu $t6, 21($fp)
    /* 800A0BB4 31CF0001 */  andi $t7, $t6, 0x0001
    /* 800A0BB8 51E00004 */  .word 0x51E00004
    /* 800A0BBC 93C40014 */  lbu $a0, 20($fp)
    /* 800A0BC0 0C021E38 */  jal 0x800878E0
    /* 800A0BC4 24040001 */  addiu $a0, $zero, 1
    /* 800A0BC8 93C40014 */  lbu $a0, 20($fp)
    /* 800A0BCC 240100FF */  addiu $at, $zero, 255
    /* 800A0BD0 14810005 */  bne $a0, $at, 0x800A0BE8
    /* 800A0BD4 00000000 */  nop
    /* 800A0BD8 0C021C17 */  jal 0x8008705C
    /* 800A0BDC 24040020 */  addiu $a0, $zero, 32
    /* 800A0BE0 10000004 */  beq $zero, $zero, 0x800A0BF4
    /* 800A0BE4 8FC30000 */  lw $v1, 0($fp)
    /* 800A0BE8 0C0228E3 */  jal 0x8008A38C
    /* 800A0BEC 00000000 */  nop
    /* 800A0BF0 8FC30000 */  lw $v1, 0($fp)
    /* 800A0BF4 00003825 */  or $a3, $zero, $zero
    /* 800A0BF8 50600008 */  .word 0x50600008
    /* 800A0BFC 86A30016 */  lh $v1, 22($s5)
    /* 800A0C00 8C640014 */  lw $a0, 20($v1)
    /* 800A0C04 92A50015 */  lbu $a1, 21($s5)
    /* 800A0C08 0C022852 */  jal 0x8008A148
    /* 800A0C0C 92A60014 */  lbu $a2, 20($s5)
    /* 800A0C10 10000034 */  beq $zero, $zero, 0x800A0CE4
    /* 800A0C14 87C4001C */  lh $a0, 28($fp)
    /* 800A0C18 86A30016 */  lh $v1, 22($s5)
    /* 800A0C1C 3C0F8014 */  lui $t7, 0x8014
    /* 800A0C20 00003825 */  or $a3, $zero, $zero
    /* 800A0C24 04620011 */  .word 0x04620011
    /* 800A0C28 92A50015 */  lbu $a1, 21($s5)
    /* 800A0C2C 97D80008 */  lhu $t8, 8($fp)
    /* 800A0C30 92A50015 */  lbu $a1, 21($s5)
    /* 800A0C34 92A60014 */  lbu $a2, 20($s5)
    /* 800A0C38 0018CA83 */  sra $t9, $t8, 10
    /* 800A0C3C 001970C0 */  sll $t6, $t9, 3
    /* 800A0C40 01EE7821 */  addu $t7, $t7, $t6
    /* 800A0C44 8DEF8670 */  lw $t7, -31120($t7)
    /* 800A0C48 0003C080 */  sll $t8, $v1, 2
    /* 800A0C4C 0303C023 */  subu $t8, $t8, $v1
    /* 800A0C50 0018C0C0 */  sll $t8, $t8, 3
    /* 800A0C54 01F81021 */  addu $v0, $t7, $t8
    /* 800A0C58 0C022852 */  jal 0x8008A148
    /* 800A0C5C 8C440014 */  lw $a0, 20($v0)
    /* 800A0C60 10000020 */  beq $zero, $zero, 0x800A0CE4
    /* 800A0C64 87C4001C */  lh $a0, 28($fp)
    /* 800A0C68 92A50015 */  lbu $a1, 21($s5)
    /* 800A0C6C 24010003 */  addiu $at, $zero, 3
    /* 800A0C70 3C048012 */  lui $a0, 0x8012
    /* 800A0C74 14A1000A */  bne $a1, $at, 0x800A0CA0
    /* 800A0C78 2484ED08 */  addiu $a0, $a0, -4856
    /* 800A0C7C 93D90014 */  lbu $t9, 20($fp)
    /* 800A0C80 3C018012 */  lui $at, 0x8012
    /* 800A0C84 00003825 */  or $a3, $zero, $zero
    /* 800A0C88 A039ED0B */  sb $t9, -4853($at)
    /* 800A0C8C 92A60014 */  lbu $a2, 20($s5)
    /* 800A0C90 0C022852 */  jal 0x8008A148
    /* 800A0C94 92A50015 */  lbu $a1, 21($s5)
    /* 800A0C98 10000012 */  beq $zero, $zero, 0x800A0CE4
    /* 800A0C9C 87C4001C */  lh $a0, 28($fp)
    /* 800A0CA0 50A00010 */  .word 0x50A00010
    /* 800A0CA4 87C4001C */  lh $a0, 28($fp)
    /* 800A0CA8 97CE0008 */  lhu $t6, 8($fp)
    /* 800A0CAC 3C198014 */  lui $t9, 0x8014
    /* 800A0CB0 27398670 */  addiu $t9, $t9, -31120
    /* 800A0CB4 000E7A83 */  sra $t7, $t6, 10
    /* 800A0CB8 000FC0C0 */  sll $t8, $t7, 3
    /* 800A0CBC 03191821 */  addu $v1, $t8, $t9
    /* 800A0CC0 8C6E0004 */  lw $t6, 4($v1)
    /* 800A0CC4 00003825 */  or $a3, $zero, $zero
    /* 800A0CC8 51C00006 */  .word 0x51C00006
    /* 800A0CCC 87C4001C */  lh $a0, 28($fp)
    /* 800A0CD0 8C620000 */  lw $v0, 0($v1)
    /* 800A0CD4 92A60014 */  lbu $a2, 20($s5)
    /* 800A0CD8 0C022852 */  jal 0x8008A148
    /* 800A0CDC 8C440014 */  lw $a0, 20($v0)
    /* 800A0CE0 87C4001C */  lh $a0, 28($fp)
    /* 800A0CE4 96A30012 */  lhu $v1, 18($s5)
    /* 800A0CE8 0083082A */  slt $at, $a0, $v1
    /* 800A0CEC 14200003 */  bne $at, $zero, 0x800A0CFC
    /* 800A0CF0 246FFFFF */  addiu $t7, $v1, -1
    /* 800A0CF4 A7CF001C */  sh $t7, 28($fp)
    /* 800A0CF8 87C4001C */  lh $a0, 28($fp)
    /* 800A0CFC 87C20018 */  lh $v0, 24($fp)
    /* 800A0D00 02D3082A */  slt $at, $s6, $s3
    /* 800A0D04 04410003 */  bgez $v0, 0x800A0D14
    /* 800A0D08 00408825 */  or $s1, $v0, $zero
    /* 800A0D0C 10000001 */  beq $zero, $zero, 0x800A0D14
    /* 800A0D10 00008825 */  or $s1, $zero, $zero
    /* 800A0D14 10200002 */  beq $at, $zero, 0x800A0D20
    /* 800A0D18 00000000 */  nop
    /* 800A0D1C 02C09825 */  or $s3, $s6, $zero
    /* 800A0D20 1200003E */  beq $s0, $zero, 0x800A0E1C
    /* 800A0D24 0224082A */  slt $at, $s1, $a0
    /* 800A0D28 0224082A */  slt $at, $s1, $a0
    /* 800A0D2C 1020006E */  beq $at, $zero, 0x800A0EE8
    /* 800A0D30 2656FFFF */  addiu $s6, $s2, -1
    /* 800A0D34 8FB90098 */  lw $t9, 152($sp)
    /* 800A0D38 32D8FFFF */  andi $t8, $s6, 0xFFFF
    /* 800A0D3C 0300B025 */  or $s6, $t8, $zero
    /* 800A0D40 0332B821 */  addu $s7, $t9, $s2
    /* 800A0D44 26F7FFFF */  addiu $s7, $s7, -1
    /* 800A0D48 87C4001C */  lh $a0, 28($fp)
    /* 800A0D4C 96A20012 */  lhu $v0, 18($s5)
    /* 800A0D50 02339021 */  addu $s2, $s1, $s3
    /* 800A0D54 0092082A */  slt $at, $a0, $s2
    /* 800A0D58 92A50015 */  lbu $a1, 21($s5)
    /* 800A0D5C 10200004 */  beq $at, $zero, 0x800A0D70
    /* 800A0D60 00401825 */  or $v1, $v0, $zero
    /* 800A0D64 00919823 */  subu $s3, $a0, $s1
    /* 800A0D68 26730001 */  addiu $s3, $s3, 1
    /* 800A0D6C 02339021 */  addu $s2, $s1, $s3
    /* 800A0D70 8EA40018 */  lw $a0, 24($s5)
    /* 800A0D74 92A60014 */  lbu $a2, 20($s5)
    /* 800A0D78 96A70010 */  lhu $a3, 16($s5)
    /* 800A0D7C 00717023 */  subu $t6, $v1, $s1
    /* 800A0D80 01D37823 */  subu $t7, $t6, $s3
    /* 800A0D84 8FAE0090 */  lw $t6, 144($sp)
    /* 800A0D88 0071C023 */  subu $t8, $v1, $s1
    /* 800A0D8C 2719FFFF */  addiu $t9, $t8, -1
    /* 800A0D90 AFB90020 */  sw $t9, 32($sp)
    /* 800A0D94 AFAF0018 */  sw $t7, 24($sp)
    /* 800A0D98 AFA00024 */  sw $zero, 36($sp)
    /* 800A0D9C AFB6001C */  sw $s6, 28($sp)
    /* 800A0DA0 AFA00014 */  sw $zero, 20($sp)
    /* 800A0DA4 AFA20010 */  sw $v0, 16($sp)
    /* 800A0DA8 0C021E82 */  jal 0x80087A08
    /* 800A0DAC AFAE0028 */  sw $t6, 40($sp)
    /* 800A0DB0 87CF001A */  lh $t7, 26($fp)
    /* 800A0DB4 02938021 */  addu $s0, $s4, $s3
    /* 800A0DB8 2607FFFF */  addiu $a3, $s0, -1
    /* 800A0DBC AFAF0010 */  sw $t7, 16($sp)
    /* 800A0DC0 96B80012 */  lhu $t8, 18($s5)
    /* 800A0DC4 8FA40098 */  lw $a0, 152($sp)
    /* 800A0DC8 02802825 */  or $a1, $s4, $zero
    /* 800A0DCC 0311C823 */  subu $t9, $t8, $s1
    /* 800A0DD0 03337023 */  subu $t6, $t9, $s3
    /* 800A0DD4 AFAE0014 */  sw $t6, 20($sp)
    /* 800A0DD8 0C021C44 */  jal 0x80087110
    /* 800A0DDC 02E03025 */  or $a2, $s7, $zero
    /* 800A0DE0 3C0F8003 */  lui $t7, 0x8003
    /* 800A0DE4 8DEFAFC4 */  lw $t7, -20540($t7)
    /* 800A0DE8 8FB800B4 */  lw $t8, 180($sp)
    /* 800A0DEC 29E100F1 */  slti $at, $t7, 241
    /* 800A0DF0 54200003 */  .word 0x54200003
    /* 800A0DF4 8719001C */  lh $t9, 28($t8)
    /* 800A0DF8 02138021 */  addu $s0, $s0, $s3
    /* 800A0DFC 8719001C */  lh $t9, 28($t8)
    /* 800A0E00 02408825 */  or $s1, $s2, $zero
    /* 800A0E04 0200A025 */  or $s4, $s0, $zero
    /* 800A0E08 0259082A */  slt $at, $s2, $t9
    /* 800A0E0C 5420FFCF */  .word 0x5420FFCF
    /* 800A0E10 87C4001C */  lh $a0, 28($fp)
    /* 800A0E14 10000035 */  beq $zero, $zero, 0x800A0EEC
    /* 800A0E18 93CE0015 */  lbu $t6, 21($fp)
    /* 800A0E1C 10200032 */  beq $at, $zero, 0x800A0EE8
    /* 800A0E20 2656FFFF */  addiu $s6, $s2, -1
    /* 800A0E24 8FAF0098 */  lw $t7, 152($sp)
    /* 800A0E28 32CEFFFF */  andi $t6, $s6, 0xFFFF
    /* 800A0E2C 01C0B025 */  or $s6, $t6, $zero
    /* 800A0E30 01F2B821 */  addu $s7, $t7, $s2
    /* 800A0E34 26F7FFFF */  addiu $s7, $s7, -1
    /* 800A0E38 87C4001C */  lh $a0, 28($fp)
    /* 800A0E3C 02339021 */  addu $s2, $s1, $s3
    /* 800A0E40 92A50015 */  lbu $a1, 21($s5)
    /* 800A0E44 0092082A */  slt $at, $a0, $s2
    /* 800A0E48 10200004 */  beq $at, $zero, 0x800A0E5C
    /* 800A0E4C 96A20012 */  lhu $v0, 18($s5)
    /* 800A0E50 00919823 */  subu $s3, $a0, $s1
    /* 800A0E54 26730001 */  addiu $s3, $s3, 1
    /* 800A0E58 02339021 */  addu $s2, $s1, $s3
    /* 800A0E5C 8EA40018 */  lw $a0, 24($s5)
    /* 800A0E60 92A60014 */  lbu $a2, 20($s5)
    /* 800A0E64 96A70010 */  lhu $a3, 16($s5)
    /* 800A0E68 8FB90090 */  lw $t9, 144($sp)
    /* 800A0E6C 2658FFFF */  addiu $t8, $s2, -1
    /* 800A0E70 AFB80020 */  sw $t8, 32($sp)
    /* 800A0E74 AFA00024 */  sw $zero, 36($sp)
    /* 800A0E78 AFB6001C */  sw $s6, 28($sp)
    /* 800A0E7C AFB10018 */  sw $s1, 24($sp)
    /* 800A0E80 AFA00014 */  sw $zero, 20($sp)
    /* 800A0E84 AFA20010 */  sw $v0, 16($sp)
    /* 800A0E88 0C021E82 */  jal 0x80087A08
    /* 800A0E8C AFB90028 */  sw $t9, 40($sp)
    /* 800A0E90 87CE001A */  lh $t6, 26($fp)
    /* 800A0E94 02938021 */  addu $s0, $s4, $s3
    /* 800A0E98 2607FFFF */  addiu $a3, $s0, -1
    /* 800A0E9C 8FA40098 */  lw $a0, 152($sp)
    /* 800A0EA0 02802825 */  or $a1, $s4, $zero
    /* 800A0EA4 02E03025 */  or $a2, $s7, $zero
    /* 800A0EA8 AFB10014 */  sw $s1, 20($sp)
    /* 800A0EAC 0C021C44 */  jal 0x80087110
    /* 800A0EB0 AFAE0010 */  sw $t6, 16($sp)
    /* 800A0EB4 3C0F8003 */  lui $t7, 0x8003
    /* 800A0EB8 8DEFAFC4 */  lw $t7, -20540($t7)
    /* 800A0EBC 8FB800B4 */  lw $t8, 180($sp)
    /* 800A0EC0 29E100F1 */  slti $at, $t7, 241
    /* 800A0EC4 54200003 */  .word 0x54200003
    /* 800A0EC8 8719001C */  lh $t9, 28($t8)
    /* 800A0ECC 02138021 */  addu $s0, $s0, $s3
    /* 800A0ED0 8719001C */  lh $t9, 28($t8)
    /* 800A0ED4 02408825 */  or $s1, $s2, $zero
    /* 800A0ED8 0200A025 */  or $s4, $s0, $zero
    /* 800A0EDC 0259082A */  slt $at, $s2, $t9
    /* 800A0EE0 5420FFD6 */  .word 0x5420FFD6
    /* 800A0EE4 87C4001C */  lh $a0, 28($fp)
    /* 800A0EE8 93CE0015 */  lbu $t6, 21($fp)
    /* 800A0EEC 31CF0001 */  andi $t7, $t6, 0x0001
    /* 800A0EF0 11E00003 */  beq $t7, $zero, 0x800A0F00
    /* 800A0EF4 00000000 */  nop
    /* 800A0EF8 0C021C17 */  jal 0x8008705C
    /* 800A0EFC 24040001 */  addiu $a0, $zero, 1
    /* 800A0F00 3C028016 */  lui $v0, 0x8016
    /* 800A0F04 8C4213AC */  lw $v0, 5036($v0)
    /* 800A0F08 3C198014 */  lui $t9, 0x8014
    /* 800A0F0C 27390BF0 */  addiu $t9, $t9, 3056
    /* 800A0F10 0002C140 */  sll $t8, $v0, 5
    /* 800A0F14 03191821 */  addu $v1, $t8, $t9
    /* 800A0F18 27DE0020 */  addiu $fp, $fp, 32
    /* 800A0F1C 03C3082B */  sltu $at, $fp, $v1
    /* 800A0F20 5420FE5B */  .word 0x5420FE5B
    /* 800A0F24 83CF0016 */  lb $t7, 22($fp)
    /* 800A0F28 8FBF0074 */  lw $ra, 116($sp)
    /* 800A0F2C D7B40038 */  .word 0xD7B40038
    /* 800A0F30 D7B60040 */  .word 0xD7B60040
    /* 800A0F34 D7B80048 */  .word 0xD7B80048
    /* 800A0F38 8FB00050 */  lw $s0, 80($sp)
    /* 800A0F3C 8FB10054 */  lw $s1, 84($sp)
    /* 800A0F40 8FB20058 */  lw $s2, 88($sp)
    /* 800A0F44 8FB3005C */  lw $s3, 92($sp)
    /* 800A0F48 8FB40060 */  lw $s4, 96($sp)
    /* 800A0F4C 8FB50064 */  lw $s5, 100($sp)
    /* 800A0F50 8FB60068 */  lw $s6, 104($sp)
    /* 800A0F54 8FB7006C */  lw $s7, 108($sp)
    /* 800A0F58 8FBE0070 */  lw $fp, 112($sp)
    /* 800A0F5C 03E00008 */  jr $ra
    /* 800A0F60 27BD0100 */  addiu $sp, $sp, 256
