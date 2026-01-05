# Source: game_code.bin (decompressed)
# Address: 0x800A99C8

glabel car_crash_response
    /* 800A99C8 27BDFF50 */  addiu $sp, $sp, -176
    /* 800A99CC AFBF003C */  sw $ra, 60($sp)
    /* 800A99D0 AFBE0038 */  sw $fp, 56($sp)
    /* 800A99D4 AFB70034 */  sw $s7, 52($sp)
    /* 800A99D8 AFB60030 */  sw $s6, 48($sp)
    /* 800A99DC AFB5002C */  sw $s5, 44($sp)
    /* 800A99E0 AFB40028 */  sw $s4, 40($sp)
    /* 800A99E4 AFB30024 */  sw $s3, 36($sp)
    /* 800A99E8 AFB20020 */  sw $s2, 32($sp)
    /* 800A99EC AFB1001C */  sw $s1, 28($sp)
    /* 800A99F0 AFB00018 */  sw $s0, 24($sp)
    /* 800A99F4 AFA500B4 */  sw $a1, 180($sp)
    /* 800A99F8 AFA600B8 */  sw $a2, 184($sp)
    /* 800A99FC AFA700BC */  sw $a3, 188($sp)
    /* 800A9A00 8C8E006C */  lw $t6, 108($a0)
    /* 800A9A04 0080F025 */  or $fp, $a0, $zero
    /* 800A9A08 00004825 */  or $t1, $zero, $zero
    /* 800A9A0C 19C00031 */  blez $t6, 0x800A9AD4
    /* 800A9A10 8FA200B8 */  lw $v0, 184($sp)
    /* 800A9A14 0080A025 */  or $s4, $a0, $zero
    /* 800A9A18 0C02A4E4 */  jal 0x800A9390
    /* 800A9A1C 24970AFC */  addiu $s7, $a0, 2812
    /* 800A9A20 03C0A025 */  or $s4, $fp, $zero
    /* 800A9A24 0C02A4E4 */  jal 0x800A9390
    /* 800A9A28 27D70B08 */  addiu $s7, $fp, 2824
    /* 800A9A2C 03C06025 */  or $t4, $fp, $zero
    /* 800A9A30 27C40078 */  addiu $a0, $fp, 120
    /* 800A9A34 0C02A5C4 */  jal 0x800A9710
    /* 800A9A38 8FC80B00 */  lw $t0, 2816($fp)
    /* 800A9A3C 03C06025 */  or $t4, $fp, $zero
    /* 800A9A40 27C4096C */  addiu $a0, $fp, 2412
    /* 800A9A44 0C02A5C4 */  jal 0x800A9710
    /* 800A9A48 8FC80B0C */  lw $t0, 2828($fp)
    /* 800A9A4C 03C0A025 */  or $s4, $fp, $zero
    /* 800A9A50 0C02A4E4 */  jal 0x800A9390
    /* 800A9A54 27D70B14 */  addiu $s7, $fp, 2836
    /* 800A9A58 3C028012 */  lui $v0, 0x8012
    /* 800A9A5C 2442E9D2 */  addiu $v0, $v0, -5678
    /* 800A9A60 24030012 */  addiu $v1, $zero, 18
    /* 800A9A64 24040002 */  addiu $a0, $zero, 2
    /* 800A9A68 904F0000 */  lbu $t7, 0($v0)
    /* 800A9A6C 000FC080 */  sll $t8, $t7, 2
    /* 800A9A70 03D8C821 */  addu $t9, $fp, $t8
    /* 800A9A74 972E0A62 */  lhu $t6, 2658($t9)
    /* 800A9A78 55C00005 */  .word 0x55C00005
    /* 800A9A7C 8FCF168C */  lw $t7, 5772($fp)
    /* 800A9A80 2463FFFF */  addiu $v1, $v1, -1
    /* 800A9A84 1464FFF8 */  bne $v1, $a0, 0x800A9A68
    /* 800A9A88 2442FFFF */  addiu $v0, $v0, -1
    /* 800A9A8C 8FCF168C */  lw $t7, 5772($fp)
    /* 800A9A90 0003C080 */  sll $t8, $v1, 2
    /* 800A9A94 8FC51690 */  lw $a1, 5776($fp)
    /* 800A9A98 0303C023 */  subu $t8, $t8, $v1
    /* 800A9A9C 01F8C821 */  addu $t9, $t7, $t8
    /* 800A9AA0 272E0011 */  addiu $t6, $t9, 17
    /* 800A9AA4 25C4000A */  addiu $a0, $t6, 10
    /* 800A9AA8 24A5000A */  addiu $a1, $a1, 10
    /* 800A9AAC 000478C2 */  srl $t7, $a0, 3
    /* 800A9AB0 0005C0C2 */  srl $t8, $a1, 3
    /* 800A9AB4 01F8082B */  sltu $at, $t7, $t8
    /* 800A9AB8 AFCE168C */  sw $t6, 5772($fp)
    /* 800A9ABC 00604825 */  or $t1, $v1, $zero
    /* 800A9AC0 01E02025 */  or $a0, $t7, $zero
    /* 800A9AC4 14200005 */  bne $at, $zero, 0x800A9ADC
    /* 800A9AC8 03002825 */  or $a1, $t8, $zero
    /* 800A9ACC 10000003 */  beq $zero, $zero, 0x800A9ADC
    /* 800A9AD0 03002025 */  or $a0, $t8, $zero
    /* 800A9AD4 24440005 */  addiu $a0, $v0, 5
    /* 800A9AD8 00802825 */  or $a1, $a0, $zero
    /* 800A9ADC 8FB900B8 */  lw $t9, 184($sp)
    /* 800A9AE0 8FAF00B4 */  lw $t7, 180($sp)
    /* 800A9AE4 272E0004 */  addiu $t6, $t9, 4
    /* 800A9AE8 008E082B */  sltu $at, $a0, $t6
    /* 800A9AEC 14200009 */  bne $at, $zero, 0x800A9B14
    /* 800A9AF0 00000000 */  nop
    /* 800A9AF4 11E00007 */  beq $t7, $zero, 0x800A9B14
    /* 800A9AF8 03203025 */  or $a2, $t9, $zero
    /* 800A9AFC 03C02025 */  or $a0, $fp, $zero
    /* 800A9B00 01E02825 */  or $a1, $t7, $zero
    /* 800A9B04 0C02A367 */  jal 0x800A8D9C
    /* 800A9B08 8FA700BC */  lw $a3, 188($sp)
    /* 800A9B0C 1000012D */  beq $zero, $zero, 0x800A9FC4
    /* 800A9B10 00000000 */  nop
    /* 800A9B14 14A40039 */  bne $a1, $a0, 0x800A9BFC
    /* 800A9B18 25230001 */  addiu $v1, $t1, 1
    /* 800A9B1C 8FC516A4 */  lw $a1, 5796($fp)
    /* 800A9B20 3C0D8012 */  lui $t5, 0x8012
    /* 800A9B24 25AD49F0 */  addiu $t5, $t5, 18928
    /* 800A9B28 28A1000E */  slti $at, $a1, 14
    /* 800A9B2C 14200020 */  bne $at, $zero, 0x800A9BB0
    /* 800A9B30 3C158012 */  lui $s5, 0x8012
    /* 800A9B34 8FA200BC */  lw $v0, 188($sp)
    /* 800A9B38 97D816A0 */  lhu $t8, 5792($fp)
    /* 800A9B3C 24070010 */  addiu $a3, $zero, 16
    /* 800A9B40 24420002 */  addiu $v0, $v0, 2
    /* 800A9B44 00A27004 */  .word 0x00A27004
    /* 800A9B48 030E7825 */  or $t7, $t8, $t6
    /* 800A9B4C A7CF16A0 */  sh $t7, 5792($fp)
    /* 800A9B50 01E0C025 */  or $t8, $t7, $zero
    /* 800A9B54 8FCF000C */  lw $t7, 12($fp)
    /* 800A9B58 8FCE0004 */  lw $t6, 4($fp)
    /* 800A9B5C 01CFC821 */  addu $t9, $t6, $t7
    /* 800A9B60 A3380000 */  sb $t8, 0($t9)
    /* 800A9B64 8FCE000C */  lw $t6, 12($fp)
    /* 800A9B68 97D816A0 */  lhu $t8, 5792($fp)
    /* 800A9B6C 25CF0001 */  addiu $t7, $t6, 1
    /* 800A9B70 8FCE0004 */  lw $t6, 4($fp)
    /* 800A9B74 0018CA03 */  sra $t9, $t8, 8
    /* 800A9B78 AFCF000C */  sw $t7, 12($fp)
    /* 800A9B7C 01CFC021 */  addu $t8, $t6, $t7
    /* 800A9B80 A3190000 */  sb $t9, 0($t8)
    /* 800A9B84 8FCE000C */  lw $t6, 12($fp)
    /* 800A9B88 8FC416A4 */  lw $a0, 5796($fp)
    /* 800A9B8C 3059FFFF */  andi $t9, $v0, 0xFFFF
    /* 800A9B90 25CF0001 */  addiu $t7, $t6, 1
    /* 800A9B94 AFCF000C */  sw $t7, 12($fp)
    /* 800A9B98 00E4C023 */  subu $t8, $a3, $a0
    /* 800A9B9C 03197007 */  .word 0x03197007
    /* 800A9BA0 248FFFF3 */  addiu $t7, $a0, -13
    /* 800A9BA4 AFCF16A4 */  sw $t7, 5796($fp)
    /* 800A9BA8 10000009 */  beq $zero, $zero, 0x800A9BD0
    /* 800A9BAC A7CE16A0 */  sh $t6, 5792($fp)
    /* 800A9BB0 8FB800BC */  lw $t8, 188($sp)
    /* 800A9BB4 97D916A0 */  lhu $t9, 5792($fp)
    /* 800A9BB8 270E0002 */  addiu $t6, $t8, 2
    /* 800A9BBC 00AE7804 */  .word 0x00AE7804
    /* 800A9BC0 24AE0003 */  addiu $t6, $a1, 3
    /* 800A9BC4 032FC025 */  or $t8, $t9, $t7
    /* 800A9BC8 A7D816A0 */  sh $t8, 5792($fp)
    /* 800A9BCC AFCE16A4 */  sw $t6, 5796($fp)
    /* 800A9BD0 03C02025 */  or $a0, $fp, $zero
    /* 800A9BD4 26B54E70 */  addiu $s5, $s5, 20080
    /* 800A9BD8 0C02A0A1 */  jal 0x800A8284
    /* 800A9BDC AFBE00B0 */  sw $fp, 176($sp)
    /* 800A9BE0 8FBE00B0 */  lw $fp, 176($sp)
    /* 800A9BE4 8FD91694 */  lw $t9, 5780($fp)
    /* 800A9BE8 8FCF1690 */  lw $t7, 5776($fp)
    /* 800A9BEC 032FC021 */  addu $t8, $t9, $t7
    /* 800A9BF0 270E0003 */  addiu $t6, $t8, 3
    /* 800A9BF4 100000F3 */  beq $zero, $zero, 0x800A9FC4
    /* 800A9BF8 AFCE1694 */  sw $t6, 5780($fp)
    /* 800A9BFC 8FC516A4 */  lw $a1, 5796($fp)
    /* 800A9C00 27D9096C */  addiu $t9, $fp, 2412
    /* 800A9C04 AFB90040 */  sw $t9, 64($sp)
    /* 800A9C08 28A1000E */  slti $at, $a1, 14
    /* 800A9C0C 14200020 */  bne $at, $zero, 0x800A9C90
    /* 800A9C10 27CB0078 */  addiu $t3, $fp, 120
    /* 800A9C14 8FA200BC */  lw $v0, 188($sp)
    /* 800A9C18 97CF16A0 */  lhu $t7, 5792($fp)
    /* 800A9C1C 24070010 */  addiu $a3, $zero, 16
    /* 800A9C20 24420004 */  addiu $v0, $v0, 4
    /* 800A9C24 00A2C004 */  .word 0x00A2C004
    /* 800A9C28 01F87025 */  or $t6, $t7, $t8
    /* 800A9C2C A7CE16A0 */  sh $t6, 5792($fp)
    /* 800A9C30 01C07825 */  or $t7, $t6, $zero
    /* 800A9C34 8FCE000C */  lw $t6, 12($fp)
    /* 800A9C38 8FD80004 */  lw $t8, 4($fp)
    /* 800A9C3C 030EC821 */  addu $t9, $t8, $t6
    /* 800A9C40 A32F0000 */  sb $t7, 0($t9)
    /* 800A9C44 8FD8000C */  lw $t8, 12($fp)
    /* 800A9C48 97CF16A0 */  lhu $t7, 5792($fp)
    /* 800A9C4C 270E0001 */  addiu $t6, $t8, 1
    /* 800A9C50 8FD80004 */  lw $t8, 4($fp)
    /* 800A9C54 000FCA03 */  sra $t9, $t7, 8
    /* 800A9C58 AFCE000C */  sw $t6, 12($fp)
    /* 800A9C5C 030E7821 */  addu $t7, $t8, $t6
    /* 800A9C60 A1F90000 */  sb $t9, 0($t7)
    /* 800A9C64 8FD8000C */  lw $t8, 12($fp)
    /* 800A9C68 8FC416A4 */  lw $a0, 5796($fp)
    /* 800A9C6C 3059FFFF */  andi $t9, $v0, 0xFFFF
    /* 800A9C70 270E0001 */  addiu $t6, $t8, 1
    /* 800A9C74 AFCE000C */  sw $t6, 12($fp)
    /* 800A9C78 00E47823 */  subu $t7, $a3, $a0
    /* 800A9C7C 01F9C007 */  .word 0x01F9C007
    /* 800A9C80 248EFFF3 */  addiu $t6, $a0, -13
    /* 800A9C84 AFCE16A4 */  sw $t6, 5796($fp)
    /* 800A9C88 1000000A */  beq $zero, $zero, 0x800A9CB4
    /* 800A9C8C A7D816A0 */  sh $t8, 5792($fp)
    /* 800A9C90 8FAF00BC */  lw $t7, 188($sp)
    /* 800A9C94 97D916A0 */  lhu $t9, 5792($fp)
    /* 800A9C98 24070010 */  addiu $a3, $zero, 16
    /* 800A9C9C 25F80004 */  addiu $t8, $t7, 4
    /* 800A9CA0 00B87004 */  .word 0x00B87004
    /* 800A9CA4 24B80003 */  addiu $t8, $a1, 3
    /* 800A9CA8 032E7825 */  or $t7, $t9, $t6
    /* 800A9CAC A7CF16A0 */  sh $t7, 5792($fp)
    /* 800A9CB0 AFD816A4 */  sw $t8, 5796($fp)
    /* 800A9CB4 8FC516A4 */  lw $a1, 5796($fp)
    /* 800A9CB8 8FCA0B00 */  lw $t2, 2816($fp)
    /* 800A9CBC 8FC60B0C */  lw $a2, 2828($fp)
    /* 800A9CC0 28A1000C */  slti $at, $a1, 12
    /* 800A9CC4 00604025 */  or $t0, $v1, $zero
    /* 800A9CC8 254A0001 */  addiu $t2, $t2, 1
    /* 800A9CCC 1420001E */  bne $at, $zero, 0x800A9D48
    /* 800A9CD0 24C60001 */  addiu $a2, $a2, 1
    /* 800A9CD4 97D916A0 */  lhu $t9, 5792($fp)
    /* 800A9CD8 2542FEFF */  addiu $v0, $t2, -257
    /* 800A9CDC 00A27004 */  .word 0x00A27004
    /* 800A9CE0 032E7825 */  or $t7, $t9, $t6
    /* 800A9CE4 A7CF16A0 */  sh $t7, 5792($fp)
    /* 800A9CE8 01E0C825 */  or $t9, $t7, $zero
    /* 800A9CEC 8FCF000C */  lw $t7, 12($fp)
    /* 800A9CF0 8FCE0004 */  lw $t6, 4($fp)
    /* 800A9CF4 01CFC021 */  addu $t8, $t6, $t7
    /* 800A9CF8 A3190000 */  sb $t9, 0($t8)
    /* 800A9CFC 8FCE000C */  lw $t6, 12($fp)
    /* 800A9D00 97D916A0 */  lhu $t9, 5792($fp)
    /* 800A9D04 25CF0001 */  addiu $t7, $t6, 1
    /* 800A9D08 8FCE0004 */  lw $t6, 4($fp)
    /* 800A9D0C 0019C203 */  sra $t8, $t9, 8
    /* 800A9D10 AFCF000C */  sw $t7, 12($fp)
    /* 800A9D14 01CFC821 */  addu $t9, $t6, $t7
    /* 800A9D18 A3380000 */  sb $t8, 0($t9)
    /* 800A9D1C 8FCE000C */  lw $t6, 12($fp)
    /* 800A9D20 8FC416A4 */  lw $a0, 5796($fp)
    /* 800A9D24 3058FFFF */  andi $t8, $v0, 0xFFFF
    /* 800A9D28 25CF0001 */  addiu $t7, $t6, 1
    /* 800A9D2C AFCF000C */  sw $t7, 12($fp)
    /* 800A9D30 00E4C823 */  subu $t9, $a3, $a0
    /* 800A9D34 03387007 */  .word 0x03387007
    /* 800A9D38 248FFFF5 */  addiu $t7, $a0, -11
    /* 800A9D3C AFCF16A4 */  sw $t7, 5796($fp)
    /* 800A9D40 10000008 */  beq $zero, $zero, 0x800A9D64
    /* 800A9D44 A7CE16A0 */  sh $t6, 5792($fp)
    /* 800A9D48 97D816A0 */  lhu $t8, 5792($fp)
    /* 800A9D4C 2559FEFF */  addiu $t9, $t2, -257
    /* 800A9D50 00B97004 */  .word 0x00B97004
    /* 800A9D54 24B90005 */  addiu $t9, $a1, 5
    /* 800A9D58 030E7825 */  or $t7, $t8, $t6
    /* 800A9D5C A7CF16A0 */  sh $t7, 5792($fp)
    /* 800A9D60 AFD916A4 */  sw $t9, 5796($fp)
    /* 800A9D64 8FC516A4 */  lw $a1, 5796($fp)
    /* 800A9D68 24C9FFFF */  addiu $t1, $a2, -1
    /* 800A9D6C 28A1000C */  slti $at, $a1, 12
    /* 800A9D70 1420001E */  bne $at, $zero, 0x800A9DEC
    /* 800A9D74 00A9C004 */  .word 0x00A9C004
    /* 800A9D78 97D816A0 */  lhu $t8, 5792($fp)
    /* 800A9D7C 24C9FFFF */  addiu $t1, $a2, -1
    /* 800A9D80 00A97004 */  .word 0x00A97004
    /* 800A9D84 030E7825 */  or $t7, $t8, $t6
    /* 800A9D88 A7CF16A0 */  sh $t7, 5792($fp)
    /* 800A9D8C 01E0C025 */  or $t8, $t7, $zero
    /* 800A9D90 8FCF000C */  lw $t7, 12($fp)
    /* 800A9D94 8FCE0004 */  lw $t6, 4($fp)
    /* 800A9D98 01CFC821 */  addu $t9, $t6, $t7
    /* 800A9D9C A3380000 */  sb $t8, 0($t9)
    /* 800A9DA0 8FCE000C */  lw $t6, 12($fp)
    /* 800A9DA4 97D816A0 */  lhu $t8, 5792($fp)
    /* 800A9DA8 25CF0001 */  addiu $t7, $t6, 1
    /* 800A9DAC 8FCE0004 */  lw $t6, 4($fp)
    /* 800A9DB0 0018CA03 */  sra $t9, $t8, 8
    /* 800A9DB4 AFCF000C */  sw $t7, 12($fp)
    /* 800A9DB8 01CFC021 */  addu $t8, $t6, $t7
    /* 800A9DBC A3190000 */  sb $t9, 0($t8)
    /* 800A9DC0 8FCE000C */  lw $t6, 12($fp)
    /* 800A9DC4 8FC416A4 */  lw $a0, 5796($fp)
    /* 800A9DC8 3139FFFF */  andi $t9, $t1, 0xFFFF
    /* 800A9DCC 25CF0001 */  addiu $t7, $t6, 1
    /* 800A9DD0 AFCF000C */  sw $t7, 12($fp)
    /* 800A9DD4 00E4C023 */  subu $t8, $a3, $a0
    /* 800A9DD8 03197007 */  .word 0x03197007
    /* 800A9DDC 248FFFF5 */  addiu $t7, $a0, -11
    /* 800A9DE0 AFCF16A4 */  sw $t7, 5796($fp)
    /* 800A9DE4 10000006 */  beq $zero, $zero, 0x800A9E00
    /* 800A9DE8 A7CE16A0 */  sh $t6, 5792($fp)
    /* 800A9DEC 97D916A0 */  lhu $t9, 5792($fp)
    /* 800A9DF0 24AF0005 */  addiu $t7, $a1, 5
    /* 800A9DF4 AFCF16A4 */  sw $t7, 5796($fp)
    /* 800A9DF8 03387025 */  or $t6, $t9, $t8
    /* 800A9DFC A7CE16A0 */  sh $t6, 5792($fp)
    /* 800A9E00 8FC516A4 */  lw $a1, 5796($fp)
    /* 800A9E04 2479FFFC */  addiu $t9, $v1, -4
    /* 800A9E08 28A1000D */  slti $at, $a1, 13
    /* 800A9E0C 1420001E */  bne $at, $zero, 0x800A9E88
    /* 800A9E10 00B9C004 */  .word 0x00B9C004
    /* 800A9E14 97D916A0 */  lhu $t9, 5792($fp)
    /* 800A9E18 2462FFFC */  addiu $v0, $v1, -4
    /* 800A9E1C 00A2C004 */  .word 0x00A2C004
    /* 800A9E20 03387025 */  or $t6, $t9, $t8
    /* 800A9E24 A7CE16A0 */  sh $t6, 5792($fp)
    /* 800A9E28 01C0C825 */  or $t9, $t6, $zero
    /* 800A9E2C 8FCE000C */  lw $t6, 12($fp)
    /* 800A9E30 8FD80004 */  lw $t8, 4($fp)
    /* 800A9E34 030E7821 */  addu $t7, $t8, $t6
    /* 800A9E38 A1F90000 */  sb $t9, 0($t7)
    /* 800A9E3C 8FD8000C */  lw $t8, 12($fp)
    /* 800A9E40 97D916A0 */  lhu $t9, 5792($fp)
    /* 800A9E44 270E0001 */  addiu $t6, $t8, 1
    /* 800A9E48 8FD80004 */  lw $t8, 4($fp)
    /* 800A9E4C 00197A03 */  sra $t7, $t9, 8
    /* 800A9E50 AFCE000C */  sw $t6, 12($fp)
    /* 800A9E54 030EC821 */  addu $t9, $t8, $t6
    /* 800A9E58 A32F0000 */  sb $t7, 0($t9)
    /* 800A9E5C 8FD8000C */  lw $t8, 12($fp)
    /* 800A9E60 8FC416A4 */  lw $a0, 5796($fp)
    /* 800A9E64 304FFFFF */  andi $t7, $v0, 0xFFFF
    /* 800A9E68 270E0001 */  addiu $t6, $t8, 1
    /* 800A9E6C AFCE000C */  sw $t6, 12($fp)
    /* 800A9E70 00E4C823 */  subu $t9, $a3, $a0
    /* 800A9E74 032FC007 */  .word 0x032FC007
    /* 800A9E78 248EFFF4 */  addiu $t6, $a0, -12
    /* 800A9E7C AFCE16A4 */  sw $t6, 5796($fp)
    /* 800A9E80 10000006 */  beq $zero, $zero, 0x800A9E9C
    /* 800A9E84 A7D816A0 */  sh $t8, 5792($fp)
    /* 800A9E88 97CF16A0 */  lhu $t7, 5792($fp)
    /* 800A9E8C 24B90004 */  addiu $t9, $a1, 4
    /* 800A9E90 AFD916A4 */  sw $t9, 5796($fp)
    /* 800A9E94 01F87025 */  or $t6, $t7, $t8
    /* 800A9E98 A7CE16A0 */  sh $t6, 5792($fp)
    /* 800A9E9C 18600034 */  blez $v1, 0x800A9F70
    /* 800A9EA0 00003025 */  or $a2, $zero, $zero
    /* 800A9EA4 3C038012 */  lui $v1, 0x8012
    /* 800A9EA8 2463E9C0 */  addiu $v1, $v1, -5696
    /* 800A9EAC 8FC516A4 */  lw $a1, 5796($fp)
    /* 800A9EB0 24C60001 */  addiu $a2, $a2, 1
    /* 800A9EB4 28A1000E */  slti $at, $a1, 14
    /* 800A9EB8 54200022 */  .word 0x54200022
    /* 800A9EBC 90790000 */  lbu $t9, 0($v1)
    /* 800A9EC0 906F0000 */  lbu $t7, 0($v1)
    /* 800A9EC4 97D916A0 */  lhu $t9, 5792($fp)
    /* 800A9EC8 000FC080 */  sll $t8, $t7, 2
    /* 800A9ECC 03D87021 */  addu $t6, $fp, $t8
    /* 800A9ED0 95C20A62 */  lhu $v0, 2658($t6)
    /* 800A9ED4 00A27804 */  .word 0x00A27804
    /* 800A9ED8 032FC025 */  or $t8, $t9, $t7
    /* 800A9EDC A7D816A0 */  sh $t8, 5792($fp)
    /* 800A9EE0 0300C825 */  or $t9, $t8, $zero
    /* 800A9EE4 8FD8000C */  lw $t8, 12($fp)
    /* 800A9EE8 8FCF0004 */  lw $t7, 4($fp)
    /* 800A9EEC 01F87021 */  addu $t6, $t7, $t8
    /* 800A9EF0 A1D90000 */  sb $t9, 0($t6)
    /* 800A9EF4 8FCF000C */  lw $t7, 12($fp)
    /* 800A9EF8 97D916A0 */  lhu $t9, 5792($fp)
    /* 800A9EFC 25F80001 */  addiu $t8, $t7, 1
    /* 800A9F00 8FCF0004 */  lw $t7, 4($fp)
    /* 800A9F04 00197203 */  sra $t6, $t9, 8
    /* 800A9F08 AFD8000C */  sw $t8, 12($fp)
    /* 800A9F0C 01F8C821 */  addu $t9, $t7, $t8
    /* 800A9F10 A32E0000 */  sb $t6, 0($t9)
    /* 800A9F14 8FCF000C */  lw $t7, 12($fp)
    /* 800A9F18 8FC416A4 */  lw $a0, 5796($fp)
    /* 800A9F1C 304EFFFF */  andi $t6, $v0, 0xFFFF
    /* 800A9F20 25F80001 */  addiu $t8, $t7, 1
    /* 800A9F24 AFD8000C */  sw $t8, 12($fp)
    /* 800A9F28 00E4C823 */  subu $t9, $a3, $a0
    /* 800A9F2C 032E7807 */  .word 0x032E7807
    /* 800A9F30 2498FFF3 */  addiu $t8, $a0, -13
    /* 800A9F34 AFD816A4 */  sw $t8, 5796($fp)
    /* 800A9F38 1000000B */  beq $zero, $zero, 0x800A9F68
    /* 800A9F3C A7CF16A0 */  sh $t7, 5792($fp)
    /* 800A9F40 90790000 */  lbu $t9, 0($v1)
    /* 800A9F44 97CE16A0 */  lhu $t6, 5792($fp)
    /* 800A9F48 00197880 */  sll $t7, $t9, 2
    /* 800A9F4C 03CFC021 */  addu $t8, $fp, $t7
    /* 800A9F50 97190A62 */  lhu $t9, 2658($t8)
    /* 800A9F54 00B97804 */  .word 0x00B97804
    /* 800A9F58 24B90003 */  addiu $t9, $a1, 3
    /* 800A9F5C 01CFC025 */  or $t8, $t6, $t7
    /* 800A9F60 A7D816A0 */  sh $t8, 5792($fp)
    /* 800A9F64 AFD916A4 */  sw $t9, 5796($fp)
    /* 800A9F68 14C8FFD0 */  bne $a2, $t0, 0x800A9EAC
    /* 800A9F6C 24630001 */  addiu $v1, $v1, 1
    /* 800A9F70 03C02025 */  or $a0, $fp, $zero
    /* 800A9F74 01603025 */  or $a2, $t3, $zero
    /* 800A9F78 2548FFFF */  addiu $t0, $t2, -1
    /* 800A9F7C AFA90044 */  sw $t1, 68($sp)
    /* 800A9F80 0C02A1DE */  jal 0x800A8778
    /* 800A9F84 AFAB0050 */  sw $t3, 80($sp)
    /* 800A9F88 8FA80044 */  lw $t0, 68($sp)
    /* 800A9F8C 03C02025 */  or $a0, $fp, $zero
    /* 800A9F90 0C02A1DE */  jal 0x800A8778
    /* 800A9F94 8FA60040 */  lw $a2, 64($sp)
    /* 800A9F98 03C02025 */  or $a0, $fp, $zero
    /* 800A9F9C 8FAD0050 */  lw $t5, 80($sp)
    /* 800A9FA0 8FB50040 */  lw $s5, 64($sp)
    /* 800A9FA4 0C02A0A1 */  jal 0x800A8284
    /* 800A9FA8 AFBE00B0 */  sw $fp, 176($sp)
    /* 800A9FAC 8FBE00B0 */  lw $fp, 176($sp)
    /* 800A9FB0 8FCE1694 */  lw $t6, 5780($fp)
    /* 800A9FB4 8FCF168C */  lw $t7, 5772($fp)
    /* 800A9FB8 01CFC021 */  addu $t8, $t6, $t7
    /* 800A9FBC 27190003 */  addiu $t9, $t8, 3
    /* 800A9FC0 AFD91694 */  sw $t9, 5780($fp)
    /* 800A9FC4 0C02A07F */  jal 0x800A81FC
    /* 800A9FC8 03C02025 */  or $a0, $fp, $zero
    /* 800A9FCC 8FAE00BC */  lw $t6, 188($sp)
    /* 800A9FD0 51C00007 */  .word 0x51C00007
    /* 800A9FD4 8FC21694 */  lw $v0, 5780($fp)
    /* 800A9FD8 0C02A05D */  jal 0x800A8174
    /* 800A9FDC 03C02025 */  or $a0, $fp, $zero
    /* 800A9FE0 8FCF1694 */  lw $t7, 5780($fp)
    /* 800A9FE4 25F80007 */  addiu $t8, $t7, 7
    /* 800A9FE8 AFD81694 */  sw $t8, 5780($fp)
    /* 800A9FEC 8FC21694 */  lw $v0, 5780($fp)
    /* 800A9FF0 8FBF003C */  lw $ra, 60($sp)
    /* 800A9FF4 8FBE0038 */  lw $fp, 56($sp)
    /* 800A9FF8 8FB00018 */  lw $s0, 24($sp)
    /* 800A9FFC 8FB1001C */  lw $s1, 28($sp)
    /* 800AA000 8FB20020 */  lw $s2, 32($sp)
    /* 800AA004 8FB30024 */  lw $s3, 36($sp)
    /* 800AA008 8FB40028 */  lw $s4, 40($sp)
    /* 800AA00C 8FB5002C */  lw $s5, 44($sp)
    /* 800AA010 8FB60030 */  lw $s6, 48($sp)
    /* 800AA014 8FB70034 */  lw $s7, 52($sp)
    /* 800AA018 0002C8C2 */  srl $t9, $v0, 3
    /* 800AA01C 03201025 */  or $v0, $t9, $zero
    /* 800AA020 03E00008 */  jr $ra
    /* 800AA024 27BD00B0 */  addiu $sp, $sp, 176
