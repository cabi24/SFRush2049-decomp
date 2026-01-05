# Source: game_code.bin (decompressed)
# Address: 0x800D1AB8

glabel car_color_handler
    /* 800D1AB8 27BDFFB8 */  addiu $sp, $sp, -72
    /* 800D1ABC 00003825 */  or $a3, $zero, $zero
    /* 800D1AC0 00001825 */  or $v1, $zero, $zero
    /* 800D1AC4 1900002D */  blez $t0, 0x800D1B7C
    /* 800D1AC8 00002025 */  or $a0, $zero, $zero
    /* 800D1ACC 3C108015 */  lui $s0, 0x8015
    /* 800D1AD0 3C0C8015 */  lui $t4, 0x8015
    /* 800D1AD4 3C058015 */  lui $a1, 0x8015
    /* 800D1AD8 24A52818 */  addiu $a1, $a1, 10264
    /* 800D1ADC 258CA250 */  addiu $t4, $t4, -23984
    /* 800D1AE0 26102734 */  addiu $s0, $s0, 10036
    /* 800D1AE4 24110001 */  addiu $s1, $zero, 1
    /* 800D1AE8 240D0808 */  addiu $t5, $zero, 2056
    /* 800D1AEC 240603B8 */  addiu $a2, $zero, 952
    /* 800D1AF0 27A20030 */  addiu $v0, $sp, 48
    /* 800D1AF4 008D0019 */  multu $a0, $t5
    /* 800D1AF8 24840001 */  addiu $a0, $a0, 1
    /* 800D1AFC 00007012 */  mflo $t6
    /* 800D1B00 018E4821 */  addu $t1, $t4, $t6
    /* 800D1B04 852A07C6 */  lh $t2, 1990($t1)
    /* 800D1B08 01460019 */  multu $t2, $a2
    /* 800D1B0C 00007812 */  mflo $t7
    /* 800D1B10 00AF5821 */  addu $t3, $a1, $t7
    /* 800D1B14 81780359 */  lb $t8, 857($t3)
    /* 800D1B18 5B00000B */  .word 0x5B00000B
    /* 800D1B1C 817900EF */  lb $t9, 239($t3)
    /* 800D1B20 813907E8 */  lb $t9, 2024($t1)
    /* 800D1B24 860E0000 */  lh $t6, 0($s0)
    /* 800D1B28 032E082A */  slt $at, $t9, $t6
    /* 800D1B2C 50200006 */  .word 0x50200006
    /* 800D1B30 817900EF */  lb $t9, 239($t3)
    /* 800D1B34 24E70001 */  addiu $a3, $a3, 1
    /* 800D1B38 00077C00 */  sll $t7, $a3, 16
    /* 800D1B3C 1000000A */  beq $zero, $zero, 0x800D1B68
    /* 800D1B40 000F3C03 */  sra $a3, $t7, 16
    /* 800D1B44 817900EF */  lb $t9, 239($t3)
    /* 800D1B48 00037040 */  sll $t6, $v1, 1
    /* 800D1B4C 004E7821 */  addu $t7, $v0, $t6
    /* 800D1B50 52390006 */  .word 0x52390006
    /* 800D1B54 00047400 */  sll $t6, $a0, 16
    /* 800D1B58 24630001 */  addiu $v1, $v1, 1
    /* 800D1B5C 0003C400 */  sll $t8, $v1, 16
    /* 800D1B60 00181C03 */  sra $v1, $t8, 16
    /* 800D1B64 A5EA0000 */  sh $t2, 0($t7)
    /* 800D1B68 00047400 */  sll $t6, $a0, 16
    /* 800D1B6C 000E2403 */  sra $a0, $t6, 16
    /* 800D1B70 0088082A */  slt $at, $a0, $t0
    /* 800D1B74 1420FFDF */  bne $at, $zero, 0x800D1AF4
    /* 800D1B78 00000000 */  nop
    /* 800D1B7C 3C058015 */  lui $a1, 0x8015
    /* 800D1B80 24A52818 */  addiu $a1, $a1, 10264
    /* 800D1B84 27A20030 */  addiu $v0, $sp, 48
    /* 800D1B88 10600053 */  beq $v1, $zero, 0x800D1CD8
    /* 800D1B8C 240603B8 */  addiu $a2, $zero, 952
    /* 800D1B90 18600014 */  blez $v1, 0x800D1BE4
    /* 800D1B94 00002025 */  or $a0, $zero, $zero
    /* 800D1B98 27B00018 */  addiu $s0, $sp, 24
    /* 800D1B9C 0004C040 */  sll $t8, $a0, 1
    /* 800D1BA0 0058C821 */  addu $t9, $v0, $t8
    /* 800D1BA4 87290000 */  lh $t1, 0($t9)
    /* 800D1BA8 24840001 */  addiu $a0, $a0, 1
    /* 800D1BAC 0004C400 */  sll $t8, $a0, 16
    /* 800D1BB0 01260019 */  multu $t1, $a2
    /* 800D1BB4 00182403 */  sra $a0, $t8, 16
    /* 800D1BB8 0083082A */  slt $at, $a0, $v1
    /* 800D1BBC 00007012 */  mflo $t6
    /* 800D1BC0 00AE7821 */  addu $t7, $a1, $t6
    /* 800D1BC4 C5E40100 */  lwc1 $f4, 256($t7)
    /* 800D1BC8 00097080 */  sll $t6, $t1, 2
    /* 800D1BCC 020E7821 */  addu $t7, $s0, $t6
    /* 800D1BD0 4600218D */  .word 0x4600218D
    /* 800D1BD4 44193000 */  .word 0x44193000
    /* 800D1BD8 1420FFF0 */  bne $at, $zero, 0x800D1B9C
    /* 800D1BDC ADF90000 */  sw $t9, 0($t7)
    /* 800D1BE0 00002025 */  or $a0, $zero, $zero
    /* 800D1BE4 1860003C */  blez $v1, 0x800D1CD8
    /* 800D1BE8 27B00018 */  addiu $s0, $sp, 24
    /* 800D1BEC 24890001 */  addiu $t1, $a0, 1
    /* 800D1BF0 00097C00 */  sll $t7, $t1, 16
    /* 800D1BF4 000FC403 */  sra $t8, $t7, 16
    /* 800D1BF8 00095400 */  sll $t2, $t1, 16
    /* 800D1BFC 000ACC03 */  sra $t9, $t2, 16
    /* 800D1C00 0303082A */  slt $at, $t8, $v1
    /* 800D1C04 00805825 */  or $t3, $a0, $zero
    /* 800D1C08 10200017 */  beq $at, $zero, 0x800D1C68
    /* 800D1C0C 03205025 */  or $t2, $t9, $zero
    /* 800D1C10 000A7040 */  sll $t6, $t2, 1
    /* 800D1C14 000B7840 */  sll $t7, $t3, 1
    /* 800D1C18 004EC821 */  addu $t9, $v0, $t6
    /* 800D1C1C 004FC021 */  addu $t8, $v0, $t7
    /* 800D1C20 870E0000 */  lh $t6, 0($t8)
    /* 800D1C24 872C0000 */  lh $t4, 0($t9)
    /* 800D1C28 000EC880 */  sll $t9, $t6, 2
    /* 800D1C2C 02197821 */  addu $t7, $s0, $t9
    /* 800D1C30 000C7080 */  sll $t6, $t4, 2
    /* 800D1C34 8DF80000 */  lw $t8, 0($t7)
    /* 800D1C38 020EC821 */  addu $t9, $s0, $t6
    /* 800D1C3C 8F2F0000 */  lw $t7, 0($t9)
    /* 800D1C40 030F082A */  slt $at, $t8, $t7
    /* 800D1C44 50200003 */  .word 0x50200003
    /* 800D1C48 254A0001 */  addiu $t2, $t2, 1
    /* 800D1C4C 01405825 */  or $t3, $t2, $zero
    /* 800D1C50 254A0001 */  addiu $t2, $t2, 1
    /* 800D1C54 000A7400 */  sll $t6, $t2, 16
    /* 800D1C58 000E5403 */  sra $t2, $t6, 16
    /* 800D1C5C 0143082A */  slt $at, $t2, $v1
    /* 800D1C60 5420FFEC */  .word 0x5420FFEC
    /* 800D1C64 000A7040 */  sll $t6, $t2, 1
    /* 800D1C68 000BC040 */  sll $t8, $t3, 1
    /* 800D1C6C 00586021 */  addu $t4, $v0, $t8
    /* 800D1C70 858D0000 */  lh $t5, 0($t4)
    /* 800D1C74 00887821 */  addu $t7, $a0, $t0
    /* 800D1C78 01E37023 */  subu $t6, $t7, $v1
    /* 800D1C7C 01A60019 */  multu $t5, $a2
    /* 800D1C80 01C7C823 */  subu $t9, $t6, $a3
    /* 800D1C84 00047040 */  sll $t6, $a0, 1
    /* 800D1C88 004E5021 */  addu $t2, $v0, $t6
    /* 800D1C8C 00092400 */  sll $a0, $t1, 16
    /* 800D1C90 0000C012 */  mflo $t8
    /* 800D1C94 00B87821 */  addu $t7, $a1, $t8
    /* 800D1C98 A1F900EE */  sb $t9, 238($t7)
    /* 800D1C9C 85580000 */  lh $t8, 0($t2)
    /* 800D1CA0 01B8C826 */  xor $t9, $t5, $t8
    /* 800D1CA4 A5990000 */  sh $t9, 0($t4)
    /* 800D1CA8 858E0000 */  lh $t6, 0($t4)
    /* 800D1CAC 854F0000 */  lh $t7, 0($t2)
    /* 800D1CB0 01EEC026 */  xor $t8, $t7, $t6
    /* 800D1CB4 A5580000 */  sh $t8, 0($t2)
    /* 800D1CB8 85990000 */  lh $t9, 0($t4)
    /* 800D1CBC 854F0000 */  lh $t7, 0($t2)
    /* 800D1CC0 0004C403 */  sra $t8, $a0, 16
    /* 800D1CC4 0303082A */  slt $at, $t8, $v1
    /* 800D1CC8 032F7026 */  xor $t6, $t9, $t7
    /* 800D1CCC 03002025 */  or $a0, $t8, $zero
    /* 800D1CD0 1420FFC6 */  bne $at, $zero, 0x800D1BEC
    /* 800D1CD4 A58E0000 */  sh $t6, 0($t4)
    /* 800D1CD8 03E00008 */  jr $ra
    /* 800D1CDC 27BD0048 */  addiu $sp, $sp, 72
