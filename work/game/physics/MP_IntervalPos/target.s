# Source: game_code.bin (decompressed)
# Address: 0x800A0D00

glabel MP_IntervalPos
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
