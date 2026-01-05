# Source: game_code.bin (decompressed)
# Address: 0x800ABCC8

glabel transmission_ratio_get
    /* 800ABCC8 27BDFF60 */  addiu $sp, $sp, -160
    /* 800ABCCC AFBF0034 */  sw $ra, 52($sp)
    /* 800ABCD0 AFB40030 */  sw $s4, 48($sp)
    /* 800ABCD4 AFB3002C */  sw $s3, 44($sp)
    /* 800ABCD8 AFB20028 */  sw $s2, 40($sp)
    /* 800ABCDC AFB10024 */  sw $s1, 36($sp)
    /* 800ABCE0 AFB00020 */  sw $s0, 32($sp)
    /* 800ABCE4 F7B40018 */  .word 0xF7B40018
    /* 800ABCE8 AFA400A0 */  sw $a0, 160($sp)
    /* 800ABCEC AFA500A4 */  sw $a1, 164($sp)
    /* 800ABCF0 AFA600A8 */  sw $a2, 168($sp)
    /* 800ABCF4 10E00004 */  beq $a3, $zero, 0x800ABD08
    /* 800ABCF8 AFA700AC */  sw $a3, 172($sp)
    /* 800ABCFC 8C980040 */  lw $t8, 64($a0)
    /* 800ABD00 10000002 */  beq $zero, $zero, 0x800ABD0C
    /* 800ABD04 AFB80084 */  sw $t8, 132($sp)
    /* 800ABD08 AFA00084 */  sw $zero, 132($sp)
    /* 800ABD0C 3C108011 */  lui $s0, 0x8011
    /* 800ABD10 26107530 */  addiu $s0, $s0, 30000
    /* 800ABD14 00001825 */  or $v1, $zero, $zero
    /* 800ABD18 8E040000 */  lw $a0, 0($s0)
    /* 800ABD1C 0C001F10 */  jal 0x80007C40
    /* 800ABD20 AFA3009C */  sw $v1, 156($sp)
    /* 800ABD24 8FA400A0 */  lw $a0, 160($sp)
    /* 800ABD28 8E050000 */  lw $a1, 0($s0)
    /* 800ABD2C 0C02542B */  jal 0x800950AC
    /* 800ABD30 00403025 */  or $a2, $v0, $zero
    /* 800ABD34 1440019A */  bne $v0, $zero, 0x800AC3A0
    /* 800ABD38 8FA3009C */  lw $v1, 156($sp)
    /* 800ABD3C 3C198011 */  lui $t9, 0x8011
    /* 800ABD40 8F3974B4 */  lw $t9, 29876($t9)
    /* 800ABD44 3C0F8015 */  lui $t7, 0x8015
    /* 800ABD48 3C188015 */  lui $t8, 0x8015
    /* 800ABD4C 332E0008 */  andi $t6, $t9, 0x0008
    /* 800ABD50 11C00006 */  beq $t6, $zero, 0x800ABD6C
    /* 800ABD54 00000000 */  nop
    /* 800ABD58 81EF6994 */  lb $t7, 27028($t7)
    /* 800ABD5C 15E00003 */  bne $t7, $zero, 0x800ABD6C
    /* 800ABD60 00000000 */  nop
    /* 800ABD64 10000193 */  beq $zero, $zero, 0x800AC3B4
    /* 800ABD68 24020001 */  addiu $v0, $zero, 1
    /* 800ABD6C 8F18A110 */  lw $t8, -24304($t8)
    /* 800ABD70 24010002 */  addiu $at, $zero, 2
    /* 800ABD74 3C0F8015 */  lui $t7, 0x8015
    /* 800ABD78 1701000A */  bne $t8, $at, 0x800ABDA4
    /* 800ABD7C 00000000 */  nop
    /* 800ABD80 86190010 */  lh $t9, 16($s0)
    /* 800ABD84 24010004 */  addiu $at, $zero, 4
    /* 800ABD88 8FAE00B8 */  lw $t6, 184($sp)
    /* 800ABD8C 17210005 */  bne $t9, $at, 0x800ABDA4
    /* 800ABD90 00000000 */  nop
    /* 800ABD94 11C00003 */  beq $t6, $zero, 0x800ABDA4
    /* 800ABD98 00000000 */  nop
    /* 800ABD9C 10000185 */  beq $zero, $zero, 0x800AC3B4
    /* 800ABDA0 24020001 */  addiu $v0, $zero, 1
    /* 800ABDA4 81EF6994 */  lb $t7, 27028($t7)
    /* 800ABDA8 55E00008 */  .word 0x55E00008
    /* 800ABDAC 960E0012 */  lhu $t6, 18($s0)
    /* 800ABDB0 96180012 */  lhu $t8, 18($s0)
    /* 800ABDB4 33190020 */  andi $t9, $t8, 0x0020
    /* 800ABDB8 57200004 */  .word 0x57200004
    /* 800ABDBC 960E0012 */  lhu $t6, 18($s0)
    /* 800ABDC0 1000017C */  beq $zero, $zero, 0x800AC3B4
    /* 800ABDC4 24020001 */  addiu $v0, $zero, 1
    /* 800ABDC8 960E0012 */  lhu $t6, 18($s0)
    /* 800ABDCC 2401FFFB */  addiu $at, $zero, -5
    /* 800ABDD0 2406FFFF */  addiu $a2, $zero, -1
    /* 800ABDD4 01C17824 */  and $t7, $t6, $at
    /* 800ABDD8 15E00003 */  bne $t7, $zero, 0x800ABDE8
    /* 800ABDDC 8FA400A0 */  lw $a0, 160($sp)
    /* 800ABDE0 10000174 */  beq $zero, $zero, 0x800AC3B4
    /* 800ABDE4 24020001 */  addiu $v0, $zero, 1
    /* 800ABDE8 3C058012 */  lui $a1, 0x8012
    /* 800ABDEC 24A51D40 */  addiu $a1, $a1, 7488
    /* 800ABDF0 AFA3009C */  sw $v1, 156($sp)
    /* 800ABDF4 0C029193 */  jal 0x800A464C
    /* 800ABDF8 A3A6007B */  sb $a2, 123($sp)
    /* 800ABDFC 8FA3009C */  lw $v1, 156($sp)
    /* 800ABE00 10400002 */  beq $v0, $zero, 0x800ABE0C
    /* 800ABE04 83A6007B */  lb $a2, 123($sp)
    /* 800ABE08 24060001 */  addiu $a2, $zero, 1
    /* 800ABE0C 3C058012 */  lui $a1, 0x8012
    /* 800ABE10 24A51D44 */  addiu $a1, $a1, 7492
    /* 800ABE14 8FA400A0 */  lw $a0, 160($sp)
    /* 800ABE18 AFA3009C */  sw $v1, 156($sp)
    /* 800ABE1C 0C029193 */  jal 0x800A464C
    /* 800ABE20 A3A6007B */  sb $a2, 123($sp)
    /* 800ABE24 8FA3009C */  lw $v1, 156($sp)
    /* 800ABE28 10400002 */  beq $v0, $zero, 0x800ABE34
    /* 800ABE2C 83A6007B */  lb $a2, 123($sp)
    /* 800ABE30 00003025 */  or $a2, $zero, $zero
    /* 800ABE34 3C028015 */  lui $v0, 0x8015
    /* 800ABE38 80422570 */  lb $v0, 9584($v0)
    /* 800ABE3C 10400003 */  beq $v0, $zero, 0x800ABE4C
    /* 800ABE40 00000000 */  nop
    /* 800ABE44 10C00005 */  beq $a2, $zero, 0x800ABE5C
    /* 800ABE48 00000000 */  nop
    /* 800ABE4C 14400005 */  bne $v0, $zero, 0x800ABE64
    /* 800ABE50 24010001 */  addiu $at, $zero, 1
    /* 800ABE54 14C10003 */  bne $a2, $at, 0x800ABE64
    /* 800ABE58 00000000 */  nop
    /* 800ABE5C 10000155 */  beq $zero, $zero, 0x800AC3B4
    /* 800ABE60 24020001 */  addiu $v0, $zero, 1
    /* 800ABE64 3C048014 */  lui $a0, 0x8014
    /* 800ABE68 24843FC8 */  addiu $a0, $a0, 16328
    /* 800ABE6C 0C0238F0 */  jal 0x8008E3C0
    /* 800ABE70 AFA3009C */  sw $v1, 156($sp)
    /* 800ABE74 8FA3009C */  lw $v1, 156($sp)
    /* 800ABE78 14400003 */  bne $v0, $zero, 0x800ABE88
    /* 800ABE7C 00404025 */  or $t0, $v0, $zero
    /* 800ABE80 1000014C */  beq $zero, $zero, 0x800AC3B4
    /* 800ABE84 24020001 */  addiu $v0, $zero, 1
    /* 800ABE88 96090012 */  lhu $t1, 18($s0)
    /* 800ABE8C 00002825 */  or $a1, $zero, $zero
    /* 800ABE90 24070001 */  addiu $a3, $zero, 1
    /* 800ABE94 31380008 */  andi $t8, $t1, 0x0008
    /* 800ABE98 53000009 */  .word 0x53000009
    /* 800ABE9C 31398000 */  andi $t9, $t1, 0x8000
    /* 800ABEA0 82190016 */  lb $t9, 22($s0)
    /* 800ABEA4 3C0E8015 */  lui $t6, 0x8015
    /* 800ABEA8 25CE0E28 */  addiu $t6, $t6, 3624
    /* 800ABEAC 032E1021 */  addu $v0, $t9, $t6
    /* 800ABEB0 804F0000 */  lb $t7, 0($v0)
    /* 800ABEB4 25F80001 */  addiu $t8, $t7, 1
    /* 800ABEB8 A0580000 */  sb $t8, 0($v0)
    /* 800ABEBC 31398000 */  andi $t9, $t1, 0x8000
    /* 800ABEC0 13200006 */  beq $t9, $zero, 0x800ABEDC
    /* 800ABEC4 3C188014 */  lui $t8, 0x8014
    /* 800ABEC8 3C028015 */  lui $v0, 0x8015
    /* 800ABECC 24420F78 */  addiu $v0, $v0, 3960
    /* 800ABED0 8C4E0000 */  lw $t6, 0($v0)
    /* 800ABED4 25CF0001 */  addiu $t7, $t6, 1
    /* 800ABED8 AC4F0000 */  sw $t7, 0($v0)
    /* 800ABEDC 27180BDC */  addiu $t8, $t8, 3036
    /* 800ABEE0 93060000 */  lbu $a2, 0($t8)
    /* 800ABEE4 8E040004 */  lw $a0, 4($s0)
    /* 800ABEE8 AFA90040 */  sw $t1, 64($sp)
    /* 800ABEEC 24C6FFFF */  addiu $a2, $a2, -1
    /* 800ABEF0 0006CE00 */  sll $t9, $a2, 24
    /* 800ABEF4 00193603 */  sra $a2, $t9, 24
    /* 800ABEF8 AFA80088 */  sw $t0, 136($sp)
    /* 800ABEFC 0C024B8B */  jal 0x80092E2C
    /* 800ABF00 AFA3009C */  sw $v1, 156($sp)
    /* 800ABF04 8FA80088 */  lw $t0, 136($sp)
    /* 800ABF08 8FA3009C */  lw $v1, 156($sp)
    /* 800ABF0C 8FA90040 */  lw $t1, 64($sp)
    /* 800ABF10 AD020008 */  sw $v0, 8($t0)
    /* 800ABF14 A5030010 */  sh $v1, 16($t0)
    /* 800ABF18 86040014 */  lh $a0, 20($s0)
    /* 800ABF1C 2405FFFF */  addiu $a1, $zero, -1
    /* 800ABF20 A105005C */  sb $a1, 92($t0)
    /* 800ABF24 A5040050 */  sh $a0, 80($t0)
    /* 800ABF28 A5040058 */  sh $a0, 88($t0)
    /* 800ABF2C 83AF00B3 */  lb $t7, 179($sp)
    /* 800ABF30 44802000 */  .word 0x44802000
    /* 800ABF34 31380400 */  andi $t8, $t1, 0x0400
    /* 800ABF38 A500005A */  sh $zero, 90($t0)
    /* 800ABF3C AD050060 */  sw $a1, 96($t0)
    /* 800ABF40 A10F0065 */  sb $t7, 101($t0)
    /* 800ABF44 13000003 */  beq $t8, $zero, 0x800ABF54
    /* 800ABF48 E5040068 */  swc1 $f4, 104($t0)
    /* 800ABF4C 8FB900A0 */  lw $t9, 160($sp)
    /* 800ABF50 AD19006C */  sw $t9, 108($t0)
    /* 800ABF54 312E0080 */  andi $t6, $t1, 0x0080
    /* 800ABF58 11C00003 */  beq $t6, $zero, 0x800ABF68
    /* 800ABF5C 8FAF0084 */  lw $t7, 132($sp)
    /* 800ABF60 35F88000 */  ori $t8, $t7, 0x8000
    /* 800ABF64 AFB80084 */  sw $t8, 132($sp)
    /* 800ABF68 96190012 */  lhu $t9, 18($s0)
    /* 800ABF6C 8FAF0084 */  lw $t7, 132($sp)
    /* 800ABF70 3C010040 */  lui $at, 0x0040
    /* 800ABF74 332E0040 */  andi $t6, $t9, 0x0040
    /* 800ABF78 11C00003 */  beq $t6, $zero, 0x800ABF88
    /* 800ABF7C 31390004 */  andi $t9, $t1, 0x0004
    /* 800ABF80 01E1C025 */  or $t8, $t7, $at
    /* 800ABF84 AFB80084 */  sw $t8, 132($sp)
    /* 800ABF88 1720000A */  bne $t9, $zero, 0x800ABFB4
    /* 800ABF8C 3C058012 */  lui $a1, 0x8012
    /* 800ABF90 820E0016 */  lb $t6, 22($s0)
    /* 800ABF94 24010005 */  addiu $at, $zero, 5
    /* 800ABF98 240F0002 */  addiu $t7, $zero, 2
    /* 800ABF9C 15C10003 */  bne $t6, $at, 0x800ABFAC
    /* 800ABFA0 00000000 */  nop
    /* 800ABFA4 10000004 */  beq $zero, $zero, 0x800ABFB8
    /* 800ABFA8 A1000004 */  sb $zero, 4($t0)
    /* 800ABFAC 10000002 */  beq $zero, $zero, 0x800ABFB8
    /* 800ABFB0 A10F0004 */  sb $t7, 4($t0)
    /* 800ABFB4 A1000004 */  sb $zero, 4($t0)
    /* 800ABFB8 82020016 */  lb $v0, 22($s0)
    /* 800ABFBC 24010004 */  addiu $at, $zero, 4
    /* 800ABFC0 8FB800AC */  lw $t8, 172($sp)
    /* 800ABFC4 14410007 */  bne $v0, $at, 0x800ABFE4
    /* 800ABFC8 24A51D48 */  addiu $a1, $a1, 7496
    /* 800ABFCC 53000006 */  .word 0x53000006
    /* 800ABFD0 24010006 */  addiu $at, $zero, 6
    /* 800ABFD4 91190004 */  lbu $t9, 4($t0)
    /* 800ABFD8 372E0010 */  ori $t6, $t9, 0x0010
    /* 800ABFDC A10E0004 */  sb $t6, 4($t0)
    /* 800ABFE0 82020016 */  lb $v0, 22($s0)
    /* 800ABFE4 24010006 */  addiu $at, $zero, 6
    /* 800ABFE8 14410015 */  bne $v0, $at, 0x800AC040
    /* 800ABFEC 8FA400A0 */  lw $a0, 160($sp)
    /* 800ABFF0 AFA80088 */  sw $t0, 136($sp)
    /* 800ABFF4 0C029193 */  jal 0x800A464C
    /* 800ABFF8 AFA90040 */  sw $t1, 64($sp)
    /* 800ABFFC 8FA80088 */  lw $t0, 136($sp)
    /* 800AC000 1040000A */  beq $v0, $zero, 0x800AC02C
    /* 800AC004 8FA90040 */  lw $t1, 64($sp)
    /* 800AC008 910F0004 */  lbu $t7, 4($t0)
    /* 800AC00C 3C028015 */  lui $v0, 0x8015
    /* 800AC010 244215F0 */  addiu $v0, $v0, 5616
    /* 800AC014 35F80040 */  ori $t8, $t7, 0x0040
    /* 800AC018 A1180004 */  sb $t8, 4($t0)
    /* 800AC01C 8C590000 */  lw $t9, 0($v0)
    /* 800AC020 272E0001 */  addiu $t6, $t9, 1
    /* 800AC024 10000006 */  beq $zero, $zero, 0x800AC040
    /* 800AC028 AC4E0000 */  sw $t6, 0($v0)
    /* 800AC02C 3C028015 */  lui $v0, 0x8015
    /* 800AC030 24421688 */  addiu $v0, $v0, 5768
    /* 800AC034 8C4F0000 */  lw $t7, 0($v0)
    /* 800AC038 25F80001 */  addiu $t8, $t7, 1
    /* 800AC03C AC580000 */  sw $t8, 0($v0)
    /* 800AC040 8FB900B4 */  lw $t9, 180($sp)
    /* 800AC044 8FAE00AC */  lw $t6, 172($sp)
    /* 800AC048 5320009B */  .word 0x5320009B
    /* 800AC04C 3C01BF80 */  lui $at, 0xBF80
    /* 800AC050 11C00013 */  beq $t6, $zero, 0x800AC0A0
    /* 800AC054 8FAF00A0 */  lw $t7, 160($sp)
    /* 800AC058 C5E60034 */  lwc1 $f6, 52($t7)
    /* 800AC05C 25050014 */  addiu $a1, $t0, 20
    /* 800AC060 E5060038 */  swc1 $f6, 56($t0)
    /* 800AC064 8FB800A0 */  lw $t8, 160($sp)
    /* 800AC068 C7080038 */  lwc1 $f8, 56($t8)
    /* 800AC06C E508003C */  swc1 $f8, 60($t0)
    /* 800AC070 8FB900A0 */  lw $t9, 160($sp)
    /* 800AC074 C72A003C */  lwc1 $f10, 60($t9)
    /* 800AC078 E50A0040 */  swc1 $f10, 64($t0)
    /* 800AC07C 8FA400A0 */  lw $a0, 160($sp)
    /* 800AC080 AFA90040 */  sw $t1, 64($sp)
    /* 800AC084 AFA80088 */  sw $t0, 136($sp)
    /* 800AC088 AFA5003C */  sw $a1, 60($sp)
    /* 800AC08C 0C0235AC */  jal 0x8008D6B0
    /* 800AC090 24840010 */  addiu $a0, $a0, 16
    /* 800AC094 8FA80088 */  lw $t0, 136($sp)
    /* 800AC098 10000013 */  beq $zero, $zero, 0x800AC0E8
    /* 800AC09C 8FA90040 */  lw $t1, 64($sp)
    /* 800AC0A0 8FAE00A8 */  lw $t6, 168($sp)
    /* 800AC0A4 3C048011 */  lui $a0, 0x8011
    /* 800AC0A8 25050014 */  addiu $a1, $t0, 20
    /* 800AC0AC C5C40000 */  lwc1 $f4, 0($t6)
    /* 800AC0B0 2484418C */  addiu $a0, $a0, 16780
    /* 800AC0B4 E5040038 */  swc1 $f4, 56($t0)
    /* 800AC0B8 8FAF00A8 */  lw $t7, 168($sp)
    /* 800AC0BC C5E60004 */  lwc1 $f6, 4($t7)
    /* 800AC0C0 E506003C */  swc1 $f6, 60($t0)
    /* 800AC0C4 8FB800A8 */  lw $t8, 168($sp)
    /* 800AC0C8 C7080008 */  lwc1 $f8, 8($t8)
    /* 800AC0CC E5080040 */  swc1 $f8, 64($t0)
    /* 800AC0D0 AFA90040 */  sw $t1, 64($sp)
    /* 800AC0D4 AFA80088 */  sw $t0, 136($sp)
    /* 800AC0D8 0C0235AC */  jal 0x8008D6B0
    /* 800AC0DC AFA5003C */  sw $a1, 60($sp)
    /* 800AC0E0 8FA80088 */  lw $t0, 136($sp)
    /* 800AC0E4 8FA90040 */  lw $t1, 64($sp)
    /* 800AC0E8 31390800 */  andi $t9, $t1, 0x0800
    /* 800AC0EC 13200007 */  beq $t9, $zero, 0x800AC10C
    /* 800AC0F0 87B800A6 */  lh $t8, 166($sp)
    /* 800AC0F4 8FAE0084 */  lw $t6, 132($sp)
    /* 800AC0F8 3C010004 */  lui $at, 0x0004
    /* 800AC0FC 2403FFFF */  addiu $v1, $zero, -1
    /* 800AC100 01C17825 */  or $t7, $t6, $at
    /* 800AC104 1000000C */  beq $zero, $zero, 0x800AC138
    /* 800AC108 AFAF0084 */  sw $t7, 132($sp)
    /* 800AC10C 2401FFFE */  addiu $at, $zero, -2
    /* 800AC110 17010009 */  bne $t8, $at, 0x800AC138
    /* 800AC114 87A300A6 */  lh $v1, 166($sp)
    /* 800AC118 8FA400A8 */  lw $a0, 168($sp)
    /* 800AC11C AFA80088 */  sw $t0, 136($sp)
    /* 800AC120 0C02AD4F */  jal 0x800AB53C
    /* 800AC124 AFA90040 */  sw $t1, 64($sp)
    /* 800AC128 8FA80088 */  lw $t0, 136($sp)
    /* 800AC12C 8FA90040 */  lw $t1, 64($sp)
    /* 800AC130 10000001 */  beq $zero, $zero, 0x800AC138
    /* 800AC134 00401825 */  or $v1, $v0, $zero
    /* 800AC138 82190016 */  lb $t9, 22($s0)
    /* 800AC13C 24010006 */  addiu $at, $zero, 6
    /* 800AC140 3C048011 */  lui $a0, 0x8011
    /* 800AC144 1721000F */  bne $t9, $at, 0x800AC184
    /* 800AC148 2484418C */  addiu $a0, $a0, 16780
    /* 800AC14C 2403FFFF */  addiu $v1, $zero, -1
    /* 800AC150 AFA30090 */  sw $v1, 144($sp)
    /* 800AC154 8FA5003C */  lw $a1, 60($sp)
    /* 800AC158 AFA80088 */  sw $t0, 136($sp)
    /* 800AC15C 0C0235AC */  jal 0x8008D6B0
    /* 800AC160 AFA90040 */  sw $t1, 64($sp)
    /* 800AC164 8FAE0084 */  lw $t6, 132($sp)
    /* 800AC168 3C010005 */  lui $at, 0x0005
    /* 800AC16C 34212000 */  ori $at, $at, 0x2000
    /* 800AC170 01C17825 */  or $t7, $t6, $at
    /* 800AC174 8FA30090 */  lw $v1, 144($sp)
    /* 800AC178 8FA80088 */  lw $t0, 136($sp)
    /* 800AC17C 8FA90040 */  lw $t1, 64($sp)
    /* 800AC180 AFAF0084 */  sw $t7, 132($sp)
    /* 800AC184 3C188015 */  lui $t8, 0x8015
    /* 800AC188 8718A108 */  lh $t8, -24312($t8)
    /* 800AC18C 31390100 */  andi $t9, $t1, 0x0100
    /* 800AC190 8FA5003C */  lw $a1, 60($sp)
    /* 800AC194 2B010002 */  slti $at, $t8, 2
    /* 800AC198 14200003 */  bne $at, $zero, 0x800AC1A8
    /* 800AC19C 00033400 */  sll $a2, $v1, 16
    /* 800AC1A0 5720000E */  .word 0x5720000E
    /* 800AC1A4 312F0300 */  andi $t7, $t1, 0x0300
    /* 800AC1A8 8D040008 */  lw $a0, 8($t0)
    /* 800AC1AC 00067403 */  sra $t6, $a2, 16
    /* 800AC1B0 01C03025 */  or $a2, $t6, $zero
    /* 800AC1B4 AFA90040 */  sw $t1, 64($sp)
    /* 800AC1B8 AFA80088 */  sw $t0, 136($sp)
    /* 800AC1BC AFA30090 */  sw $v1, 144($sp)
    /* 800AC1C0 0C02389B */  jal 0x8008E26C
    /* 800AC1C4 8FA70084 */  lw $a3, 132($sp)
    /* 800AC1C8 8FA80088 */  lw $t0, 136($sp)
    /* 800AC1CC 8FA30090 */  lw $v1, 144($sp)
    /* 800AC1D0 8FA90040 */  lw $t1, 64($sp)
    /* 800AC1D4 AD02000C */  sw $v0, 12($t0)
    /* 800AC1D8 312F0300 */  andi $t7, $t1, 0x0300
    /* 800AC1DC 51E00015 */  .word 0x51E00015
    /* 800AC1E0 82190016 */  lb $t9, 22($s0)
    /* 800AC1E4 04600012 */  bltz $v1, 0x800AC230
    /* 800AC1E8 0003C100 */  sll $t8, $v1, 4
    /* 800AC1EC 0303C021 */  addu $t8, $t8, $v1
    /* 800AC1F0 0018C080 */  sll $t8, $t8, 2
    /* 800AC1F4 3C028013 */  lui $v0, 0x8013
    /* 800AC1F8 00581021 */  addu $v0, $v0, $t8
    /* 800AC1FC 8C42E708 */  lw $v0, -6392($v0)
    /* 800AC200 C50A0038 */  lwc1 $f10, 56($t0)
    /* 800AC204 C508003C */  lwc1 $f8, 60($t0)
    /* 800AC208 C4440024 */  lwc1 $f4, 36($v0)
    /* 800AC20C 46045181 */  .word 0x46045181
    /* 800AC210 E5060038 */  swc1 $f6, 56($t0)
    /* 800AC214 C44A0028 */  lwc1 $f10, 40($v0)
    /* 800AC218 C5060040 */  lwc1 $f6, 64($t0)
    /* 800AC21C 460A4101 */  .word 0x460A4101
    /* 800AC220 E504003C */  swc1 $f4, 60($t0)
    /* 800AC224 C448002C */  lwc1 $f8, 44($v0)
    /* 800AC228 46083281 */  .word 0x46083281
    /* 800AC22C E50A0040 */  swc1 $f10, 64($t0)
    /* 800AC230 82190016 */  lb $t9, 22($s0)
    /* 800AC234 24010004 */  addiu $at, $zero, 4
    /* 800AC238 5721001A */  .word 0x5721001A
    /* 800AC23C 8D05000C */  lw $a1, 12($t0)
    /* 800AC240 8D040008 */  lw $a0, 8($t0)
    /* 800AC244 0C02AED6 */  jal 0x800ABB58
    /* 800AC248 AFA80088 */  sw $t0, 136($sp)
    /* 800AC24C 3C014248 */  lui $at, 0x4248
    /* 800AC250 44812000 */  .word 0x44812000
    /* 800AC254 3C058013 */  lui $a1, 0x8013
    /* 800AC258 24A5E700 */  addiu $a1, $a1, -6400
    /* 800AC25C 4604003C */  .word 0x4604003C
    /* 800AC260 24060044 */  addiu $a2, $zero, 68
    /* 800AC264 8FA80088 */  lw $t0, 136($sp)
    /* 800AC268 4502000E */  .word 0x4502000E
    /* 800AC26C 8D05000C */  lw $a1, 12($t0)
    /* 800AC270 8D04000C */  lw $a0, 12($t0)
    /* 800AC274 00041C00 */  sll $v1, $a0, 16
    /* 800AC278 00037403 */  sra $t6, $v1, 16
    /* 800AC27C 01C60019 */  multu $t6, $a2
    /* 800AC280 00007812 */  mflo $t7
    /* 800AC284 00AFC021 */  addu $t8, $a1, $t7
    /* 800AC288 8F020000 */  lw $v0, 0($t8)
    /* 800AC28C 00860019 */  multu $a0, $a2
    /* 800AC290 34590020 */  ori $t9, $v0, 0x0020
    /* 800AC294 00007012 */  mflo $t6
    /* 800AC298 00AE7821 */  addu $t7, $a1, $t6
    /* 800AC29C ADF90000 */  sw $t9, 0($t7)
    /* 800AC2A0 8D05000C */  lw $a1, 12($t0)
    /* 800AC2A4 AFA80088 */  sw $t0, 136($sp)
    /* 800AC2A8 0C02AEF4 */  jal 0x800ABBD0
    /* 800AC2AC 25040044 */  addiu $a0, $t0, 68
    /* 800AC2B0 8FA80088 */  lw $t0, 136($sp)
    /* 800AC2B4 3C01BF80 */  lui $at, 0xBF80
    /* 800AC2B8 44813000 */  .word 0x44813000
    /* 800AC2BC C6000018 */  lwc1 $f0, 24($s0)
    /* 800AC2C0 46003032 */  .word 0x46003032
    /* 800AC2C4 00000000 */  nop
    /* 800AC2C8 45020008 */  .word 0x45020008
    /* 800AC2CC E5000054 */  swc1 $f0, 84($t0)
    /* 800AC2D0 8D040008 */  lw $a0, 8($t0)
    /* 800AC2D4 0C02AED6 */  jal 0x800ABB58
    /* 800AC2D8 AFA80088 */  sw $t0, 136($sp)
    /* 800AC2DC 8FA80088 */  lw $t0, 136($sp)
    /* 800AC2E0 10000002 */  beq $zero, $zero, 0x800AC2EC
    /* 800AC2E4 E5000054 */  swc1 $f0, 84($t0)
    /* 800AC2E8 E5000054 */  swc1 $f0, 84($t0)
    /* 800AC2EC 86180010 */  lh $t8, 16($s0)
    /* 800AC2F0 24010006 */  addiu $at, $zero, 6
    /* 800AC2F4 53010005 */  .word 0x53010005
    /* 800AC2F8 8FAF00AC */  lw $t7, 172($sp)
    /* 800AC2FC 910E0004 */  lbu $t6, 4($t0)
    /* 800AC300 35D90008 */  ori $t9, $t6, 0x0008
    /* 800AC304 A1190004 */  sb $t9, 4($t0)
    /* 800AC308 8FAF00AC */  lw $t7, 172($sp)
    /* 800AC30C 8FB800A0 */  lw $t8, 160($sp)
    /* 800AC310 51E00005 */  .word 0x51E00005
    /* 800AC314 82190016 */  lb $t9, 22($s0)
    /* 800AC318 8F0E004C */  lw $t6, 76($t8)
    /* 800AC31C 1000000D */  beq $zero, $zero, 0x800AC354
    /* 800AC320 AD0E0008 */  sw $t6, 8($t0)
    /* 800AC324 82190016 */  lb $t9, 22($s0)
    /* 800AC328 83B800B3 */  lb $t8, 179($sp)
    /* 800AC32C 24010004 */  addiu $at, $zero, 4
    /* 800AC330 17210008 */  bne $t9, $at, 0x800AC354
    /* 800AC334 001870C0 */  sll $t6, $t8, 3
    /* 800AC338 3C0F8014 */  lui $t7, 0x8014
    /* 800AC33C 8DEF92D0 */  lw $t7, -27952($t7)
    /* 800AC340 01D87021 */  addu $t6, $t6, $t8
    /* 800AC344 000E7080 */  sll $t6, $t6, 2
    /* 800AC348 01EEC821 */  addu $t9, $t7, $t6
    /* 800AC34C 8F380020 */  lw $t8, 32($t9)
    /* 800AC350 AD180008 */  sw $t8, 8($t0)
    /* 800AC354 3C0F8011 */  lui $t7, 0x8011
    /* 800AC358 8DEF74B4 */  lw $t7, 29876($t7)
    /* 800AC35C 3C198015 */  lui $t9, 0x8015
    /* 800AC360 000F7240 */  sll $t6, $t7, 9
    /* 800AC364 05C1000C */  bgez $t6, 0x800AC398
    /* 800AC368 00000000 */  nop
    /* 800AC36C 8F39A110 */  lw $t9, -24304($t9)
    /* 800AC370 24010006 */  addiu $at, $zero, 6
    /* 800AC374 3C048015 */  lui $a0, 0x8015
    /* 800AC378 17210005 */  bne $t9, $at, 0x800AC390
    /* 800AC37C 24841AA8 */  addiu $a0, $a0, 6824
    /* 800AC380 0C0238F0 */  jal 0x8008E3C0
    /* 800AC384 AFA80088 */  sw $t0, 136($sp)
    /* 800AC388 8FA80088 */  lw $t0, 136($sp)
    /* 800AC38C AC480004 */  sw $t0, 4($v0)
    /* 800AC390 0C02ADF6 */  jal 0x800AB7D8
    /* 800AC394 01008825 */  or $s1, $t0, $zero
    /* 800AC398 10000006 */  beq $zero, $zero, 0x800AC3B4
    /* 800AC39C 24020001 */  addiu $v0, $zero, 1
    /* 800AC3A0 24630001 */  addiu $v1, $v1, 1
    /* 800AC3A4 2401007A */  addiu $at, $zero, 122
    /* 800AC3A8 1461FE5B */  bne $v1, $at, 0x800ABD18
    /* 800AC3AC 26100030 */  addiu $s0, $s0, 48
    /* 800AC3B0 00001025 */  or $v0, $zero, $zero
    /* 800AC3B4 8FBF0034 */  lw $ra, 52($sp)
    /* 800AC3B8 D7B40018 */  .word 0xD7B40018
    /* 800AC3BC 8FB00020 */  lw $s0, 32($sp)
    /* 800AC3C0 8FB10024 */  lw $s1, 36($sp)
    /* 800AC3C4 8FB20028 */  lw $s2, 40($sp)
    /* 800AC3C8 8FB3002C */  lw $s3, 44($sp)
    /* 800AC3CC 8FB40030 */  lw $s4, 48($sp)
    /* 800AC3D0 03E00008 */  jr $ra
    /* 800AC3D4 27BD00A0 */  addiu $sp, $sp, 160
