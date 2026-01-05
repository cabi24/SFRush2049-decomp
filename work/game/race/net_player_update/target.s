# Source: game_code.bin (decompressed)
# Address: 0x800F1944

glabel net_player_update
    /* 800F1944 1203000E */  beq $s0, $v1, 0x800F1980
    /* 800F1948 AFBF0014 */  sw $ra, 20($sp)
    /* 800F194C 24050002 */  addiu $a1, $zero, 2
    /* 800F1950 14A30023 */  bne $a1, $v1, 0x800F19E0
    /* 800F1954 3C118014 */  lui $s1, 0x8014
    /* 800F1958 263160C0 */  addiu $s1, $s1, 24768
    /* 800F195C 822E0000 */  lb $t6, 0($s1)
    /* 800F1960 3C068014 */  lui $a2, 0x8014
    /* 800F1964 24C661C0 */  addiu $a2, $a2, 25024
    /* 800F1968 000E7880 */  sll $t7, $t6, 2
    /* 800F196C 00CFC021 */  addu $t8, $a2, $t7
    /* 800F1970 8F020000 */  lw $v0, 0($t8)
    /* 800F1974 1040001A */  beq $v0, $zero, 0x800F19E0
    /* 800F1978 00000000 */  nop
    /* 800F197C 12020018 */  beq $s0, $v0, 0x800F19E0
    /* 800F1980 2479FFFF */  addiu $t9, $v1, -1
    /* 800F1984 3C068014 */  lui $a2, 0x8014
    /* 800F1988 3C118014 */  lui $s1, 0x8014
    /* 800F198C 263160C0 */  addiu $s1, $s1, 24768
    /* 800F1990 24C661C0 */  addiu $a2, $a2, 25024
    /* 800F1994 24050002 */  addiu $a1, $zero, 2
    /* 800F1998 24120003 */  addiu $s2, $zero, 3
    /* 800F199C AC990000 */  sw $t9, 0($a0)
    /* 800F19A0 07210003 */  bgez $t9, 0x800F19B0
    /* 800F19A4 03201825 */  or $v1, $t9, $zero
    /* 800F19A8 AC920000 */  sw $s2, 0($a0)
    /* 800F19AC 02401825 */  or $v1, $s2, $zero
    /* 800F19B0 5203FFF4 */  .word 0x5203FFF4
    /* 800F19B4 2479FFFF */  addiu $t9, $v1, -1
    /* 800F19B8 14A30009 */  bne $a1, $v1, 0x800F19E0
    /* 800F19BC 00000000 */  nop
    /* 800F19C0 822E0000 */  lb $t6, 0($s1)
    /* 800F19C4 000E7880 */  sll $t7, $t6, 2
    /* 800F19C8 00CFC021 */  addu $t8, $a2, $t7
    /* 800F19CC 8F020000 */  lw $v0, 0($t8)
    /* 800F19D0 10400003 */  beq $v0, $zero, 0x800F19E0
    /* 800F19D4 00000000 */  nop
    /* 800F19D8 5602FFEA */  .word 0x5602FFEA
    /* 800F19DC 2479FFFF */  addiu $t9, $v1, -1
    /* 800F19E0 3C158014 */  lui $s5, 0x8014
    /* 800F19E4 26B560FC */  addiu $s5, $s5, 24828
    /* 800F19E8 82A70000 */  lb $a3, 0($s5)
    /* 800F19EC 3C068014 */  lui $a2, 0x8014
    /* 800F19F0 3C118014 */  lui $s1, 0x8014
    /* 800F19F4 263160C0 */  addiu $s1, $s1, 24768
    /* 800F19F8 24C661C0 */  addiu $a2, $a2, 25024
    /* 800F19FC 16070011 */  bne $s0, $a3, 0x800F1A44
    /* 800F1A00 24120003 */  addiu $s2, $zero, 3
    /* 800F1A04 3C198015 */  lui $t9, 0x8015
    /* 800F1A08 8F396944 */  lw $t9, 26948($t9)
    /* 800F1A0C 3C0F8015 */  lui $t7, 0x8015
    /* 800F1A10 332E3000 */  andi $t6, $t9, 0x3000
    /* 800F1A14 11C0000B */  beq $t6, $zero, 0x800F1A44
    /* 800F1A18 00000000 */  nop
    /* 800F1A1C 8DEF694C */  lw $t7, 26956($t7)
    /* 800F1A20 3C048014 */  lui $a0, 0x8014
    /* 800F1A24 24846131 */  addiu $a0, $a0, 24881
    /* 800F1A28 31F83000 */  andi $t8, $t7, 0x3000
    /* 800F1A2C 130000AF */  beq $t8, $zero, 0x800F1CEC
    /* 800F1A30 00000000 */  nop
    /* 800F1A34 80820000 */  lb $v0, 0($a0)
    /* 800F1A38 2C590001 */  sltiu $t9, $v0, 1
    /* 800F1A3C 100000AB */  beq $zero, $zero, 0x800F1CEC
    /* 800F1A40 A0990000 */  sb $t9, 0($a0)
    /* 800F1A44 1460001E */  bne $v1, $zero, 0x800F1AC0
    /* 800F1A48 3C0E8015 */  lui $t6, 0x8015
    /* 800F1A4C 8DCE6944 */  lw $t6, 26948($t6)
    /* 800F1A50 3C028015 */  lui $v0, 0x8015
    /* 800F1A54 31CF3000 */  andi $t7, $t6, 0x3000
    /* 800F1A58 11E00019 */  beq $t7, $zero, 0x800F1AC0
    /* 800F1A5C 00000000 */  nop
    /* 800F1A60 8C42694C */  lw $v0, 26956($v0)
    /* 800F1A64 30581000 */  andi $t8, $v0, 0x1000
    /* 800F1A68 5300000A */  .word 0x5300000A
    /* 800F1A6C 30582000 */  andi $t8, $v0, 0x2000
    /* 800F1A70 82390000 */  lb $t9, 0($s1)
    /* 800F1A74 272EFFFF */  addiu $t6, $t9, -1
    /* 800F1A78 A22E0000 */  sb $t6, 0($s1)
    /* 800F1A7C 822F0000 */  lb $t7, 0($s1)
    /* 800F1A80 05E1009A */  bgez $t7, 0x800F1CEC
    /* 800F1A84 00000000 */  nop
    /* 800F1A88 10000098 */  beq $zero, $zero, 0x800F1CEC
    /* 800F1A8C A2320000 */  sb $s2, 0($s1)
    /* 800F1A90 30582000 */  andi $t8, $v0, 0x2000
    /* 800F1A94 13000095 */  beq $t8, $zero, 0x800F1CEC
    /* 800F1A98 00000000 */  nop
    /* 800F1A9C 82390000 */  lb $t9, 0($s1)
    /* 800F1AA0 272E0001 */  addiu $t6, $t9, 1
    /* 800F1AA4 A22E0000 */  sb $t6, 0($s1)
    /* 800F1AA8 822F0000 */  lb $t7, 0($s1)
    /* 800F1AAC 29E10004 */  slti $at, $t7, 4
    /* 800F1AB0 1420008E */  bne $at, $zero, 0x800F1CEC
    /* 800F1AB4 00000000 */  nop
    /* 800F1AB8 1000008C */  beq $zero, $zero, 0x800F1CEC
    /* 800F1ABC A2200000 */  sb $zero, 0($s1)
    /* 800F1AC0 3C028015 */  lui $v0, 0x8015
    /* 800F1AC4 8C42694C */  lw $v0, 26956($v0)
    /* 800F1AC8 30580C00 */  andi $t8, $v0, 0x0C00
    /* 800F1ACC 1300002E */  beq $t8, $zero, 0x800F1B88
    /* 800F1AD0 304F0003 */  andi $t7, $v0, 0x0003
    /* 800F1AD4 30590400 */  andi $t9, $v0, 0x0400
    /* 800F1AD8 13200015 */  beq $t9, $zero, 0x800F1B30
    /* 800F1ADC 304E0800 */  andi $t6, $v0, 0x0800
    /* 800F1AE0 246EFFFF */  addiu $t6, $v1, -1
    /* 800F1AE4 AC8E0000 */  sw $t6, 0($a0)
    /* 800F1AE8 05C10003 */  bgez $t6, 0x800F1AF8
    /* 800F1AEC 01C01825 */  or $v1, $t6, $zero
    /* 800F1AF0 AC920000 */  sw $s2, 0($a0)
    /* 800F1AF4 02401825 */  or $v1, $s2, $zero
    /* 800F1AF8 5203FFFA */  .word 0x5203FFFA
    /* 800F1AFC 246EFFFF */  addiu $t6, $v1, -1
    /* 800F1B00 14A3007A */  bne $a1, $v1, 0x800F1CEC
    /* 800F1B04 00000000 */  nop
    /* 800F1B08 822F0000 */  lb $t7, 0($s1)
    /* 800F1B0C 000FC080 */  sll $t8, $t7, 2
    /* 800F1B10 00D8C821 */  addu $t9, $a2, $t8
    /* 800F1B14 8F220000 */  lw $v0, 0($t9)
    /* 800F1B18 10400074 */  beq $v0, $zero, 0x800F1CEC
    /* 800F1B1C 00000000 */  nop
    /* 800F1B20 5602FFF0 */  .word 0x5602FFF0
    /* 800F1B24 246EFFFF */  addiu $t6, $v1, -1
    /* 800F1B28 10000070 */  beq $zero, $zero, 0x800F1CEC
    /* 800F1B2C 00000000 */  nop
    /* 800F1B30 11C0006E */  beq $t6, $zero, 0x800F1CEC
    /* 800F1B34 246F0001 */  addiu $t7, $v1, 1
    /* 800F1B38 29E10004 */  slti $at, $t7, 4
    /* 800F1B3C AC8F0000 */  sw $t7, 0($a0)
    /* 800F1B40 14200003 */  bne $at, $zero, 0x800F1B50
    /* 800F1B44 01E01825 */  or $v1, $t7, $zero
    /* 800F1B48 AC800000 */  sw $zero, 0($a0)
    /* 800F1B4C 00001825 */  or $v1, $zero, $zero
    /* 800F1B50 5203FFF9 */  .word 0x5203FFF9
    /* 800F1B54 246F0001 */  addiu $t7, $v1, 1
    /* 800F1B58 14A30064 */  bne $a1, $v1, 0x800F1CEC
    /* 800F1B5C 00000000 */  nop
    /* 800F1B60 82380000 */  lb $t8, 0($s1)
    /* 800F1B64 0018C880 */  sll $t9, $t8, 2
    /* 800F1B68 00D97021 */  addu $t6, $a2, $t9
    /* 800F1B6C 8DC20000 */  lw $v0, 0($t6)
    /* 800F1B70 1040005E */  beq $v0, $zero, 0x800F1CEC
    /* 800F1B74 00000000 */  nop
    /* 800F1B78 5602FFEF */  .word 0x5602FFEF
    /* 800F1B7C 246F0001 */  addiu $t7, $v1, 1
    /* 800F1B80 1000005A */  beq $zero, $zero, 0x800F1CEC
    /* 800F1B84 00000000 */  nop
    /* 800F1B88 11E00058 */  beq $t7, $zero, 0x800F1CEC
    /* 800F1B8C 3C088014 */  lui $t0, 0x8014
    /* 800F1B90 250860F0 */  addiu $t0, $t0, 24816
    /* 800F1B94 81180000 */  lb $t8, 0($t0)
    /* 800F1B98 0000A025 */  or $s4, $zero, $zero
    /* 800F1B9C 16180003 */  bne $s0, $t8, 0x800F1BAC
    /* 800F1BA0 00000000 */  nop
    /* 800F1BA4 10000040 */  beq $zero, $zero, 0x800F1CA8
    /* 800F1BA8 A1000000 */  sb $zero, 0($t0)
    /* 800F1BAC 16070016 */  bne $s0, $a3, 0x800F1C08
    /* 800F1BB0 30590002 */  andi $t9, $v0, 0x0002
    /* 800F1BB4 13200010 */  beq $t9, $zero, 0x800F1BF8
    /* 800F1BB8 3C048014 */  lui $a0, 0x8014
    /* 800F1BBC 24846131 */  addiu $a0, $a0, 24881
    /* 800F1BC0 808E0000 */  lb $t6, 0($a0)
    /* 800F1BC4 3C058015 */  lui $a1, 0x8015
    /* 800F1BC8 3C068014 */  lui $a2, 0x8014
    /* 800F1BCC 160E000A */  bne $s0, $t6, 0x800F1BF8
    /* 800F1BD0 00003825 */  or $a3, $zero, $zero
    /* 800F1BD4 82240000 */  lb $a0, 0($s1)
    /* 800F1BD8 8CA5A160 */  lw $a1, -24224($a1)
    /* 800F1BDC 0C033010 */  jal 0x800CC040
    /* 800F1BE0 8CC661A8 */  lw $a2, 25000($a2)
    /* 800F1BE4 50400005 */  .word 0x50400005
    /* 800F1BE8 A2A00000 */  sb $zero, 0($s5)
    /* 800F1BEC 3C138014 */  lui $s3, 0x8014
    /* 800F1BF0 26736148 */  addiu $s3, $s3, 24904
    /* 800F1BF4 A2700000 */  sb $s0, 0($s3)
    /* 800F1BF8 A2A00000 */  sb $zero, 0($s5)
    /* 800F1BFC 3C038014 */  lui $v1, 0x8014
    /* 800F1C00 10000029 */  beq $zero, $zero, 0x800F1CA8
    /* 800F1C04 8C634DA0 */  lw $v1, 19872($v1)
    /* 800F1C08 3C138014 */  lui $s3, 0x8014
    /* 800F1C0C 26736148 */  addiu $s3, $s3, 24904
    /* 800F1C10 826F0000 */  lb $t7, 0($s3)
    /* 800F1C14 30580002 */  andi $t8, $v0, 0x0002
    /* 800F1C18 160F0004 */  bne $s0, $t7, 0x800F1C2C
    /* 800F1C1C 00000000 */  nop
    /* 800F1C20 A2600000 */  sb $zero, 0($s3)
    /* 800F1C24 10000020 */  beq $zero, $zero, 0x800F1CA8
    /* 800F1C28 0200A025 */  or $s4, $s0, $zero
    /* 800F1C2C 1300001E */  beq $t8, $zero, 0x800F1CA8
    /* 800F1C30 00000000 */  nop
    /* 800F1C34 14A3001C */  bne $a1, $v1, 0x800F1CA8
    /* 800F1C38 00000000 */  nop
    /* 800F1C3C 82240000 */  lb $a0, 0($s1)
    /* 800F1C40 00003825 */  or $a3, $zero, $zero
    /* 800F1C44 0004C880 */  sll $t9, $a0, 2
    /* 800F1C48 00D97021 */  addu $t6, $a2, $t9
    /* 800F1C4C 8DC20000 */  lw $v0, 0($t6)
    /* 800F1C50 3C068014 */  lui $a2, 0x8014
    /* 800F1C54 1440000A */  bne $v0, $zero, 0x800F1C80
    /* 800F1C58 00000000 */  nop
    /* 800F1C5C 3C058015 */  lui $a1, 0x8015
    /* 800F1C60 8CA5A160 */  lw $a1, -24224($a1)
    /* 800F1C64 0C033010 */  jal 0x800CC040
    /* 800F1C68 8CC661A8 */  lw $a2, 25000($a2)
    /* 800F1C6C 10400002 */  beq $v0, $zero, 0x800F1C78
    /* 800F1C70 3C038014 */  lui $v1, 0x8014
    /* 800F1C74 A2700000 */  sb $s0, 0($s3)
    /* 800F1C78 1000000B */  beq $zero, $zero, 0x800F1CA8
    /* 800F1C7C 8C634DA0 */  lw $v1, 19872($v1)
    /* 800F1C80 16020006 */  bne $s0, $v0, 0x800F1C9C
    /* 800F1C84 3C048014 */  lui $a0, 0x8014
    /* 800F1C88 24846131 */  addiu $a0, $a0, 24881
    /* 800F1C8C A2B00000 */  sb $s0, 0($s5)
    /* 800F1C90 A0800000 */  sb $zero, 0($a0)
    /* 800F1C94 10000004 */  beq $zero, $zero, 0x800F1CA8
    /* 800F1C98 A2600000 */  sb $zero, 0($s3)
    /* 800F1C9C 14A20002 */  bne $a1, $v0, 0x800F1CA8
    /* 800F1CA0 00000000 */  nop
    /* 800F1CA4 A1100000 */  sb $s0, 0($t0)
    /* 800F1CA8 1643000A */  bne $s2, $v1, 0x800F1CD4
    /* 800F1CAC 3C058015 */  lui $a1, 0x8015
    /* 800F1CB0 3C068014 */  lui $a2, 0x8014
    /* 800F1CB4 8CC661A8 */  lw $a2, 25000($a2)
    /* 800F1CB8 8CA5A160 */  lw $a1, -24224($a1)
    /* 800F1CBC 82240000 */  lb $a0, 0($s1)
    /* 800F1CC0 02003825 */  or $a3, $s0, $zero
    /* 800F1CC4 0C033010 */  jal 0x800CC040
    /* 800F1CC8 24140001 */  addiu $s4, $zero, 1
    /* 800F1CCC 3C018014 */  lui $at, 0x8014
    /* 800F1CD0 AC2061A8 */  sw $zero, 25000($at)
    /* 800F1CD4 16900005 */  bne $s4, $s0, 0x800F1CEC
    /* 800F1CD8 00000000 */  nop
    /* 800F1CDC 0C03C484 */  jal 0x800F1210
    /* 800F1CE0 24120005 */  addiu $s2, $zero, 5
    /* 800F1CE4 3C118014 */  lui $s1, 0x8014
    /* 800F1CE8 263160C0 */  addiu $s1, $s1, 24768
    /* 800F1CEC 0C03C3D1 */  jal 0x800F0F44
    /* 800F1CF0 823E0000 */  lb $fp, 0($s1)
    /* 800F1CF4 8FBF0014 */  lw $ra, 20($sp)
    /* 800F1CF8 27BD0018 */  addiu $sp, $sp, 24
    /* 800F1CFC 03E00008 */  jr $ra
    /* 800F1D00 00000000 */  nop
