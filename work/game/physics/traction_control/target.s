# Source: game_code.bin (decompressed)
# Address: 0x800ACC18

glabel traction_control
    /* 800ACC18 27BDFF20 */  addiu $sp, $sp, -224
    /* 800ACC1C 3C0E8015 */  lui $t6, 0x8015
    /* 800ACC20 8DCE26F0 */  lw $t6, 9968($t6)
    /* 800ACC24 AFBF002C */  sw $ra, 44($sp)
    /* 800ACC28 AFB10028 */  sw $s1, 40($sp)
    /* 800ACC2C AFB00024 */  sw $s0, 36($sp)
    /* 800ACC30 F7B60018 */  .word 0xF7B60018
    /* 800ACC34 F7B40010 */  .word 0xF7B40010
    /* 800ACC38 AFA400E0 */  sw $a0, 224($sp)
    /* 800ACC3C AFA500E4 */  sw $a1, 228($sp)
    /* 800ACC40 AFA600E8 */  sw $a2, 232($sp)
    /* 800ACC44 AFA700EC */  sw $a3, 236($sp)
    /* 800ACC48 95CF0000 */  lhu $t7, 0($t6)
    /* 800ACC4C 30A4FFFF */  andi $a0, $a1, 0xFFFF
    /* 800ACC50 24820001 */  addiu $v0, $a0, 1
    /* 800ACC54 004F082A */  slt $at, $v0, $t7
    /* 800ACC58 10200003 */  beq $at, $zero, 0x800ACC68
    /* 800ACC5C 0004C140 */  sll $t8, $a0, 5
    /* 800ACC60 10000002 */  beq $zero, $zero, 0x800ACC6C
    /* 800ACC64 00401825 */  or $v1, $v0, $zero
    /* 800ACC68 00001825 */  or $v1, $zero, $zero
    /* 800ACC6C 3C028015 */  lui $v0, 0x8015
    /* 800ACC70 8C422034 */  lw $v0, 8244($v0)
    /* 800ACC74 0304C021 */  addu $t8, $t8, $a0
    /* 800ACC78 0003C940 */  sll $t9, $v1, 5
    /* 800ACC7C 0018C080 */  sll $t8, $t8, 2
    /* 800ACC80 0323C821 */  addu $t9, $t9, $v1
    /* 800ACC84 0019C880 */  sll $t9, $t9, 2
    /* 800ACC88 03028021 */  addu $s0, $t8, $v0
    /* 800ACC8C 02004025 */  or $t0, $s0, $zero
    /* 800ACC90 2606000C */  addiu $a2, $s0, 12
    /* 800ACC94 8FA700E8 */  lw $a3, 232($sp)
    /* 800ACC98 27A50044 */  addiu $a1, $sp, 68
    /* 800ACC9C 0C02B2DD */  jal 0x800ACB74
    /* 800ACCA0 03228821 */  addu $s1, $t9, $v0
    /* 800ACCA4 C600005C */  lwc1 $f0, 92($s0)
    /* 800ACCA8 C6060060 */  lwc1 $f6, 96($s0)
    /* 800ACCAC C7A40044 */  lwc1 $f4, 68($sp)
    /* 800ACCB0 2604003C */  addiu $a0, $s0, 60
    /* 800ACCB4 46003201 */  .word 0x46003201
    /* 800ACCB8 C6060058 */  lwc1 $f6, 88($s0)
    /* 800ACCBC 26050048 */  addiu $a1, $s0, 72
    /* 800ACCC0 27A700BC */  addiu $a3, $sp, 188
    /* 800ACCC4 46082282 */  .word 0x46082282
    /* 800ACCC8 C7A8004C */  lwc1 $f8, 76($sp)
    /* 800ACCCC 46065080 */  .word 0x46065080
    /* 800ACCD0 46002282 */  .word 0x46002282
    /* 800ACCD4 460A4181 */  .word 0x460A4181
    /* 800ACCD8 44805000 */  .word 0x44805000
    /* 800ACCDC 00000000 */  nop
    /* 800ACCE0 4604503C */  .word 0x4604503C
    /* 800ACCE4 46023583 */  .word 0x46023583
    /* 800ACCE8 4502000C */  .word 0x4502000C
    /* 800ACCEC C7A4004C */  lwc1 $f4, 76($sp)
    /* 800ACCF0 C6060070 */  lwc1 $f6, 112($s0)
    /* 800ACCF4 3C013F00 */  lui $at, 0x3F00
    /* 800ACCF8 44810000 */  .word 0x44810000
    /* 800ACCFC 46064282 */  .word 0x46064282
    /* 800ACD00 C608006C */  lwc1 $f8, 108($s0)
    /* 800ACD04 46085180 */  .word 0x46085180
    /* 800ACD08 46062283 */  .word 0x46062283
    /* 800ACD0C 46005202 */  .word 0x46005202
    /* 800ACD10 1000000E */  beq $zero, $zero, 0x800ACD4C
    /* 800ACD14 46004500 */  .word 0x46004500
    /* 800ACD18 C7A4004C */  lwc1 $f4, 76($sp)
    /* 800ACD1C C6060068 */  lwc1 $f6, 104($s0)
    /* 800ACD20 C6080064 */  lwc1 $f8, 100($s0)
    /* 800ACD24 3C013F00 */  lui $at, 0x3F00
    /* 800ACD28 46062282 */  .word 0x46062282
    /* 800ACD2C C7A60044 */  lwc1 $f6, 68($sp)
    /* 800ACD30 44810000 */  .word 0x44810000
    /* 800ACD34 3C01BF00 */  lui $at, 0xBF00
    /* 800ACD38 46085100 */  .word 0x46085100
    /* 800ACD3C 44814000 */  .word 0x44814000
    /* 800ACD40 46043283 */  .word 0x46043283
    /* 800ACD44 46085182 */  .word 0x46085182
    /* 800ACD48 46003500 */  .word 0x46003500
    /* 800ACD4C 4406A000 */  .word 0x4406A000
    /* 800ACD50 0C02B2F1 */  jal 0x800ACBC4
    /* 800ACD54 E7A20058 */  swc1 $f2, 88($sp)
    /* 800ACD58 4406A000 */  .word 0x4406A000
    /* 800ACD5C 2624003C */  addiu $a0, $s1, 60
    /* 800ACD60 26250048 */  addiu $a1, $s1, 72
    /* 800ACD64 0C02B2F1 */  jal 0x800ACBC4
    /* 800ACD68 27A700B0 */  addiu $a3, $sp, 176
    /* 800ACD6C 4406B000 */  .word 0x4406B000
    /* 800ACD70 27A400BC */  addiu $a0, $sp, 188
    /* 800ACD74 27A500B0 */  addiu $a1, $sp, 176
    /* 800ACD78 0C02B2F1 */  jal 0x800ACBC4
    /* 800ACD7C 27A7008C */  addiu $a3, $sp, 140
    /* 800ACD80 4406B000 */  .word 0x4406B000
    /* 800ACD84 26040030 */  addiu $a0, $s0, 48
    /* 800ACD88 26250030 */  addiu $a1, $s1, 48
    /* 800ACD8C 0C02B2F1 */  jal 0x800ACBC4
    /* 800ACD90 27A700A4 */  addiu $a3, $sp, 164
    /* 800ACD94 C7A00090 */  lwc1 $f0, 144($sp)
    /* 800ACD98 C7A200AC */  lwc1 $f2, 172($sp)
    /* 800ACD9C C7AC00A8 */  lwc1 $f12, 168($sp)
    /* 800ACDA0 C7AE0094 */  lwc1 $f14, 148($sp)
    /* 800ACDA4 46020102 */  .word 0x46020102
    /* 800ACDA8 C7B000A4 */  lwc1 $f16, 164($sp)
    /* 800ACDAC C7B2008C */  lwc1 $f18, 140($sp)
    /* 800ACDB0 460E6282 */  .word 0x460E6282
    /* 800ACDB4 8FA200F0 */  lw $v0, 240($sp)
    /* 800ACDB8 27A400A4 */  addiu $a0, $sp, 164
    /* 800ACDBC 46107182 */  .word 0x46107182
    /* 800ACDC0 27A50098 */  addiu $a1, $sp, 152
    /* 800ACDC4 27A3008C */  addiu $v1, $sp, 140
    /* 800ACDC8 27A60098 */  addiu $a2, $sp, 152
    /* 800ACDCC 460A2201 */  .word 0x460A2201
    /* 800ACDD0 46121102 */  .word 0x46121102
    /* 800ACDD4 E7A80098 */  swc1 $f8, 152($sp)
    /* 800ACDD8 460C9202 */  .word 0x460C9202
    /* 800ACDDC 46043281 */  .word 0x46043281
    /* 800ACDE0 46008182 */  .word 0x46008182
    /* 800ACDE4 E7AA009C */  swc1 $f10, 156($sp)
    /* 800ACDE8 46064101 */  .word 0x46064101
    /* 800ACDEC E7A400A0 */  swc1 $f4, 160($sp)
    /* 800ACDF0 C48A0000 */  lwc1 $f10, 0($a0)
    /* 800ACDF4 24630004 */  addiu $v1, $v1, 4
    /* 800ACDF8 0066082B */  sltu $at, $v1, $a2
    /* 800ACDFC E44A0000 */  swc1 $f10, 0($v0)
    /* 800ACE00 C4A80000 */  lwc1 $f8, 0($a1)
    /* 800ACE04 24420004 */  addiu $v0, $v0, 4
    /* 800ACE08 24840004 */  addiu $a0, $a0, 4
    /* 800ACE0C E4480008 */  swc1 $f8, 8($v0)
    /* 800ACE10 C466FFFC */  lwc1 $f6, -4($v1)
    /* 800ACE14 24A50004 */  addiu $a1, $a1, 4
    /* 800ACE18 1420FFF5 */  bne $at, $zero, 0x800ACDF0
    /* 800ACE1C E4460014 */  swc1 $f6, 20($v0)
    /* 800ACE20 3C013F00 */  lui $at, 0x3F00
    /* 800ACE24 44812000 */  .word 0x44812000
    /* 800ACE28 27A20080 */  addiu $v0, $sp, 128
    /* 800ACE2C 02001825 */  or $v1, $s0, $zero
    /* 800ACE30 27A4008C */  addiu $a0, $sp, 140
    /* 800ACE34 4604A001 */  .word 0x4604A001
    /* 800ACE38 C60C0054 */  lwc1 $f12, 84($s0)
    /* 800ACE3C 24420004 */  addiu $v0, $v0, 4
    /* 800ACE40 0044082B */  sltu $at, $v0, $a0
    /* 800ACE44 460C0482 */  .word 0x460C0482
    /* 800ACE48 1020000D */  beq $at, $zero, 0x800ACE80
    /* 800ACE4C C4700030 */  lwc1 $f16, 48($v1)
    /* 800ACE50 46128382 */  .word 0x46128382
    /* 800ACE54 C46C0000 */  lwc1 $f12, 0($v1)
    /* 800ACE58 24420004 */  addiu $v0, $v0, 4
    /* 800ACE5C 0044082B */  sltu $at, $v0, $a0
    /* 800ACE60 24630004 */  addiu $v1, $v1, 4
    /* 800ACE64 460C7300 */  .word 0x460C7300
    /* 800ACE68 E44CFFF8 */  swc1 $f12, -8($v0)
    /* 800ACE6C C60C0054 */  lwc1 $f12, 84($s0)
    /* 800ACE70 C4700030 */  lwc1 $f16, 48($v1)
    /* 800ACE74 460C0482 */  .word 0x460C0482
    /* 800ACE78 1420FFF5 */  bne $at, $zero, 0x800ACE50
    /* 800ACE7C 00000000 */  nop
    /* 800ACE80 46128382 */  .word 0x46128382
    /* 800ACE84 C46C0000 */  lwc1 $f12, 0($v1)
    /* 800ACE88 24630004 */  addiu $v1, $v1, 4
    /* 800ACE8C 460C7300 */  .word 0x460C7300
    /* 800ACE90 E44CFFFC */  swc1 $f12, -4($v0)
    /* 800ACE94 27A20074 */  addiu $v0, $sp, 116
    /* 800ACE98 02201825 */  or $v1, $s1, $zero
    /* 800ACE9C 27A40080 */  addiu $a0, $sp, 128
    /* 800ACEA0 C62C0054 */  lwc1 $f12, 84($s1)
    /* 800ACEA4 24420004 */  addiu $v0, $v0, 4
    /* 800ACEA8 0044082B */  sltu $at, $v0, $a0
    /* 800ACEAC 460C0402 */  .word 0x460C0402
    /* 800ACEB0 1020000D */  beq $at, $zero, 0x800ACEE8
    /* 800ACEB4 C4720030 */  lwc1 $f18, 48($v1)
    /* 800ACEB8 46109382 */  .word 0x46109382
    /* 800ACEBC C46C0000 */  lwc1 $f12, 0($v1)
    /* 800ACEC0 24420004 */  addiu $v0, $v0, 4
    /* 800ACEC4 0044082B */  sltu $at, $v0, $a0
    /* 800ACEC8 24630004 */  addiu $v1, $v1, 4
    /* 800ACECC 460C7300 */  .word 0x460C7300
    /* 800ACED0 E44CFFF8 */  swc1 $f12, -8($v0)
    /* 800ACED4 C62C0054 */  lwc1 $f12, 84($s1)
    /* 800ACED8 C4720030 */  lwc1 $f18, 48($v1)
    /* 800ACEDC 460C0402 */  .word 0x460C0402
    /* 800ACEE0 1420FFF5 */  bne $at, $zero, 0x800ACEB8
    /* 800ACEE4 00000000 */  nop
    /* 800ACEE8 46109382 */  .word 0x46109382
    /* 800ACEEC C46C0000 */  lwc1 $f12, 0($v1)
    /* 800ACEF0 24630004 */  addiu $v1, $v1, 4
    /* 800ACEF4 460C7300 */  .word 0x460C7300
    /* 800ACEF8 E44CFFFC */  swc1 $f12, -4($v0)
    /* 800ACEFC 4406B000 */  .word 0x4406B000
    /* 800ACF00 27A40080 */  addiu $a0, $sp, 128
    /* 800ACF04 27A50074 */  addiu $a1, $sp, 116
    /* 800ACF08 0C02B2F1 */  jal 0x800ACBC4
    /* 800ACF0C 27A70068 */  addiu $a3, $sp, 104
    /* 800ACF10 C60C0074 */  lwc1 $f12, 116($s0)
    /* 800ACF14 C6080078 */  lwc1 $f8, 120($s0)
    /* 800ACF18 C6300074 */  lwc1 $f16, 116($s1)
    /* 800ACF1C C6260078 */  lwc1 $f6, 120($s1)
    /* 800ACF20 460C4281 */  .word 0x460C4281
    /* 800ACF24 8FA900F0 */  lw $t1, 240($sp)
    /* 800ACF28 27A30068 */  addiu $v1, $sp, 104
    /* 800ACF2C 46103201 */  .word 0x46103201
    /* 800ACF30 46145102 */  .word 0x46145102
    /* 800ACF34 27A40074 */  addiu $a0, $sp, 116
    /* 800ACF38 01201025 */  or $v0, $t1, $zero
    /* 800ACF3C 46144282 */  .word 0x46144282
    /* 800ACF40 46046080 */  .word 0x46046080
    /* 800ACF44 4616B482 */  .word 0x4616B482
    /* 800ACF48 460A8380 */  .word 0x460A8380
    /* 800ACF4C 46021280 */  .word 0x46021280
    /* 800ACF50 4612B102 */  .word 0x4612B102
    /* 800ACF54 46007387 */  .word 0x46007387
    /* 800ACF58 460E1180 */  .word 0x460E1180
    /* 800ACF5C 46062202 */  .word 0x46062202
    /* 800ACF60 460E5100 */  .word 0x460E5100
    /* 800ACF64 46049182 */  .word 0x46049182
    /* 800ACF68 46064281 */  .word 0x46064281
    /* 800ACF6C 4602B102 */  .word 0x4602B102
    /* 800ACF70 C7A60058 */  lwc1 $f6, 88($sp)
    /* 800ACF74 46045200 */  .word 0x46045200
    /* 800ACF78 46064002 */  .word 0x46064002
    /* 800ACF7C 00000000 */  nop
    /* 800ACF80 C452000C */  lwc1 $f18, 12($v0)
    /* 800ACF84 24630004 */  addiu $v1, $v1, 4
    /* 800ACF88 C474FFFC */  lwc1 $f20, -4($v1)
    /* 800ACF8C 46120582 */  .word 0x46120582
    /* 800ACF90 5064000B */  .word 0x5064000B
    /* 800ACF94 4616A481 */  .word 0x4616A481
    /* 800ACF98 4616A481 */  .word 0x4616A481
    /* 800ACF9C C4740000 */  lwc1 $f20, 0($v1)
    /* 800ACFA0 24630004 */  addiu $v1, $v1, 4
    /* 800ACFA4 24420004 */  addiu $v0, $v0, 4
    /* 800ACFA8 E472FFF8 */  swc1 $f18, -8($v1)
    /* 800ACFAC C452000C */  lwc1 $f18, 12($v0)
    /* 800ACFB0 46120582 */  .word 0x46120582
    /* 800ACFB4 5464FFF9 */  .word 0x5464FFF9
    /* 800ACFB8 4616A481 */  .word 0x4616A481
    /* 800ACFBC 4616A481 */  .word 0x4616A481
    /* 800ACFC0 24420004 */  addiu $v0, $v0, 4
    /* 800ACFC4 E472FFFC */  swc1 $f18, -4($v1)
    /* 800ACFC8 27A80068 */  addiu $t0, $sp, 104
    /* 800ACFCC 01203025 */  or $a2, $t1, $zero
    /* 800ACFD0 8FA700E8 */  lw $a3, 232($sp)
    /* 800ACFD4 0C02B2DD */  jal 0x800ACB74
    /* 800ACFD8 8FA500EC */  lw $a1, 236($sp)
    /* 800ACFDC 8FBF002C */  lw $ra, 44($sp)
    /* 800ACFE0 D7B40010 */  .word 0xD7B40010
    /* 800ACFE4 D7B60018 */  .word 0xD7B60018
    /* 800ACFE8 8FB00024 */  lw $s0, 36($sp)
    /* 800ACFEC 8FB10028 */  lw $s1, 40($sp)
    /* 800ACFF0 03E00008 */  jr $ra
    /* 800ACFF4 27BD00E0 */  addiu $sp, $sp, 224
