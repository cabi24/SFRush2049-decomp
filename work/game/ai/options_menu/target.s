# Source: game_code.bin (decompressed)
# Address: 0x800DCDF4

glabel options_menu
    /* 800DCDF4 27BDFFD0 */  addiu $sp, $sp, -48
    /* 800DCDF8 AFBF0024 */  sw $ra, 36($sp)
    /* 800DCDFC 0C02DD28 */  jal 0x800B74A0
    /* 800DCE00 24040001 */  addiu $a0, $zero, 1
    /* 800DCE04 3C148014 */  lui $s4, 0x8014
    /* 800DCE08 269461D0 */  addiu $s4, $s4, 25040
    /* 800DCE0C 02802025 */  or $a0, $s4, $zero
    /* 800DCE10 00002825 */  or $a1, $zero, $zero
    /* 800DCE14 0C001C9C */  jal 0x80007270
    /* 800DCE18 24060001 */  addiu $a2, $zero, 1
    /* 800DCE1C 0C02D080 */  jal 0x800B4200
    /* 800DCE20 2412000B */  addiu $s2, $zero, 11
    /* 800DCE24 00408025 */  or $s0, $v0, $zero
    /* 800DCE28 02802025 */  or $a0, $s4, $zero
    /* 800DCE2C 00002825 */  or $a1, $zero, $zero
    /* 800DCE30 0C001D78 */  jal 0x800075E0
    /* 800DCE34 00003025 */  or $a2, $zero, $zero
    /* 800DCE38 00167080 */  sll $t6, $s6, 2
    /* 800DCE3C 00177880 */  sll $t7, $s7, 2
    /* 800DCE40 01F77823 */  subu $t7, $t7, $s7
    /* 800DCE44 01D67023 */  subu $t6, $t6, $s6
    /* 800DCE48 000E7080 */  sll $t6, $t6, 2
    /* 800DCE4C 000F7880 */  sll $t7, $t7, 2
    /* 800DCE50 01F77823 */  subu $t7, $t7, $s7
    /* 800DCE54 01D67023 */  subu $t6, $t6, $s6
    /* 800DCE58 000E70C0 */  sll $t6, $t6, 3
    /* 800DCE5C 000F7880 */  sll $t7, $t7, 2
    /* 800DCE60 3C198015 */  lui $t9, 0x8015
    /* 800DCE64 02001025 */  or $v0, $s0, $zero
    /* 800DCE68 27393FD8 */  addiu $t9, $t9, 16344
    /* 800DCE6C 01CFC021 */  addu $t8, $t6, $t7
    /* 800DCE70 03198021 */  addu $s0, $t8, $t9
    /* 800DCE74 8606001C */  lh $a2, 28($s0)
    /* 800DCE78 860F0018 */  lh $t7, 24($s0)
    /* 800DCE7C 8602001E */  lh $v0, 30($s0)
    /* 800DCE80 3C088012 */  lui $t0, 0x8012
    /* 800DCE84 25088E20 */  addiu $t0, $t0, -29152
    /* 800DCE88 2442FFD8 */  addiu $v0, $v0, -40
    /* 800DCE8C 00023C00 */  sll $a3, $v0, 16
    /* 800DCE90 24030001 */  addiu $v1, $zero, 1
    /* 800DCE94 AD030000 */  sw $v1, 0($t0)
    /* 800DCE98 AD030004 */  sw $v1, 4($t0)
    /* 800DCE9C 04C10003 */  bgez $a2, 0x800DCEAC
    /* 800DCEA0 00067043 */  sra $t6, $a2, 1
    /* 800DCEA4 24C10001 */  addiu $at, $a2, 1
    /* 800DCEA8 00017043 */  sra $t6, $at, 1
    /* 800DCEAC 01CF2021 */  addu $a0, $t6, $t7
    /* 800DCEB0 860F001A */  lh $t7, 26($s0)
    /* 800DCEB4 0004C400 */  sll $t8, $a0, 16
    /* 800DCEB8 0018CC03 */  sra $t9, $t8, 16
    /* 800DCEBC 03202025 */  or $a0, $t9, $zero
    /* 800DCEC0 04410003 */  bgez $v0, 0x800DCED0
    /* 800DCEC4 00027043 */  sra $t6, $v0, 1
    /* 800DCEC8 24410001 */  addiu $at, $v0, 1
    /* 800DCECC 00017043 */  sra $t6, $at, 1
    /* 800DCED0 01CF2821 */  addu $a1, $t6, $t7
    /* 800DCED4 24A50014 */  addiu $a1, $a1, 20
    /* 800DCED8 0005C400 */  sll $t8, $a1, 16
    /* 800DCEDC 240FFFFF */  addiu $t7, $zero, -1
    /* 800DCEE0 00077403 */  sra $t6, $a3, 16
    /* 800DCEE4 01C03825 */  or $a3, $t6, $zero
    /* 800DCEE8 AFAF0010 */  sw $t7, 16($sp)
    /* 800DCEEC 00182C03 */  sra $a1, $t8, 16
    /* 800DCEF0 AFA00014 */  sw $zero, 20($sp)
    /* 800DCEF4 0C02F81E */  jal 0x800BE078
    /* 800DCEF8 AFBE0018 */  sw $fp, 24($sp)
    /* 800DCEFC 0C02CFD4 */  jal 0x800B3F50
    /* 800DCF00 00000000 */  nop
    /* 800DCF04 8618001C */  lh $t8, 28($s0)
    /* 800DCF08 860E0018 */  lh $t6, 24($s0)
    /* 800DCF0C 860F001E */  lh $t7, 30($s0)
    /* 800DCF10 0040B825 */  or $s7, $v0, $zero
    /* 800DCF14 07010003 */  bgez $t8, 0x800DCF24
    /* 800DCF18 0018C843 */  sra $t9, $t8, 1
    /* 800DCF1C 27010001 */  addiu $at, $t8, 1
    /* 800DCF20 0001C843 */  sra $t9, $at, 1
    /* 800DCF24 8618001A */  lh $t8, 26($s0)
    /* 800DCF28 032EF021 */  addu $fp, $t9, $t6
    /* 800DCF2C 00027080 */  sll $t6, $v0, 2
    /* 800DCF30 01C27023 */  subu $t6, $t6, $v0
    /* 800DCF34 01F8C821 */  addu $t9, $t7, $t8
    /* 800DCF38 12A00003 */  beq $s5, $zero, 0x800DCF48
    /* 800DCF3C 032E9823 */  subu $s3, $t9, $t6
    /* 800DCF40 10000002 */  beq $zero, $zero, 0x800DCF4C
    /* 800DCF44 2412000B */  addiu $s2, $zero, 11
    /* 800DCF48 2412000A */  addiu $s2, $zero, 10
    /* 800DCF4C 02802025 */  or $a0, $s4, $zero
    /* 800DCF50 00002825 */  or $a1, $zero, $zero
    /* 800DCF54 0C001C9C */  jal 0x80007270
    /* 800DCF58 24060001 */  addiu $a2, $zero, 1
    /* 800DCF5C 0C02D080 */  jal 0x800B4200
    /* 800DCF60 AFB30028 */  sw $s3, 40($sp)
    /* 800DCF64 8FB30028 */  lw $s3, 40($sp)
    /* 800DCF68 00408025 */  or $s0, $v0, $zero
    /* 800DCF6C 02802025 */  or $a0, $s4, $zero
    /* 800DCF70 00002825 */  or $a1, $zero, $zero
    /* 800DCF74 0C001D78 */  jal 0x800075E0
    /* 800DCF78 00003025 */  or $a2, $zero, $zero
    /* 800DCF7C 52A00004 */  .word 0x52A00004
    /* 800DCF80 24120016 */  addiu $s2, $zero, 22
    /* 800DCF84 10000002 */  beq $zero, $zero, 0x800DCF90
    /* 800DCF88 24120001 */  addiu $s2, $zero, 1
    /* 800DCF8C 24120016 */  addiu $s2, $zero, 22
    /* 800DCF90 0C02DD28 */  jal 0x800B74A0
    /* 800DCF94 02402025 */  or $a0, $s2, $zero
    /* 800DCF98 12A00003 */  beq $s5, $zero, 0x800DCFA8
    /* 800DCF9C 3C118018 */  lui $s1, 0x8018
    /* 800DCFA0 10000002 */  beq $zero, $zero, 0x800DCFAC
    /* 800DCFA4 00009025 */  or $s2, $zero, $zero
    /* 800DCFA8 24120001 */  addiu $s2, $zero, 1
    /* 800DCFAC 001EB400 */  sll $s6, $fp, 16
    /* 800DCFB0 00167C03 */  sra $t7, $s6, 16
    /* 800DCFB4 000F2400 */  sll $a0, $t7, 16
    /* 800DCFB8 0004C403 */  sra $t8, $a0, 16
    /* 800DCFBC 03002025 */  or $a0, $t8, $zero
    /* 800DCFC0 2631A4E0 */  addiu $s1, $s1, -23328
    /* 800DCFC4 0272C823 */  subu $t9, $s3, $s2
    /* 800DCFC8 8E380004 */  lw $t8, 4($s1)
    /* 800DCFCC 03372821 */  addu $a1, $t9, $s7
    /* 800DCFD0 00057400 */  sll $t6, $a1, 16
    /* 800DCFD4 000E2C03 */  sra $a1, $t6, 16
    /* 800DCFD8 01E0B025 */  or $s6, $t7, $zero
    /* 800DCFDC 0C02DC75 */  jal 0x800B71D4
    /* 800DCFE0 8F0603B0 */  lw $a2, 944($t8)
    /* 800DCFE4 12A00003 */  beq $s5, $zero, 0x800DCFF4
    /* 800DCFE8 02802025 */  or $a0, $s4, $zero
    /* 800DCFEC 10000002 */  beq $zero, $zero, 0x800DCFF8
    /* 800DCFF0 2412000A */  addiu $s2, $zero, 10
    /* 800DCFF4 2412000B */  addiu $s2, $zero, 11
    /* 800DCFF8 00002825 */  or $a1, $zero, $zero
    /* 800DCFFC 0C001C9C */  jal 0x80007270
    /* 800DD000 24060001 */  addiu $a2, $zero, 1
    /* 800DD004 0C02D080 */  jal 0x800B4200
    /* 800DD008 AFB30028 */  sw $s3, 40($sp)
    /* 800DD00C 3C118018 */  lui $s1, 0x8018
    /* 800DD010 2631A4E0 */  addiu $s1, $s1, -23328
    /* 800DD014 8FB30028 */  lw $s3, 40($sp)
    /* 800DD018 00408025 */  or $s0, $v0, $zero
    /* 800DD01C 02802025 */  or $a0, $s4, $zero
    /* 800DD020 00002825 */  or $a1, $zero, $zero
    /* 800DD024 0C001D78 */  jal 0x800075E0
    /* 800DD028 00003025 */  or $a2, $zero, $zero
    /* 800DD02C 12A00003 */  beq $s5, $zero, 0x800DD03C
    /* 800DD030 24120001 */  addiu $s2, $zero, 1
    /* 800DD034 10000001 */  beq $zero, $zero, 0x800DD03C
    /* 800DD038 24120016 */  addiu $s2, $zero, 22
    /* 800DD03C 0C02DD28 */  jal 0x800B74A0
    /* 800DD040 02402025 */  or $a0, $s2, $zero
    /* 800DD044 12A00003 */  beq $s5, $zero, 0x800DD054
    /* 800DD048 00162400 */  sll $a0, $s6, 16
    /* 800DD04C 10000002 */  beq $zero, $zero, 0x800DD058
    /* 800DD050 24120001 */  addiu $s2, $zero, 1
    /* 800DD054 00009025 */  or $s2, $zero, $zero
    /* 800DD058 0004CC03 */  sra $t9, $a0, 16
    /* 800DD05C 03202025 */  or $a0, $t9, $zero
    /* 800DD060 8E39000C */  lw $t9, 12($s1)
    /* 800DD064 02722823 */  subu $a1, $s3, $s2
    /* 800DD068 00057400 */  sll $t6, $a1, 16
    /* 800DD06C 000E2C03 */  sra $a1, $t6, 16
    /* 800DD070 972E0032 */  lhu $t6, 50($t9)
    /* 800DD074 8E380010 */  lw $t8, 16($s1)
    /* 800DD078 000E7880 */  sll $t7, $t6, 2
    /* 800DD07C 030FC821 */  addu $t9, $t8, $t7
    /* 800DD080 0C02DC75 */  jal 0x800B71D4
    /* 800DD084 8F260000 */  lw $a2, 0($t9)
    /* 800DD088 8FBF0024 */  lw $ra, 36($sp)
    /* 800DD08C 3C028012 */  lui $v0, 0x8012
    /* 800DD090 24428E20 */  addiu $v0, $v0, -29152
    /* 800DD094 240E0003 */  addiu $t6, $zero, 3
    /* 800DD098 AC4E0004 */  sw $t6, 4($v0)
    /* 800DD09C AC400000 */  sw $zero, 0($v0)
    /* 800DD0A0 03E00008 */  jr $ra
    /* 800DD0A4 27BD0030 */  addiu $sp, $sp, 48
